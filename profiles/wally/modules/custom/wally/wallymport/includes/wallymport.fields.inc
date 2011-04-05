<?php 
/**
 * @defgroup wallymport.field Wally fields specifics Callback functions
 * @{
 * This file contain all fields specific functions of wally xml.   
 * These functions will be called by the wimprt_process_package(), 
 * wimprt_process_object() or wimprt_process_type() functions.
 */

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
function wimprt_create_or_load_package_articlepackagetype($dom,&$log){
  $node=new stdClass;
  $node->type='wally_articlepackage';

  $ext_ref=$dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue;
  if($node_temp=wallytoolbox_get_node_by_cckfield(array("field_externalreference"=>$ext_ref),"wally_articlepackage")){
    $node=$node_temp[0];
  }

  $uid=variable_get('wallymport_defaultuser',1);
  $node->uid=$uid;

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
  $node=new stdClass;
  $node->type='wally_gallerypackage';

  $ext_ref=$dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue;
  if($node_temp=wallytoolbox_get_node_by_cckfield(array("field_externalreference"=>$ext_ref),"wally_gallerypackage")){
    $node=$node_temp[0];
  }

  $uid=variable_get('wallymport_defaultuser',1);
  $node->uid=$uid;

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
  $node=new stdClass;
  $node->type='wally_pollpackage';

  $ext_ref=$dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue;
  if($node_temp=wallytoolbox_get_node_by_cckfield(array("field_externalreference"=>$ext_ref),"wally_pollpackage")){
    $node=$node_temp[0];
  }

  $uid=variable_get('wallymport_defaultuser',1);
  $node->uid=$uid;

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
  $node->field_packageid[0]['value']= $dom_element->getAttributeNode("PackageID")->value;
  if ($dom_element->hasAttribute("PublicationDate")) {
    $node->field_publicationdate[0] = _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("PublicationDate")->value);
  } elseif ($dom_element->hasAttribute("CreationDate")) {
    $node->field_publicationdate[0] = _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("CreationDate")->value);
  } else  {
    $node->field_publicationdate[0] = _wimprthelp_date_to_ccktime(date("Y-m-dTH:i:s"));
  }
  
  if($dom_element->hasAttribute("CreationDate"))
    $node->field_creationdate[0] = _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("CreationDate")->value);
  if($dom_element->hasAttribute("LastUpdateDate"))
    $node->changed = _wimprthelp_date_to_timestamp($dom_element->getAttributeNode("LastUpdateDate")->value);
  if($dom_element->hasAttribute("EmbargoDate"))
    $node->field_embargodatetime[0] = _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("EmbargoDate")->value);
  if($dom_element->hasAttribute("UnPublishDate"))
    $node->field_unpublishdate[0] = _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("UnPublishDate")->value);
  $node->language = $dom_element->getAttributeNode("Language")->value;
  
  if(!$dom_element->hasAttribute("PackageLayout")){
    wimprt_set_field_packagelayout($node,'Article',$log);
  }
  //Map XML element
  wimprt_process_package_default($node, $dom_element, $log, $preview);
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
   $node->field_packageid[0]['value']= $dom_element->getAttributeNode("PackageID")->value;
  if($dom_element->hasAttribute("PublicationDate"))
    $node->field_publicationdate[0] = _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("PublicationDate")->value);
  if($dom_element->hasAttribute("CreationDate"))
    $node->created = _wimprthelp_date_to_timestamp($dom_element->getAttributeNode("CreationDate")->value);
  if($dom_element->hasAttribute("LastUpdateDate"))
    $node->changed = _wimprthelp_date_to_timestamp($dom_element->getAttributeNode("LastUpdateDate")->value);
  if($dom_element->hasAttribute("EmbargoDate"))
    $node->field_embargodatetime[0]=_wimprthelp_date_to_ccktime($dom_element->getAttributeNode("EmbargoDate")->value);
  if($dom_element->hasAttribute("UnPublishDate"))
    $node->field_unpublishdate[0]= _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("UnPublishDate")->value);
  $node->language= $dom_element->getAttributeNode("Language")->value;
  
  //Map XML element
  wimprt_process_package_default($node, $dom_element, $log, $preview);
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
   $node->field_packageid[0]['value']= $dom_element->getAttributeNode("PackageID")->value;
  if($dom_element->hasAttribute("PublicationDate"))
    $node->field_publicationdate[0] = _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("PublicationDate")->value);
  if($dom_element->hasAttribute("CreationDate"))
    $node->created = _wimprthelp_date_to_timestamp($dom_element->getAttributeNode("CreationDate")->value);
  if($dom_element->hasAttribute("LastUpdateDate"))
    $node->changed = _wimprthelp_date_to_timestamp($dom_element->getAttributeNode("LastUpdateDate")->value);
  if($dom_element->hasAttribute("EmbargoDate"))
    $node->field_embargodatetime[0]=_wimprthelp_date_to_ccktime($dom_element->getAttributeNode("EmbargoDate")->value);
  if($dom_element->hasAttribute("UnPublishDate"))
    $node->field_unpublishdate[0]= _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("UnPublishDate")->value);
  $node->language= $dom_element->getAttributeNode("Language")->value;
  
  //Map XML element
  wimprt_process_package_default($node, $dom_element, $log, $preview);
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
  $value=$dom->nodeValue;
  $node->field_sourcexml[0]['value']= _wimprthelp_plain_text($value);
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
  $text=$dom->nodeValue;
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
  $value=$dom->nodeValue;
  $node->field_summary[0]['value']= _wimprthelp_plain_text($value);
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
  $value=$dom->nodeValue;
  $node->field_internalreference[0]['value']= _wimprthelp_plain_text($value);
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
  $value=$dom->nodeValue;
  $node->field_externalreference[0]['value']= _wimprthelp_plain_text($value);
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
  $value=$dom->nodeValue;
  $node->field_productid[0]['value']=$value;
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

  if(variable_get('wallymport_debug',0) or true){
    $log['log'][]='process Destinations: '.$dom->nodeName;
  }
  $term = array();
  $result = array(); 
  $vid=variable_get('wallymport_destinationpath',null);
  if ($vid) {
		$i=0;
		foreach($dom->getElementsByTagName('Destination') as $destination) {
			$path = $destination->getElementsByTagName("DestinationPath")->item(0)->nodeValue;
			$terms = wallytoolbox_taxonomy_get_term_by_path($path, $vid);
			if ($terms) {
				// As we check the taxonomy full path and specify a vocabulary
				// we can only have one result.
				$result[$i]['tid']=$terms[0]->tid;

				$result[$i]['target']=$destination->getAttributeNode("Position")->value;
				$result[$i]['layout']=$destination->getAttributeNode("DocumentLayout")->value;

				$result[$i]['rank']=$destination->getAttributeNode("DestinationRank")->value;
        $result[$i]['time_rank']=strtotime($node->field_publicationdate[0]['value']) + ($result[$i]['rank'] * 60 * 60 * variable_get('cckdestinations_rank_delta_t', 1));
			} else {
				drupal_set_message('Path "'.$path.'" not found in vocabulary ('.$vid.'). Destination rejected.','notice');	
			}
		$i++;
		}
	
		// If no destinations at all > Package can't be created.
		if (count($result)) {
			$node->field_destinations=$result;
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
    $node->field_channels=array(array("value"=>serialize($result)));
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
    $node->field_freeaccess[0]['value'] = null; 
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

	$node->field_authors = array(); 

	if(variable_get('wallymport_debug',0) or true){
		$log['log'][]='process Persons: '.$dom->nodeName;
	  }

	$results=wimprt_process_list($dom,$log,'type');

	// Get Location Taxonomy for all nodes.
	foreach($results as $result) {
		if (isset($result->field_persontaxonomy)) {
			if ($result->field_persontaxonomy[0]) {
				$node->field_authors[] = array("value"=>$result->field_persontaxonomy[0]["value"]);
				$node->taxonomy[] = taxonomy_get_term($result->field_persontaxonomy[0]["value"]);  
			}
		}
	} 
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

	if(variable_get('wallymport_debug',0) or true) {
		$log['log'][]='process PackageLayout: '.$dom->nodeName;
  }

	if (is_numeric($value)) {
    $node->field_packagelayout[] = $value;
  } elseif($tax_term = taxonomy_get_term_by_name($value)) {
    $node->field_packagelayout[] = $tax_term[0]->tid;
  } else {
		drupal_set_message('PackageLayout not found: '.$lid.', assuming new packagelayout.','error');
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
  $value=$dom->nodeValue;
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
    $node->field_externaluri[] = $value;
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
	$object=wimprt_process_object($dom, $log, 'TextObject', $preview);
	$node->field_mainstory[0]['nid']=$object->nid;
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
	$embedobjects=$dom->getElementsByTagName('EmbeddedObjects')->item(0);
	foreach(wimprt_process_list($embedobjects, $log, 'object', $preview) as $result){
    if (!array_key_exists($result->nid, $results)) {
      $results[$result->nid]=$result->nid;
      $node->field_embededobjects[]['nid']=$result->nid;
      if ($preview)
        $node->field_embededobjects_nodes[]=$result;
    }
  }

  // Linkslists objects
  $links = array(); 
  $node->field_linkedobjects = array();
  $node->field_linkedobjects_nodes = array();
	$linklists=$dom->getElementsByTagName('LinksLists')->item(0);
  $linklist=$dom->getElementsByTagName('LinksList')->item(0);
  if($dom->getElementsByTagName('LinksList')->item(0)){
    if($linklist->getElementsByTagName('Links')->item(0)->nodeValue!=NULL){
      foreach(wimprt_process_list($linklists, $log, 'object', $preview) as $result){
        if (!array_key_exists($result->nid, $links)) {
          $links[$result->nid]=$result->nid;
          $node->field_linkedobjects[]['nid']=$result->nid;
          if ($preview)
            $node->field_linkedobjects_nodes[]=$result;
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
function wimprt_objecttype_preprocess(&$node,$dom){
  $node->object_id= $dom->getAttributeNode("ObjectID")->value;
  if($dom->hasAttribute("ObjectDescription"))
    $node->object_description= $dom->getAttributeNode("ObjectDescription")->value;
  $node->field_publicationdate[0]= _wimprthelp_date_to_ccktime($dom->getAttributeNode("PublicationDate")->value);
  $node->created= _wimprthelp_date_to_timestamp($dom->getAttributeNode("CreationDate")->value);
  if($dom->hasAttribute("LastUpdateDate"))
    $node->changed= _wimprthelp_date_to_timestamp($dom->getAttributeNode("LastUpdateDate")->value);
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
  $node=new stdClass;
  $node->type='wally_textobject';
  
  $ext_ref=$dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue;
  if($node_temp=wallytoolbox_get_node_by_cckfield(array("field_externalreference"=>$ext_ref),'wally_textobject')){
    $node=$node_temp[0];
  }
  
  $uid=variable_get('wallymport_defaultuser',1);
  $node->uid=$uid;
  
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
function wimprt_process_object_textobject (&$node, $dom_element, &$log){
  //Map XML Attributes 
  //put common attributes
  wimprt_objecttype_preprocess($node,$dom_element);
  
  wimprt_process_object_default($node,$dom_element,$log);
  
  //Exception for TextObject
  if ($dom_element->getElementsByTagName("TextTitle")->length) {
    $node->title = $dom_element->getElementsByTagName("TextTitle")->item(0)->nodeValue;
  }
  
  if ($dom_element->getElementsByTagName("Title")->length) {
    $node->field_objecttitle = array(array("value"=>$dom_element->getElementsByTagName("Title")->item(0)->nodeValue));
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
  $i=0;
    
  foreach($dom->getElementsByTagName("PollQuestion") as $pollquestion){
    $node->field_poll[$i]['active']=1;
    $feature=array();
    $choice=1;
    if($pollquestion->hasAttribute("Choice")){
    if($pollquestion->getAttributeNode("Choice")->value==="radioButton")
      $choice=0;
    }
    $feature['CancelVote_Allow']=1;
    $feature['ResultsAllow']=0;
    $feature['PollType']=$choice;
    $node->field_poll[$i]['poll_features']=serialize($feature);
    foreach($pollquestion->getElementsByTagName("Question") as $question)
      $node->field_poll[$i]['question']=$pollquestion->nodeValue;
      $j=0;
    foreach($pollquestion->getElementsByTagName("Answer") as $answer){
      $answers[$j]['choice']=$answer->nodeValue;
      $answers[$j]['votes']=null;
      $j+=1;
      }
      if($j>0) {
        $node->field_poll[$i]['choice']=serialize($answers);
      }
    $i+=1;
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
  $ext_ref=$dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue;
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
function wimprt_process_object_pollobjecttype(&$node,$dom,&$log){
  
  static $proceed_file_path;
  //put common attribute
  wimprt_objecttype_preprocess($node,$dom);

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

  $ext_ref=$dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue;
  if($node_temp=wallytoolbox_get_node_by_cckfield(array("field_externalreference"=>$ext_ref),"wally_photoobject")){
    $node=$node_temp[0];
  }

  $uid=variable_get('wallymport_defaultuser',1);
  $node->uid=$uid;

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
function wimprt_process_object_photoobjecttype(&$node,$dom,&$log){
  static $procced_file_path;
  $value=$dom->nodeValue;
  
  //put common attributes
  wimprt_objecttype_preprocess($node,$dom);

  //if (!isset($node->field_summary[0]['value']) || empty($node->field_thumbnail[0]))
    $node->field_summary[0]['value'] = _wimprthelp_plain_text($value);
  
  $dom_thumbnails = $dom->getElementsByTagName('Thumbnail');
  $thumb_uri = $dom->getElementsByTagName('Thumbnail')->item(0) ? $dom->getElementsByTagName('Thumbnail')->item(0)->getAttribute('URI') : '';
  //if (!isset($node->field_thumbnail[0]) || count($node->field_thumbnail[0])<8)
  $node->field_thumbnail[0]=_wimprthelp_create_file_from_uri($thumb_uri);
    
    
  $file_uri=$dom->getElementsByTagName('FileURI')->item(0)->nodeValue;
  //if (!isset($node->field_photofile[0]) || count($node->field_photofile[0])<8)
  $rdpartyMatch =emfield_parse_embed(null,$file_uri, 'emimage');  
  if(empty($rdpartyMatch)){//No a 3rdparty
    $node->field_photofile[0]=_wimprthelp_create_file_from_uri($file_uri);
  }else{//Is a 3rd party
    $node->field_photo3rdparty[0]['embed']=$file_uri;
  }
    
  
  wimprt_process_object_default($node,$dom,$log);

  if ($dom->getElementsByTagName("Caption")->length) {
    $node->field_summary[0]["value"] = $dom->getElementsByTagName("Caption")->item(0)->nodeValue;
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

  $ext_ref=$dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue;
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
function wimprt_process_object_audioobjecttype(&$node,$dom,&$log){
  static $procced_file_path;
  $value=$dom->nodeValue;
  
  //put common attributes
  wimprt_objecttype_preprocess($node,$dom);

  
  $file_uri=$dom->getElementsByTagName('FileURI')->item(0)->nodeValue;

  $rdpartyMatch =emfield_parse_embed(null,$file_uri, 'emaudio');  
  if(empty($rdpartyMatch)){//No a 3rdparty
    $node->field_audiofile[0]=_wimprthelp_create_file_from_uri($file_uri);
  }else{//Is a 3rd party
    $node->field_audio3rdparty[0]['embed']=$file_uri;
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
  $node=new stdClass;
  $node->type='wally_videoobject';

  $ext_ref=$dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue;
  if($node_temp=wallytoolbox_get_node_by_cckfield(array("field_externalreference"=>$ext_ref),"wally_videoobject")){
    $node=$node_temp[0];
  }

  $uid=variable_get('wallymport_defaultuser',1);
  $node->uid=$uid;

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
function wimprt_process_object_videoobjecttype(&$node,$dom,&$log){
  static $procced_file_path;
  $value=$dom->nodeValue;
  
  //put common attributes
  wimprt_objecttype_preprocess($node,$dom);


  $file_uri=$dom->getElementsByTagName('FileURI')->item(0)->nodeValue; 
  $rdpartyMatch =emfield_parse_embed(null,$file_uri, 'emvideo');  
  if(empty($rdpartyMatch)){//No a 3rdparty
    $node->field_videofile[0]=_wimprthelp_create_file_from_uri($file_uri);
  }else{//Is a 3rd party
    $node->field_video3rdparty[0]['embed']=$file_uri;
  }
  
  $node->field_link[0]['url']= $dom->getElementsByTagName('LinkURI')->item(0)->nodeValue;
  $node->field_link[0]['title']= $dom->getElementsByTagName('LinkTitle')->item(0)->nodeValue;
  wimprt_process_object_default($node,$dom,$log);
  //$node->field_video3rdparty[0]['embed']=$dom->getElementsByTagName('LinkURI')->item(0)->nodeValue;
  //$node->field_video3rdparty[0]['title']=$dom->getElementsByTagName('LinkTitle')->item(0)->nodeValue;
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
  $node=new stdClass;
  $node->type='wally_digitalobject';

  $ext_ref=$dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue;
  if($node_temp=wallytoolbox_get_node_by_cckfield(array("field_externalreference"=>$ext_ref),"wally_digitalobject")){
    $node=$node_temp[0];
  }

  $uid=variable_get('wallymport_defaultuser',1);
  $node->uid=$uid;

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
function wimprt_process_object_digitalobjecttype(&$node,$dom,&$log){
  static $procced_file_path;
  $value=$dom->nodeValue;
  
  //put common attributes
  wimprt_objecttype_preprocess($node,$dom);


  $file_uri=$dom->getElementsByTagName('FileURI')->item(0)->nodeValue;
  $rdpartyMatch =emfield_parse_embed(null,$file_uri, 'emother');  
  if(empty($rdpartyMatch)){//No a 3rdparty
    $node->field_objectfile[0]=_wimprthelp_create_file_from_uri($file_uri);
  }else{//Is a 3rd party
    $node->field_object3rdparty[0]['embed']=$file_uri;
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
  $thumbnail=$dom->nodeValue;
   $node->field_thumbnail[0]=_wimprthelp_create_file_from_uri($thumbnail);
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
function wimprt_process_type_link(&$node,$dom,&$log){
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

  $ext_ref=$dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue;
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
  $ext_ref=$dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue;
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
  $value=$dom->nodeValue;
  $node->field_textarette[0]['value']=$value;
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
  $value=$dom->nodeValue;
  $node->field_textforetitle[0]['value']=$value;
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
  $value=$dom->nodeValue;
  $node->title=$value;
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
function wimprt_set_field_subtitle(&$node,$dom,&$log){
  $value=$dom->nodeValue;
  $node->field_textsubtitle[0]['value']=$value;
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
  $value=$dom->nodeValue;
  $node->field_textchapo[0]['value']=$value;
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
  $value=$dom->nodeValue;
  $node->field_textbody[0]['value']=$value;
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
  $value=$dom->nodeValue;
  $node->field_extractmedium[0]['value']=$value;
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
  $value=$dom->nodeValue;
  $node->field_extractshort[0]['value']=$value;
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
  $value=$dom->nodeValue;
  $node->field_copyright[0]['value']=$value;
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
	
	// Get the LocationID attribnute to load node.
	$lid = $dom->getAttributeNode("LocationID")->value;
	if ($location = wallytoolbox_get_node_by_cckfield(array("field_externalreference"=>$lid),"wally_locationtype")) {
		if (count($location)==1) {
			$result = $location[0];
		} else {
			// TO DO FATAL :: found 2 (or more) node reference with same externalref !
			drupal_set_message('Found 2 or more node reference for LocationID: '.$lid.'.','error');
			$result = null;
		}
	} else {
		//drupal_set_message('LocationID not Found for LocationID: '.$lid.', assuming new location.','error');
		$result = new StdClass();
	}

	$uid=variable_get('wallymport_defaultuser',1);
	$result->uid = $uid;
	
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
function wimprt_process_type_location(&$node,$dom,&$log){

	if (!($vid=variable_get('wallymport_location',null))) {
		// TO DO FATAL :: No location taxonomy vocabulary
		drupal_set_message('Location Taxonomy Vocabulary  not Found. See admin.','error');
		return null;
	}

// UID
	$node->uid=variable_get('wallymport_defaultuser',1);

// External Reference
	$node->field_externalreference = array(array("value"=>$dom->getAttributeNode("LocationID")->value)); 

// Note Type
	$node->type='wally_locationtype';

// Node Title 
	$t = $dom->getElementsByTagName("LocationName");
	if ($t->length) {
		$node->title = _wimprthelp_plain_text($t->item(0)->nodeValue);
	} else {
		$node->title = _wimprthelp_plain_text($dom->getAttributeNode("LocationID")->value);
	}

// Taxonomy TID (from vocaulary Locations).
	$term = $dom->getAttributeNode("LocationID")->value;
	if ($taxonomy = wallytoolbox_taxonomy_get_term_by_path($term, $vid)) {
		// Taxonomy term exist - fetch the tid.
		$taxonomy = $taxonomy[0]; 
		$tid = $taxonomy->tid;
	} else {
		// New taxonomy term. Create & get the tid.
		$tid = wallytoolbox_add_taxonomy_term(_wimprthelp_plain_text($term), $vid, _wimprthelp_plain_text($term), 0);
		$taxonomy = taxonomy_get_term($tid);
	};
	// Set the taxonomy field.
	$node->field_locationtaxonomy = array( array("value" => $tid) );
	// Set drupal node->taxonomy. 
	$node->taxonomy = array($tid => $taxonomy); 

// field_objectdescription 
	$node->field_objectdescription = array(array("value"=>_wimprthelp_plain_text($dom->getAttributeNode("LocationID")->value)));

// field_address
	$addresses = $dom->getElementsByTagName("Address");
	if ($addresses->length) {
		$node->field_address = _wimprt_address_dom_to_ccklocation($addresses,$log, $node->title);
	}

// location lat/long - In this case will re-write field_adress values.
/*
	$locationgeo = $dom->getElementsByTagName("LocationGeo");
	if ($locationgeo->length) {

		$locationgeo = explode(",", $locationgeo->item(0)->nodeValue);
		$lat = $locationgeo[0]; 
		$lon = $locationgeo[1]; 
		if (isset($locationgeo[2])) $zoom = $locationgeo[2]; 
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
*/

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
function wimprt_set_field_markers(&$node,$dom,&$log){

	$node->field_markers = array(); 

	if(variable_get('wallymport_debug',0) or true){
		$log['log'][]='process Markers: '.$dom->nodeName;
	  }

	$results=wimprt_process_list($dom,$log,'type');

	// Get Location Taxonomy for all nodes.
	foreach($results as $result) {
		if (isset($result->field_locationtaxonomy)) {
			if ($result->field_locationtaxonomy[0]) {
				$node->field_markers[] = array("value"=>$result->field_locationtaxonomy[0]["value"]);
				$node->taxonomy[] = taxonomy_get_term($result->field_locationtaxonomy[0]["value"]);  
			}
		}
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

	if(variable_get('wallymport_debug',0) or true){
		$log['log'][]='process Locations: '.$dom->nodeName;
	  }

	$results=wimprt_process_list($dom,$log,'type');

	// Get Location Taxonomy for all nodes.
	foreach($results as $result) {
		if (isset($result->field_locationtaxonomy)) {
			if ($result->field_locationtaxonomy[0]) {
				$node->field_locations[] = array("value"=>$result->field_locationtaxonomy[0]["value"]);
				$node->taxonomy[] = taxonomy_get_term($result->field_locationtaxonomy[0]["value"]);  
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

	$result = array("field_tags"=>array(), "field_free_tags"=>array());
	
	if(variable_get('wallymport_debug',0) or true){
		$log['log'][]='process Taxonomy: '.$dom->nodeName;
	  }

	$freetagtaxonomy_vid=variable_get('wallymport_freetagtaxonomy',null);
	$classifiedtagtaxonomy_vid=variable_get('wallymport_classifiedtagtaxonomy',null);
	
	if ($classifiedtagtaxonomy_vid && $freetagtaxonomy_vid) {
		$vocabularies = $dom->getElementsByTagName("Vocabulary");
		if ($vocabularies->length) {
			
			$node->field_tags = array(); 
			$node->field_free_tags = array(); 
			
			foreach($vocabularies as $vocabulary) {
				wimprt_set_field_taxonomy_vocabulary($vocabulary, &$result);

				foreach($result["field_tags"] as $term) {
					if (!in_array(array("value"=>$term->tid), $node->field_tags)) {
						$node->field_tags[] = array("value"=>$term->tid);
						$node->taxonomy[] = $term; 
					}
				}

				foreach($result["field_free_tags"] as $term) {
					if (!in_array(array("value"=>$term->tid), $node->field_free_tags)) {
						$node->field_free_tags[] = array("value"=>$term->tid);
						$node->taxonomy[] = $term; 
					}
				}
			}
		} else {
			drupal_set_message('WARNING: No vocabulary tag found in taxonomy element. Skipping taxonomy process.','notice');
		}
	} else {
		drupal_set_message('Free tagg / classified tags taxonomy vocabularies not set. See wally/settings.','error');
		return null;
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
function wimprt_set_field_taxonomy_vocabulary($vocabulary, &$result) {

	$freetagtaxonomy_vid = variable_get('wallymport_freetagtaxonomy',null);
	$classifiedtagtaxonomy_vid = variable_get('wallymport_classifiedtagtaxonomy',null);
	$taxonomy_recursive = wallytoolbox_toboolean(variable_get('wallymport_taxonomy_recusive', true));

	$vocabulary_name = $vocabulary->getAttributeNode("VocabularyName")->value;
	if (isset($vocabulary->getAttributeNode("VocabularyDescription")->value)) {
		$VocabularyDescription = $vocabulary->getAttributeNode("VocabularyDescription")->value;
	}

	if (isset($vocabulary->getAttributeNode("VocabularyType")->value)) {
		$VocabularyType = $vocabulary->getAttributeNode("VocabularyType")->value;
	} else {
		$VocabularyType = "free";
	}

	$vid = 0; 
	if ($VocabularyType == "free") {
		$vid = $freetagtaxonomy_vid;
		$result_set = "field_free_tags";
	} else {
		$vid = $classifiedtagtaxonomy_vid;
		$result_set = "field_tags";
	} 

	$terms = $vocabulary->getElementsByTagName("Term");
	if ($terms->length) {
		foreach($terms as $term) {
			$path = trim(trim($vocabulary_name),"/")."/".trim(trim($term->nodeValue),"/");
      if ($t = wallytoolbox_taxonomy_get_term_by_path($path, $vid)) {
				// Cool, we got a term for the correct VID.
				if ($taxonomy_recursive) {
					$recursive_path = "";
					foreach( explode("/",trim($path,"/")) as $t)
					{
						$recursive_path = $recursive_path."/".$t;
						$t = wallytoolbox_taxonomy_get_term_by_path($recursive_path, $vid);
						$result[$result_set][] = $t[0];
					}
				} else {
					$result[$result_set][] = $t[0];
				}
			} else {
				$tid = null; 
				$recursive_path = "";
				foreach( explode("/",trim($path,"/")) as $t)
				{
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
					if ($taxonomy_recursive) $result[$result_set][] = taxonomy_get_term($tid);
				}
				// The node will only assigned to the last term from PATH.
				if (!$taxonomy_recursive) $result[$result_set][] = taxonomy_get_term($tid);
			}
		}
	} else {
		drupal_set_message('No term found in vocabulary: '.$vocabulary_name.'.','error');
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

	$node->field_entities = array(); 

	if(variable_get('wallymport_debug',0) or true){
		$log['log'][]='process Entities: '.$dom->nodeName;
	  }

	$results=wimprt_process_list($dom,$log,'type');

	// Get Location Taxonomy for all nodes.
	foreach($results as $result) {
		if (isset($result->field_persontaxonomy)) {
			if ($result->field_persontaxonomy[0]) {
				$node->field_entities[] = array("value"=>$result->field_entity[0]["value"]);
				$node->taxonomy[] = taxonomy_get_term($result->field_entity[0]["value"]);  
			}
		}
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
	
	// Get the EntityID attribute to load node if exist.
	
	$lid = $dom->getAttributeNode("EntityID")->value;
	if ($entity = wallytoolbox_get_node_by_cckfield(array("field_externalreference"=>$lid),"wally_entitytype")) {
		if (count($entity)==1) {
			$result = $entity[0];
		} else {
			// TO DO FATAL :: found 2 (or more) node reference with same externalref !
			drupal_set_message('Found 2 or more node reference for EntityID: '.$lid.'.','error');
			$result = null;
		}
	} else {
		drupal_set_message('Entity not Found for EntityID: '.$lid.', assuming new entity.','error');
		$result = new StdClass();
	}
	
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
function wimprt_process_type_entity(&$node,$dom,&$log){

	if (!($vid=variable_get('wallymport_entity',null))) {
		// TO DO FATAL :: No location taxonomy vocabulary
		drupal_set_message('Entity Taxonomy Vocabulary  not Found. See admin.','error');
		return null;
	}

// Node type
	$node->type = "wally_entitytype";

// External Reference
	$node->field_externalreference = array(array("value"=>$dom->getAttributeNode("EntityID")->value)); 
			   
// Node UID 
	$uid=variable_get('wallymport_defaultuser',1);
	$node->uid = $uid;

// Node Title 
	$t = $dom->getElementsByTagName("EntityTitle");
	if ($t->length) {
		$node->title = _wimprthelp_plain_text($t->item(0)->nodeValue);
	} else {
		$node->title = _wimprthelp_plain_text($dom->getAttributeNode("EntityID")->value);
	}

// Taxonomy TID (from vocaulary Locations).
	$term = $dom->getAttributeNode("EntityID")->value;
	if ($taxonomy = wallytoolbox_taxonomy_get_term_by_path($term, $vid)) {
		// Taxonomy term exist - fetch the tid.
		$taxonomy = $taxonomy[0]; 
		$tid = $taxonomy->tid;
	} else {
		// New taxonomy term. Create & get the tid.
		$tid = wallytoolbox_add_taxonomy_term(_wimprthelp_plain_text($term), $vid, _wimprthelp_plain_text($term), 0);
		$taxonomy = taxonomy_get_term($tid);
	};
	// Set the taxonomy field.
	$node->field_entity = array( array("value" => $tid) );
	// Set drupal node->taxonomy. 
	$node->taxonomy = array($tid => $taxonomy); 

// Description
	$t = $dom->getElementsByTagName("EntityDescription");
	if ($t->length) {
		$node->field_objectdescription = array(array("value"=>_wimprthelp_plain_text($t->item(0)->nodeValue)));
	}

// Services
	$t = $dom->getElementsByTagName("EntityServices");
	if ($t->length) {
		$node->field_services = array(array("value"=>_wimprthelp_plain_text($t->item(0)->nodeValue)));
	}

// Locations
	$t = $dom->getElementsByTagName("Locations");
	if ($t->length) {
	
		$node->field_locations = array(); 

		if(variable_get('wallymport_debug',0) or true){
			$log['log'][]='process Locations: '.$t->nodeName;
		  }

		$results=wimprt_process_list($t->item(0),$log,'type');

		// Get Location Taxonomy for all nodes.
		foreach($results as $result) {
			if (isset($result->field_locationtaxonomy)) {
				if ($result->field_locationtaxonomy[0]) {
					$node->field_locations[] = array("value"=>$result->field_locationtaxonomy[0]["value"]);
					$node->taxonomy[] = taxonomy_get_term($result->field_locationtaxonomy[0]["value"]);  
				}
			}
		} 

	}

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

	if(variable_get('wallymport_debug',0) or true){
		$log['log'][]='process Persons: '.$dom->nodeName;
	  }

	$results=wimprt_process_list($dom,$log,'type');

	// Get Location Taxonomy for all nodes.
	foreach($results as $result) {
		if (isset($result->field_persontaxonomy)) {
			if ($result->field_persontaxonomy[0]) {
				$node->field_persons[] = array("value"=>$result->field_persontaxonomy[0]["value"]);
				$node->taxonomy[] = taxonomy_get_term($result->field_persontaxonomy[0]["value"]);  
			}
		}
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
	
	// Get the LocationID attribnute to load node.
	
	$lid = $dom->getAttributeNode("PersonID")->value;
	if ($person = wallytoolbox_get_node_by_cckfield(array("field_externalreference"=>$lid),"wally_persontype")) {
		if (count($person)==1) {
			$result = $person[0];
		} else {
			// TO DO FATAL :: found 2 (or more) node reference with same externalref !
			drupal_set_message('Found 2 or more node reference for PersonID: '.$lid.'.','error');
			$result = null;
		}
	} else {
		drupal_set_message('Person not Found for PersonID: '.$lid.', assuming new person.','error');
		$result = new StdClass();
	}
	
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
function wimprt_process_type_person(&$node,$dom,&$log){

	if (!($vid=variable_get('wallymport_person',null))) {
		// TO DO FATAL :: No location taxonomy vocabulary
		drupal_set_message('Person Taxonomy Vocabulary  not Found. See admin.','error');
		return null;
	}
	
// Node type
	$node->type = "wally_persontype";

// External Reference
	$node->field_externalreference = array(array("value"=>$dom->getAttributeNode("PersonID")->value)); 

// Node UID 
	$uid=variable_get('wallymport_defaultuser',1);
	$node->uid = $uid;

// Node Title 
	$t = $dom->getElementsByTagName("PersonName");
	if ($t->length) {
		$node->title = _wimprthelp_plain_text($t->item(0)->nodeValue);
	} else {
		$node->title = _wimprthelp_plain_text($dom->getAttributeNode("PersonID")->value);
	}

// Taxonomy TID (from vocaulary Locations).
	$term = $dom->getAttributeNode("PersonID")->value;
	if ($taxonomy = wallytoolbox_taxonomy_get_term_by_path($term, $vid)) {
		// Taxonomy term exist - fetch the tid.
		$taxonomy = $taxonomy[0]; 
		$tid = $taxonomy->tid;
	} else {
		// New taxonomy term. Create & get the tid.
		$tid = wallytoolbox_add_taxonomy_term(_wimprthelp_plain_text($term), $vid, _wimprthelp_plain_text($term), 0);
		$taxonomy = taxonomy_get_term($tid);
	};
	// Set the taxonomy field.
	$node->field_persontaxonomy = array( array("value" => $tid) );
	// Set drupal node->taxonomy. 
	$node->taxonomy = array($tid => $taxonomy); 

// Last Name
	$t = $dom->getElementsByTagName("PersonLastName");
	if ($t->length) {
		$node->field_personlastname = array(array("value"=>_wimprthelp_plain_text($t->item(0)->nodeValue)));
	}

// First Name
	$t = $dom->getElementsByTagName("PersonFirstName");
	if ($t->length) {
		$node->field_personfirstname = array(array("value"=>_wimprthelp_plain_text($t->item(0)->nodeValue)));
	}

// First Name
	$t = $dom->getElementsByTagName("PersonNickName");
	if ($t->length) {
		$node->field_personnickname = array(array("value"=>_wimprthelp_plain_text($t->item(0)->nodeValue)));
	}

// Birth Date
	$t = $dom->getElementsByTagName("PersonBirthDate");
	if ($t->length) {
		$node->field_personbirthdate = array(_wimprthelp_date_to_ccktime($t->item(0)->nodeValue));
	}

// Sex
	$t = $dom->getElementsByTagName("PersonSex");
	if ($t->length) {
		$node->field_personsex = array(array("value"=>_wimprthelp_plain_text($t->item(0)->nodeValue)));
	}

// Email
	$t = $dom->getElementsByTagName("PersonEmail");
	if ($t->length) {
		$node->field_personemail = array(array("email"=>_wimprthelp_plain_text($t->item(0)->nodeValue)));
	}

// Phone number
	$t = $dom->getElementsByTagName("PersonPhoneNumber");
	if ($t->length) {
		$node->field_phonenumber = array(array("value"=>_wimprthelp_plain_text($t->item(0)->nodeValue)));
	}

// Website
	$t = $dom->getElementsByTagName("PersonWebSite");
	if ($t->length) {
		$site = array(); 
		$site["url"]=$t->item(0)->nodeValue; 
		$site["title"]=$t->item(0)->nodeValue;
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
function _wimprt_address_dom_to_ccklocation($addresses, &$log, $location_name=null) {

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
