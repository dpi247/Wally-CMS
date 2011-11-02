<?php
// $Id: yaxim.admin.inc,v 1.0 2010/08/23 14:39:00 rso Exp $

/**
 * @file
 *   Import Press Content to Drupal structure Administration
 */

/**
 * Settings form.
 */
function wallydam_page_admin_form($form_state) {
 
  
  $form['wallydam_updateurl'] = array(
    '#type' => 'textfield',
    '#title' => t('URL to send the informations of the node'),
    '#description' => t('URL to send the information of the node.'),
    '#default_value' => variable_get('wallydam_updateurl', 'http://esb2.rossel.be:8081/rest/pandamServices/updateUrl'),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  $form['wallydam_updatepackageurl'] = array(
    '#type' => 'textfield',
    '#title' => t('URL to send the updated packages'),
    '#description' => t('URL to send the updated package.'),
    '#default_value' => variable_get('wallydam_updatepackageurl', ''),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  $form['wallydam_cronmaxnumber'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum number of packages sent in a post'),
    '#description' => t('Maximum number of packages sent in a post'),
    '#default_value' => variable_get('wallydam_cronmaxnumber', 50),
    '#size' => 80,
    '#maxlenght' => 512,
    '#required' => TRUE,
  );
  $form['wallydam_wallyprefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Wally Prefix'),
    '#description' => t('Used in Pandam to identified de source web site'),
    '#default_value' => variable_get('wallydam_wallyprefix', ''),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  return system_settings_form($form);
}

/**
 * Validate function for the settings form.
 */
function wallydam_page_admin_form_validate($form, &$form_state) {
  
  
  if (!is_numeric($form_state['values']['wallydam_cronmaxnumber'])){
    form_set_error('wallydam_cronmaxnumber', t('The maximum number of packages must be a numeric value'));
  }
}