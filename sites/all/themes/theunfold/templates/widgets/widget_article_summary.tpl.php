<a href="<?php print $widget['url']; ?>">
  <?php print $widget['figure'];?>
  <span class="cat"><?php print $widget['barette']; ?></span>
  <h2>
	<?php print $widget['title'];?>
  </h2>
  <span class="meta">
	  <?php if ($widget['by'] != NULL){?>
  	    <span class="author"><?php print t('Published') ?> <time datetime="<?php print $widget['publicationdate'] ?>">
  		  <?php print date('d/m/Y', $widget['publicationdate']).' '.t('at').' '.date('H:i', $widget['publicationdate'])?></time> <?php print $widget['by']; ?>
  		</span>
      <?php } else { ?>
  	    <span class="author"><?php print t('Published') ?> <time datetime="<?php print $widget['publicationdate'] ?>">
  		  <?php print date('d/m/Y', $widget['publicationdate']).' '.t('at').' '.date('H:i', $widget['publicationdate'])?></time>
  		</span>
      <?php } ?>
      <span id="comment-count"><i class="icon-comment"></i><?php print $widget['comment_count']; ?>
      <?php if ($widget['mediaicons']!="") { print "| ".$widget['mediaicons']; } ?>
    </span>
</a>
<div class="description">
  <p><?php  print $widget['textchapo']; ?></p>
</div>
<?php print $widget['linklist']; ?>
  