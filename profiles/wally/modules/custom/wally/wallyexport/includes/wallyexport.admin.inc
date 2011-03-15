<?php
// $Id: yaxim.admin.inc,v 1.0 2010/08/23 14:39:00 rso Exp $

/**
 * @file
 *   Export Press Content to Drupal structure Administration
 */

/**
 * Settings form.
 */
function wallyexport_page_admin_form($form_state) {
  $form['wallyexport_admin_dest'] = array(
    '#type' => 'textfield',
    '#title' => t('Export destination directory path'),
    '#description' => t('This must be an existing and writable path where zipped data is written.'),
    '#default_value' => variable_get('wallyexport_admin_source', 'sites/default/files/export'),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  
  return system_settings_form($form);
}

/**
 * Validate function for the settings form.
 */
function wallyexport_page_admin_form_validate($form, &$form_state) {
  $path = $form_state['values']['wallyexport_admin_dest'];
  $dir_exist=file_check_directory($path,0);
  if (!$dir_exist) {
    form_set_error('wallyexport_admin_dest', t('The path provided is not an existing writable directory.'));
  }
}

/**
 * Export packages form.
 */
function wallyexport_page_export_form($form_state) {
  $form['wallyexport_export_nids'] = array(
    '#type' => 'textfield',
    '#title' => t('Packages\' nids to export'),
    '#description' => t('Comma separated nids of the packages to be exported. The keyword "all" is accepted.'),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  
  $form['wallyexport_export_submit'] = array(
    '#type' => 'submit',
    '#title' => t('Export'),
    '#value' => t('Export'),
  );
  
  return $form;
}

/**
 * Validate function for the export packages form.
 * 
 * @todo
 *   Check if the validation doesn't take to much time.
 */
function wallyexport_page_export_form_validate(&$form, &$form_state) {
  $nids_list = $form_state['values']['wallyexport_export_nids'];
  if (strtolower($nids_list) != 'all') {
    $nids = explode(',', $nids_list);
    foreach ($nids as $nid) {
      $node = node_load($nid);
      if ($node->type != 'wally_articlepackage' &&
          $node->type != 'wally_gallerypackage' &&
          $node->type != 'wally_pollpackage')
        form_set_error('wallyexport_export_nids', t('The node '.$nid.' isn\'t a package.'));
    }
  }
}

/**
 * Submit function for the export packages form.
 */
function wallyexport_page_export_form_submit(&$form, &$form_state) {
  $nids_list = $form_state['values']['wallyexport_export_nids'];
  if (strtolower($nids_list) == 'all') {
    $nids = array();
    $result = db_query('SELECT `nid` FROM {node} WHERE `type` = "wally_articlepackage" OR `type` = "wally_gallerypackage" OR `type` = "wally_pollpackage"');
    while ($res = db_fetch_object($result)) {
      $nids[] = $res->nid;
    }
  } else {
    $nids = explode(',', $nids_list);
  }

  wallyexport_export_packages($nids);
}
