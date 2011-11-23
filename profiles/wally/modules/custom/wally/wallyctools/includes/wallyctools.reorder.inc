<?php

function wallyctools_page_reorder_destination($limit = 25, $destination_tid = NULL, $target = NULL) {
  $output = drupal_get_form('wallyctools_form_change_destination_form', 'reorder', $limit, $destination_tid, $target);
  $output .= drupal_get_form('wallyctools_form_reorder_destination_form', $limit, $destination_tid, $target);
  return $output;
}

function wallyctools_form_change_destination_form($form_state, $redirect, $limit, $destination_tid = NULL, $target = NULL) {
  $form = array();

  if (!isset($form_state['post']['operation']) && !isset($form_state['post']['time_rank'])) {
    drupal_add_js(drupal_get_path('module', 'wallytoolbox').'/scripts/shortdest.js');
    
    $options_and_classe = cckdestinations_get_classes_and_options();
    $vocabulary_id = variable_get('wallymport_destinationpath', 0);
  
    $form['destination'] = array(
      '#type' => 'fieldset',
      '#title' => t('Change destination path and target'),
    );
  
    $form['destination']['tid'] = array(
      '#title'            => t('Destination Path'),
      '#type'             => 'select',
      '#default_value'    => $destination_tid,
      '#options'          => array(array('Not Set')) + $options_and_classe['tid']['#options'],
      '#required'         => TRUE,
       // specify theming callback & add a new #attribute to the element.
      '#theme'            => 'cckdestinations_cascading_select',
      '#attributes'       => array('class' => 'tid'),
      '#vid'              => $vocabulary_id,
    );
  
    $form['destination']['target'] = array(
      '#title'            => t('Target'),
      '#type'             => 'select',
      '#default_value'    => $target,
      '#options'          => $options_and_classe['target']['#options'],
      '#classes'          => $options_and_classe['target']['#classes'],
      '#required'         => TRUE,
       // specify theming callback & add a new #attribute to the element.
      '#theme'            => 'cckdestinations_cascading_select',
      '#attributes'       => array('class' => 'target'),
    );
    
    $form['destination']['pager_limit'] = array(
      '#title'            => t('Items per page'),
      '#type'             => 'textfield',
      '#default_value'    => $limit,
      '#required'         => TRUE,
    );
    
    $form['destination']['submit'] = array(
      '#type'             => 'submit',
      '#value'            => t('Change'),
    );
    
    $form['redirect_path'] = array(
      '#type'             => 'value',
      '#value'            => $redirect,
    );
  }
  
  return $form;
}

function wallyctools_form_change_destination_form_submit($form, $form_state) {
  $values = $form_state['values'];
  drupal_goto('admin/wally/wallyctools-operations/'.$values['redirect_path'].'/'.$values['pager_limit'].'/'.$values['tid'].'/'.$values['target']);
}

function wallyctools_form_reorder_destination_form($form_state, $limit = 25, $destination_tid = NULL, $target = NULL){
  if ($target && $destination_tid) {
    $view_infos = wallyctools_get_view_info_from_redacblock($target);
    $view = views_get_view($view_infos['view_name']);
    
    $conf = array(
      'override_pager_settings' => TRUE,
      'nodes_per_page' => $limit,
      'offset' => 0,
      'use_pager' => FALSE,
      'pager_id' => 0,
      'more_link' => FALSE,
      'url' => FALSE,
    );
    wallyctools_prepare_view_for_redacblock($view, $view_infos['view_id'], $target, array($destination_tid), $conf);
    
    $view->execute();
    $results = $view->result;
    $view->destroy();

    $form = array("#tree" => true);
  
    foreach ($results as $j => $result) {
      $db_row = db_query('SELECT n.title, n.type, n.changed
                          FROM {node} n
                          WHERE n.nid = %d', $result->nid);
      $row = db_fetch_object($db_row);
      $row->nid = $result->nid;
      $row->field_destinations_time_rank = $result->node_data_field_destinations_field_destinations_time_rank;
      
      //create a partial table row containing the data from the table
      //add our static "row" data into a form value
      $form['rows'][$row->nid]['data'] = array(
        '#type' => 'value',
        '#value' => $row->title,
      );
  
      $form['rows'][$row->nid]['type'] = array(
        '#type' => 'value',
        '#value' => $row->type,
      );
      
      $form['rows'][$row->nid]['last_update'] = array(
        '#type' => 'value',
        '#value' => $row->changed,
      );
  
      //now create the weight form element.
      //NOTE how we add the id into the element key
      $form['rows'][$row->nid]['weight'] = array(
        '#type' => 'weight',
        '#default_value' => $j,
        //add a specific class in here - we need this later
        '#attributes' => array('class' => 'weight'),
        '#delta' => $limit,
      );
  
      $form['rows'][$row->nid]['time_rank'] = array(
        '#type' => 'value',
        '#value' => $row->field_destinations_time_rank,
      );
  
      $form['rows'][$row->nid]['original_weight'] = array(
        '#type' => 'value',
        '#value' => $j,
      );
  
      $form['rows'][$row->nid]['nid'] = array(
        '#type' => 'value',
        '#value' => $row->nid,
      );
    }
  
    $form['destination_tid'] = array(
      '#type' => 'value',
      '#value' => $destination_tid,
    );
    
    $form['target'] = array(
      '#type' => 'value',
      '#value' => $target,
    );
    
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save changes'),
    );

    $form['goto_current'] = array(
      '#type' => 'submit',
      '#value' => t('Go to current destination'),
      '#submit' => array('wallyctools_form_goto_current_submit'),
    );
  } else {
    drupal_set_message(t('Please choose a destination path and a target.'));
  }
  
  $form['pager_limit'] = array(
    '#type' => 'value',
    '#value' => $limit,
  );

  $form['back'] = array(
    '#type' => 'submit',
    '#value' => t('Return to front page'),
    '#submit' => array('wallyctools_form_back_to_front_submit'),
  );

  return $form;
}

function wallyctools_form_goto_current_submit($form, &$form_state) {
  $values = $form_state['values'];
  drupal_goto('taxonomy/term/'.$values['destination_tid']);
}

function wallyctools_form_back_to_front_submit($form, &$form_state) {
  drupal_goto();
}

function wallyctools_form_reorder_destination_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $tid = $values['destination_tid'];
  $target = $values['target'];
  
  $order_by_weight = array();
  foreach($form_state['values']['rows'] as $key=>$data){
    $order_by_weight[$data['weight']] = $data;
  }

  $new_time_rank = array();
  $weights = array_keys($order_by_weight);
  $min_weight = min($weights);
  $max_weight = max($weights);
  $order_by_weight[$max_weight+1]['time_rank'] = $order_by_weight[$max_weight]['time_rank'] - 30;
  for($i=$max_weight; $i>=$min_weight; $i--) {
    $elem_nid = $order_by_weight[$i]['nid'];
    $elem_node = node_load($elem_nid);
    $new_time_rank[$elem_nid] = $order_by_weight[$i+1]['time_rank'] + 30;
    $order_by_weight[$i]['time_rank'] = $new_time_rank[$elem_nid];
    
    foreach ($elem_node->field_destinations as $delta => $dest) {
      $dest['rank'] = 'DNC'.$dest['rank'];
      if ($dest['tid'] == $tid && $dest['target'] == $target) {
        $dest['time_rank'] = $new_time_rank[$elem_nid];
      }
      $elem_node->field_destinations[$delta] = $dest;
    }
    
    node_save($elem_node);
  }
}

function wallyctools_page_destination_batch_operations($limit = 25, $destination_tid = NULL, $target = NULL) {
  $output = drupal_get_form('wallyctools_form_change_destination_form', 'batchoperations', $limit, $destination_tid, $target);
  $output .= wallyctools_view_batch_operations($limit, $destination_tid, $target);
  $output .= drupal_get_form('wallyctools_form_batch_operations_form', $destination_tid);
  return $output;
}

function wallyctools_view_batch_operations($limit = 25, $destination_tid = NULL, $target = NULL) {
  $return = '';
  
  if ($target && $destination_tid) {
    $view = views_get_view('batch_destinations');
    $view->set_display('page_1');
    $view->override_path = $_GET['q'];
    $view->set_arguments(array($destination_tid, $target));
    $view->pager['use_pager'] = TRUE;
    $view->pager['items_per_page'] = $limit;
  
    $return = $view->render();
    $return .= theme('pager', NULL, $limit, 0);
  } else {
    drupal_set_message(t('Please choose a destination path and a target.'));
  }
  
  return $return;
}

function wallyctools_form_batch_operations_form($form_state, $destination_tid = NULL) {
  $form = array();
  
  if ($destination_tid) {
    $form['destination_tid'] = array(
      '#type' => 'value',
      '#value' => $destination_tid,
    );
    
    $form['goto_current'] = array(
      '#type' => 'submit',
      '#value' => t('Go to current destination'),
      '#submit' => array('wallyctools_form_goto_current_submit'),
    );
  }
  
  $form['back'] = array(
    '#type' => 'submit',
    '#value' => t('Return to front page'),
    '#submit' => array('wallyctools_form_back_to_front_submit'),
  );

  return $form;
}
