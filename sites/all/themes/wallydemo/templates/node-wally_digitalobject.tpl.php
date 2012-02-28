<?php

drupal_add_css(drupal_get_path('theme', 'wallydemo').'/css/article.css','file','screen');
$legende = "";
$digitalObject_summary = $node->field_summary[0]['value'];
if($digitalObject_summary != ""){
  $legende = "<p>".$digitalObject_summary."</p>";
}
$digitalObject_emcode = $node->field_object3rdparty[0]['view'];
$title = $node->title;
$date_edition = _wallydemo_date_edition_diplay($node->created, 'default');

?>
<div id="page_object" class="embed_digital">
<h1><?php print $title; ?></h1>
<p class="time"><?php print $date_edition; ?></p>
<?php print $digitalObject_emcode; ?>
<?php print $legende; ?>
</div>

<?php 
//ici unset des variables
unset($node);
?>