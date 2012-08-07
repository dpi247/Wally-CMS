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
drupal_add_css($themeroot . '/css/article.css');

// Give the index of the row into the view.
$themeroot = drupal_get_path('theme', 'wallydemo');

// Get the string name of the current domain
$domain_url = $_SERVER["SERVER_NAME"];
$domain = 'sudinfo';

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


/* 
 * Récupération du path
 */
$theme_path = drupal_get_path('theme', 'wallydemo');

/*  
 * Récupération de la date de publication du package -> $node_publi_date
 */
$node_publi_date = strtotime($node->field_publicationdate[0]['value']);

/* Affichage de la date au format souhaité
 * Les formats sont:
 * 
 * 'filinfo' -> '00h00'
 * 'unebis' -> 'jeudi 26 mai 2011, 15:54'
 * 'default' -> 'publié le 26/05 à 15h22'
 * 
 */ 
 
$date_edition = "<p class=\"publiele\">Publié le " ._wallydemo_date_edition_diplay($node_publi_date, 'date_jour_heure') ."</p>";
 

/*
* Récupération du mainstory
*/
$mainstory = $node->field_mainstory_nodes[0];

/* Récupération du chapeau de l'article -> $strapline
* Le nombre de caractères attendus pour ce chapeau est spécifié dans $strapline_length
* Si aucune limitation n'est attendue, laisser la valeur de $strapline_length à 0 *
*/

if ($mainstory->type == "wally_textobject"){
  node_build_content($mainstory, $teaser, $page);
  $strapline = $mainstory->field_textchapo[0]['safe'];
} else {
   $strapline = $mainstory->field_summary[0]['safe'];
}

$package_signature = _wallydemo_get_package_signature($mainstory);

$main_title = $mainstory->title;
$chapeau = "";
if (isset($strapline)){
  $chapeau = "<p class=\"chapeau\">" .$strapline ."</p>";
}
$texte_article = $mainstory->field_textbody[0]['safe'];
$signature = "<p class=\"auteur\">".$package_signature."</p>";

drupal_add_css($themeroot . '/css/article.css');


$nb_comment = $node->comment_count;
if ($nb_comment == 0){
  $reagir = "réagir";
} elseif ($nb_comment == 1){
  $reagir = $nb_comment."&nbsp;réaction";
} else {
  $reagir = $nb_comment."&nbsp;réactions";
}
/*
 * Génération des liens de partage
 */
$socialSharingBaseUrl = wallydemo_get_social_sharing_base_url($mainDestination, $domain);
if ($node_publi_date < 1333443600){
  $socialSharingDomainAndPathUrl = $socialSharingBaseUrl."/".$node_path;
} else {
  $socialSharingDomainAndPathUrl = $socialSharingBaseUrl."/node/".$node_id;
}
$fixedDomainAndPathUrl = "http://www.sudpresse.be/$node_path";
/**
 * Encre vers l'ours 
 */
$embed_package_anchor = '';
if (is_array($node->embed_package)){
  foreach ($node->embed_package as $emb_pack){
    $embed_package_class = '';
    switch ($emb_pack['package']->type){
      case 'wally_articlepackage': $embed_package_class = 'media-press';break;
      case 'wally_gallerypackage': $embed_package_class = 'media-photo';break;
      case 'wally_pollpackage': $embed_package_class = 'media-pool';break;
    }
    $embed_package_anchor .= '<p class="'.$embed_package_class.'"><span></span><a href="#anchor_'.$emb_pack['package']->nid.'">Lire aussi : '.$emb_pack['title'].'</a></p>';
  }
}

?>

<div id="article">
CCOCOCOCOCOOCOOCOCOCOCOOC
<?php echo $breadcrumb; ?>
  <ul class="liensutiles">
    <li class="envoyer"><?php print forward_modal_link("node/".$node->nid, wallydemo_check_plain($main_title), "<img src=\"/".$theme_path."/images/ico_envoyer2.gif\" alt=\"Envoyer à\" title=\"Envoyer à\" width=\"19\" height=\"16\" />"); ?></li>
    <li class="imprimer"><a href="javascript:window.print();"><img src="/<?php echo $theme_path; ?>/images/ico_imprimer2.gif" alt="Imprimer" title="Imprimer"  width="22" height="20" /></a></li>
    <li class="reagir"><a href="<?php print $node_path; ?>#ancre_commentaires"><?php print $reagir; ?></a></li>
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
      <div>
        <a href="http://twitter.com/share" class="twitter-share-button"
          data-url="<?php print $socialSharingDomainAndPathUrl; ?>"
     	  data-via="sudpresseonline"
     	  data-text="<?php print str_replace('"', '', $main_title); ?>"
     	  data-related="lameuse.be:Toute l'information du La Meuse,LaGazette_be:Toute l'information de La Nouvelle Gazette,xalambert:Responsable de la rédaction de Sudpresse.be"
     	  data-count="horizontal"      
     	  data-lang="fr">Tweet</a></div>
    </li>
    <li class="google">
      <div class="g-plusone" data-size="medium" data-href="<?php print $socialSharingDomainAndPathUrl; ?>"></div>
    </li>
  </ul>
  <h1><?php print wallydemo_check_plain($main_title); ?></h1>
  <div id="picture"> 
    <?php print $mediabox_html; ?>
    <?php print '<div id = "link">'.$linkslist_html.'</div>'; ?> 
  </div>
  <?php print $chapeau; 
        print $signature; 
        print $embed_package_anchor;
        print $date_edition; 
        print $texte_article; 
        print $htmltags_html; 
        print $bottom_html; ?>
  
  <?php if (is_array($node->embed_package)){
    foreach ($node->embed_package as $emb_pack){
  
    //Embed Gallery
    if ($emb_pack['package']->type == 'wally_articlepackage'){?>
      <div id="anchor_<?php print $emb_pack['package']->nid;?>" class="emb_package emb_package_article emb_package_bear">
        <div class="ico_pic"></div>
        <h2><?php print $emb_pack['title'];?></h2>
        <?php if ($emb_pack['photo_object'][0]):?>
          <div class="emb_package_bpic">
            <?php print theme('imagecache', 'une_manchette_217x145', $emb_pack['photo_object'][0]->field_photofile[0]['filepath'], $emb_pack['photo_object'][0]->title, $emb_pack['photo_object'][0]->title);?>
            <p class="pic_description"><?php print $emb_pack['photo_object'][0]->title;?></p>
            <p class="credit"><span><?php print $emb_pack['photo_object'][0]->field_copyright[0]['value'];?></span></p>
          </div>
        <?php endif;?>
        <div class="emb_package_text"><?php print $emb_pack['text'];?></div>
      </div>
    <?php } elseif ($emb_pack['package']->type == 'wally_gallerypackage'){
      drupal_add_js(drupal_get_path('theme', 'wallydemo').'/scripts/script-gallery.js');?>
      <div id="anchor_<?php print $emb_pack['package']->nid;?>" class="emb_package emb_package_article emb_package_gallery">
      <div class="ico_pic"><span>Galerie photo. </span></div>
         
            <h2><?php print $emb_pack['title'];?></h2>
            <div class="emb_package_text"><?php print $emb_pack['text'] ; ?></div>
          <?php 
            $count = 0;
            $class = '';
            $images = '';
            $thumbnails = '';
            foreach ($emb_pack['photo_object'] as $img){
              if ($count > 0){
                $class = 'hidden';
              }
              $images .= '<div class="emb_package_bpic '.$class.'" id="'.$emb_pack['package']->nid.$img->nid.'">';
              $images .= theme('imagecache', 'pagallery_450x300', $img->field_photofile[0]['filepath'], $img->title, $img->title);
              $images .= '<p class="pic_description">'.$img->title.'</p>';
              $images .= '<p class="credit"><span>'.$img->field_copyright[0]['value'].'</span></p>';
              $images .= '</div>';
              $thumbnails .= '<li id="'.$emb_pack['package']->nid.$img->nid.'">'.theme('imagecache', 'divers_120x80', $img->field_photofile[0]['filepath'], $img->title, $img->title).'</li>';
              $count++;
              if ($count > 9){
                break;
              }
            } 
          ?>
       	<?php print $images;?>
        <div class="emb_package_thumbs">
          <ul>
          	<?php print $thumbnails;?>
          </ul>
        </div>
      </div>
    <?php } elseif ($emb_pack['package']->type == 'wally_pollpackage'){//embed Poll?>
      <div id="anchor_<?php print $emb_pack['package']->nid;?>" class="emb_package emb_package_article emb_package_poll">
        <div class="ico_pic"><span>Sondage. </span></div>
        <h2><?php print $emb_pack['title'];?></h2>
        <div class="emb_package_thumbs">
          <ul>
            <?php 
            foreach ($emb_pack['photo_object'] as $img){
              print '<li>'.theme('imagecache', 'divers_201x134', $img->field_photofile[0]['filepath'], $img->title, $img->title).'</li>';
            }?>
          </ul>
        </div>
        <div class="emb_package_text"><?php print $emb_pack['text'] ; ?></div>
        <?php print node_view($emb_pack['mainobject']);?>
      </div>
    <?php }?>
  <?php }
  }?>
  
  <!-- FACEBOOK REACTIONS -->
  <?php if ($node->comment == 2) { ?>
    <a id="ancre_commentaires" name="ancre_commentaires" href="#ancre_commentaires" /></a>
    <?php print theme("spreactions_facebook", $node_id, $socialSharingBaseUrl, $socialSharingDomainAndPathUrl, $socialSharingDomainAndPathUrl); ?>
  <?php } ?>
  
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

?>
