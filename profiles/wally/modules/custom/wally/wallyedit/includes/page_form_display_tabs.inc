<?php

module_load_include('inc', 'wallyedit', 'includes/config_api');

/**
 * Show existing profiles and tabs and add new ones
 */
function wallyedit_page_profiles_overview () {
  $profiles_header = array(t('Name'), t('Default profile'), array('data' => t('Operations'), 'colspan' => '2'));
  $profiles_rows = array();
  $profiles_caption = t('Profiles List');

  $db_profiles = db_query('SELECT * FROM {wallyedit_profiles}');
  $profiles = array();
  while ($profile = db_fetch_array($db_profiles)) {
    $profiles[$profile['pid']] = $profile;
    $operations = l(t('Choose content type'), 'admin/content/wallydisplaytabs/displaytabs/'. $profile['pid']);
    $operations .= ' · <a href="/admin/content/wallydisplaytabs/deleteprofile/'.$profile['pid'].'?destination='.$_GET['q'].'">'.t('Delete').'</a>';
    $operations .= ' · <a href="/admin/content/wallydisplaytabs/setdefaultprofile/'.$profile['pid'].'?destination='.$_GET['q'].'">'.t('Use as default').'</a>';
    $row = array(
      check_plain($profile['label']),
      (isset($profile['default_profile']) && $profile['default_profile'] == 1) ? t('Yes') : t('No'),
      array('data' => $operations),
    );

    $profiles_rows[] = $row;
  }

  $output = !empty($profiles_rows) ? theme('table', $profiles_header, $profiles_rows, array(), $profiles_caption) : '';
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
    
    if (empty($interface_types)) {
      drupal_set_message(t('Edition types not saved, the classical Drupal node edition will always be used.'), 'notice');
    }
    
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
          '#value' => $name,
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
 * Show groups for a content type
 */
function wallyedit_form_groups_overview_form(&$form_state, $profile_id, $content_key) {
  $form = array(
    '#tree' => TRUE,
  );

  $content_names = node_get_types('names');
  foreach ($content_names as $key => $name) {
    $type_url_str = str_replace('_', '-', $key);
    if ($content_key == $type_url_str) {
      $type_name = $key;
      break;
    }
  }

  $form['content_key'] = array(
    '#type' => 'value',
    '#value' => $type_name,
  );

  $groups = wyditadmin_get_groups($type_name);
  unset($groups[0]);
  $max_weight = 0;
  foreach ($groups as $group) {
    if (abs($group['weight']) > $max_weight) {
      $max_weight = abs($group['weight']);
    }
  }
  if ($max_weight < count($groups)) {
    $max_weight = count($groups);
  }
  $form['rows'] = array();
  foreach ($groups as $group) {
    $form['rows'][$group['gid']]['label'] = array(
      '#type' => 'value',
      '#value' => $group['label'],
    );

    $operations = '<a href="/admin/content/wallydisplaytabs/grouplabel/'.$group['gid'].'/'.$profile_id.'?destination='.$_GET['q'].'">'.t('Edit Label').'</a>';
    $operations .= ' · <a href="/admin/content/wallydisplaytabs/deletegroup/'.$group['gid'].'/'.$profile_id.'?destination='.$_GET['q'].'">'.t('Delete').'</a>';
    $form['rows'][$group['gid']]['operations'] = array(
      '#type' => 'value',
      '#value' => $operations,
    );

    $form['rows'][$group['gid']]['weight'] = array(
      '#type' => 'weight',
      '#default_value' => $group['weight'],
      //add a specific class in here - we need this later
      '#attributes' => array('class' => 'weight'),
      '#delta' => $max_weight,
    );
  }

  if (!empty($form['rows'])) {
    $form['save_order'] = array(
      '#type' => 'submit',
      '#value' => t('Save order'),
      '#weight' => 0,
    );
  }

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Add a new group'),
    '#description' => t('Enter group label here'),
    '#default_value' => '',
  );

  $form['add'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
  );
  
  drupal_set_message(t('Groups defined for a content type are common to every profiles.'), 'notice');

  return $form;
}

function wallyedit_form_groups_overview_form_validate ($form, &$form_state) {
  $values = $form_state['values'];
  if ($values['op'] == $values['add']) {
    if ($values['label'] == '') {
      form_set_error('label', t('Empty label given!'));
    }
  }
}

function wallyedit_form_groups_overview_form_submit ($form, &$form_state) {
  $values = $form_state['values'];

  if ($values['op'] == $values['add']) {
    $new_group = array();
    $bad_car = array(' ', '-');
    $new_group['name'] = strtolower(str_replace($bad_car, '_', $values['label'])).'_'.time();
    $new_group['label'] = $values['label'];
    $new_group['weight'] = 0;
    $new_group['pid'] = $values['profile_id'];
    $new_group['content_type'] = $values['content_key'];

    drupal_write_record('wallyedit_groups', $new_group);
    drupal_set_message(t('Group "@group" saved!', array('@group' => $new_group['label'])));
  } elseif ($values['op'] == $values['save_order']) {
    foreach ($values['rows'] as $group_id => $group_values) {
      db_query('UPDATE {wallyedit_groups} SET weight = %d WHERE gid = %d', $group_values['weight'], $group_id);
    }
    drupal_set_message(t('Group order saved!'));
  }
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
        $type_name = $key;
        break;
      }
    }

    $form['content_key'] = array(
      '#type' => 'value',
      '#value' => $type_name,
    );

    $tabs = wyditadmin_get_tabs($profile_id, $type_name);
    unset($tabs[0]);
    $max_weight = 0;
    foreach ($tabs as $tab) {
      if (abs($tab['weight']) > $max_weight) {
        $max_weight = abs($tab['weight']);
      }
    }
    if ($max_weight < count($tabs)) {
      $max_weight = count($tabs);
    }
    $form['rows'] = array();
    foreach ($tabs as $tab) {
      $form['rows'][$tab['tid']]['label'] = array(
        '#type' => 'value',
        '#value' => $tab['label'],
      );

      $operations = '<a href="/admin/content/wallydisplaytabs/tablabel/'.$tab['tid'].'/'.$profile_id.'?destination='.$_GET['q'].'">'.t('Edit Label').'</a>';
      $operations .= $tab['erasable'] ? ' · <a href="/admin/content/wallydisplaytabs/deletetab/'.$tab['tid'].'/'.$profile_id.'?destination='.$_GET['q'].'">'.t('Delete').'</a>' : '';
      $form['rows'][$tab['tid']]['operations'] = array(
        '#type' => 'value',
        '#value' => $operations,
      );
      
      $form['rows'][$tab['tid']]['weight'] = array(
        '#type' => 'weight',
        '#default_value' => $j,
        //add a specific class in here - we need this later
        '#attributes' => array('class' => 'weight'),
        '#delta' => $max_weight,
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

function wallyedit_form_tabs_overview_form_validate ($form, &$form_state) {
  $values = $form_state['values'];
  if ($values['op'] == $values['add']) {
    if ($values['label'] == '') {
      form_set_error('label', t('Empty label given!'));
    }
  }
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

    drupal_write_record('wallyedit_tabs', $new_tab);
    drupal_set_message(t('Tab "@tab" saved!', array('@tab' => $new_tab['label'])));
  } elseif ($values['op'] == $values['save_order']) {
    foreach ($values['rows'] as $tab_id => $tab_values) {
      db_query('UPDATE {wallyedit_tabs} SET weight = %d WHERE tid = %d', $tab_values['weight'], $tab_id);
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

  $existing_fields = wyditadmin_get_existing_fields($type_name);
  $flat_fields = wyditadmin_get_fields_flat($profile_id, $type_name);
  $tabs = wyditadmin_get_tabs($profile_id, $type_name);
  $groups = wyditadmin_get_groups($type_name);

  $groups_options = array();
  foreach($groups as $group){
    $groups_options[$group['gid']] = $group['label'];
  }

  $tabs_options = array();
  foreach($tabs as $tab){
    $tabs_options[$tab['tid']] = $tab['label'];
  }

  $form=array(
    '#tree' => TRUE,
  );

  foreach ($flat_fields as $field_name => $field) {
    $form['fields'][$field_name]['fid'] = array(
      '#type' => 'value',
      '#value' => $flat_fields[$field_name]['fid'],
    );
    $default_field = $flat_fields[$field_name];

    $form['fields'][$field_name]['label'] = array(
      '#type' => 'markup',
      '#value' => $default_field['label'],
    );

    $form['fields'][$field_name]['group'] = array(
      '#type' => 'select',
      '#options' => $groups_options,
      '#default_value' => $default_field['gid'],
    );

    $form['fields'][$field_name]['weight'] = array(
      "#type" => 'weight',
      '#default_value' => $default_field['weight'],
    );

    if (isset($existing_fields[$field_name]['type']) && $existing_fields[$field_name]['type'] == 'nodereference') {
      $form['fields'][$field_name]['wallyedit'] = array(
        '#type' => 'select',
        '#options' => array(0 => t("Exclude"), 1 => t("Normal")),
      );
      $hits = 0;
      foreach ($existing_fields[$field_name]['referenceable_types'] as $key => $value) {
        if ($key === $value){
          $hits++;
        }
      }
      if ($existing_fields[$field_name]['multiple'] == 0 && $hits == 1) {
        $form['fields'][$field_name]["wallyedit"]["#options"][2] = t('Inline');
        $form['fields'][$field_name]["wallyedit"]["#options"][3] = t('Select box');
      }
      else {
        $form['fields'][$field_name]["wallyedit"]["#options"][3] = t('Select box');
      }

      $form['fields'][$field_name]['wallyedit']['#default_value'] = $default_field['display'];
    }

    $form['fields'][$field_name]['tab'] = array(
      "#type" => 'select',
      "#options" => $tabs_options,
      '#default_value' => $default_field['tid'],
    );
    
    unset($existing_fields[$field_name]);
  }

  foreach ($existing_fields as $field_name => $field) {
    $default_field = $field + array(
      'gid' => 0,
      'tid' => 0,
    );

    $form['fields'][$field_name]['label'] = array(
      '#type' => 'markup',
      '#value' => $default_field['label'],
    );

    $form['fields'][$field_name]['group'] = array(
      '#type' => 'select',
      '#options' => $groups_options,
      '#default_value' => $default_field['gid'],
    );

    $form['fields'][$field_name]['weight'] = array(
      "#type" => 'weight',
      '#default_value' => $default_field['weight'],
    );

    if (isset($field['type']) && $field['type'] == 'nodereference') {
      $form['fields'][$field_name]['wallyedit'] = array(
        '#type' => 'select',
        '#options' => array(0 => t("Exclude"), 1 => t("Normal")),
      );
      $hits = 0;
      foreach ($field['referenceable_types'] as $key => $value) {
        if ($key === $value){
          $hits++;
        }
      }
      if ($field['multiple'] == 0 && $hits == 1) {
        $form['fields'][$field_name]["wallyedit"]["#options"][2] = t('Inline');
        $form['fields'][$field_name]["wallyedit"]["#options"][3] = t('Select box');
      }
      else {
        $form['fields'][$field_name]["wallyedit"]["#options"][3] = t('Select box');
      }

      $form['fields'][$field_name]['wallyedit']['#default_value'] = $default_field['display'];
    }

    $form['fields'][$field_name]['tab'] = array(
      "#type" => 'select',
      "#options" => $tabs_options,
      '#default_value' => $default_field['tid'],
    );
  }

  $form['profile_id'] = array(
    '#type' => 'value',
    '#value' => $profile_id,
  );

  $form['type'] = array(
    '#type' => 'value',
    '#value' => $type_name,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#title' => t('Save'),
    '#value' => t('Save'),
  );

  return $form;
}

function wallyedit_form_fields_overview_form_validate ($form, &$form_state) {
  $values = $form_state['values'];
  $type_name = $values['type'];
  $profile_id = $values['profile_id'];
  $fields = $values['fields'];
  $existing_fields = wyditadmin_get_existing_fields($type_name);

  $required_fields = array();
  foreach ($values['fields'] as $field_name => $field) {
    if (!$field['tab'] && $existing_fields[$field_name]['required']) {
      $required_fields[] = $existing_fields[$field_name]['label'];
    }
  }
  
  if (!empty($required_fields)) {
    //form_set_error('', t('The following fields are required and are not placed in a tab : @fields', array('@fields' => implode(', ', $required_fields))));
    drupal_set_message(t('The following fields are required and are not placed in a tab : @fields. Default values (if set) will be used for these fields.', array('@fields' => implode(', ', $required_fields))), 'error');
  }
}

function wallyedit_form_fields_overview_form_submit ($form, &$form_state) {
  $values = $form_state['values'];
  $type_name = $values['type'];
  $profile_id = $values['profile_id'];
  $fields = $values['fields'];
  $existing_fields = wyditadmin_get_existing_fields($type_name);

  uasort($fields, '_wallyedit_sort_fields_helper');

  $base_config = array(
    'pid' => $profile_id,
    'content_type' => $type_name,
  );

  foreach ($fields as $field_name => $field_value) {
    if (isset($field_value['wallyedit'])) {
      $display_value = $field_value['wallyedit'];
    } else {
      if ($field_value['tab'] == 0) {
        $display_value = 0;
      } else {
        $display_value = 1;
      }
    }
    $field_to_save = $base_config + array(
      'tid' => $field_value['tab'],
      'gid' => $field_value['group'],
      'weight' => $field_value['weight'],
      'display' => $display_value,
    );

    $field_to_save = array_merge($field_to_save, isset($existing_fields[$field_name]) ? $existing_fields[$field_name] : array());
    if (isset($field_value['fid'])) {
      $field_to_save['fid'] = $field_value['fid'];
      drupal_write_record('wallyedit_fields', $field_to_save, 'fid');
    } else {
      drupal_write_record('wallyedit_fields', $field_to_save);
    }
  }
  drupal_set_message(t('Tabs and weight configuration saved'));
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

  $form['default_profile'] = array(
    '#type' => 'hidden',
    '#value' => '0',
  );

  return $form;
}

function wallyedit_form_add_new_profile_form_validate ($form, &$form_state) {
  $values = $form_state['values'];
  if ($values['label'] == '') {
    form_set_error('label', t('Empty label given!'));
  }
}

function wallyedit_form_add_new_profile_form_submit(&$form, &$form_state) {
  $values = $form_state['values'];

  drupal_write_record('wallyedit_profiles', $values);
  drupal_set_message(t('Profile "@profile" saved!', array('@profile' => $values['label'])));

  foreach (node_get_types('names') as $content_type => $ct_name) {
    $main_tab = array(
      'pid' => $values['pid'],
      'content_type' => $content_type,
      'name' => 'main_'.$values['pid'].'_'.$content_type,
      'label' => t('Main'),
      'weight' => 1,
      'erasable' => 0,
    );
    drupal_write_record('wallyedit_tabs', $main_tab);
    $meta_tab = array(
      'pid' => $values['pid'],
      'content_type' => $content_type,
      'name' => 'meta_'.$values['pid'].'_'.$content_type,
      'label' => t('Meta'),
      'weight' => 2,
      'erasable' => 0,
    );
    drupal_write_record('wallyedit_tabs', $meta_tab);
  }
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
    db_query('DELETE FROM {wallyedit_tabs} WHERE pid = %d', $profile_id);
    db_query('DELETE FROM {wallyedit_fields} WHERE pid = %d', $profile_id);
    drupal_set_message(t('Profile deleted.'));
  } else {
    drupal_set_message(t('No profile matching the requested key.'), 'warning');
  }
}

/**
 * Form to edit a group label
 */
function wallyedit_form_edit_label_group_form (&$form_state, $group_id) {
  $group = db_fetch_array(db_query('SELECT label FROM {wallyedit_groups} WHERE gid = %d', $group_id));

  $form = array();

  $form['new_label'] = array(
    '#type' => 'textfield',
    '#title' => t('New Label'),
    '#default_value' => $group['label'],
  );

  $form['group_id'] = array(
    '#type' => 'hidden',
    '#value' => $group_id,
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

function wallyedit_form_edit_label_group_form_validate ($form, &$form_state) {
  $values = $form_state['values'];
  if ($values['new_label'] == '') {
    form_set_error('new_label', t('Empty label given!'));
  }
}

function wallyedit_form_edit_label_group_form_submit ($form, $form_state) {
  $values = $form_state['values'];
  if ($values['op'] == $values['submit']) {
    db_query('UPDATE {wallyedit_groups} SET label = "%s" WHERE gid = %d', $values['new_label'], $values['group_id']);
  }
  if (!isset($_GET['destination'])) {
    drupal_goto('admin/content');
  }
}

/**
 * Form to delete a group
 */
function wallyedit_form_delete_group_form (&$form_state, $group_id) {
  $redirect = isset($_GET['destination']) ? $_GET['destination'] : 'admin/content';
  
  if ($group = db_fetch_array(db_query('SELECT label FROM {wallyedit_groups} WHERE gid = %d', $group_id))) {
    
  } else {
    drupal_set_message(t('Group not found!'), 'error');
    drupal_goto($redirect);
  }
  $question = t('Are you sure you want to delete group "@group"?', array('@group' => $group['label']));
  
  $add_form = array();
  $add_form['group_id'] = array(
    '#type' => 'value',
    '#value' => $group_id,
  );
  
  $redirect = isset($_GET['destination']) ? $_GET['destination'] : 'admin/content';
  $form = confirm_form($add_form, $question, $redirect);
  $form['#redirect'] = $redirect;
  return $form;
}

function wallyedit_form_delete_group_form_submit(&$form, &$form_state) {
  $values = $form_state['values'];
  $group_id = $values['group_id'];

  if ($group = db_result(db_query('SELECT * FROM {wallyedit_groups} WHERE gid = %d', $group_id))) {
    db_query('DELETE FROM {wallyedit_groups} WHERE gid = %d', $group_id);
    drupal_set_message(t('Group deleted.'));
  } else {
    drupal_set_message(t('No group matching the requested key.'), 'warning');
  }
}

/**
 * Set the default profile for all users if no others is chosen
 */
function wallyedit_page_set_default_profile ($profile_id) {
  db_query('UPDATE {wallyedit_profiles} SET `default_profile` = 0');
  db_query('UPDATE {wallyedit_profiles} SET `default_profile` = 1 WHERE pid = %d', $profile_id);
  $redirect = isset($_GET['destination']) ? $_GET['destination'] : 'admin/content';
  drupal_goto($redirect);
}

/**
 * Form to edit a tab label
 */
function wallyedit_form_edit_label_tab_form (&$form_state, $tab_id) {
  $tab = db_fetch_array(db_query('SELECT label FROM {wallyedit_tabs} WHERE tid = %d', $tab_id));

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

function wallyedit_form_edit_label_tab_form_validate ($form, &$form_state) {
  $values = $form_state['values'];
  if ($values['new_label'] == '') {
    form_set_error('new_label', t('Empty label given!'));
  }
}

function wallyedit_form_edit_label_tab_form_submit ($form, $form_state) {
  $values = $form_state['values'];
  if ($values['op'] == $values['submit']) {
    db_query('UPDATE {wallyedit_tabs} SET label = "%s" WHERE tid = %d', $values['new_label'], $values['tab_id']);
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
  
  if ($tab = db_fetch_array(db_query('SELECT label FROM {wallyedit_tabs} WHERE tid = %d', $tab_id))) {
    
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

  if ($erasable = db_result(db_query('SELECT erasable FROM {wallyedit_tabs} WHERE tid = %d', $tab_id))) {
    if ($erasable) {
      db_query('DELETE FROM {wallyedit_tabs} WHERE tid = %d', $tab_id);
      drupal_set_message(t('Tab deleted.'));
    } else {
      drupal_set_message(t('This tab can\'t be deleted.'), 'warning');
    }
  } else {
    drupal_set_message(t('No tab matching the requested key.'), 'warning');
  }
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

