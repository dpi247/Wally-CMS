<?php

/**
 * Implementation of hook_default_page_manager_handlers().
 */
function wallydefaultpages_default_page_manager_handlers() {
  $export = array();
  $handler = new stdClass;
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'term_view_http_response';
  $handler->task = 'term_view';
  $handler->subtask = '';
  $handler->handler = 'http_response';
  $handler->weight = -28;
  $handler->conf = array(
    'title' => 'Topic-Destination Redirection',
    'arguments' => array(
      'argument_terms_1' => array(
        'keyword' => 'term',
        'identifier' => variable_get('page_manager_term_view_type', 'multiple') == 'multiple' ? t('Term(s) being viewed') : t('Term being viewed'),
        'id' => 1,
        'name' => variable_get('page_manager_term_view_type', 'multiple') == 'multiple' ? 'terms' : 'term',
        'settings' => array('input_form' => 'tid', 'breadcrumb' => variable_get('page_manager_taxonomy_breadcrumb', TRUE)),
        'default' => '404',
      ),
    ),
    'contexts' => array(),
    'relationships' => array(
      0 => array(
        'context' => 'argument_terms_1',
        'name' => 'topictermfromterm',
        'id' => 1,
        'identifier' => 'Topic Term of a Term',
        'keyword' => 'topicterm',
      ),
    ),
    'code' => '301',
    'destination' => 'taxonomy/term/%topicterm:tid',
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'topicterm',
          'settings' => NULL,
          'context' => 'argument_terms_1',
          'not' => FALSE,
        ),
        1 => array(
          'name' => 'term_vocabulary',
          'settings' => array(
            'vids' => array(
              2 => 2,
            ),
          ),
          'context' => 'argument_terms_1',
          'not' => TRUE,
        ),
      ),
      'logic' => 'and',
    ),
  );

  $export['term_view_http_response'] = $handler;
  $handler = new stdClass;
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'term_view_panel_context';
  $handler->task = 'term_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = -29;
  $handler->conf = array(
    'title' => 'Default Destination Page',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'access' => array(
      'plugins' => array(
        1 => array(
          'name' => 'term_vocabulary',
          'settings' => array(
            'vids' => array(
              2 => 2,
            ),
          ),
          'context' => 'argument_terms_1',
          'not' => FALSE,
        ),
      ),
      'logic' => 'and',
    ),
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
        0 => 'argument_terms_1.tids',
      ),
      'override_title' => 0,
      'override_title_text' => '',
      'wally_rule' => '15',
      'required context' => NULL,
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
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;

  $export['term_view_panel_context'] = $handler;
  $handler = new stdClass;
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'term_view_panel_context_2';
  $handler->task = 'term_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = -30;
  $handler->conf = array(
    'title' => 'Default Topic-Destination page',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(
      0 => array(
        'context' => 'argument_terms_1',
        'name' => 'topictermfromterm',
        'id' => 1,
        'identifier' => 'Topic Term of a Term',
        'keyword' => 'topicterm',
      ),
    ),
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'topicterm',
          'settings' => NULL,
          'context' => 'argument_terms_1',
          'not' => FALSE,
        ),
        1 => array(
          'name' => 'term_vocabulary',
          'settings' => array(
            'vids' => array(
              2 => 2,
            ),
          ),
          'context' => 'argument_terms_1',
          'not' => FALSE,
        ),
      ),
      'logic' => 'and',
    ),
  );
  $display = new panels_display;
  $display->layout = 'detailpage';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'header' => NULL,
      'colone' => NULL,
      'coltwo' => NULL,
      'colthree' => NULL,
      'footer' => NULL,
      'colmain' => NULL,
      'colrelated' => NULL,
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
      'required context' => NULL,
      'override_title' => 0,
      'override_title_text' => '',
      'wally_rule' => '15',
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
    $pane->panel = 'colmain';
    $pane->type = 'views_panes';
    $pane->subtype = 'nodes_from_term-panel_pane_1';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => array(
        0 => 'argument_terms_1',
      ),
      'wally_rule' => '15',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $display->content['new-2'] = $pane;
    $display->panels['colmain'][1] = 'new-2';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;

  $export['term_view_panel_context_2'] = $handler;
  $handler = new stdClass;
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'term_view_panel_context_3';
  $handler->task = 'term_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = -27;
  $handler->conf = array(
    'title' => 'Default Topic page',
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
    $pane->subtype = 'nodes_from_term-panel_pane_1';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'wally_rule' => '15',
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
    $pane->type = 'term_list';
    $pane->subtype = 'term_list';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'title' => NULL,
      'type' => 'related',
      'list_type' => 'ul',
      'context' => 'argument_terms_1',
      'override_title' => 0,
      'override_title_text' => '',
      'wally_rule' => '15',
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

  $export['term_view_panel_context_3'] = $handler;
  return $export;
}
