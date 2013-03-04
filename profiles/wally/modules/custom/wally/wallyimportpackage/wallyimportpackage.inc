<?php

module_load_include('inc', 'wallymport', 'includes/wallymport.fieldshelpers');

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
function wallyimportpackage_process_packages($dom, &$log, $importer){
  $return = array();
  module_invoke_all('wimprt_process_packages', $dom, $log, 'before_process');
  foreach($dom->getElementsByTagName('Package') as $package_element){
    $return = wallyimportpackage_process_package($package_element, $log, $importer);
  }
  module_invoke_all('wimprt_process_packages', $dom, $log, 'after_process');
  $return['log'] = $log;
  return $return;
}

/**
 * hook_wimprt_process_package 
 */
function wallyimportpackage_wimprt_process_package($node, $dom, $log, $type, $importer){
  if ($type == 'before_process'){
    $uri = explode('/', $_SERVER['REQUEST_URI']);
    if (array_pop($uri) == 'preview'){
      $importer->wallymport_preview = TRUE;
    }
  }
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
function wallyimportpackage_process_package($dom, &$log, $importer, $callback = NULL) {
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
    if (function_exists("wallyimportpackage_process_package_".$callback)){
      call_user_func("wallyimportpackage_process_package_".$callback, &$node, $dom, &$log, $importer);
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

      $content = drupal_get_form('wallymport_preview_form', $prenode->cache_name,'disabled');
      $return['content'] = $content;
    } elseif (isset($node->wallymport_delete)){
      drupal_set_message('The package '.$node->ext_ref.' has been deleted');
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
function wallyimportpackage_create_or_load_package_articlepackagetype($dom, &$log, $importer) {
  $node = new stdClass;
  $node->type = 'wally_articlepackage';
  $node->wallymport_node = TRUE;
  wallyimportpackage_create_or_load_preprocesspackage($dom, $node, $importer);
  return $node;
}

/**
 * Delete the node without checking permissions.
 *
 * @param $nid
 *   The nid of the node
 */
function wallyimportpackage_node_delete($nid){
  $node = node_load($nid, NULL, TRUE);
  db_query('DELETE FROM {node} WHERE nid = %d', $node->nid);
  db_query('DELETE FROM {node_revisions} WHERE nid = %d', $node->nid);
  db_query('DELETE FROM {node_access} WHERE nid = %d', $node->nid);

  // Call the node-specific callback (if any):
  node_invoke($node, 'delete');
  node_invoke_nodeapi($node, 'delete');

  // Clear the page and block caches.
  cache_clear_all();

  // Remove this node from the search index if needed.
  if (function_exists('search_wipe')) {
    search_wipe($node->nid, 'node');
  }
  watchdog('content', '@type: deleted %title.', array('@type' => $node->type, '%title' => $node->title));
  drupal_set_message(t('@type %title has been deleted.', array('@type' => node_get_types('name', $node), '%title' => $node->title)));
}

/**
 * Preprocess Create or load package
 * 
 * The node is created/deleted/updated/replaced depending on the action
 * 
 * @param $dom
 *   The DomDocument who is being proceed (A DomElement)
 * @param $node
 *   The node
 * @return
 *   A node object
 */
function wallyimportpackage_create_or_load_preprocesspackage($dom, &$node, $importer){
  
  $node_temporary = NULL;
  $ext_ref = _wimprthelp_plain_text($dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue);
  if ($node_temp = wallytoolbox_get_node_by_cckfield(array("field_externalreference" => $ext_ref), $node->type)){
    $node_temporary = $node_temp[0];
  }
  
  if ($dom->hasAttribute('Action')){
    $action = $dom->getAttribute('Action');
  } else {
    $action = 'CreateReplace';
  }
  switch($action){
    case 'Delete':
      if ($node_temporary != NULL){
        wallyimportpackage_node_delete($node_temporary->nid);
        $node->ext_ref = $ext_ref;
      } else {
        drupal_set_message('The package doesn\'t exist. It can be deleted. Package rejected.','error');
        $node->rejected = TRUE;
      }
      $node->wallymport_delete = TRUE;
      break;
    case 'CreateReplace':
      if ($node_temporary != NULL){
        $node->nid = $node_temporary->nid;
        $node->vid = $node_temporary->vid;
      }
      break;
    case 'CreateUpdate':
      if ($node_temporary != NULL){
        $node = $node_temporary;
        cckdestinations_force_dnc($node);
      }
      break;
    case 'Update':
      if ($node_temporary != NULL){
        $node = $node_temporary;    
      } else {
        $node->rejected = TRUE;
        $node->wallymport_delete = TRUE;
        drupal_set_message('The package doesn\'t exist. It can ben updated. Package rejected.','error');
      }
      break;
    case 'Preview':
      $importer->wallymport_preview = TRUE;
      break;
  }
  
  $uid = $importer->default_user;
  $node->uid = $uid;
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
function wallyimportpackage_create_or_load_package_gallerypackagetype($dom, &$log, $importer){
  $node = new stdClass;
  $node->type = 'wally_gallerypackage';
  $node->wallymport_node = TRUE;
  wallyimportpackage_create_or_load_preprocesspackage($dom, $node, $importer);
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
function wallyimportpackage_create_or_load_package_pollpackagetype($dom, &$log, $importer){
  $node = new stdClass;
  $node->type = 'wally_pollpackage';
  $node->wallymport_node = TRUE;
  wallyimportpackage_create_or_load_preprocesspackage($dom, $node, $importer);
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
function wallyimportpackage_process_package_articlepackagetype(&$node, $dom_element, &$log, $importer){
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
function wallyimportpackage_process_package_galleryPackageType(&$node, $dom_element, &$log, $importer){
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
function wallyimportpackage_process_package_pollpackagetype(&$node, $dom_element, &$log, $importer){
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
  
  //Map XML element
  wimprt_process_node_default($node, $dom_element, $log, $importer);

  if (!isset($node->field_packagelayout[0]) && $tax_term = taxonomy_get_term_by_name('Article')) {
    $node->field_packagelayout[]['value'] = $tax_term[0]->tid;
  }
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
function wallyimportpackage_set_field_sourcexml(&$node, $dom, &$log, $importer){
  $dom_sourcexml = new DOMDocument();
  $node_sourcexml = $dom_sourcexml->importNode($dom, TRUE);
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
function wallyimportpackage_set_field_packagetitle(&$node, $dom, &$log, $importer){
  $text = $dom->nodeValue;
  $node->title = _wimprthelp_plain_text($text);
  $node->field_displayed_title = array(array('value' => $text));
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
function wallyimportpackage_set_field_packagedescription(&$node, $dom, &$log, $importer){
  $value = $dom->nodeValue;
  $node->field_objectdescription[0]['value'] = $value;
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
function wallyimportpackage_set_field_internalreference(&$node, $dom, &$log, $importer){
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
function wallyimportpackage_set_field_externalreference(&$node, $dom, &$log, $importer){
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
function wallyimportpackage_set_field_embeddedobjectsorder(&$node, $dom, &$log, $importer){
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
function wallyimportpackage_set_field_productid(&$node, $dom, &$log, $importer){
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
function wallyimportpackage_set_field_destinations(&$node, $dom, $log, $importer) {
  if (variable_get('wallymport_debug',0) or TRUE){
    $log['log'][] = 'process Destinations: '.$dom->nodeName;
  }
  $term = array();
  $result = array();
  $vid = variable_get('wallymport_destinationpath',NULL);
  if ($vid) {
    $hook_is_empty = 'cckdestinations_content_is_empty';
    $temp_results = $temp_result = array();
    $i = 0;

    $multiple_destinations = cckdestinations_get_multiple_dests();
    $hierarchies = wallyctools_get_hierarchies();

    foreach($dom->getElementsByTagName('Destination') as $destination) {
      $path = $destination->getElementsByTagName("DestinationPath")->item(0)->nodeValue;
      $terms = wallytoolbox_taxonomy_get_term_by_path($path, $vid);
      if ($terms || array_key_exists($path, $multiple_destinations)) {
        if ($destination->hasAttribute("Position")) {
          $temp_result['target'] = $destination->getAttributeNode("Position")->value;
          $temp_result['layout'] = $destination->getAttributeNode("DocumentLayout")->value;
          $temp_result['rank'] = $destination->hasAttribute("DestinationRank") ? $destination->getAttributeNode("DestinationRank")->value : 'DNC';
        } else {
          $temp_result['target'] = 'none';
          $temp_result['layout'] = 'none';
          $temp_result['rank'] = 0;
        }

        if ($terms) {
          // As we check the taxonomy full path and specify a vocabulary
          // we can only have one result.
          $temp_result['tid'] = $terms[0]->tid;
          $temp_results[$i] = $temp_result;
          $i++;
        } else {
          foreach ($multiple_destinations[$path]['reals'] as $term) {
            if (isset($hierarchies[$term['tid']]['redacblocks'][$temp_result['target']])) {
              $real_dest = $temp_result;
              $real_dest['tid'] = $term['tid'];
              $temp_results[$i] = $real_dest;
              $i++;
            }
          }
        }
      } else {
        drupal_set_message(t('Path "@path" not found in vocabulary (@vid). Destination rejected.', array('@path' => $path, '@vid' => $vid)), 'notice');
      }
    }

    $check_double = array();
    $i = 0;
    foreach ($temp_results as $temp_result) {
      if (!in_array($temp_result['tid'].':'.$temp_result['target'], $check_double)) {
        $check_double[] = $temp_result['tid'].':'.$temp_result['target'];

        if ($actual_dest = _wallyimportpackage_find_destination($node->field_destinations, $temp_result)) {
          $temp_result['rank'] = (($actual_dest['rank'] == $temp_result['rank']) || ($temp_result['rank'] == 'DNC')) ? 'DNC'.$actual_dest['rank'] : $temp_result['rank'];
          $temp_result['time_rank'] = $actual_dest['time_rank'];
        }

        if ($temp_result['rank'] == 'DNC') {
          $temp_result['rank'] = 0;
        }

        $result[$i] = $temp_result;
        $i++;
      }
    }

    // If no destinations at all > Package can't be created.
    if (count($result)) {
      $node->field_destinations = $result;
    } else {
      // @TODO: FATAL ERROR - PACKAGE DONE - NEED CHECK
      $node->rejected = '';
      $log['error'] = TRUE;
      $log['error_msg'] = t('No destination AT ALL for current package. Package rejected.');
      drupal_set_message($log['error_msg'],'error');
      return '';
    }
  } else {
    drupal_set_message(t('Destination Path vocabulary not set. See settings.'),'error');
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
function wallyimportpackage_set_field_editions(&$node, $dom, $log, $importer) {
  if (variable_get('wallymport_debug',0) or TRUE){
    $log['log'][] = 'process Editions: '.$dom->nodeName;
  }

  $result = array();
  $valid = (bool) FALSE;
  $terms = array();
  $vid = variable_get('wallymport_edition', NULL);
  if ($vid) {
    // Read all terms from vocabulary and set if to False (default);
    $terms = taxonomy_get_tree($vid);
    foreach ($terms as $term) {
      $result[$term->name]["Publish"] = FALSE;
      $result[$term->name]["EditionTid"] = $term->tid;
      $result[$term->name]["EditionID"] = $term->name;
    }

    // Get edition from DOM & update array.
    foreach($dom->getElementsByTagName('Edition') as $edition) {
      $editionID = $edition->getAttributeNode("EditionID")->value;
      $value = wallytoolbox_toboolean($edition->nodeValue);
      if (strtolower($editionID) == "all") {
        foreach ($terms as $term) $result[$term->name]["Publish"] = $value;
        $valid = $value;
        drupal_set_message('Editions ALL found. All editions set to '.$value.'. Editions iteration terminated.','notice');
        break;
      } else {
        if (array_key_exists($editionID, $result)) {
          $valid = ($valid || $value);
          $result[$editionID]['Publish'] = $value;
        } else {
          drupal_set_message('Edition: "'.$editionID.'" not found in vocabulary. Edition rejected','error');
        }
      }
    }
    if ($valid) {
      $node->field_editions = array(array("value"=>serialize($result)));
    } else {
      //$node->rejected = '';
      drupal_set_message(t('ALL Edition set to false.'),'error');
      // TODO: FATAL ERROR - PACKAGE - DONE NEED CHECK
    }
  } else {
    drupal_set_message('Edition vocabulary not set. See settings.','error');
    return '';
  }
}

/**
* Process field Auto Publish from DOM element.
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
function wallyimportpackage_set_field_autopublish(&$node, $dom, &$log, $importer){
  module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
  if (wallytoolbox_toboolean($dom->nodeValue)){
    $node->field_autopublish[0]['value'] = "Autopublish";
  } else {
    $node->field_autopublish[0]['value'] = "No";
    $node->status = 0;
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
function wallyimportpackage_set_field_channels(&$node, $dom, $log, $importer) {
  if (variable_get('wallymport_debug',0) or TRUE){
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
    $error_msg = t('No channels found. Package rejected');
    drupal_set_message($error_msg, 'error');
    $log['error'] = TRUE;
    $log['error_msg'] = $error_msg;
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
function wallyimportpackage_set_field_commentsallowed(&$node, $dom, &$log, $importer){
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
function wallyimportpackage_set_field_freeaccess(&$node, $dom, &$log, $importer){
  module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
  (wallytoolbox_toboolean($dom->nodeValue)) ? 
    $node->field_freeaccess[0]['value'] = "Free" : 
    $node->field_freeaccess[0]['value'] = NULL; 
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
function wallyimportpackage_set_field_packagelayout(&$node, $dom, &$log, $importer){
  $value = $dom->nodeValue;
  $node->field_packagelayout = array();

  if (variable_get('wallymport_debug',0) or TRUE) {
    $log['log'][] = 'process PackageLayout: '.$dom->nodeName;
  }

  if (is_numeric($value)) {
    $node->field_packagelayout[]['value'] = $value;
  } elseif ($tax_term = taxonomy_get_term_by_name($value)) {
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
function wallyimportpackage_set_field_summary(&$node, $dom, &$log, $importer){
  $value = $dom->nodeValue;
  $node->field_summary[0]['value'] = $value;
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
function wallyimportpackage_set_field_externaluri(&$node, $dom, &$log, $importer){
  $value = $dom->nodeValue;
  $node->field_externaluri = array();

  if (variable_get('wallymport_debug',0) or TRUE) {
    $log['log'][] = 'process ExternalURI: '.$dom->nodeName;
  }

  $uri_response = drupal_http_request($value, array(), 'GET', NULL, wally_variable_get('wallytoolbox_http_request_retry', 1), wally_variable_get('wallytoolbox_http_request_timeout', 3));
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
function wallyimportpackage_set_field_packagecopyright(&$node, $dom, &$log, $importer){
  $value = $dom->nodeValue;
  $node->field_packagecopyright = array();

  if (variable_get('wallymport_debug',0) or TRUE) {
    $log['log'][] = 'process PackageCopyright: '.$dom->nodeName;
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
function wallyimportpackage_set_field_mainstory(&$node, $dom, &$log, $importer) {
  // Process the object & getting NID
  $object = wimprt_process_object($dom, $log, $importer, 'TextObjectType');
  if (!$object->rejected && _wimprthelp_check_is_node($object)) {
    if ($importer->wallymport_preview) {
      $node->field_mainstory_nodes[0] = $object;
      $node->taxonomy = $object->taxonomy;
      $node->field_markers = $object->field_markers;
      $node->field_locations = $object->field_locations;
      $node->field_entities = $object->field_entities;
      $node->field_persons = $object->field_persons;
      $node->field_free_tags = $object->field_free_tags;
      $node->field_concepts = $object->field_concepts;
      $node->field_iptc = $object->field_iptc;
    } else {
      $node->field_mainstory[0]['nid'] = $object->nid;
    }
  } else {
    $node->rejected = TRUE;
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
function wallyimportpackage_set_field_mainobject(&$node, $dom, &$log, $importer) {
  if ($dom->hasAttribute('xsi:type')){
    $object = new stdClass;
    if (strtolower($dom->getAttributeNode('xsi:type')->value) == 'photoobjecttype') {
      $object = wimprt_process_object($dom, $log, $importer, 'photoobjecttype');
    } elseif (strtolower($dom->getAttributeNode('xsi:type')->value) == 'audioobjecttype') {
      $object = wimprt_process_object($dom, $log, $importer, 'audioobjecttype');
    } elseif (strtolower($dom->getAttributeNode('xsi:type')->value) == 'MultimediaObjectType') {
      $object = wimprt_process_object($dom, $log, $importer, 'digitalobjecttype');
    } elseif (strtolower($dom->getAttributeNode('xsi:type')->value) == 'videoobjecttype') {
      $object = wimprt_process_object($dom, $log, $importer, 'videoobjecttype');
    }
        
    if (!$object->rejected && _wimprthelp_check_is_node($object)) {
      if ($importer->wallymport_preview) {
        $node->field_mainobject_nodes[0] = $object;
        $node->taxonomy = $object->taxonomy;
        $node->field_markers = $object->field_markers;
        $node->field_locations = $object->field_locations;
        $node->field_entities = $object->field_entities;
        $node->field_persons = $object->field_persons;
        $node->field_free_tags = $object->field_free_tags;
        $node->field_concepts = $object->field_concepts;
        $node->field_iptc = $object->field_iptc;
      } else {
        $node->field_mainobject[0]['nid'] = $object->nid;
      }
    } else {
      $node->rejected = TRUE;
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
function wallyimportpackage_set_field_poll(&$node, $dom, &$log, $importer){
  // Process the object & getting NID
  $object = wimprt_process_object($dom, $log, $importer, 'pollobjecttype');

  if (!$object->rejected && _wimprthelp_check_is_node($object)) {
    if ($importer->wallymport_preview) {
      $node->field_mainpoll_nodes[0] = $object;
    } else {
      $node->field_mainpoll[0]['nid'] = $object->nid;
    }
  } else {
    $node->rejected = TRUE;
  }
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
function wallyimportpackage_set_field_EmbeddedContent(&$node, $dom, &$log, $importer){
  // Embeded objects
  static $i = 100;
  $results = array();
  $node->field_embededobjects = array();
  $node->field_embededobjects_nodes = array();
  $arraytemporaire = array();
  if($embedobjects = $dom->getElementsByTagName('EmbeddedObjects')->item(0)) {
    foreach(wimprt_process_list($embedobjects, $log, $importer, 'object') as $result) {
      if ((true||!$result->rejected) && (!array_key_exists($result->nid, $results) || $importer->wallymport_preview)) {
        $results[$result->nid] = $result->nid;
        $embedobjectsorder = $result->embeddedobjectsorder ? $result->embeddedobjectsorder : $i++;
        $arraytemporaire[$embedobjectsorder]['nid'] = $result->nid;
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
  $linkslists = $dom->getElementsByTagName('LinksLists')->item(0);
  $linkslist = $dom->getElementsByTagName('LinksList')->item(0);
  if ($linkslists && $linkslist){
    if ($linkslist->getElementsByTagName('Links')->item(0)->nodeValue != NULL){
      foreach (wimprt_process_list($linkslists, $log, $importer, 'object') as $result) {
        if (!$result->rejected && (!array_key_exists($result->nid, $links) || $importer->wallymport_preview)) {
          $links[$result->nid] = $result->nid;
          $node->field_linkedobjects[]['nid'] = $result->nid;
          if ($importer->wallymport_preview){
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
function wallyimportpackage_objecttype_preprocess(&$node, $dom, $importer){
  $node->wallymport_node = TRUE;
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
function wallyimportpackage_create_or_load_object_textobjecttype($dom, &$log, $importer){
  $node = new stdClass;
  $node->type = 'wally_textobject';
  $node->wallymport_node = TRUE;
  $ext_ref = _wimprthelp_plain_text($dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue);
  if ($node_temp = wallytoolbox_get_node_by_cckfield(array("field_externalreference" => $ext_ref),'wally_textobject')){
    $node = $node_temp[0];
  }
  
  $uid = $importer->default_user;
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
function wallyimportpackage_process_object_textobjecttype (&$node, $dom_element, &$log, $importer){
  //Map XML Attributes 
  //put common attributes
  
  wallyimportpackage_objecttype_preprocess($node, $dom_element, $importer);
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
 * Process field Body from DOM element.
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
function wallyimportpackage_set_field_body(&$node, $dom, &$log, $importer){
  $value = $dom->nodeValue;
  $node->field_body[0]['value'] = $value;
  $node->field_body[0]['format'] = wallyimportpackage_get_default_format();
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
function wallyimportpackage_set_field_pollquestions(&$node, $dom, &$log, $importer){
  if (module_exists('webform')){
    $node->webform = array();
    $node->webform['confirmation'] = '';
    $node->webform['confirmation_format'] = 2;
    $node->webform['redirect_url'] = '<none>';
    $node->webform['status'] = 1;
    $node->webform['block'] = 0;
    $node->webform['teaser'] = 0;
    $node->webform['allow_draft'] = 0;
    $node->webform['auto_save'] = 0;
    $node->webform['submit_notice'] = 1;
    $node->webform['submit_text'] = '';
    $node->webform['submit_limit'] = 1;
    $node->webform['submit_interval'] = -1;
    $node->webform['total_submit_limit'] = -1;
    $node->webform['total_submit_interval'] = -1;
    $node->webform['record_exists'] = TRUE;
    $node->webform['roles'] = array(0 => 1, 1 => 2);
    $node->webform['emails'] = array();

    $i = 1;
    foreach($dom->getElementsByTagName("PollQuestion") as $pollquestion){
      if (isset($node->nid)){
        $node->webform['components'][$i]['nid'] = $node->nid;
      }
      $node->webform['components'][$i]['cid'] = $i;
      $node->webform['components'][$i]['form_key'] = _webform_safe_name($pollquestion->getElementsbyTagName("Question")->item(0)->nodeValue);
      $node->webform['components'][$i]['name'] = $pollquestion->getElementsbyTagName("Question")->item(0)->nodeValue;
      $node->webform['components'][$i]['type'] = 'select';
      $node->webform['components'][$i]['value'] = '';
      $node->webform['components'][$i]['mandatory'] = 0;
      $node->webform['components'][$i]['pid'] = 0;
      $node->webform['components'][$i]['weight'] = 0;
      $node->webform['components'][$i]['page_num'] = 1;
      $node->webform['components'][$i]['extra'] = array();

      //Answer
      $enter = '';
      $items = '';
      foreach($pollquestion->getElementsByTagName("Answer") as $answer){
        if ($answer->nodeValue && !empty($answer->nodeValue)) {
          $items .= $enter.$answer->nodeValue.'|'.$answer->nodeValue;
          $enter = "\n";
        }
      }

      //Radio / checkboxes
      $choice = 1;
      if ($pollquestion->hasAttribute("Choice")){
        if ($pollquestion->getAttributeNode("Choice")->value === "radioButton"){
          $choice = 0;
        }
      }

      $node->webform['components'][$i]['extra']['items'] = $items;
      $node->webform['components'][$i]['extra']['multiple'] = $choice;
      $node->webform['components'][$i]['extra']['title_display'] =  'before';
      $node->webform['components'][$i]['extra']['private'] = 0;
      $node->webform['components'][$i]['extra']['aslist'] = 0;
      $node->webform['components'][$i]['extra']['optrand'] = 0;
      $node->webform['components'][$i]['extra']['conditional_operator'] = '=';
      $node->webform['components'][$i]['extra']['other_option'] = NULL;
      $node->webform['components'][$i]['extra']['other_text'] = 'Other...';
      $node->webform['components'][$i]['extra']['description'] = '';
      $node->webform['components'][$i]['extra']['custom_keys'] = FALSE;
      $node->webform['components'][$i]['extra']['options_source'] = '';
      $node->webform['components'][$i]['extra']['conditional_component'] = '';
      $node->webform['components'][$i]['extra']['conditional_values'] = '';

      $i++;
    }
  } else {
    drupal_set_message(t('You must install the module "WebForm" to use the poll import'), 'error');
    $node->rejected = TRUE;
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
function wallyimportpackage_create_or_load_object_pollobjecttype($dom, &$log, $importer){
  $node = new stdClass;
  $node->type = 'wally_pollobject';
  $node->wallymport_node = TRUE;
  $ext_ref = _wimprthelp_plain_text($dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue);
  if ($node_temp = wallytoolbox_get_node_by_cckfield(array("field_externalreference" => $ext_ref), 'wally_pollobject')){
    $node = $node_temp[0];
  }

  $uid = $importer->default_user;
  $node->uid = $uid;

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
function wallyimportpackage_process_object_pollobjecttype(&$node, $dom, &$log, $importer){
  static $proceed_file_path;
  //put common attribute
  wallyimportpackage_objecttype_preprocess($node, $dom, $importer);

  wimprt_process_node_default($node, $dom, $log, $importer);
   
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
function wallyimportpackage_create_or_load_object_photoobjecttype($dom, &$log, $importer){
  $node = new stdClass;
  $node->type = 'wally_photoobject';
  $node->wallymport_node = TRUE;
  $ext_ref =_wimprthelp_plain_text($dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue);
  if ($node_temp = wallytoolbox_get_node_by_cckfield(array("field_externalreference" => $ext_ref), "wally_photoobject")){
    $node = $node_temp[0];
  }

  $uid = $importer->default_user;
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
function wallyimportpackage_process_object_photoobjecttype(&$node, $dom, &$log, $importer) {
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
  
  //put common attributes
  wallyimportpackage_objecttype_preprocess($node, $dom, $importer);
  
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
function wallyimportpackage_create_or_load_object_audioobjecttype($dom, &$log, $importer){
  $node = new stdClass;
  $node->type = 'wally_audioobject';
  $node->wallymport_node = TRUE;
  $ext_ref = _wimprthelp_plain_text($dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue);
  if ($node_temp = wallytoolbox_get_node_by_cckfield(array("field_externalreference" => $ext_ref), "wally_audioobject")){
    $node = $node_temp[0];
  }

  $uid = $importer->default_user;
  $node->uid = $uid;

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
function wallyimportpackage_process_object_audioobjecttype(&$node, $dom, &$log, $importer){
  $value = $dom->nodeValue;
  
  //put common attributes
  wallyimportpackage_objecttype_preprocess($node, $dom, $importer);

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
function wallyimportpackage_create_or_load_object_videoobjecttype($dom, &$log, $importer){
  $node = new stdClass;
  $node->type = 'wally_videoobject';
  $node->wallymport_node = TRUE;
  $ext_ref = _wimprthelp_plain_text($dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue);
  if ($node_temp = wallytoolbox_get_node_by_cckfield(array("field_externalreference" => $ext_ref),"wally_videoobject")){
    $node = $node_temp[0];
  }

  $uid = $importer->default_user;
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
function wallyimportpackage_process_object_videoobjecttype(&$node, $dom, &$log, $importer){
  static $procced_file_path;
  $value = $dom->nodeValue;
  
  //put common attributes
  wallyimportpackage_objecttype_preprocess($node, $dom, $importer);

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
function wallyimportpackage_create_or_load_object_multimediaobjecttype($dom, &$log, $importer){
  $node = new stdClass;
  $node->type = 'wally_digitalobject';
  $node->wallymport_node = TRUE;
  $ext_ref = $dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue;
  if ($node_temp = wallytoolbox_get_node_by_cckfield(array("field_externalreference" => $ext_ref),"wally_digitalobject")){
    $node = $node_temp[0];
  }

  $uid = $importer->default_user;
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
function wallyimportpackage_process_object_multimediaobjecttype(&$node, $dom, &$log, $importer){
  $value = $dom->nodeValue;
  
  //put common attributes
  wallyimportpackage_objecttype_preprocess($node, $dom, $importer);

  $token_user = $importer->default_user ? user_load($importer->default_user) : NULL;
  _wallymportpackage_manage_file_from_uri($node, $importer, $dom, $token_user, 'emother', 'wally_multimediaobject', 'field_objectfile', 'field_object3rdparty');
 
  //$node->field_link[0]['url']= $dom->getElementsByTagName('LinkURI')->item(0)->nodeValue;
  //$node->field_link[0]['title']= $dom->getElementsByTagName('LinkTitle')->item(0)->nodeValue;
  wimprt_process_node_default($node, $dom, $log, $importer);
  
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
function wallyimportpackage_set_field_thumbnail(&$node, $dom, &$log, $importer){
  $thumbnail = $dom->nodeValue;
  $node->field_thumbnail[0] = _wimprthelp_create_file_from_uri($thumbnail, '', $importer);
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
function wallyimportpackage_process_object_link(&$node, $dom, &$log, $importer){
  wallyimportpackage_typetype_preprocess($node, $dom, $importer);

  //$node->type='wally_linktype';
  wimprt_process_type_default($node, $dom, $log, $importer);

  $uri = $dom->getElementsByTagName('URI')->item(0);
  $uri_value = $uri->nodeValue;
  $uri = parse_url($uri_value);

  if (strtolower($uri['scheme']) == "extref"){
    $ext_ref = $uri['host'];

    if ($link = wallytoolbox_get_node_by_cckfield(array("field_externalreference" => $ext_ref))) {
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
function wallyimportpackage_process_type_link(&$node, $dom, &$log, $importer){
  wallyimportpackage_typetype_preprocess($node, $dom, $importer);

  //$node->type='wally_linktype';
  wimprt_process_type_default($node, $dom, $log, $importer);

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
function wallyimportpackage_create_or_load_object_linkslist($dom, &$log, $importer){
  $node = new stdClass;
  $node->type = 'wally_linkslistobject';
  $node->wallymport_node = TRUE;
  $ext_ref =_wimprthelp_plain_text($dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue);
  if ($node_temp = wallytoolbox_get_node_by_cckfield(array("field_externalreference" => $ext_ref),"wally_linkslistobject")){
    $node = $node_temp[0];
  }
  
  $uid = $importer->default_user;
  $node->uid = $uid;

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
function wallyimportpackage_create_or_load_object_link($dom, &$log, $importer){
  $node = new stdClass;
  $node->type = 'wally_linktype';
  //je pense qu'il faut enlever ca car les liens n'ont pas de tag ExternalReference
  $ext_ref = _wimprthelp_plain_text($dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue);
  if ($node_temp = wallytoolbox_get_node_by_cckfield(array("field_externalreference" => $ext_ref), $node->type)){
    $node = $node_temp[0];
  }

  $uid = $importer->default_user ? $importer->default_user : 1;
  $node->uid = $uid;

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
function wallyimportpackage_create_or_load_type_link($dom, &$log, $importer){
  $node = new stdClass;
  $node->type = 'wally_linktype';
  //je pense qu'il faut enlever ca car les liens n'ont pas de tag ExternalReference
  $ext_ref = _wimprthelp_plain_text($dom->getElementsByTagName('ExternalReference')->item(0)->nodeValue);
  if ($node_temp = wallytoolbox_get_node_by_cckfield(array("field_externalreference" => $ext_ref), $node->type)){
    $node = $node_temp[0];
  }

  $uid = $importer->default_user ? $importer->default_user : 1;
  $node->uid = $uid;

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
function wallyimportpackage_process_object_linkslist(&$node, $dom, &$log, $importer){
  wimprt_process_node_default($node, $dom, $log, $importer);
  $results = array();
  $links = $dom->getElementsByTagName("Links")->item(0);
  
  //DDU: We need to reset the field first see: http://rossel.audaxis.com/jira/browse/WALLY-237
  $node->field_links_list = array();
  $node->field_links_list_nodes = array();
  
  foreach(wimprt_process_list($links, $log, $importer, 'type') as $result){
    if (!array_key_exists($result->nid, $results) || $importer->wallymport_preview) {
      $link_item[$result->nid] = $result->nid;
      $node->field_links_list[]['nid'] = $result->nid;
      if ($importer->wallymport_preview){
        $node->field_links_list_nodes[] = $result;
      }
    }
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
function wallyimportpackage_set_field_title(&$node, $dom, &$log, $importer){
  $value = $dom->nodeValue;
  $node->title = _wimprthelp_plain_text($value);
  $node->field_displayed_title = array(array('value' => $value));
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
function wallyimportpackage_set_field_rating(&$node, $dom, &$log, $importer){
  $value = $dom->nodeValue;
  $vid = variable_get('wallymport_rating', NULL);
  $item = wallytoolbox_taxonomy_get_term_by_path($value, $vid);
  $node->field_rating[0]['value'] = $item[0]->tid;
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
function wallyimportpackage_set_field_textbarette(&$node, $dom, &$log, $importer){
  $value = $dom->nodeValue;
  $node->field_textbarette[0]['value'] = $value;
  $node->field_textbarette[0]['format'] = wallyimportpackage_get_default_format();
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
function wallyimportpackage_set_field_textforetitle(&$node, $dom, &$log, $importer){
  $value = $dom->nodeValue;
  $node->field_textforetitle[0]['value'] = $value;
  $node->field_textforetitle[0]['format'] = wallyimportpackage_get_default_format();
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
function wallyimportpackage_set_field_texttitle(&$node, $dom, &$log, $importer){
  $value = $dom->nodeValue;
  $node->field_objecttitle = array(array("value" => $value));
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
function wallyimportpackage_set_field_textsubtitle(&$node, $dom, &$log, $importer){
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
function wallyimportpackage_set_field_textchapo(&$node, $dom, &$log, $importer){
  $value = $dom->nodeValue;
  $node->field_textchapo[0]['value'] = $value;
  $node->field_textchapo[0]['format'] = wallyimportpackage_get_default_format();
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
function wallyimportpackage_set_field_textbody(&$node, $dom, &$log, $importer){
  $value = $dom->nodeValue;
  $node->field_textbody[0]['value'] = $value;
  $node->field_textbody[0]['format'] = wallyimportpackage_get_default_format();
}

function wallyimportpackage_get_default_format(){
  return variable_get('filter_default_format', 1);
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
function wallyimportpackage_set_field_extractmedium(&$node, $dom, &$log, $importer){
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
function wallyimportpackage_set_field_extractshort(&$node, $dom, &$log, $importer){
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
function wallyimportpackage_set_field_copyright(&$node, $dom, &$log, $importer){
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
function wallyimportpackage_set_field_byline(&$node, $dom, &$log, $importer){
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
function wallyimportpackage_create_or_load_type_location($dom, $log, $importer){
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
  }
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
function wallyimportpackage_process_type_location(&$node, $dom, &$log, $importer){
  wallyimportpackage_typetype_preprocess($node, $dom, $importer);

  // UID
  $node->uid = $importer->default_user ? $importer->default_user : 1;

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
    $node->field_address = _wallyimportpackage_address_dom_to_ccklocation($addresses, $log, $node->title);
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
        $address['latitude'] = $lat;
        $address['longitude'] = $lon;
        $address['locpick']["user_latitude"] = $lat;
        $address['locpick']["user_longitude"] = $lon;
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
function wallyimportpackage_set_field_marker(&$node, $dom, &$log, $importer){
  $node->field_markers = array();

  if (variable_get('wallymport_debug',0) or TRUE){
    $log['log'][] = 'process Marker: '.$dom->nodeName;
  }
  $node->field_markers[0] = array();
  $address = _wallyimportpackage_address_dom_to_ccklocation($dom->getElementsByTagName("Address"), $log);
  $node->field_markers[0] = $address[0];
  $node->field_markers[0]['name'] = $dom->getAttribute('Name');

  if ($dom->getElementsByTagName("LocationGeo")->length){
    list($longitude, $latitude, $zoom) = explode(',', $dom->getElementsByTagName('LocationGeo')->item(0)->nodeValue);
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
function wallyimportpackage_set_field_locations(&$node, $dom, &$log, $importer){
  $node->field_locations = array();

  if (variable_get('wallymport_debug',0) or TRUE){
    $log['log'][] = 'process Locations: '.$dom->nodeName;
  }
  $termes = $dom->getElementsByTagName("Term");
  $results = array();
  foreach ($termes as $terme){
    $results[] = wimprt_process_type($terme, $log, $importer, 'location');
  }

  // Get Location Taxonomy for all nodes.
  $node->field_locations = array();
  foreach($results as $result) {
    if (isset($result->field_locationtaxonomy)) {
      if ($result->field_locationtaxonomy[0]) {
        $node->field_locations[] = array("value" => $result->field_locationtaxonomy[0]["value"]);
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
function wallyimportpackage_set_field_taxonomy(&$node, $dom, $log, $importer) {
  $result = array();
  if (variable_get('wallymport_debug',0) or TRUE) {
    $log['log'][] = 'process Taxonomy: '.$dom->nodeName;
  }
  $freetagtaxonomy_vid = variable_get('wallymport_freetagtaxonomy', NULL);
  if ($freetagtaxonomy_vid) {
    $vocabularies = $dom->getElementsByTagName("Vocabulary");
    if ($vocabularies->length) {

      foreach($vocabularies as $vocabulary) {
        $function_exist = FALSE;
        foreach ($importer->importer_functions as $importer_function){
          if (function_exists($importer_function.'_set_field_'.$vocabulary->getAttributeNode("VocabularyName")->value)){
            if (isset($node->nid)){
              $vid = _wimprthelp_getTaxonomyVidFromDamName($vocabulary->getAttributeNode("VocabularyName")->value);
              wallytoolbox_taxonomy_unbindalltermofnodefromavocabulary($vocabulary->getAttributeNode("VocabularyName")->value, $node, $vid);
            }
            call_user_func($importer_function.'_set_field_'.$vocabulary->getAttributeNode("VocabularyName")->value, $node, $vocabulary, $log, $importer);
            $function_exist = TRUE;
            break;
          }
        }
        if (!$function_exist & function_exists("wimprt_set_field_".$vocabulary->getAttributeNode("VocabularyName")->value)){
          if (isset($node->nid)){
            $vid = _wimprthelp_getTaxonomyVidFromDamName($vocabulary->getAttributeNode("VocabularyName")->value);
            wallytoolbox_taxonomy_unbindalltermofnodefromavocabulary($vocabulary->getAttributeNode("VocabularyName")->value, $node, $vid);
          }
          call_user_func('wimprt_set_field_'.$vocabulary->getAttributeNode("VocabularyName")->value, $node, $vocabulary, $log, $importer);
          $function_exist = TRUE;
        }
        if (!$function_exist){
          $vid = _wimprthelp_getTaxonomyVidFromDamName($vocabulary->getAttributeNode("VocabularyName")->value);
          if ($vid != NULL){
            if (isset($node->nid)){
              wallytoolbox_taxonomy_unbindalltermofnodefromavocabulary($vocabulary->getAttributeNode("VocabularyName")->value, $node, $vid);
            }
          }
          wallyimportpackage_set_field_taxonomy_vocabulary($vocabulary, $result, $vid);
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
function wallyimportpackage_set_field_taxonomy_vocabulary($vocabulary, &$result, $vid) {
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
    } else {
      $result_set = _wimprthelp_getfieldnamefromvocabularyname($vocabulary_name);
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
              $tid = wallytoolbox_add_taxonomy_term($t, $vid, $t, 0, NULL);
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
function wallyimportpackage_set_field_entities(&$node, $dom, &$log, $importer){
  if (variable_get('wallymport_debug',0) or TRUE){
    $log['log'][] = 'process Entities: '.$dom->nodeName;
  }
  $termes = $dom->getElementsByTagName("Term");
  $results = array();
  foreach ($termes as $terme){
    $results[] = wimprt_process_type($terme, $log, $importer, 'entity');
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
function wallyimportpackage_typetype_preprocess(&$node, $dom, $importer){
  if ($importer->wallymport_preview){
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
function wallyimportpackage_create_or_load_type_entity($dom, $log, $importer){
  if (!($vid = variable_get('wallymport_entity', NULL))) {
    // TO DO FATAL :: No location taxonomy vocabulary
    $error_msg = t('Entity Taxonomy Vocabulary not Found. See admin.');
    drupal_set_message($error_msg, 'error');
    $node->rejected = TRUE;
    $log['error'] = TRUE;
    $log['error_msg'] = $error_msg;
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
    } elseif (count($result) > 1){
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
  }
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
function wallyimportpackage_process_type_entity(&$node, $dom, &$log, $importer){
  wallyimportpackage_typetype_preprocess($node, $dom, $importer);

  // Node type
  $node->type = "wally_entitytype";

  // External Reference
  //$node->field_externalreference = array(array("value" => $dom->getAttributeNode("EntityID")->value));

  // Node UID
  $node->uid = $importer->default_user ? $importer->default_user : 1;

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
function wallyimportpackage_set_field_persons(&$node, $dom, &$log, $importer){
  $node->field_persons = array();

  if (variable_get('wallymport_debug',0) or TRUE){
    $log['log'][] = 'process Persons: '.$dom->nodeName;
  }
  $termes = $dom->getElementsByTagName("Term");
  $results = array();
  foreach ($termes as $terme){
    $results[] = wimprt_process_type($terme, $log, $importer, 'person');
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
function wallyimportpackage_set_field_authors(&$node, $dom, &$log, $importer){
  if (variable_get('wallymport_debug',0) or TRUE){
    $log['log'][] = 'process Authors: '.$dom->nodeName;
  }
  $termes = $dom->getElementsByTagName("Term");
  $results = array();
  foreach ($termes as $terme){
    $results[] = wimprt_process_type($terme, $log, $importer, 'author');
  }
  // Get Location Taxonomy for all nodes.
  $node->field_authors = array();
  foreach($results as $result) {
    if (isset($result->field_authortaxonomy)) {
      if ($result->field_authortaxonomy[0]) {
        $node->field_authors[] = array("value" => $result->field_authortaxonomy[0]["value"]);
        $node->taxonomy[$result->field_authortaxonomy[0]["value"]] = taxonomy_get_term($result->field_authortaxonomy[0]["value"]);
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
function wallyimportpackage_create_or_load_type_person($dom, $log, $importer){
  if (!($vid = variable_get('wallymport_person',NULL))) {
    // TO DO FATAL :: No location taxonomy vocabulary
    $error_msg = t('Person Taxonomy Vocabulary  not Found. See admin.');
    drupal_set_message($error_msg, 'error');
    $result->rejected = TRUE;
    $log['error'] = TRUE;
    $log['error_msg'] = $error_msg;
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
  }
  // Set the taxonomy field.

  $result->field_persontaxonomy = array( array("value" => $tid) );
  // Set drupal node->taxonomy.
  $result->taxonomy = array($tid => $taxonomy);
  return $result;
}

/**
 * Create or load an author type depending of term.
 * 
 * If no node match term, a new node is created.
 * 
 * @param $dom
 *   The DomDocument who is being proceed (A DomElement)
 * @param $log
 *   The log object build recursively in different operation. this could track error in the process and build nodes.
 *
 * @return
 *   A node object
 */
function wallyimportpackage_create_or_load_type_author($dom, $log, $importer){
  if (!($vid = variable_get('wallymport_author',NULL))) {
    // TO DO FATAL :: No location taxonomy vocabulary
    $error_msg = t('Author Taxonomy Vocabulary  not Found. See admin.');
    drupal_set_message($error_msg, 'error');
    $result->rejected = TRUE;
    $log['error'] = TRUE;
    $log['error_msg'] = $error_msg;
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
  }
  // Set the taxonomy field.

  $result->field_authortaxonomy = array( array("value" => $tid) );
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
function wallyimportpackage_process_type_person(&$node, $dom, &$log, $importer){
  wallyimportpackage_typetype_preprocess($node, $dom, $importer);

  // Node type
  $node->type = "wally_persontype";

  // External Reference supprimé
  // $node->field_externalreference = array(array("value"=>$dom->getAttributeNode("PersonID")->value));

  // Node UID
  $node->uid = $importer->default_user ? $importer->default_user : 1;

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
 * Process Author type from DOM.
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
function wallyimportpackage_process_type_author(&$node, $dom, &$log, $importer){
  wallyimportpackage_process_type_person($node, $dom, $log, $importer);
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
function _wallyimportpackage_address_dom_to_ccklocation($addresses, &$log, $location_name = NULL) {
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

function _wallyimportpackage_find_destination($current_destinations, $new_dest) {
  if (is_array($current_destinations)) {
    foreach($current_destinations as $cur_dest) {
      if (($cur_dest['tid'] == $new_dest['tid']) && ($cur_dest['target'] == $new_dest['target'])) {
        if (strstr($cur_dest['rank'], 'DNC')) {
          $cur_dest['rank'] = substr($cur_dest['rank'], 3);
        }
        return $cur_dest;
      }
    }
  }
  return FALSE;
}

function _wallymportpackage_manage_file_from_uri(&$node, $importer, $dom, $token_user, $emfield_module, $object_type, $field_name, $rdfile_name) {
  module_load_include('inc', 'content', 'includes/content.crud');
  
  $dom_fileuri = $dom->getElementsByTagName('FileURI');
  $dom_uri = $dom->getElementsByTagName('URI');
  if ($dom_fileuri->item(0)) {
    $file_uri = $dom_fileuri->item(0)->nodeValue;
  } elseif ($dom_uri->item(0)) {
    $file_uri = $dom_uri->item(0)->nodeValue;
  } else {
    return;
  }
  
  $uri = parse_url($file_uri);
  if ($uri['scheme'] == 'embed') {
    // If embed we don't want to treat it like a 3rd party
    $rdpartyMatch = array();
  } else {
    $rdpartyMatch = emfield_parse_embed(NULL, $file_uri, $emfield_module);
  }

  if (empty($rdpartyMatch)){//No a 3rdparty
    // Copy the file in the correct subfolder
    $field = content_field_instance_read(array('type_name' => $object_type, 'field_name' => $field_name));
    $new_path = module_exists('token') ? token_replace($field[0]['widget']['file_path'], 'user', $token_user) : $field[0]['widget']['file_path'];
    $node->{$field_name}[0] = _wimprthelp_create_file_from_uri($file_uri, $new_path, $importer);
  } else {//Is a 3rd party
    $node->{$rdfile_name}[0]['embed'] = $file_uri;
  }
}
