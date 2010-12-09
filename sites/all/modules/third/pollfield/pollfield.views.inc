<?php
/**
*implementation hook_views_data
*it takes form uid and join it with uid in user table 
*/

function pollfield_views_data(){
  $data['pollfield_votes']['table']['group']  = t('Pollfield');
  
  $data['pollfield_votes']['table']['join'] = array(
   'node' => array(
      'type' => 'INNER',      
      'left_field' => 'nid',
      'field' => 'nid',
     ),
    
  );
  $data['pollfield_votes']['uid'] = array(
    'title' => t('User ID'),
    'help' => t('User id of user who cast this vote.'),
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
	$data['pollfield_votes']['nid'] = array(
    'title' => t('Node ID'),
    'help' => t('Node id of node where pollfield is placed.'),
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
  $data['pollfield_votes']['field_name_delta'] = array(
    'title' => t('Pollfield ID'),
    'help' => t('Id of pollfield if there is more than one pollfiend in one node.'),
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

   $data['pollfield_votes']['field_name'] = array(
    'title' => t('Field'),
    'help' => t('Name of field.'),

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
	$data['pollfield_votes']['field_table'] = array(
    'title' => t('Content type'),
    'help' => t('Content type where pollfield is stored.'),

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
	$data['pollfield_votes']['question'] = array(
    'title' => t('Question'),
    'help' => t('Question of poll.'),

    'field' => array(
      'handler' => 'views_handler_field_pollfield_question',
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
  
  $data['pollfield_votes']['delta'] = array(
    'title' => t('Choice'),
    'help' => t('User choice.'),

    'field' => array(
      'handler' => 'views_handler_field_pollfield_delta',
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
 
  
  return $data;
}
/**
* hook alter data 
*/
function pollfield_views_data_alter(&$data){
  
}
/**
* hook regsister handlers to views
*/
function pollfield_views_handlers() {
  
  return array(
     'handlers' => array(
      'views_handler_field_pollfield_delta' => array(
        'parent' => 'views_handler_field',
      ),
      'views_handler_field_pollfield_question' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}


