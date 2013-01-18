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
function wallyimportpackageV13_node_process_link(&$node, $dom, &$log, $importer){
  wallyimportpackage_typetype_preprocess($node, $dom, $importer);

  wimprt_node_process_fields($node, $dom, $log, $importer);

  $uri = $dom->getElementsByTagName('URI')->item(0);
  $uri_value = $uri->nodeValue;
  $uri = parse_url($uri->nodeValue);
  if ($uri['scheme'] == "externalRef"){
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
