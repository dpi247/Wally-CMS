<h1><a href="/node/<?php print $nid?>"><?php print $title;?></a></h1>

<div class="article-image">
<?php if ($photoobject != NULL && $photoobject->field_photofile[0]['filename'] != NULL){
    $preset = 'print_182x192';
    print theme('imagecache', $preset, $photoobject->field_photofile[0]['filepath'], '', '');
    ?>  
<?php }?>

<?php if ($photoobject != NULL && $photoobject->field_summary[0]['value'] != NULL){?>
  <?php print $photoobject->field_summary[0]['value'];?>    
<?php }?>
</div>

<div class="article-body int250">
  <div class="article-description">
    <?php print $textbody;?>
  </div>
</div>
    
