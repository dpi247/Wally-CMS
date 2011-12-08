<?php
// $Id: pub-content.tpl.php,v 1.0 2011/02/18 ODM $

/**
 * Available variables:
 *  - $content : contents the code returned by the webservice (content and header)
 *  - $delta : the delta of the block
 */
?>

<?php
  print $content['content'];
  wallytoolbox_add_meta($content['meta']['attributes'], $content['meta']['content']);
?>
