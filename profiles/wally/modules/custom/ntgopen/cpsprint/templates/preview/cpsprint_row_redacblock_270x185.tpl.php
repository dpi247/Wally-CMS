<h1><span class="headline"><?php if ($mainstory->field_textbarette[0]['value'] != NULL){print $mainstory->field_textbarette[0]['value'].'.';}?></span> <a href="/node/<?php print $node->nid?>"><?php print $mainstory->title;?></a></h1>
<?php if ($mainstory->field_textchapo[0]['value'] != NULL){?>
  <h2 class="heading"><?php print $mainstory->field_textchapo[0]['value'];?></h2>
<?php }?>

<div class="article-image">
<?php if ($photoobject != NULL && $photoobject->field_photofile[0]['filename'] != NULL){
    $preset = 'print_376x215';
    print theme('imagecache', $preset, $photoobject->field_photofile[0]['filepath'], '', '');
    ?>  
<?php }?>

<?php if ($photoobject != NULL && $photoobject->field_summary[0]['value'] != NULL){?>
  <?php print $photoobject->field_summary[0]['value'];?>    
<?php }?>
</div>

<div class="article-body">
  <div class="article-description">
    <?php print $mainstory->field_textbody[0]['value'];?>
  </div>
</div>
    

