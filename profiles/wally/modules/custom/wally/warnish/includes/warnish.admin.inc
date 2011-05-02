<?php

/**
 * @file
 * warnish.admin.inc
 *
 * Administrative functions for Varnish integration.
 */
 
/**
 * Menu callback for warnish admin settings.
 */
function warnish_admin_settings_form() {
  $form = array();
  
  if (!extension_loaded('sockets')) {
    drupal_set_message(t('<a href="http://php.net/manual/en/sockets.installation.php">PHP Sockets extension</a> not enabled. Varnish terminal communication configuration skipped.'), 'error');
    return system_settings_form($form);
  }

  // Begin socket-dependent configuration.
  
  // Decide whether or not to flush caches on cron runs.
  $form['warnish_flush_cron'] = array(
    '#type' => 'radios',
    '#title' => t('Flush page cache on cron?'),
    '#options' => array(
      0 => t('Disabled'), 
      1 => t('Enabled (with respect for cache_lifetime)'),
    ),
    '#default_value' => variable_get('warnish_flush_cron', 0),
    '#description' => t('Internally Drupal will attempt to flush its page cache every time cron.php runs. This can mean too-frequent cache flushes if you have cron running frequently. NOTE: this cache flush is global!'),
  );

  $form['warnish_legacy'] = array(
    '#type' => 'checkbox',
    '#title' => t('Warnish Legacy Mode'),
    '#default_value' => variable_get('warnish_legacy', 0),
    '#required' => TRUE,
    '#description' => t('Check this if you use a version of Varnish lower than 2.1'),
  );
      
  $form['warnish_control_terminal'] = array(
    '#type' => 'textfield',
    '#title' => t('Warnish Control Terminal'),
    '#default_value' => variable_get('warnish_control_terminal', '127.0.0.1:6082'),
    '#required' => TRUE,    
    '#description' => t('Set this to the server IP or hostname that Varnish runs on (e.g. 127.0.0.1:6082). This must be configured for Drupal to talk to Varnish. Separate multiple servers with spaces.'),
  );
  $form['warnish_control_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Warnish Control Key'),
    '#default_value' => variable_get('warnish_control_key', ''),
    '#description' => t('Optional: if you have established a secret key for control terminal access, please put it here.'),
  );
  
  $form['warnish_socket_timeout'] = array(
   '#type' => 'textfield',
   '#title' => t('Warnish connection timeout (milliseconds)'),
   '#default_value' => variable_get('warnish_socket_timeout', WARNISH_DEFAULT_TIMETOUT),
   '#description' => t('If Varnish is running on a different server, you may need to increase this value.'),
   '#required' => TRUE,
  );
  
  $form['warnish_cache_clear'] = array(
    '#type' => 'radios',
    '#title' => t('Warnish Cache Clearing'),
    '#options' => array(
      WARNISH_DEFAULT_CLEAR => t('Drupal Default'), 
      WARNISH_NO_CLEAR => t('None'),
    ),
    '#default_value' => variable_get('warnish_cache_clear', WARNISH_DEFAULT_CLEAR),
    '#description' => t('What kind of cache clearing Warnish should utilize. Drupal default will clear all page caches on node updates and cache flush events. None will allow pages to persist for their full max-age; use this if you want to write your own cache-clearing logic.'),
  );
  
  // Detect expire module and add this option.
  if (module_exists('expire')) {
    $form['warnish_cache_clear']['#options'][WARNISH_SELECTIVE_CLEAR] = t('Selective (experimental; uses expire.module)');
    $form['warnish_cache_clear']['#description'] .= ' '. t('Selective will clear a list of URLs you specify as well as any node urls themselves.');
  } else {
    $form['warnish_cache_clear']['#description'] .= ' '. t('Installing the !link will enable "Selective" clearing.', array('!link' => '<a href="http://drupal.org/project/expire" target="_blank">'. t('Expire module') .'</a>'));
  }

  // Check status
  $form['warnish_stats'] = array(
    '#type' => 'item',
    '#title' => t('Status'),
    '#value' => theme('warnish_status', warnish_get_status()),
  );

  return system_settings_form($form);
}

function warnish_admin_settings_form_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['warnish_socket_timeout']) || $form_state['values']['warnish_socket_timeout'] < 0) {
    form_set_error('warnish_socket_timeout', t('Varnish connection timeout must be a positive number.'));
  } else {
    $form_state['values']['warnish_socket_timeout'] = (int) $form_state['values']['warnish_socket_timeout'];
  }
}

/**
 * Menu callback for warnish admin settings.
 */
function warnish_admin_reports_page() {
  // connect to Varnish and do a full status report
  $status = _warnish_terminal_run('stats');
  $output = '';
  foreach ($status as $stat) {
    $output .= '<pre>'. $stat['msg'] .'</pre>';
  }
  return $output;
}
