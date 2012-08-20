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

// First block
  case 1:
    foreach ($rows as $row_id => $row) :
      switch ($row_id) :
        case 0:
        case 1:
        case 6:
        case 7:
          print $row;
          break;
        case 2:
?>
<div class="wrap-columns clearfix">
  <div class="intern-col first column limit">
<?php
          print $row;
          break;
        case 3:
          print $row;
?>
  </div>
<?php
          break;
        case 4:
?>
  <div class="intern-col last column">
<?php
          print $row;
          break;
        case 5:
          print $row;
?>
  </div>
</div>
<?php
          break;
      endswitch;
    endforeach;
    break;
// End of first block

// Second and third blocks
  case 2:
  case 3:
    foreach ($rows as $row_id => $row) :
      switch ($row_id) :
        case 4:
        case 5:
          print $row;
          break;
        case 0:
?>
<div class="wrap-columns clearfix">
  <div class="intern-col first column limit">
<?php
          print $row;
          break;
        case 1:
          print $row;
?>
  </div>
<?php
          break;
        case 2:
?>
  <div class="intern-col last column">
<?php
          print $row;
          break;
        case 3:
          print $row;
?>
  </div>
</div>
<?php
          break;
      endswitch;
    endforeach;
    break;
// End of second and third blocks

endswitch;
?>
