<?php
// $Id: pub-purl-content.tpl.php,v 1.0 2011/02/18 ODM $

/**
 * Available variables:
 *  - $content : contents the code returned by the webservice (content and header)
 *  - $delta : the delta of the block
 */
?>
<!--  PURL  -->
<?php
  print $content['content'];
?>
<!--  /PURL  -->