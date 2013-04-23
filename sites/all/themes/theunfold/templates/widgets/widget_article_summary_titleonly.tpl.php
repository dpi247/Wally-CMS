  <a href="/<?php print $widget['url']; ?>">
   <h2>
	<?php print $widget['title'];?>
	<span class="meta">
	<?php if ($widget['by'] != NULL){?>
  	<span class="author"><?php print t('Published') ?> <time datetime="<?php print $widget['publicationdate'] ?>">
  		<?php print date('d/m/Y', $widget['publicationdate']).' '.t('at').' '.date('H:i', $widget['publicationdate'])?></time> <?php print $widget['by']; ?></span>
  <?php } else { ?>
  	<span class="author"><?php print t('Published') ?> <time datetime="<?php print $widget['publicationdate'] ?>">
  		<?php print date('d/m/Y', $widget['publicationdate']).' '.t('at').' '.date('H:i', $widget['publicationdate'])?></time></span>
  <?php } ?>
     </span>
   </h2>
  </a>  
