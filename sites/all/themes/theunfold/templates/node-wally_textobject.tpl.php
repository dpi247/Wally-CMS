<span class="cat"><?php print $barette?></span>
<h1><?php print $title;?></h1>
<div class="meta">
	<?php if ($by != NULL){?>
  	<p class="author"><?php print $by?></p>
	<?php }?>
	<p class="date">
		<?php print t('Published') ?> <time datetime="<?php print $publicationdate?>">
		<?php print date("d/m/Y ", $publicationdate).t('at').date(" H:i", $publicationdate)?></time> |
		<span id="comment-count"><a href="#comments-container" class="comment-count-link"><i class="lsf">comment</i><?php print $comment_count; ?></a></span>
	</p>
</div>
<h2 class="heading"><?php print $textchapo;?></h2>

<div class="article-body">
	<div class="article-description">
	<?php print $body;?>
	</div>
</div>