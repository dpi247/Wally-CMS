<?php
// $Id: api-functions.tpl.php,v 1.1.2.2 2010/04/21 23:21:12 drumm Exp $

/**
 * @file api-functions.tpl.php
 * Theme implementation to show a list of functions, their source and description.
 *
 * Available variables:
 * - $function['function']: Function link.
 * - $function['file']: File link.
 * - $function['description']: Function description.
 */
?>
<dl class="api-functions">
<?php foreach ($functions as $function) { ?>
  <dt><?php print $function['function'] ?> <small>in <?php print $function['file'] ?></small></dt>
  <dd><?php print $function['description'] ?></dd>
<?php } ?>
</dl>
