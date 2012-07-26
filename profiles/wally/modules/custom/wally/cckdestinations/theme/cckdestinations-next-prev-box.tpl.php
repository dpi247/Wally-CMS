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
<h2>
  <a href="<?php print $path; ?>">
    <?php print t('NEXT'); ?>
  </a>
</h2>
<hr>
<?php
      break;
    case 'prev':
?>
<h2>
  <a href="<?php print $path; ?>">
    <?php print t('PREVIOUS'); ?>
  </a>
</h2>
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
<hr>
<a href="<?php print $path; ?>">
  <?php print $content; ?>
</a>
