<?php 
    $tid  = $variables["view"]->args[0];
    $dest = taxonomy_get_term($tid);
    $parents = array_reverse(taxonomy_get_parents_all($tid));
    $rssurl =   "/feed";    
    foreach ($parents as $p) {
      $rssurl .= "/".$p->name;
    }
    $destName = $dest->name;   
    $path = "/".drupal_get_path("theme", "wallydemo")."/images/";
    $breadcrumb = _wallydemo_breadcrumb_display($tid);

    //dsm($variables["view"],"vzerzerzer");

    ?>
<div id="une_bis" class="<?php print $destName; ?>">
  <div class="titre_une_bis">
    <?php echo $breadcrumb; ?> 
    <div class="right feed"> <a title="Télécharger le flux de syndication des régions" href="<?php print $rssurl?>"> <img width="12" height="12" alt="RSS" src="<?php print $path; ?>feed.png"> </a></div> 
  <div class="right"><?php //print _wallydemo_pager('',10,0,array('tid'=>$tid),9);?> </div>
  </div>
  <?php if ($admin_links): ?>
  <div class="views-admin-links views-hide"> <?php print $admin_links; ?> </div>
  <?php endif; ?>
  <?php if ($header): ?>
  <div class="view-header"> <?php print $header; ?> </div>
  <?php endif; ?>
  <?php if ($exposed): ?>
  <div class="view-filters"> <?php print $exposed; ?> </div>
  <?php endif; ?>
  <?php if ($attachment_before): ?>
  <div class="attachment attachment-before"> <?php print $attachment_before; ?> </div>
  <?php endif; ?>
  <?php if ($rows): ?>
  <h1><span><?php print $destName; ?></span></h1>
  <?php print $rows; ?>
  <?php elseif ($empty): ?>
  <div class="view-empty"> <?php print $empty; ?> </div>
  <?php endif; ?>
  <?php if ($attachment_after): ?>
  <div class="attachment attachment-after"> <?php print $attachment_after; ?> </div>
  <?php endif; ?>
  <?php if ($more): ?>
  <?php print $more; ?>
  <?php endif; ?>
  <?php if ($footer): ?>
  <div class="view-footer"> <?php print $footer; ?> </div>
  <?php endif; ?>
  <?php if ($feed_icon): ?>
  <div class="feed"> <?php print $feed_icon; ?> </div>
  <?php endif; ?>
</div>
