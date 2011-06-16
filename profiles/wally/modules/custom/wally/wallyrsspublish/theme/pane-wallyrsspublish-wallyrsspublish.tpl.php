<?php
/**
 * 
 * Display a list about the different Destination Path.
 */
$taxos = taxonomy_get_tree(variable_get('wallymport_destinationpath', '2'));
$contenu = "<ul>";
foreach ($taxos as $taxo){
	$name = (string)($taxo->name);
	$link = '/rss/'.$name.'/rss.xml';
	$contenu .='<li><a href="'.$link.'">'.$name."</a></li>";
}
$contenu .= "</ul>";
print $contenu;