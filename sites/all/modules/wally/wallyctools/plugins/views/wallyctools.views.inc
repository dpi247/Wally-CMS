<?php

/**
 * @file
 * Contains Views plugin definitions for the panel pane display.
 */

/**
 * Implementation of hook_views_plugins
 */
function wallyctools_views_plugins() {
   $path = drupal_get_path('module', 'wallyctools');
  $views_path = drupal_get_path('module', 'views');
  require_once "./$path/theme/theme.inc";
  
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
    /*
    'style' => array( //declare the views_json style plugin
      'redacblock_style' => array(
       'path' =>  drupal_get_path('module', 'wallyctools') . '/plugins/views',
        'title' => t('Redacblock Style'),
        'help' => t('Displays nodes in the JSON data format.'),
        'handler' => 'wallyctools_views_plugin_style_redacblock',
        'theme' => 'wallyctools_style_redacblock',
        'theme file' => 'theme.inc',
        'theme path' => drupal_get_path('module', 'wallyctools') . "/theme",
        'uses row plugin' => TRUE,
        //'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',      
      ),
    ),
    */
  'row' => array( //declare the unformatted row plugin
      'redacblock_row' => array(
        'title' => t('Redacblock Row'),
        'help' => t('(Displays the node according to the layout of his destination.'),
        'path' =>  drupal_get_path('module', 'wallyctools') . '/plugins/views',
        'handler' => 'wallyctools_views_plugin_row_redacblock',
        'theme' => 'wallyctools_row_redacblock',
        'theme file' => 'theme.inc',
        'theme path' => drupal_get_path('module', 'wallyctools') . "/theme",
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
      )
     )  
  );
}
