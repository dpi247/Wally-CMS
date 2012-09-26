<?php 

//dsm($variables["view"]); 

drupal_add_css(drupal_get_path('theme', 'wallydemo').'/css/hda.css','file','screen');

// Le package -> $node 

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
  $strapline_length = 0;
  $strapline = _wallydemo_get_strapline($mainstory,$node,$strapline_length);  
} else {  
  $mainstory = $node->field_mainobject_nodes[0];
  $mainstory_type = $mainstory->type;
  $strapline_length = 200;
  $strapline = wallydemo_get_textTeaser($embedtext_html,$strapline_length);    
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

/* Récupération du chapeau de l'article -> $strapline
 * Le nombre de caractères attendus pour ce chapeau est spécifié dans $strapline_length
 * Si aucune limitation n'est attendue, laisser la valeur de $strapline_length à 0
 * 
 * print($strapline);
 */

//$strapline_length = 0;
//$strapline = _wallydemo_get_strapline($mainstory,$node,$strapline_length);

$mainstory->type == "wally_textobject";

$chapeau = "";
if(isset($strapline)){
$chapeau = "<p>" .$strapline ."</p>";
}

?>


<div class="hero">
  <?php if($photo == TRUE){ ?>
  <a href="<?php print $node_url; ?>">
     <?php $photoObject_img = theme('imagecache', 'divers_95x140', $photoObject_path, $photoObject_summary, $photoObject_summary);
     print($photoObject_img); ?>
  </a>
  <h4><a href="<?php print $node_url; ?>">
    <?php print wallydemo_check_plain($title); ?>
  </a></h4>
  <?php print $chapeau; ?>
  <span class="vote"><a href="/homme_de_l_annee_liege/participation/index.php?page=vote">Voter</a></span>
  <?php } 
  unset($node);
  unset($mainstory);
  unset($photoObject);
  ?>
</div>
