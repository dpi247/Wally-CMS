<?php
/**
 * Wally default template for rendering Presons Lists 
 * <ul><li></li></ul> style. 
 */

if (!function_exists('wally_tpl_persons_li')) {
  function wally_tpl_persons_li($persons) {

    $content = "";
    foreach ($persons as $person) {
      $content .= "<li class='".$person->field_personsex[0]["value"]."' >"; 
        if (isset($person->field_persontaxonomy)) {
          $t = taxonomy_get_term($person->field_persontaxonomy[0]['value']);
          $taxo_path = taxonomy_term_path($t);
          $content .= "<a href='".base_path().$taxo_path."'>".$person->title."</a>";
        } else {
          $content .= $person->title;
        }
      $content .= "</li>"; 
    }
    $content = "<ul>".$content."</ul>";
 
    return $content; 
  }
}

print wally_tpl_persons_li($persons);
