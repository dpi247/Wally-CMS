<?php

function wallyrsspublish_preprocess_views_view_rss(&$vars) {

  global $base_url;
  global $language;

  $view     = &$vars['view'];
  $options  = &$vars['options'];
  $items    = &$vars['rows'];
  
  $name = str_replace('-', ' ', $view->args[0]);
  $tax = taxonomy_get_term_by_name($name);
  $tid=$tax[0]->tid;
  $descr= check_plain($tax[0]->description);
  
  //VERIFIER QUE LA VARIABLE SOIT !=NULL
  $data=db_fetch_array(db_query('SELECT * FROM {rsspublish} WHERE tid= %d',$tid));
  dsm($data);
  
  $vars['title'] = empty($data['title']) ? $name : $data['title'];
  $vars['description'] = empty($data['description']) ? $descr : $data['description'];
  $vars['copyright'] = $data['copyright'];
  
  
  
  $style = &$view->style_plugin;
  // Figure out which display which has a path we're using for this feed. If there isn't
  // one, use the global $base_url
  $link_display_id = $view->display_handler->get_link_display();
  if ($link_display_id && !empty($view->display[$link_display_id])) {
    $path = $view->display[$link_display_id]->handler->get_path();
  }

  if ($path) {
    $path = $view->get_url(NULL, $path);
    $url_options = array('absolute' => TRUE);
    if (!empty($view->exposed_raw_input)) {
      $url_options['query'] = $view->exposed_raw_input;
    }

    // Compare the link to the default home page; if it's the default home page, just use $base_url.
    if ($path == variable_get('site_frontpage', 'node')) {
      $path = '';
    }
    $vars['link'] = check_url(url($path, $url_options));
  }

  $vars['langcode'] = check_plain($language->language);
  $vars['namespaces'] = drupal_attributes($style->namespaces);
  $vars['items'] = $items;
  $vars['channel_elements'] = format_xml_elements($style->channel_elements);

  drupal_set_header('Content-Type: application/rss+xml; charset=utf-8');
}



function wallyrsspublish_preprocess_views_view_row_rss(&$vars) {
	$view     = &$vars['view'];
	$options  = &$vars['options'];
	$item     = &$vars['row'];
	$presetname='rsspublish_preset';


	$nid= $item->nid;
	$my_node = node_load($nid);
	wallycontenttypes_packagepopulate(&$my_node,'show');
	$descr = "";
    if ($my_node->type == 'wally_articlepackage'){
	  $mainstory = $my_node->field_mainstory_nodes[0];
	  $teaser_length = 300;
	  $teaser = theme("wallyct_teaser", $mainstory->field_textbody[0]['value'], $teaser_length, $my_node);

	  if (isset($mainstory->field_textchapo[0]['value'])) {
		$descr = $mainstory->field_textchapo[0]['value'];
	  }
	  else {
		$descr = $teaser;
	  }
    }





	$imain = $my_node->field_embededobjects_nodes[0];

	$type = $imain->field_photofile[0]['filemime'];
	$filesize = $imain->field_photofile[0]['filesize'];
	
	$imainstory = $imain->field_photofile[0]['filepath'];
	$file_img = theme('imagecache', $presetname,  $imainstory);
	$path_photo = imagecache_create_url($presetname, $imainstory);

	
	$vars['filesize'] = $filesize;
	$vars['item'] = $item;
	$vars['url'] = $path_photo;
	$vars['type']= $type;
	$vars['description'] = check_plain($descr);
	$vars['title'] = check_plain($item->title);
	$vars['link'] = check_url($item->link);
	$vars['item_elements'] = empty($item->elements) ? '' : format_xml_elements($item->elements);
	
}