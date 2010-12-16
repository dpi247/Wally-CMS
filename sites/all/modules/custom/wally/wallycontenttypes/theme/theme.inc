<?php

/**
 * Build all the page/node suggestions template for a node of type "package"
 * of any content type with Destinations information.   
 *
 *  Wally specific suggestions are (for pages):  
 * 
 *  - page_{destinations_terms_underscore_separated}.tpl.php
 *  - page_{destinations_terms_underscore_separated}_{node_type}.tpl.php
 *  - page_{destinations_terms_underscore_separated}_{node_type}_{package_layout}.tpl.php
 *  - page_{node_type}_{package_layout}.tpl.php
 *  - page_{node_type}.tpl.php
 *  - page.tpl.php
 * 
 *  Wally specific suggestions are (for nodes):  
 * 
 *  - node-{node_type}.tpl.php
 *  - node_{destinations_terms_underscore_separated}.tpl.php
 *  - node_{destinations_terms_underscore_separated}_{node_type}.tpl.php
 *  - node_{destinations_terms_underscore_separated}_{node_type}_{package_layout}.tpl.php
 *  - node_{node_type}_{package_layout}.tpl.php
 *  - node_{node_type}.tpl.php < node.tpl.php
 *
 *  Wally specific suggestions are (other - types):  
 * 
 *  - {type}-{node_type}.tpl.php
 *  - {type}_{destinations_terms_underscore_separated}.tpl.php
 *  - {type}_{destinations_terms_underscore_separated}_{node_type}.tpl.php
 *  - {type}_{destinations_terms_underscore_separated}_{node_type}_{package_layout}.tpl.php
 *  - {type}_{node_type}_{package_layout}.tpl.php
 *  - {type}_{node_type}.tpl.php < node.tpl.php
 * 
 * @param $variables
 *   Templating variables
 * 
 * @param $type
 *   Prefix added to suggestions (default = page). Could be "page" or "node".
 * 
 * @return 
 *   Array of suggestion pages templates.
 */
function wallycontenttypes_getpackagesuggestions(&$variables, $type="page") {

  $destinations = array();
  $suggestion = array(); 
  $tax_children = array(); 
  $vid=variable_get('wallymport_destinationpath',null);
  $path = ""; 
  $package_layout = (isset($variables['node'])) ? wallycontenttypes_getpackagelayout($variables['node']) : NULL ; 

  // For a node, destination is taken on the 1st destination. 
  if (isset($variables['node']->field_destinations[0]['tid'])) {
    $destinations = array($variables['node']->field_destinations[0]['tid']);
  }
 
  (isset($variables['node']->type)) ? $suggestion[] = $type . '_' . strtolower($variables['node']->type) : NULL ;
 
  if ($package_layout) $suggestion[] = $type . '_' . strtolower($variables['node']->type) . '_' . $package_layout;

  if (count($destinations)) {
    $destination = $destinations[0];
    $tax_children = taxonomy_get_parents_all($destination, $vid);
    $count = count($tax_children);
    for ($i = $count-1; $i >= 0; $i--) {
      ($i == $count-1) ? $path .= strtolower($tax_children[$i]->name) : $path .= "_".strtolower($tax_children[$i]->name);
      if ($type) $suggestion[] = $type . '_' . $path;
      if (isset($variables['node']->type)) $suggestion[] = $type . '_' . $path . '_' . strtolower($variables['node']->type);
      if ($package_layout) $suggestion[] = $type . '_' .$path . '_' . strtolower($variables['node']->type) . '_' . $package_layout;
    }
  }
  return $suggestion;  
}

/**
 * Extract page layout information from the node. 
 *
 * @param $node
 *   The node currently processed
 *
 * @return 
 *   The Package layout as a string or NULL.
 */
function wallycontenttypes_getpackagelayout($node) {
  if (isset($node->field_packagelayout)) {
    $package_layout = taxonomy_get_term($node->field_packagelayout[0]["value"]);
    $package_layout = strtolower(trim($package_layout->name));
  } else {
    $package_layout = NULL;
  }
  return $package_layout; 
}
