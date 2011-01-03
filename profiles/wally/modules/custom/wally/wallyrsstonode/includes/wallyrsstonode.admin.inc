<?php
// $Id: yaxim.admin.inc,v 1.0 2010/08/23 14:39:00 rso Exp $

/**
 * @file
 *   Import Press Content to Drupal structure Administration
 */
 
function wallyrsstonode_page_admin() {
  $return = '';
  
  $return .= _wallyrsstonode_buildtableforadminpage();
  $return .= drupal_get_form('wallyrsstonode_page_admin_form');
  
  return $return;
}

/**
 * Settings form.
 */
function wallyrsstonode_page_admin_form($form_state) {
  $form['wallyrsstonode_name'] = array(
    '#type' => 'textfield',
    '#title' => t('RSS feed name'),
    '#description' => t('The name you want to give to this feed.'),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
    '#weight' => 0,
  );
  
  $form['wallyrsstonode_uri'] = array(
    '#type' => 'textfield',
    '#title' => t('RSS feed URI'),
    '#description' => t('URI of the requested RSS feed.'),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
    '#weight' => 0.1,
  );
  
  $minutes_list = array();
  for ($i=1; $i<=60; $i++) $minutes_list[$i] = $i;
  
  $form['wallyrsstonode_deltat'] = array(
    '#type' => 'select',
    '#title' => t('Time between updates'),
    '#description' => t('Time, in minutes, between updates for this feed.'),
    '#options' => $minutes_list,
    '#default_value' => 5,
    '#required' => TRUE,
    '#weight' => 1,
  );
  
  $form['wallyrsstonode_signature'] = array(
    '#type' => 'textfield',
    '#title' => t('Signature (HTML allowed)'),
    '#description' => t('Signature used as copyright for the article package.'),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => FALSE,
    '#weight' => 2,
  );
  
  $all_vocabularies = taxonomy_get_vocabularies();
  foreach ($all_vocabularies as $vocabulary) {
    if ($vocabulary->name=='Document Type') {
      $form['wallyrsstonode_pack_layout'] = taxonomy_form($vocabulary->vid);
      $form['wallyrsstonode_pack_layout']['#required'] = TRUE;
      $form['wallyrsstonode_pack_layout']['#weight'] = 3;
      break;
    }
  }
  
  /*$form['group_destinations'] = array(
    '#theme' => 'fieldset',
    '#title' => t('Destinations'),
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
    '#weight' => 4,
    '#description' => t('Destinations'),
    '#attributes' => array('class'=>'group-destinations'),
  );
  
  $form['group_destinations']['field_destinations'] = array(
    '#theme' => 'content_multiple_values',
    '#title' => t('Destinations'),
    '#description' => t('Destinations'),
    '#tree' => TRUE,
    '#required' => TRUE,
    '#weight' => 4,
    '#prefix' => '<div id="field-destinations-items">',
    '#suffix' => '</div>',
    '#field_name' => 'field_destinations',
  );*/
  
  $form['wallyrsstonode_field_destinations'][0] = array(
    '#title' => t('Destinations'),
    '#description' => t('Destinations'),
    '#type' => 'cckdestinations_widget',
    '#weight' => 4,
  );
  
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
  dsm($form);
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
  $object['destination'] = $form['wallyrsstonode_field_destinations'][0]['tid']['#value'].'/'.$form['wallyrsstonode_field_destinations'][0]['target']['#value'].'/'.$form['wallyrsstonode_field_destinations'][0]['layout']['#value'].'/'.$form['wallyrsstonode_field_destinations'][0]['rank']['#value'];
  $object['delta_t'] = $form['wallyrsstonode_deltat']['#value'];
  $object['signature'] = $form['wallyrsstonode_signature']['#value'];
  $object['package_layout'] = $form['wallyrsstonode_pack_layout']['#value'];
  drupal_write_record('rsstonode_feeds', $object);
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
    $rows[] = $temp_row;
  }
  return theme_table($header, $rows);
}
