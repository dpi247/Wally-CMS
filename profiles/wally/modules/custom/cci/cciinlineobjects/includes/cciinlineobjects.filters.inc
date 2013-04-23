<?php

/**
 * Process a text and replace inline objects tag by actual objects
 * @param string $text text to process
 */
function cciinlineobjects_inline_objects_tags($text, $prenode_cache_name = FALSE) {
  $cleanup_regex = "@<processedinline>.*<\/processedinline>@sU";
  $text = preg_replace($cleanup_regex, '', $text);

  $tag_regex = "@<ccix:inline(\s.*)?>.*<\/ccix:inline(\s.*)?>@sU";
  $tag_matches = array();
  preg_match_all($tag_regex, $text, $tag_matches);

  if (!empty($tag_matches[0])) {
    if (!$prenode_cache_name) {
      $first_tag_values = _cciinlineobjects_inline_objects_get_tag_values($tag_matches[1][0]);
      $prenode_cache_name = (isset($first_tag_values['prenid']) && !empty($first_tag_values['prenid'])) ? $first_tag_values['prenid'] : FALSE;
    }

    if ($prenode_cache_name) {
      // We are in WallyEdit or in preview

      module_load_include('inc', 'wallyedit', 'includes/Prenode');
      $prenode = prenode_load($prenode_cache_name, array());
      $form_state = $prenode->form_state;
      $embed_objects = isset($form_state[$form_state['type']]['field_embededobjects']) ? $form_state[$form_state['type']]['field_embededobjects'] : FALSE;

      if ($embed_objects && is_array($embed_objects) && !empty($embed_objects)) {
        foreach ($tag_matches[0] as $match_id => $match_val) {
          $tag_values = _cciinlineobjects_inline_objects_get_tag_values(trim($tag_matches[1][$match_id]));

          if (isset($tag_values['extref']) && !empty($tag_values['extref'])) {
            list($extref_type, $extref_val) = explode(':', $tag_values['extref']);
            $found_embed = FALSE;

            foreach ($embed_objects as $embed_object) {
              $embed_object = $embed_object['#node'];
              switch ($extref_type) {
                case 'nid':
                  if (isset($embed_object->nid) && $embed_object->nid == $extref_val) {
                    $found_embed = $embed_object;
                  }
                  break;

                case 'prenid':
                  if (isset($embed_object->prenid) && $embed_object->prenid == $extref_val) {
                    $found_embed = $embed_object;
                  }
                  break;

                default:
                  if (isset($embed_object->field_externalreference[0]['value']) && $embed_object->field_externalreference[0]['value'] == $tag_values['extref']) {
                    $found_embed = $embed_object;
                  }
                  break;
              }

              if ($found_embed) {
                $text = _cciinlineobjects_inline_objects_process_text($text, $match_val, $found_embed, $tag_values);
                break;
              }
            }
          }
        }
      }
    } else {
      // Standard treatment

      foreach ($tag_matches[0] as $match_id => $match_val) {
        $tag_values = _cciinlineobjects_inline_objects_get_tag_values(trim($tag_matches[1][$match_id]));

        if (isset($tag_values['extref']) && !empty($tag_values['extref'])) {
          list($extref_type, $extref_val) = explode(':', $tag_values['extref']);
          $found_embed = FALSE;

          switch ($extref_type) {
            case 'nid':
              if ($embed_object = node_load($extref_val)) {
                $found_embed = $embed_object;
              }
              break;

            default:
              if ($embed_nodes = wallytoolbox_get_node_by_cckfield(array('field_externalreference' => $tag_values['extref']))) {
                $found_embed = $embed_nodes[0];
              }
              break;
          }

          if ($found_embed) {
            $text = _cciinlineobjects_inline_objects_process_text($text, $match_val, $found_embed, $tag_values);
          }
        }
      }
    }
  }

  return $text;
}

function _cciinlineobjects_inline_objects_get_tag_values($raw_attributes) {
  $tag_values = array();
  $attr_regex = '@\w.*\s?=\s?[\'"].*[\'"]@U';
  $attr_matches = array();
  preg_match_all($attr_regex, $raw_attributes, $attr_matches);

  foreach ($attr_matches[0] as $raw_attr) {
    $expl_attr = explode('=', $raw_attr);
    $tag_values[$expl_attr[0]] = trim($expl_attr[1], '\'"');
  }

  return $tag_values;
}

function _cciinlineobjects_inline_objects_process_text($text, $match_val, $found_embed, $tag_values) {
  $processed_embed = theme('filter_inline_objects_preview', $found_embed, $tag_values);
  $processed_embed = $match_val.'<processedinline>'.$processed_embed.'</processedinline>';
  return is_string($processed_embed) ? str_replace($match_val, $processed_embed, $text) : $text;
}
