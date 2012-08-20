<?php 
/*dsm($subtype,'subtype');
dsm($context,'context');*/
dsm($feed,'feed');
dsm($options,'options');
	/* Récupération du path vers notre theme -> $theme_path
	 */
	$theme_path = drupal_get_path('theme','wallydemo');
	

	$bloc_title = $options['title'];
	
 $content = "";
 $cpt = 1;
 $feed_length = count($feed);
  foreach ($feed as $k=>$item) {
	  $node =$item['node'];
  	$node_path = drupal_get_path_alias("node/".$node->nid);

	  	/* Récupération du mainstory et de la photo principale du package.
	 * Le package peut être articlePackage ou galleryPackage
	 * 
	 * Objet principal du package -> $mainstory
	 * 
	 * Photo principale du package -> $photoObject
	 * Path vers cette image -> $photoObject_path
	 * Taille de la photo sur le serveur -> $photoObject_size
	 * 
	 * S'il y a bien une image à afficher -> $photo==TRUE
	 * 
	 * Pour l'affichage de la photo via son preset imagecache :
	 * 
	 * $photoObject_img = theme('imagecache', '???presetImagecache???', $explfilepath[sizeof($explfilepath)-1], $explfilepath[sizeof($explfilepath)-1], $explfilepath[sizeof($explfilepath)-1], array('class'=>'postimage2')); ?>
	 * print($photoObject_img);
	 */
	$photo = FALSE;
	$photoObject_path = "";
	if($node->type == "wally_articlepackage"){
	  $mainstory = $node->field_mainstory_nodes[0];
	} else {  
	  $mainstory = $node->field_mainobject_nodes[0];
	  $mainstory_type = $mainstory->type;
	  if($mainstory_type == "wally_photoobject"){ 
	    $photoObject_path = $mainstory->field_photofile[0]['filepath'];
	    $photoObject_summary = $mainstory->field_summary[0]['value'];
	    $photoObject_filename = $mainstory->field_photofile[0]["filename"];
	    $explfilepath = explode('/', $photoObject_path);
	    $photoObject_size == $mainstory->field_photofile[0]['filesize'];
	    if (isset($photoObject_path) && $photoObject_size > 0) {
	      $photo = TRUE;
	    }
	  }
	}
	if ($photoObject_path == ""){
	  $embeded_objects = $node->field_embededobjects_nodes;
	  $photoObject = wallydemo_get_first_photoEmbededObject_from_package($embeded_objects);
	  If ($photoObject) {
	    $photoObject_path = $photoObject->field_photofile[0]['filepath'];
	    $photoObject_summary = $photoObject->field_summary[0]['value'];
	    $photoObject_filename = $photoObject->field_photofile[0]["filename"];
	    $explfilepath = explode('/', $photoObject_path);
	    $photoObject_size = $photoObject->field_photofile[0]['filesize'];
	    if (isset($photoObject_path) && $photoObject_size > 0) {
	      $photo = TRUE;
	    }    
	  }
	}
		  if($photo == TRUE){ 
	$photoObject_img = theme('imagecache', 'une_medium_127x85', $photoObject_filename, $photoObject_summary, $photoObject_summary);
			} 
		else { 
	$photoObject_img = "<img src=\"".$theme_path."/images/default_pic.png\" width=\"127\" height=\"85\" />";
			} 

	  $flow_id = $item['__flow_id'];
	  $feed_name = $options['flows'][$flow_id]->name;
     $tid = $options['flows'][$flow_id]->conf['destination']['tid'];
      $destination_path = drupal_get_path_alias('taxonomy/term/'.$tid);

	  $title = $mainstory->title;
 		if($cpt == 0){
 			$content .= "<li class=\"first\">";
		} else if ($cpt == $feed_length){
 			$content .= "<li class=\"last\">";
		} else {	
			$content .= "<li>";
		}
		$content .="<a href='".$node_path."'>".$photoObject_img."</a>";	
      $content .="-<a href='".$destination_path."'>".$feed_name."</a> : <a href='".$node_path."'>".wallydemo_check_plain($title)."</a>
             </li>";

$cpt++;
}
 
?>

<div>
  <h2><a target="_blank" href="<?php print $target; ?>"><?php print $bloc_title; ?></a></h2>
  <ul class="clearfix"><?php print $content; ?>
  </ul>
</div>
