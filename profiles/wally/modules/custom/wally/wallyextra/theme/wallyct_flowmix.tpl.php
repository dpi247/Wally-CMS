<?php

if (!function_exists('rss_feed')) {
  function rss_feed($feed) {
    $content = "";
    foreach ($feed as $k=>$item) {
      $content .= "<li><a href='".$item['ExternalURI']['value']."'>".check_plain($item['MainStory']['Title']['value'])."</a>";
      if(isset($item['MainStory']['TextBody']['value'])&& $item['MainStory']['TextBody']['value']!= ""){
      	$content .= "<span>".check_plain($item['MainStory']['TextBody']['value'])."</span>";
      }
      $content .= "</li>";
    }
    return $content; 
  }
}

if (isset($options['override_title'])) {
  $title = t($options['override_title_text']);
} else {
  $title = t('Flow mix');
}

?>

<div class="flowmix-bloc">
	<h2><? print check_plain($title); ?></h2>
	<div class="flowmix-list">
		<ul>
		<?php print rss_feed($feed); ?>
		</ul>
	</div>
</div>
