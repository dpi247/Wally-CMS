<?php 

$node = $result["wallynode"][0];  


// Le package -> $node 

/* Récupération de l'id du package -> $node_id
 * 
 */
$node_id = $node->nid;
//dsm($node);

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
if ($nb_comment == 0) $reagir = "réagir";
elseif ($nb_comment == 1) $nb_comment."&nbsp;réaction";
else $reagir = $nb_comment."&nbsp;réactions";


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
//$strapline = _wallydemo_get_strapline($mainstory,$node,$strapline_length);
$strapline = html_entity_decode($result["snippet"]);

/* Pour le dernier élément du breadcrumb : */
$breadcrumb = _wallydemo_breadcrumb_display($node->field_destinations[0]["tid"],"last");

//dsm($result);

?>
<div class="article">
  <h2><a href="/<?php print($node_path) ?>"><?php print($title) ?></a></h2>
  <?php if($photo == TRUE){ ?>
  <a href="/<?php print($node_path) ?>">
  <?php $photoObject_img = theme('imagecache', 'unebis_small_90x66', $photoObject_filename, $photoObject_summary, $photoObject_summary);
  print($photoObject_img); ?>
  </a>
  <?php } ?>
  <span class="novisited"><?php print($breadcrumb) ?></span> <span class="time time_unebis"> | <?php print($date_edition) ?> | </span> <?php print($strapline); ?>
  <p class="comment"><a title="Commentez cet article !" href="/<?php print $node_path; ?>#ancre_commentaires"><?php print($reagir) ?></a></p>
</div>

<?php 
//ici unset variables
unset($node);
unset($mainstory);
unset($photoObject);  
?>