<?php
/**
 * Wally default template for rendering Presons Lists 
 * <ul><li></li></ul> style. 
 */

if (!function_exists('wally_tpl_persons_li')) {
  function wally_tpl_persons_li($persons) {

    $content = "";
    foreach ($persons as $person) {
      $content .= "<li>"; 
        if (isset($person->field_persontaxonomy)) {
          $content .= "<a href='/taxonomy/term/".$person->field_persontaxonomy[0]['value']."'>".$person->title."</a>";
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
