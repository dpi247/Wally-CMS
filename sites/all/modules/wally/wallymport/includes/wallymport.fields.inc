<?php 

function wimprt_process_package_articlepackagetype(&$node,$dom_element,&$log){
  
  //Map XML Attributes 
  $node->type='wally_articlepackage';
  $node->uid=1;
  
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
//@todo: process result
$linklists=$dom->getElementsByTagName('LinksLists')->item(0);
$result=wimprt_process_list($linklists,$log,'object');
//@todo: process result

}

function wimprt_process_object_photoobjecttype(&$node,$dom,&$log){
  $node->type='wally_photoobject';
  
  
  $value=$dom->nodeValue;
  $node->field_summary[0]['value']= _wimprthelp_plain_text($value);
  
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




function  wimprt_process_object_linkslist(&$node,$dom,&$log){
//dsm('linkList');
  
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
  
  $node->type='wally_textobject';
  $node->uid=1;
  
  //Map Element 
  // wimprt_process_object_default will call wimprt_process_object_locations,...
}

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
	
	// Node type
	$node->type = "wally_locationtype";

	// Node UID 
	$uid=variable_get('wallymport_defaultuser',1);
	$node->uid = $uid;

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




	// to be continue .......


	
  

}

function wimprt_set_field_locations(&$node,$dom,&$log){

	if(variable_get('wallymport_debug',0) or true){
		$log['log'][]='process Locations: '.$dom->nodeName;
	  }
  
  
	$result=wimprt_process_list($dom,$log,'type');

//  $name= $dom->nodeName;
//  $node->{$name}[0]['value']=$dom->nodeValue;

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


