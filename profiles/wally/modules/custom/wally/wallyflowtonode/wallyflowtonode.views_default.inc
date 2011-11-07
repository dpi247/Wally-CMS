<?php

/**
 * Implementation of hook_views_default_views().
 */
function wallyflowtonode_views_default_views() {
  $views = array();

  // Exported view: flow_to_node_queue
  $view = new view;
  $view->name = 'flow_to_node_queue';
  $view->description = 'Get latest unpublished nodes for a flow to node feed';
  $view->tag = 'Node queue for a flow to node feed';
  $view->base_table = 'node';
  $view->core = 0;
  $view->api_version = '2';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'title' => array(
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
    ),
    'field_publicationdate_value' => array(
      'id' => 'field_publicationdate_value',
      'table' => 'node_data_field_publicationdate',
      'field' => 'field_publicationdate_value',
    ),
  ));
  $handler->override_option('sorts', array(
    'field_publicationdate_value' => array(
      'order' => 'DESC',
      'delta' => -1,
      'id' => 'field_publicationdate_value',
      'table' => 'node_data_field_publicationdate',
      'field' => 'field_publicationdate_value',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'field_externalreference_value' => array(
      'default_action' => 'ignore',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => '',
      'breadcrumb' => '',
      'default_argument_type' => 'fixed',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'glossary' => 1,
      'limit' => '5',
      'case' => 'none',
      'path_case' => 'none',
      'transform_dash' => 0,
      'id' => 'field_externalreference_value',
      'table' => 'node_data_field_externalreference',
      'field' => 'field_externalreference_value',
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
        1 => 0,
        2 => 0,
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
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => '0',
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
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('items_per_page', 20);
  $handler->override_option('use_pager', '1');
  $handler->override_option('style_plugin', 'bulk');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'title' => 'title',
      'field_publicationdate_value' => 'field_publicationdate_value',
    ),
    'info' => array(
      'title' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'field_publicationdate_value' => array(
        'sortable' => 0,
        'separator' => '',
      ),
    ),
    'default' => '-1',
    'execution_type' => '1',
    'display_type' => '1',
    'hide_select_all' => 0,
    'display_result' => 1,
    'merge_single_action' => 1,
    'views_bulk_operations_fields_action' => array(
      'php_code' => 0,
      'display_fields' => array(
        '_all_' => '_all_',
      ),
    ),
    'operations' => array(
      'node_mass_update-9c585624b9b3af0b4687d5f97f35e047' => array(
        'selected' => TRUE,
        'skip_confirmation' => 1,
      ),
      'wallyflowtonode_reject_nodes' => array(
        'selected' => TRUE,
        'skip_confirmation' => 1,
      ),
    ),
  ));

  $views[$view->name] = $view;

  return $views;
}
