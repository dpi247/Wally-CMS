<?php 
$dest_tid = $view->args[0];
$destObject = taxonomy_get_term($dest_tid);
$dest_name = $destObject->name;
$dest_path = taxonomy_term_path($destObject);
$dest_url = check_url(drupal_get_path_alias($dest_path));
?>

<div class="onemoreisaid">
  <?php if ($dest_name != ""): ?>
  <h2> <a href="<?php print($dest_url); ?>"> <?php print $dest_name; ?></a></h2>
  <?php endif; ?>
  <?php foreach ($rows as $id => $row):?>
  <?php print $row; ?>
  <?php endforeach; ?>
</div>
