<?php

/**
 * Implementation of hook_default_page_manager_pages().
 */
function wallydashboard_default_page_manager_pages() {

  $pages = array();
  //Main Dashboard 
  $page = new stdClass;
$page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
$page->api_version = 1;
$page->name = 'dashboard';
$page->task = 'page';
$page->admin_title = 'Dashboard';
$page->admin_description = '';
$page->path = 'admin/wally/wallydashboard/content';
$page->access = array(
  'plugins' => array(
    0 => array(
      'name' => 'perm',
      'settings' => array(
        'perm' => 'administer dashboard',
      ),
      'context' => 'logged-in-user',
      'not' => FALSE,
    ),
  ),
);
$page->menu = array(
  'type' => 'default tab',
  'title' => 'Dashboard',
  'name' => 'admin',
  'weight' => '0',
  'parent' => array(
    'type' => 'tab',
    'title' => 'Dashboard',
    'name' => 'admin',
    'weight' => '0',
  ),
);
$page->arguments = array();
$page->conf = array();
$page->default_handlers = array();
$handler = new stdClass;
$handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
$handler->api_version = 1;
$handler->name = 'page_dashboard_panel_context';
$handler->task = 'page';
$handler->subtask = 'dashboard';
$handler->handler = 'panel_context';
$handler->weight = 0;
$handler->conf = array(
  'title' => 'Dashboard',
  'no_blocks' => 0,
  'pipeline' => 'standard',
  'css_id' => '',
  'css' => '',
  'contexts' => array(
    0 => array(
      'name' => 'user',
      'id' => 1,
      'identifier' => 'User',
      'keyword' => 'user',
      'context_settings' => array(
        'type' => 'current',
        'user' => '',
        'uid' => '',
      ),
    ),
  ),
  'relationships' => array(),
);
$display = new panels_display;
$display->layout = 'twocol_bricks';
$display->layout_settings = array();
$display->panel_settings = array(
  'style_settings' => array(
    'default' => NULL,
    'top' => NULL,
    'left_above' => NULL,
    'right_above' => NULL,
    'middle' => NULL,
    'left_below' => NULL,
    'right_below' => NULL,
    'bottom' => NULL,
  ),
);
$display->cache = array();
$display->title = '';
$display->content = array();
$display->panels = array();
  $pane = new stdClass;
  $pane->pid = 'new-1';
  $pane->panel = 'left_above';
  $pane->type = 'views_panes';
  $pane->subtype = 'dashboard_last_created_article-panel_pane_1';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'items_per_page' => '5',
    'context' => array(
      0 => 'empty',
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
  $display->panels['left_above'][0] = 'new-1';
  $pane = new stdClass;
  $pane->pid = 'new-2';
  $pane->panel = 'left_above';
  $pane->type = 'views_panes';
  $pane->subtype = 'dashboard_last_edited_article-panel_pane_1';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'items_per_page' => '5',
    'context' => array(
      0 => 'empty',
    ),
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $display->content['new-2'] = $pane;
  $display->panels['left_above'][1] = 'new-2';
  $pane = new stdClass;
  $pane->pid = 'new-3';
  $pane->panel = 'left_below';
  $pane->type = 'lastprenode';
  $pane->subtype = 'lastprenode';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'override_title' => 0,
    'override_title_text' => '',
    'item_number' => '3',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $display->content['new-3'] = $pane;
  $display->panels['left_below'][0] = 'new-3';
  $pane = new stdClass;
  $pane->pid = 'new-4';
  $pane->panel = 'middle';
  $pane->type = 'views_panes';
  $pane->subtype = 'most_view-panel_pane_2';
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
  $display->content['new-4'] = $pane;
  $display->panels['middle'][0] = 'new-4';
  $pane = new stdClass;
  $pane->pid = 'new-5';
  $pane->panel = 'right_above';
  $pane->type = 'views_panes';
  $pane->subtype = 'dashboard_mylast_created_article-panel_pane_1';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'items_per_page' => '5',
    'context' => array(
      0 => 'context_user_1',
      1 => 'empty',
    ),
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $display->content['new-5'] = $pane;
  $display->panels['right_above'][0] = 'new-5';
  $pane = new stdClass;
  $pane->pid = 'new-6';
  $pane->panel = 'right_above';
  $pane->type = 'views_panes';
  $pane->subtype = 'dashboard_mylast_edited_article-panel_pane_1';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'items_per_page' => '5',
    'context' => array(
      0 => 'context_user_1',
      1 => 'empty',
    ),
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $display->content['new-6'] = $pane;
  $display->panels['right_above'][1] = 'new-6';
  $pane = new stdClass;
  $pane->pid = 'new-7';
  $pane->panel = 'right_below';
  $pane->type = 'views_panes';
  $pane->subtype = 'dashbord_last_import_package-panel_pane_1';
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
  $display->content['new-7'] = $pane;
  $display->panels['right_below'][0] = 'new-7';
  $pane = new stdClass;
  $pane->pid = 'new-8';
  $pane->panel = 'right_below';
  $pane->type = 'views_panes';
  $pane->subtype = 'dashbord_last_import_package-panel_pane_2';
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
  $pane->position = 1;
  $display->content['new-8'] = $pane;
  $display->panels['right_below'][1] = 'new-8';
  $pane = new stdClass;
  $pane->pid = 'new-9';
  $pane->panel = 'top';
  $pane->type = 'shortcut';
  $pane->subtype = 'shortcut';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'shortcuts' => array(
      'wallycontenttypes_create_article' => 'wallycontenttypes_create_article',
      'wallycontenttypes_create_gallery' => 'wallycontenttypes_create_gallery',
      'wallycontenttypes_create_poll' => 'wallycontenttypes_create_poll',
      'wallydashboard_manage_package' => 'wallydashboard_manage_package',
      'cckdestinations_reorder' => 'cckdestinations_reorder',
      'wallydashboard_manage_pages' => 'wallydashboard_manage_pages',
    ),
    'override_title' => 1,
    'override_title_text' => '',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $display->content['new-9'] = $pane;
  $display->panels['top'][0] = 'new-9';
$display->hide_title = PANELS_TITLE_FIXED;
$display->title_pane = '0';
$handler->conf['display'] = $display;
$page->default_handlers[$handler->name] = $handler;
  
  
  $pages[$page->name] = $page;
  
  
  //System Dashboard
  $page = new stdClass;
$page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
$page->api_version = 1;
$page->name = 'dashboard_system';
$page->task = 'page';
$page->admin_title = 'Dashboard : System';
$page->admin_description = '';
$page->path = 'admin/wally/wallydashboard/system';
$page->access = array(
  'plugins' => array(
    0 => array(
      'name' => 'perm',
      'settings' => array(
        'perm' => 'administer dashboard',
      ),
      'context' => 'logged-in-user',
      'not' => FALSE,
    ),
  ),
  'logic' => 'and',
);
$page->menu = array(
  'type' => 'tab',
  'title' => 'System',
  'name' => 'navigation',
  'weight' => '10',
  'parent' => array(
    'type' => 'none',
    'title' => '',
    'name' => 'admin',
    'weight' => '0',
  ),
);
$page->arguments = array();
$page->conf = array();
$page->default_handlers = array();
$handler = new stdClass;
$handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
$handler->api_version = 1;
$handler->name = 'page_dashboard_system_panel_context';
$handler->task = 'page';
$handler->subtask = 'dashboard_system';
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
$display->layout = 'twocol_bricks';
$display->layout_settings = array();
$display->panel_settings = array(
  'style_settings' => array(
    'default' => NULL,
    'top' => NULL,
    'left_above' => NULL,
    'right_above' => NULL,
    'middle' => NULL,
    'left_below' => NULL,
    'right_below' => NULL,
    'bottom' => NULL,
  ),
);
$display->cache = array();
$display->title = '';
$display->content = array();
$display->panels = array();
  $pane = new stdClass;
  $pane->pid = 'new-1';
  $pane->panel = 'left_above';
  $pane->type = 'top403';
  $pane->subtype = 'top403';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'top403_nbrLine' => '5',
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
  $display->panels['left_above'][0] = 'new-1';
  $pane = new stdClass;
  $pane->pid = 'new-2';
  $pane->panel = 'left_above';
  $pane->type = 'top404';
  $pane->subtype = 'top404';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'top404_nbrLine' => '5',
    'override_title' => 0,
    'override_title_text' => '',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $display->content['new-2'] = $pane;
  $display->panels['left_above'][1] = 'new-2';
  $pane = new stdClass;
  $pane->pid = 'new-3';
  $pane->panel = 'middle';
  $pane->type = 'dblog';
  $pane->subtype = 'dblog';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'dblog_nbrLine' => '5',
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
  $display->content['new-3'] = $pane;
  $display->panels['middle'][0] = 'new-3';
  $pane = new stdClass;
  $pane->pid = 'new-4';
  $pane->panel = 'right_above';
  $pane->type = 'status';
  $pane->subtype = 'status';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'override_title' => 0,
    'override_title_text' => '',
    'requirements' => array(
      'Access to update.php' => 'Access to update.php',
      'CTools CSS Cache' => 'CTools CSS Cache',
      'Configuration file' => 'Configuration file',
      'Cron maintenance tasks' => 'Cron maintenance tasks',
      'Database updates' => 'Database updates',
      'Apache Solr' => 'Apache Solr',
    ),
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $display->content['new-4'] = $pane;
  $display->panels['right_above'][0] = 'new-4';
$display->hide_title = PANELS_TITLE_FIXED;
$display->title_pane = '0';
$handler->conf['display'] = $display;
$page->default_handlers[$handler->name] = $handler;
  
  $pages[$page->name] = $page;
  
  
  //Node Dashboard
  $page = new stdClass;
$page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
$page->api_version = 1;
$page->name = 'wallydashboard_node';
$page->task = 'page';
$page->admin_title = 'Dashboard : Node';
$page->admin_description = '';
$page->path = 'node/%node/wallydashboard';
$page->access = array(
  'plugins' => array(
    0 => array(
      'name' => 'perm',
      'settings' => array(
        'perm' => 'administer dashboard',
      ),
      'context' => 'logged-in-user',
      'not' => FALSE,
    ),
  ),
  'logic' => 'and',
);
$page->menu = array(
  'type' => 'tab',
  'title' => 'Dashboard',
  'name' => 'navigation',
  'weight' => '10',
  'parent' => array(
    'type' => 'none',
    'title' => '',
    'name' => 'navigation',
    'weight' => '0',
  ),
);
$page->arguments = array(
  'node' => array(
    'id' => 1,
    'identifier' => 'Node: ID',
    'name' => 'nid',
    'settings' => array(),
  ),
);
$page->conf = array();
$page->default_handlers = array();
$handler = new stdClass;
$handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
$handler->api_version = 1;
$handler->name = 'page_wallydashboard_node_panel_context';
$handler->task = 'page';
$handler->subtask = 'wallydashboard_node';
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
$display->layout = 'twocol_bricks';
$display->layout_settings = array();
$display->panel_settings = array(
  'style_settings' => array(
    'default' => NULL,
    'top' => NULL,
    'left_above' => NULL,
    'right_above' => NULL,
    'middle' => NULL,
    'left_below' => NULL,
    'right_below' => NULL,
    'bottom' => NULL,
  ),
);
$display->cache = array();
$display->title = '';
$display->content = array();
$display->panels = array();
  $pane = new stdClass;
  $pane->pid = 'new-1';
  $pane->panel = 'top';
  $pane->type = 'showstats48h';
  $pane->subtype = 'showstats48h';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'context' => 'argument_nid_1',
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
  $display->panels['top'][0] = 'new-1';
$display->hide_title = PANELS_TITLE_FIXED;
$display->title_pane = '0';
$handler->conf['display'] = $display;
$page->default_handlers[$handler->name] = $handler;
  
  $pages[$page->name] = $page;
  //Term Dashboard
$page = new stdClass;
$page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
$page->api_version = 1;
$page->name = 'wallydashboard_term';
$page->task = 'page';
$page->admin_title = 'Dashboard : Term';
$page->admin_description = '';
$page->path = 'taxonomy/term/%term/wallydashboard';
$page->access = array(
  'plugins' => array(
    0 => array(
      'name' => 'perm',
      'settings' => array(
        'perm' => 'administer dashboard',
      ),
      'context' => 'logged-in-user',
      'not' => FALSE,
    ),
  ),
  'logic' => 'and',
);
$page->menu = array(
  'type' => 'tab',
  'title' => 'Dashboard',
  'name' => 'navigation',
  'weight' => '10',
  'parent' => array(
    'type' => 'none',
    'title' => '',
    'name' => 'navigation',
    'weight' => '0',
  ),
);
$page->arguments = array(
  'term' => array(
    'id' => 1,
    'identifier' => 'Taxonomy term: ID',
    'name' => 'term',
    'settings' => array(
      'input_form' => 'tid',
      'vids' => array(
        2 => 2,
        1 => 0,
        12 => 0,
        3 => 0,
        4 => 0,
        5 => 0,
        7 => 0,
        6 => 0,
        11 => 0,
        8 => 0,
        9 => 0,
        10 => 0,
      ),
      'breadcrumb' => 1,
    ),
  ),
);
$page->conf = array();
$page->default_handlers = array();
$handler = new stdClass;
$handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
$handler->api_version = 1;
$handler->name = 'page_wallydashboard_term_panel_context';
$handler->task = 'page';
$handler->subtask = 'wallydashboard_term';
$handler->handler = 'panel_context';
$handler->weight = 0;
$handler->conf = array(
  'title' => 'Panel',
  'no_blocks' => 0,
  'pipeline' => 'standard',
  'css_id' => '',
  'css' => '',
  'contexts' => array(
    0 => array(
      'name' => 'user',
      'id' => 1,
      'identifier' => 'User',
      'keyword' => 'user',
      'context_settings' => array(
        'type' => 'current',
        'user' => '',
        'uid' => '',
      ),
    ),
  ),
  'relationships' => array(),
);
$display = new panels_display;
$display->layout = 'twocol_bricks';
$display->layout_settings = array();
$display->panel_settings = array(
  'style_settings' => array(
    'default' => NULL,
    'top' => NULL,
    'left_above' => NULL,
    'right_above' => NULL,
    'middle' => NULL,
    'left_below' => NULL,
    'right_below' => NULL,
    'bottom' => NULL,
  ),
);
$display->cache = array();
$display->title = '';
$display->content = array();
$display->panels = array();
  $pane = new stdClass;
  $pane->pid = 'new-1';
  $pane->panel = 'left_above';
  $pane->type = 'views_panes';
  $pane->subtype = 'dashboard_mylast_created_article-panel_pane_1';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'items_per_page' => '20',
    'context' => array(
      0 => 'context_user_1',
      1 => 'argument_term_1',
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
  $display->panels['left_above'][0] = 'new-1';
  $pane = new stdClass;
  $pane->pid = 'new-2';
  $pane->panel = 'left_above';
  $pane->type = 'views_panes';
  $pane->subtype = 'dashboard_mylast_edited_article-panel_pane_1';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'items_per_page' => '5',
    'context' => array(
      0 => 'context_user_1',
      1 => 'argument_term_1',
    ),
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $display->content['new-2'] = $pane;
  $display->panels['left_above'][1] = 'new-2';
  $pane = new stdClass;
  $pane->pid = 'new-3';
  $pane->panel = 'right_above';
  $pane->type = 'views_panes';
  $pane->subtype = 'dashboard_last_created_article-panel_pane_1';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'items_per_page' => '5',
    'context' => array(
      0 => 'argument_term_1',
    ),
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $display->content['new-3'] = $pane;
  $display->panels['right_above'][0] = 'new-3';
  $pane = new stdClass;
  $pane->pid = 'new-4';
  $pane->panel = 'right_above';
  $pane->type = 'views_panes';
  $pane->subtype = 'dashboard_last_edited_article-panel_pane_1';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'items_per_page' => '5',
    'context' => array(
      0 => 'argument_term_1',
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
  $display->panels['right_above'][1] = 'new-4';
  $pane = new stdClass;
  $pane->pid = 'new-5';
  $pane->panel = 'top';
  $pane->type = 'views_panes';
  $pane->subtype = 'most_view-panel_pane_2';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'items_per_page' => '5',
    'context' => array(
      0 => 'argument_term_1',
    ),
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $display->content['new-5'] = $pane;
  $display->panels['top'][0] = 'new-5';
$display->hide_title = PANELS_TITLE_FIXED;
$display->title_pane = '0';
$handler->conf['display'] = $display;
$page->default_handlers[$handler->name] = $handler;
  ;
  
  $pages[$page->name] = $page;
  return $pages;
  
}
