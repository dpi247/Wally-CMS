<?php
/*
 * Template for rendering a articlepackage-article node type.
 */
$mainstory = $node->field_mainstory_nodes[0];
dpm($node);
?>
    
<div class="content">
<?php
	if (isset($mainstory)) {
		print theme("wallyct_mainstory", $mainstory, $node); 
  }
?>
<?php
  foreach ($node->field_embededobjects_nodes as $embededobject){
    print theme('node', $embededobject) ;    
  }
?>

<?php
  print theme("wallyct_linkedobjects", $node->field_linkedobjects_nodes, $node);
?>

</div>

<?php print $links?>
<div class="clear"></div>
