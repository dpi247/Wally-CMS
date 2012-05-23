<?php

function wallyextra_page_admin_form ($form_state) {
  $form = array();
  
  $form['flowmix'] = array(
    '#type' => 'fieldset',
    '#title' => t('FlowMix settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  
  $form['flowmix']['wallyextra_flowmix_files_days_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Remove files older than (in days)'),
    '#description' => t('The WallyExtra cron will remove files older than this amount of days'),
    '#default_value' => wally_variable_get('wallyextra_flowmix_files_days_limit', 7),
  );
  
  return wally_system_settings_form($form, TRUE);
}
