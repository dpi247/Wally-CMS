<?php 

/**
 * Process the packages element of a wally DomDocument object.
 * Initiate the node object Call an apropriate function to process the childs of the packages element (package)
 *
 * @param $dom
 *   The DomDocument who is being proceed
 * @param $log
 *   The log object build recursively in different operation. this could track error in the process and build nodes
 * @param $importer
 *   The importer object used to import
 * @return
 *   the log object
 */
function wallyimportpackageV15_process_packages($dom, &$log, $importer){
  $return = array();
  module_invoke_all('wimprt_process_packages', $dom, $log, 'before_process');
  foreach($dom->getElementsByTagName('Package') as $package_element){
    $return = wallyimportpackageV15_process_package($package_element, $log, $importer);
  }
  module_invoke_all('wimprt_process_packages', $dom, $log, 'after_process');
  $return['log'] = $log;
  return $return;
}

/**
 * Call an apropriate function to process the childs of the package element depending the attribute callback of the child element.
 *
 * The naming convention to build a node from a specific package callback is: wimprt_process_package_CALLCACK
 * if no specific function found nothing is done.
 *
 * @param $node$importer,
 *   The Node that's being build
 * @param $dom
 *   The DomDocument who is being proceed (A DomElement)
 * @param $log
 *   The log object build recursively in different operation. this could track error in the process and build nodes
 *
 * @return
 *   the node object that has been build
 */
function wallyimportpackageV15_process_package($dom, &$log, $importer, $callback = NULL) {
  module_load_include('inc', 'wallymport', 'includes/wallymport.process');
  $callback = wimprt_set_callback($dom, $callback);
  $node = wimprt_create_or_load($dom, $importer, 'package', $callback, $log);
  $return = array();
  //@todo: place in walymportlog module
  if (variable_get('wallymport_debug',0) or TRUE){
    $log['log'][] = 'process package: '.$dom->nodeName;
  }

  if (!isset($node->rejected) & !isset($node->wallymport_delete)){
    module_invoke_all('wimprt_process_package', $node, $dom, $log, 'before_process',$importer);

    // ?Posibility of extension with 'hook_wallymport_info' / CTools Plugin?
    if (function_exists("wallyimportpackageV15_process_package_".$callback)){
      call_user_func("wallyimportpackageV15_process_package_".$callback, &$node, $dom, &$log, $importer);
    }

    module_invoke_all('wimprt_process_package', $node, $dom, $log, 'after_process',$importer);
    
    drupal_alter('wimprt_package_after_process', $node);
  }

  if (!empty($node) && !isset($node->rejected) && wallytoolbox_check_is_node($node)) {
    if ($importer->wallymport_preview) {
      module_load_include('inc', 'wallyedit', 'includes/Prenode');

      $prenode = new Prenode();
      $prenode->ConvertNodeToPrenode($node);
      $old_cache_name = $prenode->form_state['cache name'];
      $fake_fs = array('cache name' => $old_cache_name);
      prenode_delete($fake_fs);
      $prenode->cache_name = 'wallymport_preview_'.$old_cache_name;
      $form_state_prenode = $prenode->setFormState();
      $form_state_prenode['DoNotPopulate'] = TRUE;
      $content = drupal_get_form('wallymport_preview_form', $prenode->cache_name, 'disabled');
      $return['content'] = $content;
    } elseif (isset($node->wallymport_delete)){
      drupal_set_message(t('The package @ext_ref has been deleted', array('@ext_ref' => $node->ext_ref)));
    } else {
      $node->wallymport_node = TRUE;
      node_save($node);
      module_invoke_all('wallymport_nodeprocess', $node, $dom, $log, $importer, 'process_package', 'postsave');
    }
  } elseif (!isset($node->wallymport_delete) & !isset($node->wallymport_update)) {
    wallycontenttypes_delete_orphans_objects($node);
  }

  $log['node'] = array('current' => $node,'next' => $log['node']);
  
  return $return;
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
  if ($dom_element->hasAttribute('Action')){
    $action = $dom_element->getAttribute('Action');
  } else {
    $action = 'CreateReplace';
  }
  switch($action){
    case 'Preview':
    case 'CreateUpdate':
    case 'Update':
      if ($dom_element->hasAttribute("ForceLastUpdate")) {
        module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
        $force_edit_update = wallytoolbox_toboolean($dom_element->getAttributeNode("ForceLastUpdate")->value);
      } else {
        $force_edit_update = FALSE;
      }
      if ($node->nid != NULL && $force_edit_update) {
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
  if ($dom_element->hasAttribute('Action')){
    $action = $dom_element->getAttribute('Action');
  } else {
    $action = 'CreateReplace';
  }
  switch($action){
    case 'CreateReplace':
      $node->field_editorialupdatedate[0] = _wimprthelp_date_to_ccktime(date("Y-m-d\TH:i:s"));
      break;
    case 'Preview':
    case 'CreateUpdate':
    case 'Update':
      if ($dom_element->hasAttribute("ForceLastUpdate")) {
        module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
        $force_edit_update = wallytoolbox_toboolean($dom_element->getAttributeNode("ForceLastUpdate")->value);
      } else {
        $force_edit_update = FALSE;
      }
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
  if ($dom_element->hasAttribute('Action')){
    $action = $dom_element->getAttribute('Action');
  } else {
    $action = 'CreateReplace';
  }
  switch($action){
    case 'CreateReplace':
      $node->field_editorialupdatedate[0] = _wimprthelp_date_to_ccktime(date("Y-m-d\TH:i:s"));
      break;
    case 'Preview':
    case 'CreateUpdate':
    case 'Update':
      if ($dom_element->hasAttribute("ForceLastUpdate")) {
        module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
        $force_edit_update = wallytoolbox_toboolean($dom_element->getAttributeNode("ForceLastUpdate")->value);
      } else {
        $force_edit_update = FALSE;
      }
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
