<?php 

$item['titre'] = $feed[0]['MainStory']['Title']['value'];
$item['image_path'] = $feed[0]['EmbeddedContent']['EmbeddedObjects']['Object'][0]['LocaleImage']['filepath'];
$item['url'] = $feed[0]['ExternalURI']['value'];

if ($item['image_path'] == "") {
  $item['image_path'] = "/sites/all/themes/wallydemo/images/default_pic_last-vids.gif"; 
}

// transformation d'une image "source" en thumbnail via image cache.
// theme('imagecache', 'divers_78x60', $item['Package']['EmbeddedContent']['EmbeddedObjects']['Object'][0]['LocaleImage']['filepath']);
?>
<div class="bloc-01 en-media">
	<h2><a href="<?php print $item['url'] ; ?>">Image du jour</a></h2>
	<a href="<?php print $item['url'] ; ?>">
		<?php print theme('imagecache', 'article_300x200', $item["image_path"], $item['titre'], $item['titre']) ?>
	</a>
	<span><?php print $item['titre'] ; ?></span>
</div>