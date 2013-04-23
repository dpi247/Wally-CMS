<?php
/*
 *
 * 
 */

$node_path = drupal_get_path_alias("/node/".$node->nid);
$imgstory = $node->field_embededobjects_nodes;
$destination_term = theme("wallyct_destinationlist", $node->field_destinations, " | " , "", "");
$main_summary = $node->field_summary[0]['value'];
$main_desc = $node->field_objectdescription [0]['value'];
//$main_edition = $field_editions [0]['value'];
//$main_channel = $field_channels [0]['value'];
$presetname='gallery_preset';

foreach ($imgstory as $n) {
  if ($n->type == "wally_photoobject") {
    $file_path = $n->field_photofile[0]['filepath'];
    $explfilepath = explode('/', $file_path );
    $alt=$n->field_summary[0]['value'];
    $file_img[] = theme('imagecache', $presetname,  $file_path, $alt);//, array('class'=>'img'));
    $path_photo[] = imagecache_create_url($presetname, $file_path);
    $title_img[] = $n->title;
    $summary[]= $n->field_summary[0]['value'];
  }
}

drupal_add_js('
  $(document).ready(function() {
    diapo("gal_plugin_'.$node->nid.'");
  });
', 'inline');

?>
<div id="gal_plugin_<?php print $node->nid; ?>" class="gallery">
<h2><?php print $node->title ; ?></h2>
<?php print $main_summary ; ?> <br/> 
<?php print $main_desc ; ?>
  <div class="main_image">
    <?php print $file_img[0]; ?>
      <div class="desc">
        <a href="#" class="collapse">Close Me!</a>
      </div>
      <div class="block">
            <h2><?php print $title_img[0]; ?></h2>
				  <div class="date">
					 Publié le <?php print date('d M Y', $imgstory[0]->created) ?>
					 <span> // <?php print $destination_term; ?></span>
				  </div>
					<p> <?php print $summary[0] ?></p>
          </div>
  </div>
  <div class="image_thumb">
    <ul>
      <?php $i = 0; ?>
      <?php foreach ($file_img as $image) { ?>
        <li class="thumb"><a href=" <?php print $path_photo[$i]; ?> "><?php print $image; ?></a>
     	    <div class="block">
     			 
			   <h2><?php print $title_img[$i]; ?></h2>
				  <div class="date">
					 Publié le <?php print date('d M Y', $imgstory[$i]->created) ?>
					 <span> // <?php print $destination_term; ?></span>
				  </div>
				<p> <?php print $summary[$i] ?></p>
			 </div>
		 </li>
       <?php $i++; } ?>
    </ul>
  </div>  
</div>
