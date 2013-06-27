<?php

function _cciinlineobjects_preprocess_filter_render_photo($embed_object, $attributes) {
  $rendered_object = '';

  $dimensions = '';
  $photo_path = $embed_object->field_photofile[0]['filepath'];
  if (isset($attributes['preset']) && !empty($attributes['preset'])) {
    $src = imagecache_create_url($attributes['preset'], $photo_path);
  } elseif ((isset($attributes['width']) && !empty($attributes['width'])) ||
      (isset($attributes['height']) && !empty($attributes['height']))) {
    $src = $photo_path;
    $dimensions .= $attributes['width'] ? ' width="'.$attributes['width'].'"' : '';
    $dimensions .= $attributes['height'] ? ' height="'.$attributes['height'].'"' : '';
  }
  if (!empty($src)) {
    $styles = '';
    $src = "src=\"/$src\"";
    $align = $attributes['align'] ? 'align="'.$attributes['align'].'"' : '';
    if ($attributes['align'] && $attributes['align'] == 'center') {
      $tmp_style = 'display:block;margin:';
      $tmp_style .= $attributes['runaround'] ? $attributes['runaround'].'px' : '0';
      $tmp_style .= ' auto;';
      $styles = $tmp_style;
    } elseif ($attributes['runaround']) {
      $styles = 'margin:'.$attributes['runaround'].'px;';
    }
    $style = empty($styles) ? '' : " style=\"$styles\"";
    $alt = (isset($embed_object->field_summary[0]['value']) && !empty($embed_object->field_summary[0]['value'])) ? ' alt="'.$embed_object->field_summary[0]['value'].'"' : '';
    $attributes = $dimensions.$align.$alt.$style;
    $rendered_object = "<img $src$attributes />";
  }

  return $rendered_object;
}

function _cciinlineobjects_preprocess_filter_render_video($embed_object, $attributes) {
  $rendered_object = '';

  $url = 'http://'.$_SERVER['HTTP_HOST'].'/'.$embed->field_videofile[0]['filepath'];
  $item = array('embed' => $url);
  $modules = array('emvideo');
  $emfield = FALSE;
  foreach ($modules as $module) {
    $field = array();
    $item = _emfield_field_submit_id($field, $item, $module);

    if (!empty($item['provider'])) {
      $element = array(
        '#item' => $item,
        '#formatter' => 'default',
        '#node' => $node,
      );

      $function = $module.'_field_formatter';
      $rendered_object = $function($field, $element['#item'], $element['#formatter'], $element['#node']);
    }
  }

  return $rendered_object;
}

function _cciinlineobjects_preprocess_filter_render_link($embed_object, $attributes) {
  $rendered_object = '';

  if (isset($embed_object->field_link_item[0]['url']) && !empty($embed_object->field_link_item[0]['url']) && !strstr($embed_object->field_link_item[0]['url'], 'extref://')) {
    $item = array('embed' => $embed_object->field_link_item[0]['url']);
    if ($embed_object->field_link_item[0]['query'] != NULL){
      $item['embed'] .= '?'.$embed_object->field_link_item[0]['query'];
    }

    $modules = array('emvideo', 'emother', 'emimage', 'emaudio', 'embonus', 'emimport', 'eminline', 'emthumb', 'emwave', 'image_ncck', 'video_cck', 'slideshare');
    foreach ($modules as $module) {
      $item = _emfield_field_submit_id($field, $item, $module);
      if (!empty($item['provider'])) {
        $element = array(
          '#item' => $item,
          '#formatter' => 'default',
          '#node' => $node,
        );
        $content = emfield_emfield_field_formatter($field, $element['#item'], $element['#formatter'], $element['#node'], 'emvideo', $options);

        if ($module == "emimage" || $module == 'emvideo') {
          $dimensions = FALSE;
          if (isset($attributes['preset']) && !empty($attributes['preset'])) {
            $preset = imagecache_preset_by_name($attributes['preset']);
            $wally_crop_found = FALSE;
            foreach ($preset['actions'] as $action) {
              if ($action['action'] == 'wallycontenttypes_wallycrop') {
                $wally_crop_found = $action;
                $wally_crop_ratio = $wally_crop_found['data']['width'] / $wally_crop_found['data']['height'];
              }
              if ($action['action'] == 'imagecache_scale' && $wally_crop_found) {
                $width = $action['data']['width'];
                $height = $width / $wally_crop_ratio;
                $dimensions = TRUE;
                break;
              }
            }
            if ($wally_crop_found && !$dimensions) {
              $width = $wally_crop_found['data']['width'];
              $height = $wally_crop_found['data']['height'];
              $dimensions = TRUE;
            }
          } elseif ((isset($attributes['width']) && !empty($attributes['width'])) &&
              (isset($attributes['height']) && !empty($attributes['height']))) {
            $width = $attributes['width'];
            $height = $attributes['width'];
            $dimensions = TRUE;
          }
          if ($dimensions) {
            $content = preg_replace('+width=("|\')([0-9]{3})?("|\')+','width="'.$width.'"', $content);
            $content = preg_replace('+height=("|\')([0-9]{3})?("|\')+','height="'.$height.'"', $content);
          }
        }

        $rendered_object = $content;

        break;
      }
    }
  }

  return $rendered_object;
}

function _cciinlineobjects_preprocess_filter_render_preview($embed_object, $attributes) {
  $rendered_object = '';
  
  $dimensions = '';
  $photo_path = drupal_get_path('module', 'cciinlineobjects').'/images/preview_img.png';
  if (isset($attributes['preset']) && !empty($attributes['preset'])) {
    $src = imagecache_create_url($attributes['preset'], $photo_path);
  } elseif ((isset($attributes['width']) && !empty($attributes['width'])) ||
      (isset($attributes['height']) && !empty($attributes['height']))) {
    $src = $photo_path;
    $dimensions .= $attributes['width'] ? 'width="'.$attributes['width'].'"' : '';
    $dimensions .= $attributes['height'] ? 'height="'.$attributes['height'].'"' : '';
  }
  if (!empty($src)) {
    $src = "src=\"$src\"";
    $align = $attributes['align'] ? 'align="'.$attributes['align'].'"' : '';
    $runaround = $attributes['runaround'] ? 'style="margin:'.$attributes['runaround'].'px;"' : '';
    $alt = (isset($embed_object->field_summary[0]['value']) && !empty($embed_object->field_summary[0]['value'])) ? ' alt="'.$embed_object->field_summary[0]['value'].'"' : '';
    $attributes = $dimensions.$align.$runaround.$alt;
    $rendered_object = "<img $src$attributes />";
  }
  
  return $rendered_object;
}
