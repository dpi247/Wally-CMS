<div class="search">
<?php

if($nbitem > 0) { 
  if ($nbitem == 1) print '<div id="nbeleme">1 article trouvé</div>';
  else print '<div id="nbeleme">'.$nbitem.' articles trouvés</div>';
}


foreach($results as $result) {
	if ($nbr>=$from && $nbr<$to) {
		// display the result
		switch ($result["wallynode"][0]->type) {
			case "wally_photoobject":
				print theme("spapachesolr_panels_gresult_item_photo", $result, "line"); 
				break;
			case "wally_videoobject":
				print theme("spapachesolr_panels_gresult_item_video", $result, "line"); 
				break;				
			case "wally_articlepackage":
				print theme("spapachesolr_panels_gresult_item_text", $result, "line"); 
				break;
			case "wally_digitalobject":
				print theme("spapachesolr_panels_gresult_item_digital", $result, "line"); 
				break;
		/*	default:
				print "autres ... a faire ...";
				break;   */
			}
            //dsm($result["wallynode"][0]->type);
	}
	$nbr++;
}
//ici unset variables
unset($results);
?>
</div>
