<?php 
// $results; $from; $to

//dsm($result, 'VIDEO');

if (count($results)>0) {
	print "<div class=\"search_galerie\"><div class=\"galerie_video\">digital-01";
	foreach($results as $result) {
		if ($nbr>=$from && $nbr<$to) {
			// display the result
			print theme("spapachesolr_panels_gresult_item_digital", $result, "default"); 
		}
		$nbr++;
	}
if (count($results)>9) {
	print "<div class=\"plusdesearch\"><a href=\"#\" class=\"arrow novisited\">Plus de vidéos</a></div>";
	}
print "</div></div>";
}
//ici unset variables
unset($results);
?>