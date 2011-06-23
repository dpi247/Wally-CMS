<?php
// $Id: yaxim.admin.inc,v 1.0 2010/08/23 14:39:00 rso Exp $

/**
 * @file
 *   Import Press Content to Drupal structure Administration
 */

 
function wallyrsstonode_page_admin_add_feed($op = '', $rss_key = 0) {
  $default_form_values = _wallyrsstonode_getdefaultvalues();
  switch ($op) {
    case 'edit':
      if ($feed_infos = db_fetch_array(db_query('SELECT * FROM `rsstonode_feeds` WHERE `rss_key`=%d', $rss_key))) {
        $default_form_values = _wallyrsstonode_getdefaultvalues($feed_infos);
      } else {
        drupal_set_message('No feed matching the requested key.', "warning");
      }
      break;
    default:
  }
  
  $return = '';
  $return .= drupal_get_form('wallyrsstonode_form_add_feed_form', $default_form_values);
  
  return $return;
}

function wallyrsstonode_page_admin_list_feeds($op = '', $rss_key = 0) {
  switch ($op) {
    case 'delete':
      if (db_result(db_query('SELECT * FROM `rsstonode_feeds` WHERE `rss_key`=%d', $rss_key))) {
        db_query('DELETE FROM `rsstonode_feeds` WHERE `rss_key`=%d', $rss_key);
      } else {
        drupal_set_message('No feed matching the requested key.', "warning");
      }
      break;
    case 'edit':
      drupal_goto('admin/wally/wallyrsstonode/add/edit/'.$rss_key);
      break;
    case 'queue':
      return _wallyrsstonode_build_feed_queue($rss_key);
      break;
  }
  
  return _wallyrsstonode_build_feeds_list();
}

function wallyrsstonode_page_admin_run_cron() {
  $result = wallyrsstonode_cron();
  drupal_set_message($result[0]);
  return '';
}

/**
 * Settings form.
 */
function wallyrsstonode_form_add_feed_form($form_state, $default_form_values) {
  if (isset($default_form_values['rss_key'])) {
    $form['wallyrsstonode_rsskey'] = array(
      '#type' => 'hidden',
      '#required' => FALSE,
      '#default_value' => $default_form_values['rss_key'],
    );
  }
  
  $form['wallyrsstonode_name'] = array(
    '#type' => 'textfield',
    '#title' => t('RSS feed name'),
    '#description' => t('The name you want to give to this feed.'),
    '#default_value' => $default_form_values['name'],
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
    '#weight' => 0,
  );
  
  $form['wallyrsstonode_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('External reference prefix.'),
    '#description' => t('Feed name by default.'),
    '#default_value' => $default_form_values['prefix'],
    '#size' => 80,
    '#maxlength' => 512,
    '#weight' => 0.05,
  );
  
  $form['wallyrsstonode_uri'] = array(
    '#type' => 'textfield',
    '#title' => t('RSS feed URI'),
    '#description' => t('URI of the requested RSS feed.'),
    '#default_value' => $default_form_values['uri'],
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
    '#weight' => 0.1,
  );
  
  $form['wallyrsstonode_publish'] = array(
    '#type' => 'radios',
    '#title' => t('Publishing comportement'),
    '#default_value' => $default_form_values['publish'],
    '#options' => array(t('Place nodes in a queue and choose which ones to publish'), t('Publish all nodes')),
    '#required' => TRUE,
    '#weight' => 0.15,
  );
  
  $body_html_options = array();
  $body_html_query = db_query("SELECT * FROM `filter_formats`");
  $i = 0;
  while ($body_html_element = db_fetch_array($body_html_query)) {
    $body_html_options[] = $body_html_element['name'];
    if ($body_html_element['name'] == $default_form_values['body_html'])
      $default_body_html = $i;
    $i++;
  }

  $form['wallyrsstonode_bodyhtml'] = array(
    '#type' => 'select',
    '#title' => t('Use HTML in the text body'),
    '#description' => t('Use HTML in the text body.'),
    '#options' => $body_html_options,
    '#default_value' => isset($default_body_html) ? $default_body_html : 0,
    '#weight' => 0.2,
  );
  
  $minutes_list = array();
  for ($i=1; $i<=60; $i++) $minutes_list[$i] = $i;
  
  $form['wallyrsstonode_deltat'] = array(
    '#type' => 'select',
    '#title' => t('Time between updates'),
    '#description' => t('Time, in minutes, between updates for this feed.'),
    '#options' => $minutes_list,
    '#default_value' => $default_form_values['delta_t'],
    '#required' => TRUE,
    '#weight' => 1,
  );
  
  $form['wallyrsstonode_signature'] = array(
    '#type' => 'textfield',
    '#title' => t('Signature (HTML allowed)'),
    '#description' => t('Signature used as copyright for the article package.'),
    '#default_value' => $default_form_values['signature'],
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => FALSE,
    '#weight' => 2,
  );
  
  $form['wallyrsstonode_channelimg'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use the default RSS feed image as package image if no other image found.'),
    '#default_value' => $default_form_values['channel_img'],
    '#weight' => 2.1,
  );
  
  $all_vocabularies = taxonomy_get_vocabularies();
  foreach ($all_vocabularies as $vocabulary) {
    if ($vocabulary->name=='Document Type') {
      $form['wallyrsstonode_pack_layout'] = taxonomy_form($vocabulary->vid, $default_form_values['package_layout']);
      $form['wallyrsstonode_pack_layout']['#required'] = TRUE;
      $form['wallyrsstonode_pack_layout']['#weight'] = 4;
      break;
    }
  }
  
  $default_destinations = explode('/', $default_form_values['destination']);
  module_load_include("inc",'content','includes/content.node_form');
  $wallyrsstonode_destinations = content_fields('field_destinations');
  $wallyrsstonode_destinations['widget']['default_value'][0]['tid'] = $default_destinations[0];
  $wallyrsstonode_destinations['widget']['default_value'][0]['target'] = $default_destinations[1];
  $wallyrsstonode_destinations['widget']['default_value'][0]['layout'] = $default_destinations[2];
  $wallyrsstonode_destinations['widget']['default_value'][0]['rank'] = $default_destinations[3];
  $form['wallyrsstonode_destinations'] = (array) content_field_form($form,$form_state,$wallyrsstonode_destinations);
  $form['wallyrsstonode_destinations']['#weight'] = 3;
  unset($form['wallyrsstonode_destinations']['field_destinations'][1]);
  unset($form['wallyrsstonode_destinations']['field_destinations']['field_destinations_add_more']);

  $form['wallyrsstonode_submit'] = array(
    '#type' => 'submit',
    '#title' => t('Save feed'),
    '#value' => t('Save feed'),
    '#weight' => 5,
  );
  
  $form['#redirect'] = 'admin/wally/wallyrsstonode';
  
  return $form;
}

/**
 * Validate function for the settings form.
 */
function wallyrsstonode_form_add_feed_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  $uri_response = drupal_http_request($values['wallyrsstonode_uri']);
  if ($uri_response->status_message != 'OK') {
    form_set_error('wallyrsstonode_uri', t('The URI doesn\'t respond.'));
  }
}

/**
 * Validate function for the settings form.
 */
function wallyrsstonode_form_add_feed_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $object = array();
  $object['name'] = $values['wallyrsstonode_name'];
  $object['prefix'] = $values['wallyrsstonode_prefix'];
  $object['uri'] = $values['wallyrsstonode_uri'];
  $object['publish'] = $values['wallyrsstonode_publish'];
  $rank = strstr($values['field_destinations'][0]['rank'], 'DNC') ? substr($values['field_destinations'][0]['rank'], 3) : $values['field_destinations'][0]['rank'];
  $object['destination'] = $values['field_destinations'][0]['tid'].'/'.$values['field_destinations'][0]['target'].'/'.$values['field_destinations'][0]['layout'].'/'.$rank;
  $object['delta_t'] = $values['wallyrsstonode_delta_t'];
  $object['signature'] = $values['wallyrsstonode_signature'];
  $object['channel_img'] = $values['wallyrsstonode_channelimg'];
  $object['package_layout'] = $values['wallyrsstonode_pack_layout'];
  $object['body_html'] = $form['wallyrsstonode_bodyhtml']['#options'][$values['wallyrsstonode_bodyhtml']];
  if (isset($values['wallyrsstonode_rsskey'])) {
    $object['rss_key'] = $values['wallyrsstonode_rsskey'];
    drupal_write_record('rsstonode_feeds', $object, array('rss_key'));
  } else {
    drupal_write_record('rsstonode_feeds', $object);
  }
}

/**
 * Display table containing the current RSS feeds.
 */
function _wallyrsstonode_build_feeds_list() {  
  $header = array(
    array('data' => t('Name')),
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
  
  $db_results = db_query("SELECT * FROM `rsstonode_feeds`");
  $rows = array();
  while ($db_result = db_fetch_array($db_results)) {
    $temp_row = array();
    $temp_row['name'] = check_plain($db_result['name']);
    $temp_row['uri'] = check_plain($db_result['uri']);
    $temp_row['signature'] = check_plain($db_result['signature']);
    $temp_row['delta_t'] = check_plain($db_result['delta_t']);
    $temp_row['channel_img'] = $db_result['channel_img']==1 ? 'yes' : 'no';
    $temp_row['destination'] = check_plain($db_result['destination']);
    $temp_row['package_layout'] = check_plain($db_result['package_layout']);
    $temp_row['body_html'] = check_plain($db_result['body_html']);
    $temp_row['publish'] = _wallyrsstonode_html_table_queue($db_result);
    $temp_row['actions'] = _wallyrsstonode_html_table_actions($db_result);
    $rows[] = $temp_row;
  }
  return theme_table($header, $rows);
}

function _wallyrsstonode_html_table_actions($feed_infos) {
  $return = l(t('Edit'), 'admin/wally/wallyrsstonode/list/edit/'.$feed_infos['rss_key']).' ';
  $return .= l(t('Delete'), 'admin/wally/wallyrsstonode/list/delete/'.$feed_infos['rss_key']);
  
  return $return;
}

function _wallyrsstonode_html_table_queue($feed_infos) {
  if ($feed_infos['publish']) {
    $return = '';
  } else {
    $return = l(t('Show queue'), 'admin/wally/wallyrsstonode/list/queue/'.$feed_infos['rss_key']).' ';
  }
  
  return $return;
}

function _wallyrsstonode_getdefaultvalues($feed_infos = array()) {
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

function _wallyrsstonode_build_feed_queue($rss_key) {
  $feeds_infos = db_fetch_array(db_query('SELECT * FROM `rsstonode_feeds` WHERE `rss_key`=%d', $rss_key));

  if ($feeds_infos['publish']) {
    drupal_set_message(t('All the nodes created from this feed are automatically published'));
    drupal_goto('admin/wally/wallyrsstonode');
  }
  
  $view = views_get_view('rss_to_node_queue');
  $view->set_display('default');
  $arg = (strlen($feeds_infos['prefix']) > 5) ? substr($feeds_infos['prefix'], 0, 5) : $feeds_infos['prefix'];
  while(strlen($arg) < 5)
    $arg .= ':';
  $view->set_arguments(array($arg));

  return $view->render();
}
