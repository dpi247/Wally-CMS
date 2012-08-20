<?php
/**
 * 
 * Render the html for the header
 * @param $xml
 */
if (function_exists('spcarburant_getCachedData')) :
$my_data = spcarburant_getCachedData("carburant_header_bloc"); 
//dsm($my_data);
?>
<li class="services widget_carburant">
 <a href="/services/carburant/">
  <?php print($my_data['price']); ?>
  <br>
  <?php print($my_data['legende']); ?>
 </a>
</li>
<?php endif; ?>
