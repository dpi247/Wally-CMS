<?php

/**
 * @file
 *   Context module esi code.
 */

class context_reaction_esi_block extends context_reaction_block {
  protected function build_block($block, $reset = FALSE) {
    require_once(drupal_get_path('module', 'esi') . '/esi.inc');
    $config = esi_get_settings($block->module . '_' . $block->delta);

    if ($config['scope'] && variable_get('esi_mode', ESI_MODE)) {
      $block->content = theme('esi_tag', 'block', $block);
    }
    else {
      $array = module_invoke($block->module, 'block', 'view', $block->delta);
      if (isset($array) && is_array($array)) {
        foreach ($array as $k => $v) {
          $block->$k = $v;
        }
      }
    }

    if (!empty($block->content)) {
      // Only query for custom block title if block core compatibility is enabled.
      if (!variable_get('context_reaction_block_disable_core', FALSE)) {
        global $user, $theme_key;
        $block->title = db_result(db_query("SELECT title FROM {blocks} WHERE module = '%s' AND delta = '%s' AND theme = '%s'", $block->module, $block->delta, $theme_key));
      }
      // Override default block title if a custom display title is present.
      if (!empty($block->title)) {
        // Check plain here to allow module generated titles to keep any markup.
        $block->subject = $block->title == '<none>' ? '' : check_plain($block->title);
      }
      if (!isset($block->subject)) {
        $block->subject = '';
      }
    }
    return $block;
  }
}
