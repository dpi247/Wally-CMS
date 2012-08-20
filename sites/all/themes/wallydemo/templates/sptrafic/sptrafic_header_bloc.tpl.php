<?php 
if (function_exists('sptrafic_getCachedData')) {
  $my_data = sptrafic_getCachedData('trafic_header_bloc');
  
?>
<li class="services widget_trafic"><a href="/services/trafic/">Trafic<br /><?php print $my_data['etat'] ; ?></a></li>
<?php 
//unset variables
unset($my_data);
}
?>