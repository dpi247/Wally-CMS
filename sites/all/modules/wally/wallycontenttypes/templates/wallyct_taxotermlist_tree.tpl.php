<?php
/**
 * Wally default template for rendering taxonomy term list 
 */
if (!function_exists('wally_toolbox_taxonomy_tree')) {
  function wally_toolbox_taxonomy_tree($tid, $child=NULL) {
    $result = array(); 

dsm($term); 

    $t = taxonomy_get_term($tid); 
    $result[$tid]["value"] = $t;

    if ($child) $result[$tid]["child"] = $child;

    dsm($result);
    
    if ($p = taxonomy_get_parents($tid)) {
      return wally_toolbox_taxonomy_tree($p->tid, $result);
    } else {
      return $result; 
    }
  }
}

if (!function_exists('wally_tpl_taxonomyterm_li_tree')) {
  function wally_tpl_taxonomyterm_li_tree($tids) {
    $content = "";
    
    $tree = array(); 

    foreach ($tids as $tid) {
      
      $taxonomy_trunk = wally_toolbox_taxonomy_tree($tid["value"]); 

      dsm($taxonomy_trunk);

    }

      $content .= "<li>"; 
      $content .= "<a href='".$taxo_path."'>"; 
      $content .= "you you"; 
      $content .= "</a>"; 
      $content .= "</li>"; 


    $content = "<ul>".$content."</ul>";
    return $content; 
  }
}
print "tree"; 
print wally_tpl_taxonomyterm_li_tree($tids);
