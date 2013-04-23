<h1><?php print $widget['title'];?></h1>

<h2 class="heading"><?php print $widget['summary'];?></h2>
<figure><?php print $widget['media'];?></figure>
<div class="meta">
		
	<span class="author"><?php print t('Published') ?> 
	 <?php print $widget['by']; ?>
  	</span>
  	<span class="date">
  	<time datetime="<?php print $widget['publicationdate'] ?>">
  	  <?php print date('d/m/Y', $widget['publicationdate']).' '.t('at').' '.date('H:i', $widget['publicationdate'])?>
  	</time>
  	</span>

</div>
