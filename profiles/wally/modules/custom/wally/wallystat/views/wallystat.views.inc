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
    ),
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
  
  return $data;
}
