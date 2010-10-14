<?php
// $Id: yaxim.admin.inc,v 1.0 2010/08/23 14:39:00 rso Exp $

/**
 * @file
 *   Import Press Content to Drupal structure Administration
 */

/**
 * Settings form.
 */
function wallymport_page_admin_form($form_state) {
  $path = drupal_get_path('module', 'wallymport');
  $form['wallymport_source'] = array(
    '#type' => 'textfield',
    '#title' => t('Import source directory path'),
    '#description' => t('This must be an existing and writable path where input zipped data is received.'),
    '#default_value' => variable_get('wallymport_source', 'sites/default/files/import'),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => FALSE,
  );
  $form['wallymport_definition'] = array(
    '#type' => 'textfield',
    '#title' => t('Import Data Structure Definition file'),
    '#description' => t('XSD (schema) file name, with the path.'),
    '#default_value' => variable_get('wallymport_definition', $path.'/definitions/packages.xsd'),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
   $form['wallymport_temp'] = array(
    '#type' => 'textfield',
    '#title' => t('Import temporary directory path'),
    '#description' => t('This must be an existing and writable path where input zipped data will be unpacked.'),
    '#default_value' => variable_get('wallymport_temp', file_directory_temp()),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => FALSE,
  );
  $form['wallymport_debug'] = array(
    '#type' => 'radios',
    '#title' => t('Import debug level'),
    '#description' => t('When creating your own import definition file, if you set this value to <em>No data creation</em>, no nodes, files, or locations will be written to the database. If you set this value to <em>Verbose</em> or higher, check html source of the import result for debug info.'),
    '#default_value' => variable_get('wallymport_debug', 0),
	'#options' => array(t('No debug'), t('Verbose'), t('No data creation')),
    '#required' => TRUE,
  );
  return system_settings_form($form);
}

/**
 * Validate function for the settings form.
 */
function wallymport_page_admin_form_validate($form, &$form_state) {
  
  $path = $form_state['values']['wallymport_source'];
  $dir_exist=file_check_directory($path,0);	// IN D7 this function becomes file_prepare_directory()
  if (!$dir_exist) {
    form_set_error('wallymport_source', t('The path provided is not an existing writable directory.'));
  }
  
  $file = $form_state['values']['wallymport_definition'];
  $file_readable=is_readable($file);
  if (!$file_readable) {
    form_set_error('wallymport_definition', t('The filename provided is not an existing readable file.'));
  }

  
}

