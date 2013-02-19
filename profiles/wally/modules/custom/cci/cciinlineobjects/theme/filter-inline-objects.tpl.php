<?php
/**
 * Available vars :
 *  - $embed_object : Drupal node of the inline object to process
 *  - $attributes : array of attributes found in the inline tag
 */

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
      $attributes = $dimensions.$align.$runaround;
      print "<img $src$attributes />";
    }
    break;
    
  default :
    break;
}
?>