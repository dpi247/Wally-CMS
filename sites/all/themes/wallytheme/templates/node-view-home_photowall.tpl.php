<?php 

$row_index=$variables["view"]->row_index;

$node_id = $node->nid;
$node_title = $node->title;
$node_path = drupal_get_path_alias("node/".$node->nid);
$mainstory = $node->field_mainstory_nodes[0];

$embeded_objects = $node->field_embededobjects_nodes;
$photoObject = custom_soirmag_get_first_photoEmbededObject_from_package($embeded_objects);
If ($photoObject) {
	$photoObject_path = $photoObject->field_photofile[0]['filepath'];
	$explfilepath = explode('/', $photoObject_path);
}

switch ($row_index){
	case 0:
		$divclass = "grid_4 alpha";
		$img = theme('imagecache', 'sm_photowall_large', $explfilepath[sizeof($explfilepath)-1], $explfilepath[sizeof($explfilepath)-1], $explfilepath[sizeof($explfilepath)-1]);
		break;
	case 1:
		$divclass = "grid_4 omega";
		$img = theme('imagecache', 'sm_photowall_large', $explfilepath[sizeof($explfilepath)-1], $explfilepath[sizeof($explfilepath)-1], $explfilepath[sizeof($explfilepath)-1]);
		break;
	case 2:
		$divclass = "grid_2 alpha";
		$img = theme('imagecache', 'sm_photowall_tall', $explfilepath[sizeof($explfilepath)-1], $explfilepath[sizeof($explfilepath)-1], $explfilepath[sizeof($explfilepath)-1]);
		break;
	case 3:
		$divclass = "grid_4";
		$img = theme('imagecache', 'sm_photowall_central', $explfilepath[sizeof($explfilepath)-1], $explfilepath[sizeof($explfilepath)-1], $explfilepath[sizeof($explfilepath)-1]);
		break;
	case 4:
		$divclass = "grid_2 omega";
		$img = theme('imagecache', 'sm_photowall_tall', $explfilepath[sizeof($explfilepath)-1], $explfilepath[sizeof($explfilepath)-1], $explfilepath[sizeof($explfilepath)-1]);
		break;
	case 5:
		$divclass = "grid_4 alpha";
		$img = theme('imagecache', 'sm_photowall_large', $explfilepath[sizeof($explfilepath)-1], $explfilepath[sizeof($explfilepath)-1], $explfilepath[sizeof($explfilepath)-1]);
		break;
	case 6:
		$divclass = "grid_2";
		$img = theme('imagecache', 'sm_photowall_mini', $explfilepath[sizeof($explfilepath)-1], $explfilepath[sizeof($explfilepath)-1], $explfilepath[sizeof($explfilepath)-1]);
		break;
	case 7:
		$divclass = "grid_2 omega";
		$img = theme('imagecache', 'sm_photowall_mini', $explfilepath[sizeof($explfilepath)-1], $explfilepath[sizeof($explfilepath)-1], $explfilepath[sizeof($explfilepath)-1]);
		break;
}
?>
<div class="<?php echo $divclass;?>"><a href="<?php echo $node_path?>"><?php echo $img?><h2><?php echo $node_title?></h2></a></div>