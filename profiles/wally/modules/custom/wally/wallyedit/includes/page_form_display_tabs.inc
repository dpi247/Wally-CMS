<?php

/**
 * Show existing profiles and tabs and add new ones
 */
function wallyedit_page_profiles_overview () {
  $profiles_header = array(t('Name'), array('data' => t('Operations'), 'colspan' => '2'));
  $profiles_rows = array();
  $profiles_caption = t('Profiles List');

  $db_profiles = db_query('SELECT * FROM {wallyedit_profiles}');
  $profiles = array();
  while ($profile = db_fetch_array($db_profiles)) {
    $profiles[$profile['pid']] = $profile;
    $row = array(
      check_plain($profile['label']),
      array('data' => l(t('Choose content type'), 'admin/content/wallydisplaytabs/displaytabs/'. $profile['pid']).' · <a href="/admin/content/wallydisplaytabs/deleteprofile/'.$profile['pid'].'?destination='.$_GET['q'].'">'.t('Delete').'</a>'),
    );

    $profiles_rows[] = $row;
  }

  $output = theme('table', $profiles_header, $profiles_rows, array(), $profiles_caption);
  $output .= drupal_get_form('wallyedit_form_add_new_profile_form');

  return $output;
}

/**
 * List content types for a profile and choose edition type
 */
function wallyedit_form_tabs_content_types_overview_form (&$form_state, $profile_id) {
  $form = array(
    '#tree' => TRUE,
  );

  if (is_numeric($profile_id)) {
    $interface_types = variable_get('wallyedit_content_types_interface_type_'.$profile_id, array());
    $types = node_get_types();
    $names = node_get_types('names');
    $editions = array(
      'wally' => 'Wally',
      'drupal' => 'Drupal',
    );
    $form['profile_id'] = array(
      '#type' => 'value',
      '#value' => $profile_id,
    );
    $form['rows'] = array();

    foreach ($names as $key => $name) {
      $type = $types[$key];
      if (node_hook($type, 'form')) {
        $form['rows'][$key] = array();

        $type_url_str = str_replace('_', '-', $type->type);

        $form['rows'][$key]['name'] = array(
          '#type' => 'value',
          '#value' => l($name, 'admin/content/node-type/'. $type_url_str),
        );

        $form['rows'][$key]['type'] = array(
          '#type' => 'value',
          '#value' => check_plain($type->type),
        );

        $form['rows'][$key]['description'] = array(
          '#type' => 'value',
          '#value' => $type->description,
        );

        $form['rows'][$key]['edition'] = array(
          '#type' => 'select',
          '#options' => $editions,
          '#default_value' => $interface_types[$key],
        );

        $form['rows'][$key]['operations'] = array(
          '#type' => 'value',
          '#value' => l(t('Organise fields into tabs'), 'admin/content/wallydisplaytabs/displaytabs/'.$profile_id.'/'.$type_url_str),
        );
      }
    }
    
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
  } else {
    drupal_set_message(t('No profile given!'), 'error');
  }
  
  return $form;
}

function wallyedit_form_tabs_content_types_overview_form_submit ($form, &$form_state) {
  $values = $form_state['values'];
  
  $content_editions = array();
  foreach ($values['rows'] as $content_key => $content_values) {
    $content_editions[$content_key] = $content_values['edition'];
  }
  
  variable_set('wallyedit_content_types_interface_type_'.$values['profile_id'], $content_editions);
  drupal_set_message(t('Configuration saved.'));
}

/**
 * Show tabs for a profile and a content type
 */
function wallyedit_form_tabs_overview_form (&$form_state, $profile_id, $content_key) {
  $form = array(
    '#tree' => TRUE,
  );

  if (is_numeric($profile_id)) {
      $form['profile_id'] = array(
      '#type' => 'value',
      '#value' => $profile_id,
    );

    $content_names = node_get_types('names');
    foreach ($content_names as $key => $name) {
      $type_url_str = str_replace('_', '-', $key);
      if ($content_key == $type_url_str) {
        $content_key = $key;
        break;
      }
    }

    $form['content_key'] = array(
      '#type' => 'value',
      '#value' => $content_key,
    );

    $db_tabs = db_query('SELECT * FROM {wallyedit_tabs_config} WHERE pid = %d AND content_type = "%s" ORDER BY weight ASC', $profile_id, $content_key);
    $count_tabs = db_result(db_query('SELECT COUNT(*) FROM {wallyedit_tabs_config} WHERE pid = %d AND content_type = "%s"', $profile_id, $content_key));
    for ($j = 1; $tab = db_fetch_array($db_tabs); $j++) {
      $form['rows'][$tab['tid']]['label'] = array(
        '#type' => 'value',
        '#value' => $tab['label'],
      );

      $operations = l(t('Manage fields'), 'admin/content/wallydisplaytabs/displaytabs/'.$profile_id.'/'.$type_url_str.'/fields');
      $operations .= ' · <a href="/admin/content/wallydisplaytabs/tablabel/'.$tab['tid'].'?destination='.$_GET['q'].'">'.t('Edit Label').'</a>';
      $operations .= $tab['erasable'] ? ' · <a href="/admin/content/wallydisplaytabs/deletetab/'.$tab['tid'].'?destination='.$_GET['q'].'">'.t('Delete').'</a>' : '';
      $form['rows'][$tab['tid']]['operations'] = array(
        '#type' => 'value',
        '#value' => $operations,
      );
      
      $form['rows'][$tab['tid']]['weight'] = array(
        '#type' => 'weight',
        '#default_value' => $j,
        //add a specific class in here - we need this later
        '#attributes' => array('class' => 'weight'),
        '#delta' => $count_tabs,
      );
    }

    $form['save_order'] = array(
      '#type' => 'submit',
      '#value' => t('Save order'),
      '#weight' => 0,
    );

    $form['label'] = array(
      '#type' => 'textfield',
      '#title' => t('Add a new tab'),
      '#description' => t('Enter tab label here'),
      '#default_value' => '',
    );

    $form['erasable'] = array(
      '#type' => 'hidden',
      '#title' => t('Erasable'),
      '#value' => 1,
    );

    $form['add'] = array(
      '#type' => 'submit',
      '#value' => t('Add'),
    );
  } else {
    drupal_set_message(t('No profile given!'), 'error');
  }

  return $form;
}

function wallyedit_form_tabs_overview_form_submit ($form, &$form_state) {
  $values = $form_state['values'];

  if ($values['op'] == $values['add']) {
    $new_tab = array();
    $bad_car = array(' ', '-');
    $new_tab['name'] = strtolower(str_replace($bad_car, '_', $values['label'])).'_'.time();
    $new_tab['label'] = $values['label'];
    $new_tab['weight'] = 0;
    $new_tab['pid'] = $values['profile_id'];
    $new_tab['content_type'] = $values['content_key'];
    $new_tab['erasable'] = $values['erasable'];

    drupal_write_record('wallyedit_tabs_config', $new_tab);
    drupal_set_message(t('Tab "@tab" saved!', array('@tab' => $new_tab['label'])));
  } elseif ($values['op'] == $values['save_order']) {
    foreach ($values['rows'] as $tab_id => $tab_values) {
      db_query('UPDATE {wallyedit_tabs_config} SET weight = %d WHERE tid = %d', $tab_values['weight'], $tab_id);
    }
    drupal_set_message(t('Tab order saved!'));
  }
}

/**
 * Organise fields for a profile and a content type
 */
function wallyedit_form_fields_overview_form (&$form_state, $profile_id, $content_key) {
  $content_names = node_get_types('names');
  foreach ($content_names as $key => $name) {
    $type_url_str = str_replace('_', '-', $key);
    if ($content_key == $type_url_str) {
      $type_name = $key;
      break;
    }
  }
  
  $onglets = wallyedit_get_onglets($profile_id, $type_name,$form_state);

  $form=array(
    '#tree' => TRUE,
  );

  $form['profile_id'] = array(
    '#type' => 'value',
    '#value' => $profile_id,
  );

  $form['type'] = array(
    '#type' => 'value',
    '#value' => $type_name,
  );

  foreach($onglets as $index_onglet => $onglet){
    $options_onglet[$index_onglet] = $onglet['label'];
  }

  foreach($onglets as $index_onglet => $onglet){
    foreach($onglet["elements"] as $index_element => $element){
      $form['level1'][$index_element]['field_name']=array(
        '#type' => 'markup',
        '#value' => isset($element['label']) ? $element['label'] : $element['field_name'].$element['group_name'],
      );

      $form['level1'][$index_element]['weight']=array(
        "#type"=>'weight',
        '#default_value'=>$weight,
        //"#attributes"=>array('class'=>"my-elements-weight-lev1 my-elements-weight-lev1-". $index_onglet),
      );

      if(isset($element['type']) && $element['type'] == 'nodereference') {
        $form['level1'][$index_element]['wallyedit'] = array(
          '#type'=>'select',
          '#options'=>array(0 => t("Exclude"), 1 => t("Normal")),
        );
        $hits = 0;
        foreach ($element['referenceable_types'] as $key => $value) {
          if ($key === $value)
          $hits++;
        }
        if ($element['multiple'] == 0 && $hits == 1) {
          $form['level1'][$index_element]["wallyedit"]["#options"][2] = t('Inline');
          $form['level1'][$index_element]["wallyedit"]["#options"][3] = t('Select box');
        } else {
          $form['level1'][$index_element]["wallyedit"]["#options"][3] = t('Select box');
        }
      }
      if(isset($element['display_settings']['wallyedit'])) {
        $form['level1'][$index_element]["wallyedit"]["#default_value"] = $element['display_settings']['wallyedit'];
      } else {
        $form['level1'][$index_element]["wallyedit"]["#default_value"] = 1;
      }

      $form['level1'][$index_element]['onglet'] = array(
        "#type"=>'select',
        "#options"=>$options_onglet,
        '#default_value'=>$index_onglet,
        //"#attributes"=>array('class'=>"my-elements-onglet-lev1 my-elements-onglet-lev1-". $index_onglet),
      );

      if(isset($element['group_name'])) {
        //Gentlemans, We have a band here
        foreach($element["fields"] as $index_field=>$field){
          $form['level1'][$index_element]['level2'][$index_field]['field_name']=array(
            '#type' => 'markup',
            '#value' => isset($field['label']) ? $field['label'] : $field['field_name'],
          );

          $form['level1'][$index_element]['level2'][$index_field]['weight']=array(
            "#type"=>'weight',
            '#default_value'=>$weight,
            "#attributes"=>array('class'=>"my-elements-weight-lev2 my-elements-weight-lev2-". $index_field),
          );

          if(isset($element['type']) && $element['type'] == 'nodereference') {
            $form['level1'][$index_element]['wallyedit'] = array(
              '#type'=>'select',
              '#options'=>array(0 => t("Exclude"), 1 => t("Normal")),
            );
            $hits = 0;
            foreach ($element['referenceable_types'] as $key => $value) {
              if ($key === $value)
              $hits++;
            }
            if ($element['multiple'] == 0 && $hits == 1) {
              $form['level1'][$index_element]["wallyedit"]["#options"][2] = t('Inline');
              $form['level1'][$index_element]["wallyedit"]["#options"][3] = t('Select box');
            } else {
              $form['level1'][$index_element]["wallyedit"]["#options"][3] = t('Select box');
            }
          }
          if(isset($element['display_settings']['wallyedit'])) {
            $form['level1'][$index_element]["wallyedit"]["#default_value"] = $element['display_settings']['wallyedit'];
          } else {
            $form['level1'][$index_element]["wallyedit"]["#default_value"] = 1;
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

function wallyedit_form_fields_overview_form_submit ($form, &$form_state) {
  $values = $form_state['values'];
  $type_name = $values['type'];
  $profile_id = $values['profile_id'];
  $level1['level1'] = $values['level1'];

  uasort($level1['level1'], '_wallyedit_sort_fields_helper');
  foreach ($level1['level1'] as &$element) {
    if (is_array($element['level2']) && !empty($element['level2'])) {
      uasort($element['level2'], '_wallyedit_sort_fields_helper');
    }
  }

  wallyedit_save_tabs_and_weight($type_name, $profile_id, $level1);
}

function wallyedit_save_tabs_and_weight($type_name, $profile_id, $level1) {
  $tabs_config = array(
    'pid' => $profile_id,
    'content_type' => $type_name,
    'tabs' => $level1,
  );

  if ($sid = db_result(db_query('SELECT sid FROM {wallyedit_tabs_edition} WHERE pid = %d AND content_type = "%s"', $profile_id, $type_name))) {
    $tabs_config['sid'] = $sid;
    if(drupal_write_record('wallyedit_tabs_edition', $tabs_config, 'sid')) {
      drupal_set_message(t('Tabs and weight configuration saved'));
    } else {
      drupal_set_message(t('An error occured during tabs and weight save'), 'error');
    }
  } else {
    if(drupal_write_record('wallyedit_tabs_edition', $tabs_config)) {
      drupal_set_message(t('Tabs and weight configuration saved'));
    } else {
      drupal_set_message(t('An error occured during tabs and weight save'), 'error');
    }
  }
}

/**
 * Add new profile
 */
function wallyedit_form_add_new_profile_form(&$form_state) {
  $form = array();

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Add a new profile'),
    '#description' => t('Enter profile label here'),
    '#default_value' => '',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#title' => t('Add'),
    '#value' => t('Add'),
  );

  return $form;
}

function wallyedit_form_add_new_profile_form_submit(&$form, &$form_state) {
  $values = $form_state['values'];

  drupal_write_record('wallyedit_profiles', $values);
  drupal_set_message(t('Profile "@profile" saved!', array('@profile' => $values['label'])));

  foreach (node_get_types('names') as $content_type => $ct_name) {
    $main_tab = array(
      'name' => 'main_'.$values['pid'].'_'.$content_type,
      'label' => t('Main'),
      'weight' => 1,
      'pid' => $values['pid'],
      'content_type' => $content_type,
      'erasable' => 0,
    );
    drupal_write_record('wallyedit_tabs_config', $main_tab);
    $meta_tab = array(
      'name' => 'meta_'.$values['pid'].'_'.$content_type,
      'label' => t('Meta'),
      'weight' => 2,
      'pid' => $values['pid'],
      'content_type' => $content_type,
      'erasable' => 0,
    );
    drupal_write_record('wallyedit_tabs_config', $meta_tab);
  }
}

function wallyedit_form_display_tabs_form (&$form_state, $type, $profile_id) {
  module_load_include("inc",'content','includes/content.node_form');
  module_load_include("inc",'content','includes/content.admin');
  module_load_include('inc', 'node', 'node.pages');
  $form = drupal_retrieve_form('content_field_overview_form', $form_state, $type->type);

  $node = new Stdclass;
  $node->type = $type->type;
  $node->name = '';
  
  // Get node form
  $node_form = drupal_retrieve_form(str_replace('-','_',$type->type).'_node_form', $form_state, $node);
  drupal_prepare_form('node_form', $node_form, $form_state);

  // Remove unnecessery fields and groups from the form
  foreach ($form['#field_rows'] as $field_id => $field_name) {
    if ($field_name == '_add_new_field' || $field_name == '_add_existing_field') {
      unset($form['#field_rows'][$field_id]);
    } elseif (isset($node_form[$field_name]) && is_array($node_form[$field_name]) && $node_form[$field_name]['#type'] == 'fieldset') {
      _wallyedit_tabs_add_new_group_to_form($form, $node_form[$field_name], $field_name);
    }
  }
  foreach ($form['#group_rows'] as $group_id => $group_name) {
    if ($group_name == '_add_new_group')
      unset($form['#group_rows'][$group_id]);
  }
  unset($form['_add_new_field']);
  unset($form['_add_existing_field']);
  unset($form['_add_new_group']);

  foreach ($form as $field_name => $field) {
    if (is_array($field) && isset($field['configure']))
      unset($form[$field_name]['configure']);
    if (is_array($field) && isset($field['remove']))
      unset($form[$field_name]['remove']);
  }

  $fields = wydit_get_fields_infos_for_typename($type->type, 'fields');

  foreach ($form["#field_rows"] as $field) {
    $form[$field]["wallyedit"]=array(
      '#type'=>'select',
      '#options'=>array(0=>t("Exclude"),1=>t("Normal")),
    );
    if(isset($fields[$field]['type']) && $fields[$field]['type']=='nodereference') {
      $hits = 0;
      foreach ($fields[$field]['referenceable_types'] as $key => $value) {
        if ($key === $value)
          $hits++;
      }
      if ($fields[$field]['multiple'] == 0 && $hits == 1) {
        $form[$field]["wallyedit"]["#options"][2]=t('Inline');
        $form[$field]["wallyedit"]["#options"][3]=t('Select box');
      } else {
        $form[$field]["wallyedit"]["#options"][3]=t('Select box');
      }
    }
    if(isset($fields[$field]['display_settings']['wallyedit'])) {
      $form[$field]["wallyedit"]["#default_value"]=$fields[$field]['display_settings']['wallyedit'];
    } else {
      $form[$field]["wallyedit"]["#default_value"]=1;
    }
  }

  foreach ($form["#group_rows"] as $group) {
    $form[$group]["wallyedit"]=array(
      '#type'=>'select',
      '#options'=>array(0=>t("Exclude"),1=>t("Normal")),
    );
    if(isset($fields[$group]['display_settings']['wallyedit'])) {
      $form[$group]["wallyedit"]["#default_value"]=$fields[$group]['display_settings']['wallyedit'];
    } else {
      $form[$group]["wallyedit"]["#default_value"]=1;
    }
  }

  $form['#parameters'][0] = 'wallyedit_form_display_tabs_form';

  $form['#field_rows'] = array_values($form['#field_rows']);

  return $form;
}

/**
 * Form to delete a profile
 */
function wallyedit_form_delete_profile_form (&$form_state, $profile_id) {
  $redirect = isset($_GET['destination']) ? $_GET['destination'] : 'admin/content';
  
  if ($profile = db_fetch_array(db_query('SELECT * FROM {wallyedit_profiles} WHERE pid = %d', $profile_id))) {
    
  } else {
    drupal_set_message(t('Profile not found!'), 'error');
    drupal_goto($redirect);
  }
  $question = t('Are you sure you want to delete profile "@profile"?', array('@profile' => $profile['label']));
  
  $add_form = array();
  $add_form['pid'] = array(
    '#type' => 'value',
    '#value' => $profile_id,
  );
  
  $form = confirm_form($add_form, $question, $redirect);
  $form['#redirect'] = $redirect;
  return $form;
}

function wallyedit_form_delete_profile_form_submit(&$form, &$form_state) {
  $values = $form_state['values'];
  $profile_id = $values['pid'];

  if (db_result(db_query('SELECT * FROM {wallyedit_profiles} WHERE pid = %d', $profile_id))) {
    db_query('DELETE FROM {wallyedit_profiles} WHERE pid = %d', $profile_id);
    db_query('DELETE FROM {wallyedit_tabs_config} WHERE pid = %d', $profile_id);
    db_query('DELETE FROM {wallyedit_tabs_edition} WHERE pid = %d', $profile_id);
    drupal_set_message(t('Profile deleted.'));
  } else {
    drupal_set_message(t('No profile matching the requested key.'), 'warning');
  }
}

/**
 * Form to edit a tab label
 */
function wallyedit_form_edit_label_tab_form (&$form_state, $tab_id) {
  $tab = db_fetch_array(db_query('SELECT label FROM {wallyedit_tabs_config} WHERE tid = %d', $tab_id));

  $form = array();

  $form['new_label'] = array(
    '#type' => 'textfield',
    '#title' => t('New Label'),
    '#default_value' => $tab['label'],
  );

  $form['tab_id'] = array(
    '#type' => 'hidden',
    '#value' => $tab_id,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#title' => t('Save'),
    '#value' => t('Save'),
  );

  $form['cancel'] = array(
    '#type' => 'submit',
    '#title' => t('Cancel'),
    '#value' => t('Cancel'),
  );

  return $form;
}

function wallyedit_form_edit_label_tab_form_submit ($form, $form_state) {
  $values = $form_state['values'];
  if ($values['op'] == $values['submit']) {
    db_query('UPDATE {wallyedit_tabs_config} SET label = "%s" WHERE tid = %d', $values['new_label'], $values['tab_id']);
  }
  if (!isset($_GET['destination'])) {
    drupal_goto('admin/content');
  }
}

/**
 * Form to delete a tab
 */
function wallyedit_form_delete_tab_form (&$form_state, $tab_id) {
  $redirect = isset($_GET['destination']) ? $_GET['destination'] : 'admin/content';
  
  if ($tab = db_fetch_array(db_query('SELECT label FROM {wallyedit_tabs_config} WHERE tid = %d', $tab_id))) {
    
  } else {
    drupal_set_message(t('Tab not found!'), 'error');
    drupal_goto($redirect);
  }
  $question = t('Are you sure you want to delete tab "@tab"?', array('@tab' => $tab['label']));
  
  $add_form = array();
  $add_form['tab_id'] = array(
    '#type' => 'value',
    '#value' => $tab_id,
  );
  
  $redirect = isset($_GET['destination']) ? $_GET['destination'] : 'admin/content';
  $form = confirm_form($add_form, $question, $redirect);
  $form['#redirect'] = $redirect;
  return $form;
}

function wallyedit_form_delete_tab_form_submit(&$form, &$form_state) {
  $values = $form_state['values'];
  $tab_id = $values['tab_id'];

  if ($erasable = db_result(db_query('SELECT erasable FROM {wallyedit_tabs_config} WHERE tid = %d', $tab_id))) {
    if ($erasable) {
      db_query('DELETE FROM {wallyedit_tabs_config} WHERE tid = %d', $tab_id);
      drupal_set_message(t('Tab deleted.'));
    } else {
      drupal_set_message(t('This tab can\'t be deleted.'), 'warning');
    }
  } else {
    drupal_set_message(t('No tab matching the requested key.'), 'warning');
  }
}

/**
 * Return list of profiles
 */
function wyditadmin_get_profiles() {
  static $profiles = array();
  if (empty($profiles)) {
    $db_profiles = db_query('SELECT * FROM {wallyedit_profiles}');
    while ($profile = db_fetch_array($db_profiles)) {
      $profiles[$profile['pid']] = $profile;
    }
  }
  return $profiles;
}

/**
 * Return list of tabs for a profile and a content type
 */
function wyditadmin_get_tabs($profile_id, $type_name) {
  static $tabs = array();
  if (!isset($tabs[$profile_id][$type_name])) {
    $db_tabs = db_query('SELECT * FROM {wallyedit_tabs} WHERE pid = %d AND content_type = "%s"', $profile_id, $type_name);
    while ($tab = db_fetch_array($db_tabs)) {
      $tabs[$profile_id][$type_name][$tab['tid']] = $tab;
    }
  }
  return $tabs[$profile_id][$type_name];
}

/**
 * Return list of groups for a content type
 */
function wyditadmin_get_groups($type_name) {
  static $groups = array();
  if (!isset($groups[$type_name])) {
    $db_groups = db_query('SELECT * FROM {wallyedit_groups} WHERE content_type = "%s"', $type_name);
    while ($group = db_fetch_array($db_groups)) {
      $groups[$type_name][$group['gid']] = $group;
    }
  }
  return $groups[$type_name];
}

/**
 * Return list of fields in a flat stucture
 */
function wyditadmin_get_fields_flat($profile_id, $type_name) {
  static $fields = array();
  if (!isset($fields[$profile_id][$type_name])) {
    $db_fields = db_query('SELECT * FROM {wallyedit_fields} WHERE pid = %d AND content_type = "%s"', $profile_id, $type_name);
    while ($field = db_fetch_array($db_fields)) {
      $fields[$profile_id][$type_name][$field['fid']] = $field;
    }
  }
  return $fields[$profile_id][$type_name];
}

/**
 * Return list of fields in a tree stucture
 */
function wyditadmin_get_fields_tree($profile_id, $type_name) {
  static $tree_fields = array();
  if (!isset($tree_fields[$profile_id][$type_name])) {
    $tabs = wyditadmin_get_tabs($profile_id, $type_name);
    $groups = wyditadmin_get_groups($type_name);
    $flat_fields = wyditadmin_get_fields_flat($profile_id, $type_name);
    foreach ($tabs as $tab) {
      $tree_fields[$profile_id][$type_name][$tab['name']] = $tab;
      $tree_fields[$profile_id][$type_name][$tab['name']]['elements'] = array();
      foreach ($groups as $group) {
        $tree_fields[$profile_id][$type_name][$tab['name']]['elements'][$group['name']] = $group;
        $tree_fields[$profile_id][$type_name][$tab['name']]['elements'][$group['name']]['fields'] = array();
      }
    }
    foreach ($flat_fields as $fid => $field) {
      $tab_name = $tabs[$field['tid']]['name'];
      $group_name = $field['gid'] != 0 ? $groups[$field['gid']]['name'] : 'none';
      if ($group_name != 'none') {
        $tree_fields[$profile_id][$type_name][$tab_name]['elements'][$group_name]['fields'][$field['name']] = $field;
      } else {
        $tree_fields[$profile_id][$type_name][$tab_name]['elements'][$field['name']] = $field;
      }
    }
    foreach ($tabs as $tab) {
      foreach ($groups as $group) {
        if (empty($tree_fields[$profile_id][$type_name][$tab['name']]['elements'][$group['name']]['fields'])) {
          unset ($tree_fields[$profile_id][$type_name][$tab['name']]['elements'][$group['name']]);
        }
      }
      if (empty($tree_fields[$profile_id][$type_name][$tab['name']]['elements'])) {
        unset ($tree_fields[$profile_id][$type_name][$tab['name']]);
      }
    }
  }
  return $tree_fields[$profile_id][$type_name];
}

/**
 * Sort function for items order.
 * (copied form element_sort(), which acts on #weight keys)
 */
function _wallyedit_sort_fields_helper ($a, $b) {
  $a_weight = (is_array($a) && isset($a['weight'])) ? $a['weight'] : 0;
  $b_weight = (is_array($b) && isset($b['weight'])) ? $b['weight'] : 0;
  if ($a_weight == $b_weight) {
    return 0;
  }
  return ($a_weight < $b_weight) ? -1 : 1;
}

