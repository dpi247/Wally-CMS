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

// Récupération du tid

$tid  = $variables["view"]->args[0];
$dest = taxonomy_get_term($tid);
$destName = $dest->name; 

// Récupération du theme_path

$theme_path = drupal_get_path('theme', 'wallydemo');


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

// détermine la taille du bloc.
$last = count($node->view->result)-1;
$current = $node->view->row_index;
	if ($current==$last) { 
		$classlast = " class=\"last\""; } 
	else { $classlast = ""; 
	}
/* N'affiche la photo partout sauf dans la Une bis Filinfo
   Dans les autres Une bis, affiche l'image par défaut quand il n'y a pas de photo */

if ($tid != 20) { 
	if($photo == TRUE){ 
	$photoObject_img = theme('imagecache', 'une_small_78x52', $photoObject_filename, $photoObject_summary, $photoObject_summary);}
	else { $photoObject_img = "<img src=\"".$theme_path."/images/default_pic_78x52.png\">"; }}
  
/*  Récupération de la date de publication du package -> $node_publi_date
 */
$node_publi_date = strtotime($node->field_publicationdate[0]['value']);

/* Affichage de la date au format souhaité
 * Les formats sont:
 * 
 *  'filinfo' -> '00:00'
 *  'unebis' -> 'lundi 01 janvier 2011, 00h00'
 *  'date_courte' -> '30/05 - 13h24'
 *  'default' -> 'publié le 01/01 à 00h00'
 * 
 * print($date_edition);
 */ 
 
if ($tid == 20) { 
$date_edition = "<span class=\"time\">" ._wallydemo_date_edition_diplay($node_publi_date, 'date_courte') ."</span>&nbsp;";
}

?>
 <li<?php print $classlast ; ?>>
 	<?php print $photoObject_img; ?><?php print $date_edition; ?>
	 <a href="<?php print check_url($node_path); ?>"><?php print wallydemo_check_plain($title); ?></a>
 </li>  
 
 <?php 
//ici unset des variables
unset($node);
unset($mainstory);
unset($photoObject);
?>