<div id="cross-sp" class="crossmedia">
  <h2><a target="_blank" href="http://www.sudpresse.be">Sudpresse.be</a></h2>
   <ul class="clearfix">
   <?php if (!function_exists('rss_feed')) { ?>
    <?php print _custom_soirmag_rss_crossmedia_gen($feed); ?>
   <?php } ?> 
  </ul>
</div>