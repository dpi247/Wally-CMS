<?php

function wallypressfilters_mapping_export($filter_name/*, $format*/) {
  $mapping = new stdClass();
  $mapping->filter = $filter_name;
  $mapping->formats = array();
  $mapping->tags = array();
  
  for ($i = 1; $i <=2; $i++) {
    $format = $i;
    if ($format_infos = filter_formats($format)) {
      $mapping->formats[$format] = array(
        'format' => $format_infos->format,
        'name' => $format_infos->name,
      );
      
      $tags = wallypressfilters_gettagsfromfilterandformat($filter_name, $format);
      foreach ($tags as $tag) {
        if (!isset($mapping->tags[$tag->tag])) {
          $mapping->tags[$tag->tag] = array(
            'type' => $tag->type,
          );
        }
        $mapping->tags[$tag->tag][$format] = array(
          'operation' => $tag->settings['operation'],
          'html_tag' => $tag->settings['html_tag'],
          'classes' => $tag->settings['classes'],
        );
      }
    }
  }

  drupal_json($mapping);
}
