<h2 class="section-title">
	<?php if ($link != NULL){?>
	<a href="<?php print $link?>"><?php print $title?></a>
	<?php } else {?>
       <?php print $title;?> 
   <?php }?>
</h2>
<ul class="article-group">
  <?php foreach ($flow as $item){?>
  	<li class="article-inline small">
  	<?php if ($item['link'] != NULL){?><a href="<?php print $item['link']?>"><?php }?>
  	<?php if ($item['img'] != NULL){?>
  		  <figure>
  		  <?php print theme('imagecache', '120x120', $item['img'],'', '');?>
  		  </figure>
  		<?php }?>
  		<h2><?php print $item['title'];?></h2>
  	<?php if ($item['link'] != NULL){?></a><?php }?>
  	
    <?php if ($item['description'] != NULL && $item['description'] != ''){?>
    	<div class="description"><?php print $item['description']?></div> 
    <?php }?>
  	</li>
  <?php }?>
</ul>