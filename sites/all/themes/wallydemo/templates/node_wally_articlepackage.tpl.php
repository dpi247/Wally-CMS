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

/* R̩cup̩ration du path
 *
 */
$theme_path = drupal_get_path('theme', 'wallydemo');
$themeroot = $theme_path;

//drupal_add_css(drupal_get_path('theme', 'wallydemo').'/css/article.css','file','screen');
drupal_add_js(drupal_get_path('theme', 'wallydemo').'/scripts/jquery.scrollTo-min.js');
drupal_add_js(drupal_get_path('theme', 'wallydemo').'/scripts/jquery.localscroll-min.js');
drupal_add_js(drupal_get_path('theme', 'wallydemo').'/scripts/script-article.js');
drupal_add_css($themeroot . '/css/article.css');

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
module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
$aliases = wallytoolbox_get_all_aliases("node/".$node->nid);
$node_path = $aliases[0];

/* R̩cup̩ration du chapeau de l'article -> $strapline
 * Le nombre de caract̬res attendus pour ce chapeau est sp̩cifi̩ dans $strapline_length
 * Si aucune limitation n'est attendue, laisser la valeur de $strapline_length �  0
 *
 * print($strapline);
 */
$mainstory = $node->field_mainstory_nodes[0];
if ($mainstory->type == "wally_textobject"){
  node_build_content($mainstory, $teaser, $page);
  $strapline = $mainstory->field_textchapo[0]['safe'];
} else {
  $strapline = $mainstory->field_summary[0]['safe'];
}

/*  R̩cup̩ration de la date de publication du package -> $node_publi_date
 *
 *
 * Affichage de la date au format souhait̩
 * Les formats sont:
 *
 * 'filinfo' -> '00h00'
 * 'unebis' -> 'jeudi 26 mai 2011, 15:54'
 * 'default' -> 'publi̩ le 26/05 �  15h22'
 *
 * print($date_edition);
 */

if(!isset($node->field_editorialupdatedate[0]['safe'])){
  $field_editorialupdatedate=$node->field_publicationdate[0]['safe'];
  
}
else{
  $field_editorialupdatedate=$node->field_editorialupdatedate[0]['safe'];
  
  
}
$editorialupdatedate = strtotime($field_editorialupdatedate);
$node_publi_date = strtotime($node->field_publicationdate[0]['safe']);

$date_edition = "<p class=\"publiele\">Last update editorial: " ._wallydemo_date_edition_diplay($editorialupdatedate, 'date_jour_heure') ."</p>";
$date_systeme = "<p class=\"publiele\">Last update system: " ._wallydemo_date_edition_diplay($node_publi_date, 'date_jour_heure') ."</p>";

$package_signature = _wallydemo_get_package_signature($mainstory) ;

$main_title = $mainstory->title;
$chapeau = "";
if (isset($strapline)){
  $chapeau = "<p class=\"chapeau\">" .$strapline ."</p>";
}
$texte_article = $mainstory->field_textbody[0]['safe'];
$signature = "<p class=\"auteur\">".$package_signature."</p>";
$byline="<p class=\"byline\">" .$mainstory->field_byline[0]['safe'] ."</p>";;
$extract_short="<p class=\"extract_short\">" .$mainstory->field_extractshort[0]['safe'] ."</p>";;
$extract_medium="<p class=\"extract_medium\">" .$mainstory->field_extractmedium[0]['safe'] ."</p>";

$nb_comment = $node->comment_count;
if ($nb_comment == 0) {
  $reagir = "r̩agir";
} else if ($nb_comment == 1) {
  $reagir = $nb_comment."&nbsp;r̩action";
} else {
  $reagir = $nb_comment."&nbsp;r̩actions";
}

/*
 * G̩n̩ration des liens de partage
 */
$socialSharingBaseUrl = wallydemo_get_social_sharing_base_url($mainDestination,$domain);
$socialSharingDomainAndPathUrl = $socialSharingBaseUrl."/".$node_path;
$fixedDomainAndPathUrl = "http://www.sudpresse.be/$node_path";

?>

<div id="article">
	<?php echo $breadcrumb; ?>
	
	<?php if($bool_node_page):?>
	
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
	
	<?php endif;?>
	<h1>
	<?php print wallydemo_check_plain($main_title); ?>
	</h1>

	<div id="picture">
   <?php
	print $mediabox_html;
	print '<div id = "link">'.$linkslist_html.'</div>';
	?>
	</div>

<?php
	print $byline;
	print $chapeau;
	print $signature;
	print $date_edition;
	print $date_systeme;
	print $texte_article;
	print $extract_short;
	print $extract_medium;
	print $bottom_html;
?>
<h3>Tags</h3>
<div>
<?php
print $htmltags_html; 
?>
</div>

<?php if($bool_node_page):?>
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

<?php endif;?>
<?php
//ici unset des variables
unset($node);
unset($mainstory);
?>
