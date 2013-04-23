<?php
/**
* Callback function to supply a list of content types.
*/
function wallydashboard_statpackage_ctools_content_types() {

  return array(
    'single' => TRUE,
    'title' => t('Stat package'),
    'category' => t('Dashboard'),
    'description' => t('Display package stat'),
    'defaults' => array(),
  );
}

function wallydashboard_statpackage_content_type_edit_form(&$form, &$form_state) {
  // The current configuration
  if ($modules = module_invoke_all('wallydashboard_statpackage')) {
    $form['statpackage']['#tree'] = TRUE;
    foreach ($modules as $module_name => $stat_packages){
      foreach ($stat_packages as $stat_pack_name => $stat_pack_info){
        $default_value = 1;
        if ($form_state['conf']['statpackage'][$module_name.'_'.$stat_pack_name]['status'] != NULL){
          $default_value = $form_state['conf']['statpackage'][$module_name.'_'.$stat_pack_name]['status'];
        }
        $form['statpackage'][$module_name.'_'.$stat_pack_name]['status'] = array(
          '#type' => 'select',
          '#title' => $stat_pack_info['title'],
          '#options' => array(t('Disabled'), t('Enabled')),
          '#default_value' => $default_value,
        );
        $default_value = $stat_pack_info['default_operator'];
        if ($form_state['conf']['statpackage'][$module_name.'_'.$stat_pack_name]['operator'] != NULL){
          $default_value = $form_state['conf']['statpackage'][$module_name.'_'.$stat_pack_name]['operator'];
        }
        $form['statpackage'][$module_name.'_'.$stat_pack_name]['operator'] = array(
          '#type' => 'select',
          '#options' => array('<' => '<', '>' => '>'),
          '#default_value' => $default_value,
        );
        $default_value = $stat_pack_info['default_value'];
        if ($form_state['conf']['statpackage'][$module_name.'_'.$stat_pack_name]['status'] != NULL){
          $default_value = $form_state['conf']['statpackage'][$module_name.'_'.$stat_pack_name]['value'];
        }
        $form['statpackage'][$module_name.'_'.$stat_pack_name]['value'] = array(
          '#type' => 'textfield',
          '#default_value' => $default_value,
          '#description' => $stat_pack_info['units'],
        );
        $form['statpackage'][$module_name.'_'.$stat_pack_name]['function'] = array(
          '#type' => 'value',
          '#value' => $stat_pack_info['function'],
        );
        $form['statpackage'][$module_name.'_'.$stat_pack_name]['title'] = array(
          '#type' => 'value',
          '#value' => $stat_pack_info['title'],
        );
      }
    }
  } else {
    drupal_set_message('error', t('There is no plugin for this content type'));
  }
}

function wallydashboard_statpackage_content_type_edit_form_submit(&$form, &$form_state) {
  // For each part of the form defined in the 'defaults' array set when you
  // defined the content type, copy the value from the form into the array
  // of items to be saved. We don't ever want to use
  // $form_state['conf'] = $form_state['values'] because values contains
  // buttons, form id and other items we don't want stored. CTools will handle
  // the actual form submission.
  foreach ($form_state['values']['statpackage'] as $key => $value){
    $form_state['conf']['statpackage'][$key] = $value;
  }
}

/**
* Output function for the 'shortcut' content type.
*/
function wallydashboard_statpackage_content_type_render($subtype, $conf, $panel_args, $context,$a) {

  $block->title = t('Stat Package');
  $requirements = array();
  foreach ($conf['statpackage'] as $key => $value) {
    if ($value['status'] == 1){
      if (function_exists($value['function'])){
        $result = call_user_func($value['function']);
        $severity = 0;
        if ($value['operator'] == '<' ){
          if ($result['value'] < $value['value']){
            $severity = 2;
          } 
        } elseif ($value['operator'] ==  '>'){
          if ($result['value'] > $value['value']){
            $severity = 2;
          }
        }
        $requirements[] = array(
          'title'    => $value['title'],
          'value'    => $result['display'],
          'severity' => $severity, 
        );
      }
    }
  }
  $block->content = theme('wallydashboard_status', $requirements);
  return $block;
}

