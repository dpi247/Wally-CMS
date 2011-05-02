<?php
// $Id: wallyrules.rules.inc,v 1.0 $

/**
 * @file
 * This file provides the rules integration for this module.
 */

/**
 * Implementation of hook_rules_action_info().
 */
function wallyrules_rules_action_info() {
  $actions = array();
  
  $actions['wallyrules_flush_home_page_cache'] = array(
    'label' => t('Flush home page cache'),
    'module' => 'Wally Rules',
  );
  
  $actions['wallyrules_flush_node_page_cache'] = array(
    'label' => t('Flush the node page cache for this node'),
    'arguments' => array(
      'node' => array('type' => 'node', 'label' => t('Node')),
    ),
    'module' => 'Wally Rules',
  );
  
  $actions['wallyrules_flush_destinations_page_cache'] = array(
    'label' => t('Flush the destinations pages cache for this node'),
    'arguments' => array(
      'node' => array('type' => 'node', 'label' => t('Node')),
    ),
    'module' => 'Wally Rules',
  );
  
  $actions['wallyrules_flush_custom_page_cache'] = array(
    'label' => t('Flush custom pages cache'),
    'module' => 'Wally Rules',
  );
  
  $actions['wallyrules_flush_imagecache_presets'] = array(
    'label' => t('Flush imagecache presets for this node'),
    'arguments' => array(
      'node' => array('type' => 'node', 'label' => t('Node')),
    ),
    'module' => 'Wally Rules',
  );
  
  $actions['wallyrules_flush_blocks_cache'] = array(
    'label' => t('Flush blocks cache'),
    'module' => 'Wally Rules',
  );
  
  return $actions;
}

/**
 * Action to flush the page cache of the home page
 */
function wallyrules_flush_home_page_cache($settings) {
  _wallyrules_flush_all_pages($settings, 'home');
}

/**
 * Action to flush the page cache of the node page
 */
function wallyrules_flush_node_page_cache($node, $settings) {
  _wallyrules_flush_all_pages($settings, 'node/'.$node->nid);
}

/**
 * Action to flush the page cache of the destinations pages related to a node
 */
function wallyrules_flush_destinations_page_cache($node, $settings) {
  if (isset($node->field_destinations)) {
    $paths = array();
    foreach ($node->field_destinations as $destination)
      $paths[] = 'taxonomy/term/'.$destination['tid'];
    
    _wallyrules_flush_all_pages($settings, $paths);
  }
}

/**
 * Action to flush the page cache of custom pages
 */
function wallyrules_flush_custom_page_cache($settings) {
  $urls = explode("\n", str_replace(array("\n", "\r\n"), "\n", trim($settings['urls_to_flush'])));
  $paths = array();
  foreach ($urls as $url)
    $paths[] = drupal_get_normal_path(trim($url, '/'));
  
  _wallyrules_flush_all_pages($settings, $paths);
}

/**
 * Action to flush imagecache presets for this node
 */
function wallyrules_flush_imagecache_presets($node) {
  if (isset($node->field_photofile[0])) {
    imagecache_image_flush($node->field_photofile[0]['filepath']);
  }
}

/**
 * Action to flush blocks cache
 */
function wallyrules_flush_blocks_cache($settings) {
  foreach ($settings['blocks_to_flush'] as $module_name => $module_to_flush) {
    foreach ($module_to_flush as $block_id => $block_to_flush) {
      if ($block_to_flush !== 0) {
        if (substr($block_id, 0, 3) == 'id_') {
          $expl_id = explode('_', $block_id, 2);
          $block_id = $expl_id[1];
        }
        
        cache_clear_all($module_name.':'.$block_id, 'cache_block', TRUE);
      }
    }
  }
}

/**
 * Test the flush type (drupal, varnish or both) and call wallytoolbox_flush_all_pages
 */
function _wallyrules_flush_all_pages($settings, $paths) {
  if ($settings['caches_to_flush']['varnish_cache'] !== 0) {
    if ($settings['caches_to_flush']['drupal_cache'] !== 0) {
      $flush_type = 'both';
    } else {
      $flush_type = 'varnish';
    }
  } else {
    $flush_type = 'drupal';
  }
  
  wallytoolbox_flush_all_pages($paths, TRUE, $flush_type);
}
