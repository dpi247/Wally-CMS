<? //print theme("sp_leaderboard"); ?>
<div id="top_header">
	<ul>		
		<? print theme("sp_logo"); ?>
       	<? 
       	$menu = wallydemo_menu_get_cache("menu-selectboxeditions");
       	print theme("sp_regions_date",$menu); ?>
      	<? print theme("sp_recherche_menu"); ?>
      	<? print theme("sp_carburant"); ?>
		<? print theme("sp_trafic"); ?>
		<? print theme("sp_meteo"); ?>
	</ul>
</div>	
<? /* print theme("sp_menu"); */?>
