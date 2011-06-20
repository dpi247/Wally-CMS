<?php 
$row_index=$variables["view"]->row_index;
$view_node_id = $variables["id"];
$node_id = $node->nid;

$photoObject_path = "";

if($node->type == "wally_articlepackage"){
  $mainstory = $node->field_mainstory_nodes[0];
} else {  
  $mainstory = $node->field_mainobject_nodes[0];
  $mainstory_type = $mainstory->type;
  if($mainstory_type == "wally_photoobject"){
    $photoObject_path = $mainstory->field_photofile[0]['filepath'];
    $explfilepath = explode('/', $photoObject_path);
    $photoObject_size = $mainstory->field_photofile[0]['filesize'];
  }
}

$exturl = preg_split('/:::/', $mainstory->field_externalreference[0]["safe"], -1);
if ( isset($exturl[1])) {
	$node_path = $exturl[1];
} else {
	$node_path = drupal_get_path_alias("node/".$node->nid);
} 

if ($photoObject_path == ""){
  $embeded_objects = $node->field_embededobjects_nodes;
  dsm($embeded_objects);
  /*$photoObject = custom_soirmag_get_first_photoEmbededObject_from_package($embeded_objects);
  If ($photoObject) {
    $photoObject_path = $photoObject->field_photofile[0]['filepath'];
    $explfilepath = explode('/', $photoObject_path);
    $photoObject_size = $photoObject->field_photofile[0]['filesize'];
  }*/
}

$photo_large_url = "";
$photo_thumb_url = "";

if ($photoObject_path != "" && $photoObject_size > 0){
	$photo_large_url = imagecache_create_url('sm_event_main', $photoObject_path);
  $photo_thumb_url = imagecache_create_url('sm_event_thumb', $photoObject_path);
}

$node_title = $mainstory->title;

switch ($row_index){
	case 0:
		?>
			<a class="main-event" href="<?php echo $node_path?>">
      <?php if ($photo_large_url != ""){ ?>	
			<img src="<?php echo $photo_large_url; ?>" width="620" height="298" alt="<?php echo $node_title?>" />
			<?php } ?>
			<h1><a id="main-event-a" href="<?php echo $node_path?>"><?php echo $node_title?></a></h1>
			</a>
			<ul>
				<li><a href="<?php echo $node_path?>">
				  <?php if ($photo_thumb_url != ""){ ?>
					<img rel="<?php echo $photo_large_url; ?>" src="<?php echo $photo_thumb_url; ?>" width="136" height="82" alt="<?php echo $node_title?>" />
				  <?php } ?>
				</a></li>
		<?php
		break;
	case 1:
		?>
				<li><a href="<?php echo $node_path?>">
				  <?php if ($photo_thumb_url != ""){ ?>
					<img rel="<?php echo $photo_large_url; ?>" src="<?php echo $photo_thumb_url; ?>" width="136" height="82" alt="<?php echo $node_title?>" />
				  <?php } ?>
				</a></li>
		<?php
		break;
	case 2:
		?>
				<li><a href="<?php echo $node_path?>">
				  <?php if ($photo_thumb_url != ""){ ?>
					<img rel="<?php echo $photo_large_url; ?>" src="<?php echo $photo_thumb_url; ?>" width="136" height="82" alt="<?php echo $node_title?>" />
				  <?php } ?>
				</a></li>
				</ul>
		<?php
		break;
  default:
  	break;	
}
