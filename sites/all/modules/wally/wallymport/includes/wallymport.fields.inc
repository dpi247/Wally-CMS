<?php 
function wimprt_create_or_load_package_articlepackagetype($dom,&$log){
  
  $node=new stdClass;
  $node->type='wally_articlepackage';
  $node->uid=1;

  $ext_ref=$dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue;
  if($node_temp=wallytoolbox_get_node_by_cckfield(array("field_externalreference"=>$ext_ref),"wally_articlepackage")){
    $node=$node_temp[0];
  }
  return $node;
}

function wimprt_process_package_articlepackagetype(&$node,$dom_element,&$log){
  
  //Map XML Attributes 
  
  if($dom_element->hasAttribute("UnPublicationDate"))
    $node->field_unpublishdate[0]= _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("UnPublicationDate")->value);
  
  $node->language= $dom_element->getAttributeNode("Language")->value;
  $node->created= _wimprthelp_date_to_timestamp($dom_element->getAttributeNode("CreationDate")->value);
  $node->changed= _wimprthelp_date_to_timestamp($dom_element->getAttributeNode("LastUpdateDate")->value);
  $node->field_packageid[0]['value']= $dom_element->getAttributeNode("PackageID")->value;
  $node->field_embargodatetime[0]=_wimprthelp_date_to_ccktime($dom_element->getAttributeNode("EmbargoDate")->value);
  //Map XML element
  wimprt_process_package_default($node,$dom_element,$log);
  
  
}

//***************************

function wimprt_set_field_packagedescription(&$node,$dom,&$log){
  $value=$dom->nodeValue;
  $node->field_summary[0]['value']= _wimprthelp_plain_text($value);
}
function wimprt_set_field_packagetitle(&$node,$dom,&$log){
  $text=$dom->nodeValue;
  $node->title= _wimprthelp_plain_text($text);
}
function wimprt_set_field_externalreference(&$node,$dom,&$log){
  $value=$dom->nodeValue;
  $node->field_externalreference[0]['value']= _wimprthelp_plain_text($value);
}

function wimprt_set_field_mainstory(&$node,$dom,&$log){
	$object=wimprt_process_object($dom,$log,'TextObject');
	$node->field_mainstory[0]['nid']=$object->nid;
}

function wimprt_set_field_EmbeddedContent(&$node,$dom,&$log){
	$embedobjects=$dom->getElementsByTagName('EmbeddedObjects')->item(0);
	$result=wimprt_process_list($embedobjects,$log,'object');
	$linklists=$dom->getElementsByTagName('LinksLists')->item(0);
	$result=wimprt_process_list($linklists,$log,'object');
}
  

function wimprt_create_or_load_object_textobject($dom,&$log){
  $node=new stdClass;
  $node->type='wally_textobject';
  $node->uid=1;
  
  $ext_ref=$dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue;
  if($node_temp=wallytoolbox_get_node_by_cckfield(array("field_externalreference"=>$ext_ref),'wally_textobject')){
    $node=$node_temp[0];
  }
  return $node;
}
  

function wimprt_create_or_load_object_photoobjecttype($dom,&$log){
  $node=new stdClass;
  $node->type='wally_photoobject';
  $node->uid=1;

  $ext_ref=$dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue;
  if($node_temp=wallytoolbox_get_node_by_cckfield(array("field_externalreference"=>$ext_ref),"wally_photoobject")){
    $node=$node_temp[0];
  }
  return $node;
}
function wimprt_process_object_photoobjecttype(&$node,$dom,&$log){
  static $procced_file_path;
  $value=$dom->nodeValue;
  $node->field_summary[0]['value']= _wimprthelp_plain_text($value);
  $file_uri=$dom->getElementsByTagName('FileURI')->item(0)->nodeValue;
  
  $node->field_thumbnail[0]=_wimprthelp_create_file_from_uri($file_uri);
  wimprt_process_object_default($node,$dom,$log);

}
function wimprt_set_field_links(&$node,$dom,&$log){
$result=wimprt_process_list($dom,$log,'type');
}


function wimprt_create_or_load_type_link($dom,$log){	
	
	// Get the Link attribnute to load node.
	$uri = $dom->getElementsByTagName("URI")->item(0)->nodeValue;
	$uri=parse_url($uri);
    
    $result = new StdClass();
	if($uri['scheme']=="externalRef"){
	    $ext_ref=$uri['host'];
		if ($llink = wallytoolbox_get_node_by_cckfield(array("field_externalreference"=>$ext_ref),"wally_linktype")) {
    		if (count($llink)==1) {
    			$result = $link[0];
        	}
		}
	}
	return $result; 
}

function wimprt_process_type_link(&$node,$dom,&$log){
  $node->type='wally_linktype';
  wimprt_process_type_default($node,$dom,$log);
  
 
  $uri=$dom->getElementsByTagName('URI')->item(0);
  $uri_value=$uri->nodeValue;
  $uri=parse_url($uri->nodeValue);
  if($uri['scheme']=="externalRef"){
	    $ext_ref=$uri['host'];
		if ($link = wallytoolbox_get_node_by_cckfield(array("field_externalreference"=>$ext_ref))) {
    		if (count($link)==1) {
    			 $node->field_internal_link[0]['nid'] = $link[0]->nid;
        	}
		}
	}
	else{
	 $node->field_link_item[0]['title']=$node->title;
	 $node->field_link_item[0]['url']=$uri_value;
     $node->field_link_item[0]['attributes']=array('target'=>'_blank');
	  
	}
  
  }


function wimprt_create_or_load_object_linkslist($dom,&$log){
  $node=new stdClass;
  $node->type='wally_linkslistobject';
  $node->uid=1;
  $ext_ref=$dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue;
  if($node_temp=wallytoolbox_get_node_by_cckfield(array("field_externalreference"=>$ext_ref),"wally_linkslistobject")){
    $node=$node_temp[0];
  }
  return $node;
}
function wimprt_process_object_linkslist(&$node,$dom,&$log){
  wimprt_process_object_default($node,$dom,$log);
  
}

function wimprt_set_field_title(&$node,$dom,&$log){
  $value=$dom->nodeValue;
  $node->title= _wimprthelp_plain_text($value);
}

function wimprt_set_field_summary(&$node,$dom,&$log){
  $value=$dom->nodeValue;
  $node->field_summary[0]['value']= _wimprthelp_plain_text($value);
}



function wimprt_set_field_productid(&$node,$dom,&$log){
  $value=$dom->nodeValue;
  $node->field_productid[0]['value']=$value;
}

function wimprt_process_object_textobject (&$node,$dom_element,&$log){
  //Map XML Attributes 
  $node->created= $dom_element->getAttributeNode("DocumentType")->value;
  $node->object_id= $dom_element->getAttributeNode("ObjectID")->value;
  $node->object_description= $dom_element->getAttributeNode("ObjectDescription")->value;
  $node->publication_date= $dom_element->getAttributeNode("PublicationDate")->value;
  
  wimprt_process_object_default($node,$dom_element,$log);
  
  //Map Element 
  // wimprt_process_object_default will call wimprt_process_object_locations,...
}

function wimprt_create_or_load_type_location($dom,$log){	
	// Node UID 
	$uid=variable_get('wallymport_defaultuser',1);
	$result->uid = $uid;
	
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
		drupal_set_message('LocationID not Found for LocationID: '.$lid.', assuming new location.','error');
		$result = new StdClass();
	}
	return $result; 
}

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
	$node->title = _wimprthelp_plain_text($dom->getAttributeNode("LocationID")->value);

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
		$node->field_address = wimprt_address_dom_to_ccklocation($addresses,$log, _wimprthelp_plain_text($dom->getAttributeNode("LocationID")->value));
	}

// location lat/long - In this case will re-write field_adress values.
	$locationgeo = $dom->getElementsByTagName("LocationGeo");
	if ($locationgeo->length) {

		$locationgeo = explode(",", $locationgeo->item(0)->nodeValue);
		$lat = $locationgeo[0]; 
		$lon = $locationgeo[1]; 
		if (isset($locationgeo[2])) $zoom = $locationgeo[2]; 
		$address_cpt = 0;
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

// CCK location specific field
	if (isset($node->field_address)) {
		$node->location =(array) $node->field_address[0];
		$node->locations = $node->field_address;
	}

// Send back the node
	return($node);
}

function wimprt_address_dom_to_ccklocation($addresses, &$log, $location_name=null) {

	$address_line = array(); 
	$adress_item = array();
	$array_node = array(); 
	
	foreach ($addresses as $address) {
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
			$adress_item["name"] = $address->getElementsByTagName("City")->item(0)->nodeValue;
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
}

function wimprt_set_field_editions(&$node,$dom,$log) {

	if(variable_get('wallymport_debug',0) or true){
		$log['log'][]='process Editions: '.$dom->nodeName;
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
				drupal_set_message('Editions ALL found. All edition set to '.$value.'. Editions iteration terminated.','error');
				break;
			} else {
				if (array_key_exists($editionID, $result)) {
					$valid = ($valid || $value);
					$result[$editionID][Publish]=$value;
				} else {
					drupal_set_message('Edition: "'.$editionID.'" not found in vocabulary. Edition rejected','error');
				}
			}
		}

		if ($valid) { 
				$node->field_editions=array(0=>serialize($result));
		} else {
			drupal_set_message('ALL Edition set to false. Package should be rejected','error');
			// TODO: FATAL ERROR - PACKAGE
		}
		
	} else {
		drupal_set_message('Edition vocabulary not set. See settings.','error');
		return '';
	}
}

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
				$result[$i]["DestinationPathTid"]=$terms[0]->tid;
				$result[$i]["DestinationPath"]=$path;
				$result[$i]["DestinationRank"]=$destination->getAttributeNode("DestinationRank")->value;
				$result[$i]["DocumentLayout"]=$destination->getAttributeNode("DocumentLayout")->value;
			} else {
				drupal_set_message('Path "'.$path.'" not found in vocabulary ('.$vid.'). Destination rejected.','error');	
			}
		$i++;
		}

		// If no destinations at all > Package can't be created.
		if (count($result)) {
			$node->field_destinations=array(0=>serialize($result)); 
		} else {
			// TODO: FATAL ERROR - PACKAGE
			drupal_set_message('No destination AT ALL for current package.','error');
			return '';
		}
	} else {
		drupal_set_message('Destination Path vocabulary not set. See settings.','error');
		return '';
	}
}

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
					if (!in_array($term->tid, $node->field_tags)) {
						$node->field_tags[] = $term->tid;
						$node->taxonomy[] = $term; 
					}
				}

				foreach($result["field_free_tags"] as $term) {
					if (!in_array($term->tid, $node->field_free_tags)) {
						$node->field_free_tags[] = $term->tid;
						$node->taxonomy[] = $term; 
					}
				}
			}
		} else {
			drupal_set_message('No taxonomy terms of free tagging found.','error');
		}
	} else {
		drupal_set_message('Free tagg / classified tags taxonomy vocabularies not set. See settings.','error');
		return null;
	}
}

function wimprt_set_field_taxonomy_vocabulary($vocabulary, &$result) {

	$freetagtaxonomy_vid = variable_get('wallymport_freetagtaxonomy',null);
	$classifiedtagtaxonomy_vid = variable_get('wallymport_classifiedtagtaxonomy',null);
//	$freetagtaxonomy_keyword = variable_get('wallymport_freetagtaxonomyword', 'FreeTag');
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
			$path = trim($vocabulary_name,"/")."/".trim($term->nodeValue,"/"); 
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
					// The node is  assigned to all the terms from PATH.
					if ($taxonomy_recursive) $result[$result_set][] = taxonomy_get_term($tid);
				}
				// The node is only assigned to the last term from PATH.
				if (!$taxonomy_recursive) $result[$result_set][] = taxonomy_get_term($tid);
			}
		}
	} else {
		drupal_set_message('No term found in vocabulary: '.$vocabulary_name.'.','error');
	}
}


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
