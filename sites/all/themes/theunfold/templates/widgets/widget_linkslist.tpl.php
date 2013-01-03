<h2 class="section-title">
	<?php if ($link != NULL){?>
	<a href="<?php print $widget['link']?>"><?php print $widget['title']?></a>
	<?php } else {?>
       <?php print $widget['title'];?> 
   <?php }?>
</h2>
<ul class="article-list">
<?php 
  if (count($widget['links']) > 0){
    foreach ($widget['links'] as $item){?>
	  <li>
	  <?php if ($item['link'] != NULL){?>
		<a href="<?php print $item['link']?>"><?php print $item['title'];?></a></li>
      <?php } else {?>
        <?php print $item['title'];?> 
      <?php }?> 
    <?php }?>
  <?php }?>
</ul>