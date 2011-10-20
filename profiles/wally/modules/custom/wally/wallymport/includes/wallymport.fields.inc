<?php 
/**
 * @defgroup wallymport.field Wally fields specifics Callback functions
 * @{
 * This file contain all fields specific functions of wally xml.   
 * These functions will be called by the wimprt_process_package(), 
 * wimprt_process_object() or wimprt_process_type() functions.
 */

/**
 * Process the packages element of a wally DomDocument object.
 * Initiate the node object Call an apropriate function to process the childs of the packages element (package)
 *
 * @param $dom
 *   The DomDocument who is being proceed
 * @param $log
 *   The log object build recursively in different operation. this could track error in the process and build nodes
 *
 * @return
 *   the log object
 */
function wimprt_process_packages($dom, &$log, $preview = FALSE){
  $return = array();
  module_invoke_all('wimprt_process_packages', $dom, $log, 'before_process');
  foreach($dom->getElementsByTagName('Package') as $package_element){
    $return['packages'] = wimprt_process_package($package_element, $log, $preview);
    //$log[]=$log;
  }
  module_invoke_all('wimprt_process_packages',$dom,$log,'after_process');

  $return['log'] = $log;
  return $return;
}


/**
 * Call an apropriate function to process the childs of the package element depending the attribute callback of the child element.
 * 
 * The naming convention to build a node from a specific package callback is: wimprt_process_package_CALLCACK
 * if no specific function found nothing is done.
 *
 * @param $node
 *   The Node that's being build
 * @param $dom
 *   The DomDocument who is being proceed (A DomElement)
 * @param $log
 *   The log object build recursively in different operation. this could track error in the process and build nodes
 *
 * @return
 *   the node object that has been build
 */
function wimprt_process_package($dom, &$log, $preview = FALSE, $callback = NULL) {
  $callback = wimprt_set_callback($dom, $callback);
  $node = wimprt_create_or_load($dom, 'package', $callback, $log);

  //@todo: place in walymportlog module
  if(variable_get('wallymport_debug',0) or TRUE){
    $log['log'][]='process package: '.$dom->nodeName;
  }

  module_invoke_all('wimprt_process_package',$node,$dom,$log,'before_process');

  // ?Posibility of extension with 'hook_wallymport_info' / CTools Plugin?
  if (function_exists("wimprt_process_package_".$callback)){
    call_user_func("wimprt_process_package_".$callback, &$node, $dom, &$log, $preview);
  }

  module_invoke_all('wimprt_process_package', $node, $dom, $log, 'after_process');
  if (!empty($node) && !isset($node->rejected)) {
    if ($preview) {
      require_once './'.drupal_get_path('module', 'wallyedit').'/includes/Prenode.inc';
      $prenode = new Prenode();
      $prenode->ConvertNodeToPrenode($node);
      
      //require_once './'.drupal_get_path('module', 'wallyedit').'/includes/callback_preview.inc';
      $prenode->cache_name = 'wallymport_preview_'.$prenode->form_state['cache name'];
      $form_state_prenode = $prenode->setFormState();
      $form_state_prenode['DoNotPopulate'] = true;
      ctools_object_cache_set('prenode', $prenode->cache_name, $form_state_prenode);
      return $prenode->cache_name;
    } else {
      //watchdog('Wallymport end creation', time().' '.print_r($node, TRUE), array(), WATCHDOG_NOTICE);
      node_save($node);
    }
  } else {
    wallycontenttypes_delete_orphans_objects($node);
  }

  $log['node'] = array('current' => $node,'next' => $log['node']);
  
  return;
}

/**
 * Process the messages element of a wally DomDocument object.
 * Initiate the node object Call an apropriate function to process the childs of the packages element (package)
 *
 * @param $dom
 *   The DomDocument who is being proceed
 * @param $log
 *   The log object build recursively in different operation. this could track error in the process and build nodes
 *
 * @return
 *   the log object
 */
function wimprt_process_messages($dom, &$log, $preview = FALSE){
  $return = array();
  module_invoke_all('wimprt_process_messages', $dom, $log, 'before_process');
  foreach ($dom->getElementsByTagName('Message') as $message_element){
    $return['messages'] = wimprt_process_message($message_element, $log, $preview);
  }
  module_invoke_all('wimprt_process_messages',$dom,$log,'after_process');

  $return['log'] = $log;
  return $return;
}

/**
 * Call an apropriate function to process the childs of the message element depending the attribute callback of the child element.
 * 
 * The naming convention to build a node from a specific package callback is: wimprt_process_package_CALLCACK
 * if no specific function found nothing is done.
 *
 * @param $node
 *   The Node that's being build
 * @param $dom
 *   The DomDocument who is being proceed (A DomElement)
 * @param $log
 *   The log object build recursively in different operation. this could track error in the process and build nodes
 *
 * @return
 *   the node object that has been build
 */
function wimprt_process_message($dom, &$log, $preview = FALSE, $callback = NULL) {
  
  $object = $dom->getElementsByTagName('Object')->item(0);
  $callback = wimprt_set_callback($object);
  list($callback,$rest) = explode('type', $callback);
  
  $node = wimprt_create_or_load_message_tme($object, $log, $callback);
  if ($callback != 'textobject'){
    $callback .= 'type';
  }
  //@todo: place in walymportlog module
  if(variable_get('wallymport_debug',0) or TRUE){
    $log['log'][] = 'process message: '.$dom->nodeName;
  }
  if ($node != NULL){
    module_invoke_all('wimprt_process_message',$node,$dom,$log,'before_process');
    // ?Posibility of extension with 'hook_wallymport_info' / CTools Plugin?
    if (function_exists("wimprt_process_object_".$callback)){
      call_user_func("wimprt_process_object_".$callback, &$node, $object, &$log, $preview);
    }
    module_invoke_all('wimprt_process_message', $node, $dom, $log, 'after_process');
  }
  if (!empty($node) && !isset($node->rejected)) {
    if ($preview) {
      require_once './'.drupal_get_path('module', 'wallyedit').'/includes/Prenode.inc';
      $prenode = new Prenode();
      $prenode->ConvertNodeToPrenode($node);
      
      //require_once './'.drupal_get_path('module', 'wallyedit').'/includes/callback_preview.inc';
      $prenode->cache_name = 'wallymport_preview_'.$prenode->form_state['cache name'];
      $form_state_prenode = $prenode->setFormState();
      $form_state_prenode['DoNotPopulate'] = TRUE;
      ctools_object_cache_set('prenode', $prenode->cache_name, $form_state_prenode);
      return $prenode->cache_name;
    } else {
      //watchdog('Wallymport end creation', time().' '.print_r($node, TRUE), array(), WATCHDOG_NOTICE);
      node_save($node);
    }
  } else {
    wallycontenttypes_delete_orphans_objects($node);
  }

  $log['node'] = array('current' => $node, 'next' => $log['node']);
  
  return;
}
/**
 * Create or load a object depending of external reference.
 * 
 * If no node match the external reference, a new node is created.
 * 
 * @param $dom
 *   The DomDocument who is being proceed (A DomElement)
 * @param $log
 *   The log object build recursively in different operation. this could track error in the process and build nodes.
 *
 * @return
 *   A node object
 */
function wimprt_create_or_load_message_tme($dom, &$log, $callback){

  $node = NULL;
  $type = 'wally_'.$callback;
  $ext_ref = _wimprthelp_plain_text($dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue);
  
  if ($node_temp = wallytoolbox_get_node_by_cckfield(array("field_externalreference" => $ext_ref), $type)){
    $node = $node_temp[0];
  }
  
  return $node;
}
/**
 * Create or load a article package depending of external reference.
 * 
 * If no node match the external reference, a new node is created.
 * 
 * @param $dom
 *   The DomDocument who is being proceed (A DomElement)
 * @param $log
 *   The log object build recursively in different operation. this could track error in the process and build nodes.
 *
 * @return
 *   A node object
 */
function wimprt_create_or_load_package_articlepackagetype($dom,&$log) {
  $node = new stdClass;
  $node->type = 'wally_articlepackage';
  $ext_ref = _wimprthelp_plain_text($dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue);
  
  //watchdog('ext ref', $ext_ref);
  if($node_temp = wallytoolbox_get_node_by_cckfield(array("field_externalreference"=>$ext_ref),"wally_articlepackage")){
    $node = $node_temp[0];
  }
  //watchdog('Wallymport pack creation', time().' '.print_r($node, TRUE), array(), WATCHDOG_NOTICE);
  //watchdog('Wallymport create articlepackage', 'Node : '.print_r($node, TRUE), array(), WATCHDOG_NOTICE);
  //watchdog('Wallymport create articlepackage', 'Dom : '.print_r(dom2array($dom), TRUE), array(), WATCHDOG_NOTICE);
  
  $uid = variable_get('wallymport_defaultuser',1);
  $node->uid = $uid;

  return $node;
}

/**
 * Create or load a gallery package depending of external reference.
 * 
 * If no node match the external reference, a new node is created.
 * 
 * @param $dom
 *   The DomDocument who is being proceed (A DomElement)
 * @param $log
 *   The log object build recursively in different operation. this could track error in the process and build nodes.
 *
 * @return
 *   A node object
 */
function wimprt_create_or_load_package_gallerypackagetype($dom,&$log){
  $node = new stdClass;
  $node->type = 'wally_gallerypackage';

  $ext_ref = _wimprthelp_plain_text($dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue);
  if($node_temp = wallytoolbox_get_node_by_cckfield(array("field_externalreference"=>$ext_ref),"wally_gallerypackage")){
    $node = $node_temp[0];
  }

  $uid = variable_get('wallymport_defaultuser',1);
  $node->uid = $uid;

  return $node;
}

/**
 * Create or load a poll package depending of external reference.
 * 
 * If no node match the external reference, a new node is created.
 * 
 * @param $dom
 *   The DomDocument who is being proceed (A DomElement)
 * @param $log
 *   The log object build recursively in different operation. this could track error in the process and build nodes.
 *
 * @return
 *   A node object
 */
function wimprt_create_or_load_package_pollpackagetype($dom,&$log){
  $node = new stdClass;
  $node->type = 'wally_pollpackage';

  $ext_ref = _wimprthelp_plain_text($dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue);
  if($node_temp = wallytoolbox_get_node_by_cckfield(array("field_externalreference"=>$ext_ref),"wally_pollpackage")){
    $node = $node_temp[0];
  }

  $uid = variable_get('wallymport_defaultuser',1);
  $node->uid = $uid;

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
function wimprt_process_package_articlepackagetype(&$node, $dom_element, &$log, $preview = FALSE){
  
  //Map XML Attributes 
  $node->field_packageid[0]['value'] = $dom_element->getAttributeNode("PackageID")->value;
  if ($dom_element->hasAttribute("PublicationDate")) {
    $node->field_publicationdate[0] = _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("PublicationDate")->value);
  } else {
    $node->field_publicationdate[0] = _wimprthelp_date_to_ccktime(date("Y-m-dTH:i:s"));
  }
  if($dom_element->hasAttribute("CreationDate"))
    $node->field_creationdate[0] = _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("CreationDate")->value);
  if($dom_element->hasAttribute("LastUpdateDate"))
    $node->changed = _wimprthelp_date_to_timestamp($dom_element->getAttributeNode("LastUpdateDate")->value);
  if($dom_element->hasAttribute("EmbargoDate")) {
    $node->field_embargodatetime[0] = _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("EmbargoDate")->value);
    if (strtotime($node->field_embargodatetime[0]['value']) > time())
      $node->status = 0;
    else
      $node->status = 1;
  }
  if($dom_element->hasAttribute("UnPublishDate")) {
    $node->field_unpublishdate[0] = _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("UnPublishDate")->value);
    if (strtotime($node->field_unpublishdate[0]['value']) < time()){
      $node->status = 0;
    }
  }
  $node->language = $dom_element->getAttributeNode("Language")->value;
  //jusque ici ok
  //Map XML element
  wimprt_process_package_default($node, $dom_element, $log, $preview);
  if (!isset($node->field_packagelayout[0]) && $tax_term = taxonomy_get_term_by_name('Article')){
    $node->field_packagelayout[]['value'] = $tax_term[0]->tid;
  }
}

/**
 * Process gallery package from DOM.
 * 
 * MAP fields of the gallery package content type
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
function wimprt_process_package_galleryPackageType(&$node, $dom_element, &$log, $preview = FALSE){
  
  //Map XML Attributes 
  $node->field_packageid[0]['value'] = $dom_element->getAttributeNode("PackageID")->value;
  if ($dom_element->hasAttribute("PublicationDate")) {
    $node->field_publicationdate[0] = _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("PublicationDate")->value);
  } else {
    $node->field_publicationdate[0] = _wimprthelp_date_to_ccktime(date("Y-m-dTH:i:s"));
  }
  
  if($dom_element->hasAttribute("CreationDate"))
    $node->field_creationdate[0] = _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("CreationDate")->value);
  if($dom_element->hasAttribute("LastUpdateDate"))
    $node->changed = _wimprthelp_date_to_timestamp($dom_element->getAttributeNode("LastUpdateDate")->value);
  if($dom_element->hasAttribute("EmbargoDate")) {
    $node->field_embargodatetime[0] = _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("EmbargoDate")->value);
    if (strtotime($node->field_embargodatetime[0]['value']) > time())
      $node->status = 0;
    else
      $node->status = 1;
  }
  if($dom_element->hasAttribute("UnPublishDate")) {
    $node->field_unpublishdate[0] = _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("UnPublishDate")->value);
    if (strtotime($node->field_unpublishdate[0]['value']) < time())
      $node->status = 0;
  }
  
  //Map XML element
  wimprt_process_package_default($node, $dom_element, $log, $preview);
  
  if (!isset($node->field_packagelayout[0]) && $tax_term = taxonomy_get_term_by_name('Article'))
    $node->field_packagelayout[]['value'] = $tax_term[0]->tid;
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
function wimprt_process_package_pollpackagetype(&$node, $dom_element, &$log, $preview = FALSE){
  
  //Map XML Attributes 
  $node->field_packageid[0]['value'] = $dom_element->getAttributeNode("PackageID")->value;
  if ($dom_element->hasAttribute("PublicationDate")) {
    $node->field_publicationdate[0] = _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("PublicationDate")->value);
  } else {
    $node->field_publicationdate[0] = _wimprthelp_date_to_ccktime(date("Y-m-dTH:i:s"));
  }
  
  if($dom_element->hasAttribute("CreationDate"))
    $node->field_creationdate[0] = _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("CreationDate")->value);
  if($dom_element->hasAttribute("LastUpdateDate"))
    $node->changed = _wimprthelp_date_to_timestamp($dom_element->getAttributeNode("LastUpdateDate")->value);
  if($dom_element->hasAttribute("EmbargoDate")) {
    $node->field_embargodatetime[0] = _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("EmbargoDate")->value);
    if (strtotime($node->field_embargodatetime[0]['value']) > time())
      $node->status = 0;
    else
      $node->status = 1;
  }
  if($dom_element->hasAttribute("UnPublishDate")) {
    $node->field_unpublishdate[0] = _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("UnPublishDate")->value);
    if (strtotime($node->field_unpublishdate[0]['value']) < time())
      $node->status = 0;
  }
  
  //Map XML element
  wimprt_process_package_default($node, $dom_element, $log, $preview);
  
  if (!isset($node->field_packagelayout[0]) && $tax_term = taxonomy_get_term_by_name('Article'))
    $node->field_packagelayout[]['value'] = $tax_term[0]->tid;
}

/**
 * Process field SourceXML from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_sourcexml(&$node,$dom,&$log){
  $dom_sourcexml = new DOMDocument();
  $node_sourcexml = $dom_sourcexml->importNode($dom,TRUE);
  $dom_sourcexml->appendChild($node_sourcexml);
  $value = $dom_sourcexml->saveXML($dom_sourcexml);

  $node->field_sourcexml[0]['value'] = $value;
}

/**
 * Process field Package Title from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_packagetitle(&$node,$dom,&$log){
  $text = $dom->nodeValue;
  $node->title = _wimprthelp_plain_text($text);
}

/**
 * Process field Package Description from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_packagedescription(&$node,$dom,&$log){
  $value = $dom->nodeValue;
  $node->field_objectdescription[0]['value'] = _wimprthelp_plain_text($value);
}

/**
 * Process field Internal Reference from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_internalreference(&$node,$dom,&$log){
  $value = $dom->nodeValue;
  $node->field_internalreference[0]['value'] = _wimprthelp_plain_text($value);
}

/**
 * Process field External Reference from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_externalreference(&$node,$dom,&$log){
  $value = $dom->nodeValue;
  $node->field_externalreference[0]['value'] = _wimprthelp_plain_text($value);
}

/**
 * Process field Embeded Objects Order from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_embeddedobjectsorder(&$node,$dom,&$log){
  $value = $dom->nodeValue;
  $node->embeddedobjectsorder = _wimprthelp_plain_text($value);
}

/**
 * Process field ProductID from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_productid(&$node,$dom,&$log){
  $value = $dom->nodeValue;
  $node->field_productid[0]['value'] = $value;
}

/**
 * Process field Destinations from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_destinations(&$node,$dom,$log) {
  if (variable_get('wallymport_debug',0) or TRUE){
    $log['log'][] = 'process Destinations: '.$dom->nodeName;
  }
  $term = array();
  $result = array(); 
  $vid = variable_get('wallymport_destinationpath',NULL);
  if ($vid) {
	$i = 0;
	foreach($dom->getElementsByTagName('Destination') as $destination) {
	  $path = $destination->getElementsByTagName("DestinationPath")->item(0)->nodeValue;
	  $terms = wallytoolbox_taxonomy_get_term_by_path($path, $vid);
	  if ($terms) {
	    // As we check the taxonomy full path and specify a vocabulary
		// we can only have one result.
		$result[$i]['tid'] = $terms[0]->tid;
		$result[$i]['target'] = $destination->getAttributeNode("Position")->value;
		$result[$i]['layout'] = $destination->getAttributeNode("DocumentLayout")->value;
		$result[$i]['rank'] = $destination->getAttributeNode("DestinationRank")->value;
			
		$actual_dest = isset($node->field_destinations[$i]) ? $node->field_destinations[$i] : array('tid' => '', 'target' => '', 'layout' => '', 'rank' => '');
		if ($result[$i]['tid'] == $actual_dest['tid'] && $result[$i]['target'] == $actual_dest['target'] && $result[$i]['layout'] == $actual_dest['layout'] && $result[$i]['rank'] == $actual_dest['rank']) {
		  $result[$i]['rank'] = 'DNC'.$result[$i]['rank'];
		  $result[$i]['time_rank'] = $actual_dest['time_rank'];
		}
	  } else {
		drupal_set_message('Path "'.$path.'" not found in vocabulary ('.$vid.'). Destination rejected.','notice');	
	  }
	  $i++;
	}

	// If no destinations at all > Package can't be created.
	if (count($result)) {
	  $node->field_destinations = $result;
	} else {
	  // @TODO: FATAL ERROR - PACKAGE DONE - NEED CHECK
      $node->rejected = '';
	  drupal_set_message('No destination AT ALL for current package. Package rejected.','error');
	  return '';
	}
  } else {
	drupal_set_message('Destination Path vocabulary not set. See settings.','error');
	return '';
  }
}

/**
 * Process field Editions from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_editions(&$node,$dom,$log) {
	if(variable_get('wallymport_debug',0) or true){
		$log['log'][] = 'process Editions: '.$dom->nodeName;
  }

	$result = array();
	$valid = (bool) false;
	$terms = array(); 
	$vid=variable_get('wallymport_edition',null);
	if ($vid) {
		// Read all terms from vocabulary and set if to False (default);
		$terms = taxonomy_get_tree($vid);
		foreach ($terms as $term) {
			$result[$term->name]["Publish"]=false;
			$result[$term->name]["EditionTid"]=$term->tid;
			$result[$term->name]["EditionID"]=$term->name;
		}

		// Get edition from DOM & update array.
		foreach($dom->getElementsByTagName('Edition') as $edition) {
			$editionID = $edition->getAttributeNode("EditionID")->value;
			$value = wallytoolbox_toboolean($edition->nodeValue);
			if (strtolower($editionID)=="all") {
				foreach ($terms as $term) $result[$term->name]["Publish"]=$value;
				$valid = $value;
				drupal_set_message('Editions ALL found. All editions set to '.$value.'. Editions iteration terminated.','notice');
				break;
			} else {
				if (array_key_exists($editionID, $result)) {
					$valid = ($valid || $value);
					$result[$editionID]['Publish']=$value;
				} else {
					drupal_set_message('Edition: "'.$editionID.'" not found in vocabulary. Edition rejected','error');
				}
			}
		}
		if ($valid) { 
      $node->field_editions=array(array("value"=>serialize($result)));
		} else {
      //$node->rejected = '';
			drupal_set_message('ALL Edition set to false. Package rejected','error');
			// TODO: FATAL ERROR - PACKAGE - DONE NEED CHECK
		}
		
	} else {
		drupal_set_message('Edition vocabulary not set. See settings.','error');
		return '';
	}
}

/**
 * Process field Channels from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_channels(&$node,$dom,$log) {

  if(variable_get('wallymport_debug',0) or true){
	$log['log'][] = 'process Channels: '.$dom->nodeName;
  }
  $result = array();
  // Get edition from DOM & update array.
  foreach($dom->getElementsByTagName('Channel') as $channel) {
    $channelName = $channel->getAttributeNode("Name")->value;
    $result[] = $channelName;
  }

  if (!empty($result)) { 
    $node->field_channels = array(array("value" => serialize($result)));
  } else {
    $node->rejected = '';
    drupal_set_message('No channels found. Package rejected','error');
    // TODO: FATAL ERROR - PACKAGE - DONE NEED CHECK
  }
}

/**
 * Process field Comments Allowed from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_commentsallowed(&$node,$dom,&$log){
	$result = COMMENT_NODE_DISABLED;

	switch ($dom->nodeValue) {
		case "Disabled":
			$result = COMMENT_NODE_DISABLED; 
			break;
		case "Read Only":
			$result = COMMENT_NODE_READ_ONLY; 
			break;
		case "Read-Write":
			$result = COMMENT_NODE_READ_WRITE; 
			break;
		default:
			$result = COMMENT_NODE_DISABLED; 
			break;
	}
	$node->comment = $result;
	return $result;
}

/**
 * Process field Free Access from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
 * Anything else then a booelan true value will 
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
function wimprt_set_field_freeaccess(&$node,$dom,&$log){
  (wallytoolbox_toboolean($dom->nodeValue)) ? 
    $node->field_freeaccess[0]['value'] = "Free" : 
    $node->field_freeaccess[0]['value'] = NULL; 
}

/**
 * Process field AutoPublish from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
 * Anything else then a booelan true value will 
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
function wimprt_set_field_autopublish(&$node, $dom, &$log){
  //@todo ???
}


/**
 * Process field PackageLayout from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_packagelayout(&$node,$dom,&$log){
  $value = $dom->nodeValue;
  $node->field_packagelayout = array();

  if (variable_get('wallymport_debug',0) or TRUE) {
	$log['log'][] = 'process PackageLayout: '.$dom->nodeName;
  }

  if (is_numeric($value)) {
    $node->field_packagelayout[]['value'] = $value;
  } elseif($tax_term = taxonomy_get_term_by_name($value)) {
    $node->field_packagelayout[]['value'] = $tax_term[0]->tid;
  } else {
    if ($tax_term = taxonomy_get_term_by_name('Article')) {
      $node->field_packagelayout[]['value'] = $tax_term[0]->tid;
    } else {
	  drupal_set_message('PackageLayout not found: '.$value.'.','notice');
    }
  }
}

/**
 * Process field Summary from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_summary(&$node,$dom,&$log){
  $value = $dom->nodeValue;
  $node->field_summary[0]['value']= _wimprthelp_plain_text($value);
}

/**
 * Process field ExternalURI from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_externaluri(&$node,$dom,&$log){
  $value = $dom->nodeValue;
  $node->field_externaluri = array();

	if(variable_get('wallymport_debug',0) or true) {
		$log['log'][]='process ExternalURI: '.$dom->nodeName;
  }

	$uri_response = drupal_http_request($value);
  if ($uri_response->status_message == 'OK') {
    $node->field_externaluri[0]['value'] = $value;
  }
}

/**
 * Process field PackageCopyright from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_packagecopyright(&$node,$dom,&$log){
  $value = $dom->nodeValue;
  $node->field_packagecopyright = array();

	if(variable_get('wallymport_debug',0) or true) {
		$log['log'][]='process PackageCopyright: '.$dom->nodeName;
  }

	$node->field_packagecopyright[] = $value;
}

/**
 * Process field Main Story from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_mainstory(&$node, $dom, &$log, $preview = FALSE) {
  // Process the object & getting NID
  $object = wimprt_process_object($dom, $log, 'TextObject', $preview);
  $node->field_mainstory[0]['nid'] = $object->nid;
  if ($preview) {
    $node->field_mainstory_nodes[0] = $object;
  }
}

/**
 * Process field Main Object from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_mainobject(&$node, $dom, &$log, $preview = FALSE) {
  if($dom->hasAttribute('xsi:type')){
    $object=new stdClass;
    if(strtolower($dom->getAttributeNode('xsi:type')->value)=='photoobjecttype')
      $object=wimprt_process_object($dom, $log, 'photoobjecttype', $preview);
    elseif(strtolower($dom->getAttributeNode('xsi:type')->value)=='audioobjecttype')
      $object=wimprt_process_object($dom, $log, 'audioobjecttype', $preview);
    elseif(strtolower($dom->getAttributeNode('xsi:type')->value)=='MultimediaObjectType')
      $object=wimprt_process_object($dom, $log, 'digitalobjecttype', $preview);
    elseif(strtolower($dom->getAttributeNode('xsi:type')->value)=='videoobjecttype')
      $object=wimprt_process_object($dom, $log, 'videoobjecttype', $preview);
        
    if($object->nid) {
      $node->field_mainobject[0]['nid']=$object->nid;
      if ($preview)
        $node->field_mainobject_nodes[0]=$object;
    }
  }
}
  
/**
 * Process field Poll from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_poll(&$node, $dom, &$log, $preview = FALSE){
  // Process the object & getting NID
	$object=wimprt_process_object($dom, $log, 'pollobjecttype', $preview);
	$node->field_mainpoll[0]['nid']=$object->nid;
	if ($preview)
	  $node->field_mainpoll_nodes[0]=$object;
}
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
function wimprt_set_field_EmbeddedContent(&$node, $dom, &$log, $preview = FALSE){
  // Embeded objects
  $results = array(); 
  $node->field_embededobjects = array();
  $node->field_embededobjects_nodes = array();
  $arraytemporaire = array();
  $embedobjects = $dom->getElementsByTagName('EmbeddedObjects')->item(0);
  foreach(wimprt_process_list($embedobjects, $log, 'object', $preview) as $result){
    if (!array_key_exists($result->nid, $results) || $preview) {
      $results[$result->nid] = $result->nid;
      $arraytemporaire[$result->embeddedobjectsorder][]['nid'] = $result->nid;
      if ($preview){
        $node->field_embededobjects_nodes[] = $result;
      }
    }
  }
  //Place object in the correct order (depend of the embeddedobjectorder)
  foreach ($arraytemporaire as $array_key => $array_value){
    foreach ($array_value as $array_valuee){
      array_push($node->field_embededobjects,$array_valuee);
    }
  }
  
  // Linkslists objects
  $links = array(); 
  $node->field_linkedobjects = array();
  $node->field_linkedobjects_nodes = array();
  $linklists = $dom->getElementsByTagName('LinksLists')->item(0);
  $linklist = $dom->getElementsByTagName('LinksList')->item(0);
  if ($dom->getElementsByTagName('LinksList')->item(0)){
    if ($linklist->getElementsByTagName('Links')->item(0)->nodeValue != NULL){
      foreach (wimprt_process_list($linklists, $log, 'object', $preview) as $result){
        if (!array_key_exists($result->nid, $links) || $preview) {
          $links[$result->nid] = $result->nid;
          $node->field_linkedobjects[]['nid'] = $result->nid;
          if ($preview){
            $node->field_linkedobjects_nodes[] = $result;
          }
        }
      }
    } else {
      drupal_set_message('WARNING: No LinksList found in LinksLists element. Skipping LinksLists process.','notice');
    }
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
function wimprt_objecttype_preprocess(&$node,$dom,$preview){
  $node->field_objectid[0]['value'] = $dom->getAttributeNode("ObjectID")->value;
  if ($dom->hasAttribute("ObjectDescription")){
    $node->object_description = $dom->getAttributeNode("ObjectDescription")->value;
  }
  $node->field_publicationdate[0] = _wimprthelp_date_to_ccktime($dom->getAttributeNode("PublicationDate")->value);
  $node->created = _wimprthelp_date_to_timestamp($dom->getAttributeNode("CreationDate")->value);
  if ($dom->hasAttribute("LastUpdateDate")){
    $node->changed = _wimprthelp_date_to_timestamp($dom->getAttributeNode("LastUpdateDate")->value);
  }
  if ($preview){
    $node->rejected = TRUE;
  }
}

/**
 * Create or load a text object depending of external reference.
 * 
 * If no node match the external reference, a new node is created.
 * 
 * @param $dom
 *   The DomDocument who is being proceed (A DomElement)
 * @param $log
 *   The log object build recursively in different operation. this could track error in the process and build nodes.
 *
 * @return
 *   A node object
 */
function wimprt_create_or_load_object_textobject($dom,&$log){
  $node = new stdClass;
  $node->type = 'wally_textobject';
  
  $ext_ref = _wimprthelp_plain_text($dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue);
  if ($node_temp = wallytoolbox_get_node_by_cckfield(array("field_externalreference" => $ext_ref),'wally_textobject')){
    $node = $node_temp[0];
  }
  
  $uid = variable_get('wallymport_defaultuser',1);
  $node->uid = $uid;
  
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
function wimprt_process_object_textobject (&$node, $dom_element, &$log, $preview){
  //Map XML Attributes 
  //put common attributes
  wimprt_objecttype_preprocess($node, $dom_element, $preview);
  wimprt_process_object_default($node, $dom_element, $log);
  //Exception for TextObject
  if ($dom_element->getElementsByTagName("TextTitle")->length) {
    $node->title = $dom_element->getElementsByTagName("TextTitle")->item(0)->nodeValue;
  }
  
  if ($dom_element->getElementsByTagName("Title")->length) {
    $node->field_objecttitle = array(array("value" => $dom_element->getElementsByTagName("Title")->item(0)->nodeValue));
  } 
}

/**
 * Process field Poll Questions from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_pollquestions(&$node,$dom,&$log){
  $i = 0;
    
  foreach($dom->getElementsByTagName("PollQuestion") as $pollquestion){
    $node->field_poll[$i]['active'] = 1;
    $feature = array();
    $choice = 1;
    if ($pollquestion->hasAttribute("Choice")){
      if($pollquestion->getAttributeNode("Choice")->value === "radioButton"){
        $choice = 0;
      }
    }
    $feature['CancelVote_Allow'] = 1;
    $feature['ResultsAllow'] = 0;
    $feature['PollType'] = $choice;
    $node->field_poll[$i]['poll_features']=serialize($feature);
    foreach($pollquestion->getElementsByTagName("Question") as $question){
      $node->field_poll[$i]['question'] = $pollquestion->nodeValue;
    }
    $j = 0;
    foreach($pollquestion->getElementsByTagName("Answer") as $answer){
      $answers[$j]['choice'] = $answer->nodeValue;
      $answers[$j]['votes'] = NULL;
      $j += 1;
    }
    if($j > 0) {
      $node->field_poll[$i]['choice'] = serialize($answers);
    }
    $i += 1;
  }
}

/**
 * Create or load a poll object depending of external reference.
 * 
 * If no node match the external reference, a new node is created.
 * 
 * @param $dom
 *   The DomDocument who is being proceed (A DomElement)
 * @param $log
 *   The log object build recursively in different operation. this could track error in the process and build nodes.
 *
 * @return
 *   A node object
 */
function wimprt_create_or_load_object_pollobjecttype($dom,&$log){
  $node=new stdClass;
  $node->type='wally_pollobject';
  $ext_ref=_wimprthelp_plain_text($dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue);
  if($node_temp=wallytoolbox_get_node_by_cckfield(array("field_externalreference"=>$ext_ref),'wally_pollobject')){
    $node=$node_temp[0];
  }
  
  $uid=variable_get('wallymport_defaultuser',1);
  $node->uid=$uid;
  
  return $node;
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
function wimprt_process_object_pollobjecttype(&$node,$dom,&$log,$preview){
  
  static $proceed_file_path;
  //put common attribute
  wimprt_objecttype_preprocess($node,$dom,$preview);

  wimprt_process_object_default($node,$dom,$log);
   
 ///Exception for TextObject
  $date1=null;
  $date2=null;
  if ($dom->getElementsByTagName("DateOpen")->length) 
    $date1=$dom->getElementsByTagName("DateOpen")->item(0)->nodeValue;
  if($dom->getElementsByTagName("DateClose")->length) 
    $date2=$dom->getElementsByTagName("DateClose")->item(0)->nodeValue;
  $node->field_polldate[0]=_wimprthelp_polldate_to_ccktime($date1,$date2);

  return $node;

}

/**
 * Create or load a photo object depending of external reference.
 * 
 * If no node match the external reference, a new node is created.
 * 
 * @param $dom
 *   The DomDocument who is being proceed (A DomElement)
 * @param $log
 *   The log object build recursively in different operation. this could track error in the process and build nodes.
 *
 * @return
 *   A node object
 */
function wimprt_create_or_load_object_photoobjecttype($dom,&$log){
  $node=new stdClass;
  $node->type='wally_photoobject';

  $ext_ref =_wimprthelp_plain_text($dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue);
  if ($node_temp = wallytoolbox_get_node_by_cckfield(array("field_externalreference"=>$ext_ref),"wally_photoobject")){
    $node = $node_temp[0];
  }

  $uid = variable_get('wallymport_defaultuser',1);
  $node->uid = $uid;

  return $node;
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
function wimprt_process_object_photoobjecttype(&$node,$dom,&$log,$preview){
  static $procced_file_path;
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
  wimprt_objecttype_preprocess($node,$dom,$preview);

  //if (!isset($node->field_summary[0]['value']) || empty($node->field_thumbnail[0]))
  $node->field_summary[0]['value'] = _wimprthelp_plain_text($value);
  
  $dom_thumbnails = $dom->getElementsByTagName('Thumbnail');
  $thumb_uri = $dom->getElementsByTagName('Thumbnail')->item(0) ? $dom->getElementsByTagName('Thumbnail')->item(0)->getAttribute('URI') : '';
  //if (!isset($node->field_thumbnail[0]) || count($node->field_thumbnail[0])<8)
  $node->field_thumbnail[0] = _wimprthelp_create_file_from_uri($thumb_uri);
    
    
  $file_uri = $dom->getElementsByTagName('FileURI')->item(0)->nodeValue;
  //if (!isset($node->field_photofile[0]) || count($node->field_photofile[0])<8)
  $rdpartyMatch = emfield_parse_embed(null,$file_uri, 'emimage');  
  if (empty($rdpartyMatch)){//No a 3rdparty
    $node->field_photofile[0] = _wimprthelp_create_file_from_uri($file_uri);
  } else {//Is a 3rd party
    $node->field_photo3rdparty[0]['embed'] = $file_uri;
  }
  wimprt_process_object_default($node, $dom, $log);

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
 * Create or load a audio object depending of external reference.
 * 
 * If no node match the external reference, a new node is created.
 * 
 * @param $dom
 *   The DomDocument who is being proceed (A DomElement)
 * @param $log
 *   The log object build recursively in different operation. this could track error in the process and build nodes.
 *
 * @return
 *   A node object
 */
function wimprt_create_or_load_object_audioobjecttype($dom,&$log){
  $node=new stdClass;
  $node->type='wally_audioobject';

  $ext_ref=_wimprthelp_plain_text($dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue);
  if($node_temp=wallytoolbox_get_node_by_cckfield(array("field_externalreference"=>$ext_ref),"wally_audioobject")){
    $node=$node_temp[0];
  }

  $uid=variable_get('wallymport_defaultuser',1);
  $node->uid=$uid;

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
function wimprt_process_object_audioobjecttype(&$node,$dom,&$log,$preview){
  static $procced_file_path;
  $value = $dom->nodeValue;
  
  //put common attributes
  wimprt_objecttype_preprocess($node,$dom,$preview);

  
  $file_uri = $dom->getElementsByTagName('FileURI')->item(0)->nodeValue;

  $rdpartyMatch = emfield_parse_embed(null,$file_uri, 'emaudio');  
  if (empty($rdpartyMatch)){//No a 3rdparty
    $node->field_audiofile[0] = _wimprthelp_create_file_from_uri($file_uri);
  } else {//Is a 3rd party
    $node->field_audio3rdparty[0]['embed']=$file_uri;
  }

  //Link
  $link_title  = $dom->getElementsByTagName('LinkTitle')->item(0)->nodeValue;
  $link_uri = $dom->getElementsByTagName('LinkURI')->item(0)->nodeValue;
  if ($link_title != NULL | $link_uri != NULL){
    $node->field_link[0]['url'] = $link_uri;
    $node->field_link[0]['title'] = $link_title;
  }
  wimprt_process_object_default($node,$dom,$log);
  
  return $node;

}

/**
 * Create or load a video object depending of external reference.
 * 
 * If no node match the external reference, a new node is created.
 * 
 * @param $dom
 *   The DomDocument who is being proceed (A DomElement)
 * @param $log
 *   The log object build recursively in different operation. this could track error in the process and build nodes.
 *
 * @return
 *   A node object
 */
function wimprt_create_or_load_object_videoobjecttype($dom,&$log){
  $node = new stdClass;
  $node->type = 'wally_videoobject';

  $ext_ref = _wimprthelp_plain_text($dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue);
  if ($node_temp = wallytoolbox_get_node_by_cckfield(array("field_externalreference" => $ext_ref),"wally_videoobject")){
    $node = $node_temp[0];
  }

  $uid = variable_get('wallymport_defaultuser',1);
  $node->uid = $uid;

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
function wimprt_process_object_videoobjecttype(&$node,$dom,&$log,$preview){
  static $procced_file_path;
  $value = $dom->nodeValue;
  
  //put common attributes
  wimprt_objecttype_preprocess($node,$dom,$preview);

  $file_uri = $dom->getElementsByTagName('FileURI')->item(0)->nodeValue; 
  $rdpartyMatch = emfield_parse_embed(null,$file_uri, 'emvideo');  
  if (empty($rdpartyMatch)){//No a 3rdparty
    $node->field_videofile[0] = _wimprthelp_create_file_from_uri($file_uri);
  } else {//Is a 3rd party
    $node->field_video3rdparty[0]['embed'] = $file_uri;
  }
  
  $node->field_link[0]['url'] = $dom->getElementsByTagName('LinkURI')->item(0)->nodeValue;
  $node->field_link[0]['title'] = $dom->getElementsByTagName('LinkTitle')->item(0)->nodeValue;
  wimprt_process_object_default($node,$dom,$log);
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
 * Create or load a digital object depending of external reference.
 * 
 * If no node match the external reference, a new node is created.
 * 
 * @param $dom
 *   The DomDocument who is being proceed (A DomElement)
 * @param $log
 *   The log object build recursively in different operation. this could track error in the process and build nodes.
 *
 * @return
 *   A node object
 */
function wimprt_create_or_load_object_digitalobjecttype($dom,&$log){
  $node = new stdClass;
  $node->type = 'wally_digitalobject';

  $ext_ref = $dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue;
  if ($node_temp = wallytoolbox_get_node_by_cckfield(array("field_externalreference"=>$ext_ref),"wally_digitalobject")){
    $node = $node_temp[0];
  }

  $uid = variable_get('wallymport_defaultuser',1);
  $node->uid = $uid;

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
function wimprt_process_object_digitalobjecttype(&$node,$dom,&$log,$preview){
  static $procced_file_path;
  $value = $dom->nodeValue;
  
  //put common attributes
  wimprt_objecttype_preprocess($node,$dom,$preview);


  $file_uri = $dom->getElementsByTagName('FileURI')->item(0)->nodeValue;
  $rdpartyMatch = emfield_parse_embed(null,$file_uri, 'emother');  
  if (empty($rdpartyMatch)){//No a 3rdparty
    $node->field_objectfile[0] = _wimprthelp_create_file_from_uri($file_uri);
  } else {//Is a 3rd party
    $node->field_object3rdparty[0]['embed'] = $file_uri;
  }
  
 
  //$node->field_link[0]['url']= $dom->getElementsByTagName('LinkURI')->item(0)->nodeValue;
  //$node->field_link[0]['title']= $dom->getElementsByTagName('LinkTitle')->item(0)->nodeValue;
  wimprt_process_object_default($node,$dom,$log);
  
  return $node;

}

/**
 * Process field Thumbnail from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_thumbnail(&$node,$dom,&$log){
  $thumbnail = $dom->nodeValue;
  $node->field_thumbnail[0] = _wimprthelp_create_file_from_uri($thumbnail);
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
function wimprt_process_type_link(&$node,$dom,&$log,$preview){
  wimprt_typetype_preprocess(&$node,$dom,$preview);
  
  //$node->type='wally_linktype';
  wimprt_process_type_default($node,$dom,$log);

  $uri=$dom->getElementsByTagName('URI')->item(0);
  $uri_value=$uri->nodeValue;
  $uri=parse_url($uri->nodeValue);
  if($uri['scheme']=="externalRef"){
    $ext_ref=$uri['path'];
    if ($link = wallytoolbox_get_node_by_cckfield(array("field_externalreference"=>$ext_ref))) {
      if (count($link)==1) {
        $node->field_internal_link[0]['nid'] = $link[0]->nid;
      }
    }
  } else{
    $node->field_link_item[0]['title']=$node->title;
    $node->field_link_item[0]['url']=$uri_value;
    $node->field_link_item[0]['attributes']=array('target'=>'_blank');
  }
}


/**
 * Create or load a links list object depending of external reference.
 * 
 * If no node match the external reference, a new node is created.
 * 
 * @param $dom
 *   The DomDocument who is being proceed (A DomElement)
 * @param $log
 *   The log object build recursively in different operation. this could track error in the process and build nodes.
 *
 * @return
 *   A node object
 */
function wimprt_create_or_load_object_linkslist($dom,&$log){
  $node=new stdClass;
  $node->type='wally_linkslistobject';

  $ext_ref=_wimprthelp_plain_text($dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue);
  if($node_temp=wallytoolbox_get_node_by_cckfield(array("field_externalreference"=>$ext_ref),"wally_linkslistobject")){
    $node=$node_temp[0];
  }
  
  $uid=variable_get('wallymport_defaultuser',1);
  $node->uid=$uid;

  return $node;
}

/**
 * Create or load a link type depending of external reference.
 * 
 * If no node match the external reference, a new node is created.
 * 
 * @param $dom
 *   The DomDocument who is being proceed (A DomElement)
 * @param $log
 *   The log object build recursively in different operation. this could track error in the process and build nodes.
 *
 * @return
 *   A node object
 */
function wimprt_create_or_load_type_link($dom,&$log){
  $node=new stdClass;
  $node->type='wally_linktype';
  //je pense qu'il faut enlever ca car les liens n'ont pas de tag ExternalReference
  $ext_ref=_wimprthelp_plain_text($dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue);
  if($node_temp=wallytoolbox_get_node_by_cckfield(array("field_externalreference"=>$ext_ref),$node->type)){
    $node=$node_temp[0];
  }

  $uid=variable_get('wallymport_defaultuser',1);
  $node->uid=$uid;

  return $node;
}

/**
 * Process Linklist object from DOM.
 * 
 * MAP fields of the links list object content type
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
function wimprt_process_object_linkslist(&$node,$dom,&$log){
  wimprt_process_object_default($node,$dom,$log);
  
  $links=$dom->getElementsByTagName("Links")->item(0);
  foreach(wimprt_process_list($links,$log,'type') as $result){
    $node->field_links_list[]['nid']=$result->nid;
  }
}


/**
 * Process field Title from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_title(&$node,$dom,&$log){
  $value=$dom->nodeValue;
  $node->title= _wimprthelp_plain_text($value);
}

/**
 * Process field Rating from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_rating(&$node,$dom,&$log){
  $value=$dom->nodeValue;
  $vid=variable_get('wallymport_rating',null);
  $item=wallytoolbox_taxonomy_get_term_by_path($value,$vid);
  $node->field_rating[0]['value']= $item[0]->tid;
}

/**
 * Process field Text Barette from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_textbarette(&$node,$dom,&$log){
  $value = $dom->nodeValue;
  $node->field_textarette[0]['value'] = $value;
}

/**
 * Process field Text Foretitle from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_textforetitle(&$node,$dom,&$log){
  $value = $dom->nodeValue;
  $node->field_textforetitle[0]['value'] = $value;
}

/**
 * Process field Text Title from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_texttitle(&$node,$dom,&$log){
  $value = $dom->nodeValue;
  $node->title = $value;
}

/**
 * Process field Sub-Title from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_textsubtitle(&$node,$dom,&$log){
  $value = $dom->nodeValue;
  $node->field_textsubtitle[0]['value'] = $value;
}

/**
 * Process field Text Chapo from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_textchapo(&$node,$dom,&$log){
  $value = $dom->nodeValue;
  $node->field_textchapo[0]['value'] = $value;
}

/**
 * Process field Text Body from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_textbody(&$node,$dom,&$log){
  $value = $dom->nodeValue;
  $node->field_textbody[0]['value'] = $value;
}

/**
 * Process field Text Extract Medium from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_extractmedium(&$node,$dom,&$log){
  $value = $dom->nodeValue;
  $node->field_extractmedium[0]['value'] = $value;
}

/**
 * Process field Text Extract Short from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_extractshort(&$node,$dom,&$log){
  $value = $dom->nodeValue;
  $node->field_extractshort[0]['value'] = $value;
}

/**
 * Process field Copyright from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_copyright(&$node,$dom,&$log){
  $value = $dom->nodeValue;
  $node->field_copyright[0]['value'] = $value;
}

/**
 * Process field Copyright from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_byline(&$node,$dom,&$log){
  $value = $dom->nodeValue;
  $node->field_byline[0]['value'] = $value;
}

/**
 * Create or load a location type depending of external reference.
 * 
 * If no node match the external reference, a new node is created.
 * 
 * @param $dom
 *   The DomDocument who is being proceed (A DomElement)
 * @param $log
 *   The log object build recursively in different operation. this could track error in the process and build nodes.
 *
 * @return
 *   A node object
 */
function wimprt_create_or_load_type_location($dom,$log){	

  if (!($vid = variable_get('wallymport_location',NULL))) {
	// TO DO FATAL :: No location taxonomy vocabulary
	drupal_set_message('Location Taxonomy Vocabulary  not Found. See admin.','error');
	$node->rejected = TRUE;
	return NULL;
  }
  // Taxonomy TID (from vocabulary Locations).
  $term = $dom->getAttributeNode("Name")->value;
  if ($taxonomy = wallytoolbox_taxonomy_get_term_by_path($term, $vid)) {
	// Taxonomy term exist - fetch the tid.
	$taxonomy = $taxonomy[0]; 
	$tid = $taxonomy->tid;
	$result = wallytoolbox_get_node_by_tid($tid,"wally_locationtype");
	if ($result == NULL){
	  $taxonomy = taxonomy_get_term($tid);
	  $result = new stdClass();
	}
	elseif (count($result) > 1){
	  drupal_set_message('Found 2 or more node reference for LocationID: '.$lid.'.','error');
	  return NULL;
	} else {
      $result = $result[0];
	}
  } else {
	// New taxonomy term. Create & get the tid.
	$tid = wallytoolbox_add_taxonomy_term(_wimprthelp_plain_text($term), $vid, _wimprthelp_plain_text($term), 0);
	$taxonomy = taxonomy_get_term($tid);
	$result = new stdClass();
  };
  // Set the taxonomy field.
  $result->field_locationtaxonomy = array( array("value" => $tid) );
  // Set drupal node->taxonomy. 
  $result->taxonomy = array($tid => $taxonomy); 
  return $result;

}

/**
 * Process Location Type from DOM.
 * 
 * MAP fields of the location type content type
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
function wimprt_process_type_location(&$node,$dom,&$log,$preview){
  wimprt_typetype_preprocess(&$node,$dom,$preview);

  // UID
  $node->uid = variable_get('wallymport_defaultuser',1);

//  External Reference Supprimé
//  $node->field_externalreference = array(array("value"=>$dom->getAttributeNode("LocationID")->value)); 

  // Note Type
  $node->type = 'wally_locationtype';

  // Node Title 
  $node->title = $dom->getAttributeNode("Name")->value;

  // field_objectdescription supprimé 
  //$node->field_objectdescription = array(array("value" => _wimprthelp_plain_text($dom->getAttributeNode("LocationID")->value)));

  // field_address
  $addresses = $dom->getElementsByTagName("Address");
  if ($addresses->length) {
	$node->field_address = _wimprt_address_dom_to_ccklocation($addresses, $log, $node->title);
  }

  // location lat/long - In this case will re-write field_adress values.

  $locationgeo = $dom->getElementsByTagName("LocationGeo");
  if ($locationgeo->length) {
	$locationgeo = explode(",", $locationgeo->item(0)->nodeValue);
	$lat = $locationgeo[0]; 
	$lon = $locationgeo[1]; 
	if (isset($locationgeo[2])) {
	  $zoom = $locationgeo[2];
	} 
	$address_cpt = 0;
	// BAD >>> ARRAY et en plus peut etre multiple ? 
	if ($node->field_address) {
	  foreach ($node->field_address as $address) {
	    $address->latitude = $lat;
	    $address->longitude = $lon;
		$address->locpick["user_latitude"] = $lat;
		$address->locpick["user_longitude"] = $lon;
		$node->field_address[$address_cpt] = $address;
		$address_cpt++;  
	  }
	}
  }


  // Send back the node
  return($node);
}

/**
 * Process field Locations Medium from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_marker(&$node,$dom,&$log){

  $node->field_markers = array(); 

  if(variable_get('wallymport_debug',0) or true){
	$log['log'][] = 'process Marker: '.$dom->nodeName;
  }
  $node->field_markers[0] = array();
  $address = _wimprt_address_dom_to_ccklocation($dom->getElementsByTagName("Address"),$log);
  $node->field_markers[0] = $address[0];
  $node->field_markers[0]['name'] = $dom->getAttribute('Name');
  
  if($dom->getElementsByTagName("LocationGeo")->length){
    list($longitude,$latitude,$zoom) = explode(',',$dom->getElementsByTagName('LocationGeo')->item(0)->nodeValue);
    $node->field_markers[0]['locpick']['user_latitude'] = $latitude;
    $node->field_markers[0]['locpick']['user_longitude'] = $longitude;
  }
  return $results; 
}

/**
 * Process field Locations Medium from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_locations(&$node,$dom,&$log){

  $node->field_locations = array(); 

  if (variable_get('wallymport_debug',0) or TRUE){
	$log['log'][] = 'process Locations: '.$dom->nodeName;
  }
  $termes = $dom->getElementsByTagName("Term");
  $results = array();
  foreach ($termes as $terme){
    $results[] = wimprt_process_type($terme,$log,'location');
  }
 
  // Get Location Taxonomy for all nodes.
  $node->field_locations = array();
  foreach($results as $result) {
    if (isset($result->field_locationtaxonomy)) {
	  if ($result->field_locationtaxonomy[0]) {
	    $node->field_locations[] = array("value"=>$result->field_locationtaxonomy[0]["value"]);
		$node->taxonomy[$result->field_locationtaxonomy[0]["value"]] = taxonomy_get_term($result->field_locationtaxonomy[0]["value"]);  
      }
    }
  }
  return $results; 
}

/**
 * Process field Taxonomy from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_taxonomy(&$node,$dom,$log) {
  
  $result = array();
  if (variable_get('wallymport_debug',0) or TRUE) {
	$log['log'][] = 'process Taxonomy: '.$dom->nodeName;
  }
  $freetagtaxonomy_vid = variable_get('wallymport_freetagtaxonomy', NULL);
  if ($freetagtaxonomy_vid) {
	$vocabularies = $dom->getElementsByTagName("Vocabulary");
	if ($vocabularies->length) {
		
	  foreach($vocabularies as $vocabulary) {
	     
	    if (function_exists('wimprt_set_field_'.$vocabulary->getAttributeNode("VocabularyName")->value)){
	      if (isset($node->nid)){
	        $vid = _wimprthelp_getTaxonomyVidFromDamName($vocabulary->getAttributeNode("VocabularyName")->value);
	        wallytoolbox_taxonomy_unbindalltermofnodefromavocabulary($vocabulary->getAttributeNode("VocabularyName")->value, $node, $vid);
	      } 
	      call_user_func('wimprt_set_field_'.$vocabulary->getAttributeNode("VocabularyName")->value, $node, $vocabulary, $log);
	    } else { 
	      $vid = _wimprthelp_getTaxonomyVidFromDamName($vocabulary->getAttributeNode("VocabularyName")->value);
	      if ($vid != NULL){
	        if (isset($node->nid)){
  	          wallytoolbox_taxonomy_unbindalltermofnodefromavocabulary($vocabulary->getAttributeNode("VocabularyName")->value, $node, $vid);
  	        }
	      }
  		  wimprt_set_field_taxonomy_vocabulary($vocabulary, &$result, $vid);
  		  if (!isset($node->nid)){
  		    $node->field_free_tags = array();
  		    $node->field_concepts = array();
  		    $node->field_iptc = array();
  		  }
  		  if (isset($result['field_free_tags'])){
  		    foreach($result['field_free_tags'] as $term) {
  		      if (!in_array(array("value" => $term->tid), $node->field_free_tags)) {  
  		        $node->field_free_tags[] =  array("value" => $term->tid);
  			    $node->taxonomy[] = $term; 
  		      }
  		    }
  		  }
  		  if (isset($result['field_concepts'])){
	        foreach($result['field_concepts'] as $term) {
  		      if (!in_array(array("value" => $term->tid), $node->field_concepts)) {  
  		        $node->field_concepts[] =  array("value" => $term->tid);
  			    $node->taxonomy[] = $term; 
  		      }
  		    }
  		  }
  		  if (isset($result['field_iptc'])){
	        foreach($result['field_iptc'] as $term) {
  		      if (!in_array(array("value" => $term->tid), $node->field_iptc)) {  
  		        $node->field_iptc[] =  array("value" => $term->tid);
  			    $node->taxonomy[] = $term; 
  		      }
  		    }
  		  }
	    }
	  }
	} else {
	  drupal_set_message('WARNING: No vocabulary tag found in taxonomy element. Skipping taxonomy process.','notice');
	}
  } else {
	drupal_set_message('Free tag taxonomy vocabulary not set. See wally/settings.','error');
	return NULL;
  }
}

/**
 * Search or Create Terms & Vocabularies
 * 
 * Search and return array taxonomy terms for a given vocabularies. 
 * If DOM attribute Vocabulary Name is given, we search into 
 * Classified taxonomy. If no, asuming it's a free tag.
 *
 * If taxonomy term exist, element is return else, a new term
 * is added to taxonomy vocabulary.
 *  
 * @param $vocabulary
 *   Node currently processed.
 * @param &$result
 *   An array of Taxonomy Term
 *
 * @return
 *   Nothing but modify $result
 */
function wimprt_set_field_taxonomy_vocabulary($vocabulary, &$result, $vid) {
 // $freetagtaxonomy_vid = variable_get('wallymport_freetagtaxonomy',NULL);
 // $classifiedtagtaxonomy_vid = variable_get('wallymport_classifiedtagtaxonomy',NULL);
  $taxonomy_recursive = wallytoolbox_toboolean(variable_get('wallymport_taxonomy_recusive', TRUE));

  $vocabulary_name = $vocabulary->getAttributeNode("VocabularyName")->value;
  if (isset($vocabulary->getAttributeNode("VocabularyDescription")->value)) {
	$VocabularyDescription = $vocabulary->getAttributeNode("VocabularyDescription")->value;
  }

  if ($vid == NULL){
    $result_set = NULL;
    $vid = wallytoolbox_taxonomy_getvocabularyvidbyname($vocabulary_name);
    if ($vid == NULL){
      $vid = wallytoolbox_taxonomy_createvocabularybyname($vocabulary_name);
    }
  } else {
    $result_set = _wimprthelp_getfieldnamefromvocabularyname($vocabulary_name);
  }
  $terms = $vocabulary->getElementsByTagName("Term");
  if ($terms->length) {
	foreach($terms as $term) {
	  $path = trim(trim($term->getAttribute('Name')),"/");
      if ($t = wallytoolbox_taxonomy_get_term_by_path($path, $vid)) {
		// Cool, we got a term for the correct VID.
		if ($taxonomy_recursive) {
		  $recursive_path = "";
		  foreach( explode("/",trim($path,"/")) as $t){
			$recursive_path = $recursive_path."/".$t;
			$t = wallytoolbox_taxonomy_get_term_by_path($recursive_path, $vid);
			if ($result_set != NULL)
			  $result[$result_set][] = $t[0];
		  }
		} else {
		  if ($result_set != NULL)
		    $result[$result_set][] = $t[0];
		}
	  } else {
		$tid = NULL; 
		$recursive_path = "";
		foreach( explode("/",trim($path,"/")) as $t){
		  
          $recursive_path = $recursive_path."/".$t;
    	  $t_parent = wallytoolbox_taxonomy_get_term_by_path($recursive_path, $vid);
    	  if ($t_parent) {
    	    $tid = $t_parent[0]->tid;
    	  } else {
    		if ($tid) {
    		  $tid = wallytoolbox_add_taxonomy_term($t, $vid, $t, 0, $tid);
    		} else {
    		  $tid = wallytoolbox_add_taxonomy_term($t, $vid, $t, 0, null);
    		}
    	  }
    	   // The node will be assigned to all the terms from PATH.
           if ($taxonomy_recursive & $result_set != NULL) $result[$result_set][] = taxonomy_get_term($tid);
		  
		}
		// The node will only assigned to the last term from PATH.
		if (!$taxonomy_recursive & $result_set != NULL) $result[$result_set][] = taxonomy_get_term($tid);
	  }
	}
  } 
}

/**
 * Process field Entities from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_entities(&$node,$dom,&$log){

  if (variable_get('wallymport_debug',0) or TRUE){
	$log['log'][] = 'process Entities: '.$dom->nodeName;
  }
  $termes = $dom->getElementsByTagName("Term");
  $results = array();
  foreach ($termes as $terme){
    $results[] = wimprt_process_type($terme,$log,'entity');
  }
 
  // Get Location Taxonomy for all nodes.
  $node->field_entities = array();
  foreach($results as $result) {
    if (isset($result->field_entity)) {
	  if ($result->field_entity[0]) {
	    $node->field_entities[] = array("value" => $result->field_entity[0]["value"]);
	    $node->taxonomy[$result->field_entity[0]["value"]] = taxonomy_get_term($result->field_entity[0]["value"]);  
      }
    }
  }
}

/**
 * Process common attributes of types from DOM element.
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
function wimprt_typetype_preprocess(&$node,$dom,$preview){
  if ($preview){
    $node->rejected = TRUE;
  }
}

/**
 * Create or load a entity type depending of EntityID.
 * 
 * If no node match the EntityID, a new node is created.
 * 
 * @param $dom
 *   The DomDocument who is being proceed (A DomElement)
 * @param $log
 *   The log object build recursively in different operation. this could track error in the process and build nodes.
 *
 * @return
 *   A node object
 */
function wimprt_create_or_load_type_entity($dom,$log){	
	
  if (!($vid = variable_get('wallymport_entity', NULL))) {
    // TO DO FATAL :: No location taxonomy vocabulary
	drupal_set_message('Entity Taxonomy Vocabulary not Found. See admin.','error');
	$node->rejected = TRUE;
	return NULL;
  }

  // Taxonomy TID (from vocabulary Locations).
  $term = $dom->getAttributeNode("Name")->value;
  if ($taxonomy = wallytoolbox_taxonomy_get_term_by_path($term, $vid)) {
	// Taxonomy term exist - fetch the tid.
	$taxonomy = $taxonomy[0]; 
	$tid = $taxonomy->tid;
	$result = wallytoolbox_get_node_by_tid($tid,"wally_entitytype");
	if ($result == NULL){
	  $taxonomy = taxonomy_get_term($tid);
	  $result = new stdClass();
	}
	elseif (count($result) > 1){
	  drupal_set_message('Found 2 or more node reference for EntityID: '.$lid.'.','error');
	  return NULL;
	} else {
      $result = $result[0];
	}
  } else {
	// New taxonomy term. Create & get the tid.
	$tid = wallytoolbox_add_taxonomy_term(_wimprthelp_plain_text($term), $vid, _wimprthelp_plain_text($term), 0);
	$taxonomy = taxonomy_get_term($tid);
	$result = new stdClass();
  };
  // Set the taxonomy field.
  $result->field_entity = array( array("value" => $tid) );
  // Set drupal node->taxonomy. 
  $result->taxonomy = array($tid => $taxonomy); 

  return $result;
}

/**
 * Process Entity type from DOM.
 * 
 * MAP fields of the entity content type
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
function wimprt_process_type_entity(&$node,$dom,&$log,$preview){
 
  
  wimprt_typetype_preprocess(&$node, $dom, $preview);

  // Node type
  $node->type = "wally_entitytype";
  
  // External Reference
  //$node->field_externalreference = array(array("value" => $dom->getAttributeNode("EntityID")->value)); 		   
  
  // Node UID 
  $node->uid = variable_get('wallymport_defaultuser',1);

  // Node Title 
  $node->title = $dom->getAttributeNode("Name")->value;
  
  // Description
  $t = $dom->getElementsByTagName("EntityDescription");
  if ($t->length) {
	$node->field_objectdescription = array(array("value" => _wimprthelp_plain_text($t->item(0)->nodeValue)));
  }

  // Services
  $t = $dom->getElementsByTagName("EntityServices");
  if ($t->length) {
	$node->field_entityservices = array(array("value" => _wimprthelp_plain_text($t->item(0)->nodeValue)));
  }

// Locations : supprimées
/*
  $t = $dom->getElementsByTagName("Locations");
  if ($t->length) {
	$node->field_locations = array(); 
    if (variable_get('wallymport_debug',0) or TRUE){
	  $log['log'][] = 'process Locations: '.$t->nodeName;
	}
	$results = wimprt_process_list($t->item(0), $log, 'type');
	// Get Location Taxonomy for all nodes.
	foreach ($results as $result) {
	  if (isset($result->field_locationtaxonomy)) {
		if ($result->field_locationtaxonomy[0]) {
		  $node->field_locations[] = array("value"=>$result->field_locationtaxonomy[0]["value"]);
		  $node->taxonomy[] = taxonomy_get_term($result->field_locationtaxonomy[0]["value"]);  
		}
	  }
	} 
  }
*/
  // Send back the node
  return($node);

}

/**
 * Process field Persons from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_persons(&$node,$dom,&$log){
  
  $node->field_persons = array(); 

  if (variable_get('wallymport_debug',0) or TRUE){
	$log['log'][] = 'process Persons: '.$dom->nodeName;
  }
  $termes = $dom->getElementsByTagName("Term");
  $results = array();
  foreach ($termes as $terme){
    $results[] = wimprt_process_type($terme,$log,'person');
  }
  // Get Location Taxonomy for all nodes.
  $node->field_persons = array();
  foreach($results as $result) {
    if (isset($result->field_persontaxonomy)) {
	  if ($result->field_persontaxonomy[0]) {
	    $node->field_persons[] = array("value" => $result->field_persontaxonomy[0]["value"]);
		$node->taxonomy[$result->field_persontaxonomy[0]["value"]] = taxonomy_get_term($result->field_persontaxonomy[0]["value"]);  
      }
    }
  }
}
/**
 * Process field Authors from DOM element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
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
function wimprt_set_field_authors(&$node,$dom,&$log){
  
  if (variable_get('wallymport_debug',0) or TRUE){
	$log['log'][] = 'process Persons: '.$dom->nodeName;
  }
  if (!($vid = variable_get('wallymport_author', NULL))) {
    // TO DO FATAL :: No location taxonomy vocabulary
	drupal_set_message('Author Taxonomy Vocabulary not Found. See admin.','error');
	$node->rejected = TRUE;
  }

  // Taxonomy TID (from vocabulary Locations).
  
  $terms = $dom->getElementsByTagName("Term");
  $tids = array();
  foreach ($terms as $term){
    $term_name = $term->getAttributeNode("Name")->value;
    if ($taxonomy = wallytoolbox_taxonomy_get_term_by_path($term_name, $vid)) {
	// Taxonomy term exist - fetch the tid.
	  $taxonomy = $taxonomy[0]; 
	  $tids[] = $taxonomy->tid;
    } else {
	  $tids[] = wallytoolbox_add_taxonomy_term(_wimprthelp_plain_text($term_name), $vid, _wimprthelp_plain_text($term_name), 0);
    }
  }
  // Get Location Taxonomy for all nodes.
  $node->field_authors = array();
  foreach($tids as $tid) {
    $node->field_authors[] = array("value" => $tid);
	$node->taxonomy[$tid] = taxonomy_get_term($tid);  
  }
}
/**
 * Create or load a person type depending of PersonID.
 * 
 * If no node match the PersonID, a new node is created.
 * 
 * @param $dom
 *   The DomDocument who is being proceed (A DomElement)
 * @param $log
 *   The log object build recursively in different operation. this could track error in the process and build nodes.
 *
 * @return
 *   A node object
 */
function wimprt_create_or_load_type_person($dom,$log){	

  if (!($vid = variable_get('wallymport_person',NULL))) {
	// TO DO FATAL :: No location taxonomy vocabulary
	drupal_set_message('Person Taxonomy Vocabulary  not Found. See admin.','error');
	$result->rejected = TRUE;
	return NULL;
  }
  // Taxonomy TID (from vocabulary Locations).
  $term = $dom->getAttributeNode("Name")->value;
  if ($taxonomy = wallytoolbox_taxonomy_get_term_by_path($term, $vid)) {
    // Taxonomy term exist - fetch the tid.
	$taxonomy = $taxonomy[0]; 
	$tid = $taxonomy->tid;
	$result = wallytoolbox_get_node_by_tid($tid,"wally_persontype");
	if ($result == NULL){
	  $taxonomy = taxonomy_get_term($tid);
	  $result = new stdClass();
	}
	elseif (count($result) > 1){
	  drupal_set_message('Found 2 or more node reference for PersonID: '.$lid.'.','error');
	  return NULL;
	} else {
      $result = $result[0];
	}
  } else {
	// New taxonomy term. Create & get the tid.
	$tid = wallytoolbox_add_taxonomy_term(_wimprthelp_plain_text($term), $vid, _wimprthelp_plain_text($term), 0);
	$taxonomy = taxonomy_get_term($tid);
	$result = new stdClass();
  };
  // Set the taxonomy field.
  
  $result->field_persontaxonomy = array( array("value" => $tid) );
  // Set drupal node->taxonomy. 
  $result->taxonomy = array($tid => $taxonomy); 
  return $result;
}

/**
 * Process Person type from DOM.
 * 
 * MAP fields of the Person content type
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
function wimprt_process_type_person(&$node,$dom,&$log,$preview){
  wimprt_typetype_preprocess(&$node,$dom,$preview);

  // Node type
  $node->type = "wally_persontype";

// External Reference supprimé
//  $node->field_externalreference = array(array("value"=>$dom->getAttributeNode("PersonID")->value)); 

  // Node UID 
  $node->uid = variable_get('wallymport_defaultuser',1);

  // Node Title 
  $node->title = $dom->getAttributeNode("Name")->value;
  
  // Last Name
  $t = $dom->getElementsByTagName("PersonLastName");
  if ($t->length) {
	$node->field_personlastname = array(array("value" => _wimprthelp_plain_text($t->item(0)->nodeValue)));
  }

  // First Name
  $t = $dom->getElementsByTagName("PersonFirstName");
  if ($t->length) {
	$node->field_personfirstname = array(array("value" => _wimprthelp_plain_text($t->item(0)->nodeValue)));
  }

  // Nick Name
  $t = $dom->getElementsByTagName("PersonNickName");
  if ($t->length) {
	$node->field_personnickname = array(array("value" => _wimprthelp_plain_text($t->item(0)->nodeValue)));
  }

  // Birth Date
  $t = $dom->getElementsByTagName("PersonBirthDate");
  if ($t->length) {
	$node->field_personbirthdate = array(_wimprthelp_date_to_ccktime($t->item(0)->nodeValue));
  }

  // Sex
  $t = $dom->getElementsByTagName("PersonSex");
  if ($t->length) {
	$node->field_personsex = array(array("value" => _wimprthelp_plain_text($t->item(0)->nodeValue)));
  }

  // Email
  $t = $dom->getElementsByTagName("PersonEmail");
  if ($t->length) {
	$node->field_personemail = array(array("email" => _wimprthelp_plain_text($t->item(0)->nodeValue)));
  }

  // Phone number
  $t = $dom->getElementsByTagName("PersonPhoneNumber");
  if ($t->length) {
	$node->field_phonenumber = array(array("value" => _wimprthelp_plain_text($t->item(0)->nodeValue)));
  }

  // Website
  $t = $dom->getElementsByTagName("PersonWebSite");
  if ($t->length) {
	$site = array(); 
	$site["url"] = $t->item(0)->nodeValue; 
	$site["title"] = $t->item(0)->nodeValue;
	$site["attributes"] = Array();
	$node->field_personwebsite = array($site);
  }
  // Send back the node
  return($node);

}

/**
 * Convert a adress DOM element to a CCK_location form element.
 * 
 * MAP field from the a DOM (XML) element or attributes. 
 * 
 * @param $addresses
 *   The DomDocument who is being proceed (A Adresses DomElement)
 * @param $log
 *   The log object build recursively in different operation. this could track error in the process and build nodes.
 * @param $location_name
 *   The Name given to the cck_location element. If ommitted, city name will be used. 
 *
 * @return
 *   Array of cck_location element.
 */
function _wimprt_address_dom_to_ccklocation($addresses, &$log, $location_name = NULL) {

  $address_line = array(); 
  $array_node = array(); 

  foreach ($addresses as $address) {
	
    $adress_item = array();	
	$address_line_cpt = 0; 
	$address_lines = $address->getElementsByTagName("AddressLine");
	if ($address_lines->length) {
	  foreach($address_lines as $address_line) {
		switch ($address_line_cpt) {
		  case 0:
			$adress_item["street"] = $address_line->nodeValue;
			break; 
		  case 1:
			$adress_item["additional"] = $address_line->nodeValue;
			break;
		  default:
			drupal_set_message('More than 2 adress lines into xml. Ignoring: "'.$address_line->nodeValue.'".','error');
		}
		$address_line_cpt++;
	  }
	}
    if ($address->getElementsByTagName("City")->length) {
	  $adress_item["city"] = $address->getElementsByTagName("City")->item(0)->nodeValue;
	}	
	if ($location_name) {
	  $adress_item["name"] = $location_name;
	} else {
	  $adress_item["name"] = $adress_item["city"];
	}

    if ($address->getElementsByTagName("Zip")->length) {
	  $adress_item["postal_code"] = $address->getElementsByTagName("Zip")->item(0)->nodeValue;
	}

	if ($address->getElementsByTagName("State")->length) {
	  $adress_item["province_name"] = $address->getElementsByTagName("State")->item(0)->nodeValue;
	}

	if ($address->getElementsByTagName("Country")->length) {
	  $adress_item["country_name"] = $address->getElementsByTagName("Country")->item(0)->nodeValue;
	}
	$array_node[] = _wimprthelp_array_to_ccklocation($adress_item);
  }
  return $array_node;
}

/**
 * @} End of "defgroup wallymport.field".
 */
