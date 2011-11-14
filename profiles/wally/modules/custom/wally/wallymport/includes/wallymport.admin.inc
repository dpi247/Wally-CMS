<?php
// $Id: yaxim.admin.inc,v 1.0 2010/08/23 14:39:00 rso Exp $

/**
 * @file
 *   Import Press Content to Drupal structure Administration
 */



/**
 * Settings form.
 */
function wallymport_page_links_form($form_state){
  $form = array();
  $dirs[] = variable_get('wallymport_source', 'sites/default/files/import');
  $dirs[] = variable_get('wallymport_import_error', 'sites/default/files/import/error');
  $dirs[] = variable_get('wallymport_import_done', 'sites/default/files/import/done');
  foreach($dirs as $dir){
    if(file_check_directory($dir, 0)){
      $form[$dir] = array(
        '#type' => 'fieldset',
        '#title' => t($dir),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );
      $files = file_scan_directory($dir, '.*\.zip', array('.', '..', 'CVS'), 0, FALSE);
      foreach($files as $file){
        $form[$dir][$file->basename]=array(
          "#type"=>'markup',
          "#value"=>'<a href="'.file_create_url($file->filename).'">'.$file->basename.'</a><br />',
        );
      }
    }
  }
  return $form;
}
/**
 * Create/update importer form.
 */
function wallymport_page_importer_form($form_state, $edit_id = NULL){
  ctools_include('dependent');
  ctools_add_js('dependent');
  if ($edit_id != NULL){
    wallymport_getimporter($edit_id);
  }
  $form['wallymport']['#tree'] = TRUE;
  
  $modules = module_invoke_all('wallymport_importer');
  $importers_name = array();
  foreach ($modules as $importers){
    foreach ($importers as $importer){
      $importers_name[$importer['importer_type']] = $importer['importer_type'];
    }
  }
  $form['wallymport']['importer_type'] = array(
    '#type' => 'select',
    '#title' => t('Importer'),
    '#description' => t('Select the importer'),
    '#options' => $importers_name,
    '#required' => TRUE,
  );
  $form['wallymport']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('The unique Name of the importer.'),
    '#size' => 80,
    '#maxlength' => 100,
    '#required' => TRUE,
  );
  $form['wallymport']['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#description' => t(''),
    '#size' => 80,
    '#maxlength' => 1000,
  );
  $form['wallymport']['source_directory_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Import source directory path'),
    '#description' => t('This must be an existing and writable path where input zipped data is received.'),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  
  $form['wallymport']['done_directory_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Imported packages directory path'),
    '#description' => t('This must be an existing and writable path where zipped processed packages are moved after import.'),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  
  $form['wallymport']['error_directory_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Not imported packages (with errors) directory path'),
    '#description' => t('This must be an existing and writable path where zipped packages which could not be imported are moved after import.'),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  
  $form['wallymport']['temp_directory_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Import temporary directory path'),
    '#description' => t('This must be an existing and writable path where input zipped data will be unpacked.'),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  $form['wallymport']['xsd_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Import Data Structure Definition file'),
    '#description' => t('XSD (schema) file name, with the path.'),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  $form['wallymport']['zip'] = array(
    '#type' => 'select',
    '#title' => t('Are the data to import zipped?'),
    '#description' => t('Indicates if the data to import are zipped'),
    '#options' => array('' => '', 0 => t('No'), 1 => t('Yes')),
    '#required' => TRUE,
  );
  $form['wallymport']['processed_by_post'] = array(
    '#type' => 'select',
    '#title' => t('Are the data imported during POST?'),
    '#description' => t('Indicates if the data are processed during the POST?'),
    '#options' => array('' => '', 0 => t('No'), 1 => t('Yes')),
    '#required' => TRUE,
  );
   $form['wallymport']['processed_by_cron'] = array(
    '#type' => 'select',
    '#title' => t('Are the data imported by a CRON job?'),
    '#description' => t('Indicates if the data are processed by a CRON job?'),
    '#options' => array('' => '', 0 => t('No'), 1 => t('Yes')),
    '#required' => TRUE,
  );
  $form['wallymport']['cron_frequency'] = array (
    '#type'             => 'textfield',
    '#title'            => t('CRON frequency'),
    '#description'      => t('CRON frequency (in minutes)'),
    '#required'         => FALSE,
    '#dependency'       => array('edit-wallymport-processed-by-cron' => array(1)),
    '#process'          => array('ctools_dependent_process'),
  );
  $form['wallymport']['cron_keep_log'] = array (
    '#type'             => 'select',
    '#title'            => t('Keep CRON logs?'),
    '#options' => array(t('No'),t('Yes')),
    '#required'         => FALSE,
    '#dependency'       => array('edit-wallymport-processed-by-cron' => array(1)),
    '#process'          => array('ctools_dependent_process'),
  );
  $form['wallymport']['default_user'] = array(
    '#type' => 'textfield',
    '#title' => t('Drupal user used for imports.'),
    '#description' => t('This user will be used as author for creating content.'),
    '#size' => 3,
    '#maxlength' => 8,
    '#required' => TRUE,
  );
  $form['buttons']['submit'] = array('#type' => 'submit', '#value' => t('Save') );
  
  if ($edit_id != NULL){
    $importer = wallymport_getimporter($edit_id);
    $form['wallymport']['iid'] = array('#type' => 'value',);
    foreach ($form['wallymport'] as $key => $value){
      if ($key != '#tree' & $key != 'cron_frequency') {
        $form['wallymport'][$key]['#default_value'] = $importer->$key;
      } elseif ($key = 'cron_frequency'){
        $form['wallymport'][$key]['#default_value'] = $importer->$key / 60;
      }
    }
    $form['delete'] = array(
      '#type' => 'markup',
      '#value' => '<a href = "/admin/wally/wallymport/settings/delete/'.$importer->iid.'">'.t('Delete').'</a>',
    );
    $form['wallymport']['name']['#disabled'] = TRUE;
    $form['wallymport']['name']['#value'] = $importer->name;
    $form['wallymport']['importer_name']['#disabled'] = TRUE;
    $form['wallymport']['importer_name']['#value'] = $importer->importer_name;
    
  }
  return $form;
}
/*
 * Validation of createimporter_form 
 */
function wallymport_page_importer_form_validate($form, &$form_state){
 
  $path = $form_state['values']['wallymport']['source_directory_path'];
  $dir_exist = file_check_directory($path,0);	// IN D7 this function becomes file_prepare_directory()
  if (!$dir_exist) {
    form_set_error('wallymport][source_directory_path', t('Source directory path : The path provided is not an existing writable directory.'));
  }
  $path = $form_state['values']['wallymport']['done_directory_path'];
  $dir_exist = file_check_directory($path,0);	// IN D7 this function becomes file_prepare_directory()
  if (!$dir_exist) {
    form_set_error('wallymport][done_directory_path', t('Done directory path : The path provided is not an existing writable directory.'));
  }
  $path = $form_state['values']['wallymport']['error_directory_path'];
  $dir_exist = file_check_directory($path,0);	// IN D7 this function becomes file_prepare_directory()
  if (!$dir_exist) {
    form_set_error('wallymport][error_directory_path', t('Error directory path : The path provided is not an existing writable directory.'));
  }
  $path = $form_state['values']['wallymport']['temp_directory_path'];
  $dir_exist = file_check_directory($path,0);	// IN D7 this function becomes file_prepare_directory()
  if (!$dir_exist) {
    form_set_error('wallymport][temp_directory_path', t('Tempory directory path : The path provided is not an existing writable directory.'));
  }
  $file = $form_state['values']['wallymport']['xsd_path'];
  $is_readable = is_readable($file);
  if (!$is_readable) {
    form_set_error('wallymport][xsd_path', t('XSD path : The path provided is not an existing writable directory.'));
  }
  if (!is_numeric($form_state['values']['wallymport']['cron_frequency']) & $form_state['values']['wallymport']['cron_frequency'] != ''){
    form_set_error('wallymport][cron_frequency',t('The CRON frequency must be an integer'));
  } else {
    $form_state['values']['wallymport']['cron_frequency'] = $form_state['values']['wallymport']['cron_frequency'] * 60;
    $form_state['values']['wallymport']['cron_timestamp'] = time();
  }

  if (!user_load($form_state['values']['wallymport']['default_user'])){
    form_set_error('wallymport][default_user',t('The user id does not exist'));
  }
  $path = $form_state['values']['wallymport']['preview_source_directory_path'];
  $dir_exist = file_check_directory($path,0);	// IN D7 this function becomes file_prepare_directory()
  if (!$dir_exist & $form_state['values']['wallymport']['preview_source_directory_path'] != '') {
    form_set_error('wallymport][preview_source_directory_path', t('Preview source directory path : The path provided is not an existing writable directory.'));
  }
  $path = $form_state['values']['wallymport']['preview_error_directory_path'];
  $dir_exist = file_check_directory($path,0);	// IN D7 this function becomes file_prepare_directory()
  if (!$dir_exist & $form_state['values']['wallymport']['preview_source_directory_path'] != '') {
    form_set_error('wallymport][preview_error_directory_path', t('Preview error directory path : The path provided is not an existing writable directory.'));
  }
  $path = $form_state['values']['wallymport']['preview_temp_directory_path'];
  $dir_exist = file_check_directory($path,0);	// IN D7 this function becomes file_prepare_directory()
  if (!$dir_exist & $form_state['values']['wallymport']['preview_source_directory_path'] != '') {
    form_set_error('wallymport][preview_temp_directory_path', t('Preview temporary directory path : The path provided is not an existing writable directory.'));
  }
  $form_state['values']['wallymport']['name'] = check_plain($form_state['values']['wallymport']['name']);
  $form_state['values']['wallymport']['description'] = check_plain($form_state['values']['wallymport']['description']); 
  if (isset($form_state['values']['wallymport']['iid'])){
    $form_state['values']['update'] = array('iid');
  } else {
    if (preg_match('/[^a-z0-9_]/', $form_state['values']['wallymport']['name'])) {
      form_set_error('wallymport][name', t('Importer name must be alphanumeric or underscores only.'));
    } elseif (wallymport_getimporterbyname($form_state['values']['wallymport']['name']) != NULL){
      form_set_error('wallymport][name', t('You must use a unique name for this import.'));
    }
    $form_state['values']['update'] = array();
  }
}
/*
 * Submit of createimporter_form 
 */
function wallymport_page_importer_form_submit($form, &$form_state){
  $result = drupal_write_record('wallymport_importer', $form_state['values']['wallymport'], $form_state['values']['update']);
  
  if ($result != FALSE){
    $message = 'The importer as been created/updated';
  } else {
    $message = 'The importer as not been created/udpated';
  }
  drupal_set_message(t($message));
  drupal_goto('/admin/wally/wallymport/settings/general');
}
/**
 * Delete importer form.
 */
function wallymport_page_importerdelete_form($form_state,$delete_id = NULL){
  
  if ($delete_id != NULL){
    $form['wallymport']['#tree'] = TRUE;
    $form['wallymport']['iid'] = array(
      '#type' => 'value',
      '#value' => $delete_id,
    );
    $form['text'] = array(
      '#type' => 'markup',
      '#value' => t('Are you sure you want to delete this importer? '),
    );
    $form['buttons']['submit'] = array('#type' => 'submit', '#value' => t('Delete') );
  }
  return $form;
}
/*
 * Submit of the importer delete form
 */
function wallymport_page_importerdelete_form_submit($form,$form_state){

  $result = db_query("DELETE FROM {wallymport_importer} WHERE iid = %d", $form_state['values']['wallymport']['iid']);
  if ($result != FALSE){
    $message = 'The importer as been deleted';
  } else {
    $message = 'The importer as not been deleted';
  }
  drupal_set_message(t($message));
  drupal_goto('/admin/wally/wallymport/settings/general');
  
}
/*
 * administration page of wallymport
 */
function wallymport_page_admin(){
  $content = '';
  if (db_table_exists('wallymport_importer')){
    $importers = wallymport_getimporters();
    $yesno = array(t('No'), t('Yes'));
    
    if ($importers != NULL){
      $header = array(t('Importeur'),t('Name'),t('Description'),t('POST'), t('CRON'), t('URL to process'), t('Operations'));
      $rows = array();
      foreach ($importers as $importer){
        $logs = '';
        $post = '';
        if ($importer->processed_by_cron == 1){
          $logs = ' (<a href = "/admin/wally/wallymport/settings/log/'.$importer->iid.'">logs</a>)';
        }
        if ($importer->processed_by_post == 1){
          $post = ' (<a href = "/wallymport/post/'.$importer->name.'">'.url('wallymport/post/'.$importer->name, array('absolute' => TRUE)).'</a>)';
        }
        $rows[] = array(
          $importer->importer_type, 
          $importer->name, 
          $importer->description, 
          $yesno[$importer->processed_by_post].$post,
          $yesno[$importer->processed_by_cron].$logs,
          '<a href = "/wallymport/process/'.$importer->name.'">'.url('wallymport/process/'.$importer->name, array('absolute' => TRUE)).'</a>',
          '<a href = "/admin/wally/wallymport/settings/edit/'.$importer->iid.'">'.t('edit').'</a>, <a href = "/admin/wally/wallymport/settings/delete/'.$importer->iid.'">'.t('delete').'</a>',
        );
      }
      $content .= theme_table($header,$rows);
    } else {
      $content = t('There is no importer. To add one, click <a href = "/admin/wally/wallymport/settings/add">here</a>');
    }
  } else {
    $content = t('This module needs to be updated');
  }
  return $content;
}
/**
 * Importer logs
 * @param $iid
 * importer id 
 * @return page content
 */

function wallymport_page_importer_logs($iid){
  $importer = wallymport_getimporter($iid);
  $logs = unserialize($importer->cron_log);
  rsort($logs);
  $content = '';
  foreach ($logs as $log){
    $content .= theme('wallymport_logs', $log);
  }
  return $content;
}