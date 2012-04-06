<?php
if (isset($_GET['nid']) && is_numeric($_GET['nid'])){

  $nid = $_GET['nid'];
  $tstamp = time();	
	
  //Ceci n'est pas encore utilisé dans la v1... le but est de créer les trending topics.
  if (isset($_GET['terms'])){
	//ON s'assure que tous les termes ici sont bien des id numériques et on recrée $safe_terms avec les valeurs vérifiées.
	$term_array = explode(' ', $_GET['terms']);
	if (is_array($term_array)){
	  $safe_terms = array();
	  foreach ($term_array as $term){
		if (is_numeric($term)){
		  $safe_terms[] = $term; 
		}
	  }
	  $terms = implode(' ', $safe_terms);
	}
  }
  
  $wally_install_root = $_SERVER['DOCUMENT_ROOT'];
	
  chdir($wally_install_root);
  include_once ('./includes/bootstrap.inc');
  drupal_bootstrap(DRUPAL_BOOTSTRAP_DATABASE);

  $params = array();
  $params['nid'] = $nid;
  $params['timestamp']  = $tstamp;
  
  if (isset($terms) && $terms != ''){
    $params['title']   = $_GET['title'];
    $params['param01'] = $_GET['param01'];
    $params['param02'] = $_GET['param02'];
    $params['param03'] = $_GET['param03'];
    $params['param04'] = $_GET['param04'];
    $params['param05'] = $_GET['param05'];
    $params['param06'] = $_GET['param06'];
    $params['param07'] = $_GET['param07'];
    $params['param08'] = $_GET['param08'];
    $params['param09'] = $_GET['param09'];
    $params['param10'] = $_GET['param10'];
    $params['terms']   = $terms;
    
    cache_set('wallystat_'.$nid.rand(), $params, 'cache_wallystat_tempdata');  
   } else {
    cache_set('wallystat_'.$node.rand(), $params, 'cache_wallystat_tempdata');
  }
}



