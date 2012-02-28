<?php 
drupal_add_css(drupal_get_path('theme', 'wallydemo').'/css/article.css','file','screen');
$textObject_copyright = "";
$textObject_copyright = $node->field_copyright[0]['value'];
if($textObject_copyright != ""){
  $copyright = "<div class=\"copyright\">".$textObject_copyright."</div>";
}
$chapeau = "";
$textObject_chapo = $node->field_textchapo[0]['value'];
if($textObject_chapo != ""){
  $chapeau = "<p class=\"chapeau\">".$textObject_chapo."</p>";
}
$body = "";
$body = $node->field_textbody[0]['value'];
$title = $node->title;
$date_edition = _wallydemo_date_edition_diplay($node->created, 'default');

?>
  <div id="page_object" class="embed_text">
    <h1><?php print $title; ?></h1>
    <?php print $chapeau; ?>
    <?php print $copyright; ?>
    <p class="time"><?php print $date_edition; ?></p>
    <?php print $body; ?>
  </div>

<?php 
//ici unset des variables
unset($node);
?>