<?php 
/*dsm($text,'texte');
dsm($path,'path');
dsm($options,'options');*/
if($_GET["solrsort"]) $param = $_GET["solrsort"];
$paramI = explode(" ", $param);
$paramI = $paramI[0];

$opt = $options["query"]["solrsort"];
$optI = explode(" ", $opt);
$optI = $optI[0];

$tri = '';
$current = '';
$nom = '';
if(isset($optI) && optI!='' && isset($paramI) && $paramI!='') {
  if($optI == $paramI) $current = ' current';
} else  {
  $tri = ' class="current"';
}
if($opt) {
  if(preg_match("/asc/", $opt)) {
    $tri = ' class="asc'.$current.'"';
    $nom = 'Ascendant';
  } else {
    $nom = 'Descendant';
    $tri = ' class="desc'.$current.'"';
  }
}


print "<li$tri>".apachesolr_l($text,  $path, $options).'</li>';


?>
