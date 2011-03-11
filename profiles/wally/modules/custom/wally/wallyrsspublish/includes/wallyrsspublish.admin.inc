<?php

function wallyrsspublish_settingsrss_form(){

  $all_voc = taxonomy_get_vocabularies();
    foreach ($all_voc as $voc) {
      if ($voc->name == 'Destination Path') {
        $taxos = taxonomy_get_tree($voc->vid);
        break;
    }
  }
  $form = array();
  $form['#attributes'] = array('enctype' => "multipart/form-data");
  foreach ($taxos as $taxo){
  	
  	$tid=$taxo->tid;
  	$name=str_replace(' ', '-', $taxo->name);
  	$descr=$taxo->description;
  	
  	$form[$name.'settings'] = array(
	    '#type' => 'fieldset',
	    '#title' => t($name.' RSS Settings'),
	    '#description' => t('General parameters used to display a '.$name.' rss.'),
	    '#collapsible' => TRUE,
	    '#collapsed' => TRUE,
  	);
  	$form[$name.'settings']['wallyrsspublish_'.$name.'_rsstid'] = array(
    '#type' => 'hidden',
    '#title' => t($name.' tid'),
    '#default_value' => $tid, 
    '#description' => t('Rss tid.'),
    '#required'=>TRUE,
  	);
  	$form[$name.'settings']['wallyrsspublish_'.$name.'_rsstitle'] = array(
    '#type' => 'textfield',
    '#title' => t($name.' Title'),
    '#default_value' => $name, 
    '#description' => t('Rss title.'),
    '#required'=>TRUE,
  	);
  	$form[$name.'settings']['wallyrsspublish_'.$name.'_rssdescription'] = array(
    '#type' => 'textfield',
    '#title' => t($dest.' Description'),
    '#default_value' => $descr,
    '#description' => t('Rss Description.'),
  	);
  	$form[$name.'settings']['wallyrsspublish_'.$name.'_rssimage'] = array(
    '#type' => 'file',
    '#title' =>t('Attach an image'),
    '#description' => t('RSS image'),
    '#size' => 100,
  	);
  	$form[$name.'settings']['wallyrsspublish_'.$name.'_rsscopyright'] = array(
    '#type' => 'textfield',
    '#title' => t('Copyright'),
  	);
  }
  
  $form['settings'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
  
	
  
}

function wallyrsspublish_settingsrss_form_validate($form, $form_state) {
	foreach ($destination as $dest){
    if ($form_state['values']['wallyrsspublish_'.$dest.'_rsstitle'] == '') {
    form_set_error('', t('You must select a title for the ' .$dest. ' group of settings.'));
  }
 }
}

function wallyrsspublish_settingsrss_form_submit($form, &$form_state) {
$all_voc = taxonomy_get_vocabularies();
    foreach ($all_voc as $voc) {
      if ($voc->name == 'Destination Path') {
        $taxos = taxonomy_get_tree($voc->vid);
        break;
    }
  }
  
  
  foreach ($taxos as $taxo){
  $name=str_replace(' ', '-', $taxo->name);
  $object = array();
  $object['tid'] = $form_state['values']['wallyrsspublish_'.$name.'_rsstid'];
  $object['title'] = $form_state['values']['wallyrsspublish_'.$name.'_rsstitle'];
  $object['description'] = $form_state['values']['wallyrsspublish_'.$name.'_rssdescription'];
  $file = file_save_upload('wallyrsspublish_'.$name.'_rssimage');
  $nameim = $file->filename;
  if(!empty($nameim)){
    $chemin_destination = 'sites/default/files/photorss/';
    $folder = $chemin_destination.$nameim;
    file_move($file->filepath, $folder);
    wallyrsspublish_create_file($folder); 
    $object['imagefile'] = $folder;
  }
  $object['copyright'] = $form_state['values']['wallyrsspublish_'.$name.'_rsscopyright'];
  $feed_infos = db_fetch_array(db_query('SELECT `rss_key` FROM `rsspublish` WHERE `tid`=%d', $object['tid']));
  
  
  if (empty($feed_infos)) {
  	drupal_write_record('rsspublish', $object);
  } else {
    $object['rss_key'] = $feed_infos['rss_key'];
    drupal_write_record('rsspublish', $object, array('rss_key'));
  }
  }
}

function wallyrsspublish_create_file($filepath){
  global $user;
  $file = new stdClass();
  $file->filename = basename($filepath);
  $file->filepath = $filepath;
  $file->filemime = file_get_mimetype($filepath);
  $file->filesize = filesize($filepath);

  $file->uid = $user->uid;
  $file->status = FILE_STATUS_PERMANENT;
  $file->timestamp = time();
  drupal_write_record('files', $file);
  return  field_file_load($filepath);
}