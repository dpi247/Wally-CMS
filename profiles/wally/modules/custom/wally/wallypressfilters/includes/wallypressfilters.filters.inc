<?php

/**
 * Replace tags with others
 * @param string $tag tag to replace
 * @param array $settings settings of this replacement
 * @param string $text text to process
 */
function wallypressfilters_replace_tags($tag, $settings, $text) {
  $tag_regex = "@<$tag(\s.*)?>(.*)<\/$tag(\s.*)?>@sU";
  $tag_matches = array();
  preg_match_all($tag_regex, $text, $tag_matches);
  if (!empty($tag_matches[0])) {
    foreach ($tag_matches[0] as $match_id => $match_val) {
      $tag_values = array();
      $tag_values['name'] = $tag;
      $tag_values['body'] = $tag_matches[2][$match_id];
      $tag_values['attributes'] = array();

      $raw_attributes = trim($tag_matches[1][$match_id]);
      $attr_regex = '@\w.*\s?=\s?[\'"].*[\'"]@U';
      $attr_matches = array();
      preg_match_all($attr_regex, $raw_attributes, $attr_matches);
      foreach ($attr_matches[0] as $raw_attr) {
        $expl_attr = explode('=', $raw_attr, 2);
        $tag_values['attributes'][$expl_attr[0]] = trim($expl_attr[1], '\'"');
      }

      $replacement = '';
      switch ($settings['replace-method']) {
        case 'basic':
          $replacement = $settings['basic-pattern'];
          $replacement = str_replace('$tag.body', $tag_values['body'], $replacement); // Replace $tag.body with tag content
          foreach ($tag_values['attributes'] as $attr_name => $attr_val) {
            // Replace each $tag.xxx with the matching value
            $replacement = str_replace('$tag.'.$attr_name, $attr_val, $replacement);
          }
          $replacement = preg_replace('@\$tag.*([\s"])@U', '$1', $replacement); // Remove non used $tag.xxx
          $replacement = preg_replace('@\s[^\s]*\s?=\s?"\s*"@U', '', $replacement); // Remove empty attributes
          $replacement = preg_replace('@(=\s?")\s+([^"]*")@U', '$1$2', $replacement); // Remove unecessary spaces in attributes values
          break;
      
        case 'advanced':
          $tag = $tag_values;
          $replacement = eval($settings['advanced-php']);
          break;
      }

      $text = is_string($replacement) ? str_replace($match_val, $replacement, $text) : $text;
    }
  }

  return $text;
}

/**
 * Remove tags and their content
 * @param string $tag tag to remove
 * @param array $settings settings of this replacement
 * @param string $text text to process
 */
function wallypressfilters_strip_tags($tag, $settings, $text) {
  $tag_regex = "@<$tag(\s.*)?>(.*)<\/$tag>@U";
  $text = preg_replace($tag_regex, '$2', $text);
  return $text;
}

/**
 * Remove tags' content
 * @param string $tag tag to remove
 * @param array $settings settings of this replacement
 * @param string $text text to process
 */
function wallypressfilters_skip_tags($tag, $settings, $text) {
  $tag_regex = "@<$tag(\s.*)?>.*<\/$tag>@U";
  $text = preg_replace($tag_regex, '', $text);
  return $text;
}
