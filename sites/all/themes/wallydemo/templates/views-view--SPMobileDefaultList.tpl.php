  <?php 
    //print_r($variables); 
    $tid  = $variables["view"]->args[0];
    $dest = taxonomy_get_term($tid);
    $destName = $dest->name; 
	
	
	
  ?>
<h1 id="h1_taxonomy"><span><?php print $destName; ?></span></h1>  

<div id="liste_articles">  
    
    <?php if ($admin_links): ?>
      <div class="views-admin-links views-hide">
        <?php print $admin_links; ?>
      </div>
    <?php endif; ?>
  
    <?php if ($header): ?>
      <div class="view-header">
        <?php print $header; ?>
      </div>
    <?php endif; ?>
     
    <?php if ($exposed): ?>
      <div class="view-filters">
        <?php print $exposed; ?>
      </div>
    <?php endif; ?>
  
    <?php if ($attachment_before): ?>
      <div class="attachment attachment-before">
        <?php print $attachment_before; ?>
      </div>
    <?php endif; ?>

    <?php if ($rows): ?>
     <?php if ($tid == 20) {?> 
     <div id="filinfo">
	 <?php print $rows; ?></div><?php } 
	   else { print $rows;}?>
    <?php elseif ($empty): ?>
      <div class="view-empty">
        <?php print $empty; ?>
      </div>
    <?php endif; ?>
  
    <?php if ($attachment_after): ?>
      <div class="attachment attachment-after">
        <?php print $attachment_after; ?>
      </div>
    <?php endif; ?>
  
    <?php if ($more): ?>
      <?php print $more; ?>
    <?php endif; ?>
  
    <?php if ($footer): ?>
      <div class="view-footer">
        <?php print $footer; ?>
      </div>
    <?php endif; ?>
  
    <?php if ($feed_icon): ?>
      <div class="feed">
        <?php print $feed_icon; ?>
      </div>
    <?php endif; ?>
 </div>
 <div class="pagin"><?php print _wallydemo_pager('',10,0,array('tid'=>$tid),9);?></div>
 