<?php 

// $results; $from; $to
if (count($results)>0) {
print "<div class=\"search_galerie\"><div class=\"galerie_photo\">";
	foreach($results as $result) {
	if ($nbr>=$from && $nbr<$to) {
		// display the result
		print theme("spapachesolr_panels_gresult_item_photo", $result, "main_size"); 
	}
	$nbr++;
}
if (count($results)>9) {
	print "<div class=\"plusdesearch\"><a href=\"".$_SERVER['SCRIPT_URI']."?filters=type%3Awally_photoobject\" class=\"arrow novisited\">Plus de photos</a></div>";
	}
print "</div></div>";
}
//ici unset variables
unset($results);
