<?php
// $Id: supercron.inc,v 1.1 2010/03/18 23:02:38 63reasons Exp $
//include_once(drupal_get_path('module', 'supercron') . '/supercron-main.inc');
include_once('supercron-main.inc');

/**
 * @file
 * Contains helper methods
 */

// SuperCron invoke-all jobs path
define('SUPERCRON_INVOKE_ALL',                  'admin/reports/status/run-supercron');

// Variable constants; duplicating literals is an open invitation to bugs
define('SUPERCRON_SCRIPT_NAME',                 'supercron.php');
//define('SUPERCRON_PHP_BINARY_PATH_PLACEHOLDER', t('[path to php binary]'));
define('SUPERCRON_PHP_BINARY_PATH_PLACEHOLDER', '[path to php binary]');
define('SUPERCRON_ADMIN_PERMISSION',            'administer supercron');
define('SUPERCRON_EXCEPTION_FEED_PERMISSION',   'access the supercron exception feed');
define('SUPERCRON_AJAX_CALLBACK',               'admin/settings/supercron/get-results');
define('SUPERCRON_ADMIN_MENU_PATH_CRONTAB_CONFIGURATION', 'admin/settings/supercron/invocation');
define('SUPERCRON_ADMIN_MENU_PATH_SETTINGS', 'admin/settings/supercron/settings');
define('SUPERCRON_ADMIN_MENU_PATH_FIREWALL', 'admin/settings/supercron/firewall');
define('SUPERCRON_ADMIN_MENU_PATH_AUTOCRON', 'admin/settings/supercron/autocron');
define('SUPERCRON_MENU_PATH_EXCEPTION_RSS_FEED', 'supercron/exceptions');
define('SUPERCRON_MENU_PATH_RUN_CRON', 'admin/reports/status/run-cron');
define('SUPERCRON_MENU_PATH_AUTO_CRON', 'supercron/run-autocron-');
define('SUPERCRON_AUTOCRON_URL', 'supercron_autocron_url');
define('SUPERCRON_EXCEPTIONS_RSS_URL', 'supercron_exceptions_RSS_url');

define('SUPERCRON_SAFETY_VARIABLE',  'supercron_safety');
define('SUPERCRON_FIREWALL_ENABLED_VARIABLE',   'supercron_firewall_enabled');
define('SUPERCRON_FIREWALL_MODE_VARIABLE',      'supercron_firewall_mode');
define('SUPERCRON_THROTTLE_SUPPORT_VARIABLE',   'supercron_respond_to_throttling');
define('SUPERCRON_CLEAR_LOGS',      'supercron_dblog_clear_messages');
define('SUPERCRON_SCRIPT_PATH',                 'supercron_script_path');
define('SUPERCRON_SCRIPT_PATH_DRUPAL_ROOT',     1);
define('SUPERCRON_SCRIPT_PATH_MODULE_ROOT',     2);
define('SUPERCRON_SCRIPT_PATH_CUSTOM',          3);
define('SUPERCRON_PHP_BINARY_PATH',             'supercron_php_binary_path');
define('SUPERCRON_AUTOCRON_TRESHOLD',           'supercron_autocron_treshold');
define('SUPERCRON_AUTOCRON_ENABLED',           'supercron_autocron_enabled');
define('SUPERCRON_AUTOCRON_AUTOENABLED',           'supercron_autocron_auto_enabled');
define('SUPERCRON_AUTOCRON_AUTOENABLED_TRESHOLD',           'supercron_autocron_auto_enabled_treshold');
define('SUPERCRON_AUTOCRON_AUTOENABLED_TRESHOLD_DEFAULT_HOURS', (int)12); // 12 hours in minutes
define('SUPERCRON_AUTOCRON_AUTOENABLED_TRESHOLD_DEFAULT', (int)SUPERCRON_AUTOCRON_AUTOENABLED_TRESHOLD_DEFAULT_HOURS*60*60); // 12 hours in minutes

function supercron_drupal_cron_run() {
  // If not in 'safe mode', increase the maximum execution time:
  if (!ini_get('safe_mode')) {
    set_time_limit(240);
  }

  // Fetch the cron semaphore
  $semaphore = variable_get('cron_semaphore', FALSE);

  if ($semaphore) {
    if (time() - $semaphore > 3600) {
      // Either cron has been running for more than an hour or the semaphore
      // was not reset due to a database error.
      watchdog('cron', 'Cron has been running for more than an hour and is most likely stuck.', array(), WATCHDOG_ERROR);

      // Release cron semaphore
      variable_del('cron_semaphore');
    }
    else {
      // Cron is still running normally.
      watchdog('cron', 'Attempting to re-run cron while it is already running.', array(), WATCHDOG_WARNING);
    }
  }
  else {
    // Register shutdown callback
    register_shutdown_function('drupal_cron_cleanup');

    // Lock cron semaphore
    variable_set('cron_semaphore', time());

    // Iterate through the modules calling their cron handlers (if any):
    supercron_module_invoke_all_cron();

    // Record cron time
    variable_set('cron_last', time());
    watchdog('cron', 'Cron run completed.', array(), WATCHDOG_NOTICE);

    // Release cron semaphore
    variable_del('cron_semaphore');
    drupal_goto(getenv("HTTP_REFERER"));
  }
}

function supercron_module_set_exec_intervals_and_times_count($module, $start_time, $end_time) {
  $interval = $end_time - $start_time;

  db_query(
    "UPDATE {supercron_enabled_mods} "
      . "SET timestamp = %d,"
      . " last_exec_interval = %d,"
      . " total_exec_interval = total_exec_interval + %d,"
      . " times_count = times_count + 1"
      . " WHERE module_name = '%s'",
    $start_time, $interval, $interval, $module
  );
}

function supercron_clear_logs() {
  if (module_exists('dblog') && variable_get(SUPERCRON_CLEAR_LOGS, TRUE)) {
    db_query(
      "DELETE FROM {watchdog} WHERE type = 'cron' AND severity = %d",
      WATCHDOG_NOTICE
    );
  }
}


function supercron_module_add_output($module, $output) {
  db_query(
    "UPDATE {supercron_enabled_mods}"
    . " SET output = Concat(output, '%s<br />')"
    . " WHERE module_name = '%s'",
    $output, $module
  );
}

function supercron_module_invoke_all_cron() {
  if (variable_get(SUPERCRON_CLEAR_LOGS, TRUE)) {
    supercron_clear_logs();
  }

  module_load_include('inc', 'supercron', 'supercron');
  return _supercron_module_invoke_all_cron();
}

function supercron_invoke_one($module, $return = FALSE, $multithread = FALSE) {
  try {
    ob_start();
    $start_time = time();
    module_invoke($module, 'cron');
    supercron_module_set_exec_intervals_and_times_count($module, $start_time, time());
    $output = ob_get_contents();
    ob_end_clean();
    if (!empty($output)) {
      supercron_module_add_output($module, $output);
    }

    // Record cron time
    variable_set('cron_last', time());
  }
  catch (Exception $e) {
    ob_end_clean();
    $massage = $e->getMessage();
    watchdog(
      'cron exception',
      t("Module name: @module;\nException message: @message;"),
      array(
        '@module'  => $module,
        '@message' => !empty($massage) ? $massage : t('no message')
      ),
      WATCHDOG_ERROR
    );
  }

  if ($return) {
    drupal_goto(getenv("HTTP_REFERER"));
  }
}

// based on Kyle Florence's code at http://php.net/manual/en/function.rand.php
function _supercron_rand_str($length = 128, $chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890!@#$%^&*()_+=-[]{}\|\'"<>,.?/')
{
  mt_srand(); // not necessary, except when it is
    // Length of character list
    $chars_length = (strlen($chars) - 1);

    // Start our string
    $string = $chars{mt_rand(0, $chars_length)};

    // Generate random string
    for ($i = 1; $i < $length; $i = strlen($string))
    {
        // Grab a random character from our list
        $r = $chars{rand(0, $chars_length)};

        // Make sure the same two characters don't appear next to each other
        if ($r != $string{$i - 1}) $string .=  $r;
    }

    // Return the string
    return $string;
}

function _supercron_make_safety_token()
{
  return md5(_supercron_rand_str() . '-' .rand() .'-'. mt_rand() . '-' . _supercron_rand_str()); // yes, we are a tad paranoid
}

function _supercron_get_safety_variable() {
    $value = variable_get(SUPERCRON_SAFETY_VARIABLE, '');
    if (empty($value))
    {
        $value = _supercron_make_safety_token();
        variable_set(SUPERCRON_SAFETY_VARIABLE, $value);
    }
  return $value;
}

function _supercron_parallel_call($url)
{
  $parts = parse_url($url);
  $u     = $parts['path'] .'?'. $parts['query'];
  $port  = isset($parts['port']) ? $parts['port'] : 80;
  $fp    = fsockopen($parts['host'], $port, $errno, $errstr, 20);
  if ($fp) {
        $out = "GET {$u} HTTP/1.1\r\n";
        $out .= "Host: {$parts['host']}\r\n";
        $out .= "Connection: Close\r\n\r\n";
        fwrite($fp, $out);

        stream_set_blocking($fp, FALSE);
        stream_set_timeout($fp, 2);
        fclose($fp);
      }
}

function supercron_list_of_cron_modules() {
  return module_implements('cron', TRUE);
}

function supercron_fetch_all($sort = 0, $only_enabled = 0) {
  $filter = $only_enabled ? ' WHERE enabled = 1' : '';
  $sort   = $sort == 0 ? "ASC" : "DESC";

  $modules = array();
  $q = db_query('SELECT * FROM {supercron_enabled_mods}'. $filter .' ORDER BY weight ' . $sort);
  while ($module = db_fetch_object($q)) {
    $modules[] = $module;
  }

  return $modules;
}

function supercron_list_handled_modules() {
  $result = array();
  $modulenames = db_query('SELECT module_name FROM {supercron_enabled_mods} ORDER BY module_name ASC');
  while ($row = db_fetch_object($modulenames)) {
    $result [] = $row->module_name;
  }
  return $result;
}

function supercron_remove_disabled_modules() {
  $modules = supercron_list_handled_modules();
  foreach ($modules as $module) {
    if (!module_hook($module, 'cron')) {
      db_query('DELETE FROM {supercron_enabled_mods} WHERE module_name = \'%s\'', $module);
    }
  }
}

function supercron_update_module_list() {
  supercron_remove_disabled_modules();
  supercron_add_newly_enabled_modules();
}

function supercron_add_newly_enabled_modules() {
  $modules = supercron_list_of_cron_modules();
  $max_weight = db_result(db_query("SELECT MAX(weight) FROM {supercron_enabled_mods}"));
  if ($max_weight == NULL) {
    $max_weight = 0;
  }
  else {
    $max_weight++;
  }

  foreach ($modules as $module) {
    $is = db_result(db_query_range(
      "SELECT COUNT(*) FROM {supercron_enabled_mods} WHERE module_name = '%s'",
      $module, 0, 1
    ));

    if (!$is) {
      db_query(
        "INSERT INTO {supercron_enabled_mods} (module_name, weight, output) VALUES ('%s', %d, '')",
        $module, $max_weight
      );

      $max_weight++;
    }
  }
}

function supercron_rule_is_valid($rule) {
  // check for valid rule format
  $format = '([0-9*,/-]+)';
  $format = "@^{$format}\s+{$format}\s+{$format}\s+{$format}\s+{$format}$@";
  if (!preg_match($format, $rule, $rules)) {
    return FALSE;
  }

  return $rules;
}

function _supercron_module_invoke_all_cron() {
  global $base_url;

  $safety = _supercron_get_safety_variable();
  supercron_update_module_list();
  
  $modules         = array();
  $run_in_parallel = array();
  foreach (supercron_fetch_all(0, 1) as $module) {
    $modules[] = $module;
    //echo '<pre>'.print_r($module,1).'</pre>';
    $run_in_parallel[$module->module_name] = $module->detached;
  }
  unset($modules_config_result);

  // there is no cron jobs enabled.
  if (empty($modules)) {
    variable_set('cron_last', time());
    return FALSE;
  }

  variable_set('cron_semaphore', time());

  foreach ($modules as $module) {
      
    if (FALSE === supercron_should_run($module)) {
      continue;
    }

    if ($run_in_parallel[$module->module_name]) {
      
      $url = supercron_get_script_URL();
      $url .= '?safety='. drupal_urlencode($safety);
      $url .= '&module='. drupal_urlencode($module->module_name);
      $url .= '&multithread=yes';
      _supercron_parallel_call($url);
    }
    else {
      supercron_invoke_one($module->module_name);
    }
  }

  // Record cron time
  variable_set('cron_last', time());
  watchdog('cron', 'Cron run completed.', array(), WATCHDOG_NOTICE);

  // Release cron semaphore
  variable_del('cron_semaphore');

  return TRUE;
}

/**
 * Used by _supercron_should_run().
 */
function _supercronDecodeRule($rule, $min, $max) {
  if ($rule == '*')
    return range($min, $max);

  $result = array();
  foreach (explode(',', $rule) as $token) {
    if (preg_match('/^([0-9]+)-([0-9]+)$/', $token, $r)) {
      $result = array_merge($result, range($r[1], $r[2]));
    }
    elseif (preg_match('/^\*\/([0-9]+)$/', $token, $r)) {
      for ($i = $min; $i <= $max; $i++)
        if ($i % $r[1] == 0)
          $result[] = $i;
    }
    elseif (is_numeric($token)) {
      $result[] = $token;
    }
  }
  return $result;
}

/**
 * Used by _supercron_should_run().
 */
function _supercronT($time, $d = -1, $h = -1, $m = -1) {
  if ($d < 0) {
    return date('n', $time) * 31 * 24 * 60 + date('j', $time) * 24 * 60 + date('H', $time) * 60 + date('i', $time);
  }
  else {
    return $time * 31 * 24 * 60 + $d * 24 * 60 + $h * 60 + $m;
  }
}

/**
 * Used by _supercron_should_run().
 */
function _supercronMonDaysFromWeekDays($year, $mon, $weekdays) {
  $result = array();
  for ($i = 1; checkdate($mon, $i, $year); $i++) {
    $w = date('w', mktime(12, 00, 00, $mon, $i, $year));
    if (in_array($w, $weekdays)) {
      $result[] = $i;
    }
  }
  return $result;
}

/**
 * Limit job run by checking rule.
 */
function supercron_should_run($module) {
  if (TRUE === empty($module->rule)) {
    return TRUE;
  }

  $now = $_SERVER['REQUEST_TIME'];

  // job never run or run one year ago
  if ((!$module->timestamp) || ($now - $module->timestamp > 365 * 86400)) {
    return TRUE;
  }
  return _supercron_should_run($module, $now);
}

/**
 * Time check
 */
function _supercron_should_run($module, $now) {
  // check for valid rule format
  $rules = supercron_rule_is_valid( $module->rule);
  if (FALSE === $rules) {
    watchdog('cron', 'Invalid rule found (%module): %rule', array(
      '%module' => $module->module_name,
      '%rule'   => $module->rule
    ));
    
    return FALSE;
  }

  $weekdayspec = ($rules[5] != '*');
  $mondayspec  = ($rules[3] != '*');

  $rules[5] = _supercronDecodeRule($rules[5], 0, 6);
  $rules[4] = _supercronDecodeRule($rules[4], 1, 12);
  $rules[3] = _supercronDecodeRule($rules[3], 1, 31);
  $rules[2] = _supercronDecodeRule($rules[2], 0, 23);
  $rules[1] = _supercronDecodeRule($rules[1], 0, 59);

  $last_t     = _supercronT($module->timestamp + 30);
  $now_t      = _supercronT($now);
  $now_t_delta = $now_t - $last_t + ($last_t > $now_t ? 12 * 31 * 24 * 60 : 0);
  $year      = date('Y', $module->timestamp);

  if ($mondayspec || (!$mondayspec && !$weekdayspec)) {
    $first = -1;

    foreach ($rules[4] as $mon) {
      foreach ($rules[3] as $d) {
        if (checkdate($mon, $d, $year)) {
          foreach ($rules[2] as $h) {
            foreach ($rules[1] as $m) {
              $t = _supercronT($mon, $d, $h, $m);

              if ($first < 0) {
                $first = $t;
              }

              if ($t > $last_t) {
                $next_t = $t;
                break 4;
              }
            }
          }
        }
      }
    }

    if (!$next_t) {
      $next_t = $first;
    }

    $next_t_delta = $next_t - $last_t + ($last_t > $next_t ? 12 * 31 * 24 * 60 : 0);

    if ($now_t_delta >= $next_t_delta) {
      return TRUE;
    }
  }

  if ($weekdayspec) {
    foreach ($rules[4] as $mon) {
      foreach (_supercronMonDaysFromWeekDays($year, $mon, $rules[5]) as $d) {
        foreach ($rules[2] as $h) {
          foreach ($rules[1] as $m) {
            $t = _supercronT($mon, $d, $h, $m);

            if ($t > $last_t) {
              $next_t = $t;
              break 4;
            }
          }
        }
      }
    }

    if (!$next_t) {
      // ust get the first of following year (if day_of_week is specified it's
      // not the same of previous one)
      foreach ($rules[4] as $mon) {
        foreach (_supercronMonDaysFromWeekDays($year + 1, $mon, $rules[5]) as $d) {
          foreach ($rules[2] as $h) {
            foreach ($rules[1] as $m) {
              $next_t = _supercronT($mon, $d, $h, $m);
              break 4;
            }
          }
        }
      }
    }

    $next_t_delta = $next_t - $last_t + ($last_t > $next_t ? 12 * 31 * 24 * 60 : 0);

    if ($now_t_delta >= $next_t_delta) {
      return TRUE;
    }
  }

  return FALSE;
}
