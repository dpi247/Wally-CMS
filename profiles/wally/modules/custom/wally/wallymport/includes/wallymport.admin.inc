<?php
// $Id: yaxim.admin.inc,v 1.0 2010/08/23 14:39:00 rso Exp $

/**
 * @file
 *   Import Press Content to Drupal structure Administration
 */

/**
 * List of existing importers
 */
function wallymport_page_list_importers(){
  $content = '';
  $importers = wallymport_getimporters();
  $yesno = array(t('No'), t('Yes'));

  if ($importers != NULL){
    $header = array(t('Importeur'),t('Name'),t('Description'),t('POST'), t('CRON'), t('Importer links'), t('URL to process folder'), t('URL to process file'), t('Logs'),t('Operations'));
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
          '<a href = "/admin/wally/wallymport/settings/links/'.$importer->name.'">'.$importer->name.' links</a>',
          '<a href = "/wallymport/process/'.$importer->name.'">'.url('wallymport/process/'.$importer->name, array('absolute' => TRUE)).'</a>',
          '<a href = "/wallymport/process_file/'.$importer->name.'/exemple.ext/run">'.url('wallymport/process_file/'.$importer->name.'/exemple.ext/run', array('absolute' => TRUE)).'</a>',
          '<a href = "/admin/wally/wallymport/settings/logs/'.$importer->iid.'">Logs</a>',          
          '<a href = "/admin/wally/wallymport/settings/edit/'.$importer->iid.'">'.t('edit').'</a>, <a href = "/admin/wally/wallymport/settings/delete/'.$importer->iid.'">'.t('delete').'</a>',
      );
    }
    $content .= theme_table($header,$rows);
  } else {
    $content = t('There is no importer. To add one, click <a href = "/admin/wally/wallymport/settings/add">here</a>');
  }
  return $content;
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
  
  $importers_name = array();
  if ($modules = module_invoke_all('wallymport_importer')) {
    foreach ($modules as $importers){
      foreach ($importers as $importer){
        $importers_name[$importer['importer_type']] = $importer['importer_type'];
      }
    }
  } else {
    drupal_set_message(t('No importer installed, click !here to install one.', array('!here' => l(t('here'), 'admin/build/modules'))), 'error');
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
    '#type'        => 'select',
    '#title'       => t('Are the data imported during POST?'),
    '#description' => t('Indicates if the data are processed during the POST?'),
    '#options'     => array('' => '', 0 => t('No'), 1 => t('Yes')),
    '#required'    => TRUE,
  );
  
   $form['wallymport']['processed_by_cron'] = array(
    '#type'        => 'select',
    '#title'       => t('Are the data imported by a CRON job?'),
    '#description' => t('Indicates if the data are processed by a CRON job?'),
    '#options'     => array('' => '', 0 => t('No'), 1 => t('Yes')),
    '#required'    => TRUE,
  );
  
  $form['wallymport']['cron_frequency'] = array (
    '#type'             => 'textfield',
    '#title'            => t('CRON frequency'),
    '#description'      => t('CRON frequency (in minutes)'),
    '#required'         => FALSE,
    '#dependency'       => array('edit-wallymport-processed-by-cron' => array(1)),
    '#process'          => array('ctools_dependent_process'),
  );
  
  $form['wallymport']['default_user'] = array(
    '#type'        => 'textfield',
    '#title'       => t('Drupal user used for imports.'),
    '#description' => t('This user will be used as author for creating content.'),
    '#size'        => 3,
    '#maxlength'   => 8,
    '#required'    => TRUE,
  );
  
  $form['wallymport']['keep_log'] = array(
    '#type'        => 'select',
    '#title'       => t('Keep logs?'),
    '#description' => t('If checked, the logs are saved in db'),
    '#options'     => array('' => '', 0 => t('No'), 1 => t('Yes')),
    '#required'    => TRUE,
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

/**
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

/**
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
  drupal_goto('admin/wally/wallymport/settings/list');
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

/**
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
  drupal_goto('admin/wally/wallymport/settings/general');
}

/**
 * Importer logs
 * 
 * @param $iid
 *   Importer id 
 * 
 * @return page content
 */
function wallymport_page_importer_cron_logs($iid){
  $importer = wallymport_getimporter($iid);
  $logs = unserialize($importer->cron_log);
  $content = '';
  if ($logs != NULL){
    rsort($logs);
    foreach ($logs as $log){
      $content .= theme('wallymport_logs', $log);
    }
  } else {
    $content .= 'There is no log';
  }
  return $content;
}

/**
 * Importer logs
 * 
 * @param $iid
 *   Importer id
 * 
 * @return page content
 */
function wallymport_page_importer_logs($iid){
  $importer = wallymport_getimporter($iid);
  $count_tot = db_fetch_array(db_query("SELECT COUNT(*) FROM {wallymport_logs}"));
  $result = db_query("SELECT * FROM {wallymport_logs} logs WHERE logs.importer_name = '%s' ORDER BY logs.date DESC LIMIT 0,500", $importer->name);
  $header = array(t('Date'), t('Filename'), t('Filelocation'), t('Success'), t('Message'));
  while ($obj = db_fetch_object($result)){
    $filepath = explode('/', $obj->location);
    $filename = array_pop($filepath);
    $logs = unserialize($obj->log);
    $success = 'TRUE';
    if ($logs['error']){
      $success = 'FALSE';
    }
    $extref = '';
    if (isset ($logs['log']['extref'])){
      $extref = ' External Rerefence : '.$logs['log']['extref'];
    }
    $rows[] = array(date('d/m/Y H:i:s', $obj->date), $filename, $obj->location, $success, $logs['error_msg'].$extref);
  }
  drupal_set_message(t('@num_rows packages in the logs (500 visible)', array('@num_rows' => $count_tot['COUNT(*)'])));
  return theme_table($header,$rows);
}

/**
 * Page with the links to the folders of the importer
 */
function wallymport_page_links_form($form_state, $importer_name){
  $importer = wallymport_getimporterbyname($importer_name);

  $form = array();
  $dirs[] = $importer->source_directory_path;
  $dirs[] = $importer->error_directory_path;
  $dirs[] = $importer->done_directory_path;
  
  foreach($dirs as $dir){
    if (file_check_directory($dir, 0)){
      $form[$dir] = array(
        '#type'        => 'fieldset',
        '#title'       => t($dir),
        '#collapsible' => TRUE,
        '#collapsed'   => FALSE,
      );
      $files = file_scan_directory($dir, '.*\.zip', array('.', '..', 'CVS'), 0, FALSE);
      foreach($files as $file){
        $form[$dir][$file->basename]=array(
          "#type"  => 'markup',
          "#value" => '<a href="'.file_create_url($file->filename).'">'.$file->basename.'</a><br/>',
        );
      }
    }
  }
  return $form;
}

/**
 * General settings form for Wallymport
 */
function wallymport_page_general_settings_form($form_state) {
  $form = array();
  
  $form['semaphore'] = array(
    '#type' => 'fieldset',
    '#title' => t('Semaphores'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  
  $form['semaphore']['wallymport_process_folder_semaphore_duration'] = array(
    '#type' => 'textfield',
    '#title' => t('Folder semaphore period'),
    '#description' => t('Period during which the import folder with be locked (minutes)'),
    '#default_value' => variable_get('wallymport_process_folder_semaphore_period', 5),
  );
  
  $form['semaphore']['wallymport_process_file_semaphore_duration'] = array(
    '#type' => 'textfield',
    '#title' => t('File semaphore period'),
    '#description' => t('Period during which a processed file with be locked (minutes)'),
    '#default_value' => variable_get('wallymport_process_file_semaphore_period', 1),
  );
  
  return system_settings_form($form);
}

/**
 * Create a XML document containing all available destinations.
 * The structure of the XML follows packages.xsd.
 * 
 * <xsd:complexType name="DestinationsItem">
 *   <xsd:sequence>
 *     <xsd:element name="DestinationPath" type="xsd:string" />
 *   </xsd:sequence>
 *   <xsd:attribute name="DestinationRank" type="xsd:string" use="required" />
 *   <xsd:attribute name="Position" type="xsd:string" />
 *   <xsd:attribute name="DocumentLayout" use="required" type="xsd:string" />
 * </xsd:complexType>
 * 
 * @return
 *   The XML document as a string.
 */
function wallymport_page_destinations_items($short = FALSE) {
  module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
  $array_dests = wallytoolbox_destinations_items_array();
  $dom_dests = new DOMDocument('1.0', 'UTF-8');

  $destinations = $dom_dests->createElement('Destinations');
  $destinations = $dom_dests->appendChild($destinations);

  foreach ($array_dests as $tid => $term_infos) {
    $term = wallytoolbox_taxonomy_get_path_by_tid_or_term($tid);
    $term = strtoupper($term);
    foreach ($term_infos as $target_name => $target_infos) {
      if (!$short) {
        foreach ($target_infos as $layout_name => $layout_infos) {
          foreach ($layout_infos as $rank_elem => $true_value) {
            $destination = $dom_dests->createElement('Destination');
            $destination = $destinations->appendChild($destination);

            $destination->setAttribute('tid', $tid);
            $destination->setAttribute('Position', $target_name);
            $destination->setAttribute('DocumentLayout', $layout_name);
            $destination->setAttribute('DestinationRank', $rank_elem);

            $destination_path = $dom_dests->createElement('DestinationPath');
            $destination_path = $destination->appendChild($destination_path);
            $destination_path_text = $dom_dests->createTextNode($term);
            $destination_path->appendChild($destination_path_text);
          }
        }
      } else {
        $destination = $dom_dests->createElement('Destination');
        $destination = $destinations->appendChild($destination);

        $destination->setAttribute('tid', $tid);
        $destination->setAttribute('Position', $target_name);

        $destination_path = $dom_dests->createElement('DestinationPath');
        $destination_path = $destination->appendChild($destination_path);
        $destination_path_text = $dom_dests->createTextNode($term);
        $destination_path->appendChild($destination_path_text);
      }
    }
  }
  
  print $dom_dests->saveXML(); 
}
