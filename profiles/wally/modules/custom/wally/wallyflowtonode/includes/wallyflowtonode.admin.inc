<?php
// $Id: yaxim.admin.inc,v 1.0 2010/08/23 14:39:00 rso Exp $

/**
 * @file
 *   Import Press Content to Drupal structure Administration
 */
 
/*
function wallyflowtonode_form_alter(&$form, $form_state, $form_id) {
  $form['#after_build'][] = 'wallyflowtonode_after_build';
}

function wallyflowtonode_after_build($form, &$form_state) {
  return $form;
}
*/
 
function wallyflowtonode_page_admin($action = '', $op = '', $flow_key = 0) {
  $default_form_values = _wallyflowtonode_getdefaultvalues();
  switch ($op) {
    case 'delete':
      if (db_result(db_query('SELECT * FROM {wallyflowtonode_flows} WHERE `flow_key`=%d', $flow_key))) {
        db_query('DELETE FROM {wallyflowtonode_flows} WHERE `flow_key`=%d', $flow_key);
      } else {
        drupal_set_message('No feed matching the requested key.', "warning");
      }
      break;
    case 'edit':
      if ($feed_infos = db_fetch_array(db_query('SELECT * FROM {wallyflowtonode_flows} WHERE `flow_key`=%d', $flow_key))) {
        $default_form_values = _wallyflowtonode_getdefaultvalues($feed_infos);
      } else {
        drupal_set_message('No feed matching the requested key.', "warning");
      }
      break;
    default:
  }
  
  $return = '';
  $return .= _wallyflowtonode_buildtableforadminpage();
  
  return $return;
}


/**
 * Display table containing the current RSS feeds.
 */
function _wallyflowtonode_buildtableforadminpage() {  
  $header = array(
    array(
      'data' => 'Name',
    ),   
    array(
      'data' => 'URI',
    ),
    array(
      'data' => 'Signature',
    ),
    array(
      'data' => 'Iteration (min)',
    ),
    array(
      'data' => 'Default image',
    ),
    array(
      'data' => 'Destination',
    ),
    array(
      'data' => 'Package layout',
    ),
    array(
      'data' => 'Body filter',
    ),
    array(
      'data' => 'Actions',
    ),
  );
  
  $db_results = db_query("SELECT * FROM {wallyflowtonode_flows}");
  $rows = array();
  while ($db_result = db_fetch_array($db_results)) {
    dsm($db_result);
    $temp_row = array();
    $temp_row['name'] = check_plain($db_result['name']);
    $temp_row['uri'] = check_plain($db_result['uri']);
    $temp_row['signature'] = check_plain($db_result['signature']);
    $temp_row['delta_t'] = check_plain($db_result['delta_t']);
    $temp_row['channel_img'] = $db_result['channel_img']==1 ? 'yes' : 'no';
    $temp_row['destination'] = check_plain($db_result['destination']);
    $temp_row['package_layout'] = check_plain($db_result['package_layout']);
    $temp_row['body_html'] = check_plain($db_result['body_html']);
    $temp_row['actions'] = _wallyflowtonode_htmlcodeforthetableactions($db_result);
    $rows[] = $temp_row;
  }
  return theme_table($header, $rows);
}

function _wallyflowtonode_htmlcodeforthetableactions($feed_infos) {
  $return = l('Edit', 'admin/wally/wallyflowtonode/adminflow/edit/'.$feed_infos['flow_key']).' ';
  $return .= l('Delete', 'admin/wally/wallyflowtonode/adminflow/delete/'.$feed_infos['flow_key']);
  
  return $return;
}

function _wallyflowtonode_getdefaultvalues($feed_infos = array()) {
  $default_form_values = array();
  
  if (!empty($feed_infos)) {
    $default_form_values = $feed_infos;
  } else {
    $default_form_values['name'] = '';
    $default_form_values['uri'] = '';
    $default_form_values['signature'] = '';
    $default_form_values['channel_img'] = 0;
    $default_form_values['delta_t'] = 5;
    $default_form_values['destination'] = '0///0';
    $default_form_values['package_layout'] = '';
    $default_form_values['body_html'] = 'No HTML';
  }
  
  return $default_form_values;
}



















