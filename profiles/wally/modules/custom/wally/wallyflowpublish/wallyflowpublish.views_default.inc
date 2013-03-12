<?php

/**
 * Implementation of hook_views_default_views().
 */
function wallyflowpublish_views_default_views() {
  $views = array();
/* Before Views 3
  // Exported view: default_destination_view_feed
  $view = new view;
  $view->name = 'default_destination_view_feed';
  $view->description = 'Wally default flow destination view.';
  $view->tag = 'wally default';
  $view->base_table = 'node';
  $view->core = 0;
  $view->api_version = '2';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially 
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('sorts', array(
    'field_destinations_time_rank' => array(
      'order' => 'DESC',
      'delta' => '-1',
      'id' => 'field_destinations_time_rank',
      'table' => 'node_data_field_destinations',
      'field' => 'field_destinations_time_rank',
      'override' => array(
        'button' => 'Remplacer',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'field_destinations_tid' => array(
      'default_action' => 'not found',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => '',
      'breadcrumb' => '',
      'default_argument_type' => 'fixed',
      'default_argument' => '',
      'validate_type' => 'taxonomy_term',
      'validate_fail' => 'not found',
      'add_table' => 0,
      'require_value' => 0,
      'reduce_duplicates' => 1,
      'set_breadcrumb' => 1,
      'id' => 'field_destinations_tid',
      'table' => 'node_data_field_destinations',
      'field' => 'field_destinations_tid',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        2 => 0,
      ),
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_argument_fixed' => '',
      'default_argument_user' => 0,
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'poll' => 0,
        'wally_articlepackage' => 0,
        'wally_audioobject' => 0,
        'wally_digitalobject' => 0,
        'wally_entitytype' => 0,
        'wally_gallerypackage' => 0,
        'wally_linkslistobject' => 0,
        'wally_linktype' => 0,
        'wally_locationtype' => 0,
        'wally_persontype' => 0,
        'wally_photoobject' => 0,
        'wally_pollobject' => 0,
        'wally_pollpackage' => 0,
        'wally_textobject' => 0,
        'wally_videoobject' => 0,
        'page' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        2 => 2,
        1 => 0,
        3 => 0,
        4 => 0,
        5 => 0,
        6 => 0,
        10 => 0,
        7 => 0,
        8 => 0,
        9 => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_php' => '',
      'override' => array(
        'button' => 'Override',
      ),
    ),
    'field_destinations_target' => array(
      'id' => 'field_destinations_target',
      'table' => 'node_data_field_destinations',
      'field' => 'field_destinations_target',
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'relationship' => 'none',
    ),
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'wally_articlepackage' => 'wally_articlepackage',
        'wally_gallerypackage' => 'wally_gallerypackage',
        'wally_pollpackage' => 'wally_pollpackage',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('use_pager', 'mini');
  $handler->override_option('distinct', 1);
  $handler->override_option('row_plugin', 'node');
  $handler = $view->new_display('feed', 'Feed', 'feed_1');
  $handler->override_option('style_plugin', 'rss');
  $handler->override_option('style_options', array(
    'mission_description' => FALSE,
    'description' => '',
  ));
  $handler->override_option('row_plugin', 'node_rss');
  $handler->override_option('path', 'feed');
  $handler->override_option('menu', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler->override_option('displays', array());
  $handler->override_option('sitename_title', FALSE);
*/
  
  $view = new view;
  $view->name = 'default_destination_view_feed';
  $view->description = 'Wally default flow destination view.';
  $view->tag = 'wally default';
  $view->base_table = 'node';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  
  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  /* Sort criterion: Destinations: Destinations time_rank: Destinations (field_destinations) */
  $handler->display->display_options['sorts']['field_destinations_time_rank']['id'] = 'field_destinations_time_rank';
  $handler->display->display_options['sorts']['field_destinations_time_rank']['table'] = 'node_data_field_destinations';
  $handler->display->display_options['sorts']['field_destinations_time_rank']['field'] = 'field_destinations_time_rank';
  $handler->display->display_options['sorts']['field_destinations_time_rank']['order'] = 'DESC';
  $handler->display->display_options['sorts']['field_destinations_time_rank']['delta'] = '-1';
  /* Argument: Destinations: Destinations tid: Destinations (field_destinations) */
  $handler->display->display_options['arguments']['field_destinations_tid']['id'] = 'field_destinations_tid';
  $handler->display->display_options['arguments']['field_destinations_tid']['table'] = 'node_data_field_destinations';
  $handler->display->display_options['arguments']['field_destinations_tid']['field'] = 'field_destinations_tid';
  $handler->display->display_options['arguments']['field_destinations_tid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['field_destinations_tid']['style_plugin'] = 'default_summary';
  $handler->display->display_options['arguments']['field_destinations_tid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_destinations_tid']['validate_type'] = 'taxonomy_term';
  $handler->display->display_options['arguments']['field_destinations_tid']['validate_options']['vids'] = array(
  2 => 2,
  1 => 0,
  3 => 0,
  4 => 0,
  5 => 0,
  6 => 0,
  10 => 0,
  7 => 0,
  8 => 0,
  9 => 0,
  );
  $handler->display->display_options['arguments']['field_destinations_tid']['validate_options']['transform'] = 0;
  $handler->display->display_options['arguments']['field_destinations_tid']['add_table'] = 0;
  $handler->display->display_options['arguments']['field_destinations_tid']['require_value'] = 0;
  $handler->display->display_options['arguments']['field_destinations_tid']['reduce_duplicates'] = 1;
  $handler->display->display_options['arguments']['field_destinations_tid']['set_breadcrumb'] = 1;
  /* Argument: Destinations: Destinations target: Destinations (field_destinations) */
  $handler->display->display_options['arguments']['field_destinations_target']['id'] = 'field_destinations_target';
  $handler->display->display_options['arguments']['field_destinations_target']['table'] = 'node_data_field_destinations';
  $handler->display->display_options['arguments']['field_destinations_target']['field'] = 'field_destinations_target';
  $handler->display->display_options['arguments']['field_destinations_target']['style_plugin'] = 'default_summary';
  $handler->display->display_options['arguments']['field_destinations_target']['default_argument_type'] = 'fixed';
  /* Filter: Node: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter: Node: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'wally_articlepackage' => 'wally_articlepackage',
    'wally_gallerypackage' => 'wally_gallerypackage',
    'wally_pollpackage' => 'wally_pollpackage',
  );
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  
  /* Display: Feed */
  $handler = $view->new_display('feed', 'Feed', 'feed_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['style_plugin'] = 'rss';
  $handler->display->display_options['style_options']['mission_description'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'node_rss';
  $handler->display->display_options['path'] = 'feed';
  $translatables['default_destination_view_feed'] = array(
  t('Defaults'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('All'),
  t('Feed'),
  );
  
  $views[$view->name] = $view;

  return $views;
}
