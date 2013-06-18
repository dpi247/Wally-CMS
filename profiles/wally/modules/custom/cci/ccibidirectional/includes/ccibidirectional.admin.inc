<?php 

function ccibidirectional_form_configuration_form($form_state){
  $form = array();

  $form['ccibidirectional_lock_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable CCI lock'),
    '#default_value' => wally_variable_get('ccibidirectional_lock_enabled', 0),
  );

  $form['ccibidirectional_ws_wsdluri'] = array(
    '#type' => 'textfield',
    '#title' => t('CCI Web WSDL\'s URI'),
    '#default_value' => wally_variable_get('ccibidirectional_ws_wsdluri', ''),
  );

  return wally_system_settings_form($form, TRUE);
}
