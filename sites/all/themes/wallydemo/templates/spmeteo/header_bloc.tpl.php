<?php
  $my_data = spmeteo_getCachedData('meteo_header_bloc');
  $imgFolder = $my_data['images_folder'];
  $logo = array('Ciel serein','Ciel peu nuageux','Ciel très nuageux','Couvert','Brouillard','Pluie','Bruine','Neige','Pluie et neige','Pluie et bruine','Averses de pluie','Averses orageuses','Giboulées','Averse de neige','Averses de pluie et neige','Orage','Orageux','Verglas'); 
?>
<li class="services widget_meteo"> 
	<img src="<?php print $imgFolder; ?>/meteo_little/<?php print $my_data["temp_symbole"] ?>.gif" alt="<?php print $logo[($my_data["temp_symbole"])-1] ?>" width="20" height="20" /> 
    <a href="<?php echo drupal_get_path_alias("/meteo")?>"><?php print $logo[($my_data["temp_symbole"])-1] ?><br />

    	<span class="temp_min"><?php print $my_data["temp_min"] ?></span> / <span class="temp_max"><?php print $my_data["temp_max"] ?></span>
    </a> 
</li>
<?php 
//ici unset variables
unset($my_data);
?>