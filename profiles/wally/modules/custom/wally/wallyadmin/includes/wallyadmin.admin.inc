<?php 
// $Id: yaxim.admin.inc,v 1.0 2010/08/23 14:39:00 rso Exp $

/**
 * @file
 *   Import Press Content to Drupal structure Administration
 */

/**
 * General settings form.
 */
function wallyadmin_general_settings_form($form_state) {
  $form = array();

  $form['wallyadmin_product'] = array(
    '#type' => 'textfield',
    '#title' => t('ProductID'),
    '#description' => t('The product of the website, it must be unique.'),
    '#default_value' => wally_variable_get('wallyadmin_product', ''),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );

  $form['wallyadmin_environment'] = array(
    '#type' => 'textfield',
    '#title' => t('Environment'),
    '#description' => t('Stage, Prod, Dev, etc.'),
    '#default_value' => wally_variable_get('wallyadmin_environment', ''),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  
  return wally_system_settings_form($form, TRUE);
}