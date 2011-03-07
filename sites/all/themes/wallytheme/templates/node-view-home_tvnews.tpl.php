<?php
// $Id: node.tpl.php,v 1.5 2007/10/11 09:51:29 goba Exp $

// dsm($node,"node");
// dsm($variables,"variable");
// dsm($variables["id"]);
// dsm($variables);
// dsm($node,"node");
// dsm($variables,"var");

$row_index=$variables["view"]->row_index;

$node_id = $node->nid;
$node_title = $node->title;
$node_path = drupal_get_path_alias("node/".$node->nid);
$mainstory = $node->field_mainstory_nodes[0];
$story_path = drupal_get_path_alias("node/".$mainstory->nid);

$embeded_objects = $node->field_embededobjects_nodes;
$photoObject = custom_soirmag_get_first_photoEmbededObject_from_package($embeded_objects);
If ($photoObject) {
	$photoObject_path = $photoObject->field_photofile[0]['filepath'];
	$explfilepath = explode('/', $photoObject_path);
}

$teaser_length = 300;
$strapline = custom_soirmag_get_strapline($mainstory,$node,$strapline_length);

switch ($row_index) {
	case 0:
		// Fist pass.
		  if (isset($photoObject_path)) { 
	  		$photoObject_img = theme('imagecache', 'sm_bispage', $explfilepath[sizeof($explfilepath)-1], $explfilepath[sizeof($explfilepath)-1], $explfilepath[sizeof($explfilepath)-1], array('class'=>'postimage2'));
	  		print $photoObject_img;
			};
	  	print "<h3><a href='/".$node_path."'>".$node_title."</a></h3>";
	  	print "<p>".$strapline."</p>";
			break;  			
	case 1:
			print "<ul>";
			print "<li><a href='/".$node_path."'>".$node_title."</a></li>";
			break;
	default:
			print "<li><a href='/".$node_path."'>".$node_title."</a></li>";
			break;
}
