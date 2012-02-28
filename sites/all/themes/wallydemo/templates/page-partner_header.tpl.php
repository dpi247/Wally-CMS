<?php 
  $tTheme = theme("sp_logo");
  $menu = wallydemo_menu_get_cache("menu-selectboxeditions");
  $tTheme .= theme("sp_regions_date",$menu);
  $tTheme .= theme("sp_recherche_menu");
  $tTheme .= theme("sp_carburant");
  $tTheme .= theme("sp_trafic");
  $tTheme .= theme("sp_meteo");   	
  $tTheme =  str_replace('href="/', 'href="http://'.$_SERVER['SERVER_NAME'].'/', $tTheme);
  $tTheme =  str_replace('src="/', 'src="http://'.$_SERVER['SERVER_NAME'].'/', $tTheme);
  
?>
<div id="top_header">
	<ul>		
		<?php print $tTheme ?>
	</ul>
</div>	