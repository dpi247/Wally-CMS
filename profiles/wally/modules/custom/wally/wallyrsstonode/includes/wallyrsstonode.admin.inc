<?php
// $Id: yaxim.admin.inc,v 1.0 2010/08/23 14:39:00 rso Exp $

/**
 * @file
 *   Import Press Content to Drupal structure Administration
 */
 
/**
 * Implementation of hook_form_alter()
 */
function wallyrsstonode_form_alter(&$form, $form_state, $form_id) {
  $form['#after_build'][] = 'wallyrsstonode_after_build';
}

/**
 * Implementation of hook_after_build()
 */
function wallyrsstonode_after_build($form, &$form_state) {
  return $form;
}
 
function wallyrsstonode_page_admin($action = '', $op = '', $rss_key = 0) {
  $default_form_values = _wallyrsstonode_getdefaultvalues();
  switch ($op) {
    case 'delete':
      if (db_result(db_query('SELECT * FROM `rsstonode_feeds` WHERE `rss_key`=%d', $rss_key))) {
        db_query('DELETE FROM `rsstonode_feeds` WHERE `rss_key`=%d', $rss_key);
      } else {
        drupal_set_message('No feed matching the requested key.', "warning");
      }
      break;
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
  $return .= _wallyrsstonode_buildtableforadminpage();
  $return .= drupal_get_form('wallyrsstonode_page_admin_form', $default_form_values);
  
  return $return;
}

/**
 * Settings form.
 */
function wallyrsstonode_page_admin_form($form_state, $default_form_values) {
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
    '#default_value' => $default_body_html,
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
  
  return $form;
}

/**
 * Validate function for the settings form.
 */
function wallyrsstonode_page_admin_form_validate($form, &$form_state) {
  $uri_response = drupal_http_request($form['wallyrsstonode_uri']['#value']);
  if ($uri_response->status_message != 'OK') {
    form_set_error('wallyrsstonode_uri', t('The URI doesn\'t respond.'));
  }
}

/**
 * Validate function for the settings form.
 */
function wallyrsstonode_page_admin_form_submit($form, &$form_state) {
  $object = array();
  $object['name'] = $form['wallyrsstonode_name']['#value'];
  $object['uri'] = $form['wallyrsstonode_uri']['#value'];
  $object['destination'] = $form['wallyrsstonode_destinations']['field_destinations'][0]['tid']['#value'].'/'.$form['wallyrsstonode_destinations']['field_destinations'][0]['target']['#value'].'/'.$form['wallyrsstonode_destinations']['field_destinations'][0]['layout']['#value'].'/'.$form['wallyrsstonode_destinations']['field_destinations'][0]['rank']['#value'];
  $object['delta_t'] = $form['wallyrsstonode_deltat']['#value'];
  $object['signature'] = $form['wallyrsstonode_signature']['#value'];
  $object['package_layout'] = $form['wallyrsstonode_pack_layout']['#value'];
  $object['body_html'] = $form['wallyrsstonode_bodyhtml']['#options'][$form['wallyrsstonode_bodyhtml']['#value']];
  if (isset($form['wallyrsstonode_rsskey']['#value'])) {
    $object['rss_key'] = $form['wallyrsstonode_rsskey']['#value'];
    drupal_write_record('rsstonode_feeds', $object, array('rss_key'));
  } else {
    drupal_write_record('rsstonode_feeds', $object);
  }
}

/**
 * Display table containing the current RSS feeds.
 */
function _wallyrsstonode_buildtableforadminpage() {  
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
  
  $db_results = db_query("SELECT * FROM `rsstonode_feeds`");
  $rows = array();
  while ($db_result = db_fetch_array($db_results)) {
    $temp_row = array();
    $temp_row['name'] = check_plain($db_result['name']);
    $temp_row['uri'] = check_plain($db_result['uri']);
    $temp_row['signature'] = check_plain($db_result['signature']);
    $temp_row['delta_t'] = check_plain($db_result['delta_t']);
    $temp_row['channel_img'] = $db_result['channel_img']!='' ? 'yes' : 'no';
    $temp_row['destination'] = check_plain($db_result['destination']);
    $temp_row['package_layout'] = check_plain($db_result['package_layout']);
    $temp_row['body_html'] = check_plain($db_result['body_html']);
    $temp_row['actions'] = _wallyrsstonode_htmlcodeforthetableactions($db_result);
    $rows[] = $temp_row;
  }
  return theme_table($header, $rows);
}

function _wallyrsstonode_htmlcodeforthetableactions($feed_infos) {
  $return = l('Edit', 'admin/wally/wallyrsstonode/adminfeeds/edit/'.$feed_infos['rss_key']).' ';
  $return .= l('Delete', 'admin/wally/wallyrsstonode/adminfeeds/delete/'.$feed_infos['rss_key']);
  
  return $return;
}

function _wallyrsstonode_getdefaultvalues($feed_infos = array()) {
  $default_form_values = array();
  
  if (!empty($feed_infos)) {
    $default_form_values = $feed_infos;
  } else {
    $default_form_values['name'] = '';
    $default_form_values['uri'] = '';
    $default_form_values['signature'] = '';
    $default_form_values['delta_t'] = 5;
    $default_form_values['destination'] = '0///0';
    $default_form_values['package_layout'] = '';
    $default_form_values['body_html'] = 'No HTML';
  }
  
  return $default_form_values;
}
