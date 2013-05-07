<?php
// $Id: yaxim.admin.inc,v 1.0 2010/08/23 14:39:00 rso Exp $

/**
 * @file
 *   Import Press Content to Drupal structure Administration
 */

/**
 * Packages synchronization settings form.
 */
function wallydam_page_admin_packages_sync_form($form_state) {
  $form = array();

  $form['wallydam_updateurl'] = array(
    '#type' => 'textfield',
    '#title' => t('URL to send the informations of the node'),
    '#description' => t('URL to send the information of the node.'),
    '#default_value' => wally_variable_get('wallydam_updateurl', 'http://esb2.rossel.be:8081/rest/pandamServices/updateUrl'),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );

  $form['wallydam_updatepackageurl'] = array(
    '#type' => 'textfield',
    '#title' => t('URL to send the updated packages'),
    '#description' => t('URL to send the updated package.'),
    '#default_value' => wally_variable_get('wallydam_updatepackageurl', ''),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );

  $form['wallydam_cronmaxnumber'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum number of packages sent in a post'),
    '#description' => t('Maximum number of packages sent in a post'),
    '#default_value' => wally_variable_get('wallydam_cronmaxnumber', 50),
    '#size' => 80,
    '#maxlenght' => 512,
    '#required' => TRUE,
  );

  $form['wallydam_wallyprefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Wally Prefix'),
    '#description' => t('Used in Pandam to identified de source web site'),
    '#default_value' => wally_variable_get('wallydam_wallyprefix', ''),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );

  return wally_system_settings_form($form, TRUE);
}

/**
 * Validate function for the settings form.
 */
function wallydam_page_admin_packages_sync_form_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['wallydam_cronmaxnumber'])){
    form_set_error('wallydam_cronmaxnumber', t('The maximum number of packages must be a numeric value'));
  }
}

/**
 * DAM Search settings form
 */
function wallydam_page_admin_search_form($form_state) {
  $form = array();
  print '<pre>';
  module_load_include('inc', 'wallydam', 'includes/DAMSearch');
  $search = new DAMSearch();
  print_r($search);
  $search_query = $search->getSearchQuery();
  $search_query->addReqWord('di rupo');
  $search_query->setOption('Newsroom', array('Sudpresse'));
  $search->executeQuery();
  print_r($search);
  print '</pre>';
  
  return $form;
}

function wallydam_page_admin_archive_form(){
  $form = array();
  $options = array(FALSE => 'OFF', TRUE => 'ON');
  $form['wallydam_archive_on'] = array(
    '#type' => 'select',
    '#title' => t('Active archive retrieving'),
    '#default_value' => wally_variable_get('wallydam_archive_on', FALSE),
    '#options' => $options,
    '#required' => TRUE,
  );
  
  $importer = wallymport_getimporters();
  $options = array('' => '');
  foreach ($importer as $imp){
    $options[$imp->name] = $imp->name;
  }
  $form['wallydam_archive_importer'] = array(
    '#type' => 'select',
    '#title' => t('Importer to use to extract package'),
    '#default_value' => wally_variable_get('wallydam_archive_importer', ''),
    '#options' => $options,
    '#required' => TRUE,
  );
  
  $form['wallydam_archive_geturl'] = array(
    '#type' => 'textfield',
    '#title' => t('URL of the DAM to retrieve archive'),
    '#default_value' => wally_variable_get('wallydam_archive_geturl', ''),
    '#require' => TRUE,
  );
  return wally_system_settings_form($form, TRUE);
}