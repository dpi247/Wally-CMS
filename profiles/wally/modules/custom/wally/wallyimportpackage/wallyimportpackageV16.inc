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
  $node->field_embededobjects = array();
  $node->field_embededobjects_nodes = array();
  $arraytemporaire = array();
  if($embedobjects = $dom->getElementsByTagName('EmbeddedObjects')->item(0)) {
    foreach(wimprt_process_list($embedobjects, $log, $importer, 'object') as $result) {
      if ((true||!$result->rejected) && (!array_key_exists($result->nid, $results) || $importer->wallymport_preview)) {
        $results[$result->nid] = $result->nid;
        $order = $result->embeddedobjectsorder ? $result->embeddedobjectsorder : $i++;
        $arraytemporaire[$order]['nid'] = $result->nid;
        if ($importer->wallymport_preview){
          $node->field_embededobjects_nodes[] = $result;
        }
      }
    }
  }

  ksort($arraytemporaire);
  //Place object in the correct order (depend of the embeddedobjectorder)
  foreach ($arraytemporaire as $array_key => $array_value){
    array_push($node->field_embededobjects, $array_value);
  }

  // Linkslists objects
  $links = array();
  $node->field_linkedobjects = array();
  $node->field_linkedobjects_nodes = array();
  $arraytemporaire = array();
  $linkslists = $dom->getElementsByTagName('LinksLists')->item(0);
  $linkslist = $dom->getElementsByTagName('LinksList')->item(0);
  if ($linkslists && $linkslist){
    if ($linkslist->getElementsByTagName('Links')->item(0)->nodeValue != NULL){
      foreach (wimprt_process_list($linkslists, $log, $importer, 'object') as $result) {
        if ((true||!$result->rejected) && (!array_key_exists($result->nid, $links) || $importer->wallymport_preview)) {
          $links[$result->nid] = $result->nid;
          $order = $result->embeddedobjectsorder ? $result->embeddedobjectsorder : $i++;
          $arraytemporaire[$order]['nid'] = $result->nid;
          if ($importer->wallymport_preview){
            $node->field_linkedobjects_nodes[] = $result;
          }
        }
      }
    } else {
      drupal_set_message('WARNING: No LinksList found in LinksLists element. Skipping LinksLists process.','notice');
    }
  }
  
  ksort($arraytemporaire);
  //Place object in the correct order (depend of the embeddedobjectorder)
  foreach ($arraytemporaire as $array_key => $array_value){
    array_push($node->field_linkedobjects, $array_value);
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
  if ($dom->hasAttribute("EmbeddedObjectsOrder")){
    $node->embeddedobjectsorder = $dom->getAttributeNode("EmbeddedObjectsOrder")->value;
  }
  $node->field_objectid[0]['value'] = $dom->getAttributeNode("ObjectID")->value;
  if ($dom->hasAttribute("ObjectDescription")){
    $node->object_description = $dom->getAttributeNode("ObjectDescription")->value;
  }
  $node->field_publicationdate[0] = _wimprthelp_date_to_ccktime($dom->getAttributeNode("PublicationDate")->value);
  $node->created = _wimprthelp_date_to_timestamp($dom->getAttributeNode("CreationDate")->value);
  if ($dom->hasAttribute("LastUpdateDate")){
    $node->changed = _wimprthelp_date_to_timestamp($dom->getAttributeNode("LastUpdateDate")->value);
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
  
  if ($dom->hasAttribute("EmbeddedObjectsOrder")){
    $node->embeddedobjectsorder = $dom->getAttributeNode("EmbeddedObjectsOrder")->value;
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
