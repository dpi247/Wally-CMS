<?php

if (!function_exists('rss_feed')) {
  function rss_feed($feed) {
    $content = "";
    foreach ($feed as $k=>$item) {
      $content .= "<li><a href='".$item['Package']['ExternalURI']['value']."'>".check_plain($item['Package']['MainStory']['Title']['value'])."</a></li>";
    }
    return $content; 
  }
}

if (isset($options['override_title_text'])) {
  $title = t($options['override_title_text']);
} else {
  $title = t('RSS reader');
}
?>
<div id="bloc-top-5" class="brique">
<h2><? print check_plain($title); ?></h2>
<ul>
<?php print rss_feed($feed); ?>
</ul>
</div>
