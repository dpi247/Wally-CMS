<?php 
//
// les data (resultats sont dans $results
//

  $photo = $result["wallynode"][0];
  
	//Récupère l'id du dernier package auquel le node a été associé pour reconstruire l'url
	$packages_id = wallytoolbox_get_node_by_reference($photo);
	$packages_nb = count($packages_id);
	if($packages_nb > 1){
	  $packages_id_ordered = spapachesolr_panels_order_packages($packages_id, 'created', 'ASC');
	  $package_id = $packages_id_ordered[0]['nid'];
	}else{
	  $package_id = $packages_id[0]['nid'];
	}
	$node_path = drupal_get_path_alias("node/".$package_id);

	$embed_photo = wallydemo_get_photo_infos_and_display($photo);


	$html = ""; 
	
	switch($type) {
		// main
		case "main_size":
			$photo_display = theme('imagecache', 'unebis_small_90x66', $embed_photo['filename'], $embed_photo['title'],$embed_photo['title'].$embed_photo['credit']);
			$html .= "<a href='".$node_path."'>".$photo_display."</a>";
			break;

		default:
			// Assume "line"
			$photo_display = theme('imagecache', 'divers_201x134', $embed_photo['filename'], $embed_photo['title'],$embed_photo['title'].$embed_photo['credit']);
			$html .= "<div class='photo'>";
			$html .= "<a href='".$node_path."'>".$photo_display."</a>";
			$html .= "<h2>".$result["title"]."</h2>";
			$html .= "<p>".html_entity_decode($result["snippet"])."</p>";
			$html .= "<a href='".$node_path."'>Accéder à l'article</a>";
			$html .= "</div>";
			break; 
	}

	print $html; 
//ici unset variables
unset($result);
unset($photo);
unset($embed_photo);  	
?>
	