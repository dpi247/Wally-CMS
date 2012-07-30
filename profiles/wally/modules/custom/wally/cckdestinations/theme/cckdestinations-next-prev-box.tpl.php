<?php 
  /*
   * Variables :
   * - $type (next or prev)
   * - $node
   * - $path
   * - $title
   * - $content
   */
?>

<?php 
  switch ($type):
    case 'next':
?>
<h3>
  <a href="<?php print $path; ?>">
    <?php print t('NEXT'); ?>
  </a>
</h3>
<hr>
<?php
      break;
    case 'prev':
?>
<h3>
  <a href="<?php print $path; ?>">
    <?php print t('PREVIOUS'); ?>
  </a>
</h3>
<hr>
<?php
      break;
  endswitch;
?>

<h2>
  <a href="<?php print $path; ?>">
    <?php print $title; ?>
  </a>
</h2>
<a href="<?php print $path; ?>">
  <?php print $content; ?>
</a>
