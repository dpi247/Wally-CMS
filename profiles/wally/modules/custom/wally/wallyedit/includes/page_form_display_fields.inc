<?php

function wallyedit_node_overview_page () {
  $types = node_get_types();
  $names = node_get_types('names');
  $header = array(t('Name'), t('Type'), t('Description'), array('data' => t('Operations'), 'colspan' => '2'));
  $rows = array();

  foreach ($names as $key => $name) {
    $type = $types[$key];
    if (node_hook($type, 'form')) {
      $type_url_str = str_replace('_', '-', $type->type);
      $row = array(
        l($name, 'admin/content/node-type/'. $type_url_str),
        check_plain($type->type),
        filter_xss_admin($type->description),
      );
      // Set the edit column.
      $row[] = array('data' => l(t('Display fields'), 'admin/content/wallydisplayfields/'. $type_url_str));

      $rows[] = $row;
    }
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No content types available.'), 'colspan' => '5', 'class' => 'message'));
  }
  
  return theme('table', $header, $rows);
}

function wallyedit_form_display_fields_form (&$form_state, $type) {
  module_load_include("inc",'content','includes/content.node_form');
  module_load_include("inc",'content','includes/content.admin');
  $form = drupal_retrieve_form('content_field_overview_form', $form_state, $type->type);

  $node = new Stdclass;
  $node->type = $type->type;
  $node->name = '';
  $node_form = drupal_retrieve_form(str_replace('-','_',$type->type).'_node_form', $form_state, $node);
  drupal_prepare_form('node_form', $node_form, $form_state);

  foreach ($form['#field_rows'] as $field_id => $field_name) {
    if ($field_name == '_add_new_field' || $field_name == '_add_existing_field') {
      unset($form['#field_rows'][$field_id]);
    } elseif (isset($node_form[$field_name]) && is_array($node_form[$field_name]) && $node_form[$field_name]['#type'] == 'fieldset') {
      _wallyedit_add_new_group_to_form($form, $node_form[$field_name], $field_name);
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

  /*$groups = array();
  if (module_exists('fieldgroup')) {
  $groups = fieldgroup_groups($type_name);
  }*/

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
      if ($fields[$field]['type']['multiple'] == 0 && $hits == 1) {
        $form[$field]["wallyedit"]["#options"]=array_merge($form[$field]["wallyedit"]["#options"],array(2=>t('Inline'),3=>t('Select box')));
      } else {
        $form[$field]["wallyedit"]["#options"]=array_merge($form[$field]["wallyedit"]["#options"],array(3=>t('Select box')));
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

  $form['#parameters'][0] = 'wallyedit_form_display_fields_form';

  $form['#field_rows'] = array_values($form['#field_rows']);

  return $form;
}

function wallyedit_form_display_fields_form_submit(&$form, &$form_state) {
  wallyedit_display_fields_save_groups($form, $form_state);
  wallyedit_display_fields_save_extra_fields($form, $form_state);
  foreach ($form_state['values'] as $field_name => $values)
    if (isset($values['field']['display_settings']))
      unset($form_state['values'][$field_name]['field']['display_settings']['field']);
  content_display_overview_form_submit($form, $form_state);
}

function wallyedit_display_fields_save_groups(&$form, &$form_state) {
  $form_values = $form_state['values'];
  $groups_to_save = array();
  
  foreach ($form_values as $key => $values) {
    if (in_array($key, $form['#group_rows'])) {
      $values['display_settings']['wallyedit'] = $values['wallyedit'];
      $groups_to_save[$key] = $values;
    }
  }

  variable_set('wallyedit_groups_display_'.$form['#type_name'], $groups_to_save);
}

function wallyedit_display_fields_save_extra_fields(&$form, &$form_state) {
  $form_values = $form_state['values'];
  $fields_to_save = array();

  foreach ($form_values as $key => $values) {
    // Groups are handled in fieldgroup_display_overview_form_submit().
    if (in_array($key, $form['#field_rows']) && !in_array($key, $form['#fields'])) {
      $values['display_settings']['wallyedit'] = $values['wallyedit'];
      $fields_to_save[$key] = $values;
    }
  }
  variable_set('wallyedit_extra_fields_display_'.$form['#type_name'], $fields_to_save);
}

function _wallyedit_add_new_group_to_form(&$form, $node_fields, $field_name) {
  $form[$field_name]['group_name']['#value'] = $field_name;
  $form[$field_name]['group_type']['#value'] = 'Standard group';
  $form['#group_rows'][] = $field_name;
  $field_id = array_keys($form['#field_rows'], $field_name, TRUE);
  unset($form['#field_rows'][$field_id[0]]);
  //$from['#groups'][] = $field_name;
  $form[$field_name]['#row_type'] = 'group';
  $form[$field_name]['#disabled_row'] = FALSE;
  
  foreach ($node_fields as $node_field_name => $node_field) {
    if (!strstr($node_field_name, '#')) {
      $form[$node_field_name] = array();
      $form['#field_rows'][] = $node_field_name;
      //$form['#fields'][] = 'field_'.$node_field_name;
      $form[$node_field_name]['label']['#value'] = $node_field['#title'];
      $form[$node_field_name]['field_name']['#value'] = $node_field_name;
      $form[$node_field_name]['type']['#value'] = $node_field['#type'];
      $form[$node_field_name]['weight']['#value'] = '10';
      $form[$node_field_name]['parent'] = array(
        '#type' => 'select',
        '#options' => array($field_name => $form[$field_name]['label']['#value']),
        '#default_value' => $field_name,
      );
      $form[$node_field_name]['prev_parent'] = array(
        '#type' => 'hidden',
        '#value' => $field_name,
      );
      $form[$node_field_name]['#leaf'] = TRUE;
      $form[$node_field_name]['#row_type'] = 'field';
      $form[$node_field_name]['field'] = array(
        '#type' => 'value',
        '#value' => array(
          'field_name' => $node_field_name,
      		'type_name' => 'field',
          'type' => $node_field['#type'],
          'multiple' => 0,
        ),
      );
    }
  }
}
