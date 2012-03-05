<!-- 
Bloc à onglets pour classer les résultats dans l'ordre souhaité"
class="class .pane-apachesolr-sort" -->

<div class="votre_tri"><h2>Trier par :&nbsp;</h2>
<ul>
<?php

$param = $_GET["solrsort"];
foreach($items as $key=>$item) {
  if($key != 'type' && $key != 'sort_name') echo $item; 
}


/*if($param) {
  if(preg_match("/asc/", $param)) $tri = '<li class="ascFleche"><a href="'.$_SERVER["SCRIPT_URI"].'?solrsort='.str_replace('asc', 'desc', $param).'">Ascendant</a></li>';
  else $tri = '<li class="descFleche"><a href="'.$_SERVER["SCRIPT_URI"].'?solrsort='.str_replace('desc', 'asc', $param).'">Descendant</a></li>'; 
  print $tri;
}*/
?>
</ul></div>