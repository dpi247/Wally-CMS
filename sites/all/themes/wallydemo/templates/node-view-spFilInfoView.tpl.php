<?php 
	$node_path = wallydemo_get_node_uri($node);
	$mainstory = $node->field_mainstory_nodes[0];
	$title = $mainstory->title;
	$date_edition = _wallydemo_get_edition_date($node, 'filinfo');
?>
<li>
	<p>
		<span><?php print $date_edition ?></span>
		<a href="/<?php print check_url($node_path); ?>"><?php print wallydemo_check_plain($title); ?></a>
	</p>
</li>

<?php 
//ici unset des variables
unset($node);
unset($mainstory);
?>