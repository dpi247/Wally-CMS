<?php 
//
// les data (resultats sont dans $results
//
//	dsm($result); 
//	dsm($type,"type"); 

	$digital = $result["wallynode"][0];
	$embed_digital = wallydemo_get_digital_infos_and_display($digital);

  //Récupère l'id du dernier package auquel le node a été associé pour reconstruire l'url
  $packages_id = wallytoolbox_get_node_by_reference($digital);
  $packages_nb = count($packages_id);
  if($packages_nb > 1){
    $packages_id_ordered = spapachesolr_panels_order_packages($packages_id, 'created', 'ASC');
    $package_id = $packages_id_ordered[0]['nid'];
  }else{
    $package_id = $packages_id[0]['nid'];
  }
  $node_path = drupal_get_path_alias("node/".$package_id);	
	
	$photo_title_link = "";
	$html = ""; 
	$html .= "<a href='".$node_path."'>digital-02<img width='90' src='".$embed_digital["thumbnail"]."'></a>";
	print $html; 
	
//ici unset variables
unset($result);
unset($digital);
unset($embed_digital);	
?>
