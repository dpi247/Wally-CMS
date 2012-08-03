<?php 
// Le package -> $node 

//Give the number of view's results
$nb_results = count($view->result);

// Give the index of the row into the view.
$row_index=$variables["view"]->row_index;


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
$title = $mainstory->title;

/* Récupération du chapeau de l'article -> $strapline
 * Le nombre de caractères attendus pour ce chapeau est spécifié dans $strapline_length
 * Si aucune limitation n'est attendue, laisser la valeur de $strapline_length à 0
 * 
 * print($strapline);
 */
$strapline_length = 140;
$strapline = _wallydemo_get_strapline($mainstory,$node,$strapline_length);

$nb_comment = $node->comment_count;
if ($nb_comment == 0) $reagir = "réagir";
else if ($nb_comment == 1) $reagir = $nb_comment."&nbsp;réaction";
else $reagir = $nb_comment."&nbsp;réactions";
?>

<?php 
switch ($row_index){
	case 0: ?>
  <div class="item column">   
    <a href=<?php print check_url($node_path); ?>>
     <?php if($photo == TRUE){ 
      $photoObject_img = theme('imagecache', 'une_medium_127x85', $photoObject_path, $photoObject_summary, $photoObject_summary);
      print $photoObject_img;
     }
     	?>
    </a>
    <h2><a href="<?php print check_url($node_path); ?>"><?php print wallydemo_check_plain($title); ?></a></h2>
    <p><?php print wallydemo_check_plain($strapline); ?></p>
    <?php if($node->comment != 0){ ?>  
    <p class="comment">
      <a href="<?php print check_url($node_path); ?>#ancre_commentaires" title="Commentez cet article !"><?php print $reagir; ?></a>
    </p>
    <?php } ?>
  </div>
	<?php break;
	case 1: ?>
	  <div class="item column coince">   
    <a href="<?php print check_url($node_path); ?>">
     <?php if($photo == TRUE){ 
      $photoObject_img = theme('imagecache', 'une_medium_127x85', $photoObject_path, $photoObject_summary, $photoObject_summary);
      print $photoObject_img;
     }
      ?>
    </a>
    <h2><a href="/<?php print check_url($node_path); ?>"><?php print wallydemo_check_plain($title); ?></a></h2>
    <p><?php print wallydemo_check_plain($strapline); ?></p>
    <?php if($node->comment != 0){ ?>
    <p class="comment">
      <a href="/<?php print check_url($node_path); ?>#ancre_commentaires" title="Commentez cet article !"><?php print $reagir; ?></a>
    </p>
    <?php } ?>
  </div>	
	<?php break;
	case 2: ?>
    <div class="item column dernier">   
    <a href="<?php print check_url($node_path); ?>">
     <?php if($photo == TRUE){ 
      $photoObject_img = theme('imagecache', 'une_medium_127x85', $photoObject_path, $photoObject_summary, $photoObject_summary);
      print $photoObject_img;
     }
      ?>
    </a>
    <h2><a href="<?php print check_url($node_path); ?>"><?php print wallydemo_check_plain($title); ?></a></h2>
    <p><?php print wallydemo_check_plain($strapline); ?></p>
    <?php if($node->comment != 0){ ?>
    <p class="comment">
      <a href="<?php print check_url($node_path); ?>#ancre_commentaires" title="Commentez cet article !"><?php print $reagir; ?></a>
    </p>
    <?php } ?>
  </div>  		
	<?php break;
  case 3: ?>
    <ul>
		<li><a href="<?php print check_url($node_path); ?>"><?php print wallydemo_check_plain($title); ?></a></li>
	<?php break;    
	case 4: ?>
    <li><a href="<?php print check_url($node_path); ?>"><?php print wallydemo_check_plain($title); ?></a></li>
  <?php break;
	case 5: ?>
    <li><a href="<?php print check_url($node_path); ?>"><?php print wallydemo_check_plain($title); ?></a></li>
    </ul>
  <?php break;
	
}
//S'assurer qu'en cas de moins de 6 articles dans la vue, le <ul> ouvert précédemment soit bien fermé
if($row_index == $nb_results && $nb_results > 2 && $nb_results < 5){
	print "</ul>";
} 
?>

<?php 
//ici unset des variables
unset($node);
unset($mainstory);
unset($photoObject);
?>
