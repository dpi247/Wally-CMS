<?php 

/**
 * Menu callback for filesmigrate admin settings.
 */
function filesmigrate_admin_settings_form() {
  $form = array();
  
  $form['filesmigrate_files_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Amount of files to migrate at each cron run'),
    '#description' => t('This amount can\'t be too high for the cron to run properly.'),
    '#default_value' => wally_variable_get('filesmigrate_files_limit', 50),
    '#required' => TRUE,
  );
  
  $delete_freq_options = array(
    '5' => t('5 minutes'),
    '30' => t('30 minutes'),
    '60' => t('1 hour'),
    '360' => t('6 hours'),
    '1440' => t('1 day'),
  );
  $form['filesmigrate_delete_freq'] = array(
    '#type' => 'select',
    '#title' => t('Old files removal frequency'),
    '#description' => t('The files will be copied at each cron run but only deleted at this frequency.<br>
        The reason for this is that the removal process has to clear the caches.'),
    '#options' => $delete_freq_options,
    '#default_value' => wally_variable_get('filesmigrate_delete_freq', 60),
  );
  
  $run_period_options = array(
    'all_day' => t('All day'),
    'night' => t('Only at night (from midnight to 4 AM)'),
  );
  $form['filesmigrate_run_period'] = array(
    '#type' => 'select',
    '#title' => t('Run the migrate process during'),
    '#description' => t('This process could add some load to your infrastructure, you may want to run it during night only.'),
    '#options' => $run_period_options,
    '#default_value' => wally_variable_get('filesmigrate_run_period', 'night'),
  );
  
  // @todo choix du nouveau path des fichiers, avec token

  return wally_system_settings_form($form, TRUE);
}

/**
 * Settings form validation
 */
function filesmigrate_admin_settings_form_validate($form, $form_state) {
  $values = $form_state['values'];
  
  if (!is_numeric($values['filesmigrate_files_limit']) || $values['filesmigrate_files_limit'] < 0) {
    form_set_error('filesmigrate_files_limit', t('The amount of files to migrate must be a numeric value greater or equal than 0.'));
  }
}
