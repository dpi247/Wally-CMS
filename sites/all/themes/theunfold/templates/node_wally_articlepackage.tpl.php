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
<?php print $top_html;?>
<div class="article-body">
	<?php if ($related_html != NULL){?>
	<aside class="hidden-phone" role="complementary">
		<?php print $related_html;?>
	</aside>
	<?php }?>
	<div class="article-description">
	<?php print $body;?>
	</div>
</div>
<?php print $bottom_html;?>