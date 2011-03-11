<?php
/*
 *
 * 
 */


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
