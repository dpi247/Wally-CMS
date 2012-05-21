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

//drupal_add_css(drupal_get_path('theme', 'wallydemo').'/css/article.css','file','screen');
drupal_add_js(drupal_get_path('theme', 'wallydemo').'/scripts/jquery.scrollTo-min.js');
drupal_add_js(drupal_get_path('theme', 'wallydemo').'/scripts/jquery.localscroll-min.js');
drupal_add_js(drupal_get_path('theme', 'wallydemo').'/scripts/script-article.js');

$nodes = array();
foreach ( $node->field_embededobjects_nodes as $n)
{
	node_build_content($n);
  drupal_render($n->content);
	$nodes[] = $n; 
}
$node->field_embededobjects_nodes = $nodes;
unset($nodes); 

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
//$aliases = wallytoolbox_get_path_aliases("node/".$node->nid);
module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
$aliases = wallytoolbox_get_all_aliases("node/".$node->nid);
$node_path = $aliases[0];



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

/* Affichage de la date au format souhaité
 * Les formats sont:
 * 
 * 'filinfo' -> '00h00'
 * 'unebis' -> 'jeudi 26 mai 2011, 15:54'
 * 'default' -> 'publié le 26/05 à 15h22'
 * 
 * print($date_edition);
 */ 
 
$date_edition = "<p class=\"publiele\">Publié le " ._wallydemo_date_edition_diplay($node_publi_date, 'date_jour_heure') ."</p>";
 
/* Récupération du chapeau de l'article -> $strapline
 * Le nombre de caractères attendus pour ce chapeau est spécifié dans $strapline_length
 * Si aucune limitation n'est attendue, laisser la valeur de $strapline_length à 0
 * 
 * print($strapline);
 */

$strapline_length = 0;
$strapline = _wallydemo_get_strapline($mainstory,$node,$strapline_length);

$package_signature = _wallydemo_get_package_signature($node->field_packageauthors) ;

$main_title = $mainstory->title;
$chapeau = "<p class=\"chapeau\">" .$strapline ."</p>";
$texte_article = $mainstory->field_textbody[0]['value'];
$signature = "<p class=\"auteur\">".$package_signature."</p>";

drupal_add_css($themeroot . '/css/article.css');

$nb_comment = $node->comment_count;
if ($nb_comment == 0) $reagir = "réagir";
else if ($nb_comment == 1) $reagir = $nb_comment."&nbsp;réaction";
else $reagir = $nb_comment."&nbsp;réactions";


$embeds = wallydemo_bracket_embeddedObjects_from_package($node);

//print_r($embeds);
//exit();
if(is_array($embeds)){
	$embeds_photos = array();
	$embeds_videos = array();
	$embeds_audios = array();
	$embeds_digital = array();
	
	foreach($embeds["photos"] as $photo){
		$embed_photo = wallydemo_get_photo_infos_and_display($photo);
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
	
	//dsm($embeds_photos);
	//dsm($embeds_digital);
	//dsm($embeds_videos);	
	//dsm($node);
	//dsm($photoObject);
	//dsm($links);
	
	// Génération main bloc médias	
	// I -> on affiche l’image.
	// II -> on affiche que la première image
	// V -> on affiche la vidéo
	// VI -> on affiche que la vidéo
	// IV -> on affiche l'image dans la zone top gauche et la ou les vidéos en dessous de l'article
	
	
	$mainObject_html = "";
	$mainObject_html .= "<div class=\"allMedias\"><div class=\"wrappAllMedia\">";
	
	switch($embeds["mainObject"]->type){  
		case "wally_videoobject":				                     		
		 if(stripos($embeds_videos[0]["emcode"], 'www.youtube.com') !== FALSE) {
			 		$temp = 'height="350" width="425"';
			 		$temp2 = 'width="425" height="350"';
					$embeds_videos[0]["emcode"] = str_replace($temp, "height='200' width='300'", $embeds_videos[0]["emcode"]);
			    $embeds_videos[0]["emcode"] = str_replace($temp2, "height='200' width='300'", $embeds_videos[0]["emcode"]);
		    } else {
			   $embeds_videos[0]["emcode"] = preg_replace('+width=("|\')[0-9]{3}("|\')+','width="300"',$embeds_videos[0]["emcode"]);
			   $embeds_videos[0]["emcode"] = preg_replace('+height=("|\')[0-9]{3}("|\')+','height="200"',$embeds_videos[0]["emcode"]);
			  }
			$mainObject_html .= "<a name=\"".$embed['nid']."\" />";
			$mainObject_html .= "<div id=\"item".$embeds_videos[0]['nid']."\" class=\"item_media\">".$embeds_videos[0]["emcode"];
			if($embeds_videos[0]["summary"] != ""){
			$mainObject_html .= "<div class=\"pic_description\">".$embeds_videos[0]["summary"]."</div>";
            }
			$mainObject_html .= "</div>";
			break;
		case "wally_photoobject":
			$mainObject_html .= "<div id=\"item".$embeds_photos[0]['nid']."\" class=\"item_media\">".$embeds_photos[0]["main_size"];
			if($embeds_photos[0]["credit"] != ""){
			 $mainObject_html .= "<p class=\"credit\">".$embeds_photos[0]["credit"]."</p>";
			}
			if($embeds_photos[0]["summary"] != ""){
			 $mainObject_html .= "<p class=\"pic_description\">".$embeds_photos[0]["summary"]."</p>";
			}
			$mainObject_html .= "</div>";
		  break;
	}

	$mainObject_html .= "</div></div>";

	// Fin génération main bloc médias
	
	// Génération bloc médias vidéos affiché sous l'article
	if(count($embeds_videos) > 0){
    $cpt = 0;		
		$bottomVideosBlock = "<div id=\"bottomVideos\">";
		foreach ($embeds_videos as $embed){
		  // Si mainObject est une vidéo, il ne faut pas la réafficher ici
		  if($embeds["mainObject"]->type == 'wally_videoobject' && $cpt==0){ 
		    $cpt++;
		  	CONTINUE;
		  }
		  if(stripos($embed["emcode"], 'www.youtube.com') !== FALSE) {
        $temp = 'height="350" width="425"';
        $temp2 = 'width="425" height="350"';
        $embed["emcode"] = str_replace($temp, "height='200' width='300'", $embed["emcode"]);
        $embed["emcode"] = str_replace($temp2, "height='200' width='300'", $embed["emcode"]);
      } else {
        $embed["emcode"] = preg_replace('+width=("|\')[0-9]{3}("|\')+','width="300"',$embed["emcode"]);
        $embed["emcode"] = preg_replace('+height=("|\')[0-9]{3}("|\')+','height="200"',$embed["emcode"]);
      }		  
		  $bottomVideosBlock .= "<a name=\"".$embed['nid']."\" />";
		  $bottomVideosBlock .= "<div class=\"bottomVideos\">" .$embed[emcode];
			if($embed["summary"] != ""){
		  $bottomVideosBlock .= "<p class=\"pic_description\">".$embed["summary"]."</p>";
            }
		  $bottomVideosBlock .= "</div>";
		}
	  $bottomVideosBlock .= "</div>";
	}
	// Fin génération bloc médias vidéos affiché sous l'article
	
	// Génération html médias digitaux affichés sous l'article
	if(count($embeds_digital) > 0){
		$bottomDigitalElements = "";
		foreach ($embeds_digital as $embed){
			if($embed['provider'] == "coveritlive"){
				$bottomDigitalElements .= "<a name=\"".$embed['nid']."\" />";
				$bottomDigitalElements .= "<div class=\"digital-".$embed["type"]."\">";
				$bottomDigitalElements .= $embed["emcode"];
				$bottomDigitalElements .= "</div>";
			}
		}
	}
	// Fin génération html médias digitaux affichés sous l'article
}	
/*
 * Génération du breadcrumb
 */
$breadcrumb = _wallydemo_breadcrumb_display($node->field_destinations[0]["tid"]);


/*
 * Génération des liens de partage
 */
$fixedDomainAndPathUrl = "http://www.sudinfo.be/$node_path";
$domainAndPathUrl = "http://www.sudinfo.be/$node_path";
$linkTwitter = "http://twitter.com/share?text=$main_title";

?>
<?php echo $breadcrumb; ?>
<div class="liensutiles">
  <ul>
    <li class="reagir"><a href="<?php print $node_path; ?>#ancre_commentaires"><?php print $reagir; ?></a></li>
    <li class="facebook">
      <div id="fb-root"></div>
      <script src="http://connect.facebook.net/fr_FR/all.js#appId=276704085679009&amp;xfbml=1"></script>
      <fb:like href="<?php print $fixedDomainAndPathUrl ?>" send="true" layout="button_count" width="90" show_faces="false" action="like" font="arial" ref="article_sudpresse"></fb:like>
    </li>
  </ul>
  <ul>
    <li class="twitter"> 
      <script src="http://platform.twitter.com/widgets.js" type="text/javascript"></script>
      <div> <a href="http://twitter.com/share" class="twitter-share-button"
     data-url="<?php print $fixedDomainAndPathUrl; ?>"
     data-via="sudpresseonline"
     data-text="<?php print $main_title; ?>"
     data-related="lameuse.be:Toute l'information du La Meuse,LaGazette_be:Toute l'information de La Nouvelle Gazette,xalambert:Responsable de la rédaction de Sudpresse.be"     
     data-lang="fr">Tweet</a></div>
    </li>
    <li class="google">
      <div class="g-plusone" count="false" data-size="medium" data-href="<?php print $fixedDomainAndPathUrl; ?>"></div>
    </li>
  </ul>
</div>
<div id="article">
  <h1 id="h1_article"><?php print $main_title; ?></h1>
  <div id="picture"> <?php print $mainObject_html; ?></div>
  <?php print $chapeau; ?> <?php print $signature; ?> <?php print $date_edition; ?> <?php print $texte_article; ?>
  <?php if($bottomVideosBlock) print $bottomVideosBlock ; ?>
  <?php if($bottomDigitalElements) print $bottomDigitalElements ; ?>
  <?php /* $fid = 'http://www-dev.sudpresse.be/node/'.$node->nid; */ ?>
  <a id="ancre_commentaires" name="ancre_commentaires" href="#ancre_commentaires" />
  <div id="commentaires">
    <div id="fb-root"></div>
    <script src="http://connect.facebook.net/fr_FR/all.js"></script> 
    <script>
  	FB.init({
  	appId  : '218987958113539',
  	status : true, // check login status
  	cookie : true, // enable cookies to allow the server to access the session
  	xfbml  : true  // parse XFBML
  	});
  </script> 
    <script src="http://connect.facebook.net/fr_FR/all.js#xfbml=1"></script>
    <fb:comments href="<?php print $fixedDomainAndPathUrl; ?>" num_posts="10" width="300" />
  </div>
  <!-- Facebook social bar 
  <script>(function(d){
	  var js, id = 'facebook-jssdk'; if (d.getElementById(id)) {return;}
	  js = d.createElement('script'); js.id = id; js.async = true;
	  js.src = "//connect.facebook.net/en_US/all.js#appId=191499344249079&xfbml=1";
	  d.getElementsByTagName('head')[0].appendChild(js);
	  }(document));
	</script>
  <fb:social-bar href="<?php print $fixedDomainAndPathUrl ; ?>" trigger="0%" read_time="8" action="recommend" /> --> 
  <!-- Pour Googleplus --> 
  <script type="text/javascript">
 window.___gcfg = {lang: 'fr'};

 (function() {
   var po = document.createElement('script'); po.type = 'text/javascript'; po.async = true;
   po.src = 'https://apis.google.com/js/plusone.js';
   var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(po, s);
 })();
</script> 
</div>

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
