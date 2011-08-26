<?php

/**
 * Settings form
 */
function wallyflowpublish_settings_form(){
  global $base_url;
  
  $form = array();
  $form['#attributes'] = array('enctype' => "multipart/form-data");
  $form['#tree'] = TRUE;
  
  $db_datas = db_query('SELECT * FROM {wallyflowpublish_dests}');
  while ($db_result = db_fetch_array($db_datas))
    $datas[$db_result['tid']] = $db_result;
  
  $dest_vid = variable_get('wallymport_destinationpath', 0);
  $taxos = taxonomy_get_tree($dest_vid);

  foreach ($taxos as $taxo){
    $tid = $taxo->tid;
    $data = $datas[$tid];
    while(is_string($data['mediafed']))
      $data['mediafed'] = unserialize($data['mediafed']);
    
    $dest = str_replace(' ', '-', $taxo->name);
    $name = empty($data['title']) ? $dest : $data['title'];
    $link_name = wallytoolbox_taxonomy_get_path_by_tid_or_term($taxo->tid, $dest_vid);
    $descr = empty($data['description']) ? $taxo->description : $data['description'];
    $copy = empty($data['copyright']) ? t('Copyright') : $data['copyright'];

    $view = views_get_view('default_destination_view_feed');
    $display = $view->display['feed_1'];
    $view_path = $display->display_options['path'];
    $expl_path = explode('/%/', $view_path);
    $link = '/'.$expl_path[0].'/'.$link_name;
    $link = (isset($expl_path[1]) && $expl_path[1] != '') ? $link.'/'.$expl_path[1] : $link;
    $feed_link ='<a href="'.$link.'">'.t('View !dest feed', array('!dest' => $dest)).' ('.$base_url.$link.')</a>';

    $form[$tid] = array(
	    '#type' => 'fieldset',
	    '#title' => t('!dest Flow Settings', array('!dest' => $dest)),
	    '#collapsible' => TRUE,
	    '#collapsed' => TRUE,
    );
    
    $form[$tid]['feed_link'] = array(
      '#type' => 'markup',
      '#value' => $feed_link,
    );
    
    $form[$tid]['tid'] = array(
      '#type' => 'hidden',
      '#title' => t('!dest TID', array('!dest' => $dest)),
      '#default_value' => $tid, 
      '#description' => t('Flow tid.'),
      '#required'=>TRUE,
    );
    
    $form[$tid]['title'] = array(
      '#type' => 'textfield',
      '#title' => t('!dest Title', array('!dest' => $dest)),
      '#default_value' => $name, 
      '#description' => t('Flow title.'),
      '#required'=>TRUE,
    );
    
    $form[$tid]['description'] = array(
      '#type' => 'textfield',
      '#title' => t('!dest Description', array('!dest' => $dest)),
      '#default_value' => $descr,
      '#description' => t('Flow Description.'),
    );
    
    $form[$tid][$tid.'_imagefile'] = array(
      '#type' => 'file',
      '#title' =>t('Attach an image'),
      '#description' => t('Flow image'),
      '#size' => 100,
      '#suffix' => $data['imagefile'] ? '<img src="'.imagecache_create_url('flowpublish_preset', $data['imagefile']).'" height = "75px" />' : '',
    );
    
    $form[$tid]['copyright'] = array(
      '#type' => 'textfield',
      '#title' => t('Copyright'),
    	'#default_value' => $copy,
    );
    
    ctools_include('dependent');
    ctools_add_js('dependent');
    $mediafed_defaults = $data['mediafed'];
    
    $form[$tid]['mediafed'] = array(
      '#type' => 'fieldset',
      '#title' => t('MediaFed'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    
    $form[$tid]['mediafed']['activate'] = array(
      '#type' => 'checkbox',
      '#title' => t('Activate URL redirection for MediaFed'),
      '#default_value' => $mediafed_defaults['activate'],
    );
    
    $form[$tid]['mediafed']['redirect_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Redirection URL'),
      '#description' => t('The MediaFed URL where the users will be redirected.'),
      '#default_value' => $mediafed_defaults['redirect_url'],
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('edit-activate' => array(1)),
    );
    
    $form[$tid]['mediafed']['user_agent'] = array(
      '#type' => 'textfield',
      '#title' => t('Bypass User Agent'),
      '#description' => t('The user agent used by MediaFed, this user agent will bypass the redirection. Regex allowed (without delimiters).'),
      '#default_value' => $mediafed_defaults['user_agent'],
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('edit-activate' => array(1)),
    );
    
    if (isset($data['flow_key']) && !empty($data['flow_key'])) {
      $form[$tid]['flow_key'] = array(
        '#type' => 'hidden',
        '#value' => $data['flow_key'],
      );
    }
  }
  
  $form['submit'] = array(
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
  $values = $form_state['values'];

  foreach ($taxos as $taxo){
    $tid = $taxo->tid;
    $file = file_save_upload($tid);

    $nameim = $file->filename;
    if(!empty($nameim)){
      $folder_path = file_directory_path().'/wallyflowpublish_photos/';
      if (file_check_directory($folder_path, 1)) {
        $dest_path = $folder_path.$nameim;
        file_move($file->filepath, $dest_path);
        wallyflowpublish_create_file($dest_path);
        $values[$tid]['imagefile'] = $dest_path;
      }
    }

    if (isset($values[$tid]['flow_key'])) {
      drupal_write_record('wallyflowpublish_dests', $values[$tid], 'flow_key');
    } else {
      drupal_write_record('wallyflowpublish_dests', $values[$tid]);
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
