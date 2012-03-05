<?php
// $Id: node.tpl.php,v 1.4.2.1 2009/08/10 10:48:33 goba Exp $
/**
 * @file node.tpl.php
 *
 * Theme implementation to display a node.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: Node body or teaser depending on $teaser flag.
 * - $picture: The authors picture of the node output from theme_user_picture().
 * - $date: Formatted creation date (use $created to reformat with format_date()).
 * - $links: Themed links like "Read more", "Add new comment", etc. output from theme_links().
 * - $name: Themed username of node author output from theme_username().
 * - $node_url: Direct url of the current node.
 * - $terms: the themed list of taxonomy term links output from theme_links().
 * - $submitted: themed submission information output from theme_node_submitted().
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type, i.e. story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Node status variables:
 * - $teaser: Flag for the teaser state.
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 */

	//dsm($node);
	//dsm($embeds);
	//exit();
	//dsm($embeds_photos);
	//dsm($embeds_digital);
	//dsm($embeds_videos);	
	//dsm($photoObject);
	//dsm($links);

$nodes = array();
foreach ( $node->field_embededobjects_nodes as $n)
{
  node_build_content($n);
  drupal_render($n->content);
  $nodes[] = $n; 
}
$node->field_embededobjects_nodes = $nodes;
unset($nodes); 

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


/*  Récupération de la date de publication du package -> $node_publi_date
 */
$node_publi_date = strtotime($node->field_publicationdate[0]['value']);
 
/* Récupération du chapeau de l'article -> $strapline
 * Le nombre de caractères attendus pour ce chapeau est spécifié dans $strapline_length
 * Si aucune limitation n'est attendue, laisser la valeur de $strapline_length à 0
 * 
 * print($strapline);
 */
 

//wallytoolbox_add_meta(array("property"=>"og:type"), "Article");
//wallytoolbox_add_meta(array("property"=>"og:url"), $node_path);

$embeds = wallydemo_bracket_embeddedObjects_from_package($node);

//exit();
if(is_array($embeds)){
	$embeds_photos = array();
	$embeds_videos = array();
	$embeds_audios = array();
	$embeds_digital = array();
	
/*	foreach($embeds["photos"] as $photo){
		$embed_photo = wallydemo_get_photo_wiki_liens($photo);
		array_push($embeds_photos,$embed_photo);
	}*/
	foreach($embeds["videos"] as $video){
	  $embed_video = wallydemo_get_video_infos_and_display($video);
	  array_push($embeds_videos,$embed_video);
	}
	foreach($embeds["audios"] as $audio){
	  $embed_audio = wallydemo_get_audio_infos_and_display($audio);
	  array_push($embeds_audios,$embed_audio);
	}
	foreach($embeds["digital"] as $digital){
	  $embed_digital = wallydemo_get_digitalobject_infos_and_display($digital);
	  array_push($embeds_digital,$embed_digital);
	}	
	
	$mainObject_html = "";
	
	if(count($embeds_digital) > 0){
		foreach($embeds_digital as $embed){
		  $mainObject_html .= "<div>".$embed['emcode']."</div>";
		}
	}	
	if(count($embeds_videos) > 0){
		foreach($embeds_videos as $embed){
			  if(stripos($embed["emcode"], 'www.youtube.com') !== FALSE) {
          $temp = 'height="350" width="425"';
          $embed["emcode"] = str_replace($temp, "height='250' width='300'", $embed["emcode"]);
        } else {
         $embed["emcode"] = preg_replace('+width=("|\')[0-9]{3}("|\')+','width="300"',$embed["emcode"]);
         $embed["emcode"] = preg_replace('+height=("|\')[0-9]{3}("|\')+','',$embed["emcode"]);
        }
		  $mainObject_html .= "<div class=\"toto\">".$embed['emcode']."</div>";
		}
	}

	if(count($embeds_photos) > 0){
		foreach($embeds_photos as $embed){
			$mainObject_html .= "<p>".$embed["main_size"]."</p>";
		}
	}
}	
?>
<div id="picture"><?php print $mainObject_html; ?></div>

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
