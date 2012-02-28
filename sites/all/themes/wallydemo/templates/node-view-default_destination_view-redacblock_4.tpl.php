<?php 

// Give the index of the row into the view.
$row_index=$node->global_index;

//dsm($row_index,"ROW INDEX NEW");

//print "<h1>$row_index</h1>";

// Le package -> $node 

/* Récupération de l'id du package -> $node_id
 * 
 */
$node_id = $node->nid;

/* Récupération du path
 * 
 */
$theme_path = drupal_get_path('theme', 'wallydemo');

/* Récupération de l'alias de l'url du package -> $node_path
 * 
 * print($node_path);
 */
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

/* Récupération du titre de l'object principal donc du package à l'affichage -> $title
 * 
 * print($title);
 */  
$title = $mainstory->title;

/* Récupération du nombre de réactions sur la package
 * 
 */
$nb_comment = $node->comment_count;
if ($nb_comment == 0) $reagir = "<img src=\"".$theme_path."/images/ico_reagir_edition.png\" width=\"15\" height=\"8\" />";
else $reagir = $nb_comment;

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
 
/* Récupération du chapeau de l'article -> $strapline
 * Le nombre de caractères attendus pour ce chapeau est spécifié dans $strapline_length
 * Si aucune limitation n'est attendue, laisser la valeur de $strapline_length à 0
 * 
 * print($strapline);
 */
$strapline_length = 0;
$strapline = _wallydemo_get_strapline($mainstory,$node,$strapline_length);

/* Nombres d'articles et d'objets liés */

/*$linkedobjects = count($node->field_linkedobjects_nodes[0]->field_links_list);
if($linkedobjects > 0 ){
		$countlinkedobjects = "<li class=\"media-press\"><a href=\"/".$node_path."\">".$linkedobjects."</a></li>";
}    */

$countarticle=0;
$countdossier=0;
foreach($node->field_linkedobjects_nodes as $f){
	//dsm($f->field_links_list_nodes);
	foreach($f->field_links_list_nodes as $g) {
		//dsm($g);
		$value = $g->field_internal_link_nodes[0]->field_packagelayout[0]['value'];
		//dsm($value);
		$valuearticle = taxonomy_get_term($value);
		if($valuearticle->name=='Article Wiki'){
			$countdossier++;
		} 
		else {
			$countarticle++;
		}		
	}

}
//dsm('ZZZZ = '.$countarticle.' - '.$countdossier);
if($countarticle > 0) $countlinkedobjects = "<li class=\"media-press\"><a href=\"/".$node_path."\">".$countarticle."</a></li>";
else $countarticle = "";
if($countdossier > 0)$countlinkedossier = "<li class=\"media-dossier\"><a href=\"/".$node_path."\">".$countdossier."</a></li>";
else $countdossier = "";

$embeds = wallydemo_bracket_embeddedObjects_from_package($node);

$videosobjects = count($embeds['videos']);
if($videosobjects > 0 ){
		$countvideosobjects = "<li class=\"media-video\"><a href=\"/".$node_path."\">".$videosobjects."</a></li>";
}    
$photosobjects = count($embeds['photos']);
if($photosobjects > 1 ){
		$countphotosobjects = "<li class=\"media-photo\"><a href=\"/".$node_path."\">".$photosobjects."</a></li>";
}    
$audiosobjects = count($embeds['audios']);
if($audiosobjects > 0 ){
		$countaudiosobjects = "<li class=\"media-audio\"><a href=\"/".$node_path."\">".$audiosobjects."</a></li>";
}
foreach($embeds['digital'] as $digital){
	if(isset($digital->field_object3rdparty[0]['provider'])){
		$type = $digital->field_object3rdparty[0]['provider'];
		if($type == 'coveritlive'){
			$digitalobjects++;
		}
	}
}
if($digitalobjects > 0 ){
    $countdigitalobjects = "<li class=\"media-live\"><a href=\"/".$node_path."\"></a></li>";
}

?>
<div class="article2 md2 clearfix">

  <a href="/<?php print check_url($node_path); ?>">
  <?php if($photo == TRUE){ 
  $photoObject_img = theme('imagecache', 'unebis_small_90x66', $photoObject_filename, $photoObject_summary, $photoObject_summary);
  			} 
		else { 
  $photoObject_img = "<img src=\"".$theme_path."/images/default_pic.png\" width=\"90\" height=\"66\" />";
  			} 
  print $photoObject_img; 
		?>
  </a>


  <h2><a href="/<?php print($node_path) ?>"><?php print wallydemo_check_plain($title); ?></a></h2>
        <ul>
          <?php print($countdigitalobjects) ?>
          <?php print($countlinkedossier) ?>
          <?php print($countlinkedobjects) ?>
          <?php print($countvideosobjects) ?>
          <?php print($countphotosobjects) ?>
          <?php print($countaudiosobjects) ?>
          <li class="time"><?php print $date_edition; ?></li>
          <li class="comment"><a title="Commentez cet article !" href="/<?php print $node_path; ?>#ancre_commentaires"><?php print($reagir) ?></a></li>
        </ul>
</div>
<?php 
//ici unset des variables
unset($node);
unset($mainstory);
unset($photoObject);
unset($embeds);
unset($embeds_audios);
unset($embeds_videos);
unset($embeds_photos);
unset($embeds_digital);
?>
