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
      $content .= "<a href='".base_path().$taxo_path."'>"; 
      $content .= $t->name;
      $content .= "</a>"; 
      $content .= "</li>"; 
    }
    if ($content!="") {
      $content = "<ul>".$content."</ul>";
      return $content; 
    } else {
      return NULL; 
    }
  }
}

if ($options['override_title']) {
    $title = t($options['override_title_text']);
  } else {
    $title = t($options['taxonomyfield']);
  }
  
dsm($tids); 
print "<h2>".$title."</h2>";
print wally_tpl_taxonomyterm_li($tids);
