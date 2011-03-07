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
 * - $picture: The authors picture of the node output from
 *   theme_user_picture().
 * - $date: Formatted creation date (use $created to reformat with
 *   format_date()).
 * - $links: Themed links like "Read more", "Add new comment", etc. output
 *   from theme_links().
 * - $name: Themed username of node author output from theme_username().
 * - $node_url: Direct url of the current node.
 * - $terms: the themed list of taxonomy term links output from theme_links().
 * - $submitted: themed submission information output from
 *   theme_node_submitted().
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type, i.e. story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Node status variables:
 * - $teaser: Flag for the teaser state.
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the
 *   main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 */
drupal_add_css(drupal_get_path('theme', 'custom_soirmag').'/css/article.css');
drupal_add_css(drupal_get_path('theme', 'custom_soirmag').'/css/galerie.css');
drupal_add_css(drupal_get_path('theme', 'custom_soirmag').'/css/prettyPhoto.css');

drupal_add_js(drupal_get_path('theme', 'custom_soirmag').'/scripts/comportement_base.js');
drupal_add_js(drupal_get_path('theme', 'custom_soirmag').'/scripts/cycle.js');
drupal_add_js(drupal_get_path('theme', 'custom_soirmag').'/scripts/jquery.prettyPhoto.js');
drupal_add_js(drupal_get_path('theme', 'custom_soirmag').'/scripts/comportement_article.js');

$main_story = $node->field_mainstory_nodes[0];
$main_title = $node->field_mainstory_nodes[0]->title;

$htmltags = custom_soirmag_taxonomy_tags_particle($main_story);

$jour = date('d',$node->changed);//jour en chiff'
$jour_sem = date('l', $node->changed);//Jour de la semaine : monday, ...
$heure =  date('H', $node->changed);
$minute = date('i', $node->changed);
$mois = date('F', $node->changed);
$annee =  date('Y', $node->changed);

$myterm=taxonomy_get_term($node->field_destinations[0]['tid']);
$strterm = (string)$myterm->name;
$niceterm = str_replace(" ", "_",strtolower($strterm));


$rssurl =   "/rss/".$niceterm."/rss.xml";

$themeroot = "/".drupal_get_path('theme', 'custom_soirmag')."/";

$mydate_format = t($jour_sem).", ".$jour." ".t($mois)." ".$annee.", ".$heure."<abbr title=\"heure\">h</abbr>".$minute; 


  
  $pictures = array();
  $videos = array();
  $audios = array();
  $embeds = array();
  
  $cpt=1;
   foreach ($node->field_embededobjects_nodes as $embedobject) {
   	if ($cpt==1){
   		$first_embed_type = $embedobject->type;
   	}
   	switch ($embedobject->type) {
   		case 'wally_photoobject':
   			$photo = array();
   			$photo["nid"] = $embedobject->nid;
   			$photo["type"] = $embedobject->type;
	  	 	$photo['credit'] = $embedobject->title;
	   		$photo['summary'] = $embedobject->field_summary[0]['value'];
	  	 	$photo['fullpath'] = $embedobject->field_photofile[0]['filepath'];
	  	 	
				$photo['main_size'] = theme('imagecache', 'sm_main_article', $embedobject->field_photofile[0]["filename"],$photo['credit']); 
				$photo['main_url'] = imagecache_create_url('sm_main_article', $photo['fullpath']);
				$photo['mini'] = theme('imagecache', 'sm_mini_article', $embedobject->field_photofile[0]["filename"],$photo['credit']); 
		   	
	   		$pictures[] = $photo;
	   		$embeds[] = $photo;
   		break;
   		case 'wally_videoobject':

   			$video = array();
   			$video["nid"] = $embedobject->nid;
   			$video["type"] = $embedobject->type;
   			$video['emcode'] = $embedobject->content['group_video']['group']['field_video3rdparty']['field']['items']['#children'];
   			$video['summary'] = $embedobject->field_summary[0]['value'];
   			if(!isset($embedobject->field_link[0]["title"])){
   				$embedobject->field_link[0]["title"] = "";
   			}
   			if(!isset($embedobject->field_link[0]["url"])){
   				$embedobject->field_link[0]["url"]="";
   			}
   			if(!isset($embedobject->field_link[0]['attributes'])){
   				$embedobject->field_link[0]['attributes'] = array();
   			}
   			$video['link'] = l($embedobject->field_link[0]["title"], $embedobject->field_link[0]["url"]);
   			$video['title'] = $embedobject->title;
				$video['thumbnail'] = $embedobject->field_video3rdparty[0]['data']['thumbnail']["url"];
			
   			$videos[] = $video;
   			$embeds[] = $video;
   		break;
   		case 'wally_audioobject':
   			$audio = array();
   			$audio["nid"] = $embedobject->nid;
   			$audio["type"] = $embedobject->type;
   			$audio['emcode'] = $embedobject->content['group_audio']['group']['field_audio3rdparty']['field']['items'][0]['#children'];
   			$audio['title'] = $embedobject->title;
   			$audio['summary'] = $embedobject->field_summary[0]['value'];
   			$audios[] = $audio;
   			$embeds[] = $audio;
   			
   		break;
   		default:
   			;
   		break;
   	}
   	$cpt++;
   	
   }
   
?>
<div id="article">
					<div class="addenda clearfix">
					<p><span>Liste des Tags :</span> 
					<?php 
					echo $htmltags;
					?>
						</p>
				 
						<a class="feed" href="<?php echo $rssurl; ?>" title="S'abonner au RSS de la section"><img src="<?php echo $themeroot;?>mediastore/elements/comment_rss_16.png" width="16" height="16" alt="RSS" /></a>
					</div>
					<div class="inner">
						<h1><?php echo $main_title; ?></h1>					
		       			<small><?php echo $mydate_format; ?></small>
					
<?php if($first_embed_type=='wally_videoobject'){?>
	
	<div id="galeria">
		<?php 
		echo $videos[0]['emcode'];
		$next_video++;	
		?>
		<div class="galeria-legend">
			<span class="video-title"><strong><?php print $videos[0]['title']; ?></strong></span>
			<span class="video-legend"><?php print $videos[0]['summary']; ?></span>
			<span class="video-link"><?php print $videos[0]['link']; ?></span>
		</div>						
	</div>
<?php }elseif($first_embed_type=='wally_photoobject'){?>					
					<div id="galeria">
						<ul>
				<?php foreach ($pictures as $photo) {
					//echo "<li><a href=\"/".$photo["fullpath"]."\">\n\t<img src=\"/".$photo["main_size"]."\" width=\"416\" height=\"200\" alt=\"".$photo["credit"]."\" /></a>\n</li>\n";
					echo "<li><a href=\"/".$photo["fullpath"]."\">\n\t".$photo['main_size']."</a>\n</li>\n";
				}

				?>
							</ul>
							<?php if(count($pictures) > 1){?>
							<div class="galeria-tools clearfix">
								<a id="btn-galeria-prev"><img src="<?php echo $themeroot;?>mediastore/elements/icons/rewind.gif" width="16" height="16" alt="Image précédente" /></a>

								<span class="count-item">images X de X</span>
								<a id="btn-galeria-next"><img src="<?php echo $themeroot;?>mediastore/elements/icons/fastforward.gif" width="16" height="16" alt="Image suivante" /></a>
							</div>
							<?php }?>
							<div class="galeria-legend">
							<?php echo $pictures[0]['credit']?>
							</div>
						</div>
<?php }elseif($first_embed_type=='wally_audioobject'){?>
				<div id="galeria">
		<?php 
		echo $audios[0]['emcode'];
		$next_audio++;
		?>
							
		</div>
	
<?php }?>						
						<big><?php echo $main_story->field_textchapo[0]["value"];?></big>
						<p><?php echo $main_story->field_textbody[0]["value"];?></p>
					
<?php if(count($embeds)>1){ ?>
	<div class="mini-galerie">
		<h2>Galerie&hellip;</h2>
			<ul class="gallery clearfix">
<?php foreach ($embeds as $myembed) {
	echo "<li><a href=\"#".$myembed['type'].$myembed['nid']."\"  rel=\"prettyPhoto[inline]\">";
	
	switch ($myembed["type"]) {
		case "wally_photoobject":
			echo $myembed["mini"];
		break;
		case "wally_videoobject":
			echo "<img src=\"".$myembed["thumbnail"]."\" width=\"56\" height=\"33\" alt=\"".$myembed["title"]."\"/>";
		break;
		case "wally_audioobject":
			echo "<img src=\"".$themeroot."mediastore/elements/thumb-sound.gif\" width=\"56\" height=\"33\" alt=\"".$myembed["title"]."\"/>";
		break;
		default:
			;
		break;
	}
	echo"</a></li>\n";
	
}?>				
			</ul>
	</div>	
<?php }?>
<?php if(count($embeds)>1){ ?>

<?php foreach ($embeds as $myembed) {
	
	switch ($myembed["type"]) {
		case "wally_photoobject":
			echo "<div id=\"".$myembed['type'].$myembed['nid']."\" style=\"display:none;\">";
			echo $myembed["main_size"];
			echo "<p>".$myembed["summary"]."</p>";
		break;
		case "wally_videoobject":
			echo "<div id=\"".$myembed['type'].$myembed['nid']."\" style=\"display:none; height:400px;\">";
			echo $myembed["emcode"];
			echo "<p>".$myembed["summary"]."</p>";
		break;
		case "wally_audioobject":
			echo "<div id=\"".$myembed['type'].$myembed['nid']."\" style=\"display:none;\">";
			echo $myembed["emcode"];
			echo "<p>".$myembed["summary"]."</p>";
		break;
		default:
			;
		break;
	}
	echo"</div>";
	
}?>				

<?php }?>

	<div class="share clearfix">
							<p>Partager par mail :</p>
							<ul>
								<li><a class="par-mail" title="Envoyer cette page par E-mail" href="javascript:void(0)"><img src="<?php echo $themeroot;?>mediastore/elements/icons/email.gif" alt="E-mail" width="16" height="16" /></a></li>
							</ul>

							<p>ou imprimez-le :</p>
							<ul>
								<li><a href="javascript:print();"><img src="<?php echo $themeroot;?>mediastore/elements/icons/printer.gif" alt="imprimer" width="10" height="10" /></a></li>
							</ul>
						</div>
						<div id="fb-root"></div><script src="http://connect.facebook.net/fr_FR/all.js#appId=18373678023&amp;xfbml=1"></script><fb:comments xid="dev.soirmag.lesoir.be<?php echo $node->nid;?>" numposts="10" width="440" publish_feed="true"></fb:comments>
</div>
</div>