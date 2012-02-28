<?php
$dest_tid = $variables['view']->build_info['query_args'][3];
$destObject = taxonomy_get_term($dest_tid);
$dest_name = $destObject->name;
$dest_path = taxonomy_term_path($destObject);
$dest_url = check_url(drupal_get_path_alias($dest_path));
?>
<div class="bloc-01">
	<a href="/<?php print($dest_url); ?>">
	 <h2>Les derniers titres: <?php print($dest_name); ?></h2>
	</a>
	<div class="inner-bloc">
		<ul>
		<?php foreach ($rows as $id => $row): ?>
		    <?php print $row; ?>
		<?php endforeach; ?>
		</ul>
	</div>	
</div>