<?php

/**
 * Implementation of hook_default_page_manager_pages().
 */
function wn_home_page_default_page_manager_pages() {
  $page = new stdClass;
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'home_page';
  $page->task = 'page';
  $page->admin_title = 'Home page';
  $page->admin_description = '';
  $page->path = 'home';
  $page->access = array();
  $page->menu = array();
  $page->arguments = array();
  $page->conf = array();
  $page->default_handlers = array();
  $handler = new stdClass;
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_home_page_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'home_page';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Panel',
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
    $pane->type = 'views_panes';
    $pane->subtype = 'home_page_packages-panel_pane_1';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array();
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
      'override_title' => 0,
      'override_title_text' => '',
      'available_dests' => array(
        'News' => (object) array(
  'tid' => '1',
  'vid' => '2',
  'name' => 'News',
  'description' => 'All about news',
  'weight' => '1',
  'featid' => NULL,
  'depth' => 0,
  'parents' => array(
    0 => '0',
  ),
),
        'Sports' => (object) array(
  'tid' => '2',
  'vid' => '2',
  'name' => 'Sports',
  'description' => 'All about sports',
  'weight' => '2',
  'featid' => NULL,
  'depth' => 0,
  'parents' => array(
    0 => '0',
  ),
),
        'Economy' => (object) array(
  'tid' => '3',
  'vid' => '2',
  'name' => 'Economy',
  'description' => 'All about economy',
  'weight' => '3',
  'featid' => NULL,
  'depth' => 0,
  'parents' => array(
    0 => '0',
  ),
),
        'Politics' => (object) array(
  'tid' => '4',
  'vid' => '2',
  'name' => 'Politics',
  'description' => 'All about politic',
  'weight' => '4',
  'featid' => NULL,
  'depth' => 0,
  'parents' => array(
    0 => '0',
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
  $page->default_handlers[$handler->name] = $handler;
  $pages['home_page'] = $page;

 return $pages;

}
