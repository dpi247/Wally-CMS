<?php

function wallyctools_page_reorder_destination($destination_tid = NULL, $target = NULL) {
  $output = drupal_get_form('wallyctools_form_change_destination_form', 'reorder', $destination_tid, $target);
  $limit = 20;
  $output .= drupal_get_form('wallyctools_form_reorder_destination_form', $limit, $destination_tid, $target);
  return $output;
}

function wallyctools_form_change_destination_form($form_state, $redirect, $destination_tid = NULL, $target = NULL) {
  $form = array();
  
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
    '#required'         => FALSE,
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
    '#required'         => FALSE,
     // specify theming callback & add a new #attribute to the element.
    '#theme'            => 'cckdestinations_cascading_select',
    '#attributes'       => array('class' => 'target'),
  );
  
  $form['destination']['submit'] = array(
    '#type'             => 'submit',
    '#value'            => t('Change'),
  );
  
  $form['redirect_path'] = array(
    '#type'             => 'value',
    '#value'            => $redirect,
  );
  
  return $form;
}

function wallyctools_form_change_destination_form_submit($form, $form_state) {
  $values = $form_state['values'];
  drupal_goto('admin/wally/wallyctools/'.$values['redirect_path'].'/'.$values['tid'].'/'.$values['target']);
}

function wallyctools_form_reorder_destination_form($form_state, $limit = 20, $destination_tid = NULL, $target = NULL){
  //fetch the data from the DB
  $count_query = 'SELECT COUNT(*)
                  FROM {node} n
                  LEFT JOIN {content_field_destinations} dest ON dest.nid=n.nid
                  WHERE dest.field_destinations_tid=%d
                  AND dest.field_destinations_target="%s"
                  ORDER BY dest.field_destinations_time_rank DESC';
  $query = 'SELECT DISTINCT(dest.nid), dest.field_destinations_time_rank, n.title, n.type, n.changed, n.status
            FROM {node} n
            LEFT JOIN {content_field_destinations} dest ON dest.nid=n.nid
            WHERE dest.field_destinations_tid = %d
            AND dest.field_destinations_target = "%s"
            ORDER BY dest.field_destinations_time_rank DESC';
            //LIMIT %d, %d';
  $result = pager_query($query, $limit, 0, $count_query, $destination_tid, $target);

  $form = array("#tree" => true);

  for ($j = 1; $row = db_fetch_object($result); $j++) {
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
    
    $form['rows'][$row->nid]['published'] = array(
      '#type' => 'value',
      '#value' => $row->status,
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
  
  $form['pager_limit'] = array(
    '#type' => 'value',
    '#value' => $limit,
  );

  //Don't forget the submit button
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes'),
  );
  
  
  $form['back'] = array(
    '#type' => 'submit',
    '#value' => t('Return to front page'),
    '#submit' => array('wallyctools_form_back_to_front_submit'),
  );

  return $form;
}

function wallyctools_form_back_to_front_submit($form, &$form_state) {
  drupal_goto();
}

function wallyctools_form_reorder_destination_form_submit($form, &$form_state) {
  $order_by_weight=array();
  foreach($form_state['values']['rows'] as $key=>$data){
    $order_by_weight[$data['weight']] = $data;
  }

  $new_time_rank = array();
  $weights = array_keys($order_by_weight);
  $min_weight = min($weights);
  $max_weight = max($weights);
  for($i=$min_weight; $i<=$max_weight; $i++) {
    $elem_nid = $order_by_weight[$i]['nid'];
    if (isset($order_by_weight[$i+1]) && isset($order_by_weight[$i-1])) {
      // This is a middle element of the list
      $avg_time_rank = (int)(($order_by_weight[$i+1]['time_rank'] + $order_by_weight[$i-1]['time_rank'])/2);
      $new_time_rank[$elem_nid] = max($order_by_weight[$i+1]['time_rank'], $avg_time_rank);
    } elseif (!isset($order_by_weight[$i-1])) {
      // This is the most recent element of the list
      $new_time_rank[$elem_nid] = max($order_by_weight[$i]['time_rank'], $order_by_weight[$i+1]['time_rank'] + 30);
    } elseif (!isset($order_by_weight[$i+1])) {
      // This is the oldest element of the list
      $new_time_rank[$elem_nid] = min($order_by_weight[$i]['time_rank'], $order_by_weight[$i-1]['time_rank'] - 30);
    }
    $order_by_weight[$i]['time_rank'] = $new_time_rank[$elem_nid];
    db_query("UPDATE {content_field_destinations} SET field_destinations_time_rank=%d WHERE nid=%d and field_destinations_tid=%d and field_destinations_target='%s'", $new_time_rank[$elem_nid], $elem_nid, $form_state['values']['destination_tid'], $form_state['values']['target']);
  }
  //optionally set the redirect value in form_submit ...
}

function wallyctools_page_destination_batch_operations($destination_tid = NULL, $target = NULL) {
  $output = drupal_get_form('wallyctools_form_change_destination_form', 'batchoperations', $destination_tid, $target);
  $output .= wallyctools_view_batch_operations($destination_tid, $target);
  $output .= drupal_get_form('wallyctools_form_batch_operations_form');
  return $output;
}

function wallyctools_view_batch_operations($destination_tid = NULL, $target = NULL) {
  $view = views_get_view('batch_destinations');
  $view->set_display('page_1');
  $view->override_path = $_GET['q'];
  $view->set_arguments(array($destination_tid, $target));
  $view->pager['use_pager'] = TRUE;
  $view->pager['items_per_page'] = 20;

  $return = $view->preview();
  $return .= theme('pager', NULL, 20, 0);
  return $return;
}

function wallyctools_form_batch_operations_form($form_state) {
  $form = array();
  
  $form['back'] = array(
    '#type' => 'submit',
    '#value' => t('Return to front page'),
    '#submit' => array('wallyctools_form_back_to_front_submit'),
  );

  return $form;
}
