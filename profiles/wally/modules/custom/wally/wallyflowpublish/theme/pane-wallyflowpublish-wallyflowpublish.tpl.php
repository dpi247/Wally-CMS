<?php

/**
 * Display a list of links to the RSS feeds of the destinations.
 */
$view = views_get_view('default_destination_view_feed');
$display = $view->display['feed_1'];
$view_path = $display->display_options['path'];
$expl_path = explode('/%/', $view_path);
$dest_vid = variable_get('wallymport_destinationpath', 0);

$taxos = taxonomy_get_tree();
$contenu = "<ul>";
foreach ($taxos as $taxo){
	$name = (string)($taxo->name);
	$link_name = wallytoolbox_taxonomy_get_path_by_tid_or_term($taxo->tid, $dest_vid);
	$link = '/'.$expl_path[0].'/'.$link_name;
	$link = (isset($expl_path[1]) && $expl_path[1] != '') ? $link.'/'.$expl_path[1] : $link;
	$contenu .='<li><a href="'.$link.'">'.$name."</a></li>";
}
$contenu .= "</ul>";
print $contenu;
