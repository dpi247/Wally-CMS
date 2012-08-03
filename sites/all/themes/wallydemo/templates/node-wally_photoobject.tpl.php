<?php 
drupal_add_css(drupal_get_path('theme', 'wallydemo').'/css/article.css','file','screen');
$photoObject_filename = $node->field_photofile[0]["filename"];
$photoObject_path = $node->field_photofile[0]["filepath"];
$copyright = "";
$photoObject_copyright = $node->field_copyright[0]['value'];
if($photoObject_copyright != ""){
  $copyright = "<div class=\"copyright\">".$photoObject_copyright."</div>";
}
$legende = "";
$photoObject_summary = $node->field_summary[0]['value'];
if($photoObject_summary != ""){
  $legende = "<p>".$photoObject_summary."</p>";
}
$title = $node->title;
$date_edition = _wallydemo_date_edition_diplay($node->created, 'default');
?>
  <div id="page_object" class="embed_photo">
    <h1><?php print $title; ?></h1>
    <p class="time"><?php print $date_edition; ?></p>
    <?php $photoObject_img = theme('imagecache', 'article_300x200', $photoObject_path, $photoObject_summary, $photoObject_summary);
  print $photoObject_img; ?>
    <?php print $copyright; ?>
    <?php print $legende; ?>
  </div>

<?php 
//ici unset des variables
unset($node);
?>