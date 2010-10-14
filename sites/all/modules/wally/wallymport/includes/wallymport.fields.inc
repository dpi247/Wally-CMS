<?php 

function wimprt_process_package_articlepackagetype(&$node,$dom_element,&$log){
  
  //Map XML Attributes 
  $node->type='wally_articlepackage';
  $node->uid=1;
  $node->created_at= strtotime($dom_element->getAttributeNode("CreationDate")->value);
  $node->language= $dom_element->getAttributeNode("Language")->value;
  $node->created= $dom_element->getAttributeNode("PackageID")->value;
  $node->created_at= $dom_element->getAttributeNode("PublicationDate")->value;
  $node->created_at= $dom_element->getAttributeNode("LastUpdateDate")->value;
  $node->created_at= $dom_element->getAttributeNode("EmbargoDate")->value;
  
  //Map XML element
  //@todo: should be replaced by wimprt_process_package_default(&$node,$dom_element,&$log);
  wimprt_set_field_mainstory($node,$dom_element->getElementsByTagName("MainStory")->item(0),$log);
}

//***************************




function wimprt_set_field_mainstory(&$node,$dom,&$log){
  $object=wimprt_process_object($dom,$log,'TextObject');
  $node->MainStory['nid'][0]=$object->nid;
}

function wimprt_process_object_textobject (&$node,$dom_element,&$log){
  //Map XML Attributes 
  $node->type='wally_textobject';
  $node->uid=1;
  $node->created= $dom_element->getAttributeNode("DocumentType")->value;
  $node->object_id= $dom_element->getAttributeNode("ObjectID")->value;
  $node->object_description= $dom_element->getAttributeNode("ObjectDescription")->value;
  $node->publication_date= $dom_element->getAttributeNode("PublicationDate")->value;
  $node->last_upadte_date= $dom_element->getAttributeNode("LastUpdateDate")->value;
  //Map Element 
  // wimprt_process_object_default will call wimprt_process_object_locations,...
  wimprt_process_object_default($node,$dom_element,$log);
}

function wimprt_process_type_location(&$node,$dom,&$log){
  //Map XML Attributes 
  if($dom->hasAttribute("LocationID"))
  $node->created= $dom->getAttributeNode("LocationID")->value;
  
  
  //Map Element 
  wimprt_process_type_default($node,$dom,$log);

}

function wimprt_set_field_locations(&$node,$dom,&$log){
  
  $result=wimprt_process_list($node,$dom,'type',$log);
  $name= $dom->nodeName;
  $node->{$name}[0]['value']=$dom->nodeValue;
}

function wimprt_set_field_locationname(&$node,$dom,&$log){
  $node->location_name=$dom;
  
}


function wimprt_set_field_destinations(&$node,$dom,$log){

	$result = array(); 
	$voc=variable_get('wallymport_destinationpath',null);
	if ($voc) {
		foreach($dom->getElementsByTagName('Destination') as $destination) {
			$path = $destination->getElementsByTagName("DestinationPath")->item(0)->nodeValue;
			$terms = _taxonomy_get_term_by_path($path, $voc);
			if ($terms) {
				// As we check the taxonomy full path and specify a vocabulary
				// we can only have one result.
				$result[]["tid"]=$term[0]->tid;
				$result[]["path"]=$path;
				$result[]["rank"]=$destination->getAttributeNode("DestinationRank")->value;
				$result[]["rank"]=$destination->getAttributeNode("DocumentLayout")->value;
			} else {
				drupal_set_message('Path "'.$path.'" not found in vocabulary ('.$voc.'). Destination rejected.','error');	
			}
		}
		return serialize($result); 
	} else {
		drupal_set_message('Destination Path vocabulary not set. See settings.','error');
		return '';
	}
}



/**
 * Taxonomy get term by path
 * 
 * Search for taxonomy terms for a specific path 
 *
 * @param $path
 *   The taxonomy term path to match ( /term1/term2/term3 ). 
 * @param $voc
 *   The taxonomy vocabulary used for filtering on a single vocabulary.
 *
 * @return
 *   an array of term object
 */
function _taxonomy_get_term_by_path($path, $voc=null) {

	$t = explode("/", trim($path,"/"));
	$t = $t[count($t)-1];

	if ($voc) {
		$db_result = db_query("SELECT t.tid, t.* FROM {term_data} t WHERE LOWER(t.name) = LOWER('%s') AND t.vid = %s", trim($t), $voc);
	} else {
		$db_result = db_query("SELECT t.tid, t.* FROM {term_data} t WHERE LOWER(t.name) = LOWER('%s')", trim($t));
	}

	$result = array();

	// Reverse building of path for founded terms.
	while ($term = db_fetch_object($db_result)) {
		$ppath = "";
		$parents = taxonomy_get_parents_all($term->tid);
		foreach ($parents as $parent) $ppath = strtolower($parent->name)."/".$ppath;
		if (trim($ppath,"/")==strtolower(trim($path,"/"))) $result[] = $term;
	}
	
	return $result;
}

