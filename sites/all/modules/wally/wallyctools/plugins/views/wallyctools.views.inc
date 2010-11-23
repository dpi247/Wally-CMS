<?php

/**
 * @file
 * Contains Views plugin definitions for the panel pane display.
 */

/**
 * Implementation of hook_views_plugins
 */
function wallyctools_views_plugins() {
  return array(
    'display' => array(
      'redacblock' => array(
        'title' => t('Redac block'),
        'help' => t('TODO....'),
        'handler' => 'wallyctools_plugin_display_panel_pane',
        'path' => drupal_get_path('module', 'wallyctools') . '/plugins/views',
        'theme path' => drupal_get_path('module', 'views') . '/theme',
        'theme' => 'views_view',
        'use ajax' => TRUE,
        'use pager' => TRUE,
        'use more' => TRUE,
        'accept attachments' => TRUE,
        'help topic' => 'display-pane',
        'admin' => t('Content pane'),
      ),
    ),
  );
}
