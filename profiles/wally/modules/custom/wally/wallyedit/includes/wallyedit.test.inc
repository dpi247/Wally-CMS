<?php 

function wallyedit_test(){
 return drupal_get_form('wallyedit_test_form');

}



function wallyedit_test_form($form_state){
  $type_name='wally_articlepackage';
  
  $type=content_types(str_replace('-','_', $type_name));
  
  $groups=fieldgroup_groups($type_name) ;
  $fields=$type['fields'];
 
  //remove fields already included in a groups
  foreach($groups as $group){
    foreach($group["fields"] as $key=>$field){
      unset($fields[$key]);
    }
  }
  
  //SIMULATE FAKE ONGLETS STRUCTURE:
  $onglets=array(
    'main'=>array(
      "label"=>"MAin",
      "weight"=>2,
      "elements"=>$fields+array('group_destinations'=>$groups['group_destinations']),
    ),
    'meta'=>array(
      "label"=>"Meta",
      "weight"=>1,
      "elements"=>array('group_identification'=>$groups['group_identification']),
    ),
    'onglet1'=>array(
      "label"=>"MAin",
      "weight"=>1,
      "elements"=>array('group_content'=>$groups['group_content']),
    ),
    'onglet2'=>array(
      "label"=>"Divers",
      "weight"=>3,
      "elements"=>array('group_taxonomy'=>$groups['group_taxonomy'],'group_authoring'=>$groups['group_authoring']),
    ),
  );
  
  
  $form=array();
  
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
          
        dsm($field);
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
  return $form;
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
