<?php 

// Give the index of the row into the view.
$row_index=$variables["view"]->row_index;

//dsm($variables["view"]); 

//print "<h1>$row_index</h1>";

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

?>

<?php switch($row_index){
	case 0: 
     if($photo == TRUE){
		 $photoObject_img = theme('imagecache', 'une_small_78x52', $photoObject_filename, $photoObject_summary, $photoObject_summary);
     print($photoObject_img);
     } ?>
          <a href="/<?php print check_url($node_path); ?>">  
            <h3><?php print wallydemo_check_plain($title); ?></h3>
          </a>
	<?php break;
	case 1 : ?>
	         <ul>
            <li>
              <a href="/<?php print check_url($node_path); ?>"><?php print wallydemo_check_plain($title); ?></a>
            </li>		
  <?php break;
	case 3: ?>
            <li>
              <a href="/<?php print check_url($node_path); ?>"><?php print wallydemo_check_plain($title); ?></a>
            </li>
           </ul> 		
	<?php break;
	default:	?>
		        <li>
		          <a href="/<?php print check_url($node_path); ?>"><?php print wallydemo_check_plain($title); ?></a>
		        </li>
<?php } 
?>

<?php 
//ici unset des variables
unset($node);
unset($mainstory);
unset($photoObject);
?>
