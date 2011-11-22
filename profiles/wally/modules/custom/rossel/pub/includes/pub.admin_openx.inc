<?php
// $Id: pub.admin.inc,v 1.0 2011/02/10 ODM $
 
function pub_page_admin_openx() {
  $return = '';
  $return .= drupal_get_form('pub_page_admin_openx_form');
  
  return $return;
}

/**
 * Settings form.
 */
function pub_page_admin_openx_form($form_state) {  
  $form['openx_website'] = array(
    '#type' => 'textfield',
    '#title' => t('Website name'),
    '#description' => t('The website name used to the pub requests.'),
    '#default_value' => variable_get('pub_openx_website', ''),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  
  $form['openx_provider'] = array(
    '#type' => 'textfield',
    '#title' => t('Ads provider'),
    '#description' => t('The ads provider (Examples : AdTech, OpenX, AOS).'),
    '#default_value' => variable_get('pub_openx_provider', 'OpenX'),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  
  $form['openx_get_xml'] = array(
    '#type' => 'textfield',
    '#title' => t('URL used for getting blocks informations'),
    '#description' => t('The URL used for getting blocks informations.'),
    '#default_value' => variable_get('pub_openx_get_xml', 'http://gps1.intranet.rossel/private/script/ZZsqls.cgi?key=dumpTagsAdtech&format=xml'),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  
  $form['openx_webserv'] = array(
    '#type' => 'textfield',
    '#title' => t('URL of the webservice returning the pubs'),
    '#description' => t('The URL of the webservice returning the pubs.'),
    '#default_value' => variable_get('openx_pub_webserv', 'http://172.21.132.180:8080/Wally-Tags/rest/wally/script/detail'),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  
  $form['openx_header'] = array(
    '#type' => 'textfield',
    '#title' => t('URL of the webservice returning the header javascript content'),
    '#default_value' => variable_get('pub_openx_header', ''),
    '#size' => 80,
    '#maxlength' => 512,
  );
  
  $form['pub_submit'] = array(
    '#type' => 'submit',
    '#title' => t('Save'),
    '#value' => t('Save'),
  );
  
  return $form;
}

/**
 * Submit function for the settings form.
 */
function pub_page_admin_openx_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  variable_set('pub_openx_website', $values['openx_website']);
  variable_set('pub_openx_provider', $values['openx_provider']);
  variable_set('pub_openx_get_xml', $values['openx_get_xml']);
  variable_set('pub_openx_webserv', $values['openx_webserv']);
  variable_set('pub_openx_header', $values['openx_header']);
  drupal_set_message('Configuration saved', 'openx_notice');
}

/**
 * HTML code for the pub blocks admin page.
 */
function pub_blocks_admin_openx() {
  $return = '';
  $return .= drupal_get_form('pub_blocks_admin_openx_form');
  
  return $return;
}
