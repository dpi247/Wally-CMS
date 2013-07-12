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
  $handler->weight = -30;
  $handler->conf = array(
    'title' => 'Topic-Destination Redirection',
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
  $handler->weight = -27;
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
  $display->layout = 'base-layout-brick';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'header' => NULL,
      'colmain' => NULL,
      'colrelated' => NULL,
      'footer' => NULL,
      'contenttop' => NULL,
      'maincontentleft' => NULL,
      'maincontentright' => NULL,
      'complementary' => NULL,
      'subcontentleft' => NULL,
      'subcontentright' => NULL,
      'contentbottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass;
    $pane->pid = 'new-1';
    $pane->panel = 'complementary';
    $pane->type = 'flowmix';
    $pane->subtype = 'flowmix_drupal_org';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'flow_obj' => (object) array(
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
        'flowmix_type' => 'local',
        'reusable' => 1,
        'admin_title' => 'Flowmix Drupal.org',
        'name' => 'flowmix_drupal_org',
        'category' => 'Flowmixes',
        'admin_description' => '',
        'index' => 1,
      ),
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $display->content['new-1'] = $pane;
    $display->panels['complementary'][0] = 'new-1';
    $pane = new stdClass;
    $pane->pid = 'new-2';
    $pane->panel = 'maincontentleft';
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
    $display->content['new-2'] = $pane;
    $display->panels['maincontentleft'][0] = 'new-2';
    $pane = new stdClass;
    $pane->pid = 'new-3';
    $pane->panel = 'maincontentright';
    $pane->type = 'views_panes';
    $pane->subtype = 'most_view-panel_pane_1';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'items_per_page' => '5',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $display->content['new-3'] = $pane;
    $display->panels['maincontentright'][0] = 'new-3';
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
  $handler->weight = -29;
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
  $display->layout = 'base-layout-brick';
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
    $pane->panel = 'maincontentleft';
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
    $display->panels['maincontentleft'][0] = 'new-1';
    $pane = new stdClass;
    $pane->pid = 'new-2';
    $pane->panel = 'maincontentleft';
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
    $display->panels['maincontentleft'][1] = 'new-2';
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
  $handler->weight = -26;
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
  $display->layout = 'base-layout-brick';
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
    $pane->panel = 'maincontentleft';
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
    $display->panels['maincontentleft'][0] = 'new-1';
    $pane = new stdClass;
    $pane->pid = 'new-2';
    $pane->panel = 'maincontentright';
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
    $display->panels['maincontentright'][0] = 'new-2';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-1';
  $handler->conf['display'] = $display;
  $export['term_view_panel_context_3'] = $handler;

  $handler = new stdClass;
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'term_view_panel_context_4';
  $handler->task = 'term_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = -28;
  $handler->conf = array(
    'title' => 'News',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'term',
          'settings' => array(
            'vid' => '2',
            1 => '',
            11 => array(),
            2 => array(
              1 => '1',
            ),
            3 => array(),
            4 => array(),
            5 => '',
            7 => '',
            6 => array(),
            8 => '',
            9 => '',
            10 => array(),
          ),
          'context' => 'argument_terms_1',
          'not' => FALSE,
        ),
      ),
      'logic' => 'and',
    ),
  );
  $display = new panels_display;
  $display->layout = 'base-layout-brick';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'contenttop' => NULL,
      'maincontentleft' => NULL,
      'maincontentright' => NULL,
      'complementary' => NULL,
      'subcontentleft' => NULL,
      'subcontentright' => NULL,
      'contentbottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass;
    $pane->pid = 'new-1';
    $pane->panel = 'complementary';
    $pane->type = 'redacblock';
    $pane->subtype = 'destination_redacblock_sidebar';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'admin_title' => NULL,
      'title' => NULL,
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
      'substitute' => NULL,
      'link_to_view' => 0,
      'panel_args' => 0,
      'args' => '',
      'url' => '',
      'required context' => NULL,
      'context' => array(
        0 => 'argument_terms_1.tids',
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
    $display->panels['complementary'][0] = 'new-1';
    $pane = new stdClass;
    $pane->pid = 'new-2';
    $pane->panel = 'maincontentleft';
    $pane->type = 'redacblock';
    $pane->subtype = 'default_destination_block';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'admin_title' => NULL,
      'title' => NULL,
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
      'substitute' => NULL,
      'link_to_view' => 0,
      'panel_args' => 0,
      'args' => '',
      'url' => '',
      'required context' => NULL,
      'context' => array(
        0 => 'argument_terms_1.tids',
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
    $display->content['new-2'] = $pane;
    $display->panels['maincontentleft'][0] = 'new-2';
    $pane = new stdClass;
    $pane->pid = 'new-3';
    $pane->panel = 'maincontentright';
    $pane->type = 'views_panes';
    $pane->subtype = 'most_view-panel_pane_1';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'items_per_page' => '10',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $display->content['new-3'] = $pane;
    $display->panels['maincontentright'][0] = 'new-3';
    $pane = new stdClass;
    $pane->pid = 'new-4';
    $pane->panel = 'maincontentright';
    $pane->type = 'flowmix';
    $pane->subtype = 'flowmix_drupal_org';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'flow_obj' => (object) array(
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
        'flowmix_type' => 'fixed',
      ),
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $display->content['new-4'] = $pane;
    $display->panels['maincontentright'][1] = 'new-4';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-2';
  $handler->conf['display'] = $display;
  $export['term_view_panel_context_4'] = $handler;

  return $export;
}
