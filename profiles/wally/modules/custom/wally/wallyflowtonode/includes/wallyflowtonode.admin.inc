<?php
// $Id: yaxim.admin.inc,v 1.0 2010/08/23 14:39:00 rso Exp $

function wallyflowtonode_page_admin_list_feeds($op = '', $flow_key = 0) {
  switch ($op) {
    case 'queue':
      $return = _wallyflowtonode_build_feed_queue($flow_key);
      if ($return)
        return $return;
      else
        drupal_set_message(t('Empty queue.'));
      break;
  }
  
  return _wallyflowtonode_build_feeds_list();
}

function wallyflowtonode_form_delete_feed_form(&$form_state, $flow_key) {
  $question = t('Are you sure you want to delete this flow?');
  
  $add_form = array();
  $add_form['flow_key'] = array(
    '#type' => 'value',
    '#value' => $flow_key,
  );
  
  $form = confirm_form($add_form, $question, 'admin/wally/wallyflowtonode/list');
  $form['#redirect'] = 'admin/wally/wallyflowtonode/list';
  return $form;
}

function wallyflowtonode_form_delete_feed_form_submit(&$form, &$form_state) {
  $values = $form_state['values'];
  $flow_key = $values['flow_key'];
  
  if (db_result(db_query('SELECT * FROM {wallyflowtonode_flows} WHERE `flow_key` = %d', $flow_key))) {
    db_query('DELETE FROM {wallyflowtonode_flows} WHERE `flow_key` = %d', $flow_key);
    drupal_set_message(t('Flow deleted.'));
  } else {
    drupal_set_message(t('No flow matching the requested key.'), 'warning');
  }
}

function wallyflowtonode_page_admin_run_cron() {
  $result = wallyflowtonode_cron();
  drupal_set_message($result[0]);
  return '';
}

/**
 * Display table containing the current flows.
 */
function _wallyflowtonode_build_feeds_list() {  
  $header = array(
    array('data' => t('Name')),
    array('data' => t('Type')),
    array('data' => t('Plugin')),
    array('data' => t('URI')),
    array('data' => t('Signature')),
    array('data' => t('Iteration (min)')),
    array('data' => t('Default image')),
    array('data' => t('Destination')),
    array('data' => t('Package layout')),
    array('data' => t('Body filter')),
    array('data' => t('Queue')),
    array('data' => t('Actions')),
  );
  
  $db_results = db_query("SELECT * FROM {wallyflowtonode_flows}");
  $rows = array();
  while ($db_result = db_fetch_array($db_results)) {
    $temp_row = array();
    $temp_row['name'] = check_plain($db_result['name']);
    $temp_row['type'] = check_plain($db_result['type']);
    $temp_row['plugin_name'] = check_plain($db_result['plugin_name']);
    $temp_row['uri'] = check_plain($db_result['uri']);
    $temp_row['signature'] = check_plain($db_result['signature']);
    $temp_row['delta_t'] = check_plain($db_result['delta_t']);
    $temp_row['channel_img'] = $db_result['channel_img']==1 ? 'yes' : 'no';
    $temp_row['destination'] = check_plain($db_result['destination']);
    $temp_row['package_layout'] = check_plain($db_result['package_layout']);
    $temp_row['body_html'] = check_plain($db_result['body_html']);
    $temp_row['publish'] = _wallyflowtonode_html_table_queue($db_result);
    $temp_row['actions'] = _wallyflowtonode_html_table_actions($db_result);
    $rows[] = $temp_row;
  }
  return theme_table($header, $rows);
}

function _wallyflowtonode_html_table_actions($feed_infos) {
  $return = l(t('Edit'), 'admin/wally/wallyflowtonode/add/'.$feed_infos['flow_key']).' ';
  $return .= l(t('Delete'), 'admin/wally/wallyflowtonode/delete/'.$feed_infos['flow_key']);
  
  return $return;
}

function _wallyflowtonode_html_table_queue($feed_infos) {
  if ($feed_infos['publish']) {
    $return = '';
  } else {
    $return = l(t('Show queue'), 'admin/wally/wallyflowtonode/list/queue/'.$feed_infos['flow_key']).' ';
  }
  
  return $return;
}

function _wallyflowtonode_getdefaultvalues($feed_infos = array()) {
  $default_form_values = array();
  
  if (!empty($feed_infos)) {
    $default_form_values = $feed_infos;
  } else {
    $default_form_values['name'] = '';
    $default_form_values['prefix'] = '';
    $default_form_values['uri'] = '';
    $default_form_values['publish'] = 0;
    $default_form_values['signature'] = '';
    $default_form_values['channel_img'] = 0;
    $default_form_values['delta_t'] = 5;
    $default_form_values['destination'] = '0///0';
    $default_form_values['package_layout'] = '';
    $default_form_values['body_html'] = t('No HTML');
  }
  
  return $default_form_values;
}

function _wallyflowtonode_build_feed_queue($flow_key) {
  $feeds_infos = db_fetch_array(db_query('SELECT * FROM {wallyflowtonode_flows} WHERE `flow_key` = %d', $flow_key));

  if ($feeds_infos['publish']) {
    drupal_set_message(t('All the nodes created from this feed are automatically published'));
    drupal_goto('admin/wally/wallyflowtonode');
  }
  
  $view = views_get_view('flow_to_node_queue');
  $view->set_display('default');
  $view->pager['use_pager'] = TRUE;
  $view->pager['items_per_page'] = 20;
  $arg = (strlen($feeds_infos['prefix']) > 5) ? substr($feeds_infos['prefix'], 0, 5) : $feeds_infos['prefix'];
  while(strlen($arg) < 5)
    $arg .= ':';
  $view->set_arguments(array($arg));

  $return = $view->render();

  if (empty($view->result))
    $return = NULL;
  return $return;
}
