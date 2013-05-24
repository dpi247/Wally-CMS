<?php 
/**
* Process field source from DOM element.
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
function wallyimportpackageV20_node_set_field_source(&$node, $dom, &$log, $importer){
  $value = $dom->nodeValue;
  $node->field_source = array(array("value" => $value));
}
/**
* Process field lasturlalias from DOM element.
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
function wallyimportpackageV20_node_set_field_lasturlalias(&$node, $dom, &$log, $importer){
  $value = $dom->nodeValue;
  $node->field_lasturlalias = array(array("value" => $value));
}