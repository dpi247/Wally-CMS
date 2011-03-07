<?php
// $Id: node.tpl.php,v 1.5 2007/10/11 09:51:29 goba Exp $

$view_items_per_page = $variables['view']->pager['items_per_page'];
$view_current_page = $variables['view']->pager['current_page'];
$view_node_id = $variables["id"];
$view_node_zebra = $variables["zebra"];

$node_id = $node->nid;
$node_title = $node->title;
$node_path = drupal_get_path_alias("node/".$node->nid);
$mainstory = $node->field_mainstory_nodes[0];
$story_path = drupal_get_path_alias("node/".$mainstory->nid);
$destination_term = theme("wallyct_destinationlist", $node->field_destinations, " | " , "", "");
$node_last_modif = $node->changed;
$date_edition = custom_soirmag_date_edition_diplay($node_last_modif, 'default_destination_view');
$node_body = $node->body;

$embeded_objects = $node->field_embededobjects_nodes;
$photoObject = custom_soirmag_get_first_photoEmbededObject_from_package($embeded_objects);
If ($photoObject) {
	$photoObject_path = $photoObject->field_photofile[0]['filepath'];
	$explfilepath = explode('/', $photoObject_path);
}

if($view_node_id == "1" && (!isset($_GET['page']))){ 
  $strapline_length = 0;
} else {
  $strapline_length = 180;
}
$strapline = custom_soirmag_get_strapline($mainstory,$node,$strapline_length);
$teaser_length = 300;
$teaser = theme("wallyct_teaser", $mainstory->field_textbody[0]['value'], $teaser_length, $node);

$facebook_like_button = custom_soirmag_social_bookmarking_facebook_like($node_path);

if (!isset($_GET['page'])){
	if ($view_node_zebra == "odd"){
	 $grid_class = "omega";
	} else {
	 $grid_class = "alpha";
	}
} else {
  if ($view_node_zebra == "even"){
   $grid_class = "omega";
  } else {
   $grid_class = "alpha";
  }
}
?>
<?php if($view_node_id == "1" && (!isset($_GET['page']))){ ?>
<div id="main-excerpt" class="excerpt grid_8 alpha omega">
  <small><?php print $date_edition; ?></small>
  <h2><a href="/<?php print $node_path ; ?>"><?php print $node_title ; ?></a></h2>
  <?php if (isset($photoObject_path)) {  ?>
  <?php $photoObject_img = theme('imagecache', 'sm_bispage_big', $explfilepath[sizeof($explfilepath)-1], $explfilepath[sizeof($explfilepath)-1], $explfilepath[sizeof($explfilepath)-1], array('class'=>'postimage2')); ?> 
  <a href="/<?php print $node_path ; ?>">
  <?php print $photoObject_img ; ?>
  </a>
  <?php }; ?>
  <h3><?php print $strapline ; ?></h3>
  <p><?php print $teaser ; ?> [...]</p>
  <div class="connect_button_container">
    <?php print $facebook_like_button ; ?>
  </div>
</div>
<?php } else { ?>
<div class="excerpt grid_4 <?php print $grid_class ; ?>">
  <div class="connect_button_container">
    <?php print $facebook_like_button ; ?>
  </div>
  <small><?php print $date_edition; ?></small>
  <h2><a href="/<?php print $node_path ; ?>"><?php print $node_title ; ?></a></h2>
  <?php if (isset($photoObject_path)){  ?>
  <?php $photoObject_img = theme('imagecache', 'sm_bispage', $explfilepath[sizeof($explfilepath)-1], $explfilepath[sizeof($explfilepath)-1], $explfilepath[sizeof($explfilepath)-1], array('class'=>'postimage2')); ?>
  <a href="/<?php print $node_path ; ?>">
  <?php print $photoObject_img ; ?>
  </a>
  <?php }; ?>
  <p><?php print $strapline ; ?></p>
</div>
<?php } ?>