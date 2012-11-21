<?php

/**
 * Replace tags with others
 * @param string $tag tag to replace
 * @param array $settings settings of this replacement
 * @param string $text text to process
 */
function wallypressfilters_replace_tags($tag, $settings, $text, $type) {
  $classes = isset($settings['classes']) ? $settings['classes'] : '';
  $new_tag = $settings['html_tag'];

  // Mapping between old and new tags
  $settings_attr = explode(PHP_EOL, $settings['attributes']);
  $handled_attr = array();
  foreach ($settings_attr as $set_attr) {
    $tmp_attr = explode('|', $set_attr);
    $handled_attr[$tmp_attr[0]] = $tmp_attr[1];
  }

  // Get all attributes
  $tag_regex = "@<$tag\s*(.*)\s*>@U";
  $tag_matches = array();
  preg_match_all($tag_regex, $text, $tag_matches);

  foreach ($tag_matches[0] as $tag_key => $tag_match) {
    // Extra test needed to only replace exact match
    $extracted_tag_regex = "@<(.*)[\s>]@U";
    $extracted_tag_match = array();
    preg_match_all($extracted_tag_regex, $tag_match, $extracted_tag_match);
    if ($extracted_tag_match[1][0] != $tag) continue;

    $new_attr = '';
    $old_classes = '';

    $old_attr = $tag_matches[1][$tag_key];
    $attr_regex = '@(\w+)\s?=\s?"(.*)"@U';
    $attr_matches = array();
    preg_match_all($attr_regex, $old_attr, $attr_matches);
    foreach ($attr_matches[1] as $attr_key => $attr_match) {
      if (array_key_exists($attr_match, $handled_attr)) {
        // This attribute as a mapping
        $new_attr .= ' '.$handled_attr[$attr_match].'="'.$attr_matches[2][$attr_key].'"';
      } elseif ($attr_match == 'class') {
        // Keep old classes
        $old_classes = $attr_matches[2][$attr_key];
      } else {
        // No matching for this one, just copy it
        $new_attr .= ' '.$attr_matches[0][$attr_key];
      }
    }
    
    $new_classes_array = array();
    if (!empty($old_classes)) $new_classes_array[] = $old_classes;
    if (!empty($classes)) $new_classes_array[] = $classes;
    $new_classes = empty($new_classes_array) ? '' : implode(' ', $new_classes_array);

    $replace_string = "<$new_tag";
    $replace_string .= empty($new_attr) ? "" : " $new_attr";
    $replace_string .= empty($new_classes) ? "" : " class=\"$new_classes\"";
    $replace_string .= $type == 'marker' ? '/>' : '>';
    $text = str_replace($tag_match, $replace_string, $text);
  }

  // Then we manage the end tags
  $text = preg_replace("@<\/$tag>@U", "</$new_tag>", $text);

  return $text;
}

/**
 * Remove tags and their content
 * @param string $tag tag to remove
 * @param array $settings settings of this replacement
 * @param string $text text to process
 */
function wallypressfilters_strip_tags($tag, $settings, $text) {
  $tag_regex = "@<(\/)?$tag.*>@U";
  $tag_matches = array();
  preg_match_all($tag_regex, $text, $tag_matches);

  foreach ($tag_matches[0] as $tag_key => $tag_match) {
    // Extra test needed to only replace exact match
    $extracted_tag_regex = "@$tag"."[\s>\/]@U";
    $extracted_tag_match = array();
    preg_match_all($extracted_tag_regex, $tag_match, $extracted_tag_match);
    if (empty($extracted_tag_match) || empty($extracted_tag_match[0])) continue;

    $text = str_replace($tag_match, '', $text);
  }

  return $text;
}

/**
 * Remove tags' content
 * @param string $tag tag to remove
 * @param array $settings settings of this replacement
 * @param string $text text to process
 */
function wallypressfilters_strip_contents_tags($tag, $settings, $text) {
  $text = preg_replace("@<$tag"."[\s>].*<\/$tag>@", '', $text);
  return $text;
}

/**
 * Replace markers with some text
 * @param string $tag tag to replace
 * @param array $settings settings of this replacement
 * @param string $text text to process
 */
function wallypressfilters_convert_tags($tag, $settings, $text) {
  $tag_regex = "@<$tag.*>@U";
  $tag_matches = array();
  preg_match_all($tag_regex, $text, $tag_matches);
  
  foreach ($tag_matches[0] as $tag_key => $tag_match) {
    // Extra test needed to only replace exact match
    $extracted_tag_regex = "@$tag"."[\s\/]@U";
    $extracted_tag_match = array();
    preg_match_all($extracted_tag_regex, $tag_match, $extracted_tag_match);
    if (empty($extracted_tag_match) || empty($extracted_tag_match[0])) continue;
  
    $text = str_replace($tag_match, $settings['convert_text'], $text);
  }

  return $text;
}

/**
 * Replace tags with others according to mapping
 * @param string $tag tag to replace
 * @param array $settings settings of this replacement
 * @param string $text text to process
 */
function wallypressfilters_attr_replace_tags($tag, $settings, $text, $type) {
  if (!empty($settings['attributes-replacement-settings'])) {
    // Mapping between old and new tags
    $settings_attr = explode(PHP_EOL, $settings['attributes-replacement-settings']);
    foreach ($settings_attr as $set_attr) {
      $tmp_attr = explode('|', $set_attr);
      if (sizeof($tmp_attr) == 3) {
        $attr = $tmp_attr[0];
        $val = $tmp_attr[1];
        $new_tag = $tmp_attr[2];

        switch ($settings['attributes-replacement-sub-operation']) {
          case 'to_tag':
            $str_to_find = "@<$tag([^>]*)$attr\s?=\s?\"$val\"(.*)>(.*)<\/$tag>@U";
            $new_str = "<$new_tag $1$2>$3</$new_tag>";
            $text = preg_replace($str_to_find, $new_str, $text);
            break;

          case 'to_marker':
            $str_to_find = "@<$tag([^>]*)$attr\s?=\s?\"$val\"(.*)>";
            $str_to_find .= ($type == 'tag') ? "(.*)<\/$tag>@U" : "@U";
            $new_str = "<$new_tag $1$2>";
            $new_str .= ($type == 'tag') ? "$3" : "";
            $text = preg_replace($str_to_find, $new_str, $text);
            break;

          case 'convert':
            $str_to_find = "@<$tag([^>]*)$attr\s?=\s?\"$val\"(.*)>";
            $str_to_find .= ($type == 'tag') ? "(.*)<\/$tag>@U" : "@U";
            $new_str = $new_tag;
            $new_str .= ($type == 'tag') ? "$3" : "";
            $text = preg_replace($str_to_find, $new_str, $text);
            break;
        }
      }
    }
  }

  return $text;
}
