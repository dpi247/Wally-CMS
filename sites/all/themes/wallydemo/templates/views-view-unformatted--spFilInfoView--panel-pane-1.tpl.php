<?php

// $Id: views-view-unformatted.tpl.php,v 1.6 2008/10/01 20:52:11 merlinofchaos Exp $
/**
 * @file views-view-unformatted.tpl.php
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */

$tmpArray = $variables["view"]->filter["field_destinations_tid"]->view->filter["field_destinations_tid"]->value;
$tid = array_shift($tmpArray);
?>


<?php if (!empty($title)): ?>
	<h2><a href="/<?php print drupal_get_path_alias("taxonomy/term/".$tid)?>"><?php print $title; ?></a></h2>
<?php else : ?>
	<h2><a href="/<?php print drupal_get_path_alias("taxonomy/term/".$tid)?>">L'info en continu</a></h2>
<?php endif; ?>
<div class="inner-bloc">
<ul>
<?php foreach ($rows as $id => $row): ?>
    <?php print $row; ?>
<?php endforeach; ?>
<li class="infoc">
<a href="/<?php print drupal_get_path_alias("taxonomy/term/".$tid)?>" class="novisited arrow">Toute l'info en continu</a>
</li>
</ul>
</div>