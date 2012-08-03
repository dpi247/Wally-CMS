<?php 
//dsm($variables,'variables');

// Give the index of the row into the view.
$row_view_index=$variables["view"]->row_index;
$view_offset = $variables["view"]->pager["offset"];
$row_index=$row_view_index+$view_offset;


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
if ($node->type == "wally_articlepackage"){
  $mainstory = $node->field_mainstory_nodes[0];
} elseif ($node->type == "wally_gallerypackage") {  
  $mainstory = $node->field_mainobject_nodes[0];
  $mainstory_type = $mainstory->type;
  if ($mainstory_type == "wally_photoobject"){ 
    $photoObject_path = $mainstory->field_photofile[0]['filepath'];
    $photoObject_summary = $mainstory->field_summary[0]['value'];
    $photoObject_filename = $mainstory->field_photofile[0]["filename"];
    $photoObject_filepath = $mainstory->field_photofile[0]['filepath'];
    $explfilepath = explode('/', $photoObject_path);
    $photoObject_size = $mainstory->field_photofile[0]['filesize'];
    if ($photoObject_path != "" && $photoObject_size > 0) {
      $photo = TRUE;
    }
  }
} elseif ($node->type == 'wally_pollpackage'){
  $mainstory = $node->field_mainpoll_nodes[0];
}
if ($photoObject_path == ""){
  $embeded_objects = $node->field_embededobjects_nodes;
  $photoObject = wallydemo_get_first_photoEmbededObject_from_package($embeded_objects);
  if ($photoObject) {
    $photoObject_path = $photoObject->field_photofile[0]['filepath'];
    $photoObject_summary = $photoObject->field_summary[0]['value'];
    $photoObject_filename = $photoObject->field_photofile[0]["filename"];
    $photoObject_filepath = $photoObject->field_photofile[0]["filepath"];
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
else if ($nb_comment == 1) $reagir = $nb_comment."&nbsp;réaction";
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
 
$date_edition = _wallydemo_date_edition_diplay($node_publi_date, 'unebis');
 
/* Récupération du chapeau de l'article -> $strapline
 * Le nombre de caractères attendus pour ce chapeau est spécifié dans $strapline_length
 * Si aucune limitation n'est attendue, laisser la valeur de $strapline_length à 0
 * 
 * print($strapline);
 */
$strapline_length = 0;
$strapline = _wallydemo_get_strapline($mainstory,$node,$strapline_length);

?>

<?php 
switch ($row_index) {
	case 0:
?>
<div class="article md clearfix">
  
  <h2><a href="/<?php print check_url($node_path); ?>"><?php print wallydemo_check_plain($title); ?></a></h2>
  <a href="/<?php print check_url($node_path); ?>">
  <?php if($photo == TRUE){ 
  $photoObject_img = theme('imagecache', 'une_manchette_217x145', $photoObject_filepath, $photoObject_summary, $photoObject_summary);
  			} 
		else { 
  $photoObject_img = "<img src=\"".$theme_path."/images/default_pic.png\" width=\"217\" height=\"145\" />";
  			} 
  print $photoObject_img; 
		?>
  </a>
  <p class="time time_unebis"><?php print $date_edition; ?></p>
  <p class="text"><?php print wallydemo_check_plain($strapline); ?>
  <?php if($node->comment != 0){ ?>
  <p class="comment"><a title="Commentez cet article !" href="/<?php print check_url($node_path); ?>#ancre_commentaires"><?php print($reagir) ?></a></p>
  <?php } ?>
</div>

<?php 
	break;
	case 1:
?>

<div class="article md clearfix">
  <h2><a href="/<?php print check_url($node_path); ?>"><?php print wallydemo_check_plain($title); ?></a></h2>
  <a href="/<?php print check_url($node_path); ?>">
  <?php if($photo == TRUE){ 
  $photoObject_img = theme('imagecache', 'unebis_medium_180x120', $photoObject_filepath, $photoObject_summary, $photoObject_summary);
  			} 
		else { 
  $photoObject_img = "<img src=\"".$theme_path."/images/default_pic.png\" width=\"180\" height=\"120\" />";
  			} 
  print $photoObject_img; 
		?>
  </a>
  <p class="time time_unebis"><?php print $date_edition; ?></p>
  <p class="text"><?php print wallydemo_check_plain($strapline); ?></p>
  <?php if($node->comment != 0){ ?>
  <p class="comment"><a title="Commentez cet article !" href="/<?php print check_url($node_path); ?>#ancre_commentaires"><?php print($reagir) ?></a></p>
  <?php } ?>
</div>

<?php 
  break;
  case 2:
  ?>
<div class="article lt clearfix">
  <h2><a href="/<?php print check_url($node_path); ?>"><?php print wallydemo_check_plain($title); ?></a></h2>

  <a href="/<?php print check_url($node_path); ?>">
  <?php if($photo == TRUE){ 
  $photoObject_img = theme('imagecache', 'unebis_small_90x66', $photoObject_filepath, $photoObject_summary, $photoObject_summary);
        } 
    else { 
  $photoObject_img = "<img src=\"".$theme_path."/images/default_pic.png\" width=\"90\" height=\"66\" />";
        } 
  print $photoObject_img; 
    ?>
  </a>
  <p class="time time_unebis"><?php print $date_edition; ?></p>
  <p class="text"><?php print wallydemo_check_plain($strapline); ?></p>
  <?php if($node->comment != 0){ ?>
  <p class="comment"><a title="Commentez cet article !" href="/<?php print check_url($node_path); ?>#ancre_commentaires"><?php print($reagir) ?></a></p>
  <?php } ?>
</div>

<?php
  /*
	$cache_url = $_SERVER['HTTP_HOST'].'/'.$_GET['q'];
	$delta = 'large_rectangle_middle'; // ta position (rectangle_top par exemple)
	print _pub_getpubcontent($delta, $cache_url); 
	*/
	break;
	default:
	?>
<div class="article lt clearfix">
  <h2><a href="/<?php print check_url($node_path); ?>"><?php print wallydemo_check_plain($title); ?></a></h2>

  <a href="/<?php print check_url($node_path); ?>">
  <?php if($photo == TRUE){ 
  $photoObject_img = theme('imagecache', 'unebis_small_90x66', $photoObject_filepath, $photoObject_summary, $photoObject_summary);
  			} 
		else { 
  $photoObject_img = "<img src=\"".$theme_path."/images/default_pic.png\" width=\"90\" height=\"66\" />";
  			} 
  print $photoObject_img; 
		?>
  </a>
  <p class="time time_unebis"><?php print $date_edition; ?></p>
  <p class="text"><?php print wallydemo_check_plain($strapline); ?></p>
  <?php if($node->comment != 0){ ?>
  <p class="comment"><a title="Commentez cet article !" href="/<?php print check_url($node_path); ?>#ancre_commentaires"><?php print($reagir) ?></a></p>
  <?php } ?>
</div>

<?php 
}
unset($node);
unset($mainstory);
unset($photoObject);
?>
