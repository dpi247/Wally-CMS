<?php
// $Id: supercron.autocron-admin.inc,v 1.1 2010/03/18 23:02:38 63reasons Exp $

/**
 * @file
 * Contains administrative configuration details for the Autocron page
 */

include_once(drupal_get_path('module', 'supercron') . '/supercron-main.inc');

function supercron_autocron_form(&$form_state) {
  $autocron_enabled = variable_get(SUPERCRON_AUTOCRON_ENABLED, FALSE);
  $form['description'] = array(
      '#type' => 'markup',
      '#value' => t('Autocron lets you execute cron tasks even if you do not have a functional crontab server or third-party scheduling service. Every time a visitor comes by, Autocron will check to see if a certain amount of time has elapsed. If it has, it will launch the whole SuperCron call sequence as if it had been called by a regular crontab daemon.')
  );
  $form['autocron_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Autocron'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['autocron_fieldset']['autocron_enable'] = array(
    '#type' => 'submit',
    '#prefix' => '<div class="form-item"><label style="display:inline;font-size:120%;">' . t('Autocron is currently') . ' ' .
            ($autocron_enabled?t('enabled') . '. ' . theme_image('misc/watchdog-ok.png', t('ok'), t('ok')):t('disabled') . '. ' . theme_image('misc/watchdog-error.png', t('error'), t('error'))),
    '#suffix' => '</label></div>',
    '#value' => $autocron_enabled?t('disable'):t('enable'),
    '#submit' => array('supercron_autocron_enable'),
    '#attributes' => array('style' => 'margin:0px;')
  );

  if ($autocron_enabled) {
    $form['autocron_fieldset']['interval'] = array(
      '#type' => 'textfield',
      '#title' => t('Minimum interval between invocations (in minutes)'),
        '#size' => 4,
        '#default_value' => round(max(variable_get(SUPERCRON_AUTOCRON_TRESHOLD, 180), 60)/60),
 
    );
     $form['autocron_fieldset']['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Save changes'),
  );

  }
  return $form;
}

function supercron_autocron_enable() {
  $enabled = variable_get(SUPERCRON_AUTOCRON_ENABLED, FALSE);
  variable_set(SUPERCRON_AUTOCRON_ENABLED, !$enabled);
  drupal_set_message(t('SuperCron Autocron') .' '. (!$enabled?t('enabled'):t('disabled')) .'.');
}

function supercron_autocron_form_submit($form, &$form_state) {
    variable_set(SUPERCRON_AUTOCRON_TRESHOLD, max(1,$form_state['values']['interval'])*60);    
}