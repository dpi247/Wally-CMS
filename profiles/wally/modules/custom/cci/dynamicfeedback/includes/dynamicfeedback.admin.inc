<?php

function dynamicfeedback_form_admin_form($form_state) {
  $form = array();

  $options = module_invoke_all('dynamicfeedback_options', 'list');
  
  $form['dynamicfeedback_display_options'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Choose informations to display'),
    '#options' => $options,
    '#default_value' => wally_variable_get('dynamicfeedback_display_options', array()),
  );
  
  return wally_system_settings_form($form, TRUE);
}
