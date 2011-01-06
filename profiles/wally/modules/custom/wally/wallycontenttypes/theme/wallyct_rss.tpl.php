<?php

if (!function_exists('rssmix_feed')) {
  function rssmix_feed($feed) {
    $content = "";
    foreach ($feed as $k=>$item) {
      $content .= "<li><a href='".$item['Package']['ExternalURI']['value']."'>".$item['Package']['MainStory']['Title']['value']."</a></li>";
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
<?php print rssmix_feed($feed); ?>
</ul>
