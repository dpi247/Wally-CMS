<?php 
	$node_path = drupal_get_path_alias("node/".$node->nid);
	$mainstory = $node->field_mainstory_nodes[0];
	$title = $mainstory->title;
	$node_publi_date = strtotime($node->field_publicationdate[0]['value']);
	$date_edition = _wallydemo_date_edition_diplay($node_publi_date, 'filinfo');
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