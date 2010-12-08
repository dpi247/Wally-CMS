<?php

if (!function_exists('rss_feed')) {
  function rss_feed($feed) {
 
    $content = "";
    foreach ($feed as $k=>$item) {
      $content .= "<li><a href='".$item['link']."'>".$item['title']."</a></li>";
    }
    return $content; 
  }
}

if ($options['override_title']) {
    $title = t($options['override_title_text']);
  } else {
    $title = t($options['taxonomyfield']);
  }

?>
  <h2><? print $title; ?></h2>
  <ul>
  <?php print rss_feed($feed); ?>
  </ul>
