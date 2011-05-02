<?php
// $Id: cache_actions.views.inc,v 1.2.4.3 2010/09/30 10:46:24 fabsor Exp $

/**
 * @file
 * This file contains views hooks.
 */

/**
 * Implementation of hook_views_plugins().
 */
function cache_actions_views_plugins() {
  return array(
    'cache' => array(
      'rules' => array(
        'title' => t('Rules-triggered cache'),
        'help' => t('Caches the view for an inifinite amount of time
        and lets rules handle the clearing of the cache.'),
        'handler' => 'views_plugin_cache_rules',
        'help-topic' => 'cache-rules',
      ),
    ),
  );
}
