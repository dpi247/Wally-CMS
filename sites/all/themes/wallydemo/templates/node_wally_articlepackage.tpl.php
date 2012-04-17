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
foreach ( $node->field_embededobjects_nodes as $n){
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
$domain = 'sudinfo';

// Get the node's main destination
$mainDestination = $node->field_destinations[0]["tid"];

// Le package -> $node

/* R̩cup̩ration de l'id du package -> $node_id
 *
 */
$node_id = $node->nid;

/* R̩cup̩ration de l'alias de l'url du package -> $node_path
 *
 * print($node_path);
 */
//$aliases = wallytoolbox_get_path_aliases("node/".$node->nid);
$aliases = wallytoolbox_get_all_aliases("node/".$node->nid);
$node_path = $aliases[0];

/* R̩cup̩ration du path
 *
 */
$theme_path = drupal_get_path('theme', 'wallydemo');

/* R̩cup̩ration du mainstory et de la photo principale du package.
 * Le package peut ̻tre articlePackage ou galleryPackage
 *
 * Objet principal du package -> $mainstory
 *
 * Photo principale du package -> $photoObject
 * Path vers cette image -> $photoObject_path
 * Taille de la photo sur le serveur -> $photoObject_size
 *
 * S'il y a bien une image �  afficher -> $photo==TRUE
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
} else {
  $mainstory = $node->field_mainobject_nodes[0];
  $mainstory_type = $mainstory->type;
  if ($mainstory_type == "wally_photoobject"){
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
  if ($photoObject) {
    $photoObject_path = $photoObject->field_photofile[0]['filepath'];
    $explfilepath = explode('/', $photoObject_path);
    $photoObject_size = $photoObject->field_photofile[0]['filesize'];
    if (isset($photoObject_path) && $photoObject_size > 0) {
      $photo = TRUE;
    }
  }
}


/*  R̩cup̩ration de la date de publication du package -> $node_publi_date
 */
$node_publi_date = strtotime($node->field_publicationdate[0]['value']);

/* Affichage de la date au format souhait̩
 * Les formats sont:
 *
 * 'filinfo' -> '00h00'
 * 'unebis' -> 'jeudi 26 mai 2011, 15:54'
 * 'default' -> 'publi̩ le 26/05 �  15h22'
 *
 * print($date_edition);
 */

$date_edition = "<p class=\"publiele\">Publi̩ le " ._wallydemo_date_edition_diplay($node_publi_date, 'date_jour_heure') ."</p>";

/* R̩cup̩ration du chapeau de l'article -> $strapline
 * Le nombre de caract̬res attendus pour ce chapeau est sp̩cifi̩ dans $strapline_length
 * Si aucune limitation n'est attendue, laisser la valeur de $strapline_length �  0
 *
 * print($strapline);
 */

//$strapline_length = 0;
//$strapline = _wallydemo_get_strapline($mainstory,$node,$strapline_length);
if ($mainstory->type == "wally_textobject"){
  $strapline = $mainstory->field_textchapo[0]['value'];
} else {
  $strapline = $mainstory->field_summary[0]['value'];
}


$package_signature = _wallydemo_get_package_signature($mainstory) ;

$main_title = $mainstory->title;
$chapeau = "";
if (isset($strapline)){
  $chapeau = "<p class=\"chapeau\">" .$strapline ."</p>";
}
$texte_article = $mainstory->field_textbody[0]['value'];
$signature = "<p class=\"auteur\">".$package_signature."</p>";

$byline="<p class=\"byline\">" .$mainstory->field_byline[0]['value'] ."</p>";;
$extract_short="<p class=\"extract_short\">" .$mainstory->field_extractshort[0]['value'] ."</p>";;
$extract_medium="<p class=\"extract_medium\">" .$mainstory->field_extractmedium[0]['value'] ."</p>";

drupal_add_css($themeroot . '/css/article.css');

//wallytoolbox_add_meta(array("property"=>"og:type"), "Article");
//wallytoolbox_add_meta(array("property"=>"og:url"), $node_path);

$nb_comment = $node->comment_count;
//$nb_comment = $node->comment;
if ($nb_comment == 0) {
  $reagir = "r̩agir";
} else if ($nb_comment == 1) {
  $reagir = $nb_comment."&nbsp;r̩action";
} else {
  $reagir = $nb_comment."&nbsp;r̩actions";
}

$links = _wallydemo_get_sorted_links($node);
$links_html = "";
foreach ($links as $linksList){
  if (isset($linksList["title"])){
    $list_titre = $linksList["title"];
    $links_html .= "<div class=\"bloc-01\"><h2>".$list_titre."</h2><div class=\"inner-bloc\"><ul>";
    if (isset($linksList["links"])){
      foreach($linksList["links"] as $link){
        $link_url = $link["url"];
        $link_title = $link["title"];
        $link_target = $link["target"];
        $link_type = $link["type"];
        if ($link["packagelayout"] == 'Article Wiki') {
          $links_html .= "<li class=\"media-dossier\">" ."<a class=\"novisited\" href=\"".$link_url."\" target=\"".$link_target."\">".$link_title."</a></li>";
        } else {
          $links_html .= "<li class=\"media-press\">" ."<a href=\"".$link_url."\" target=\"".$link_target."\">".$link_title."</a></li>";
        }
      }
    }
    $links_html .= "</ul></div></div>";
  }
}

$embeds = wallydemo_bracket_embeddedObjects_from_package($node);

//print_r($embeds);
//exit();
if (is_array($embeds)){
  $embeds_photos = array();
  $embeds_videos = array();
  $embeds_audios = array();
  $embeds_digital = array();
  $embeds_link = array();
  foreach ($embeds["photos"] as $photo){
    $embed_photo = wallydemo_get_photo_infos_and_display($photo);
    array_push($embeds_photos,$embed_photo);
  }
  foreach ($embeds["videos"] as $video){
    $embed_video = wallydemo_get_video_infos_and_display($video);
    array_push($embeds_videos,$embed_video);
  }
  foreach ($embeds["audios"] as $audio){
    $embed_audio = wallydemo_get_audio_infos_and_display($audio);
    array_push($embeds_audios,$embed_audio);
  }
  foreach ($embeds["digital"] as $digital){
    $embed_digital = wallydemo_get_digitalobject_infos_and_display($digital);
    array_push($embeds_digital,$embed_digital);
  }
  foreach ($embeds["link"] as $link){
    array_push($embeds_link, $link);
  }


  //dsm($embeds_photos);
  //dsm($embeds_videos,videos);
  //dsm($node);
  //dsm($photoObject,photos);
  //dsm($links,links);

  // G̩n̩ration main bloc m̩dias
  // I -> on affiche l�۪image.
  // V -> on affiche la vid̩o top gauche et on place une image par d̩faut dans les flux.
  // VI -> on affiche la vid̩o top gauche, on place un navigateur media avec les vignettes VI.
  // IV -> on affiche l�۪image top gauche et on place la vid̩o en bas de l�۪article.
  // VII -> on affiche la vid̩o top gauche, on place un navigateur media avec les vignettes VII.
  // IVV -> on affiche l�۪image top gauche et on place les vid̩os l�۪une sur l�۪autre en bas de l�۪article.

  //teste le besoin de cr̩er une galerie medias
  $galMedias = FALSE;
  if (count($embeds_photos) > 1 || (count($embeds_photos) > 0 && $embeds["mainObject"]->type == 'wally_videoobject')){
    $galMedias = TRUE;
  }

  //calcul du nombre d'̩l̩ments affich̩s pour fixer la largeur de la div wrappAllMedia
  $cpt = 0;
  $widthMedias = 0;
  if ($galMedias == TRUE){
    if ($embeds["mainObject"]->type == 'wally_videoobject'){
      $cpt++;
    }
    foreach($embeds_photos as $embed){
      $cpt++;
    }
    $widthMedias = 300*$cpt;
  }



  $mainObject_html = "";
  $mainObject_html .= "<div class=\"allMedias\"><div class=\"wrappAllMedia\" style=\"width:".$widthMedias."px;\">";

  switch($embeds["mainObject"]->type){
     
    case "wally_videoobject":
      if (stripos($embeds_videos[0]["emcode"], 'www.youtube.com') !== FALSE) {
        $temp = 'height="350" width="425"';
        $temp2 = 'width="425" height="350"';
        $embeds_videos[0]["emcode"] = str_replace($temp, "height='200' width='300'", $embeds_videos[0]["emcode"]);
        $embeds_videos[0]["emcode"] = str_replace($temp2, "height='200' width='300'", $embeds_videos[0]["emcode"]);
      } else {
        $embeds_videos[0]["emcode"] = preg_replace('+width=("|\')[0-9]{3}("|\')+','width="300"',$embeds_videos[0]["emcode"]);
        $embeds_videos[0]["emcode"] = preg_replace('+height=("|\')[0-9]{3}("|\')+','height="200"',$embeds_videos[0]["emcode"]);
      }
      $mainObject_html .= "<a name=\"".$embeds_videos[0]['nid']."\" ></a>";
      $mainObject_html .= "<div id=\"item".$embeds_videos[0]['nid']."\" class=\"item_media\">".$embeds_videos[0]["emcode"];
      if ($embeds_videos[0]["summary"] != ""){
        $mainObject_html .= "<div class=\"pic_description\">".$embeds_videos[0]["summary"]."</div>";
      }
      $mainObject_html .= "</div>";
      if (count($embeds_photos) > 0 ){
        foreach ($embeds_photos as $embed){
          $mainObject_html .= "<div id=\"item".$embed['nid']."\" class=\"item_media\">".$embed["main_size"];
          if ($embeds_photos[0]["credit"] != ""){
            $mainObject_html .= "<p class=\"credit\">".$embed["credit"]."</p>";
          }
          if (trim(strip_tags($embed["summary"])) != "") {
            $mainObject_html .= "<p class=\"pic_description\">".strip_tags($embed["summary"])."</p>";
          }
          $mainObject_html .= "</div>";
        }
      }
      break;
       
    case "wally_photoobject":
      foreach ($embeds_photos as $embed){
        //				$mainObject_html .= "<a name=\"".$embed['nid']."\" ></a>";
        $mainObject_html .= "<div id=\"item".$embed['nid']."\" class=\"item_media\">".$embed["main_size"];
        if ($embed["credit"] != ""){
          $mainObject_html .= "<p class=\"credit\">".$embed["credit"]."</p>";
        }
        if (trim(strip_tags($embed["summary"])) != ""){
          $mainObject_html .= "<p class=\"pic_description\">".strip_tags($embed["summary"])."</p>";
        }
        $mainObject_html .= "</div>";
      }
      break;
  }

  /*
   //print_r($embeds_videos);
   if (count($embeds_videos) > 0){
   $mainObject_html .= "<div class=\"bloc-01 pf_article\"><h2>Vid̩os</h2><div class=\"inner-bloc\"><ul>";
   foreach($embeds_videos as $embed){
   $mainObject_html .="<li><a href=\"javascript:void(0)\"><img width=\"48\" height=\"32\" src=\"".$embeds_video[0]['thumbnail']."\"></a></li>";
   }
   $mainObject_html .= "</ul></div></div>";
   }
   */

  // fin div wrappAllMedia et allMedias
  $mainObject_html .= "</div></div>";

  if ($galMedias == TRUE){
    $mainObject_html .= "<div class=\"bloc-01 pf_article\"><h2>M̩dias</h2><div class=\"inner-bloc\"><ul class=\"mini-pagination\">";
    if ($embeds["mainObject"]->type == 'wally_videoobject'){
      $mainObject_html .="<li><a href=\"#item".$embeds_videos[0]['nid']."\"><img width=\"48\" height=\"32\" src=\"".$embeds_videos[0]['thumbnail']."\"></a></li>";
    }
    foreach ($embeds_photos as $embed){
      $mainObject_html .="<li><a href=\"#item".$embed['nid']."\">\n\t".$embed['mini']."</a>\n</li>\n";
    }
    $mainObject_html .= "</ul></div></div>";
  }
  // Fin g̩n̩ration main bloc m̩dias

  // G̩n̩ration bloc m̩dias vid̩os affich̩ sous l'article
  if (count($embeds_videos) > 0){
    $cpt = 0;
    $bottomVideosBlock = "<div id=\"bottomVideos\">";
    foreach ($embeds_videos as $embed){
      // Si mainObject est une vid̩o, il ne faut pas la r̩afficher ici
      if ($embeds["mainObject"]->type == 'wally_videoobject' && $cpt==0){
        $cpt++;
        CONTINUE;
      }
      $bottomVideosBlock .= "<a name=\"".$embed['nid']."\" ></a>";
      $bottomVideosBlock .= "<div class=\"bottomVideos\">" .$embed['emcode'];
      if (trim(strip_tags($embed["summary"])) != ""){
        $bottomVideosBlock .= "<p class=\"pic_description\">".strip_tags($embed["summary"])."</p>";
      }
      $bottomVideosBlock .= "</div>";
    }
    $bottomVideosBlock .= "</div>";
  }
  // Fin g̩n̩ration bloc m̩dias vid̩os affich̩ sous l'article

  // G̩n̩ration html m̩dias digitaux affich̩s sous l'article
  if (count($embeds_digital) > 0){
    $bottomDigitalElements = "";
    foreach ($embeds_digital as $embed){
      $bottomDigitalElements .= "<a name=\"".$embed['nid']."\" ></a>";
      $bottomDigitalElements .= "<div class=\"digital-".$embed["type"]."\">";
      $bottomDigitalElements .= $embed["emcode"];
      $bottomDigitalElements .= "</div>";
    }
  }

  // G̩n̩ration html m̩dias audios affich̩s sous l'article
  if (count($embeds_audios) > 0){
    $bottomAudioElements = "";
    foreach ($embeds_audios as $embed) {
      $bottomAudioElements .= "<a name=\"".$embed['nid']."\" ></a>";
      $bottomAudioElements .= "<div class=\"digital-".$embed["type"]."\">";
      $bottomAudioElements .= "<h3>".$embed['title']."</h3>";
      $bottomAudioElements .= $embed["emcode"];
      $bottomAudioElements .= "</div>";
    }
  }

  $html_embedpackages = '';
  if (count($node->embed_packages) > 0){
    foreach ($node->embed_packages as $embpackage){
      //Package ours
      $html_embedpackages .= '<div class = "clear"></div><div id = "embedded_package">';
      $html_embedpackages .= $embpackage;
      $html_embedpackages .= '</div>';
       
    }
  }
  $html_embedlinks = '';
  $link_picture = '';
  $link_thumb = '';
  $class_image = '';
  $class_thumb = '';
  if (count($node->embed_links) > 1){

    foreach ($node->embed_links as $key => $emblink){
      if(($emblink['type']=="emimage"||$emblink['type']=="emvideo")&&($emblink['provider']!='flickr_sets'&&$emblink['provider']!='slideshare')){
        $link_picture .=  '<div id ="item'.$key.'" class = "item_media '.$class_image. $emblink['type'].'">';
        $link_picture .= $emblink['content'];
        $link_picture .= '</div>';
        $link_thumb .= '<li class="'.$class_thumb.'">';
        $link_thumb .= '<a href="#item'.$key.'">';
        $link_thumb .= $emblink['thumb'];
        $link_thumb .= '</a>';
        $link_thumb .= '</li>';
        $class_image = '';
        $class_thumb = '';
      }
    }


































    $width = count($node->embed_links)*300;
    $html_embedlinks .= '<div id="links">';
    $html_embedlinks .= '<div class="allMedias">';
    $html_embedlinks .= '<div class="wrappAllMedia" style="width:'.$width.'px;">';
    $html_embedlinks .= $link_picture;
    $html_embedlinks .= '</div>';
    $html_embedlinks .= '</div>';
    $html_embedlinks .= '<div class="bloc-01 pf_article">';
    $html_embedlinks .= '<h2>M̩dias</h2>';
    $html_embedlinks .= '<div class="inner-bloc">';
    $html_embedlinks .= '<ul class="mini-pagination">';
    $html_embedlinks .= $link_thumb;
    $html_embedlinks .= '</ul>';
    $html_embedlinks .= '</div>';
    $html_embedlinks .= '</div>';
    $html_embedlinks .= '</div>';
  } elseif (count($node->embed_links) > 0 ){
      if(($emblink['type']=="emimage"||$emblink['type']=="emvideo")&&($emblink['provider']!='flickr_sets'&&$emblink['provider']!='slideshare')){
        
    $embed_link = array_pop($node->embed_links);
    $html_embedlinks .= $embed_link['image'];
      }
  }

  $html_embedurl = '';
  if (count($node->embed_url) > 0){

    $html_embedurl .= '<div class="bloc-01"><h2>'.t('Links').'</h2><div class="inner-bloc"><ul>';
    foreach($node->embed_url as $link){
      $html_embedurl .= '<li class="media-press">'.$link.'</li>';
    }
    $html_embedurl .= '</ul></div></div>';
  }
  // Fin g̩n̩ration html m̩dias digitaux affich̩s sous l'article
}
/*
 * G̩n̩ration du breadcrumb
 */
$breadcrumb = _wallydemo_breadcrumb_display($node->field_destinations[0]["tid"]);


/*
 * G̩n̩ration des liens de partage
 */
$socialSharingBaseUrl = wallydemo_get_social_sharing_base_url($mainDestination,$domain);
$socialSharingDomainAndPathUrl = $socialSharingBaseUrl."/".$node_path;
$fixedDomainAndPathUrl = "http://www.sudpresse.be/$node_path";

/*
 * R̩cup̩ration des tags de l'article et afficha html
 * et
 * On n'affiche pas les tags dans le fil info
 *
 */
$htmltags = wallydemo_taxonomy_tags_particle($node);
$taxonomy = $node->field_destinations[0]["tid"];

if ($htmltags != "" && $taxonomy != "20"){
  $listTags .= "<div class=\"tags\"><h2>Termes associ̩s : </h2>".$htmltags."</div>";
}

?>

<div
	id="article">
	<?php echo $breadcrumb; ?>
	<ul class="liensutiles">
		<li class="envoyer"><?php print forward_modal_link("node/".$node->nid,wallydemo_check_plain($main_title),"<img src=\"/".$theme_path."/images/ico_envoyer2.gif\" alt=\"Envoyer � \" title=\"Envoyer � \" width=\"19\" height=\"16\" />"); ?>
		</li>
		<li class="imprimer"><a href="javascript:window.print();"><img
				src="/<?php echo $theme_path; ?>/images/ico_imprimer2.gif"
				alt="Imprimer" title="Imprimer" width="22" height="20" /> </a></li>
		<li class="reagir"><a
			href="<?php print $node_path; ?>#ancre_commentaires"><?php print $reagir; ?>
		</a>
		</li>
		<li class="facebook">
			<div id="fb-root"></div> <script
				src="http://connect.facebook.net/fr_FR/all.js#appId=276704085679009&amp;xfbml=1"></script>
			<fb:like href="<?php print $socialSharingDomainAndPathUrl; ?>"
				send="true" layout="button_count" width="90" show_faces="false"
				action="like" font="arial" ref="article_sudpresse"></fb:like>
		</li>
		<li class="linkedin"><script src="http://platform.linkedin.com/in.js"
				type="text/javascript"></script> <script type="IN/Share"
				data-url="<?php print $socialSharingDomainAndPathUrl; ?>"></script>
		</li>
		<li class="twitter"><script
				src="http://platform.twitter.com/widgets.js" type="text/javascript"></script>
			<div>
				<a href="http://twitter.com/share" class="twitter-share-button"
					data-url="<?php print $socialSharingDomainAndPathUrl; ?>"
					data-via="sudpresseonline"
					data-text="<?php print str_replace('"', '', $main_title); ?>"
					data-related="lameuse.be:Toute l'information du La Meuse,LaGazette_be:Toute l'information de La Nouvelle Gazette,xalambert:Responsable de la r̩daction de Sudpresse.be"
					data-count="horizontal" data-lang="fr">Tweet</a>
			</div>
		</li>
		<li class="google">
			<div class="g-plusone" data-size="medium"
				data-href="<?php print $socialSharingDomainAndPathUrl; ?>"></div>
		</li>
	</ul>
	<h1>
	<?php print wallydemo_check_plain($main_title); ?>
	</h1>

	<div id="picture">
	<?php

	print '<div id = "main">'.$mainObject_html.'</div>';
	print $html_embedlinks;
	print $html_embedurl;
	print '<div id = "link">'.$links_html.'</div>';
	?>

	</div>
	<?php
	print $chapeau;
	print $signature;
	print $date_edition;
	print $texte_article;
	
	print $byline;
    print $extract_short;
	print $extract_medium;
	
	if (isset($bottomVideosBlock)) print $bottomVideosBlock ;
	if (isset($bottomDigitalElements)) print $bottomDigitalElements ;
	if (isset($bottomAudioElements)) print $bottomAudioElements ;


	$link_picture="";
	if(isset($node->embed_links)){
	  foreach ($node->embed_links as $key => $emblink){
	    if(($emblink['type']!="emimage" && $emblink['type']!="emvideo") || ($emblink['provider']=='flickr_sets'||$emblink['provider']=='slideshare')){
	      $link_picture .=  '<div class="digital-wally_audioobject" width:"100%"><div id ="itddem'.$key.'" class = "item_medddia '.$class_image. $emblink['type'].'">';
	      $link_picture .= "<h3>".$emblink['title'].'</h3>';
	      $link_picture .= $emblink['content'];
	      $link_picture .= '</div></div>';
	      $class_image = '';
	      $class_thumb = '';
	    }
	  }
	  print $link_picture;
	}




	print $html_embedpackages;
	print $listTags; ?>
	<!-- FACEBOOK REACTIONS -->
	<?php if ($node->comment == 2) { ?>
	<a id="ancre_commentaires" name="ancre_commentaires"
		href="#ancre_commentaires" /></a>
		<?php print theme("spreactions_facebook", $node_id, $socialSharingBaseUrl, $socialSharingDomainAndPathUrl, $socialSharingDomainAndPathUrl); ?>
		<?php } ?>
	<!-- Facebook social bar -->
	<script>/*(function(d){
	  var js, id = 'facebook-jssdk'; if (d.getElementById(id)) {return;}
	  js = d.createElement('script'); js.id = id; js.async = true;
	  js.src = "//connect.facebook.net/fr_FR/all.js#appId=191499344249079&xfbml=1";
	  d.getElementsByTagName('head')[0].appendChild(js);
	  }(document));*/
	</script>
	<!-- <fb:social-bar href="<?php print $socialSharingDomainAndPathUrl ; ?>" trigger="0%" read_time="8" action="recommend" /> -->
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
		unset($links);
		unset($embeds);
		unset($embeds_audios);
		unset($embeds_videos);
		unset($embeds_photos);
		unset($embeds_digital);
		?>