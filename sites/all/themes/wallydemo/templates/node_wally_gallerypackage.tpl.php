<?php

drupal_add_css(drupal_get_path('theme', 'wallydemo').'/css/article.css','file','screen');
drupal_add_js(drupal_get_path('theme', 'wallydemo').'/scripts/jquery.scrollTo-min.js');
drupal_add_js(drupal_get_path('theme', 'wallydemo').'/scripts/jquery.localscroll-min.js');
drupal_add_js(drupal_get_path('theme', 'wallydemo').'/scripts/script-article.js');
drupal_add_js(drupal_get_path('theme', 'wallydemo').'/scripts/script-gallery.js');

$mainobject = $node->field_mainobject_nodes[0];
$main_title = $node->title;

$themeroot = drupal_get_path('theme', 'wallydemo');
$embededobjects = $node->field_embededobjects_nodes;
array_unshift($embededobjects, $node->field_mainobject_nodes[0]);
$sorted_embededobjects = wallycontenttypes_sort_embededobjects($embededobjects);
$imgstory = $sorted_embededobjects['wally_photoobject'];
$videostory = $sorted_embededobjects['wally_videoobject'];
$destination_term = theme("wallyct_destinationlist", $node->field_destinations, " | " , "", "");
$main_summary = $node->field_summary[0]['safe'];

if(isset($node->field_editorialupdatedate[0]['safe']) && !empty($node->field_editorialupdatedate[0]['safe'])) {
  $field_editorialupdatedate=$node->field_editorialupdatedate[0]['safe'];
  $editorialupdatedate = strtotime($field_editorialupdatedate);
} else {
  $field_editorialupdatedate=FALSE;
}

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
$date_edition = "<p class=\"publiele\">Publié le " ._wallydemo_date_edition_diplay($node_publi_date, 'date_jour_heure');
$date_edition .= $field_editorialupdatedate ? " (mis à jour le " ._custom_sudpresse_date_edition_diplay($editorialupdatedate, 'date_jour_heure').")" : "";
$date_edition .= "</p>";

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

?>
<div id="article" class="gallery"><?php echo $breadcrumb;?>
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
  <div class="emb_package emb_package_gallery">
    <div class="ico_pic"><span>Galerie photo. </span><?php print $date_edition;?></div>
    
      <h1><?php print $main_title;?></h1>
      <div class="emb_package_text"><?php print $embedtext_html; ?></div>
    <?php 

      $count = 0;
      $class = '';
      foreach ($imgstory as $img){
        $caption = $img->field_summary[0]['safe'];
        if ($count > 0){
          $class = 'hidden';
        }
        $images .= '<div class="emb_package_bpic '.$class.'" id="'.$img->nid.'">';
        $images .= theme('imagecache', 'pagallery_450x300', $img->field_photofile[0]['filepath'], $caption, $caption);
        $images .= '<p class="pic_description">'.$caption.'</p>';
        $images .= '<p class="credit"><span>'.$img->field_copyright[0]['safe'].'</span></p>';
        $images .= '</div>';
        $thumbnails .= '<li id="'.$img->nid.'">'.theme('imagecache', 'divers_120x80', $img->field_photofile[0]['filepath'], $caption, $caption).'</li>';
        $count++;
        /*if ($count > 9){
          break;
        }*/
      } 
    ?>
 	<?php print $images;?>
  	<div class="emb_package_thumbs">
      <ul>
    	<?php print $thumbnails;?>
      </ul>
  	</div>
  </div>
  
	  

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

  
  
  