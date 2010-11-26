<?php
/**
 * Wally default template for rendering taxonomy term list 
 */

if (!function_exists('wally_tpl_taxonomyterm_li')) {
  function wally_tpl_taxonomyterm_li($tids) {
    $content = "";
    foreach ($tids as $tid) {

  
      $t = taxonomy_get_term($tid["value"]);
      $taxo_path = taxonomy_term_path($t);

      $content .= "<li>"; 
      $content .= "<a href='".$taxo_path."'>"; 
      $content .= $t->name;
      $content .= "</a>"; 
      $content .= "</li>"; 
    }
    $content = "<ul>".$content."</ul>";
    return $content; 
  }
}

print wally_tpl_taxonomyterm_li($tids);
