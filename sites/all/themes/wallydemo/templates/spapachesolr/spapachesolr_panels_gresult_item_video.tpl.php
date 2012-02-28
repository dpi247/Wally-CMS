<?php 
//
// les data (resultats sont dans $results
//

	$video = $result["wallynode"][0];
	$embed_video = wallydemo_get_video_infos_and_display($video);

  //Récupère l'id du dernier package auquel le node a été associé pour reconstruire l'url
  $packages_id = wallytoolbox_get_node_by_reference($video);
  $packages_nb = count($packages_id);
  if ($packages_nb > 1){
    $packages_id_ordered = spapachesolr_panels_order_packages($packages_id, 'created', 'ASC');
    $package_id = $packages_id_ordered[0]['nid'];
  } else {
    $package_id = $packages_id[0]['nid'];
  }
  $node_path = drupal_get_path_alias("node/".$package_id);	
	
//	dsm($embed_video); 
	
	
	$photo_title_link = "";
	
	$html = ""; 
	
	switch($type) {
      // main
	  default:
		// Assume "line"
		$html .= "<a href='".$node_path."'><span title=\"".$result["title"]."\" alt=\"".$result["title"]."\"></span><img width='90' src='".$embed_video["thumbnail"]."'></a>";
		break; 
	}
	print $html;
//ici unset variables
unset($result);
unset($video);
unset($embed_video);  	
?>
