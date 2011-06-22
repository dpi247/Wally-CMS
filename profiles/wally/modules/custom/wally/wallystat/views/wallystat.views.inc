<?php
// $Id: wallystat.views.inc
/**
 * @file
 * Provide views data and handlers for wallystat.module
 */

/**
 * Implementation of hook_views_data()
 */
function wallystat_views_data() {
  // Basic table information.

  // ----------------------------------------------------------------
  // wallystat_node_counter table

  $data['wallystat_node_counter']['table']['group']  = t('Wallystat Node statistics');

  $data['wallystat_node_counter']['table']['join'] = array(
    // ...to the node table
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  // title
  $data['wallystat_node_counter']['title'] = array(
    'title' => t('Title'),
    'help' => t('The title of the node.'),

    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // param01
  $data['wallystat_node_counter']['param01'] = array(
    'title' => t('Custom parameter 01'),
    'help' => t('The Custom parameter 01 for the stats.'),

    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // param02
  $data['wallystat_node_counter']['param02'] = array(
    'title' => t('Custom parameter 02'),
    'help' => t('The Custom parameter 02 for the stats.'),

    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // param03
  $data['wallystat_node_counter']['param03'] = array(
    'title' => t('Custom parameter 03'),
    'help' => t('The Custom parameter 03 for the stats.'),

    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // param04
  $data['wallystat_node_counter']['param04'] = array(
    'title' => t('Custom parameter 04'),
    'help' => t('The Custom parameter 04 for the stats.'),

    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // param05
  $data['wallystat_node_counter']['param05'] = array(
    'title' => t('Custom parameter 05'),
    'help' => t('The Custom parameter 05 for the stats.'),

    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // param06
  $data['wallystat_node_counter']['param06'] = array(
    'title' => t('Custom parameter 06'),
    'help' => t('The Custom parameter 06 for the stats.'),

    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // param07
  $data['wallystat_node_counter']['param07'] = array(
    'title' => t('Custom parameter 07'),
    'help' => t('The Custom parameter 07 for the stats.'),

    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // param08
  $data['wallystat_node_counter']['param08'] = array(
    'title' => t('Custom parameter 08'),
    'help' => t('The Custom parameter 08 for the stats.'),

    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // param09
  $data['wallystat_node_counter']['param09'] = array(
    'title' => t('Custom parameter 09'),
    'help' => t('The Custom parameter 09 for the stats.'),

    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // param10
  $data['wallystat_node_counter']['param10'] = array(
    'title' => t('Custom parameter 10'),
    'help' => t('The custom parameter 10 for the stats.'),

    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // totalcount
  $data['wallystat_node_counter']['totalcount'] = array(
    'title' => t('Total views'),
    'help' => t('The total number of times the node has been viewed.'),

    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // daycount
  $data['wallystat_node_counter']['daycount'] = array(
    'title' => t('Views today'),
    'help' => t('The total number of times the node has been viewed today.'),

    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // weekcount
  $data['wallystat_node_counter']['weekcount'] = array(
    'title' => t('Views this week'),
    'help' => t('The total number of times the node has been viewed this week.'),

    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // monthcount
  $data['wallystat_node_counter']['monthcount'] = array(
    'title' => t('Views this month'),
    'help' => t('The total number of times the node has been viewed this month.'),

    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // yearcount
  $data['wallystat_node_counter']['yearcount'] = array(
    'title' => t('Views this year'),
    'help' => t('The total number of times the node has been viewed this year.'),

    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // timestamp
  $data['wallystat_node_counter']['timestamp'] = array(
    'title' => t('Most recent view'),
    'help' => t('The most recent time the node has been viewed.'),

    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );


  // ----------------------------------------------------------------
  // wallystat_term_counter table

  $data['wallystat_term_counter']['table']['group']  = t('Wallystat Term statistics');

  $data['wallystat_term_counter']['table']['join'] = array(
    // ...to the node table
    'term_data' => array(
      'left_field' => 'tid',
      'field' => 'tid',
    ),
  );

  // totalcount
  $data['wallystat_term_counter']['totalcount'] = array(
    'title' => t('Total views'),
    'help' => t('The total number of times the term has been viewed.'),

    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // daycount
  $data['wallystat_term_counter']['daycount'] = array(
    'title' => t('Views today'),
    'help' => t('The total number of times the term has been viewed today.'),

    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // weekcount
  $data['wallystat_term_counter']['weekcount'] = array(
    'title' => t('Views this week'),
    'help' => t('The total number of times the term has been viewed this week.'),

    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // monthcount
  $data['wallystat_term_counter']['monthcount'] = array(
    'title' => t('Views this month'),
    'help' => t('The total number of times the term has been viewed this month.'),

    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // yearcount
  $data['wallystat_term_counter']['yearcount'] = array(
    'title' => t('Views this year'),
    'help' => t('The total number of times the term has been viewed this year.'),

    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),1333627000
  );
  
  // timestamp
  $data['wallystat_term_counter']['timestamp'] = array(
    'title' => t('Most recent view'),
    'help' => t('The most recent time the term has been viewed.'),

    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // ----------------------------------------------------------------
  // wallystat_node_hourly_counter table

  $data['wallystat_node_hourly_counter']['table']['group']  = t('Wallystat Node hourly statistics');

  $data['wallystat_node_hourly_counter']['table']['join'] = array(
    // ...to the node table
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  // title
  $data['wallystat_node_hourly_counter']['title'] = array(
    'title' => t('Title'),
    'help' => t('The title of the node.'),

    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // param01
  $data['wallystat_node_hourly_counter']['param01'] = array(
    'title' => t('Custom parameter 01'),
    'help' => t('The custom parameter 01 for the stats.'),

    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // param02
  $data['wallystat_node_hourly_counter']['param02'] = array(
    'title' => t('Custom parameter 02'),
    'help' => t('The custom parameter 02 for the stats.'),

    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // param03
  $data['wallystat_node_hourly_counter']['param03'] = array(
    'title' => t('Custom parameter 03'),
    'help' => t('The custom parameter 03 for the stats.'),

    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // param04
  $data['wallystat_node_hourly_counter']['param04'] = array(
    'title' => t('Custom parameter 04'),
    'help' => t('The Custom parameter 04 for the stats.'),

    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // param05
  $data['wallystat_node_hourly_counter']['param05'] = array(
    'title' => t('Custom parameter 05'),
    'help' => t('The Custom parameter 05 for the stats.'),

    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // param06
  $data['wallystat_node_hourly_counter']['param06'] = array(
    'title' => t('Custom parameter 06'),
    'help' => t('The Custom parameter 06 for the stats.'),

    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // param07
  $data['wallystat_node_hourly_counter']['param07'] = array(
    'title' => t('Custom parameter 07'),
    'help' => t('The Custom parameter 07 for the stats.'),

    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // param08
  $data['wallystat_node_hourly_counter']['param08'] = array(
    'title' => t('Custom parameter 08'),
    'help' => t('The Custom parameter 08 for the stats.'),

    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // param09
  $data['wallystat_node_hourly_counter']['param09'] = array(
    'title' => t('Custom parameter 09'),
    'help' => t('The Custom parameter 09 for the stats.'),

    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // param10
  $data['wallystat_node_hourly_counter']['param10'] = array(
    'title' => t('Custom parameter 10'),
    'help' => t('The custom parameter 10 for the stats.'),

    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // hourlycount
  $data['wallystat_node_hourly_counter']['count'] = array(
    'title' => t('Hourly views'),
    'help' => t('The total number of times the node has been viewed each hour the past 48 hours.'),

    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // timestamp
  $data['wallystat_node_hourly_counter']['timestamp'] = array(
    'title' => t('Hour viewed'),
    'help' => t('The hour within which the node was counted.'),

    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  return $data;
}
