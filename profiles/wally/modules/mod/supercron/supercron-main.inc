<?php

include_once(drupal_get_path('module', 'supercron') . '/supercron.inc');

/**
 * Implement hook_help().
 */
function supercron_help($path, $arg) {
  switch ($path) {
    case 'admin/help#supercron':
      $output = '<p>'. t("The SuperCron module lets you take control of your Cron tasks; you can enable or disable cron hooks, change the order in which hooks are called, capture any stray output produced by a module, get performance information, survive and document exceptions.");
      $output .= '<p>'. t('The <a href="@settings">Settings</a> page is your main interface to SuperCron; it displays every hook and its status, and lets you control the flow of execution. You can even launch cron (all of it or just the parts you want) from here.', array('@settings' => url(SUPERCRON_ADMIN_MENU_PATH_SETTINGS))) .'</p>';
      $output .= '<p>'. t('If you need help setting up a cron job on a POSIX-compliant crontab system the <a href="@crontab">Crontab configuration</a> page will help you find the right command line to call up the whole cron script, or, for advanced users, just a part of it.', array('@crontab' => url(SUPERCRON_ADMIN_MENU_PATH_CRONTAB_CONFIGURATION))) .'</p>';
      $output .= '<p>'. t('It is recommended that you turn on the built-in SuperCron <a href="@firewall">firewall</a> to prevent malicious visitors from overloading your system and thus creating a <a href="@dos-attack">denial-of-service attack</a>.', array('@firewall' => url(SUPERCRON_ADMIN_MENU_PATH_FIREWALL), '@dos-attack' => url('http://en.wikipedia.org/wiki/Denial-of-service_attack'))) .'</p>';
      $output .= '<p>'. t('An RSS feed of exceptions raised during Cron processes is also available; for the subscription to work you must give the <pre>access the supercron exception feed</pre> access right to the anonymous user before you subscribe to the obfuscated <a href="@rss">RSS URL</a>.', array('@rss' => url(supercron_rss_url()))) .'</p>';
      return $output;
      case 'admin/settings/supercron':
      $output = '<p>'. t('Here you can enable or disable parts of the cron process, change the order in which cron hooks are called, capture extraneous output or exceptions, measure performance and even make your cron tasks multithreaded.') .'</p>';
      return $output;
    case 'admin/settings/supercron/firewall':
      $output = '<p>'. t('SuperCron sports a mini firewall that offers a way to limit access to your cron start-up file and thus limits the abilities of nefarious users to overload your site for a denial-of-service attack through cron.') .'</p>';
      return $output;
    case 'admin/settings/supercron/invocation':
      $output = '<p>'. t('This section helps you configure your Crontab to either start a complete SuperCron run or call up a module hook separately') .'</p>';
      return $output;
  }
}

function supercron_rss_url() {
  $url = variable_get(SUPERCRON_EXCEPTIONS_RSS_URL, '');

  if (empty($url)) {
      // We are purposefully not using _supercron_get_safety_variable() here as this URL can be published
      // elsewhere (such as web-based feed readers) in a way that would risk compromising the safety variable
    $url = SUPERCRON_MENU_PATH_EXCEPTION_RSS_FEED . '/' . _supercron_make_safety_token() . '.rss';
    variable_set(SUPERCRON_EXCEPTIONS_RSS_URL, $url);
  }

  return $url;
}

function supercron_autocron_url() {
  $url = variable_get(SUPERCRON_AUTOCRON_URL, '');

  if (empty($url)) {
    $url = SUPERCRON_MENU_PATH_AUTO_CRON . _supercron_get_safety_variable();
    variable_set(SUPERCRON_AUTOCRON_URL, $url);
  }

  return $url;
}



/**
 * Implements hook_menu().
 */
function supercron_menu() {
  $items[supercron_rss_url()] = array(
    'title' => 'Supercron exception',
    'access arguments' => array(SUPERCRON_EXCEPTION_FEED_PERMISSION),
    'page callback' => 'supercron_exception_rss',
    'type' => MENU_CALLBACK,
  );
  $items['admin/settings/supercron'] = array(
    'title'            => 'SuperCron',
    'description'      => 'Configure how cron behaves, which cron hooks are called and in what order, capture debugging information and parallelize tasks.',
    'page callback'    => 'drupal_get_form',
    'page arguments'   => array('supercron_settings'),
    'access arguments' => array(SUPERCRON_ADMIN_PERMISSION),
    'file'             => 'supercron.admin.inc',
  );
  $items[SUPERCRON_ADMIN_MENU_PATH_SETTINGS] = array(
    'title'  => 'Settings',
    'description'      => 'The main SuperCron interface, where you can enable or disable parts of the cron process, change the order in which cron hooks are called, capture extraneous output or exceptions, measure performance and even make your cron tasks multithreaded.',
    'type'   => MENU_DEFAULT_LOCAL_TASK,
    'access arguments' => array(SUPERCRON_ADMIN_PERMISSION),
    'weight' => -10,
  );
  $items[SUPERCRON_ADMIN_MENU_PATH_FIREWALL] = array(
    'title'            => 'Firewall',
    'description'      => 'The SuperCron firewall interface lets you decide which computers can trigger a cron script call.',
    'page callback'    => 'drupal_get_form',
    'page arguments'   => array('supercron_firewall_form'),
    'access arguments' => array(SUPERCRON_ADMIN_PERMISSION),
    'type'             => MENU_LOCAL_TASK,
    'file'             => 'supercron.firewall-admin.inc',
  );
  $items['admin/settings/supercron/firewall/delete'] = array(
    'page callback'    => 'supercron_firewall_delete',
    'description'      => 'A callback method to remove a firewall host from the list.',
    'access arguments' => array(SUPERCRON_ADMIN_PERMISSION),
    'type'             => MENU_CALLBACK,
    'file'             => 'supercron.firewall-admin.inc',
  );
  $items[SUPERCRON_AJAX_CALLBACK] = array(
    'page callback'    => 'supercron_administration_get_results',
    'description'      => 'SuperCron\'s AJAX bucket of power goodness.',
    'access arguments' => array(SUPERCRON_ADMIN_PERMISSION),
    'type'             => MENU_CALLBACK,
    'file'             => 'supercron.admin.inc',
  );
  $items[SUPERCRON_ADMIN_MENU_PATH_CRONTAB_CONFIGURATION] = array(
    'title'            => 'Crontab configuration',
    'description'      => 'A page to help you configure your cron daemon.',
    'page callback'    => 'drupal_get_form',
    'page arguments'   => array('supercron_invocation_form'),
    'access arguments' => array(SUPERCRON_ADMIN_PERMISSION),
    'type'             => MENU_LOCAL_TASK,
    'weight'           => 10,
    'file'             => 'supercron.crontab-admin.inc',
  );
  $items[SUPERCRON_ADMIN_MENU_PATH_AUTOCRON] = array(
    'title'            => 'Autocron',
    'description'      => 'This lets you configure a cron substitute if you do not have access to a crontab daemon or thrid-party cron service.',
    'access arguments' => array(SUPERCRON_ADMIN_PERMISSION),
    'type'             => MENU_LOCAL_TASK,
    'page callback'    => 'drupal_get_form',
    'page arguments'   => array('supercron_autocron_form'),
    'file'             => 'supercron.autocron-admin.inc',
    'weight'           => 12,
  );
  $items['admin/settings/supercron/output/%'] = array(
    'title'            => 'Cron output in module',
    'description'      => 'This lets you view the captured output for a given hook.',
    'page callback'    => 'supercron_output',
    'page arguments'   => array(4),
    'access arguments' => array(SUPERCRON_ADMIN_PERMISSION),
    'type'             => MENU_CALLBACK,
    'file'             => 'supercron.admin.inc',
  );
  $items['admin/settings/supercron/exception/%'] = array(
    'title'            => 'Cron exceptions in module',
    'description'      => 'This lets you view the observed exceptions for a given hook.',
    'page callback'    => 'supercron_exception',
    'page arguments'   => array(4),
    'access arguments' => array(SUPERCRON_ADMIN_PERMISSION),
    'type'             => MENU_CALLBACK,
    'file'             => 'supercron.admin.inc',
  );
  $items['admin/settings/supercron/call/%/%'] = array(
    'title'            => 'Call individual cron handlers',
    'description'      => 'This lets you call a specific cron hook.',
    'page callback'    => 'supercron_invoke_one',
    'page arguments'   => array(4, 5),
    'access arguments' => array(SUPERCRON_ADMIN_PERMISSION),
    'type'             => MENU_CALLBACK,
    'file'             => 'supercron.inc',
  );
  $items[SUPERCRON_INVOKE_ALL] = array(
    'page callback'    => 'supercron_drupal_cron_run',
    'description'      => 'This lets you call every enabled cron hook in sequence.',
    'access arguments' => array(SUPERCRON_ADMIN_PERMISSION),
    'type'             => MENU_CALLBACK,
    'file'             => 'supercron.inc',
  );
  $items[SUPERCRON_MENU_PATH_RUN_CRON] = array(
    'page callback'    => 'supercron_drupal_cron_run',
    'description'      => 'This lets you call every enabled cron hook in sequence through a URL; this can help you integrate SuperCron processes in modules and Drush scripts.',
    'access arguments' => array(SUPERCRON_ADMIN_PERMISSION),
    'type'             => MENU_CALLBACK,
    'file'             => 'supercron.inc',
  );
  $items[supercron_autocron_url()] =  array(
    'page callback'    => 'supercron_drupal_cron_run',
    'description'      => 'This lets autocron call all the enabled cron hooks in sequence.',
    'access arguments' => array('access content'),
    'type'             => MENU_CALLBACK,
    'file'             => 'supercron.inc',
  );

  return $items;
}

/**
 * Implements hook_theme().
 */
function supercron_theme() {
  return array(
    'supercron_settings' => array(
      'arguments' => array('form' => NULL),
    ),
    'supercron_firewall_form' => array(
      'arguments' => array('form' => NULL),
    ),
    'supercron_invocation_form' => array(
      'arguments' => array('form' => NULL)
    )
  );
}

/**
 * Implement hook_perm().
 */
function supercron_perm() {
  return array(SUPERCRON_ADMIN_PERMISSION, SUPERCRON_EXCEPTION_FEED_PERMISSION);
}


/**
 * Implements hook_exit().
 */
function supercron_exit() {

    // We do not wish to launch AutoCron as the module is potentially being uninstalled
    $current_url = $_REQUEST['q'];
    if (strpos($current_url, 'admin/build/modules') === 0) return;
    
    $time_since_last_run = (time() - variable_get('cron_last', 0));
    $execute = (variable_get(SUPERCRON_AUTOCRON_AUTOENABLED, TRUE) AND ($time_since_last_run >= variable_get(SUPERCRON_AUTOCRON_AUTOENABLED_TRESHOLD, SUPERCRON_AUTOCRON_AUTOENABLED_TRESHOLD_DEFAULT)));
    if (!$execute) $execute = (variable_get(SUPERCRON_AUTOCRON_ENABLED, FALSE) AND ($time_since_last_run >= variable_get(SUPERCRON_AUTOCRON_TRESHOLD, 180)));
    if ($execute) _supercron_parallel_call(url(supercron_autocron_url(), array('absolute'=>TRUE)));
}

function supercron_base_path() {
  global $base_path;
  return $base_path . drupal_get_path('module', 'supercron') . '/' . SUPERCRON_SCRIPT_NAME ;
}

function supercron_standard_path() {
  global $base_path;
  return $base_path . SUPERCRON_SCRIPT_NAME ;
}

function supercron_get_script_URL() {
  global $base_url;
  $script_path = variable_get(SUPERCRON_SCRIPT_PATH, SUPERCRON_SCRIPT_PATH_MODULE_ROOT);
  
  //@todo: DDU FIX this SUPERCRON_SCRIPT_PATH_MODULE_ROOT and SUPERCRON_SCRIPT_PATH_DRUPAL_ROOT not defined
  if ($script_path === SUPERCRON_SCRIPT_PATH_DRUPAL_ROOT|| true) {
    return $base_url .'/'. SUPERCRON_SCRIPT_NAME;
  }
  elseif ($script_path == SUPERCRON_SCRIPT_PATH_MODULE_ROOT) {
    return url(drupal_get_path('module', 'supercron') . '/supercron.php', array('absolute' => TRUE));
  }
  else {
    return $script_path;
  }
}

function supercron_get_script_path() {
  $script_path = variable_get(SUPERCRON_SCRIPT_PATH, SUPERCRON_SCRIPT_PATH_MODULE_ROOT);

  if ($script_path == SUPERCRON_SCRIPT_PATH_DRUPAL_ROOT) {
    return supercron_standard_path();
  }
  elseif ($script_path == SUPERCRON_SCRIPT_PATH_MODULE_ROOT) {
    return supercron_base_path();
  }
  else {
    return '';
  }
}



/**
 * A generic function for generating RSS feeds from a set of supercron
 * exceptions.
 */
function supercron_exception_rss() {
  global $base_url, $language;

  $namespaces = array('xmlns:dc' => 'http://purl.org/dc/elements/1.1/');

  $q = db_query_range("SELECT * FROM {watchdog} WHERE type = 'cron exception' ORDER BY wid DESC", 10, 0);
  $items = '';
  while ($v = db_fetch_object($q)) {
    $variables = unserialize($row["variables"]);

    $exceptions[$row["wid"]]= array(
      'message'   => $variables["@message"],
      'location'  => $row["location"],
      'timestamp' => $row["timestamp"]
    );

  $items .= format_rss_item(
    $variables["@message"],
    $item->link,
    t($variables["@message"], unserialize($variables))
  );
}

  $channel = array(
    'version'     => '2.0',
    'title'       => variable_get('site_name', 'Drupal'),
    'link'        => $base_url,
    'description' => variable_get('site_mission', ''),
    'language'    => $language->language
  );

  $output = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
  $output .= "<rss version=\"". $channel["version"] ."\" xml:base=\"". $base_url ."\" ". drupal_attributes($namespaces) .">\n";
  $output .= format_rss_channel($channel['title'], $channel['link'], $channel['description'], $items, $channel['language']);
  $output .= "</rss>\n";

  drupal_set_header('Content-Type: application/rss+xml; charset=utf-8');
  print $output;
}


// Qualify the agressive cache warning
function supercron_form_system_performance_settings_alter(&$form, &$form_state) {
    $form['page_cache']['cache']['#description'] = str_replace('supercron', t('supercron (if cron daemon crash protection or autocron are enabled these features will not function, but the rest of supercron will function normally)'), $form['page_cache']['cache']['#description']);
}
