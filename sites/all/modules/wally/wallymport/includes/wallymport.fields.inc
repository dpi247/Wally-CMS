<?php 

function wimprt_process_package_articlepackagetype(&$node,$dom_element,&$log){
  
  //Map XML Attributes 
  $node->type='wally_articlepackage';
  $node->uid=1;
  $node->language= $dom_element->getAttributeNode("Language")->value;
  $node->created= $dom_element->getAttributeNode("PackageID")->value;
  $node->created_at= $dom_element->getAttributeNode("PublicationDate")->value;
  $node->created_at= $dom_element->getAttributeNode("EmbargoDate")->value;
  
  
  $node->created= _wimprthelp_date_to_timestamp($dom_element->getAttributeNode("CreationDate")->value);
  $node->changed= _wimprthelp_date_to_timestamp($dom_element->getAttributeNode("LastUpdateDate")->value);
 
  $node->field_embargodatetime=_wimprthelp_date_to_ccktime($dom_element->getAttributeNode("EmbargoDate")->value);
  //Map XML element
  wimprt_process_package_default($node,$dom_element,$log);
}

//***************************

function wimprt_set_field_summary(&$node,$dom,&$log){
  $text=$dom->nodeValue;
  $node->field_summary= _wimprthelp_html_text($text);
}

function wimprt_set_field_mainstory(&$node,$dom,&$log){
  $object=wimprt_process_object($dom,$log,'TextObject');
  $node->MainStory['nid'][0]=$object->nid;
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
	
	echo "<hr>SEARCH FOR > LID:".$lid;

	if ($location = wallytoolbox_get_node_by_cckfield(array("field_externalreference"=>$lid),"wally_locationtype")) {
	 //  echo "Bazinga";
	 //  var_dump($location);
	} else {
	//	echo "Mierda!";
	}
}

function wimprt_process_type_location(&$node,$dom,&$log){
  
//  echo "passe par: ".__FUNCTION__." <br>"; 
//  var_dump($dom);
//  echo "<hr>"; 
  
  if($dom->hasAttribute("LocationID"))
  $node->created = $dom->getAttributeNode("LocationID")->value;
  
  
  //Map Element 
  //wimprt_process_type_default($node,$dom,$log);

}

function wimprt_set_field_locations(&$node,$dom,&$log){

	if(variable_get('wallymport_debug',0) or true){
		$log['log'][]='process Locations: '.$dom_element->nodeName;
	  }
  
  $result=wimprt_process_list($node,$dom,'type',$log);

//  echo "passe par: ".__FUNCTION__." <br>"; 
//  var_dump($result);
//  echo "<hr>"; 
  

//  $name= $dom->nodeName;
//  $node->{$name}[0]['value']=$dom->nodeValue;

}


function wimprt_set_field_editions(&$node,$dom,$log) {

	if(variable_get('wallymport_debug',0) or true){
		$log['log'][]='process Editions: '.$dom_element->nodeName;
	  }

	$result = array();
	$valid = (bool) false;
	$terms = array(); 
	$vid=variable_get('wallymport_edition',null);
	if ($vid) {

		// Read all terms from vocabulary and set if to False (default);
		$terms = taxonomy_get_tree($vid);
		foreach ($terms as $term) {
			$result[$term->name][Publish]=false;
			$result[$term->name][EditionTid]=$term->tid;
			$result[$term->name][EditionID]=$term->name;
		}

		// Get edition from DOM & update array.
		foreach($dom->getElementsByTagName('Edition') as $edition) {
			$editionID = $edition->getAttributeNode("EditionID")->value;
			$value = wallytoolbox_toboolean($edition->nodeValue);
			if (strtolower($editionID)=="all") {
				foreach ($terms as $term) $result[$term->name][Publish]=$value;
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
		$log['log'][]='process Destinations: '.$dom_element->nodeName;
	  }

	$result = array(); 
	$vid=variable_get('wallymport_destinationpath',null);
	if ($vid) {
		$i=0;
		foreach($dom->getElementsByTagName('Destination') as $destination) {
			$path = $destination->getElementsByTagName("DestinationPath")->item(0)->nodeValue;
			$terms = wallytoolbox_taxonomy_get_term_by_path($path, $voc);
			if ($terms) {
				// As we check the taxonomy full path and specify a vocabulary
				// we can only have one result.
				$result[$i]["DestinationPathTid"]=$term[0]->tid;
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


