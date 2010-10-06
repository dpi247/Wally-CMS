<?php
/**
 * @file
 * Contains default views on behalf of the node module.
 */

/**
 * Implementation of hook_default_view_views().
 */

function pollfield_views_default_views() {
	
  $view = new view;
  $view->name = 'pollfield_votes';
  $view->description = 'A list of all content, by letter.';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = '0';
  $view->api_version = 2;
  $view->disabled = TRUE; // Edit this to true to make a default view disabled initially 
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'delta' => array(
      'label' => 'Choice',
      'exclude' => 0,
      'id' => 'delta',
      'table' => 'pollfield_votes',
      'field' => 'delta',
      'relationship' => 'none',
    ),
		'nid' => array(
      'label' => 'Node ID',
      'exclude' => 1,
      'id' => 'nid',
      'table' => 'pollfield_votes',
      'field' => 'nid',
      'relationship' => 'none',
    ),
		'field_name_delta' => array(
      'label' => 'Pollfield ID',
      'exclude' => 1,
      'id' => 'field_name_delta',
      'table' => 'pollfield_votes',
      'field' => 'field_name_delta',
      'relationship' => 'none',
    ),
    'question' => array(
      'label' => 'Question',
      'exclude' => 0,
      'id' => 'question',
      'table' => 'pollfield_votes',
      'field' => 'question',
      'relationship' => 'none',
    ),
		'field_table' => array(
      'label' => 'Table',
      'exclude' => 1,
      'id' => 'field_table',
      'table' => 'pollfield_votes',
      'field' => 'field_table',
      'relationship' => 'none',
    ),
		'field_name' => array(
      'label' => 'Field',
      'exclude' => 1,
      'id' => 'field_name',
      'table' => 'pollfield_votes',
      'field' => 'field_name',
      'relationship' => 'none',
    ),
		'name' => array(
      'label' => 'Name',
      'exclude' => 0,
      'id' => 'name',
      'table' => 'users',
      'field' => 'name',
      'relationship' => 'none',
    ),

  ));
  
  $handler->override_option('access', array(
    'type' => 'none',
  ));
	$handler = $view->new_display('page', 'Page', 'page');
  $handler->override_option('path', 'pollfield_votes');
  $handler->override_option('menu', array(
    'type' => 'none',
    'title' => '',
    'weight' => 0,
  ));
  $handler->override_option('use_ajax', '1');
  $handler->override_option('items_per_page', 36);
  $handler->override_option('use_pager', '1');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'delta' => 'delta',
      'nid' => 'nid',
      'field_name_delta' => 'field_name_delta',
			'question' => 'question',
			'field_table' => 'field_table',
			'name' => 'name',
    ),
    'info' => array(
      'delta' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'nid' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'field_name_delta' => array(
        'sortable' => 1,
        'separator' => '',
      ),
			'question' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'field_table' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'name' => array(
        'sortable' => 1,
        'separator' => '',
      ),
    ),
    'default' => 'name',
  ));
  
  $views[$view->name] = $view;
	return $views;
}
