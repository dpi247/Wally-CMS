<?php

function wallypressfilters_mapping_export($filter_name, $format) {
  $mapping = new stdClass();
  $mapping->filter = $filter_name;

  if ($format_infos = filter_formats($format)) {
    $mapping->format = array(
      'format' => $format_infos->format,
      'name' => $format_infos->name,
    );

    $mapping->tags = array();
    $tags = wallypressfilters_gettagsfromfilterandformat($filter_name, $format);
    foreach ($tags as $tag) {
      $mapping->tags[$tag->tag] = array(
        'type' => $tag->type,
        'operation' => $tag->settings['operation'],
        'html_tag' => $tag->settings['html_tag'],
        'classes' => $tag->settings['classes'],
      );
    }
  }

  drupal_json($mapping);
}
