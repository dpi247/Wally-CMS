<?php

function wallyedit_test(){
  return drupal_get_form('wallyedit_test_form');

}



function wallyedit_test_form(&$form_state){
  $type_name='wally_articlepackage';
  $profile_id = 1;

  $form_state['type'] = $type_name;
  $form_state['pid'] = $profile_id;

  $type=content_types(str_replace('-','_', $type_name));

  $groups=fieldgroup_groups($type_name) ;
  $fields=$type['fields'];

  //remove fields already included in a groups
  foreach($groups as $group){
    foreach($group["fields"] as $key=>$field){
      unset($fields[$key]);
    }
  }

  $onglets = wallyedit_get_onglets($profile_id, $type_name, $groups, $fields);
dsm($onglets);
  $form=array(
    '#tree' => TRUE,
  );

  foreach($onglets as $index_onglet=>$onglet){
    $options_onglet[$index_onglet]=$index_onglet;
  }
  foreach($onglets as $index_onglet=>$onglet){

    foreach($onglet["elements"] as $index_element=>$element){

      $form['level1'][$index_element]['field_name']=array(
        '#type' => 'markup',
        '#value' => $element['field_name'].$element['group_name']
      );
      $form['level1'][$index_element]['weight']=array(
            "#type"=>'weight',
			'#default_value'=>$weight,
      //    "#attributes"=>array('class'=>"my-elements-weight-lev1 my-elements-weight-lev1-". $index_onglet),
      );

       
      if(isset($element['type']) && $element['type']=='nodereference') {
        $form['level1'][$index_element]['wallyedit']=array(
        '#type'=>'select',
        '#options'=>array(0=>t("Exclude"),1=>t("Normal")),
        );
        $hits = 0;
        foreach ($element['referenceable_types'] as $key => $value) {
          if ($key === $value)
          $hits++;
        }
        if ($element['multiple'] == 0 && $hits == 1) {
          $form['level1'][$index_element]["wallyedit"]["#options"][2]=t('Inline');
          $form['level1'][$index_element]["wallyedit"]["#options"][3]=t('Select box');
        } else {
          $form['level1'][$index_element]["wallyedit"]["#options"][3]=t('Select box');
        }
      }
      if(isset($element['display_settings']['wallyedit'])) {
        $form['level1'][$index_element]["wallyedit"]["#default_value"]=$element['display_settings']['wallyedit'];
      } else {
        $form['level1'][$index_element]["wallyedit"]["#default_value"]=1;
      }



      $form['level1'][$index_element]['onglet']=array(
            "#type"=>'select',
            "#options"=>$options_onglet,
			'#default_value'=>$index_onglet,
      //     "#attributes"=>array('class'=>"my-elements-onglet-lev1 my-elements-onglet-lev1-". $index_onglet),
      );
      if(isset($element['group_name'])){
        //Gentlemans, We have a band here
        foreach($element["fields"] as $index_field=>$field){

          $form['level1'][$index_element]['level2'][$index_field]['field_name']=array(
            '#type' => 'markup',
            '#value' => $field['field_name']
          );
          $form['level1'][$index_element]['level2'][$index_field]['weight']=array(
            "#type"=>'weight',
			'#default_value'=>$weight,
            "#attributes"=>array('class'=>"my-elements-weight-lev2 my-elements-weight-lev2-". $index_field),
          );


          if(isset($element['type']) && $element['type']=='nodereference') {
            $form['level1'][$index_element]['wallyedit']=array(
            '#type'=>'select',
            '#options'=>array(0=>t("Exclude"),1=>t("Normal")),
            );
            $hits = 0;
            foreach ($element['referenceable_types'] as $key => $value) {
              if ($key === $value)
              $hits++;
            }
            if ($element['multiple'] == 0 && $hits == 1) {
              $form['level1'][$index_element]["wallyedit"]["#options"][2]=t('Inline');
              $form['level1'][$index_element]["wallyedit"]["#options"][3]=t('Select box');
            } else {
              $form['level1'][$index_element]["wallyedit"]["#options"][3]=t('Select box');
            }
          }
          if(isset($element['display_settings']['wallyedit'])) {
            $form['level1'][$index_element]["wallyedit"]["#default_value"]=$element['display_settings']['wallyedit'];
          } else {
            $form['level1'][$index_element]["wallyedit"]["#default_value"]=1;
          }


        }
      }
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#title' => t('Save'),
    '#value' => t('Save'),
  );

  return $form;
}

function wallyedit_test_form_submit(&$form, &$form_state) {
  $values = $form_state['values'];
  $type_name = $form_state['type'];
  $profile_id = $form_state['pid'];
  $level1['level1'] = $values['level1'];

  $tabs_config = array(
    'pid' => $profile_id,
    'content_type' => $type_name,
    'tabs' => $level1,
  );
dsm($tabs_config);
  if(drupal_write_record('wallyedit_tabs_edition', $tabs_config)) {
    drupal_set_message(t('Configuration saved'));
  } else {
    drupal_set_message(t('An error occured'), 'error');
  }
}

function wallyedit_get_onglets($profile_id, $type_name, $groups, $fields) {
  $db_tabs = db_query('SELECT tid, name, label, weight, erasable FROM {wallyedit_tabs_config} WHERE pid = %d AND content_type = "%s"', $profile_id, $type_name);
  $tab_settings = db_fetch_array(db_query('SELECT tabs FROM {wallyedit_tabs_edition} WHERE pid = %d AND content_type = "%s"', $profile_id, $type_name));

  while ($db_tab = db_fetch_array($db_tabs)) {
    $onglets[$db_tab['name']] = $db_tab;
    $onglets[$db_tab['name']]['elements'] = array();
  }
  $onglets['no_tab'] = array(
    'name' => 'no_tab',
    'label' => t('Uncategorized'),
    'weight' => 1000,
    'erasable' => 0,
    'elements' => array(),
  );

  $fields_and_groups = $fields + $groups;
  foreach ($fields_and_groups as $element_name => $element) {
    $element_settings = isset($tab_settings['level1'][$element_name]) ? $tab_settings['level1'][$element_name] : array();
    if (isset($onglets[$element_settings['onglet']])) {
      $tab_name = $element_settings['onglet'];
    } else {
      // Default tab for uncategorized elements
      $tab_name = 'no_tab';
    }
    if (isset($groups[$element_name])) {
      // This is a group
      // Merge to edit weight
      $onglets[$tab_name]['elements'][$element_name] = array_merge($groups[$element_name], $element_settings);
      foreach ($groups[$element_name]['fields'] as $field_name => $field) {
        $element_settings['level2'][$field_name] = isset($element_settings['level2'][$field_name]) ? $element_settings['level2'][$field_name] : array();
        $onglets[$tab_name]['elements'][$element_name]['fields'][$field_name] = array_merge($groups[$element_name]['fields'][$field_name], $element_settings['level2'][$field_name]);
      }
    } else {
      // This is a field
      $onglets[$tab_name]['elements'][$element_name] = array_merge($fields[$element_name], $element_settings);
    }
  }

  return $onglets;
}

function wallyedit_preprocess_wallyedit_test_form(&$variables){

  foreach($variables['form']['level1'] as $index_element=>$element){
    $onglet_name=$variables['form']['level1'][$index_element]['onglet']["#default_value"];
    $options_onglet[$onglet_name]=$onglet_name;
  }
  $variables['element_onglets'] =  $options_onglet;// + array(-1 => t('Disabled'));

  foreach (element_children($variables['form']['level1']) as $i) {
    $element = &$variables['form']['level1'][$i];

    // Only take form elements that are blocks.
    if (isset($element['weight'])) {
      // Fetch region for current block.
      $onglet = $element['onglet']['#default_value'];
      // Set special classes needed for table drag and drop.
      $variables['form']['level1'][$i]['onglet']['#attributes']['class'] = 'my-element-onglet-lev1 my-element-onglet-lev1-'. $onglet;
      $variables['form']['level1'][$i]['weight']['#attributes']['class'] = 'element-weight-lev1 element-weight-lev1-'. $onglet;


      $variables['element_listing'][$onglet][$i] = new stdClass();
      $variables['element_listing'][$onglet][$i]->row_class = isset($element['#attributes']['class']) ? $element['#attributes']['class'] : '';
      $variables['element_listing'][$onglet][$i]->block_modified = isset($element['#attributes']['class']) && strpos($element['#attributes']['class'], 'block-modified') !== FALSE ? TRUE : FALSE;
      $variables['element_listing'][$onglet][$i]->block_title =  drupal_render($element['field_name']);
      $variables['element_listing'][$onglet][$i]->region_select = drupal_render($element['onglet']);
      $variables['element_listing'][$onglet][$i]->weight_select = drupal_render($element['weight']);
      $variables['element_listing'][$onglet][$i]->wallyedit_select = drupal_render($element['wallyedit']);
      $variables['element_listing'][$onglet][$i]->configure_link = drupal_render($element['configure']);
      $variables['element_listing'][$onglet][$i]->printed = FALSE;


      if($variables['form']['level1'][$i]['level2']){
        $variables['element_listing'][$onglet][$i]->has_level2=TRUE;
        foreach(element_children($variables['form']['level1'][$i]['level2']) as $index_j=>$j){

          $variables['form']['level1'][$i]['level2'][$j]['weight']['#attributes']['class'] = 'element-weight-lev2 element-weight-lev2-'. $i;

          $subelement=&$variables['form']['level1'][$i]['level2'][$j];

          $variables['element_listing'][$onglet][$i]->level2[$j] = new stdClass();
          $variables['element_listing'][$onglet][$i]->level2[$j]->row_class = isset($subelement['#attributes']['class']) ? $subelement['#attributes']['class'] : '';
          $variables['element_listing'][$onglet][$i]->level2[$j]->block_modified = isset($subelement['#attributes']['class']) && strpos($subelement['#attributes']['class'], 'block-modified') !== FALSE ? TRUE : FALSE;
          $variables['element_listing'][$onglet][$i]->level2[$j]->block_title =  drupal_render($subelement['field_name']);
          $variables['element_listing'][$onglet][$i]->level2[$j]->weight_select = drupal_render($subelement['weight']);
          $variables['element_listing'][$onglet][$i]->level2[$j]->wallyedit_select = drupal_render($subelement['wallyedit']);
          $variables['element_listing'][$onglet][$i]->level2[$j]->configure_link = drupal_render($subelement['configure']);
          $variables['element_listing'][$onglet][$i]->level2[$j]->printed = FALSE;


        }
      }
      $last_region = $onglet;
    }
  }



  $variables['form_submit'] = drupal_render($variables['form']);
}
