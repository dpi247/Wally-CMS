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
$node_path = wallydemo_get_node_uri($node);

$nodes = array();
foreach ( $vars['field_embededobjects_nodes'] as $node)
{
	node_build_content($node);
  drupal_render($node->content);
	$nodes[] = $node; 
}
$vars['field_embededobjects_nodes'] = $nodes;
unset($nodes); 


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


/* Récupération du titre de la liste de lien  */  

$links = _wallydemo_get_sorted_links($node); 

$links_html = "";

foreach($links as $linksList){
	if(isset($linksList["title"])){
		$list_titre = $linksList["title"];
		$links_html .= "<div class=\"bloc-01\"><h2>Liens utiles</h2><ul>";
		foreach($linksList["links"] as $link){
			$link_url = $link["url"];
			$link_title = $link["title"];
			$link_target = $link["target"];
			$links_html .= "<li class=\"arrow\">" ."<a href=\"".$link_url."\" target=\"".$link_target."\">".$link_title."</a></li>";
		}
		$links_html .= "</ul></div>";
	}
}

/* Récupération du titre de l'object principal donc du package à l'affichage -> $title
 * 
 * print($title);
 */  

//drupal_add_css(drupal_get_path('theme', 'custom_sp').'/css/article.css','file','screen');
 
$title = $mainstory->title;
$chapeau = $mainstory->field_textchapo[0]['value'];
$texte = $mainstory->field_textbody[0]['value'];
$date_changed = "- Mis à jour le&nbsp;" ._wallydemo_date_edition_diplay($changed, 'date_jour_heure') ."&nbsp;-";

?>
	<?php $photoObject_img = theme('imagecache', 'info_fiche_150x150', $photoObject_filename, $photoObject_summary, $photoObject_summary);
    print $photoObject_img; ?>
    <?php print $links_html; ?>
    <h1><?php print wallydemo_check_plain($title) ?></h1>
    <div class="changed"><?php print $date_changed; ?></div>
    <div class="chapeau"><?php print $chapeau; ?></div>
    <div class="texte_fiche"><?php print $texte; ?></div>

<?php 
//ici unset des variables
unset($node);
unset($mainstory);
unset($photoObject);
unset($links);
?>
