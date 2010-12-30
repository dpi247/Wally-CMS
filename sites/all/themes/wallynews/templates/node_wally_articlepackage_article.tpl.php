<?
/*
 * Template for rendering a articlepackage-article node type.
 */

  $mainstory = $node->field_mainstory_nodes[0];

  dsm($mainstory); 
?>
<div class="content">
//// trying to get my node ....<br/>
<?php
	if (isset($mainstory)) {
		print theme("wallyct_mainstory", $mainstory, $node); 
  }
?>
<?php
  print theme("wallyct_linkedobjects", $node->field_linkedobjects_nodes, $node);
?>
//// trying to get my node ....<br/>
//// trying to get my node ....<br/>
//// trying to get my node ....<br/>
//// trying to get my node ....<br/>
//// trying to get my node ....<br/>
</div>
<div class="clear"></div>
