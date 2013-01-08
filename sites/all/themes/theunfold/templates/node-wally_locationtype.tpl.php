

<div class="article-main article-group">
  <h2 class="main-title"><?php print t('Location')?></h2>
  <?php if ($node->field_address[0]['latitude'] != NULL && $node->field_address[0]['longitude']){?>
 	<figure class="big-figure">
      <?php print theunfold_get_map_fromcoordinate($node->field_address[0]['latitude'], $node->field_address[0]['longitude'], '100%', 256);?>
	</figure>
  <?php }?>
  <h2 class="big-title"><?php print $node->title?></h2>
  <?php if ($node->field_objectdescription[0]['value'] != NULL){?>
    <p><?php print $node->field_objectdescription[0]['safe'];?></p>
  <?php }?>
  
  <?php if ($node->field_address[0]['lid'] != 0){
    
    print theme('widget_address', $node->field_address[0]);
  }?>
  
</div> 