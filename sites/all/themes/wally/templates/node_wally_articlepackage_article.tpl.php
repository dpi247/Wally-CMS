<?php
/**
 *
 * 
 */

?> 
<div id="main-content">
<?php print date('l jS \of F Y h:i:s A', $node->changed); ?> - <?php print theme("wallyct_authorslist",$node->field_authors_nodes, $node); ?>
<hr />
  <div id="main-content-right">
  <?php
    if (isset($node->field_mainstory_nodes)) {
     print theme("wallyct_mainstory", $node->field_mainstory_nodes[0], $node); 
    }
  ?>
  </div>
  <div id="main-content-left">

  </div>
</div>
