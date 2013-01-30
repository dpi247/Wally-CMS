<?php 

/**
* Page settings form
* */
function cpsprint_page_settings_form(){
  $form['cpsprint_customer'] = array(
    '#type' => 'textfield',
    '#title' => t('Customer'),
    '#description' => t('The name of the customer for CPS Print'),
    '#default_value' => variable_get('cpsprint_customer', ''),
    '#required' => TRUE,
  );
  $form['cpsprint_pub'] = array(
    '#type' => 'textfield',
    '#title' => t('Pub'),
    '#description' => t('The name of the Paper'),
    '#default_value' => variable_get('cpsprint_pub', ''),
    '#required' => TRUE,
  );
  return wally_system_settings_form($form);
}