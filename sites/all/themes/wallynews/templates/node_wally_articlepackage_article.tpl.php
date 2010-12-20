<?
/**
 * Wally default template for rendering Packages Autors List 
 */
	$mainstory = $node->field_mainstory_nodes[0];
?>
<?php
	if (isset($node->field_mainstory_nodes)) {
		print theme("wallyct_mainstory", $mainstory, $node); 
  }
?>
