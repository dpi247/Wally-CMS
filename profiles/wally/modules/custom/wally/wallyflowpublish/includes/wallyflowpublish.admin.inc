<?php

/**
 * Settings form
 */
function wallyflowpublish_settings_form(){
  $taxos = taxonomy_get_tree(variable_get('wallymport_destinationpath', 0));

  $form = array();
  $form['#attributes'] = array('enctype' => "multipart/form-data");
  foreach ($taxos as $taxo){
    $tid = $taxo->tid;
    $data = db_fetch_array(db_query('SELECT * FROM {wallyflowpublish_dests} WHERE tid = %d', $tid));
    
    $dest = str_replace(' ', '-', $taxo->name);
    $name = empty($data['title']) ? $dest : $data['title'];
    $descr = empty($data['description']) ? $taxo->description : $data['description'];
    $copy = empty($data['copyright']) ? t('Copyright') : $data['copyright'];
    
    $form[$dest.'_settings'] = array(
	    '#type' => 'fieldset',
	    '#title' => t($dest.' Flow Settings'),
	    '#description' => t('General parameters used to display a '.$dest.' flow.'),
	    '#collapsible' => TRUE,
	    '#collapsed' => TRUE,
    );
    
    $form[$dest.'_settings']['wallyflowpublish_'.$dest.'_tid'] = array(
      '#type' => 'hidden',
      '#title' => t($dest.' TID'),
      '#default_value' => $tid, 
      '#description' => t('Flow tid.'),
      '#required'=>TRUE,
    );
    
    $form[$dest.'_settings']['wallyflowpublish_'.$dest.'_title'] = array(
      '#type' => 'textfield',
      '#title' => t($dest.' Title'),
      '#default_value' => $name, 
      '#description' => t('Flow title.'),
      '#required'=>TRUE,
    );
    
    $form[$dest.'_settings']['wallyflowpublish_'.$dest.'_description'] = array(
      '#type' => 'textfield',
      '#title' => t($dest.' Description'),
      '#default_value' => $descr,
      '#description' => t('Flow Description.'),
    );
    
    $form[$dest.'_settings']['wallyflowpublish_'.$dest.'_image'] = array(
      '#type' => 'file',
      '#title' =>t('Attach an image'),
      '#description' => t('Flow image'),
      '#size' => 100,
      '#suffix' => $data['imagefile'] ? '<img src="'.imagecache_create_url('flowpublish_preset', $data['imagefile']).'" height = "75px" />' : '',
    );
    
    $form[$dest.'_settings']['wallyflowpublish_'.$dest.'_copyright'] = array(
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
function wallyflowpublish_settings_form_submit($form, &$form_state) {
  $taxos = taxonomy_get_tree(variable_get('wallymport_destinationpath', 0));

  foreach ($taxos as $taxo){
    $dest = str_replace(' ', '-', $taxo->name);
    $object = array();
    $object['tid'] = $form_state['values']['wallyflowpublish_'.$dest.'_tid'];
    $object['title'] = $form_state['values']['wallyflowpublish_'.$dest.'_title'];
    $object['description'] = $form_state['values']['wallyflowpublish_'.$dest.'_description'];
    $file = file_save_upload('wallyflowpublish_'.$dest.'_image');
    $nameim = $file->filename;
    if(!empty($nameim)){
      $folder_path = file_directory_path().'/wallyflowpublish_photos/';
      if (file_check_directory($folder_path, 1)) {
        $dest_path = $folder_path.$nameim;
        file_move($file->filepath, $dest_path);
        wallyflowpublish_create_file($dest_path);
        $object['imagefile'] = $dest_path;
      }
    }
    $object['copyright'] = $form_state['values']['wallyflowpublish_'.$dest.'_copyright'];
    $feed_infos = db_fetch_array(db_query('SELECT `flow_key` FROM {wallyflowpublish_dests} WHERE `tid` = %d', $object['tid']));

    if (empty($feed_infos)) {
      drupal_write_record('wallyflowpublish_dests', $object);
    } else {  
      $object['flow_key'] = $feed_infos['flow_key'];
      drupal_write_record('wallyflowpublish_dests', $object, array('flow_key'));
    }
  }
  
  drupal_set_message(t('Configuration saved'));
}

/**
 *
 * @param $filepath
 *
 * @return
 */
function wallyflowpublish_create_file($filepath){
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

function wallyflowpublish_ui_tools_clear_cache() {
  views_invalidate_cache();
  drupal_set_message(t('The cache has been cleared.'));
}

/**
 * Submit hook to clear Drupal's theme registry (thereby triggering
 * a templates rescan).
 */
function wallyflowpublish_ui_config_item_form_rescan($form, &$form_state) {
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
function wallyflowpublish_ui_edit_display_form_change_theme($form, &$form_state) {
  // This is just a temporary variable.
  $form_state['view']->theme = $form_state['values']['theme'];

  views_ui_cache_set($form_state['view']);
  $form_state['rerender'] = TRUE;
  $form_state['rebuild'] = TRUE;
}
