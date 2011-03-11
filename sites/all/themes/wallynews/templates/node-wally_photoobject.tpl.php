<?php
/*
 *
 * 
 */

//$destination_term = theme("wallyct_destinationlist", $node->field_destinations, " | " , "", "");
$presetname='gallery_preset';
$file_path = $node->field_photofile[0]['filepath'];
$alt = $node->field_summary[0]['value'];
$image = theme('imagecache', $presetname,  $file_path, $alt);
$path_photo = imagecache_create_url($presetname, $file_path);
$title_img = $node->title;
$summary= $node->field_summary[0]['value'];

?>
<li class="thumb">
  <a href=" <?php print $path_photo; ?> "><?php print $image; ?></a>
  <div class="block">
    <h2><?php print $title_img; ?></h2>
    <div class="date">
      Publié le <?php print date('d M Y', $node->created) ?>
      <span> <?php //print $destination_term; ?></span>
    </div>
    <p> <?php print $summary ?></p>
  </div>
</li>
