<?php print theme('imagecache', '70x70', $comment->picture,'', '');?>
<div class="comments-body">	
	<div class="meta">
		<span class="author"><?php print t('By')?> <?php print $author?></span>
		<time datetime="<?php print date('Y-m-d', $comment->timestamp);?>">
		  <?php print t(date('F', $comment->timestamp)).' '.date('j', $comment->timestamp).', '.date('Y', $comment->timestamp);?>
		</time>
	</div>
	<?php print $content;?>
	<div class="actions">
		<ul class="no-list">
			<li><a title="" href="/comment/reply/<?php print $node->nid.'/'.$comment->cid;?>"><i class="lsf">comment</i><?php print t('Reply');?></a></li>
			<li><a title="" href="/comment/edit/<?php print $comment->cid;?>"><span class="lsf">edit</span><?php print t('Edit');?></a></li>
			<li><a title="" href="/comment/delete/<?php print $comment->cid;?>"><span class="lsf">delete</span><?php print t('Delete');?></a></li>
		</ul>
	</div>
</div>