<?php

if (!function_exists('rss_feed')) {
  function rss_feed($feed) {
    $content = "";
    foreach ($feed as $k=>$item) {
      $content .= "<li><a href='".$item['ExternalURI']['value']."'>".check_plain($item['MainStory']['Title']['value'])."</a></li>";
    }
    return $content; 
  }
}
print_r(debug_backtrace());
if (isset($options['override_title'])) {
  $title = t($options['override_title_text']);
} else {
  $title = t('Flow mix');
}
?>

<h2><? print check_plain($title); ?></h2>
<ul>
<?php print rss_feed($feed); ?>
</ul>
