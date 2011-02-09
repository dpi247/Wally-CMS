<?php

function wallyrsspublish_preprocess_views_view_rss(&$vars) {
	global $base_url;
	global $language;

	$view     = &$vars['view'];
	$options  = &$vars['options'];
	$items    = &$vars['rows'];
	
	$tid = $view->args[0];
	$tax = taxonomy_get_term($tid);
	$taxo = $tax->name;

	$vars['title'] = $taxo;
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





	$imain = $my_node->field_embededobjects_nodes;

	$type = $imain[0]->field_photofile[0]['filemime'];
	$filesize = $imain[0]->field_photofile[0]['filesize'];

	$imainstory = $imain[0]->field_photofile[0]['filepath'];
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



