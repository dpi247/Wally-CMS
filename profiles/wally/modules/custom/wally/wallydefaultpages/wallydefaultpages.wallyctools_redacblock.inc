<?php

/**
 * Implementation of hook_default_wallyctools().
 */
function wallydefaultpages_default_wallyctools() {
  $export = array();

  $redacblock = new stdClass;
  $redacblock->disabled = FALSE; /* Edit this to true to make a default redacblock disabled initially */
  $redacblock->api_version = 1;
  $redacblock->name = 'destination_redacblock_sidebar';
  $redacblock->admin_title = 'Sidebar';
  $redacblock->admin_description = '';
  $redacblock->category = 'Redac Block';
  $redacblock->view_name = 'default_destination_view';
  $redacblock->view_id = 'redacblock_1';
  $redacblock->settings = array(
    'title' => 'Sidebar',
    'substitute' => 0,
    'override_pager_settings' => 0,
    'use_pager' => 0,
    'use_splitted' => 0,
    'nodes_per_page' => '10',
    'total_nodes_per_page' => '',
    'split_index_count' => '',
    'split_total_count' => '',
    'hide_pager' => 0,
    'pager_id' => '0',
    'offset' => '0',
    'more_link' => 0,
    'link_to_view' => 0,
    'panel_args' => 0,
    'args' => '',
    'url' => '',
  );
  $export['destination_redacblock_sidebar'] = $redacblock;

  return $export;
}
