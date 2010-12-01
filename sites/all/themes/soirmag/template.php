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
    
  ); 
}
