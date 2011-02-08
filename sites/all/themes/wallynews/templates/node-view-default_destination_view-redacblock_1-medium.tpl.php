<?php
	//dsm ($node);
	$title = $node->title;
	$node_path = drupal_get_path_alias("node/".$node->nid);
	$mainstory = $node->field_mainstory_nodes[0];
	$story_path = drupal_get_path_alias("node/".$mainstory->nid);
  $destination_term = theme("wallyct_destinationlist", $node->field_destinations, " | " , "", "");


  $main_image = $node->fields_embededobjects_nodes[0];
	
  $teaser_length = 300;
	$teaser = theme("wallyct_teaser", $mainstory->field_textbody[0]['value'], $teaser_length, $node);
  
	foreach ($node->field_embededobjects_nodes as $n) {
		if ($n->type == "wally_photoobject") {
      $file_path = '';
      foreach ($node->field_embededobjects_nodes as $embededobjects_node) {
        if ($embededobjects_node->type == 'wally_photoobject') {
          $file_path = $embededobjects_node->field_photofile[0]['filepath'];
          break;
        }
      }
      $explfilepath = explode('/', $file_path);
			$file_img = theme('imagecache', 'theme_medium_article_preset', $explfilepath[sizeof($explfilepath)-1], $explfilepath[sizeof($explfilepath)-1], $explfilepath[sizeof($explfilepath)-1], array('class'=>'postimage2'));
			break;
		}
	}

?>
<h2  style="">
	<a href="/<?php print $node_path; ?>" rel="main story title" title="<?php print $mainstory->title; ?>">
		<?php print $mainstory->title; ?>
	</a>
</h2>
<div class="date">
	Publié le <?php print date('d M Y', $mainstory->created) ?>
	<span> // <?php print $destination_term; ?></span>
</div> 
<a href="/<?php print $node_path; ?>" rel="main story title" title="<?php print $mainstory->title; ?>">
	<?php print $file_img; ?>
</a>
<div class="archivecontent">
<?php
	
		if (isset($mainstory->field_textchapo[0]['value'])) {
			print $mainstory->field_textchapo[0]['value'];
		} else {
			print $teaser;
		}	
	?>
</div>
<div class="clear"></div>
