<?php 

/**
* Page settings form
* */
function cpsprint_page_settings_form(){
  $form['cpsprint_customer'] = array(
    '#type' => 'textfield',
    '#title' => t('Customer'),
    '#description' => t('The name of the customer for CPS Print'),
    '#default_value' => wally_variable_get('cpsprint_customer', ''),
    '#required' => TRUE,
  );
  $form['cpsprint_pub'] = array(
    '#type' => 'textfield',
    '#title' => t('Pub'),
    '#description' => t('The name of the Paper'),
    '#default_value' => wally_variable_get('cpsprint_pub', ''),
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}
function cpsprint_page_settings_form_submit($form, &$form_state){
  wally_variable_set('cpsprint_customer', $form_state['values']['cpsprint_customer']);
  wally_variable_set('cpsprint_pub', $form_state['values']['cpsprint_pub']);
}