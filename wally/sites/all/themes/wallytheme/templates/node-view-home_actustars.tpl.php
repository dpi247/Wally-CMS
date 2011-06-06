<?php
// $Id: node.tpl.php,v 1.5 2007/10/11 09:51:29 goba Exp $

//dsm($node,"node");
//dsm($variables,"variable");

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

?>
<li>
  <a href="/<?php print $node_path ; ?>">
  <?php if (isset($photoObject_path)){  ?>
  <?php $photoObject_img = theme('imagecache', 'sm_bispage', $explfilepath[sizeof($explfilepath)-1], $explfilepath[sizeof($explfilepath)-1], $explfilepath[sizeof($explfilepath)-1], array('class'=>'postimage2')); ?>
  <?php print $photoObject_img ; ?>
  <?php }; ?>		
  <h3><?php print $node_title; ?></h3>
	</a>
</li>
