<?php
/**
 * @file views-view-unformatted.tpl.php
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 * 
 * @todo: get <li> element here, tranfer the "package layout" to another element than the "<li>"
 */
?>
<?php if (!empty($title)): ?>
<h2 class="section-title"><?php print $title; ?></h2>
<?php endif; ?>
<ul class="article-group">
<?php foreach ($rows as $id => $row): ?>
 <?php print $row; ?>
<?php endforeach; ?>
</ul>
