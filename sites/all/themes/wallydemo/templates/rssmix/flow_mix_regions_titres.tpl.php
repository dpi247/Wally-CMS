<?php
//dsm($feed, 'FEED BLOC REGIONS TITRES');
//dsm($options,'OPTIONS BLOC REGIONS TITRES');
$html = "";
$cpt = 0;
foreach ($feed as $item){
	
	$node = $item['node'];
	
	/* Récupération de l'id du package -> $node_id
	 * 
	 */
	$node_id = $node->nid;
	
	/* Récupération de l'alias de l'url du package -> $node_path
	 * 
	 * print($node_path);
	 */
	$node_path = drupal_get_path_alias("node/".$node->nid);

	/* Récupération du path vers notre theme -> $theme_path
	 */
	$theme_path = drupal_get_path('theme','wallydemo');
	
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
	
	/* Récupération du titre de l'object principal donc du package à l'affichage -> $title
	 * 
	 * print($title);
	 */  
	$node_title = $mainstory->title;
	
	/* Récupération chapeau et déclaration de son nombre de caractères
	 * 
	 */
	$strapline_length = 65;
  $node_strapline = _wallydemo_get_strapline($mainstory,$node,$strapline_length);
	
	/*  Récupération de la date de publication du package -> $node_publi_date
	 */
	$node_publi_date = strtotime($node->field_publicationdate[0]['value']);
	
	/* Affichage de la date au format souhaité
	 * Les formats sont:
	 * 
	 * 'filinfo' -> '00h00'
	 * 'unebis' -> 'jeudi 26 mai 2011, 15:54'
	 * 'default' -> 'publié le 26/05 à 15h22'
	 * 
	 * print($date_edition);
	 */ 
	 
   $date_edition = _wallydemo_date_edition_diplay($node_publi_date, 'date_courte');
   
   // Gestion spécifique destinations   
   
   $flow_name = $options['flows'][$cpt]->name;
   $tid = $options['flows'][$cpt]->conf['destination']['tid'];
   
   $destination_name = $flow_name;
   $destination_trimmed_name = strtolower($destination_name);
   $destination_trimmed_name = str_replace(" ","",$destination_trimmed_name);
   $destination_trimmed_name = str_replace(" ","",$destination_trimmed_name);
   $destination_trimmed_name = str_replace("é","e",$destination_trimmed_name);
   $destination_trimmed_name = str_replace("è","e",$destination_trimmed_name);
   $destination_trimmed_name = str_replace("ê","e",$destination_trimmed_name);
   $destination_trimmed_name = str_replace("à","a",$destination_trimmed_name);   
   
   
   switch($tid){
   	case 1:
   		$destination_path = "http://www.lameuse.be";
   	  break;
   	case 2:
      $destination_path = "http://www.lanouvellegazette.be";
   		break;
   	case 3:
   		$destination_path = "http://www.sudinfo.be";
   		break;
   	case 4:
      $destination_path = "http://www.noreclair.be";
      break;
   	case 5:
   		$destination_path = "htt://www.laprovince.be";
   		break;
   	case 6:
   		$destination_path = "http://www.lacapitale.be";
   		break;
   	default:
   		$destination = taxonomy_get_term($tid);
      $destination_path = drupal_get_path_alias('taxonomy/term/'.$tid);
   }
   
	$html .= "<div class=\"item ".$destination_trimmed_name." clearfix\">";
  $html .= "<h1><a href=\"".$destination_path."\" title=\"Aller vers ".$destination_name."\" target=\"_blank\">".$destination_name."</a></h1>";  
  $html .= "<a href=\"".$node_path."\" target=\"_blank\">";
  if($photo == TRUE){ 
	$photoObject_img = theme('imagecache', 'une_medium_127x85', $photoObject_filename, $photoObject_summary, $photoObject_summary);
			} 
		else { 
	$photoObject_img = "<img src=\"".$theme_path."/images/default_pic.png\" width=\"127\" height=\"85\" />";
			} 
  $html .= $photoObject_img;
  $html .= "</a>";  
  $html .= "<span class=\"time\">".$date_edition."</span>";
  $html .= "<h2><a href=\"".$node_path."\" target=\"_blank\">".$node_title."</a></h2>";
//  $html .= "<p>".$node_strapline."</p>";
  $html .= "</div>";
  $html.= "<div class=\"separator\">&nbsp;</div>";
	
  $cpt++;
  
  unset($item);
}
unset($feed);

?>
<div class="info-region clearfix">
  <?php print $html; ?>
</div>
<p class="info_region"><a href="<?php print drupal_get_path_alias('taxonomy/term/8'); ?>" class="arrow novisited" target="_blank">Toutes les infos régionales</a></p>
<div class="separator">&nbsp;</div>

