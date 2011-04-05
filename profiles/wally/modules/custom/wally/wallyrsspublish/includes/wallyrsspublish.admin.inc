<?php
/**
 * Implementation of hook_form().
 */
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
  	$data=db_fetch_array(db_query('SELECT * FROM {rsspublish} WHERE tid= %d',$tid));
  	
  	$dest=str_replace(' ', '-', $taxo->name);
  	$description=$taxo->description;
  	$copyright = "Copyright";
  	
  	$name = empty($data['title']) ? $dest : $data['title'];
  	$descr = empty($data['description']) ? $description : $data['description'];
  	$copy = empty($data['copyright']) ? $copyright : $data['copyright'];
  	
  	
  	$form[$dest.'settings'] = array(
	    '#type' => 'fieldset',
	    '#title' => t($dest.' RSS Settings'),
	    '#description' => t('General parameters used to display a '.$dest.' rss.'),
	    '#collapsible' => TRUE,
	    '#collapsed' => TRUE,
  	);
  	$form[$dest.'settings']['wallyrsspublish_'.$dest.'_rsstid'] = array(
    '#type' => 'hidden',
    '#title' => t($dest.' tid'),
    '#default_value' => $tid, 
    '#description' => t('Rss tid.'),
    '#required'=>TRUE,
  	);
  	$form[$dest.'settings']['wallyrsspublish_'.$dest.'_rsstitle'] = array(
    '#type' => 'textfield',
    '#title' => t($dest.' Title'),
    '#default_value' => $name, 
    '#description' => t('Rss title.'),
    '#required'=>TRUE,
  	);
  	$form[$dest.'settings']['wallyrsspublish_'.$dest.'_rssdescription'] = array(
    '#type' => 'textfield',
    '#title' => t($dest.' Description'),
    '#default_value' => $descr,
    '#description' => t('Rss Description.'),
  	);
  	$form[$dest.'settings']['wallyrsspublish_'.$dest.'_rssimage'] = array(
    '#type' => 'file',
    '#title' =>t('Attach an image'),
    '#description' => t('RSS image'),
    '#size' => 100,
  	);
  	$form[$dest.'settings']['wallyrsspublish_'.$dest.'_rsscopyright'] = array(
    '#type' => 'textfield',
    '#title' => t('Copyright'),
  	'#default_value' => $copy,
  	);
  }
  
  $form['settings'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
  
	
  
}



/**
 * Implementation of hook_form_submit().
 */
function wallyrsspublish_settingsrss_form_submit($form, &$form_state) {
$all_voc = taxonomy_get_vocabularies();
    foreach ($all_voc as $voc) {
      if ($voc->name == 'Destination Path') {
        $taxos = taxonomy_get_tree($voc->vid);
        break;
    }
  }
  
  
  foreach ($taxos as $taxo){
  $dest=str_replace(' ', '-', $taxo->name);
  $object = array();
  $object['tid'] = $form_state['values']['wallyrsspublish_'.$dest.'_rsstid'];
  $object['title'] = $form_state['values']['wallyrsspublish_'.$dest.'_rsstitle'];
  $object['description'] = $form_state['values']['wallyrsspublish_'.$dest.'_rssdescription'];
  $file = file_save_upload('wallyrsspublish_'.$dest.'_rssimage');
  $nameim = $file->filename;
  if(!empty($nameim)){
    $chemin_destination = 'sites/default/files/photorss/';
    $folder = $chemin_destination.$nameim;
    file_move($file->filepath, $folder);
    wallyrsspublish_create_file($folder); 
    $object['imagefile'] = $folder;
  }
  $object['copyright'] = $form_state['values']['wallyrsspublish_'.$dest.'_rsscopyright'];
  $feed_infos = db_fetch_array(db_query('SELECT `rss_key` FROM `rsspublish` WHERE `tid`=%d', $object['tid']));
  
  
  if (empty($feed_infos)) {
  	drupal_write_record('rsspublish', $object);
  } else {
    $object['rss_key'] = $feed_infos['rss_key'];
    drupal_write_record('rsspublish', $object, array('rss_key'));
  }
  }
}

/**
 * 
 * @param $filepath
 * 
 * @return 
 */
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

function wallyrsspublish_ui_tools_clear_cache() {
  views_invalidate_cache();
  drupal_set_message(t('The cache has been cleared.'));
}

/**
 * Submit hook to clear Drupal's theme registry (thereby triggering
 * a templates rescan).
 */
function wallyrsspublish_ui_config_item_form_rescan($form, &$form_state) {
  drupal_rebuild_theme_registry();

  // The 'Theme: Information' page is about to be shown again. That page
  // analyzes the output of theme_get_registry(). However, this latter
  // function uses an internal cache (which was initialized before we
  // called drupal_rebuild_theme_registry()) so it won't reflect the
  // current state of our theme registry. The only way to clear that cache
  // is to re-initialize the theme system:
  unset($GLOBALS['theme']);
  init_theme();

  $form_state['rerender'] = TRUE;
  $form_state['rebuild'] = TRUE;
}

/**
 * Override handler for views_ui_edit_display_form
 */
function wallyrsspublish_ui_edit_display_form_change_theme($form, &$form_state) {
  // This is just a temporary variable.
  $form_state['view']->theme = $form_state['values']['theme'];

  views_ui_cache_set($form_state['view']);
  $form_state['rerender'] = TRUE;
  $form_state['rebuild'] = TRUE;
}
