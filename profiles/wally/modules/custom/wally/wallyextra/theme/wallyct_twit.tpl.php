<?php

if (!function_exists('twit_feed')) {
  function twit_feed($feed) {
    $content = "";
    foreach ($feed as $k=>$item) {
      $content .= "<li>".check_plain($item->text)."</li>";
    }
    return $content; 
  }
}

if ($options['override_title']) {
  $title = t($options['override_title_text']);
} else {
  $title = t('Twitter reader');
}
?>

<h2><? print check_plain($title); ?></h2>
<ul>
<?php print twit_feed($feed); ?>
</ul>
