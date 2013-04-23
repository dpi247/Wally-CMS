<?php

/**
 * Implementation of hook_default_wallyctools().
 */
function wallyfundamentals_default_wallyctools() {
  $export = array();
  $redacblock = new stdClass;
  $redacblock->disabled = FALSE; /* Edit this to true to make a default redacblock disabled initially */
  $redacblock->api_version = 1;
  $redacblock->name = 'default_destination_block';
  $redacblock->admin_title = 'default_destination_block';
  $redacblock->admin_description = '';
  $redacblock->category = '';
  $redacblock->view_name = 'default_destination_view';
  $redacblock->view_id = 'redacblock_1';
  $redacblock->settings = array(
    'title' => '',
    'substitute' => 0,
    'override_pager_settings' => 0,
    'use_pager' => 0,
    'nodes_per_page' => '10',
    'pager_id' => '0',
    'offset' => '0',
    'more_link' => 0,
    'link_to_view' => 0,
    'panel_args' => 0,
    'args' => '',
    'url' => '',
    'context' => array(
      0 => '',
    ),
  );

  $export['default_destination_block'] = $redacblock;
  return $export;
}
