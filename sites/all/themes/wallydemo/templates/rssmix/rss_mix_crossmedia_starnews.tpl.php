<?php 
$class = "starnews";
$target = "http://starnews.sudpresse.be/fr/";
$title = "starnews.be";
?>
<div class="sitesrossel <?php print $class ; ?>">
  <h2><a target="_blank" href="<?php print $target; ?>"><?php print $title ; ?></a></h2>
   <ul class="clearfix">
    <?php print _wallydemo_rss_crossmedia_gen($feed); ?>
  </ul>
</div>