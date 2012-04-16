<?php 
$class = "standard";
$target = "http://standard.sudpresse.be/";
$title = "standard.be";
?>
<div class="sitesrossel <?php print $class ; ?>">
  <h2><a target="_blank" href="<?php print $target; ?>"><?php print $title ; ?></a></h2>
   <ul class="clearfix">
    <?php print _custom_sudpresse_rss_crossmedia_gen($feed); ?>
  </ul>
</div>