<?php
// $Id: supercron.firewall-admin.inc,v 1.1 2010/03/18 23:02:38 63reasons Exp $

/**
 * @file
 * Contains administrative configuration details for the Firewall page
 */

include_once(drupal_get_path('module', 'supercron') . '/supercron-main.inc');

/**
 * SuperCron firewall configuration form.
 */
function supercron_firewall_form(&$form_state) {
  $firewall_enabled = variable_get(SUPERCRON_FIREWALL_ENABLED_VARIABLE, FALSE);
  $form['firewall_enable_field'] = array(
    '#type' => 'fieldset',
    '#title' => t('Firewall status'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE
  );
  $form['firewall_enable_field']['firewall_enable'] = array(
    '#type' => 'submit',
    '#prefix' => '<div class="form-item"><label style="display:inline;font-size:120%;">' . t('The firewall is currently') . ' ' .
            ($firewall_enabled?t('enabled') . '. ' . theme_image('misc/watchdog-ok.png', t('ok'), t('ok')):t('disabled') . '. ' . theme_image('misc/watchdog-error.png', t('error'), t('error'))),
    '#suffix' => '</label></div>',
    '#value' => $firewall_enabled?t('disable'):t('enable'),
    '#submit' => array('supercron_firewall_enable'),
    '#attributes' => array('style' => 'margin:0px;')
  );

  if ($firewall_enabled) {
    $form['mode_field'] = array(
      '#type' => 'fieldset',
      '#title' => t('Firewall mode'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE
    );
    $form['mode_field']['mode'] = array(
      '#type' => 'radios',
      '#title' => t('Firewall mode'),
      '#default_value' => variable_get(SUPERCRON_FIREWALL_MODE_VARIABLE, 'only'),
      '#options' => array(
              'only' => t('Only Accept Calls From The Specified IP Addresses')
              , 'except' => t('Accept Calls From Any IP Address Except The Specified IP Addresses')),
      '#description' => t('Select the desired firewall mode.')
    );

    $options = array();

    $form['mode_field']['listing'] = array(
      '#type' => 'markup',
    );

    $form['mode_field']['insert']['ip'] = array(
      '#type' => 'textfield',
      '#title' => t('Insert new IP address'),
      '#description' => t('Some IP address like 192.168.1.2; Your IP is !ip', array(
      '!ip' => ip_address(),
      ))
    );

    $form['mode_field']['insert']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Insert'),
      '#submit' => array('supercron_firewall_add')
    );

    $form['mode_field']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#submit' => array('supercron_firewall_save')
    );

    $result = db_query('SELECT * FROM {supercron_ips}');
    while ($ip = db_fetch_object($result)) {
      if ($firewall_enabled) {
        $form[$ip->iid]['title']  = array('#value' => $ip->ip);
        $form[$ip->iid]['delete'] = array('#value' => l(t('delete'), "admin/settings/supercron/firewall/delete/" . $ip->iid));
      }
      else {
        $form[$ip->iid]['title']  = array('#value' => $ip->ip);
        $form[$ip->iid]['delete'] = array('#value' => '<span style="text-decoration: line-through;">' . t('delete') . '</span>');
      }
    }
  }

  return $form;
}

/**
 * Theming function for supercron firewall configuration form.
 */
function theme_supercron_firewall_form($form) {
  $rows = array();
  foreach (element_children($form) as $key) {
    if (isset($form[$key]['title'])) {
      $ip = &$form[$key];
      $row = array();
      $row[] = drupal_render($ip['title']);
      $row[] = drupal_render($ip['delete']);
      $rows[] = $row;
    }
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No ips available.'), 'colspan' => '2'));
  }

  if (variable_get(SUPERCRON_FIREWALL_ENABLED_VARIABLE, FALSE)) {
    $header[] = t('IP');
    $header[] = t('Operations');

    $form['mode_field']['listing']['#value'] = theme('table', $header, $rows, array('id' => 'ips', 'style' => 'width: 500px'));
    return
      drupal_render($form['firewall_enable_field'])
        . drupal_render($form['mode_field'])
        . drupal_render($form);
  }

  return drupal_render($form['firewall_enable_field']) . drupal_render($form);
}

function supercron_firewall_enable() {
  $enabled = variable_get(SUPERCRON_FIREWALL_ENABLED_VARIABLE, FALSE);
  variable_set(SUPERCRON_FIREWALL_ENABLED_VARIABLE, !$enabled);
  drupal_set_message(t('SuperCron firewall') .' '. (!$enabled?t('enabled'):t('disabled')) .'.');
}

function supercron_firewall_save() {
  variable_set(SUPERCRON_FIREWALL_MODE_VARIABLE, $_POST['mode']);
  drupal_set_message(t('SuperCron firewall mode changed.'));
}

function supercron_firewall_add() {
  db_query("INSERT INTO {supercron_ips} (ip) VALUES ('%s')", $_POST['ip']);
  drupal_set_message(t('IP !ip added.', array('!ip' => $_POST['ip'])));
}

function supercron_firewall_delete($id) {
  db_query("DELETE FROM {supercron_ips} WHERE iid = %d", $id);
  drupal_set_message(t('IP deleted.'));
  drupal_goto('admin/settings/supercron/firewall');
}


