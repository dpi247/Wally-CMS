<?php
/**
* Callback function to supply a list of content types.
*/
function wallydashboard_shortcut_ctools_content_types() {

  return array(
    'single' => TRUE,
    'title' => t('Shortcut'),
    'category' => t('Dashboard'),
    'description' => t('Display shortcuts.'),
    'defaults' => array(),
  );
}

function wallydashboard_shortcut_content_type_edit_form(&$form, &$form_state) {
  // The current configuration
  if ($modules = module_invoke_all('wallydashbord_shortcut')) {
    $options = array();
    foreach ($modules as $module_name => $shortcuts){
      foreach ($shortcuts as $shortcut_name => $shortcut_info){
        $options[$module_name.'_'.$shortcut_name] = $shortcut_info['title'];
      }
    }
    $form['shortcuts'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Choose your shorcuts'),
      '#options' => $options,
      '#default_value' => $form_state['conf']['shortcuts'],
    );
  } else {
    drupal_set_message('error', t('There is no plugin for this content type'));
  }
}

function wallydashboard_shortcut_content_type_edit_form_submit(&$form, &$form_state) {
  // For each part of the form defined in the 'defaults' array set when you
  // defined the content type, copy the value from the form into the array
  // of items to be saved. We don't ever want to use
  // $form_state['conf'] = $form_state['values'] because values contains
  // buttons, form id and other items we don't want stored. CTools will handle
  // the actual form submission.
  foreach ($form_state['values']['shortcuts'] as $key => $value){
    if ($value !== 0){
      $form_state['conf']['shortcuts'][$key] = $key;
    }
  }
}

/**
* Output function for the 'shortcut' content type.
*/
function wallydashboard_shortcut_content_type_render($subtype, $conf, $panel_args, $context,$a) {
dsm($conf);
  if ($modules = module_invoke_all('wallydashbord_shortcut')) {
    
    $shortcuts_list = array();
    foreach ($modules as $module_name => $shortcuts) {
      foreach ($shortcuts as $shortcut_name => $shortcut_info){
        if (array_key_exists($module_name.'_'.$shortcut_name, $conf['shortcuts'])){
          $shortcuts_list[] = $shortcut_info;
        }
      }
    }
    $block->title = t('Shortcut');
    $block->content = theme('wallydashboard_shortcut', $shortcuts_list);
  }
  
  return $block;
}

