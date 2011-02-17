<?php

/**
 * Implementation of hook_default_page_manager_handlers().
 */
function wallydefaultpages_default_page_manager_handlers() {
  $export = array();
  $handler = new stdClass;
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'term_view_panel_context';
  $handler->task = 'term_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Default Destination Page',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display;
  $display->layout = 'detailpage';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'header' => NULL,
      'colmain' => NULL,
      'colrelated' => NULL,
      'footer' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass;
    $pane->pid = 'new-1';
    $pane->panel = 'colmain';
    $pane->type = 'redacblock';
    $pane->subtype = 'default_destination_block';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'admin_title' => NULL,
      'title' => NULL,
      'override_pager_settings' => 0,
      'use_pager' => 0,
      'nodes_per_page' => '10',
      'pager_id' => '0',
      'offset' => '0',
      'more_link' => 0,
      'substitute' => NULL,
      'link_to_view' => 0,
      'panel_args' => 0,
      'args' => '',
      'url' => '',
      'context' => array(
        0 => 'argument_terms_1.tid',
      ),
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $display->content['new-1'] = $pane;
    $display->panels['colmain'][0] = 'new-1';
    $pane = new stdClass;
    $pane->pid = 'new-2';
    $pane->panel = 'colrelated';
    $pane->type = 'views_panes';
    $pane->subtype = 'fil_info-panel_pane_1';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'available_dests' => array(
        'News' => (object) array(
  'tid' => '1',
  'vid' => '2',
  'name' => 'News',
  'description' => 'All about news',
  'weight' => '1',
  'depth' => 0,
  'parents' => array(
    0 => '0',
  ),
),
        'Belgique' => (object) array(
  'tid' => '194',
  'vid' => '2',
  'name' => 'Belgique',
  'description' => '',
  'weight' => '0',
  'depth' => 1,
  'parents' => array(
    0 => '1',
  ),
),
        'Bruxelles' => (object) array(
  'tid' => '195',
  'vid' => '2',
  'name' => 'Bruxelles',
  'description' => '',
  'weight' => '0',
  'depth' => 2,
  'parents' => array(
    0 => '194',
  ),
),
      ),
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $display->content['new-2'] = $pane;
    $display->panels['colrelated'][0] = 'new-2';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-1';
  $handler->conf['display'] = $display;

  $export['term_view_panel_context'] = $handler;
  return $export;
}
