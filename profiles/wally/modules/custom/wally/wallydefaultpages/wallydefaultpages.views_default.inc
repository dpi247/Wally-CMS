<?php

/**
 * Implementation of hook_views_default_views().
 */
function wallydefaultpages_views_default_views() {
  $views = array();

  // Exported view: nodes_from_term
  $view = new view;
  $view->name = 'nodes_from_term';
  $view->description = '';
  $view->tag = '';
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
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  /* Argument: Destinations: Destinations tid: Destinations (field_destinations) */
  $handler->display->display_options['arguments']['field_destinations_tid']['id'] = 'field_destinations_tid';
  $handler->display->display_options['arguments']['field_destinations_tid']['table'] = 'node_data_field_destinations';
  $handler->display->display_options['arguments']['field_destinations_tid']['field'] = 'field_destinations_tid';
  $handler->display->display_options['arguments']['field_destinations_tid']['style_plugin'] = 'default_summary';
  $handler->display->display_options['arguments']['field_destinations_tid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_destinations_tid']['add_table'] = 0;
  $handler->display->display_options['arguments']['field_destinations_tid']['require_value'] = 0;
  $handler->display->display_options['arguments']['field_destinations_tid']['reduce_duplicates'] = 0;
  $handler->display->display_options['arguments']['field_destinations_tid']['set_breadcrumb'] = 0;
  /* Argument: Taxonomy: Term ID */
  $handler->display->display_options['arguments']['tid']['id'] = 'tid';
  $handler->display->display_options['arguments']['tid']['table'] = 'term_node';
  $handler->display->display_options['arguments']['tid']['field'] = 'tid';
  $handler->display->display_options['arguments']['tid']['style_plugin'] = 'default_summary';
  $handler->display->display_options['arguments']['tid']['default_argument_type'] = 'fixed';
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

  /* Display: Nodes from term */
  $handler = $view->new_display('panel_pane', 'Nodes from term', 'panel_pane_1');
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Argument: Taxonomy: Term ID */
  $handler->display->display_options['arguments']['tid']['id'] = 'tid';
  $handler->display->display_options['arguments']['tid']['table'] = 'term_node';
  $handler->display->display_options['arguments']['tid']['field'] = 'tid';
  $handler->display->display_options['arguments']['tid']['style_plugin'] = 'redacblock_style';
  $handler->display->display_options['arguments']['tid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['tid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['tid']['add_table'] = 0;
  $handler->display->display_options['arguments']['tid']['require_value'] = 0;
  $handler->display->display_options['arguments']['tid']['reduce_duplicates'] = 0;
  $handler->display->display_options['arguments']['tid']['set_breadcrumb'] = 0;
  $handler->display->display_options['argument_input'] = array(
    'field_destinations_tid' => array(
      'type' => 'context',
      'context' => 'term.tid',
      'context_optional' => 0,
      'panel' => '0',
      'fixed' => '',
      'label' => 'Destinations: Destinations tid: Destinations (field_destinations)',
    ),
  );
  $translatables['nodes_from_term'] = array(
    t('Defaults'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('All'),
    t('Nodes from term'),
    t('View panes'),
  );

  $views[$view->name] = $view;

  return $views;
}
