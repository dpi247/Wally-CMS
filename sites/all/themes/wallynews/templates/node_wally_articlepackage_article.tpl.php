<?php
  drupal_add_css(drupal_get_path('module', 'wallycontenttypes') . '/css/superfish/superfish.css', 'theme');
  drupal_add_css(drupal_get_path('module', 'wallycontenttypes') . '/css/superfish/superfish-navbar.css', 'theme');
  drupal_add_js(drupal_get_path('module', 'wallycontenttypes') . '/scripts/superfish/hoverIntent.js', 'theme');
  drupal_add_js(drupal_get_path('module', 'wallycontenttypes') . '/scripts/superfish/superfish.js','theme');

  drupal_add_js("
      $(document).ready(function() {
        jQuery('ul.sf-menu').superfish();
      });
  ", 'inline');
?> 
<?php
    if (isset($node->field_mainstory_nodes)) {
     print theme("wallyct_mainstory", $node->field_mainstory_nodes[0], $node); 
    }
?>
