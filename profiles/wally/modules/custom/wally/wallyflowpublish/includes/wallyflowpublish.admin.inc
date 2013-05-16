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
  while ($db_result = db_fetch_array($db_datas)) {
    $datas[$db_result['tid']] = $db_result;
  }
  
  $dest_vid = variable_get('wallymport_destinationpath', 0);
  $taxos = taxonomy_get_tree($dest_vid);
  $hierarchies = wallyctools_get_hierarchies();

  foreach ($taxos as $taxo){
    $tid = $taxo->tid;
    $data = $datas[$tid];
    while(is_string($data['mediafed'])) {
      $data['mediafed'] = unserialize($data['mediafed']);
    }

    $dest = str_replace(' ', '-', $taxo->name);
    $name = empty($data['title']) ? $dest : $data['title'];
    $link_name = wallytoolbox_taxonomy_get_path_by_tid_or_term($tid, $dest_vid);
    $descr = empty($data['description']) ? $taxo->description : $data['description'];
    $copy = empty($data['copyright']) ? t('Copyright') : $data['copyright'];
    $items_limit = empty($data['items_limit']) ? 20 : $data['items_limit'];

    $view = views_get_view('default_destination_view_feed');
    $display = $view->display['feed_1'];
    $view_path = $display->display_options['path'];
    $expl_path = explode('/%/', $view_path);
    $link = '/'.$expl_path[0].'/'.$link_name;
    $link = (isset($expl_path[1]) && $expl_path[1] != '') ? $link.'/'.$expl_path[1] : $link;
    $feed_link = t('View !dest feeds :', array('!dest' => $dest));
    $redacblocks = $hierarchies[$tid]['redacblocks'];
    foreach ($redacblocks as $redacblock) {
      $redac_link = $link.'/'.$redacblock['name'];
      $feed_link .= '<br><a href="'.$redac_link.'">'.$redacblock['title'].' : '.$base_url.$redac_link.'</a>';
    }

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
    
    $form[$tid]['items_limit'] = array(
      '#type' => 'textfield',
      '#title' => t('Amout of items to display'),
      '#default_value' => $items_limit,
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
      '#maxlength' => 512,
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('edit-'.$tid.'-mediafed-activate' => array(1)),
    );
    
    $form[$tid]['mediafed']['user_agent'] = array(
      '#type' => 'textfield',
      '#title' => t('Bypass User Agent'),
      '#description' => t('The user agent used by MediaFed, this user agent will bypass the redirection. Regex allowed (without delimiters).'),
      '#default_value' => $mediafed_defaults['user_agent'],
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('edit-'.$tid.'-mediafed-activate' => array(1)),
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
