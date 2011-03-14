<?php
/*
* Template for rendering a articlepackage-article node type.
*/
$mainstory = $node->field_mainstory_nodes[0];
?>

<div class="content">
<?php
if (isset($mainstory)) {
  print theme("wallyct_mainstory", $mainstory, $node); 
}
?>
<?php
print theme("wallyct_embededobjects", $node->field_embededobjects_nodes, $node);
?>

<?php
print theme("wallyct_linkedobjects", $node->field_linkedobjects_nodes, $node);
?>

</div>

<?php print $links?>
<div class="clear"></div>
