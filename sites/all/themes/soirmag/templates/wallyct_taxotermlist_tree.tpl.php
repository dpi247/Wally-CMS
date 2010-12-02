<?php
/**
 * Wally default template for rendering taxonomy term list 
 */

/*
 * Get taxonomy trunk.
 * recursly search for parent of tid till get to root.
 * 
 * @param: $tid
 *   Tid of the children
 * @param: $child
 *   Internal use for resursivity.
 * 
 * @return: recursive array of terms. 
 */
if (!function_exists('wally_toolbox_taxonomy_parent_tree')) {
  function wally_toolbox_taxonomy_parent_tree($tid, $child=NULL) {
    $result = array(); 
    $t = taxonomy_get_term($tid); 
    $result[$tid]["#value"] = $t;
    if ($child) $result[$tid]["#children"] = $child;
    if ($p = taxonomy_get_parents($tid)) {
      $k = array_keys($p);
      $k = $k[0];
      return wally_toolbox_taxonomy_parent_tree($p[$k]->tid, $result);
    } else {
      return $result; 
    }
  }
}

/*
 * Some kind of array_extend
 * recusive method for merging arrays.
 */
if (!function_exists('array_extend')) {
  function array_extend($a, $b) {
    foreach($b as $k=>$v) {
      if( is_array($v) ) {
        if( !isset($a[$k]) ) {
          $a[$k] = $v;
        } else {
          $a[$k] = array_extend($a[$k], $v);
        }
      } else {
        $a[$k] = $v;
      }
    }
    return $a;
  }
}


/*
 * Draw a nested <ul> list from a taxonomy tree.
 * 
 * @param: $tree
 *   taxonomy tree currently processed 
 *   see: wally_toolbox_taxonomy_parent_tree()
 *   and array_extend()
 * 
 * @param: $itds
 *   List of tid for some class modification to the <li> / <a>.
 * 
 * @return: nested <ul> html string. 
 */
if (!function_exists('plot_tree')) {
  function plot_tree($tree, $tids = array()) {
    $content = "<ul>";
    foreach($tree as $k=>$v) {
      
      $taxo_path = taxonomy_term_path($v['#value']);
      if (isset($tids)) {
        $included = "not_included";
        foreach($tids as $ktid=>$tid) {
          if ($tid['value'] == $v['#value']->tid) $included = "included";
        }
      }
      $content .= "<li class='".$included."'>";
      
      $content .= "<a class='".$included."' href='".base_path().$taxo_path."'>".$v['#value']->name."</a>";

      if (isset($v['#children'])) {
        $content .= plot_tree($v['#children'], $tids); 
      }
      $content .= "</li>";
    }
    
    $content .= "</ul>";
    return $content;  
  }
}


if (!function_exists('wally_tpl_taxonomyterm_li_tree')) {
  function wally_tpl_taxonomyterm_li_tree($tids) {
    $content = "";
    $tree = array(); 

    foreach ($tids as $tid) {
      $taxonomy_trunk = wally_toolbox_taxonomy_parent_tree($tid["value"], null); 
      $tree = array_extend($tree, $taxonomy_trunk); 
    }

    $content = plot_tree($tree, $tids);
    return $content; 
  }
}
if ($options['override_title']) {
    $title = t($options['override_title_text']);
  } else {
    $title = t($options['taxonomyfield']);
  }

?>
<h2><? print $title; ?></h2>
<?php print wally_tpl_taxonomyterm_li_tree($tids); ?>
