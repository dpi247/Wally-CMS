<?php
	$title = $node->title;
	$node_path = drupal_get_path_alias("/node/".$node->nid);
	$mainstory = $node->field_mainstory_nodes[0];
	$story_path = drupal_get_path_alias("/node/".$mainstory->nid);
	$mainDest = array();
	foreach ($node->field_destinations as $destination) {
		$tempDest['taxID'] = $destination['tid'];
		$tempDest['tax'] = taxonomy_get_term($destination['tid']);
		$tempDest['dest'] = $tempDest['tax']->description;
		$tempDest['path'] = drupal_get_path_alias("/taxonomy/".$destination['tid']);
		$mainDest[] = $tempDest;
	}
	$main_image = $node->fields_embededobjects_nodes[0];
	$teaser_length = 300;
	$teaser = theme("wallyct_teaser", $mainstory->field_textbody[0]['value'], $teaser_length, $node);
  
	foreach ($node->field_embededobjects_nodes as $n) {
		if ($n->type == "wally_photoobject") {
			//$file_path = "/".$n->field_photofile[0]["filepath"];        
			$file_img = theme('imagecache', 'theme_medium_article_preset', $n->field_photofile[0]["filename"], $n->field_photofile[0]["filename"], $n->field_photofile[0]["filename"], array('class'=>'postimage2'));
			break;
		}
	}
?>
<h2>
	<a href="<?php print $node_path; ?>" rel="main story title" title="<?php print $mainstory->title; ?>">
		<?php print $mainstory->title; ?>
	</a>
</h2>
<div class="date">
	Publié le <?php print date('d M Y', $mainstory->created) ?>
	<span> // 
		<?php
			foreach($mainDest as $dest) {
		?>
			<a href="<?php print $dest['path'] ?>" title="<?php print $dest['taxID']; ?>" rel="main destination">
				<?php print $dest['dest']; ?>
			</a>
		<?php
			}
		?>
	</span>
</div> 
<a href="<?php print $node_path; ?>" rel="main story title" title="<?php print $mainstory->title; ?>">
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
