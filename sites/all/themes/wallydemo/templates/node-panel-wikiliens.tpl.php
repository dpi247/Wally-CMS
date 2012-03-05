<?php
	

//drupal_add_css(drupal_get_path('theme', 'wallydemo').'/css/article.css','file','screen');
//drupal_add_js(drupal_get_path('theme', 'wallydemo').'/scripts/cycle.js');

// Give the index of the row into the view.

$themeroot = drupal_get_path('theme', 'wallydemo');

// Get the string name of the current domain
$domain_url = $_SERVER["SERVER_NAME"];
$domain = spdatastructure_getdomain($domain_url);

// Get the node's main destination
$mainDestination = $node->field_destinations[0]["tid"];

// Le package -> $node 

/* Récupération de l'id du package -> $node_id
 * 
 */
$node_id = $node->nid;

/* Récupération de l'alias de l'url du package -> $node_path
 * 
 * print($node_path);
 */
$node_path = "http://".$_SERVER['SERVER_NAME'].'/'.drupal_get_path_alias("node/".$node->nid);

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
    $explfilepath = explode('/', $photoObject_path);
    $photoObject_size = $photoObject->field_photofile[0]['filesize'];
    if (isset($photoObject_path) && $photoObject_size > 0) {
      $photo = TRUE;
    }    
  }
}

// suppression de la photo principale

function wallydemo_get_photo_wiki_liens($photoObject,$template="default"){
  $photo = array();
  $photo["nid"] = $photoObject->nid;
  $photo["title"] = $photoObject->title;
  $photo["type"] = $photoObject->type;
  $photo['credit'] = $photoObject->field_copyright[0]['value'];
  $photo['summary'] = $photoObject->field_summary[0]['value'];
  $photo['fullpath'] = $photoObject->field_photofile[0]['filepath'];
  $photo['size'] = $photoObject->field_photofile[0]['filesize'];

	switch ($template){
		case "default":
      $photo['main_size'] = "";
      $photo['main_url'] = "";
      $photo['mini'] = "";			
		  if($photo['size'] > 0){
		    
		    $photo['mini'] = theme('imagecache', 'article_48x32', $photo['filename'],$photo['summary'],$photo['summary']);
		  }
	  break;	
	}
	return $photo;
} 
 
$links = _wallydemo_get_sorted_links($node); 

//dsm($links,'LINKS');

$links_html = "";

foreach($links as $linksList){
	if(isset($linksList["title"])){
		$list_titre = $linksList["title"];
		$links_html .= "<div class=\"liens_wiki\"><h2>".$list_titre."</h2><ul>";
		foreach($linksList["links"] as $link){
			$link_url = $link["url"];
			$link_title = $link["title"];
			$link_target = $link["target"];
			$link_type = $link["type"];
			$links_html .= "<li class=\"".$link_type ."\">" ."<a href=\"".$link_url."\" target=\"".$link_target."\">".$link_title."</a></li>";
		}
		$links_html .= "</ul></div>";
	}
}



//wallytoolbox_add_meta(array("property"=>"og:type"), "Article");
//wallytoolbox_add_meta(array("property"=>"og:url"), $node_path);

/*
 * Génération des liens de partage
 */
$socialSharingBaseUrl = wallydemo_get_social_sharing_base_url($mainDestination,$domain);
$socialSharingDomainAndPathUrl = $socialSharingBaseUrl."/".$node_path;
$fixedDomainAndPathUrl = "http://www.sudpresse.be/$node_path";

?>

<div id="wiki_listes"><?php print $links_html; ?></div>
<div id="commentaires">
  <!-- FACEBOOK REACTIONS -->
  <?php if($node->fbcomment == 2) { ?>
  <a id="ancre_commentaires" name="ancre_commentaires" href="#ancre_commentaires" /></a>
  <?php print theme("spreactions_facebook", $node_id, $socialSharingBaseUrl, $socialSharingDomainAndPathUrl, $socialSharingDomainAndPathUrl); ?>
  <?php } ?>
</div>

<?php 
//ici unset des variables
unset($node);
unset($mainstory);
unset($photoObject);
unset($links);
?>
