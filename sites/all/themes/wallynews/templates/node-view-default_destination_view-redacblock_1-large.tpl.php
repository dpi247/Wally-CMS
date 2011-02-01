<?php
	dsm ($node);
	$title = $node->title;
	$node_path = drupal_get_path_alias("node/".$node->nid);
	$mainstory = $node->field_mainstory_nodes[0];
	$story_path = drupal_get_path_alias("node/".$mainstory->nid);
    $destination_term = theme("wallyct_destinationlist", $node->field_destinations, " | " , "", "");

	$teaser_length = 1000;
	$teaser = theme("wallyct_teaser", $mainstory->field_textbody[0]['value'], $teaser_length, $node);

    $main_image = null;
	foreach ($node->field_embededobjects_nodes as $embededobject) {
		if ($embededobject->type == "wally_photoobject") {
			$file_path = '';
      foreach ($node->field_embededobjects_nodes as $embededobjects_node) {
        if ($embededobjects_node->type == 'wally_photoobject') {
          $file_path = $embededobjects_node->field_photofile[0]['filepath'];
          break;
        }
      }
      $explfilepath = explode('/', $file_path);
			$main_image = theme('imagecache', 'theme_large_article_preset', $explfilepath[sizeof($explfilepath)-1], $explfilepath[sizeof($explfilepath)-1], $explfilepath[sizeof($explfilepath)-1], array('class'=>'postimage2'));
      break;
		}
	}

?>
<h2>
	<a href="/<?php print $node_path; ?>" rel="main story title" title="<?php print $mainstory->title; ?>">
		<?php print $mainstory->title; ?>
	</a>
</h2>
<div class="date">
	Publié le <?php print date('d M Y', $mainstory->created) ?>
	<span> // <?php print $destination_term; ?></span>
</div> 
<div class="archivecontent" style="min-height: 200px;">
<?php if ($main_image) { 
    print "<a href='/".$node_path."' rel='main story title' title='".$mainstory->title."'>";
    print $main_image;
    print "</a>";
  }
?>
<?php
		if (isset($mainstory->field_textchapo[0]['value'])) {
			print $mainstory->field_textchapo[0]['value'];
		} else {
			print $teaser;
		}	
	?>
</div>
<div class="clear"></div>
