<?php

function cciinlineobjects_preprocess_filter_inline_objects(&$vars) {
  $embed_object = $vars['embed_object'];
  $attributes = $vars['attributes'];
  
  switch ($embed_object->type) {
    case 'wally_photoobject' :
      $dimensions = '';
      if (isset($attributes['preset']) && !empty($attributes['preset'])) {
        $src = imagecache_create_url($attributes['preset'], $embed_object->field_photofile[0]['filepath']);
      } elseif ((isset($attributes['width']) && !empty($attributes['width'])) ||
          (isset($attributes['height']) && !empty($attributes['height']))) {
        $src = $embed_object->field_photofile[0]['filepath'];
        $dimensions .= $attributes['width'] ? 'width="'.$attributes['width'].'"' : '';
        $dimensions .= $attributes['height'] ? 'height="'.$attributes['height'].'"' : '';
      }
      if (!empty($src)) {
        $src = "src=\"$src\"";
        $align = $attributes['align'] ? 'align="'.$attributes['align'].'"' : '';
        $runaround = $attributes['runaround'] ? 'style="margin:'.$attributes['runaround'].'px;"' : '';
        $alt = (isset($embed_object->field_summary[0]['value']) && !empty($embed_object->field_summary[0]['value'])) ?
          ' alt="'.$embed_object->field_summary[0]['value'].'"' :
          '';
        $attributes = $dimensions.$align.$runaround.$alt;
        $vars['rendered_object'] = "<img $src$attributes />";
      }
      break;
  
    default :
      break;
  }
}
