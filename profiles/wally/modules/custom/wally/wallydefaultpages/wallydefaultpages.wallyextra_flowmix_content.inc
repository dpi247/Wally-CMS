<?php

/**
 * Implementation of hook_wallyextra_flowmix_content_obj().
 */
function wallydefaultpages_wallyextra_flowmix_content_obj() {
  $export = array();

  $wallyextra_flowmix = new stdClass;
  $wallyextra_flowmix->disabled = FALSE; /* Edit this to true to make a default wallyextra_flowmix disabled initially */
  $wallyextra_flowmix->api_version = 1;
  $wallyextra_flowmix->name = 'flowmix_drupal_org';
  $wallyextra_flowmix->title = '';
  $wallyextra_flowmix->admin_title = 'Flowmix Drupal.org';
  $wallyextra_flowmix->admin_description = '';
  $wallyextra_flowmix->category = 'Flowmixes';
  $wallyextra_flowmix->settings = (object) array(
    'title' => 'Drupal.org',
    'url' => '',
    'maxitem_global' => '10',
    'mix_schema' => 'alt',
    'own_schema' => '',
    'override_theme' => 0,
    'override_theme_text' => '',
    'use_ajax' => 1,
    'bypass_cache' => 0,
    'caid' => 'flowmix_1373557612',
    'flows' => array(
      0 => (object) array(
        'name' => 'Drupal.org',
        'type' => 'RSS 2.0',
        'body_html' => '1',
        'plugin_name' => 'rss',
        'conf' => array(
          'uri' => 'https://drupal.org/node/feed',
          'request_timeout' => '3',
          'maxitem' => '10',
          'embed_img' => 1,
          'channel_img' => 0,
          'get_distant_img' => 0,
        ),
      ),
    ),
  );
  $export['flowmix_drupal_org'] = $wallyextra_flowmix;

  return $export;
}
