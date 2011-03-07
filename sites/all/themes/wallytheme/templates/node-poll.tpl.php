<?php
// $Id: node.tpl.php,v 1.5 2007/10/11 09:51:29 goba Exp $
// This template is used in node page and all polls page contexts


if (!$teaser){
	drupal_add_css(drupal_get_path('theme', 'custom_soirmag').'/css/article.css');
}


?>
<?php if(!$teaser) :?>
  <div id="article">
    <div class="inner">
<?php else : ?>
  <div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?>">
<?php endif; ?>
<?php print $picture ?>

  <div class="content clear-block "> 
    <?php print $content ?>
  </div>

  <div class="clear-block">
    <div class="meta">
    <?php if ($taxonomy): ?>
      <div class="terms"><?php print $terms ?></div>
    <?php endif;?>
    </div>
    
    <?php if(!$teaser) :?>
      <div id="fb-root"></div><script src="http://connect.facebook.net/fr_FR/all.js#appId=18373678023&amp;xfbml=1"></script><fb:comments xid="dev.soirmag.lesoir.be<?php echo $node->nid;?>" numposts="10" width="440" publish_feed="true"></fb:comments>
    </div>
    <?php endif;?>
  </div>
</div>
