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

switch ($id) :
  case 1:
    foreach ($rows as $row_id => $row) :
      switch ($row_id) :
        case 0:
        case 1:
?>
<div class="article md clearfix">
  <?php print $row; ?>
</div>
<?php
          break;
        case 2:
        default:
?>
<div class="article lt clearfix">
  <?php print $row; ?>
</div>
<?php
          break;
      endswitch;
    endforeach;
    break;
    
  case 2:
    foreach ($rows as $row_id => $row) :
?>
<div class="article lt clearfix">
  <?php print $row; ?>
</div>
<?php
    endforeach;
    break;
endswitch;
?>
