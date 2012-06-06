<?php 

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
function wallyimportpackageV15_process_package_articlepackagetype(&$node, $dom_element, &$log, $importer){
  //Map XML Attributes 
  $node->field_packageid[0]['value'] = $dom_element->getAttributeNode("PackageID")->value;
  if ($dom_element->hasAttribute("PublicationDate")) {
    $node->field_publicationdate[0] = _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("PublicationDate")->value);
  } else {
    $node->field_publicationdate[0] = _wimprthelp_date_to_ccktime(date("Y-m-d\TH:i:s"));
  }
  if ($dom_element->hasAttribute("CreationDate")){
    $node->field_creationdate[0] = _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("CreationDate")->value);
  } else {
    $node->field_creationdate[0] = _wimprthelp_date_to_ccktime(date("Y-m-d\TH:i:s"));
  }
  if ($dom_element->hasAttribute("LastUpdateDate")){
    $node->changed = _wimprthelp_date_to_timestamp($dom_element->getAttributeNode("LastUpdateDate")->value);
  } else {
    $node->changed = _wimprthelp_date_to_ccktime(date("Y-m-d\TH:i:s"));
  }
  if ($dom_element->hasAttribute("EmbargoDate")) {
    $node->field_embargodatetime[0] = _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("EmbargoDate")->value);
    if (strtotime($node->field_embargodatetime[0]['value']) > time()){
      $node->status = 0;
    } else {
      $node->status = 1;
    }
  } else {
    $node->field_embargodatetime[0] = _wimprthelp_date_to_ccktime(date("Y-m-d\TH:i:s"));
  }
  if ($dom_element->hasAttribute("UnPublishDate")) {
    $node->field_unpublishdate[0] = _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("UnPublishDate")->value);
    if (strtotime($node->field_unpublishdate[0]['value']) < time()){
      $node->status = 0;
    }
  }
  if ($dom_element->hasAttribute("ForceLastUpdate")) {
    module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
    $force_edit_update = wallytoolbox_toboolean($dom_element->getAttributeNode("ForceLastUpdate")->value);
  } else {
    $force_edit_update = FALSE;
  }
  if ($dom_element->hasAttribute('Action')){
    $action = $dom_element->getAttribute('Action');
  } else {
    $action = 'CreateReplace';
  }
  switch($action){
    case 'CreateReplace':
    case 'Preview':
      $node->field_editorialupdatedate[0] = _wimprthelp_date_to_ccktime(date("Y-m-d\TH:i:s"));
      break;
    case 'CreateUpdate':
    case 'Update':
      if ($node->nid == NULL || $force_edit_update) {
        $node->field_editorialupdatedate[0] = _wimprthelp_date_to_ccktime(date("Y-m-d\TH:i:s"));
      }
      break;
  }
  $node->language = $dom_element->getAttributeNode("Language")->value;
  //jusque ici ok
  //Map XML element
  wimprt_process_node_default($node, $dom_element, $log, $importer);
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
function wallyimportpackageV15_process_package_galleryPackageType(&$node, $dom_element, &$log, $importer){
  //Map XML Attributes 
  $node->field_packageid[0]['value'] = $dom_element->getAttributeNode("PackageID")->value;
  if ($dom_element->hasAttribute("PublicationDate")) {
    $node->field_publicationdate[0] = _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("PublicationDate")->value);
  } else {
    $node->field_publicationdate[0] = _wimprthelp_date_to_ccktime(date("Y-m-d\TH:i:s"));
  }
  if ($dom_element->hasAttribute("CreationDate")){
    $node->field_creationdate[0] = _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("CreationDate")->value);
  } else {
    $node->field_creationdate[0] = _wimprthelp_date_to_ccktime(date("Y-m-d\TH:i:s"));
  }
  if ($dom_element->hasAttribute("LastUpdateDate")){
    $node->changed = _wimprthelp_date_to_timestamp($dom_element->getAttributeNode("LastUpdateDate")->value);
  } else {
    $node->changed = _wimprthelp_date_to_ccktime(date("Y-m-d\TH:i:s"));
  }
  if ($dom_element->hasAttribute("EmbargoDate")) {
    $node->field_embargodatetime[0] = _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("EmbargoDate")->value);
    if (strtotime($node->field_embargodatetime[0]['value']) > time()){
      $node->status = 0;
    } else {
      $node->status = 1;
    }
  } else {
    $node->field_embargodatetime[0] = _wimprthelp_date_to_ccktime(date("Y-m-d\TH:i:s"));
  }
  if ($dom_element->hasAttribute("UnPublishDate")) {
    $node->field_unpublishdate[0] = _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("UnPublishDate")->value);
    if (strtotime($node->field_unpublishdate[0]['value']) < time()){
      $node->status = 0;
    }
  }
  if ($dom_element->hasAttribute("ForceLastUpdate")) {
    module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
    $force_edit_update = wallytoolbox_toboolean($dom_element->getAttributeNode("ForceLastUpdate")->value);
  } else {
    $force_edit_update = FALSE;
  }
  if ($dom_element->hasAttribute('Action')){
    $action = $dom_element->getAttribute('Action');
  } else {
    $action = 'CreateReplace';
  }
  switch($action){
    case 'CreateReplace':
    case 'Preview':
      $node->field_editorialupdatedate[0] = _wimprthelp_date_to_ccktime(date("Y-m-d\TH:i:s"));
      break;
    case 'CreateUpdate':
    case 'Update':
      if ($node->nid == NULL || $force_edit_update) {
        $node->field_editorialupdatedate[0] = _wimprthelp_date_to_ccktime(date("Y-m-d\TH:i:s"));
      }
      break;
  }
  //Map XML element
  wimprt_process_node_default($node, $dom_element, $log, $importer);
  
  if (!isset($node->field_packagelayout[0]) && $tax_term = taxonomy_get_term_by_name('Article')) {
    $node->field_packagelayout[]['value'] = $tax_term[0]->tid;
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
function wallyimportpackageV15_process_package_pollpackagetype(&$node, $dom_element, &$log, $importer){
  //Map XML Attributes 
  $node->field_packageid[0]['value'] = $dom_element->getAttributeNode("PackageID")->value;
  if ($dom_element->hasAttribute("PublicationDate")) {
    $node->field_publicationdate[0] = _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("PublicationDate")->value);
  } else {
    $node->field_publicationdate[0] = _wimprthelp_date_to_ccktime(date("Y-m-d\TH:i:s"));
  }
  if ($dom_element->hasAttribute("CreationDate")){
    $node->field_creationdate[0] = _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("CreationDate")->value);
  } else {
    $node->field_creationdate[0] = _wimprthelp_date_to_ccktime(date("Y-m-d\TH:i:s"));
  }
  if ($dom_element->hasAttribute("LastUpdateDate")){
    $node->changed = _wimprthelp_date_to_timestamp($dom_element->getAttributeNode("LastUpdateDate")->value);
  } else {
    $node->changed = _wimprthelp_date_to_ccktime(date("Y-m-d\TH:i:s"));
  }
  if ($dom_element->hasAttribute("EmbargoDate")) {
    $node->field_embargodatetime[0] = _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("EmbargoDate")->value);
    if (strtotime($node->field_embargodatetime[0]['value']) > time()){
      $node->status = 0;
    } else {
      $node->status = 1;
    }
  } else {
    $node->field_embargodatetime[0] = _wimprthelp_date_to_ccktime(date("Y-m-d\TH:i:s"));
  }
  if ($dom_element->hasAttribute("UnPublishDate")) {
    $node->field_unpublishdate[0] = _wimprthelp_date_to_ccktime($dom_element->getAttributeNode("UnPublishDate")->value);
    if (strtotime($node->field_unpublishdate[0]['value']) < time()){
      $node->status = 0;
    }
  }
  if ($dom_element->hasAttribute("ForceLastUpdate")) {
    module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
    $force_edit_update = wallytoolbox_toboolean($dom_element->getAttributeNode("ForceLastUpdate")->value);
  } else {
    $force_edit_update = FALSE;
  }
  if ($dom_element->hasAttribute('Action')){
    $action = $dom_element->getAttribute('Action');
  } else {
    $action = 'CreateReplace';
  }
  switch($action){
    case 'CreateReplace':
    case 'Preview':
      $node->field_editorialupdatedate[0] = _wimprthelp_date_to_ccktime(date("Y-m-d\TH:i:s"));
      break;
    case 'CreateUpdate':
    case 'Update':
      if ($node->nid == NULL || $force_edit_update) {
        $node->field_editorialupdatedate[0] = _wimprthelp_date_to_ccktime(date("Y-m-d\TH:i:s"));
      }
      break;
  }

  //Map XML element
  wimprt_process_node_default($node, $dom_element, $log, $importer);

  if (!isset($node->field_packagelayout[0]) && $tax_term = taxonomy_get_term_by_name('Article')) {
    $node->field_packagelayout[]['value'] = $tax_term[0]->tid;
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
function wallyimportpackageV15_process_object_pollobjecttype(&$node, $dom, &$log, $importer){
  static $proceed_file_path;
  //put common attribute
  wallyimportpackage_objecttype_preprocess($node, $dom, $importer);

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
 * Process field Poll Title from DOM element.
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
function wallyimportpackageV15_set_field_polltitle(&$node, $dom, &$log, $importer){
  $value = $dom->nodeValue;
  $node->field_objecttitle = array(array("value" => $value));
}
