<?php
// $Id: supercron.php,v 1.2 2010/03/18 22:57:45 63reasons Exp $

/**
 * @file
 * Handles incoming requests to fire off regularly-scheduled tasks (cron jobs).
 */

// If you place your supercron.php in other place than Drupal root, or supercron
// directory. Specify your script here.
// $drupal_dir = '';

// Ignore user aborts and allow the script to run forever
ignore_user_abort(TRUE);

// Finding Drupal root directory
if (!isset($drupal_dir)) {
  if (!file_exists('includes/bootstrap.inc')) {
    $exp = '@^(.*)[\\\/]sites[\\\/][^\\\/]+[\\\/]modules[\\\/]supercron$@';
    preg_match($exp, getcwd(), $matches);
    
    if (!empty($matches)) {
      chdir($matches[1]);
    }
  }
}
else {
  chdir($drupal_dir);
}

if (!file_exists('includes/bootstrap.inc')) {
  exit('Can not find Drupal directory.');
}

// Running full Drupal bootstrap
include_once './includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

$safety       = isset($_GET['safety']) ? $_GET['safety'] : '';
$valid_safety = $safety === variable_get(SUPERCRON_SAFETY_VARIABLE, NULL);

// IP authorization check
if (variable_get(SUPERCRON_FIREWALL_ENABLED_VARIABLE, FALSE)) {
  $mode   = variable_get(SUPERCRON_FIREWALL_MODE_VARIABLE, 'only');
  $ip     = ip_address();
  if (is_null($ip)) $ip = '127.0.0.1'; // bash calls return a null calling IP
  $result = db_query('SELECT * FROM {supercron_ips}');
  
  $authorized = $mode === 'except';
  while ($dbip = db_fetch_object($result)) {
    if ($ip == $dbip->ip) {
      $authorized = $mode === 'only';
      break;
    }
  }

  if (!$authorized) {
    exit("IP '$ip' not authorized!");
  }
}

// Throttle check
if (
  module_exists('throttle') 
  && variable_get(SUPERCRON_THROTTLE_SUPPORT_VARIABLE, FALSE)
  && throttle_status()
) {
  exit('Site is under heavy load; cron tasks postponed.');
}

variable_set(SUPERCRON_PHP_BINARY_PATH, $_SERVER['_']);

if ($safety) {
  if (!$valid_safety) {
    exit('Safety mismatch');
  }
  
  $module = isset($_GET['module']) ? $_GET['module'] : '';
  if (!module_exists($module)) {
    exit('Specifically-called module does not exist');
  }

  // multi thread check
  $multithread = isset($_GET['multithread']) ? $_GET['multithread'] : '';
  $multithread = $multithread === 'yes';
  supercron_invoke_one($module, FALSE, $multithread);
}
else {
  supercron_module_invoke_all_cron();
}
