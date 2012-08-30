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
  $tag_regex = '@<'.$tag.'\s*(.*)\s*>@U';
  $tag_matches = array();
  preg_match_all($tag_regex, $text, $tag_matches);
  
  foreach ($tag_matches[0] as $tag_key => $tag_match) {
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

    $replace_string = '<'.$new_tag.$new_attr.' class="'.$old_classes.' '.$classes.'"';
    $replace_string .= $type == 'marker' ? '/>' : '>';
    $text = str_replace($tag_match, $replace_string, $text);
  }

  // Then we manage the end tags
  $text = preg_replace('@<\/'.$tag.'>@U', '</'.$new_tag.'>', $text);

  return $text;
}

/**
 * Remove tags and their content
 * @param string $tag tag to remove
 * @param array $settings settings of this replacement
 * @param string $text text to process
 */
function wallypressfilters_strip_tags($tag, $settings, $text) {
  $text = preg_replace('@<(\/)?'.$tag.'.*>@U', '', $text);
  return $text;
}

/**
 * Remove tags' content
 * @param string $tag tag to remove
 * @param array $settings settings of this replacement
 * @param string $text text to process
 */
function wallypressfilters_strip_contents_tags($tag, $settings, $text) {
  $text = preg_replace('@<'.$tag.'.*>.*<\/'.$tag.'>@', '', $text);
  return $text;
}

/**
 * Replace markers with some text
 * @param string $tag tag to replace
 * @param array $settings settings of this replacement
 * @param string $text text to process
 */
function wallypressfilters_convert_tags($tag, $settings, $text) {
  $text = preg_replace('@<'.$tag.'(.*)>@U', $settings['convert_text'], $text);
  return $text;
}
