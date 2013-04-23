<?php
/**
 * Available vars :
 *  - $embed_object : Drupal node of the inline object to process
 *  - $attributes : array of attributes found in the inline tag
 *  - $rendered_object : the rendered object
 */

$rendered_object = '';

switch ($embed_object->type) {
  case 'wally_photoobject' :
    $rendered_object = _cciinlineobjects_preprocess_filter_render_photo($embed_object, $attributes);
    break;

  case 'wally_videoobject':
    $rendered_object = _cciinlineobjects_preprocess_filter_render_video($embed_object, $attributes);
    break;

  case 'wally_linktype':
    $rendered_object = _cciinlineobjects_preprocess_filter_render_link($embed_object, $attributes);
    break;

  default :
    break;
}

print $rendered_object;
?>