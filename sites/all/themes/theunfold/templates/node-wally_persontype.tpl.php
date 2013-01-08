<div class="article-main article-group">
  <h2 class="main-title"><?php print t('Person')?></h2>
  <?php if (isset($node->field_photofile[0]['filepath'])){?>
	<figure class="big-figure"><?php print theme('imagecache', '300x300', $node->field_photofile[0]['filepath']);?></figure>
  <?php }?>
  <h2 class="big-title"><?php print $node->title?></h2>
  <?php if ($node->field_summary[0]['safe'] != NULL){?>
    <p><?php print $node->field_summary[0]['safe'];?></p>
  <?php }?>
  <ul class="article-list">
    <?php if ($node->field_personfirstname[0]['safe'] != NULL):?>
	  <li><span class="title"><?php print t('FirstName')?></span> : <span class="description"><?php print $node->field_personfirstname[0]['safe'];?></span></li>
    <?php endif;?>
	<?php if ($node->field_personlastname[0]['safe'] != NULL):?>
	  <li><span class="title"><?php print t('LastName')?></span> : <span class="description"><?php print $node->field_personlastname[0]['safe'];?></span></li>
    <?php endif;?>
    <?php if ($node->field_personwebsite[0]['url'] != NULL):?>
      <?php if ($node->field_personwebsite[0]['display_title'] == NULL){
        $node->field_personwebsite[0]['display_title'] = $node->field_personwebsite[0]['display_url'];
      }?>
	  <li><span class="title"><?php print t('Web Site')?></span> : <span class="description"><a href="<?php print $node->field_personwebsite[0]['url']?>"><?php print $node->field_personwebsite[0]['display_title'];?></a></span></li>
    <?php endif;?>
  </ul>
</div> 