<?php

/**
 *  Implémentation du Hook_theme(); 
 */
function soirmag_theme(&$var) {
 return array(

    'soirmag_fil_ariane' => array(
    'arguments' => array("destinations" => NULL, "node" => NULL),
    'template' => 'templates/soirmag_fil_ariane',
    ),

    'soirmag_share' => array(
    'arguments' => array("story" => NULL, "node" => NULL),
    'template' => 'templates/soirmag_share',
    ),
    
    'soirmag_header' => array(
    'arguments' => array("node" => NULL),
    'template' => 'templates/soirmag_header',
    ),
    
    'soirmag_adds' => array(
    'arguments' => array("node" => NULL, "zone" => NULL),
    'template' => 'templates/soirmag_adds',
    ),
    
    
  ); 
}

