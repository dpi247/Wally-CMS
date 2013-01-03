<div class="article-main article-group">
<h2 class="main-title"><?php print t('Entity')?></h2>
<?php if (isset($node->field_main_picture[0]['filepath'])){?>
	<figure class="big-figure"><?php print theme('imagecache', '300x300', $node->field_main_picture[0]['filepath']);?></figure>
<?php }?>
<h2 class="big-title"><?php print $node->title?></h2>
<?php if ($node->field_objectdescription[0]['value'] != NULL){?>
<p><?php print $node->field_objectdescription[0]['safe'];?></p>
<?php }?>
</div> 