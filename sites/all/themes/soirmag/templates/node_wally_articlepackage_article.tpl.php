<?php
  drupal_add_css(drupal_get_path('theme', 'soirmag') . '/css/article.css');
?> 
<?php
    if (isset($node->field_mainstory_nodes)) {
     print theme("wallyct_mainstory", $node->field_mainstory_nodes[0], $node); 
    }
?>
