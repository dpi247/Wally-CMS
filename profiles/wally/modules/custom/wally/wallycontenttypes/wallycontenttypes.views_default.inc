<?php

/**
 * Implementation of hook_views_default_views().
 */
function wallycontenttypes_views_default_views() {
  $views = array();

  // Exported view: default_destination_view_feed
  $view = new view;
  $view->name = 'default_destination_view_feed';
  $view->description = 'Wally default pre-packaged destination view. Clone this view to build your own or build new destination display from this one.';
  $view->tag = 'wally default';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('sorts', array(
    'field_destinations_rank' => array(
      'order' => 'DESC',
      'delta' => '-1',
      'id' => 'field_destinations_rank',
      'table' => 'node_data_field_destinations',
      'field' => 'field_destinations_rank',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'field_destinations_tid' => array(
      'default_action' => 'ignore',
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
      'add_table' => 1,
      'require_value' => 0,
      'reduce_duplicates' => 1,
      'set_breadcrumb' => 1,
      'id' => 'field_destinations_tid',
      'table' => 'node_data_field_destinations',
      'field' => 'field_destinations_tid',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        '2' => 0,
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
        '2' => 2,
        '1' => 0,
        '3' => 0,
        '4' => 0,
        '5' => 0,
        '6' => 0,
        '10' => 0,
        '7' => 0,
        '8' => 0,
        '9' => 0,
      ),
      'validate_argument_type' => 'convert',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_php' => '',
      'override' => array(
        'button' => 'Override',
      ),
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
  $handler->override_option('row_plugin', 'node');
  $handler = $view->new_display('redacblock', 'Default Destination block', 'redacblock_1');
  $handler->override_option('row_options', NULL);
  $handler->override_option('redacblock_layout', array(
    'small' => 'small',
    'medium' => 'medium',
    'large' => 'large',
  ));
  $handler->override_option('pane_title', '');
  $handler->override_option('pane_description', '');
  $handler->override_option('pane_category', array(
    'name' => 'View panes',
    'weight' => 0,
  ));
  $handler->override_option('allow', array(
    'use_pager' => 'use_pager',
    'items_per_page' => 'items_per_page',
    'offset' => 'offset',
    'link_to_view' => 0,
    'more_link' => 0,
    'path_override' => 0,
    'title_override' => 'title_override',
    'exposed_form' => 0,
    'fields_override' => 0,
  ));
  $handler->override_option('argument_input', array(
    'field_destinations_tid' => array(
      'type' => 'context',
      'context' => 'terms.tids',
      'context_optional' => 0,
      'panel' => '0',
      'fixed' => '',
      'label' => 'Destinations: Destinations tid: Destinations (field_destinations)',
    ),
  ));
  $handler->override_option('link_to_view', 0);
  $handler->override_option('inherit_panels_path', 0);
  $handler = $view->new_display('feed', 'Feed', 'feed_1');
  $handler->override_option('style_plugin', 'rss');
  $handler->override_option('style_options', array(
    'mission_description' => FALSE,
    'description' => '',
  ));
  $handler->override_option('row_plugin', 'node_rss');
  $handler->override_option('path', 'rss/%/rss.xml');
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

  $views[$view->name] = $view;

  return $views;
}
