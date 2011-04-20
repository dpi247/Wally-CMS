<?php
if(isset($_GET['nid']) && is_numeric($_GET['nid'])){

	$nid = $_GET['nid'];
	$tstamp = time();	
	
	//Ceci n'est pas encore utilisé dans la v1... le but est de créer les trending topics.
	if(isset($_GET['terms'])){
		//ON s'assure que tous les termes ici sont bien des id numériques et on recrée $safe_terms avec les valeurs vérifiées.
		$term_array = explode('+', $_GET['terms']);
		if(is_array($term_array)){
			$safe_terms = array();
			foreach ($term_array as $term){
				if(is_numeric($term)){
					$safe_terms[] = $term; 
				}
			}
			 $terms = implode('+',$safe_terms);
		}
	}
	
	if(isset($terms) && $terms != ''){
		$query = "INSERT INTO wallystat_tempdata (`nid`, `timestamp`, `terms`) VALUES (".$nid.", ".$tstamp.", '".$terms."')";
	}else{
		$query = "INSERT INTO wallystat_tempdata (`nid`, `timestamp`) VALUES (".$nid.", ".$tstamp.")";
	}
	$wally_install_root = $_SERVER['DOCUMENT_ROOT'];
	
	chdir($wally_install_root);
	include_once ('./includes/bootstrap.inc');
	drupal_bootstrap(DRUPAL_BOOTSTRAP_DATABASE);
	
	$result = db_query($query);
	//var_dump($result);
	//echo "<hr/>";
	
	
}



