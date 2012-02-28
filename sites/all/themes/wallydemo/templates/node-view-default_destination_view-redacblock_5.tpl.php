<?php 
// Le package -> $node 

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
 * 
 * print($theme_path);
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
$title = $mainstory->title;

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
 
$date_edition = _wallydemo_date_edition_diplay($node_publi_date, 'default');
/* Récupération du chapeau de l'article -> $strapline
 * Le nombre de caractères attendus pour ce chapeau est spécifié dans $strapline_length
 * Si aucune limitation n'est attendue, laisser la valeur de $strapline_length à 0
 * 
 * print($strapline);
 */
$strapline_length = 0;
$strapline = _wallydemo_get_strapline($mainstory,$node,$strapline_length);

/* Récupération du nombre de réactions sur la package
 * 
 */
$nb_comment = $node->comment_count;
if ($nb_comment == 0) $reagir = "réagir";
else if ($nb_comment == 1) $reagir = $nb_comment."&nbsp;réaction";
else $reagir = $nb_comment."&nbsp;réactions";

/* Récupération des liens associés au package
 * 
 */
$links = _wallydemo_get_sorted_links($node); 
$links_html = "";
if(is_array($links)){
  $links_html = "<ul>";
  foreach($links as $linksList){
    if(isset($linksList["links"])){
	    foreach($linksList["links"] as $link){
	      $link_url = $link["url"];
	      $link_title = $link["title"];
	      $link_target = $link["target"];
	      $link_type = $link["type"];
	      $links_html .= "<li class=\"".$link_type ."\">" ."<a href=\"".$link_url."\" target=\"".$link_target."\">".$link_title."</a></li>";
	    }
    }
  }
  $links_html .= "</ul>";
}
/*
 * Génération du breadcrumb
 */
$breadcrumb = _wallydemo_breadcrumb_display($node->field_destinations[0]["tid"],'une');

?>
<div class="super_event">
  <h2><a href="/<?php print check_url($node_path); ?>"><?php print wallydemo_check_plain($title); ?></a></h2>
  <div class="cadre_super_event">
    <?php if($links_html != ""){
        print $links_html;
        } 
    ?>
    <p class="comment right"><a title="Commentez cet article !" href="/<?php print check_url($node_path); ?>#ancre_commentaires"><?php print $reagir; ?></a></p>
    <p class="time_super_event"><?php print $date_edition; ?></p>
  </div>
  <?php if($photo == TRUE){ ?>
  <a class="photo_event" href="/<?php print check_url($node_path); ?>">
  <?php $photoObject_img = theme('imagecache', 'cible_top_960x320', $photoObject_filename, $photoObject_summary, $photoObject_summary);
  print $photoObject_img; ?>
  </a>
  <?php } 
  //ici unset des variables
  unset($node);
  unset($mainstory);
  unset($photoObject);
  unset($links);
  ?>
</div>
