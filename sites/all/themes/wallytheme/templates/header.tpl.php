<?php
/*
 * arg: $node en cas de contexte de noeud.
 */
?>
	<div class="container_12 clearfix">
				
		<? print theme("soirmag_header_logo", $data_array); ?>
						
		<div id="social">
		<? print theme("soirmag_header_social_bookmarks", $data_array); ?>
		</div>
		
    <div id="meteo">    
    <? print theme("soirmag_header_meteo", $data_array); ?>
    </div>  		
	</div>