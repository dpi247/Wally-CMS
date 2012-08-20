<?php 
/**
 * @file
 * Template to display destinations views.
 * 
 * @see template_preprocess_wallyctools_style_redacblock.
 *
 * $view: The view.
 * $destination: the destination argument.
 * $target: The targeted block.
 * 
 */
?>

<?php if (!empty($title)): ?>
  <h3><?php print $title; ?></h3>
<?php endif; ?>
<?php foreach ($rows as $id => $row): ?>
    <?php print $row; ?>
<?php endforeach; ?>
