<? //print theme("sp_leaderboard"); ?>
<div id="top_header">
	<ul>		
		<? print theme("sp_logo"); ?>
       	<? 
       	$menu = wallydemo_menu_get_cache("menu-selectboxeditions");
       	print theme("sp_regions_date",$menu); ?>
      	<? print theme("sp_recherche_menu"); ?>
	</ul>
</div>	
<? /* print theme("sp_menu"); */?>
