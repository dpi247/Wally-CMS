<div id="une_bis" class="<?php print $destName; ?>">
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
