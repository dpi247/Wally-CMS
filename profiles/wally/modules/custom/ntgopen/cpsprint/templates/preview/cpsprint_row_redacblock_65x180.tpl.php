<h1><a href="/node/<?php print $node->nid?>"><?php print $mainstory->title;?></a></h1>

<div class="article-body">
  <div class="article-description">
    <?php print $mainstory->field_textbody[0]['value'];?>
  </div>
</div>
  
<?php if ($photoobject != NULL && $photoobject->field_photofile[0]['filename'] != NULL){
    $preset = 'print_182x176';
    print theme('imagecache', $preset, $photoobject->field_photofile[0]['filepath'], '', '');
    ?>
    
<?php }?>

<?php if ($photoobject != NULL && $photoobject->field_summary[0]['value'] != NULL){?>
  <?php print $photoobject->field_summary[0]['value'];?>    
<?php }?>