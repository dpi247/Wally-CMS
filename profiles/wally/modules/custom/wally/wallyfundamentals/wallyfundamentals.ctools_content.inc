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
  $redacblock->admin_title = 'Default Destination Block';
  $redacblock->admin_description = 'This block will display all packages, based on the Default Destination View. ( @see /admin/build/views/ ).  ';
  $redacblock->category = '';
  $redacblock->view_name = 'default_destination_view';
  $redacblock->view_id = 'redacblock_1';
  $redacblock->settings = array(
    'admin_title' => '',
    'title' => '',
    'override_pager_settings' => FALSE,
    'use_pager' => FALSE,
    'nodes_per_page' => 10,
    'pager_id' => 0,
    'offset' => 0,
    'more_link' => FALSE,
    'substitute' => 0,
    'link_to_view' => FALSE,
    'panel_args' => FALSE,
    'args' => '',
    'url' => '',
    'context' => array(),
  );

  $export['default_destination_block'] = $redacblock;
  return $export;
}
