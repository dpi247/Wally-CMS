<?php

function wallyedit_page_list_prenodes_form(&$form_state, $limit = 25) {
  $form = array();

  $form['limit'] = array(
    '#type' => 'textfield',
    '#title' => t('PreNodes par page'),
    '#default_value' => $limit,
    '#required' => TRUE,
  );

  $form['refresh'] = array(
    '#type' => 'submit',
    '#value' => t('Refresh'),
  );

  $form['#suffix'] = wallyedit_page_list_prenodes($limit);
  $form['#suffix'] .= l(t('Delete all the prenodes'), wydit_get_url_action('delete_all'));
  
  return $form;
}

function wallyedit_page_list_prenodes_form_validate($form, &$form_state) {
  $limit = $form_state['values']['limit'];
  if (!is_numeric($limit) || $limit <= 0) {
    form_set_error('limit', t('This value must be a number greater than 0.'));
  }
}

function wallyedit_page_list_prenodes_form_submit($form, &$form_state) {
  $limit = $form_state['values']['limit'];
  drupal_goto('admin/content/wallyedit/'.$limit);
}

function wallyedit_page_list_prenodes($limit = 25) {
  global $custom_theme, $user;
  $custom_theme = variable_get('admin_theme','rubik');
  drupal_add_css(drupal_get_path('module', 'wallyedit').'/css/wallyedit.css');
  module_load_include('inc', 'wallyedit', 'includes/Prenode');

  $count_query = "SELECT count(*) from {wallyedit_prenodes}";
  $result = pager_query("SELECT * from {wallyedit_prenodes} ORDER BY updated DESC", $limit, 0, $count_query);
  
  while($obj = db_fetch_object($result)) {
    $prenode = prenode_get($obj->pid);
    if (!$prenode['locked'] || $prenode['locked']['uid'] == $user->uid) {
      $row['title']['class'] = '';
      $row['title']['title'] = t('This page is currently locked for editing by you. Nobody else may edit this page until these changes are saved or canceled.');
    } else {
      $row['title']['class'] = 'wallyedit-prenode-locked-other';
      $row['title']['title'] = t('This page is currently locked for editing by another user. You may not edit this page without breaking the lock.');
    }
    $row['title']['data'] = $obj->title;
    $row['id'] = $obj->pid;
    $row['type'] = $obj->type;
    $row['last_update'] = date('Y-m-d H:i:s', $obj->updated);
    $row['action_urls'] = l(t('Edit'), wydit_get_url_action('edit_prenode', $obj->pid));
    $row['action_urls'] .= ' · '.l(t('Delete'), wydit_get_url_action('delete', $obj->pid));
    $rows[] = $row;
  }
  $headers = array(t('Title'), t('ID'), t('Type'), t('Last update'), t('Action(s)'));

  return theme('table', $headers, $rows);
}
function wallyedit_page_list_prenodes_dashboard($limit = 25) {
  global $custom_theme, $user;
  $custom_theme = variable_get('admin_theme','rubik');
  drupal_add_css(drupal_get_path('module', 'wallyedit').'/css/wallyedit.css');
  module_load_include('inc', 'wallyedit', 'includes/Prenode');

  $count_query = "SELECT count(*) from {wallyedit_prenodes}";
  $result = pager_query("SELECT * from {wallyedit_prenodes} ORDER BY updated DESC", $limit, 0, $count_query);

  while($obj = db_fetch_object($result)) {
    $prenode = prenode_get($obj->pid);
    if (!$prenode['locked'] || $prenode['locked']['uid'] == $user->uid) {
      $row['title']['class'] = '';
      $row['title']['title'] = t('This page is currently locked for editing by you. Nobody else may edit this page until these changes are saved or canceled.');
    } else {
      $row['title']['class'] = 'wallyedit-prenode-locked-other';
      $row['title']['title'] = t('This page is currently locked for editing by another user. You may not edit this page without breaking the lock.');
    }
    $row['title']['data'] = $obj->title;
    if (preg_match('/[A-Za-z]/', $obj->pid)){
      $row['id'] = t('new');
    } else {
      $row['id'] = t('update');
    }
    $row['type'] = $obj->type;
    $row['last_update'] = date('Y-m-d H:i:s', $obj->updated);
    $row['action_urls'] = l(t('Edit'), wydit_get_url_action('edit_prenode', $obj->pid));
    $row['action_urls'] .= ' · '.l(t('Delete'), wydit_get_url_action('delete', $obj->pid));
    $rows[] = $row;
  }
  $headers = array(t('Title'), t('New/Update'), t('Type'), t('Last update'), t('Action(s)'));

  return theme('table', $headers, $rows);
}
function wallyedit_prenode_delete_confirm(&$form_state, $prenode = NULL) {
  global $custom_theme;
  $custom_theme = variable_get('admin_theme', 'rubik');
  if(!empty($prenode)) {
    $form_state['cache name'] = $prenode->cache_name;
    return confirm_form(
      $form,
      t('Are you sure you want to delete the PreNode with title : "@title" and ID : "@ID"?', array('@title' => $prenode->form_state[$prenode->form_state['type']]['#node']->title, '@ID' => $prenode->cache_name)),
      'admin/content/wallyedit',
      t('This action cannot be undone.'),
      t('Delete'),
      t('Cancel')
    );
  }
}

function wallyedit_prenode_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    module_load_include('inc', 'wallyedit', 'includes/Prenode');
    prenode_delete($form_state);
    drupal_set_message(t('The item has been deleted.'));
  }
  $form_state['redirect'] = 'admin/content/wallyedit';
  return;
}

function wallyedit_prenode_delete_all_confirm(&$form_state) {
  global $custom_theme;
  $custom_theme = variable_get('admin_theme', 'rubik');
  return confirm_form(
    $form,
    t('Are you sure you want to delete all the PreNodes?'),
    'admin/content/wallyedit',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

function wallyedit_prenode_delete_all_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    module_load_include('inc', 'wallyedit', 'includes/Prenode');
    prenode_delete_all();
  }
  $form_state['redirect'] = 'admin/content/wallyedit';
  return;
}

function wallyedit_page_cron_config_form($form_state) {
  $form = array();

  $form['wallyedit_outdated_prenodes'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum age of PreNodes (in days)'),
    '#description' => t('At cron run, all PreNodes which have not been modified for more than this amount of days will be deleted.'),
    '#default_value' => wally_variable_get('wallyedit_outdated_prenodes', 7),
    '#required' => TRUE,
  );
  
  return wally_system_settings_form($form, TRUE);
}
