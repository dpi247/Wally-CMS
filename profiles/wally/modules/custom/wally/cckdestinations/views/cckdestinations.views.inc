<?php

/**
 * @file
 * Contains functions handling views integration.
 */

/**
 * Implementation of hook_views_handlers().
 */
function cckdestinations_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'cckdestinations') .'/views',
    ),
    'handlers' => array(
      'cckdestinations_views_handler_argument_tid' => array(
        'parent' => 'views_handler_argument_term_node_tid',
      ),
      'cckdestinations_views_handler_argument_target' => array(
        'parent' => 'views_handler_argument',
      ),
      'cckdestinations_views_handler_argument_layout' => array(
        'parent' => 'views_handler_argument',
      ),
      'cckdestinations_views_handler_argument_rank' => array(
        'parent' => 'views_handler_argument',
      ),
      'cckdestinations_views_handler_argument_time_rank' => array(
        'parent' => 'views_handler_argument',
      ),
      'cckdestinations_views_handler_filter_is_empty' => array(
        'parent' => 'views_handler_filter_term_node_tid',
      ),      
      'cckdestinations_views_handler_filter_tid' => array(
        'parent' => 'views_handler_filter_term_node_tid',
      ),
      'cckdestinations_views_handler_filter_target' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'cckdestinations_views_handler_filter_layout' => array(
        'parent' => 'views_handler_filter_string',
      ),
      'cckdestinations_views_handler_filter_rank' => array(
        'parent' => 'views_handler_filter_string',
      ),
       'cckdestinations_views_handler_filter_time_rank' => array(
        'parent' => 'views_handler_filter_string',
      ),
      'cckdestinations_views_handler_field_tid' => array(
        'parent' => 'views_handler_field',
      ),
      'cckdestinations_views_handler_field_target' => array(
        'parent' => 'views_handler_field',
      ),
      'cckdestinations_views_handler_field_layout' => array(
        'parent' => 'views_handler_field',
      ),
      'cckdestinations_views_handler_field_rank' => array(
        'parent' => 'views_handler_field',
      ),
       'cckdestinations_views_handler_field_time_rank' => array(
        'parent' => 'views_handler_field',
      ),
      'cckdestinations_views_handler_field_aggregation' => array(
        'parent' => 'content_handler_field_multiple',
      ),
    ),
  );
}

/**
 * Return CCK Views data for the link_field_settings($op == 'views data').
 */
function cckdestinations_views_content_field_data($field) {
  // Build the automatic views data provided for us by CCK.
  // This creates all the information necessary for the "url" field.
  $data = content_views_field_views_data($field);

  $db_info = content_database_info($field);
  $table_alias = content_views_tablename($field);
  $field_types = _content_field_types();  
  
  // Build out additional views data for the cckdestinations "target" field.
  $data[$table_alias][$field['field_name'] .'_target'] = array(
    'group' => t('Destinations'),
    'title' => t('@label target', array('@label' => t($field_types[$field['type']]['label']))) .': '. t($field['widget']['label']) .' ('. $field['field_name'] .')',
    'help' =>  $data[$table_alias][$field['field_name'] .'_tid']['help'],
    'argument' => array(
      'field' => $db_info['columns']['target']['column'],
      'tablename' => $db_info['table'],
      'handler' => 'cckdestinations_views_handler_argument_target',
      'click sortable' => TRUE,
      'name field' => '', // TODO, mimic content.views.inc :)
      'content_field_name' => $field['field_name'],
      'allow_empty' => TRUE,
    ),
    'filter' => array(
      'field' => $db_info['columns']['target']['column'],
      'title' => t('@label target', array('@label' => t($field_types[$field['type']]['label']))) .': '. t($field['widget']['label']),
      'tablename' => $db_info['table'],
      'handler' => 'cckdestinations_views_handler_filter_target',
      'additional fields' => array(),
      'content_field_name' => $field['field_name'],
      'allow_empty' => TRUE,
    ), 
    'field' => array(
      'field' => $db_info['columns']['target']['column'],
      'title' => t('@label target', array('@label' => t($field_types[$field['type']]['label']))) .': '. t($field['widget']['label']),
      'tablename' => $db_info['table'],
      'handler' => 'cckdestinations_views_handler_field_target',
      'additional fields' => array(),
      'content_field_name' => $field['field_name'],
      'allow_empty' => TRUE,
    ),
    'sort' => array(
      'field' => $db_info['columns']['target']['column'],
      'tablename' => $db_info['table'],
      'handler' => 'content_handler_sort',
      'content_field_name' => $field['field_name'],
      'allow_empty' => TRUE,
    ),
  );
  
  
  
  
  // Build out additional views data for the cckdestinations "tid" field.
  $data[$table_alias][$field['field_name'] .'_tid'] = array(
    'group' => t('Destinations'),
    'title' => t('@label tid', array('@label' => t($field_types[$field['type']]['label']))) .': '. t($field['widget']['label']) .' ('. $field['field_name'] .')',
    'help' =>  $data[$table_alias][$field['field_name'] .'_tid']['help'],
    'argument' => array(
      'field' => $db_info['columns']['tid']['column'],
      'tablename' => $db_info['table'],
      'handler' => 'cckdestinations_views_handler_argument_tid',
      'click sortable' => TRUE,
      'name field' => '', // TODO, mimic content.views.inc :)
      'content_field_name' => $field['field_name'],
      'allow_empty' => TRUE,
    ),
    'filter' => array(
      'field' => $db_info['columns']['tid']['column'],
      'title' => t('@label tid', array('@label' => t($field_types[$field['type']]['label']))) .': '. t($field['widget']['label']),
      'tablename' => $db_info['table'],
      'handler' => 'cckdestinations_views_handler_filter_tid',
      'additional fields' => array(),
      'content_field_name' => $field['field_name'],
      'allow_empty' => TRUE,
    ),
    'field' => array(
      'field' => $db_info['columns']['tid']['column'],
      'title' => t('@label tid', array('@label' => t($field_types[$field['type']]['label']))) .': '. t($field['widget']['label']),
      'tablename' => $db_info['table'],
      'handler' => 'cckdestinations_views_handler_field_tid',
      'additional fields' => array(),
      'content_field_name' => $field['field_name'],
      'allow_empty' => TRUE,
    ),
    'relationship' => array(
      'field' => $db_info['columns']['tid']['column'],
      'tablename' => $db_info['table'],
      'handler' => 'views_handler_relationship',
      'base' => 'term_data',
      'base field' => 'tid',
      'content_field_name' => $field['field_name'],
      'allow_empty' => TRUE,
    ),
    'sort' => array(
      'field' => $db_info['columns']['tid']['column'],
      'tablename' => $db_info['table'],
      'handler' => 'content_handler_sort',
      'content_field_name' => $field['field_name'],
      'allow_empty' => TRUE,
    ),
  );
   // Build out additional views data for the cckdestinations "rank" field.
  $data[$table_alias][$field['field_name'] .'_rank'] = array(
    'group' => t('Destinations'),
    'title' => t('@label rank', array('@label' => t($field_types[$field['type']]['label']))) .': '. t($field['widget']['label']) .' ('. $field['field_name'] .')',
    'help' =>  $data[$table_alias][$field['field_name'] .'_tid']['help'],
    'argument' => array(
      'field' => $db_info['columns']['rank']['column'],
      'tablename' => $db_info['table'],
      'handler' => 'cckdestinations_views_handler_argument_rank',
      'click sortable' => TRUE,
      'name field' => '', // TODO, mimic content.views.inc :)
      'content_field_name' => $field['field_name'],
      'allow_empty' => TRUE,
    ),
    'filter' => array(
      'field' => $db_info['columns']['rank']['column'],
      'title' => t('@label rank', array('@label' => t($field_types[$field['type']]['label']))) .': '. t($field['widget']['label']),
      'tablename' => $db_info['table'],
      'handler' => 'cckdestinations_views_handler_filter_rank',
      'additional fields' => array(),
      'content_field_name' => $field['field_name'],
      'allow_empty' => TRUE,
    ),
    'field' => array(
      'field' => $db_info['columns']['rank']['column'],
      'title' => t('@label rank', array('@label' => t($field_types[$field['type']]['label']))) .': '. t($field['widget']['label']),
      'tablename' => $db_info['table'],
      'handler' => 'cckdestinations_views_handler_field_rank',
      'additional fields' => array(),
      'content_field_name' => $field['field_name'],
      'allow_empty' => TRUE,
    ),
    'sort' => array(
      'field' => $db_info['columns']['rank']['column'],
      'tablename' => $db_info['table'],
      'handler' => 'content_handler_sort',
      'content_field_name' => $field['field_name'],
      'allow_empty' => TRUE,
    ),
  );
     // Build out additional views data for the cckdestinations "rank" field.
  $data[$table_alias][$field['field_name'] .'_time_rank'] = array(
    'group' => t('Destinations'),
    'title' => t('@label time_rank', array('@label' => t($field_types[$field['type']]['label']))) .': '. t($field['widget']['label']) .' ('. $field['field_name'] .')',
    'help' =>  $data[$table_alias][$field['field_name'] .'_tid']['help'],
      'argument' => array(
      'field' => $db_info['columns']['time_rank']['column'],
      'tablename' => $db_info['table'],
      'handler' => 'cckdestinations_views_handler_argument_time_rank',
      'click sortable' => TRUE,
      'name field' => '', // TODO, mimic content.views.inc :)
      'content_field_name' => $field['field_name'],
      'allow_empty' => TRUE,
    ),
    'filter' => array(
      'field' => $db_info['columns']['time_rank']['column'],
      'title' => t('@label time_rank', array('@label' => t($field_types[$field['type']]['label']))) .': '. t($field['widget']['label']),
      'tablename' => $db_info['table'],
      'handler' => 'cckdestinations_views_handler_filter_time_rank',
      'additional fields' => array(),
      'content_field_name' => $field['field_name'],
      'allow_empty' => TRUE,
    ),
    'field' => array(
      'field' => $db_info['columns']['time_rank']['column'],
      'title' => t('@label time_rank', array('@label' => t($field_types[$field['type']]['label']))) .': '. t($field['widget']['label']),
      'tablename' => $db_info['table'],
      'handler' => 'cckdestinations_views_handler_field_time_rank',
      'additional fields' => array(),
      'content_field_name' => $field['field_name'],
      'allow_empty' => TRUE,
    ),
    'sort' => array(
      'field' => $db_info['columns']['time_rank']['column'],
      'tablename' => $db_info['table'],
      'handler' => 'content_handler_sort',
      'content_field_name' => $field['field_name'],
      'allow_empty' => TRUE,
    ),
  );
     // Build out additional views data for the cckdestinations "layout" field.
  $data[$table_alias][$field['field_name'] .'_layout'] = array(
    'group' => t('Destinations'),
    'title' => t('@label layout', array('@label' => t($field_types[$field['type']]['label']))) .': '. t($field['widget']['label']) .' ('. $field['field_name'] .')',
    'help' =>  $data[$table_alias][$field['field_name'] .'_tid']['help'],
    'argument' => array(
      'field' => $db_info['columns']['layout']['column'],
      'tablename' => $db_info['table'],
      'handler' => 'cckdestinations_views_handler_argument_layout',
      'click sortable' => TRUE,
      'name field' => '', // TODO, mimic content.views.inc :)
      'content_field_name' => $field['field_name'],
      'allow_empty' => TRUE,
    ),
    'filter' => array(
      'field' => $db_info['columns']['layout']['column'],
      'title' => t('@label layout', array('@label' => t($field_types[$field['type']]['label']))) .': '. t($field['widget']['label']),
      'tablename' => $db_info['table'],
      'handler' => 'cckdestinations_views_handler_filter_layout',
      'additional fields' => array(),
      'content_field_name' => $field['field_name'],
      'allow_empty' => TRUE,
    ),
    'field' => array(
      'field' => $db_info['columns']['layout']['column'],
      'title' => t('@label layout', array('@label' => t($field_types[$field['type']]['label']))) .': '. t($field['widget']['label']),
      'tablename' => $db_info['table'],
      'handler' => 'cckdestinations_views_handler_field_layout',
      'additional fields' => array(),
      'content_field_name' => $field['field_name'],
      'allow_empty' => TRUE,
    ),
    'sort' => array(
      'field' => $db_info['columns']['layout']['column'],
      'tablename' => $db_info['table'],
      'handler' => 'content_handler_sort',
      'content_field_name' => $field['field_name'],
      'allow_empty' => TRUE,
    ),
  );
  $additional_fields = array( 
    $db_info['columns']['rank']['column'],
    $db_info['columns']['time_rank']['column'],
    $db_info['columns']['layout']['column'],
    $db_info['columns']['target']['column'],
    );
  $data[$table_alias][$field['field_name'] .'_aggregation'] = array(
    'group' => t('Destinations'),
    'title' => t('aggregation', array('@label' => t($field_types[$field['type']]['label']))) .': '. t($field['widget']['label']) .' ('. $field['field_name'] .')',
    'help' =>  $data[$table_alias][$field['field_name'] .'_tid']['help'],
    'field' => array(
      'field' => $db_info['columns']['tid']['column'],
      'tablename' => $db_info['table'],
      'handler' => 'cckdestinations_views_handler_field_aggregation',
      'additional fields' => $additional_fields,
      //'click sortable' => TRUE,
      'name field' => '', // TODO, mimic content.views.inc :)
      'content_field_name' => $field['field_name'],
      'allow_empty' => TRUE,
    ),
  );
  return $data;
}
