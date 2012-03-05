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


/* Récupération du path
 * 
 */
$theme_path = drupal_get_path('theme', 'wallydemo');

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
$thumbnail = FALSE;
$photoObject_path = "";
if($node->type == "wally_articlepackage"){
  $mainstory = $node->field_mainstory_nodes[0];
} else {  
  $mainstory = $node->field_mainobject_nodes[0];
  $mainstory_type = $mainstory->type;
  if($mainstory_type == "wally_photoobject"){
  	if(isset($mainstory->field_thumbnail[0]) && is_array($mainstory->field_thumbnail)){
  		$photoObject_path = $mainstory->field_thumbnail[0]['filepath'];
  		$photoObject_size = $mainstory->field_thumbnail[0]['filesize'];
  		$thumbnail = TRUE;
  	}else{
      $photoObject_path = $mainstory->field_photofile[0]['filepath'];
      $photoObject_size = $mainstory->field_photofile[0]['filesize'];
  	}
    $explfilepath = explode('/', $photoObject_path);
    if (isset($photoObject_path) && $photoObject_size > 0) {
    	$photo = TRUE;
    }
  }
}
if ($photoObject_path == ""){
  $embeded_objects = $node->field_embededobjects_nodes;
  $photoObject = wallydemo_get_first_photoEmbededObject_from_package($embeded_objects);
  If ($photoObject) {
    if(isset($photoObject->field_thumbnail[0]) && is_array($photoObject->field_thumbnail)){
      $photoObject_path = $photoObject->field_thumbnail[0]['filepath'];
      $photoObject_size = $photoObject->field_thumbnail[0]['filesize'];
      $thumbnail = TRUE;
    }else{
      $photoObject_path = $photoObject->field_photofile[0]['filepath'];
      $photoObject_size = $photoObject->field_photofile[0]['filesize'];
    }
    $explfilepath = explode('/', $photoObject_path);
    if (isset($photoObject_path) && $photoObject_size > 0) {
      $photo = TRUE;
    }    
  }
}

// redéfinition de la taille de la photo principale

function wallydemo_get_photo_wiki($photoObject,$template="default",$thumbnail=FALSE){

  $photo = array();
  $photo["nid"] = $photoObject->nid;
  $photo["title"] = $photoObject->title;
  $photo["type"] = $photoObject->type;
  $photo['credit'] = $photoObject->field_copyright[0]['value'];
  $photo['summary'] = $photoObject->field_summary[0]['value'];
  if($thumbnail == TRUE){
  $photo['filename'] = $photoObject->field_thumbnail[0]['filename'];
  $photo['size'] = $photoObject->field_thumbnail[0]['filesize'];
  }else{
  $photo['filename'] = $photoObject->field_photofile[0]['filename'];
  $photo['size'] = $photoObject->field_photofile[0]['filesize'];
  }
  
	switch ($template){
		case "default":
      $photo['main_size'] = "";
      $photo['mini'] = "";			
		  if($photo['size'] > 0){
		    $photo['main_size'] = theme('imagecache', 'wiki_940x120', $photo['filename'],$photo['summary'],$photo['summary']); 
		    $photo['mini'] = theme('imagecache', 'article_48x32', $photo["filename"],$photo['summary'],$photo['summary']); 
		  }
	  break;	
	}
	return $photo;
} 

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
 
$date_edition = "<span>Dossier publié le&nbsp;</span>" ._wallydemo_date_edition_diplay($node_publi_date, 'date_jour');
$date_changed = "<span>Dernière mise à jour :&nbsp;</span>" ._wallydemo_date_edition_diplay($changed, 'unebis');
 
/* Récupération du chapeau de l'article -> $strapline
 * Le nombre de caractères attendus pour ce chapeau est spécifié dans $strapline_length
 * Si aucune limitation n'est attendue, laisser la valeur de $strapline_length à 0
 * 
 * print($strapline);
 */

$strapline_length = 0;
$strapline = _wallydemo_get_strapline($mainstory,$node,$strapline_length);

$main_title = $mainstory->title;
$chapeau = "<p class=\"chapeau\">" .$strapline ."</p>";
$texte_article = "<p class=\"texte\">" .$node->field_mainstory_nodes[0]->field_textbody[0]['value'] ."</p>";
$signature = "<p class=\"auteur\">" .$node->field_mainstory_nodes[0]->content['group_authoring']['group']['field_authors']['field']['items']['#children'] ."</p>";

$node_publi_date = strtotime($node->field_publicationdate[0]['value']);

drupal_add_css($themeroot . '/css/article.css');

//wallytoolbox_add_meta(array("property"=>"og:type"), "Article");
//wallytoolbox_add_meta(array("property"=>"og:url"), $node_path);


$nb_comment = $node->comment_count;

if ($nb_comment == 0){
	$reagir = "réagir";
}    
else {
	$reagir = $nb_comment."&nbsp;réactions";
}

$embeds = wallydemo_bracket_embeddedObjects_from_package($node);

if(is_array($embeds)){
	$embeds_photos = array();
	$embeds_videos = array();
	$embeds_audios = array();
	$embeds_digital = array();
	
	foreach($embeds["photos"] as $photo){
		$embed_photo = wallydemo_get_photo_wiki($photo,"default",$thumbnail);
		array_push($embeds_photos,$embed_photo);
	}
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
	switch($embeds["mainObject"]->type){
		/*case "wally_digitalobject":
			$mainObject_html .= $embeds_digital[0]["emcode"];
			break;*/	    
		case "wally_videoobject":
		    
			$mainObject_html .= $embeds_videos[0]["emcode"];
			if($embeds_videos[0]["credit"] != ""){
			$mainObject_html .= "<p class=\"pic_description\">".$embeds_videos[0]["title"]."</p>";
            }			
			break;
		case "wally_audioobject":
      $mainObject_html .= "<p>".$embeds_audios[0]["emcode"]."</p>";
      if($embeds_audios[0]["credit"] != ""){
       $mainObject_html .= "<p class=\"credit\">".$embeds_audios[0]["credit"]."</p>";
      }     
			break;
		case "wally_photoobject":
			$mainObject_html .= "<p>".$embeds_photos[0]["main_size"]."</p>";
			if($embeds_photos[0]["credit"] != ""){
			 $mainObject_html .= "<p class=\"credit\">".$embeds_photos[0]["credit"]."</p>";
			}
			break;
	}
}	
/*
 * Génération du breadcrumb
 */
 
$breadcrumb = _wallydemo_breadcrumb_display($node->field_destinations[0]["tid"]);

/*
 * Génération des liens de partage
 */
$socialSharingBaseUrl = wallydemo_get_social_sharing_base_url($mainDestination,$domain);
$socialSharingDomainAndPathUrl = $socialSharingBaseUrl."/".$node_path;
$fixedDomainAndPathUrl = "http://www.sudpresse.be/$node_path";


?>

<div id="wiki">
  <ul class="liensutiles">
    <li class="envoyer"><?php print forward_modal_link("node/".$node->nid,wallydemo_check_plain($main_title),"<img src=\"".$theme_path."/images/ico_envoyer2.gif\" alt=\"Envoyer à\" title=\"Envoyer à\"  width=\"19\" height=\"16\" />"); ?>
</li>
    <li class="reagir"> <a href="<?php print $node_path; ?>#ancre_commentaires"><?php print $reagir; ?></a> </li>
    <li class="facebook">
      <div id="fb-root"></div>
      <script src="http://connect.facebook.net/fr_FR/all.js#appId=276704085679009&amp;xfbml=1"></script>
      <fb:like href="<?php print $socialSharingDomainAndPathUrl; ?>" send="true" layout="button_count" width="90" show_faces="false" action="like" font="arial" ref="article_sudpresse"></fb:like>
    </li>
    <li class="linkedin"> 
      <script src="http://platform.linkedin.com/in.js" type="text/javascript"></script> 
      <script type="IN/Share" data-url="<?php print $socialSharingDomainAndPathUrl; ?>"></script></li>
    <li class="twitter"> 
      <script src="http://platform.twitter.com/widgets.js" type="text/javascript"></script>
      <div> <a href="http://twitter.com/share" class="twitter-share-button"
     data-url="<?php print $socialSharingDomainAndPathUrl; ?>"
     data-via="sudpresseonline"
     data-text="<?php print $main_title; ?>"
     data-related="lameuse.be:Toute l'information du La Meuse,LaGazette_be:Toute l'information de La Nouvelle Gazette,xalambert:Responsable de la rédaction de Sudpresse.be"
     data-count="horizontal"      
     data-lang="fr">Tweet</a> </div>
    </li>
    <li class="google">
      <div class="g-plusone" data-size="medium" data-href="<?php print $socialSharingDomainAndPathUrl; ?>"></div>
    </li>
  </ul>
  <ul class="publiele">
    <li class="edition"><?php print $date_edition; ?></li>
    <li class="changed"><?php print $date_changed; ?></li>
  </ul>
  <h1><?php print $main_title; ?></h1>
  <div id="picture"><?php print $mainObject_html; ?></div>
  <?php print $chapeau; ?> <?php print $texte_article; ?> </div>

<!-- Pour Googleplus --> 
<script type="text/javascript">
 window.___gcfg = {lang: 'fr'};

 (function() {
   var po = document.createElement('script'); po.type = 'text/javascript'; po.async = true;
   po.src = 'https://apis.google.com/js/plusone.js';
   var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(po, s);
 })();
</script>

<?php 
//ici unset des variables
unset($node);
unset($mainstory);
unset($photoObject);
unset($embeds);
unset($embeds_audios);
unset($embeds_videos);
unset($embeds_digital);
unset($embeds_photos);
?> 
