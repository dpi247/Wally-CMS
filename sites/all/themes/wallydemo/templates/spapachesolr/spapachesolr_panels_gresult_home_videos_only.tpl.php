<?php 
// $results; $from; $to

//dsm($result, 'VIDEO');

if (count($results)>0) {
print "<div class=\"search_galerie\"><div class=\"galerie_video result\"><ul>";
	foreach($results as $result) {
		//if ($nbr>=$from && $nbr<$to) {
			// display the result
			print theme("spapachesolr_panels_gresult_item_video_only", $result, "default"); 
		//}
		$nbr++;
	}
	print "</ul></div></div>";
}
//ici unset variables
unset($results);
?>