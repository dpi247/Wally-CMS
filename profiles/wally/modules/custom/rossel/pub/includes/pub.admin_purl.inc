<?php
// $Id: pub.admin.inc,v 1.0 2011/02/10 ODM $

/**
 * Settings form.
 */
function pub_page_admin_purl_form($form_state) {  
  $form['purl_website'] = array(
    '#type' => 'textfield',
    '#title' => t('Website name'),
    '#default_value' => variable_get('pub_purl_website', ''),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  
  $form['purl_provider'] = array(
    '#type' => 'textfield',
    '#title' => t('Ads provider'),
    '#description' => t('The ads provider (Examples : AdTech, OpenX, AOS).'),
    '#default_value' => variable_get('pub_purl_provider', 'Purl'),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  
  /*$form['purl_get_xml'] = array(
    '#type' => 'textfield',
    '#title' => t('URL used for getting blocks informations'),
    '#default_value' => variable_get('pub_purl_get_xml', 'http://gps1.intranet.rossel/private/script/ZZsqls.cgi?key=dumpTagsAdtech&format=xml'),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );*/
  
  $form['purl_webserv'] = array(
    '#type' => 'textfield',
    '#title' => t('URL of the webservice returning the ads'),
    '#default_value' => variable_get('pub_purl_webserv', 'http://172.21.132.180:8080/Wally-Tags/rest/wally/script/detail'),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  
  $form['purl_header'] = array(
    '#type' => 'textfield',
    '#title' => t('URL of the webservice returning the header javascript content'),
    '#default_value' => variable_get('pub_purl_header', 'http://172.21.132.180:8080/Wally-Tags/rest/wally/script/header'),
    '#size' => 80,
    '#maxlength' => 512,
  );
  
  $form['purl_meta_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name attribute for the meta tag'),
    '#default_value' => variable_get('pub_purl_meta_name', 'adpurl-keywords'),
    '#size' => 80,
    '#maxlength' => 512,
  );
  
  /*$form['purl_meta_id'] = array(
    '#type' => 'textfield',
    '#title' => t('ID attribute for the meta tag'),
    '#default_value' => variable_get('pub_purl_meta_name', 'adpurl-keywords'),
    '#size' => 80,
    '#maxlength' => 512,
  );*/
  
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
function pub_page_admin_purl_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  variable_set('pub_purl_website', $values['purl_website']);
  variable_set('pub_purl_provider', $values['purl_provider']);
  //variable_set('pub_purl_get_xml', $values['purl_get_xml']);
  variable_set('pub_purl_webserv', $values['purl_webserv']);
  variable_set('pub_purl_header', $values['purl_header']);
  variable_set('pub_purl_meta_name', $values['purl_meta_name']);
  //variable_set('pub_purl_meta_id', $values['purl_meta_id']);
  drupal_set_message(t('Configuration saved'), 'purl_notice');
}
