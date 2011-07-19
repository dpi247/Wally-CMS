<?php

/**
 * Display a list of links to the RSS feeds of the destinations.
 */
$taxos = taxonomy_get_tree(variable_get('wallymport_destinationpath', 0));
$contenu = "<ul>";
foreach ($taxos as $taxo){
	$name = (string)($taxo->name);
	$link = '/feed/'.$name;
	$contenu .='<li><a href="'.$link.'">'.$name."</a></li>";
}
$contenu .= "</ul>";
print $contenu;
