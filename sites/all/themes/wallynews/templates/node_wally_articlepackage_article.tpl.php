<?
  $mainstory = $node->field_mainstory_nodes[0];
?>
<div class="content">
<?php
	if (isset($node->field_mainstory_nodes)) {
		print theme("wallyct_mainstory", $mainstory, $node); 
  }
?>
<?php
  print theme("wallyct_linkedobjects", $node->field_linkedobjects_nodes, $node);
?>
</div>
<div class="clear"></div>
