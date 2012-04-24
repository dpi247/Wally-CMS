<?php
$html = "";

//dsm($options,"option");

 $flow_name = $options['flows'][0]->name;
 $tid = $options['flows'][0]->conf['destination']['tid'];
   
 $destination_name = $flow_name;
 $destination_trimmed_name = strtolower($destination_name);
 $destination_trimmed_name = str_replace(" ","",$destination_trimmed_name);
 $destination_trimmed_name = str_replace("é","e",$destination_trimmed_name);
 $destination_trimmed_name = str_replace("è","e",$destination_trimmed_name);
 $destination_trimmed_name = str_replace("ê","e",$destination_trimmed_name);
 $destination_trimmed_name = str_replace("à","a",$destination_trimmed_name);
 
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
  $node_path = wallydemo_get_node_uri($node);
  
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
   
  $html .= "<div class=\"item clearfix\">";
  if($photo == TRUE){
  $html .= "<a href=\"".$node_path."\" target=\"_blank\">";
    $photoObject_img = theme('imagecache', 'une_medium_127x85', $photoObject_filename, $photoObject_summary, $photoObject_summary);
    $html .= $photoObject_img;
  $html .= "</a>";  
  }
  $html .= "<span class=\"localite\">".$date_edition."</span>";
  $html .= "<h2><a href=\"".$node_path."\" target=\"_blank\">".$node_title."</a></h2>";
//  $html .= "<p>".$node_strapline."</p>";
  $html .= "</div>";
  $html .= "<div class=\"separator\">&nbsp;</div>";
  unset($item);
}
unset($feed);

?>
<div class="edition info-region <?php print $destination_trimmed_name ; ?> clearfix">
  <h1><a href="<?php print drupal_get_path_alias($tid.'/r&eacute;gions/'.$destination_trimmed_name); ?>" target="_blank"><?php print $destination_name; ?></a></h1>
  <div class="separator2">&nbsp;</div>
  <?php print $html; ?>
<p class="info_region"><a href="<?php print drupal_get_path_alias($tid.'/r&eacute;gions/'.$destination_trimmed_name); ?>" class="novisited arrow" target="_blank">Toutes les infos régionales</a></p>
</div>
<div class="separator">&nbsp;</div>

