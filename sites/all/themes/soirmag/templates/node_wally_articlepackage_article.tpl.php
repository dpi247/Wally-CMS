<?php

?> 
<?php
    if (isset($node->field_mainstory_nodes)) {
     print theme("wallyct_mainstory", $node->field_mainstory_nodes[0], $node); 
    }
?>
