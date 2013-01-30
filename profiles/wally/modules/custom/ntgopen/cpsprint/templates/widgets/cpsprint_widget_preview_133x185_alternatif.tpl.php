<h1><a href="/node/<?php print $nid?>"><?php print $title;?></a></h1>
<?php if ($textchapo != NULL){?>
  <h2 class="heading"><?php print $textchapo;?></h2>
<?php }?>
<div class="article-image">
<?php if ($photoobject != NULL && $photoobject->field_photofile[0]['filename'] != NULL){
    $preset = 'print_182x176';
    print theme('imagecache', $preset, $photoobject->field_photofile[0]['filepath'], '', '');
    ?>  
<?php }?>

<?php if ($photoobject != NULL && $photoobject->field_summary[0]['value'] != NULL){?>
  <?php print $photoobject->field_summary[0]['value'];?>    
<?php }?>
</div>

<div class="article-body">
  <div class="article-description">
    <?php print $textbody;?>
  </div>
</div>
    
