<?php
dsm($node);
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
?>
<h1>
	<a href="<?php print $node_path; ?>">
		<?php print $title; ?>
	</a>
</h1>
<h2>
	<a href="<?php print $story_path; ?>" rel="main story title" title="<?php print $mainstory->title; ?>">
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
