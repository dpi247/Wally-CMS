<?php
if(isset($_GET['nid']) && is_numeric($_GET['nid'])){

	$nid = $_GET['nid'];
	$tstamp = time();	
	
	//Ceci n'est pas encore utilisé dans la v1... le but est de créer les trending topics.
	if(isset($_GET['terms'])){
		//ON s'assure que tous les termes ici sont bien des id numériques et on recrée $safe_terms avec les valeurs vérifiées.
		$term_array = explode(' ', $_GET['terms']);
		if(is_array($term_array)){
			$safe_terms = array();
			foreach ($term_array as $term){
				if(is_numeric($term)){
					$safe_terms[] = $term; 
				}
			}
			 $terms = implode(' ',$safe_terms);
		}
	}
  
  $wally_install_root = $_SERVER['DOCUMENT_ROOT'];
	
	chdir($wally_install_root);
	include_once ('./includes/bootstrap.inc');
	drupal_bootstrap(DRUPAL_BOOTSTRAP_DATABASE);

  $params = array();
  $params[] = $_GET['param01'];
  $params[] = $_GET['param02'];
  $params[] = $_GET['param03'];
  $params[] = $_GET['param04'];
  $params[] = $_GET['param05'];
  $params[] = $_GET['param06'];
  $params[] = $_GET['param07'];
  $params[] = $_GET['param08'];
  $params[] = $_GET['param09'];
  $params[] = $_GET['param10'];

	if(isset($terms) && $terms != ''){
		db_query($query = "INSERT INTO wallystat_tempdata (`nid`, `title`, `param01`, `param02`, `param03`, `param04`, `param05`, `param06`, `param07`, `param08`, `param09`, `param10`, `timestamp`, `terms`) VALUES (%d, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', %d, '%s')", $nid, $_GET['title'], $_GET['param01'], $_GET['param02'], $_GET['param03'], $_GET['param04'], $_GET['param05'], $_GET['param06'], $_GET['param07'], $_GET['param08'], $_GET['param09'], $_GET['param10'], $tstamp, $terms);//(".$nid.", ".$tstamp.", '".$terms."')";
	}else{
		$query = "INSERT INTO wallystat_tempdata (`nid`, `timestamp`) VALUES (".$nid.", ".$tstamp.")";
	}

	
	//$result = db_query($query);
	//var_dump($result);
	//echo "<hr/>";
	
	
}



