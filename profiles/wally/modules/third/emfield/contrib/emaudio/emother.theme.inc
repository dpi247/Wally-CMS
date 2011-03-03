<?php
// $Id: emother.theme.inc $

/**
 * @file
 * This defines the various theme functions for Embedded Other Field (emother).
 */

/**
 * This will return a 'embed code' to embed the media in a page
 *
 * @param $field
 *   This is the field providing settings for the embed media.
 * @param $item
 *   This is the data returned by the field. It requires at the least to be an array with 'value' and 'provider'.
 *   $item['value'] will be the media code, and $item['provider'] will be the provider, such as coveritlive.
 * @param $formatter
 *   This is the formatter for the view.
 * @param $node
 *   This is the node object containing the field.
 * @param $options
 *   Optional array. This is to pass optional overrides.
 * @return
 *   The embed output.
 */
function theme_emother_other_embed($field, $item, $formatter, $node, $options = array()) {
  /*
    Note you can use this in node.tpl.php, substituting the proper field type:
    $field_type = 'field_other';
    $system_types = _content_type_info();
    $field = $system_types['fields'][$field_type];
    $field['widget'] = $system_types['content types'][$node->type]['fields'][$field_type]['widget'];
    print theme('emother_other_embed', $field, $node->{$field_type}[0], 'emother_embed', $node);
   */
    
  if ($item['value'] && $item['provider']) {
    $output = drupal_get_form('emother_embed_form', $field, $item, $formatter, $node, $options);
  }

  return $output;
}
/**
 * This will return a provided thumbnail image for a media.
 *
 * @param $field
 *   This is the field providing settings for the media thumbnail.
 * @param $item
 *   This is the data returned by the field. It requires at the least to be an array with 'value' and 'provider'.
 *   $item['value'] will be the media code, and $item['provider'] will be the provider, such as coveritlive.
 * @param $formatter
 *   This is the formatter for the view. This will nearly always be other_thumbnail.
 * @param $node
 *   This is the node object containing the field.
 * @param $no_link
 *   Optional. If FALSE, then we provide a link to the node.
 * @param $options
 *   Optional array. This is to pass optional overrides.
 * @return
 *   The thumbnail output.
 */
function theme_emother_other_thumbnail($field, $item, $formatter, $node, $no_link = FALSE, $options = array()) {
    
  if ($item['value'] && $item['provider']) {
    // If we've set $options['thumbnail_url'], then we'll just use that.
    // Otherwise, if we have emthumb installed, then give it a chance to override our thumbnail
    $thumbnail_url = $options['thumbnail_url'] ? $options['thumbnail_url'] : module_invoke('emthumb', 'thumbnail_url', $item);

    // if we don't have a custom thumbnail, then see if the provider gives us a thumbnail
    $thumbnail_url = $thumbnail_url ? $thumbnail_url : module_invoke('emfield', 'include_invoke', 'emother', $item['provider'], 'thumbnail', $field, $item, $formatter, $node, $width, $height, $options);

    // If we still don't have a thumbnail, then apply a default thumbnail, if it exists.
    if (!$thumbnail_url) {
      $default_thumbnail_url = $field['widget']['thumbnail_default_path'] ? $field['widget']['thumbnail_default_path'] : variable_get('emother_default_thumbnail_path', NULL);

      if ($default_thumbnail_url) {
        $thumbnail_url = base_path() . $default_thumbnail_url;
      }
    }
  }
  else {
    // Seems to be an unknown other media. Apply a default thumbnail, if it exists.
    if (!$thumbnail_url) {
      $default_thumbnail_url = $field['widget']['thumbnail_default_path'] ? $field['widget']['thumbnail_default_path'] : variable_get('emother_default_thumbnail_path', NULL);
      if ($default_thumbnail_url) {
        $thumbnail_url = base_path() . $default_thumbnail_url;
      }
    }
  }

  $link_url = isset($options['link_url']) ? $options['link_url'] : 'node/'. $node->nid;
  $link_title = isset($options['link_title']) ? $options['link_title'] : t('See other media');
  $image_title = isset($options['image_title']) ? $options['image_title'] : $link_title;
  $image_alt = isset($options['image_alt']) ? $options['image_alt'] : $link_title;

  if ($thumbnail_url) {
    $width = isset($options['width']) ? $options['width'] : NULL;
    $width = isset($width) ? $width : ($field['widget']['thumbnail_width'] ? $field['widget']['thumbnail_width'] : variable_get('emother_default_thumbnail_width', EMOTHER_DEFAULT_THUMBNAIL_WIDTH));
    $height = isset($options['height']) ? $options['height'] : NULL;
    $height = isset($height) ? $height : ($field['widget']['thumbnail_height'] ? $field['widget']['thumbnail_height'] : variable_get('emother_default_thumbnail_height', EMOTHER_DEFAULT_THUMBNAIL_HEIGHT));
    if ($no_link) { //thickbox requires the thumbnail returned without the link
      $output = '<img src="'. $thumbnail_url .'" width="'. $width  .'" height="'. $height  .'" alt="'. $image_alt .'" title="'. $image_title .'" />';
    }
    else {
      $output = l('<img src="'. $thumbnail_url .'" width="'. $width  .'" height="'. $height  .'" alt="'. $image_alt .'" title="'. $image_title .'" />', $link_url, array('html'=> true));
    }
  }
  else {
     // if all else fails, then just print a 'see other media' link.
    if ($no_link) {
      $output = ''; //thickbox won't work without a thumbnail
    }
    else {
      $output = l($link_title, $link_url);
    }
  }

  return $output;
}

/**
 * This will return a the full size media to embed in a page
 *
 * @param $field
 *   This is the field providing settings for the embed media.
 * @param $item
 *   This is the data returned by the field. It requires at the least to be an array with 'value' and 'provider'.
 *   $item['value'] will be the media code, and $item['provider'] will be the provider, such as coveritlive.
 * @param $formatter
 *   This is the formatter for the view.
 * @param $node
 *   This is the node object containing the field.
 * @param $options
 *   Optional array. This is to pass optional overrides.
 * @return
 *   The output.
 */
function theme_emother_other_other($field, $item, $formatter, $node, $options = array()) {
    
  if ($item['value'] && $item['provider']) {
    $embed = $item['value'];
    $width = isset($options['width']) ? $options['width'] : (isset($field['widget']['other_width']) ? $field['widget']['other_width'] : variable_get('emother_default_full_width', EMOTHER_DEFAULT_FULL_WIDTH));
    $height = isset($options['height']) ? $options['height'] : (isset($field['widget']['other_height']) ? $field['widget']['other_height'] : variable_get('emother_default_full_height', EMOTHER_DEFAULT_FULL_HEIGHT));
    $autoplay = isset($options['autoplay']) ? $options['autoplay'] : $field['widget']['audio_autoplay'];
    $output = module_invoke('emfield', 'include_invoke', 'emother', $item['provider'], 'other', $embed, $width, $height, $field, $item, $node ,$autoplay, $options);
  }

  return $output;
}
/**
 * This will return a the default sized media to embed in a page, here it's the fullsized media.
 *
 * @param $field
 *   This is the field providing settings for the embed media.
 * @param $item
 *   This is the data returned by the field. It requires at the least to be an array with 'value' and 'provider'.
 *   $item['value'] will be the media code, and $item['provider'] will be the provider, such as coveritlive.
 * @param $formatter
 *   This is the formatter for the view.
 * @param $node
 *   This is the node object containing the field.
 * @param $options
 *   Optional array. This is to pass optional overrides.
 * @return
 *   The output.
 */
function theme_emother_default($field, $item, $formatter, $node, $options = array()) {
    
  return theme('emother_other_other', $field, $item, $formatter, $node, $options);
}

/**
 * This will return a the preview sized media to embed in a page
 *
 * @param $field
 *   This is the field providing settings for the embed media.
 * @param $item
 *   This is the data returned by the field. It requires at the least to be an array with 'value' and 'provider'.
 *   $item['value'] will be the media code, and $item['provider'] will be the provider, such as coveritlive.
 * @param $formatter
 *   This is the formatter for the view.
 * @param $node
 *   This is the node object containing the field.
 * @param $options
 *   Optional array. This is to pass optional overrides.
 * @return
 *   The output.
 */
function theme_emother_other_preview($field, $item, $formatter, $node, $options = array()) {
    
  if ($item['value'] && $item['provider']) {
    $embed = $item['value'];
    $width = isset($options['width']) ? $options['width'] : (isset($field['widget']['preview_width']) ? $field['widget']['preview_width'] : variable_get('emother_default_preview_width', EEMOTHER_DEFAULT_PREVIEW_WIDTH));
    $height = isset($options['height']) ? $options['height'] : (isset($field['widget']['preview_height']) ? $field['widget']['preview_height'] : variable_get('emother_default_preview_height', EMOTHER_DEFAULT_PREVIEW_HEIGHT));
    $autoplay = isset($options['autoplay']) ? $options['autoplay'] : $field['widget']['audio_autoplay'];
    $output = module_invoke('emfield', 'include_invoke', 'emother', $item['provider'], 'preview', $embed, $width, $height, $field, $item, $node, $autoplay, $options);
  }

  return $output;
}

/*
 * Formatter theme functions
 */

function theme_emother_formatter_other_flash($element) {
    
  $field = content_fields($element['#field_name'], $element['#type_name']);
  return module_invoke('emother', 'field_formatter', $field, $element['#item'], $element['#formatter'], $element['#node']);
}
function theme_emother_formatter_other_embed($element) {
    
  $field = content_fields($element['#field_name'], $element['#type_name']);
  return module_invoke('emother', 'field_formatter', $field, $element['#item'], $element['#formatter'], $element['#node']);
}

function theme_emother_formatter_other_thumbnail($element) {
    
  $field = content_fields($element['#field_name'], $element['#type_name']);
  return module_invoke('emother', 'field_formatter', $field, $element['#item'], $element['#formatter'], $element['#node']);
}

function theme_emother_formatter_default($element) {
    
  $field = content_fields($element['#field_name'], $element['#type_name']);
  return module_invoke('emother', 'field_formatter', $field, $element['#item'], $element['#formatter'], $element['#node']);
}

function theme_emother_formatter_other_preview($element) {
    
  $field = content_fields($element['#field_name'], $element['#type_name']);
  return module_invoke('emother', 'field_formatter', $field, $element['#item'], $element['#formatter'], $element['#node']);
}

function theme_emother_formatter_other_other($element) {
        
  $field = content_fields($element['#field_name'], $element['#type_name']);
  return module_invoke('emother', 'field_formatter', $field, $element['#item'], $element['#formatter'], $element['#node']);
}
