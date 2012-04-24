<?php
/*
 *
 * 
 */

$themeroot = drupal_get_path('theme', 'wallydemo');
drupal_add_css($themeroot . '/css/gallery.css');
$embededobjects = $node->field_embededobjects_nodes;
array_unshift($embededobjects, $node->field_mainobject_nodes[0]);
$sorted_embededobjects = wallycontenttypes_sort_embededobjects($embededobjects);
$imgstory = $sorted_embededobjects['wally_photoobject'];
$videostory = $sorted_embededobjects['wally_videoobject'];
$destination_term = theme("wallyct_destinationlist", $node->field_destinations, " | " , "", "");
$main_summary = $field_summary[0]['value'];

$main_desc = $field_objectdescription [0]['value'];
$textstory = NULL;
foreach ($node->field_embededobjects_nodes as $embeds){
  if ($embeds->type == 'wally_textobject'){
    $text  = '<div class = "gallerytextembed">';
    $text .= '<h3>'.$embeds->title.'</h3>';
    $text .= '<p class = "chapeau">'.$embeds->field_textchapo[0]['value'].'</p>';
    $text .= $embeds->field_textbody[0]['value'];
    $text .= '</div>';
    $textstory[] = $text;
  }
}

$presetname='gallery_preset';

foreach ($imgstory as $n) {
  if ($n->type == "wally_photoobject") {
    $file_path = $n->field_photofile[0]['filepath'];
    $alt = $n->field_summary[0]['value'];
    $explfilepath = explode('/', $file_path );
    $file_img[] = theme('imagecache', $presetname,  $file_path, $alt );//, array('class'=>'img'));
    $path_photo[] = imagecache_create_url($presetname, $file_path);
    $title_img[] = $n->title;
    $summary[]= $n->field_summary[0]['value'];
  }
}
drupal_add_js('
  $(document).ready(function() {
    diapo("gal_node_'.$node->nid.'");
  });
', 'inline');
?>
<div id="gal_node_<?php print $node->nid; ?>" class="gallery">

<h2><?php print $node->title;?></h2>
<?php print $main_summary ; ?> <br/>
<?php print $main_desc ; ?>

		
  <div id = "embedded_text"class="text_thumb">
    <ul>
      
      <?php foreach ($textstory as $text) { ?>
        <?php print $text; ?>
      <?php } ?>
  	
  </ul>
  </div>
  
  <p><br/><br/></p>
  <div class="photos">
    <?php foreach ($imgstory as $image):?>
	  <div class="gallerie_photo">
        <?php print theme('imagecache', 'unebis_medium_180x120',$image->field_photofile[0]['filepath'],$image->field_photofile[0]['filename'], $image->title );?>
  	  </div>
    <?php endforeach;?>
  </div>
  <div class = "clear"></div>
  <div class="video_thumb">
    <ul>
      <?php foreach ($videostory as $video) { ?>
        <?php print theme('node', $video); ?>
      <?php } ?>
    </ul>
  </div>
  
</div>
<?php print $links ?>
