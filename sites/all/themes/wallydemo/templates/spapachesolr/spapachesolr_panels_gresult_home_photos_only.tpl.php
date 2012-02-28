<?php 
// $results; $from; $to; $nbitem
/*dsm('passe','passe');
dsm($results, 'results');
dsm($to,'to');
dsm($from,'from');*/

if($nbitem > 0) { 
  if ($nbitem == 1) $nbitems = '<div id="nbeleme">1 article trouvé</div>';
  else $nbitems = '<div id="nbeleme">'.$nbitem.' articles trouvés</div>';
}

if (count($results)>0) {
  print "<div class=\"search_galerie\">$nbitems<div class=\"galerie_photo result\"><ul>";
  foreach($results as $result) {
    //if ($nbr>=$from && $nbr<$to) {
      // display the result
      print theme("spapachesolr_panels_gresult_item_photo_only", $result, "main_size"); 
    //}
    $nbr++;
  }
  print "</ul></div></div>";
}
//ici unset variables
unset($results);