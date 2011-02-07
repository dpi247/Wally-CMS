<?php
  $node_path = drupal_get_path_alias("/node/".$node->nid);
  $imgstory = $node->field_embededobjects_nodes;
  $destination_term = theme("wallyct_destinationlist", $node->field_destinations, " | " , "", "");
  $main_summary = $node->field_summary[0]['value'];
  $main_desc = $node->field_objectdescription [0]['value'];
  //$main_edition = $field_editions [0]['value'];
  //$main_channel = $field_channels [0]['value'];
  $presetname='slider_gallery_preset';
 
foreach ($imgstory as $n) {
		if ($n->type == "wally_photoobject") {
      
      $file_path = $n->field_photofile[0]['filepath'];
      $explfilepath = explode('/', $file_path );
	  $file_img[] = theme('imagecache', $presetname,  $file_path);//, array('class'=>'img'));
	  $path_photo[] = imagecache_create_url($presetname, $file_path); 
	  $title_img[] = $n->title;
	  $summary[]= $n->field_summary[0]['value'];
		
		
		}
		
	}

//dsm($file_img);

?>
	<?//AFFICHAGE
	  // Nom de la photo ?>

<div id="gal1" class="gallery">
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
					 Publié le <?php print date('d M Y', $imgstory->created) ?>
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
					 Publié le <?php print date('d M Y', $imgstory->created) ?>
					 <span> // <?php print $destination_term; ?></span>
				  </div>
				<p> <?php print $summary[$i] ?></p>
			 </div>
		 </li>
       <?php $i++; } ?>
    </ul>
  </div>
   <div class = "test" >
   </div>   
</div>

<script>
$(document).ready(function() {	
	diapo('gal1');
});

</script>