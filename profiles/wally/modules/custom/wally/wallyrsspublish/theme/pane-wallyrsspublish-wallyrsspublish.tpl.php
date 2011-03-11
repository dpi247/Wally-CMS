<?php
$all_voc = taxonomy_get_vocabularies();
foreach ($all_voc as $voc) {
	if ($voc->name == 'Destination Path') {
		$taxos = taxonomy_get_tree($voc->vid);
		break;
	}
}
$contenu = "<ul>";
foreach ($taxos as $taxo){
	$name = (string)($taxo->name);
	$link = '/rss/'.$name.'/rss.xml';
	$contenu .='<li><a href="'.$link.'">'.$name."</a></li>";
}
$contenu .= "</ul>";
print $contenu;
?>
