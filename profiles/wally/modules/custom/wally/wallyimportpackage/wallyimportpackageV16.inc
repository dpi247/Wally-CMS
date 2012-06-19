<?php

/**
 * Process field Package Description from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
 * All linked & embeded obkect will be unlinked before creating the new list. 
 * So if package was already linked to an object and this object no more
 * part of the list in XML, the link for this object will be removed.
 * 
 * Orphan object (that aren't linked to any packages) should be
 * deleted by an external process. 
 * 
 * @param $node
 *   Node currently processed.
 * @param $dom_element
 *   The DomDocument who is being proceed (A DomElement)
 * @param $log
 *   The log object build recursively in different operation. this could track error in the process and build nodes.
 *
 * @return
 *   Nothing
 */
function wallyimportpackageV16_set_field_EmbeddedContent(&$node, $dom, &$log, $importer) {
  static $i = 100;

  // Embeded objects
  $results = array();
  $node->field_embededobjects = $arraytemporaire = array();
  $node->field_embededobjects_nodes = $arraytemporaire_nodes = array();
  if($embedobjects = $dom->getElementsByTagName('EmbeddedObjects')->item(0)) {
    foreach(wimprt_process_list($embedobjects, $log, $importer, 'object') as $result) {
      if ((true||!$result->rejected) && (!array_key_exists($result->nid, $results) || $importer->wallymport_preview)) {
        $results[$result->nid] = TRUE;
        $order = $result->embeddedobjectsorder ? $result->embeddedobjectsorder : $i++;
        $arraytemporaire[$order]['nid'] = $result->nid;
        if ($importer->wallymport_preview){
          $arraytemporaire_nodes[$order] = $result;
        }
      }
    }
  }

  ksort($arraytemporaire);
  ksort($arraytemporaire_nodes);
  // Place object in the correct order (depend of the embeddedobjectorder)
  foreach ($arraytemporaire as $array_key => $array_value){
    array_push($node->field_embededobjects, $array_value);
    if ($importer->wallymport_preview){
      array_push($node->field_embededobjects_nodes, $arraytemporaire_nodes[$array_key]);
    }
  }

  // Linkslists objects
  $links = array();
  $node->field_linkedobjects = $arraytemporaire = array();
  $node->field_linkedobjects_nodes = $arraytemporaire_nodes = array();
  $linkslists = $dom->getElementsByTagName('LinksLists')->item(0);
  $linkslist = $dom->getElementsByTagName('LinksList')->item(0);
  if ($linkslists && $linkslist){
    if ($linkslist->getElementsByTagName('Links')->item(0)->nodeValue != NULL){
      foreach (wimprt_process_list($linkslists, $log, $importer, 'object') as $result) {
        if ((true||!$result->rejected) && (!array_key_exists($result->nid, $links) || $importer->wallymport_preview)) {
          $links[$result->nid] = TRUE;
          $order = $result->embeddedobjectsorder ? $result->embeddedobjectsorder : $i++;
          $arraytemporaire[$order]['nid'] = $result->nid;
          if ($importer->wallymport_preview){
            $arraytemporaire_nodes[$order] = $result;
          }
        }
      }
    } else {
      drupal_set_message('WARNING: No LinksList found in LinksLists element. Skipping LinksLists process.','notice');
    }
  }
  
  ksort($arraytemporaire);
  ksort($arraytemporaire_nodes);
  // Place object in the correct order (depend of the embeddedobjectorder)
  foreach ($arraytemporaire as $array_key => $array_value){
    array_push($node->field_linkedobjects, $array_value);
    if ($importer->wallymport_preview){
      array_push($node->field_linkedobjects_nodes, $arraytemporaire_nodes[$array_key]);
    }
  }
}

/**
 * Process photo object from DOM.
 * 
 * MAP fields of the photo object content type
 * from the DOM (XML) element & attributes. 
 * 
 * @param $node
 *   Node currently processed.
 * @param $dom_element
 *   The DomDocument who is being proceed (A DomElement)
 * @param $log
 *   The log object build recursively in different operation. this could track error in the process and build nodes.
 *
 * @return
 *   Nothing
 * 
 * @todo
 *   Check variable name line 575, procced or proceed?
 */
function wallyimportpackageV16_process_object_photoobjecttype(&$node, $dom, &$log, $importer) {
  module_load_include('inc', 'content', 'includes/content.crud');
  $token_user = $importer->default_user ? user_load($importer->default_user) : NULL;

  $c = array();
  foreach($dom->getElementsByTagName('Crop') as $crop) {
  	if ($crop->hasAttribute("Type")) {
  	  $type_name = $crop->getAttributeNode("Type")->value;
  	  $type_array = imagecache_preset_by_name($type_name);
  	  if (!empty($type_array)){
	    $c[$type_name][0] = $crop->getAttributeNode("X")->value;
	    $c[$type_name][1] = $crop->getAttributeNode("Y")->value;
	    $c[$type_name][2] = $crop->getAttributeNode("W")->value;
	    $c[$type_name][3] = $crop->getAttributeNode("H")->value;
        $c[$type_name]["type"] = $type_array["presetid"];
      }
  	}
  }

  $presets = (json_encode($c));
  $node->field_croping[0]['value'] = $presets; 
  
  $value = $dom->nodeValue;
  
  //put common attributes
  wallyimportpackageV16_objecttype_preprocess($node, $dom, $importer);

  //if (!isset($node->field_summary[0]['value']) || empty($node->field_thumbnail[0]))
  $node->field_summary[0]['value'] = _wimprthelp_plain_text($value);
  
  $dom_thumbnails = $dom->getElementsByTagName('Thumbnail');
  $thumb_uri = $dom->getElementsByTagName('Thumbnail')->item(0) ? $dom->getElementsByTagName('Thumbnail')->item(0)->getAttribute('URI') : '';
  //if (!isset($node->field_thumbnail[0]) || count($node->field_thumbnail[0])<8)
  $thumb_field = content_field_instance_read(array('type_name' => 'wally_photoobject', 'field_name' => 'field_thumbnail'));
  $new_thumb_path = module_exists('token') ? token_replace($thumb_field[0]['widget']['file_path'], 'user', $token_user) : $thumb_field[0]['widget']['file_path'];
  $node->field_thumbnail[0] = _wimprthelp_create_file_from_uri($thumb_uri, $new_thumb_path, $importer);

  _wallymportpackage_manage_file_from_uri($node, $importer, $dom, $token_user, 'emimage', 'wally_photoobject', 'field_photofile', 'field_photo3rdparty');

  wimprt_process_node_default($node, $dom, $log, $importer);

  if ($dom->getElementsByTagName("Caption")->length) {
    $node->field_summary[0]["value"] = $dom->getElementsByTagName("Caption")->item(0)->nodeValue;
  }
  //Link
  $link_title  = $dom->getElementsByTagName('LinkTitle')->item(0)->nodeValue;
  $link_uri = $dom->getElementsByTagName('LinkURI')->item(0)->nodeValue;
  if ($link_title != NULL | $link_uri != NULL){
    $node->field_link[0]['url'] = $link_uri;
    $node->field_link[0]['title'] = $link_title;
  }

  return $node;
}

/**
 * Process digital object from DOM.
 * 
 * MAP fields of the video object content type
 * from the DOM (XML) element & attributes. 
 * 
 * @param $node
 *   Node currently processed.
 * @param $dom_element
 *   The DomDocument who is being proceed (A DomElement)
 * @param $log
 *   The log object build recursively in different operation. this could track error in the process and build nodes.
 *
 * @return
 *   Nothing
 */
function wallyimportpackageV16_process_object_multimediaobjecttype(&$node, $dom, &$log, $importer){
  $value = $dom->nodeValue;
  
  //put common attributes
  wallyimportpackageV16_objecttype_preprocess($node, $dom, $importer);

  $token_user = $importer->default_user ? user_load($importer->default_user) : NULL;
  _wallymportpackage_manage_file_from_uri($node, $importer, $dom, $token_user, 'emother', 'wally_multimediaobject', 'field_objectfile', 'field_object3rdparty');
 
  //$node->field_link[0]['url']= $dom->getElementsByTagName('LinkURI')->item(0)->nodeValue;
  //$node->field_link[0]['title']= $dom->getElementsByTagName('LinkTitle')->item(0)->nodeValue;
  wimprt_process_node_default($node, $dom, $log, $importer);
  
  return $node;
}

/**
 * Process audio object from DOM.
 * 
 * MAP fields of the audio object content type
 * from the DOM (XML) element & attributes. 
 * 
 * @param $node
 *   Node currently processed.
 * @param $dom_element
 *   The DomDocument who is being proceed (A DomElement)
 * @param $log
 *   The log object build recursively in different operation. this could track error in the process and build nodes.
 *
 * @return
 *   Nothing
 */
function wallyimportpackageV16_process_object_audioobjecttype(&$node, $dom, &$log, $importer){
  $value = $dom->nodeValue;
  
  //put common attributes
  wallyimportpackageV16_objecttype_preprocess($node, $dom, $importer);

  $token_user = $importer->default_user ? user_load($importer->default_user) : NULL;
  _wallymportpackage_manage_file_from_uri($node, $importer, $dom, $token_user, 'emaudio', 'wally_audioobject', 'field_audiofile', 'field_audio3rdparty');

  //Link
  $link_title  = $dom->getElementsByTagName('LinkTitle')->item(0)->nodeValue;
  $link_uri = $dom->getElementsByTagName('LinkURI')->item(0)->nodeValue;
  if ($link_title != NULL | $link_uri != NULL){
    $node->field_link[0]['url'] = $link_uri;
    $node->field_link[0]['title'] = $link_title;
  }
  wimprt_process_node_default($node, $dom, $log, $importer);
  
  return $node;
}

/**
 * Process article package from DOM.
 *
 * MAP fields of the article package content type
 * from the DOM (XML) element & attributes.
 *
 * @param $node
 *   Node currently processed.
 * @param $dom_element
 *   The DomDocument who is being proceed (A DomElement)
 * @param $log
 *   The log object build recursively in different operation. this could track error in the process and build nodes.
 *
 * @return
 *   Nothing
 */
function wallyimportpackageV16_process_object_link(&$node, $dom, &$log, $importer){
  wallyimportpackage_typetype_preprocess(&$node, $dom, $importer);

  //$node->type='wally_linktype';
  wimprt_process_type_default($node, $dom, $log, $importer);

  if ($dom->hasAttribute('EmbeddedObjectsOrder')){
    $node->embeddedobjectsorder = $dom->getAttributeNode('EmbeddedObjectsOrder')->value;
  }

  $uri = $dom->getElementsByTagName('URI')->item(0);
  $uri_value = $uri->nodeValue;
  $uri = parse_url($uri_value);

  if (strtolower($uri['scheme']) == 'extref'){
    $ext_ref = $uri['host'];

    if ($link = wallytoolbox_get_node_by_cckfield(array('field_externalreference' => $ext_ref))) {
      if (count($link) == 1) {
        $node->field_internal_link[0]['nid'] = $link[0]->nid;
      }
    }
  } else{
    $node->field_link_item[0]['title'] = $node->title;
    $node->field_link_item[0]['url'] = $uri_value;
    $node->field_link_item[0]['attributes'] = array('target'=>'_blank');
  }
}

/**
 * Process poll object from DOM.
 * 
 * MAP fields of the poll object content type
 * from the DOM (XML) element & attributes. 
 * 
 * @param $node
 *   Node currently processed.
 * @param $dom_element
 *   The DomDocument who is being proceed (A DomElement)
 * @param $log
 *   The log object build recursively in different operation. this could track error in the process and build nodes.
 *
 * @return
 *   Nothing
 */
function wallyimportpackageV16_process_object_pollobjecttype(&$node, $dom, &$log, $importer){
  static $proceed_file_path;

  //put common attribute
  wallyimportpackageV16_objecttype_preprocess($node, $dom, $importer);

  wimprt_process_node_default($node, $dom, $log, $importer);

  //Exception for PollObject
  if ($dom->getElementsByTagName("PollTitle")->item(0)->nodeValue) {
    $node->title = $dom->getElementsByTagName("PollTitle")->item(0)->nodeValue;
  }

  if ($dom->getElementsByTagName("Title")->length) {
    $node->field_objecttitle = array(array("value" => $dom->getElementsByTagName("Title")->item(0)->nodeValue));
  }
   
  //Exception for TextObject
  $date1 = NULL;
  $date2 = NULL;
  $date1 = $dom->getElementsByTagName("DateOpen")->item(0)->nodeValue;
  if ($dom->getElementsByTagName("DateClose")->length){
    $date2 = $dom->getElementsByTagName("DateClose")->item(0)->nodeValue;
  }
  $node->field_polldate[0]=_wimprthelp_polldate_to_ccktime($date1, $date2);

  return $node;
}

/**
 * Process video object from DOM.
 * 
 * MAP fields of the video object content type
 * from the DOM (XML) element & attributes. 
 * 
 * @param $node
 *   Node currently processed.
 * @param $dom_element
 *   The DomDocument who is being proceed (A DomElement)
 * @param $log
 *   The log object build recursively in different operation. this could track error in the process and build nodes.
 *
 * @return
 *   Nothing
 */
function wallyimportpackageV16_process_object_videoobjecttype(&$node, $dom, &$log, $importer){
  static $procced_file_path;
  $value = $dom->nodeValue;
  
  //put common attributes
  wallyimportpackageV16_objecttype_preprocess($node, $dom, $importer);

  $token_user = $importer->default_user ? user_load($importer->default_user) : NULL;
  _wallymportpackage_manage_file_from_uri($node, $importer, $dom, $token_user, 'emvideo', 'wally_videoobject', 'field_videofile', 'field_video3rdparty');
  
  $node->field_link[0]['url'] = $dom->getElementsByTagName('LinkURI')->item(0)->nodeValue;
  $node->field_link[0]['title'] = $dom->getElementsByTagName('LinkTitle')->item(0)->nodeValue;
  wimprt_process_node_default($node, $dom, $log, $importer);
  //$node->field_video3rdparty[0]['embed']=$dom->getElementsByTagName('LinkURI')->item(0)->nodeValue;
  //$node->field_video3rdparty[0]['title']=$dom->getElementsByTagName('LinkTitle')->item(0)->nodeValue;
  
 //Link
  $link_title  = $dom->getElementsByTagName('LinkTitle')->item(0)->nodeValue;
  $link_uri = $dom->getElementsByTagName('LinkURI')->item(0)->nodeValue;
  if ($link_title != NULL | $link_uri != NULL){
    $node->field_link[0]['url'] = $link_uri;
    $node->field_link[0]['title'] = $link_title;
  }
  return $node;
}

/**
 * Process Text object from DOM.
 * 
 * MAP fields of the text object content type
 * from the DOM (XML) element & attributes. 
 * 
 * @param $node
 *   Node currently processed.
 * @param $dom_element
 *   The DomDocument who is being proceed (A DomElement)
 * @param $log
 *   The log object build recursively in different operation. this could track error in the process and build nodes.
 *
 * @return
 *   Nothing
 */
function wallyimportpackageV16_process_object_textobjecttype (&$node, $dom_element, &$log, $importer){
  //Map XML Attributes 
  //put common attributes
  
  wallyimportpackageV16_objecttype_preprocess($node, $dom_element, $importer);
  wimprt_process_node_default($node, $dom_element, $log, $importer);
  //Exception for TextObject
  if ($dom_element->getElementsByTagName("TextTitle")->item(0)->nodeValue) {
    $node->title = $dom_element->getElementsByTagName("TextTitle")->item(0)->nodeValue;
  }
  
  if ($dom_element->getElementsByTagName("Title")->length) {
    $node->field_objecttitle = array(array("value" => $dom_element->getElementsByTagName("Title")->item(0)->nodeValue));
  }
}

/**
 * Process common attributes of objects from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
 * 
 * @param $node
 *   Node currently processed.
 * @param $dom_element
 *   The DomDocument who is being proceed (A DomElement)
 *
 * @return
 *   Nothing
 */
function wallyimportpackageV16_objecttype_preprocess(&$node, $dom, $importer){
  $node->wallymport_node = TRUE;
  if ($dom->hasAttribute('EmbeddedObjectsOrder')){
    $node->embeddedobjectsorder = $dom->getAttributeNode('EmbeddedObjectsOrder')->value;
  }
  $node->field_objectid[0]['value'] = $dom->getAttributeNode('ObjectID')->value;
  if ($dom->hasAttribute('ObjectDescription')){
    $node->object_description = $dom->getAttributeNode('ObjectDescription')->value;
  }
  $node->field_publicationdate[0] = _wimprthelp_date_to_ccktime($dom->getAttributeNode('PublicationDate')->value);
  $node->created = _wimprthelp_date_to_timestamp($dom->getAttributeNode('CreationDate')->value);
  if ($dom->hasAttribute('LastUpdateDate')){
    $node->changed = _wimprthelp_date_to_timestamp($dom->getAttributeNode('LastUpdateDate')->value);
  }
  if ($importer->wallymport_preview){
    $node->rejected = TRUE;
  }
}

/**
 * Process article package from DOM.
 * 
 * MAP fields of the article package content type
 * from the DOM (XML) element & attributes. 
 * 
 * @param $node
 *   Node currently processed.
 * @param $dom_element
 *   The DomDocument who is being proceed (A DomElement)
 * @param $log
 *   The log object build recursively in different operation. this could track error in the process and build nodes.
 *
 * @return
 *   Nothing
 */
function wallyimportpackageV16_process_type_link(&$node, $dom, &$log, $importer){
  wallyimportpackage_typetype_preprocess(&$node, $dom, $importer);

  //$node->type='wally_linktype';
  wimprt_process_type_default($node, $dom, $log, $importer);
  
  if ($dom->hasAttribute('EmbeddedObjectsOrder')){
    $node->embeddedobjectsorder = $dom->getAttributeNode('EmbeddedObjectsOrder')->value;
  }

  $uri = $dom->getElementsByTagName('URI')->item(0);
  $uri_value = $uri->nodeValue;
  $uri = parse_url($uri_value);

  if (strtolower($uri['scheme']) == 'extref'){
    $ext_ref = $uri['host'];

    if ($link = wallytoolbox_get_node_by_cckfield(array('field_externalreference' => $ext_ref))) {
      if (count($link) == 1) {
        $node->field_internal_link[0]['nid'] = $link[0]->nid;
      }
    }
  } else{
    $node->field_link_item[0]['title'] = $node->title;
    $node->field_link_item[0]['url'] = $uri_value;
    $node->field_link_item[0]['attributes'] = array('target'=>'_blank');
  }
}
