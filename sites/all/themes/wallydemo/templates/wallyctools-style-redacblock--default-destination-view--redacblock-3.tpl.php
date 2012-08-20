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

<div class="highlight">
<?php foreach ($rows as $id => $row): ?>
    <?php print $row; ?>
<?php endforeach; ?>
</div>
