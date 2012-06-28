<?php

function wallylog_page_main_board() {
  $status = array();

  $display_options = wally_variable_get('wallylog_main_board_display', array());

  if (!empty($display_options)) {
    include_once './includes/install.inc';
    
    foreach ($display_options as $display_option => $active) {
      if ($active) {
        $status = array_merge($status, module_invoke_all('wallylog_board', 'content', $display_option));
      }
    }
    
    uasort($status, '_wallylog_sort_fields_helper');
  } else {
    drupal_set_message(t('No status activated, please go to settings page.'), 'warning');
  }

  return theme('wallylog_page_main_board', $status);
}

function wallylog_page_logs() {
  $wally_filter = _wallylog_build_filter_query();
  $rows = array();
  $icons = array(
    WATCHDOG_DEBUG    => '',
    WATCHDOG_INFO     => '',
    WATCHDOG_NOTICE   => '',
    WATCHDOG_WARNING  => theme('image', 'misc/watchdog-warning.png', t('warning'), t('warning')),
    WATCHDOG_ERROR    => theme('image', 'misc/watchdog-error.png', t('error'), t('error')),
    WATCHDOG_CRITICAL => theme('image', 'misc/watchdog-error.png', t('critical'), t('critical')),
    WATCHDOG_ALERT    => theme('image', 'misc/watchdog-error.png', t('alert'), t('alert')),
    WATCHDOG_EMERG    => theme('image', 'misc/watchdog-error.png', t('emergency'), t('emergency')),
  );
  $classes = array(
    WATCHDOG_DEBUG    => 'wallylog-debug',
    WATCHDOG_INFO     => 'wallylog-info',
    WATCHDOG_NOTICE   => 'wallylog-notice',
    WATCHDOG_WARNING  => 'wallylog-warning',
    WATCHDOG_ERROR    => 'wallylog-error',
    WATCHDOG_CRITICAL => 'wallylog-critical',
    WATCHDOG_ALERT    => 'wallylog-alert',
    WATCHDOG_EMERG    => 'wallylog-emerg',
  );

  $output = drupal_get_form('wallylog_page_logs_filter_form');

  $header = array(
    ' ',
    array('data' => t('Type'), 'field' => 'w.type'),
    array('data' => t('Date'), 'field' => 'w.wid', 'sort' => 'desc'),
    t('Message'),
    array('data' => t('User'), 'field' => 'u.name'),
    array('data' => t('Operations')),
  );
  
  $wallylog_detail_url = 'admin/reports/wallylog/event/dblog/';
  $output .= _wallylog_page_logs_display_logs('wallylog', $header, $wally_filter, $icons, $classes, $wallylog_detail_url, t('Wallylog'));
  
  if (_wallylog_display_dblog()) {
    module_load_include('inc', 'dblog', 'dblog.admin');
    $dblog_filter = dblog_build_filter_query();
    $dblog_detail_url = 'admin/reports/event/';
    $output .= _wallylog_page_logs_display_logs('watchdog', $header, $dblog_filter, $icons, $classes, $dblog_detail_url, t('DBlog'));
  }

  return $output;
}

/**
 * Return form for wallylog administration filters.
 *
 * @ingroup forms
 * @see wallylog_page_logs_filter_form_submit()
 * @see wallylog_page_logs_filter_form_validate()
 */
function wallylog_page_logs_filter_form() {
  $session = &$_SESSION['wallylog_overview_filter'];
  $session = is_array($session) ? $session : array();
  $filters = _wallylog_filters();

  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter log messages'),
    '#theme' => 'wallylog_filters',
    '#collapsible' => TRUE,
    '#collapsed' => empty($session),
  );
  foreach ($filters as $key => $filter) {
    $form['filters']['status'][$key] = array(
      '#title' => $filter['title'],
      '#type' => 'select',
      '#multiple' => TRUE,
      '#options' => $filter['options'],
    );
    if (!empty($session[$key])) {
      $form['filters']['status'][$key]['#default_value'] = $session[$key];
    }
  }
  
  if (module_exists('dblog')) {
    $form['filters']['display_dblog'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display errors from DBlog'),
      '#description' => t('By checking this box you choose to display errors gathered by dblog module. Only logs with severity higher or equal than "error" will be displayed.'),
      '#default_value' => $session['display_dblog'],
    );
  }

  $form['filters']['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
  );
  if (!empty($session)) {
    $form['filters']['buttons']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset')
    );
  }

  return $form;
}

/**
 * Process result from wallylog administration filter form.
 */
function wallylog_page_logs_filter_form_submit($form, &$form_state) {
  $op = $form_state['values']['op'];
  $filters = _wallylog_filters();
  switch ($op) {
    case t('Filter'):
      foreach ($filters as $name => $filter) {
        if (isset($form_state['values'][$name])) {
          $_SESSION['wallylog_overview_filter'][$name] = $form_state['values'][$name];
        }
      }
      if (isset($form_state['values']['display_dblog'])) {
        $_SESSION['wallylog_overview_filter']['display_dblog'] = $form_state['values']['display_dblog'];
        if ($form_state['values']['display_dblog']) {
          unset($_SESSION['dblog_overview_filter']['type']);
          $_SESSION['dblog_overview_filter']['severity'] = array('0', '1', '2', '3');
        }
      }
      break;
    case t('Reset'):
      $_SESSION['wallylog_overview_filter'] = array();
      break;
  }
  return 'admin/reports/wallylog/logs';
}

function wallylog_page_syslog() {
  $rows = array();
  $icons = array(
    WATCHDOG_DEBUG    => '',
    WATCHDOG_INFO     => '',
    WATCHDOG_NOTICE   => '',
    WATCHDOG_WARNING  => theme('image', 'misc/watchdog-warning.png', t('warning'), t('warning')),
    WATCHDOG_ERROR    => theme('image', 'misc/watchdog-error.png', t('error'), t('error')),
    WATCHDOG_CRITICAL => theme('image', 'misc/watchdog-error.png', t('critical'), t('critical')),
    WATCHDOG_ALERT    => theme('image', 'misc/watchdog-error.png', t('alert'), t('alert')),
    WATCHDOG_EMERG    => theme('image', 'misc/watchdog-error.png', t('emergency'), t('emergency')),
  );
  $classes = array(
    WATCHDOG_DEBUG    => 'wallylog-debug',
    WATCHDOG_INFO     => 'wallylog-info',
    WATCHDOG_NOTICE   => 'wallylog-notice',
    WATCHDOG_WARNING  => 'wallylog-warning',
    WATCHDOG_ERROR    => 'wallylog-error',
    WATCHDOG_CRITICAL => 'wallylog-critical',
    WATCHDOG_ALERT    => 'wallylog-alert',
    WATCHDOG_EMERG    => 'wallylog-emerg',
  );

  $header = array(
    ' ',
    array('data' => t('Type')),
    array('data' => t('Date')),
    t('Message'),
    array('data' => t('User')),
    array('data' => t('Operations')),
  );
  
  $output = _wallylog_page_syslog_display_logs($header, $icons, $classes);

  return $output;
}

/**
 * Menu callback; displays details about a log message.
 */
function wallylog_page_event_details($type, $id) {
  $severity = watchdog_severity_levels();
  $output = drupal_get_form('wallylog_page_event_details_back_form', $type);
  
  switch ($type) {
    case 'dblog':
      $result = db_query('SELECT w.*, u.name, u.uid FROM {wallylog} w INNER JOIN {users} u ON w.uid = u.uid WHERE w.wid = %d', $id);
      $dblog = db_fetch_object($result);
      break;

    case 'syslog':
      $log_rows = _wallylog_page_syslog_build_logs();
      if ($log_rows === FALSE || !is_array($log_rows)) {
        return $output;
      }

      list($log_file, $lid) = explode(':', $id);
      $i = 0;
      while($log = array_pop($log_rows)) {
        if ($i == $lid) {
          $dblog = (object)$log;
          break;
        }
        $i++;
      }
      break;
      
    default:
      return $output;
  }

  if ($dblog) {
    $rows = array(
      array(
        array('data' => t('Type'), 'header' => TRUE),
        t($dblog->type),
      ),
      array(
        array('data' => t('Date'), 'header' => TRUE),
        format_date($dblog->timestamp, 'large'),
      ),
      array(
        array('data' => t('User'), 'header' => TRUE),
        theme('username', $dblog),
      ),
      array(
        array('data' => t('Location'), 'header' => TRUE),
        l($dblog->location, $dblog->location),
      ),
      array(
        array('data' => t('Referrer'), 'header' => TRUE),
        l($dblog->referer, $dblog->referer),
      ),
      array(
        array('data' => t('Message'), 'header' => TRUE),
        _wallylog_format_message($dblog),
      ),
      array(
        array('data' => t('Severity'), 'header' => TRUE),
        $severity[$dblog->severity],
      ),
      array(
        array('data' => t('Hostname'), 'header' => TRUE),
        check_plain($dblog->hostname),
      ),
      array(
        array('data' => t('Operations'), 'header' => TRUE),
        $dblog->link,
      ),
    );
    $attributes = array('class' => 'wallylog-event');
    $output = theme('table', array(), $rows, $attributes) . $output;
  } else {
    drupal_set_message(t('Entry not found'), 'error');
    return $output;
  }

  return $output;
}

function wallylog_page_event_details_back_form($form_state, $type) {
  $form = array();
  
  switch ($type) {
    case 'dblog':
      $redirect = '/logs';
      break;

    case 'syslog':
      $redirect = '/syslog';
      break;
      
    default:
      $redirect = '';
      break;
  }
  
  $form['#redirect'] = 'admin/reports/wallylog' . $redirect;
  
  $form['back'] = array(
    '#type' => 'submit',
    '#value' => t('Back'),
  );
  
  return $form;
}

function wallylog_page_settings_form($form_state) {
  $form = array();
  
  $form['wallylog_logs_flush_number'] = array(
    '#type' => 'select',
    '#title' => t('Discard log entries above the following row limit'),
    '#description' => t('The maximum number of rows to keep in the database log. Older entries will be automatically discarded. (Requires a correctly configured <a href="@cron">cron maintenance task</a>.)', array('@cron' => url('admin/reports/status'))),
    '#options' => drupal_map_assoc(array(100, 1000, 10000, 100000, 1000000)),
    '#default_value' => variable_get('wallylog_logs_flush_number', 1000),
  );
  
  $form['main_board'] = array(
    '#type' => 'fieldset',
    '#title' => t('Main Board Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  
  $display_options = module_invoke_all('wallylog_board', 'list');
  $form['main_board']['wallylog_main_board_display'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Display status'),
    '#options' => $display_options,
    '#default_value' => wally_variable_get('wallylog_main_board_display', array()),
  );
  
  $form['logs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Logs settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  
  $logs_modules_options = module_invoke_all('wallylog_logs');
  $form['logs']['wallylog_logs_modules'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Accept logs from'),
    '#options' => $logs_modules_options,
    '#default_value' => wally_variable_get('wallylog_logs_modules', array()),
  );

  return wally_system_settings_form($form, TRUE);
}

function _wallylog_admin_devel_status(&$error) {
  $checks = array();
  
  $checks['theme_registry'] = array(
    'title' => t('Theme registry rebuild on every request'),
    'description' => '',
  );
  if (variable_get('devel_rebuild_theme_registry', FALSE)) {
    $checks['theme_registry']['value'] = t('Yes, edit settings !link_here', array('!link_here' => l(t('here'), 'admin/settings/devel')));
    $checks['theme_registry']['severity'] = REQUIREMENT_ERROR;
    $error = TRUE;
  } else {
    $checks['theme_registry']['value'] = t('No');
    $checks['theme_registry']['severity'] = REQUIREMENT_OK;
  }
  
  $checks['query_log'] = array(
    'title' => t('Query log'),
    'description' => '',
  );
  if (variable_get('dev_query', 0)) {
    $checks['query_log']['value'] = t('Collected') . (variable_get('devel_query_display', 0) ? t(' and displayed') : '') . t(', edit settings !link_here', array('!link_here' => l(t('here'), 'admin/settings/devel')));
    $checks['query_log']['severity'] = REQUIREMENT_ERROR;
    $error = TRUE;
  } else {
    $checks['query_log']['value'] = t('Not collected');
    $checks['query_log']['severity'] = REQUIREMENT_OK;
  }
  
  return theme('status_report', $checks);
}

function _wallylog_admin_hacked_status(&$error) {
  return theme('hacked_report', _wallylog_hacked_calculate_project_data($error));
}

function _wallylog_hacked_calculate_project_data(&$error) {
  // Try to get the report form cache if we can.
  $cache = cache_get('hacked:full-report', HACKED_CACHE_TABLE);
  if (!empty($cache->data) && !$force) {
    foreach ($cache->data as $key => $project) {
      if ($project['status'] > 1) {
        $error = TRUE;
      }
      if (is_null($project['includes'])) {
        $cache->data[$key]['includes'] = array();
      }
    }
    return $cache->data;
  } else {
    $error = TRUE;
    return '';
  }
}

function _wallylog_admin_memcache_status() {
  
}

/**
 * Build query for wallylog administration filters based on session.
 */
function _wallylog_build_filter_query() {
  if (empty($_SESSION['wallylog_overview_filter'])) {
    return;
  }

  $filters = _wallylog_filters();

  // Build query
  $where = $args = array();
  foreach ($_SESSION['wallylog_overview_filter'] as $key => $filter) {
    if (is_array($filter) && isset($filter['where'])) {
      $filter_where = array();
      foreach ($filter as $value) {
        $filter_where[] = $filters[$key]['where'];
        $args[] = $value;
      }
      if (!empty($filter_where)) {
        $where[] = '('. implode(' OR ', $filter_where) .')';
      }
    }
  }
  $where = !empty($where) ? implode(' AND ', $where) : '';

  return array(
    'where' => $where,
    'args' => $args,
  );
}

function _wallylog_display_dblog() {
  $return = FALSE;
  
  if (module_exists('dblog') &&
      !empty($_SESSION['wallylog_overview_filter']) &&
      isset($_SESSION['wallylog_overview_filter']['display_dblog']) &&
      $_SESSION['wallylog_overview_filter']['display_dblog']) {
    $return = TRUE;
  }
  
  return $return;
}

/**
 * List wallylog administration filters that can be applied.
 */
function _wallylog_filters() {
  $filters = array();

  foreach (_wallylog_get_message_types() as $type) {
    $types[$type] = t($type);
  }

  if (!empty($types)) {
    $filters['type'] = array(
      'title' => t('Type'),
      'where' => "w.type = '%s'",
      'options' => $types,
    );
  }

  $filters['severity'] = array(
    'title' => t('Severity'),
    'where' => 'w.severity = %d',
    'options' => watchdog_severity_levels(),
  );

  return $filters;
}

function _wallylog_get_message_types() {
  $types = array();

  $result = db_query('SELECT DISTINCT(type) FROM {wallylog} ORDER BY type');
  while ($object = db_fetch_object($result)) {
    $types[] = $object->type;
  }

  return $types;
}

/**
 * Formats a log message for display.
 *
 * @param $dblog
 *   An object with at least the message and variables properties
 */
function _wallylog_format_message($dblog) {
  // Legacy messages and user specified text
  if ($dblog->variables === 'N;') {
    return $dblog->message;
  }
  // Message to translate with injected variables
  else {
    return t($dblog->message, unserialize($dblog->variables));
  }
}

function _wallylog_page_logs_display_logs($table, $header, $filter, $icons, $classes, $detail_url, $caption = NULL) {
  $sql = "SELECT w.wid, w.uid, w.severity, w.type, w.timestamp, w.message, w.variables, w.link, u.name FROM {$table} w INNER JOIN {users} u ON w.uid = u.uid";
  $tablesort = tablesort_sql($header);
  if (!empty($filter['where'])) {
    $result = pager_query($sql ." WHERE ". $filter['where'] . $tablesort, 50, 0, NULL, $filter['args']);
  } else {
    $result = pager_query($sql . $tablesort, 50);
  }

  $rows = array();
  while ($dblog = db_fetch_object($result)) {
    $rows[] = array('data' =>
      array(
        // Cells
        $icons[$dblog->severity],
        t($dblog->type),
        format_date($dblog->timestamp, 'small'),
        l(truncate_utf8(_wallylog_format_message($dblog), 56, TRUE, TRUE), $detail_url.$dblog->wid, array('html' => TRUE)),
        theme('username', $dblog),
        filter_xss($dblog->link),
      ),
      // Attributes for tr
      'class' => "wallylog-". preg_replace('/[^a-z]/i', '-', $dblog->type) .' '. $classes[$dblog->severity]
    );
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No log messages available.'), 'colspan' => 6));
  }

  $output = theme('table', $header, $rows, array('id' => 'admin-wallylog'), $caption);
  $output .= theme('pager', NULL, 50, 0);
  return $output;
}

function _wallylog_page_syslog_display_logs($header, $icons, $classes) {
  $log_rows = _wallylog_page_syslog_build_logs();
  if ($log_rows === FALSE || !is_array($log_rows)) {
    return '';
  }
  
  global $pager_page_array, $pager_total, $pager_total_items;
  $page = isset($_GET['page']) ? (int)$_GET['page'] : 0;
  $pager_page_array[0] = $page;
  $limit = 50;
  $i = 0;

  $rows = array();
  while ($log = array_pop($log_rows)) {
    if ($i >= $page*$limit && $i < ($page*$limit + $limit)) {
      $dblog = (object)$log;

      $rows[] = array('data' =>
        array(
          // Cells
          $icons[$dblog->severity],
          t($dblog->type),
          format_date($dblog->timestamp, 'small'),
          l(truncate_utf8(_wallylog_format_message($dblog), 56, TRUE, TRUE), 'admin/reports/wallylog/event/syslog/'.$file->name.':'.$i, array('html' => TRUE)),
          theme('username', $log_usr),
          filter_xss($dblog->link),
        ),
        // Attributes for tr
        'class' => "wallylog-". preg_replace('/[^a-z]/i', '-', $dblog->type) .' '. $classes[$dblog->severity]
      );
    }

    $i++;
  }

  if (empty($rows)) {
    $rows[] = array(array('data' => t('No log messages available.'), 'colspan' => 6));
  }

  $pager_total[0] = ceil($i/$limit);
  $pager_total_items[0] = sizeof($i);

  $output = theme('table', $header, $rows, array('id' => 'admin-wallylog'));
  $output .= theme('pager', NULL, $limit, 0);
  return $output;
}

function _wallylog_page_syslog_build_logs() {
  $syslog_id = variable_get('syslog_identity', 'drupal');
  $log_dir = variable_get('syslog_log_dir', '/var/log');
  $files = file_scan_directory($log_dir, '.*\.log' .'$', array('.', '..', 'CVS'), 0, FALSE, 'name', 0);
  if (empty($files)) {
    drupal_set_message(t('No log files in the given directory ("@dir").', array('@dir' => $log_dir)), 'error');
    return FALSE;
  }
  ksort($files);
  // Limit display at 5 days
  $files = array_slice($files, -5, 5, TRUE);

  global $base_url;
  $rows = array();

  foreach ($files as $file) {
    $handle = fopen($file->filename, 'r');
    if (!$handle) {
      drupal_set_message(t('The log file (!log_file) isn\'t readable', array('!log_file' => $file->filename)), 'error');
      continue;
    }

    while($contents = fgets($handle)) {
      if (strstr($contents, $syslog_id.': ')) {
        list($a, $raw_log) = explode($syslog_id.': ', $contents);

        $log = array();
        list($log_url,
          $log['severity'],
          $log['timestamp'],
          $log['type'],
          $log['ip'],
          $log['request_uri'],
          $log['referer'],
          $log['uid'],
          $log['link'],
          $log['message']) = explode('|', $raw_log);

        $log_usr = user_load($log['uid']);

        if ($log_url == $base_url) {
          $rows[] = $log;
        }
      }
    }
  }

  return $rows;
}

/**
 * Sort function for items order.
 * (copied form element_sort(), which acts on #weight keys)
 */
function _wallylog_sort_fields_helper ($a, $b) {
  $a_weight = (is_array($a) && isset($a['weight'])) ? $a['weight'] : 0;
  $b_weight = (is_array($b) && isset($b['weight'])) ? $b['weight'] : 0;
  if ($a_weight == $b_weight) {
    return 0;
  }
  return ($a_weight < $b_weight) ? -1 : 1;
}
