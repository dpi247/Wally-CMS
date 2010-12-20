<?php
dsm($node);
	$title = $node->title;
	$node_path = drupal_get_path_alias("/node/".$node->nid);
	$mainstory = $node->field_mainstory_nodes[0];
	$story_path = drupal_get_path_alias("/node/".$mainstory->nid);
	$maindestination = $node->field_destinations[0];
	$dest_path = drupal_get_path_alias("/taxonomy/".$maindestination['tid']);
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
		<a href="<?php print $dest_path ?>" title="<?php print $maindestination['target']; ?>" rel="main destination">
			<?php print $maindestination['target']; ?>
		</a>
	</span>
</div> 
