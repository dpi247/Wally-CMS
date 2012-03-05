<?php
drupal_add_css(drupal_get_path('theme', 'wallydemo').'/css/article.css','file','screen');
$legende = "";
$videoObject_summary = $node->field_summary[0]['value'];
if($videoObject_summary != ""){
  $legende = "<p>".$videoObject_summary."</p>";
}
$videoObject_emcode = $node->field_video3rdparty[0]['view'];
$copyright = "";
$videoObject_copyright = $node->field_copyright[0]['value'];
if($videoObject_copyright != ""){
  $copyright = "<div class=\"copyright\">".$videoObject_copyright."</div>";
}
$title = $node->title;
$date_edition = _wallydemo_date_edition_diplay($node->created, 'default');

?>
<div id="page_object" class="embed_video">
<h1><?php print $title; ?></h1>
<p class="time"><?php print $date_edition; ?></p>
<?php print $videoObject_emcode; ?>
<?php print $copyright; ?>
<?php print $legende; ?>
</div>

<?php 
//ici unset des variables
unset($node);
?>