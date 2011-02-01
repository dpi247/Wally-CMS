<?php
	dsm ($node);
			   
  $node_path = drupal_get_path_alias("node/".$node->nid);
	$mainstory = $node->field_mainstory_nodes[0];
	$story_path = drupal_get_path_alias("node/".$mainstory->nid);
  $destination_term = theme("wallyct_destinationlist", $node->field_destinations, " | " , "", "");
  
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
