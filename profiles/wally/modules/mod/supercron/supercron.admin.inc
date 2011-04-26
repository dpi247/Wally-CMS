<?php
// $Id: supercron.admin.inc,v 1.1 2010/03/18 23:02:38 63reasons Exp $

/**
 * @file
 * Contains administrative configuration details for the main Settings page
 */

//include_once(drupal_get_path('module', 'supercron') . '/supercron-main.inc');
include_once('supercron-main.inc');

function _supercron_show_script_warning() {
  drupal_set_message(
    t(
      'The !script file should copied from the !here directory to the !path directory.  ' .
      'Until that is done, your tasks will not execute as scheduled.',
      array(
        '!script' => SUPERCRON_SCRIPT_NAME,
        '!here'   => realpath(dirname(__FILE__)),
        '!path'   => realpath('./'),
      )
    ),
    'warning'
  );
}

function _supercron_check_requirements() {
  global $base_path;
  $supercron_script_path = variable_get('supercron_script_path', SUPERCRON_SCRIPT_PATH_MODULE_ROOT);
  if ($supercron_script_path == SUPERCRON_SCRIPT_PATH_DRUPAL_ROOT) {
    if (!file_exists(SUPERCRON_SCRIPT_NAME)) {
      _supercron_show_script_warning();
    }
  }
}

/**
 * Helper to render helper link.
 */
function _supercron_help_link($module_name, $file_name) {
  if (module_exists('advanced_help')) {
    return theme('advanced_help_topic', $module_name, $file_name);
  }
  
  $file_path  = base_path(). drupal_get_path('module', $module_name) . "/help/{$file_name}.html";
  $image_path = base_path(). drupal_get_path('module', $module_name) . "/images/help.png";
  return ' <a class="supercron-help" href="'. $file_path .'" target="_blank"><img src="'. $image_path .'" alt="[?]" /></a>';
}



function _supercron_format_seconds($value = NULL) {
  if (is_null($value)) {
      return '';
  }

  if (is_numeric($value)) {
      $value = sprintf("%01.2f", (float)$value);
  }

  return drupal_strlen($value) ? t('!value sec', array('!value' => $value)) : '';
}


/**
 * Process and stat data to be used by the settings form or the AJAX callback.
 */
function supercron_get_formatted_stats() {
  $details = array();
  $details['module_count'] = 0;
  $details['enabled_module_count'] = 0;
  $details['hooks_not_called'] = 0;
  $allmodules = supercron_fetch_all();
  foreach ($allmodules as &$module) {
    ++$details['module_count'];
    if ($module->enabled) {
      ++$details['enabled_module_count'];
    }
    $module->json_id   = 'supercron-ajax-id-' . $module->module_name;
    $module->never_ran = $module->times_count == 0;
    if ($module->never_ran && $module->enabled) {
      ++$details['hooks_not_called'];
    }
    $module->timestamp_formatted =
      $module->timestamp == 0
        ? '-'
        : format_date($module->timestamp, 'custom', 'm/d - H:i');
    $module->timestamp_formatted_json_id = $module->json_id . '-timestamp';

    $module->execution_time_formatted =
      ($module->never_ran)
        ? '-'
        : _supercron_format_seconds(
          ($module->last_exec_interval < 1) ? '< 1' : $module->last_exec_interval
        );

    $module->execution_time_formatted_json_id = $module->json_id . '-execution-time';

    $module->average_execution_time_formatted = $module->never_ran
      ? '-'
      : _supercron_format_seconds(
          ($module->total_exec_interval/$module->times_count < 1)
            ? '< 1'
            : sprintf("%01.2f", $module->total_exec_interval/$module->times_count)
        );
    $module->average_execution_time_formatted_json_id = $module->json_id . '-average-execution-time';
    $module->output_link = !empty($module->output)
      ? l(t('Output'), 'admin/settings/supercron/output/'. $module->id)
      : t('(empty)');
    $module->output_link_json_id = $module->json_id . '-output-link';

    $result = db_query("SELECT * FROM {watchdog} WHERE type = 'cron exception' ORDER BY wid DESC");
    $act = FALSE;
    while ($r = db_fetch_array($result)) {
      $variables = unserialize($r["variables"]);
      if ($variables["@module"] == $module->module_name) {
        $act = TRUE;
        break;
      }
    }

    $module->exception_link = $act ? l(t('Exceptions'), 'admin/settings/supercron/exception/'. $module->id) : t('(empty)');
    $module->exception_link_json_id = $module->json_id . '-exception-link';
  }

  $details['modules'] = $allmodules;
  if ($details['module_count'] == 0) {
    $details['lower_warning'] = '<strong>' . t('Warning') . ': ' . '</strong>'. t('There are currently no cron-supporting modules enabled on this site.');
  }
  elseif (($details['module_count'] != 0) && ($details['enabled_module_count'] == 0)) {
    $details['lower_warning'] = '<strong>' . t('Warning') . ': ' . '</strong>'. t('All of the cron-supporting modules on this site have been disabled.');
  }
  elseif ($details['enabled_module_count'] == $details['hooks_not_called']) {
    $details['lower_warning'] = '<strong>' . t('Warning') . ' : ' . '</strong>'. t('SuperCron has yet to be executed. For help with configuring your cron tasks please visit the !crontab tab above.', array('!crontab' => l(t('Crontab configuration'), SUPERCRON_ADMIN_MENU_PATH_CRONTAB_CONFIGURATION)));
  }
  else {
    $details['lower_warning'] = '';
  }

  return $details;
}

/**
 * SuperCron settings form.
 */
function supercron_settings($form_state) {
  global $base_path;
  // Check requirements
  _supercron_check_requirements();

  $form['supercron-upper-warning'] = array(
    '#type'  => 'markup',
    '#value' => '<div id="supercron-upper-warning" class="supercron-upper-warning-hidden"></div>'
  );

  $script_path = supercron_get_script_path();

  if (empty($_POST)) {
    if (empty($script_path)) {
      $message = 'You decided to place supercron.php in a custom location. Please ';
      $message .= 'copy supercron.php from supercron module directory to your ';
      $message .= 'selected location. Then edit $drupal_dir variable.';

      drupal_set_message($message, 'warning');
    }
    elseif (!file_exists(SUPERCRON_SCRIPT_NAME)) {
      _supercron_show_script_warning();
    }
    if (module_exists('elysia_cron'))
      drupal_set_message(t('You are running Elysia Cron simultaneously with SuperCron; please disable one or the other to avoid cron task conflicts.'), 'warning');

    if (module_exists('poormanscron'))
      drupal_set_message(t('You are running Poormanscron simultaneously with SuperCron; if you wish to use the features of SuperCron please use its <a href="@url">Autocron</a> features instead of Poormanscron for better compatibility.', array('@url'=>url(SUPERCRON_ADMIN_MENU_PATH_AUTOCRON))), 'warning');
  }

  $form = array();

  $form['invoke-all']=array(
    '#type' => 'item',
    '#value' => l(t('Invoke all cron jobs'), SUPERCRON_INVOKE_ALL, array('attributes' => array('class' => 'invoke'))),
    '#weight' => -1
  );

  // Update module list
  supercron_update_module_list();

  $stats = supercron_get_formatted_stats();
  $form['rows']['#tree'] = TRUE;
  foreach ($stats['modules'] as $module) {
    $form['rows'][$module->id]['json_id'] = array(
      '#type' => 'hidden',
      '#value' => $module->json_id,
    );

    $form['rows'][$module->id]['data'] = array(
      '#type' => 'value',
      '#value' => $module->module_name,
    );

    $form['rows'][$module->id]['call'] = array(
      '#type' => 'markup',
      '#value' => l(
        ' ',
        "admin/settings/supercron/call/{$module->module_name}/1",
        array('attributes' => array(
          'title' => t('Invoke'),
          'class' => 'invoke'
        ))
      )
    );

    $form['rows'][$module->id]['timestamp'] = array(
      '#type' => 'value',
      '#value' => $module->timestamp_formatted
    );

    $form['rows'][$module->id]['timestamp_json_id'] = array(
      '#type' => 'hidden',
      '#value' => $module->timestamp_formatted_json_id
    );

    $form['rows'][$module->id]['how_long_took'] = array(
      '#type'  => 'value',
      '#value' => $module->execution_time_formatted
    );

    $form['rows'][$module->id]['how_long_took_json_id'] = array(
      '#type' => 'hidden',
      '#value' => $module->execution_time_formatted_json_id
    );


    $form['rows'][$module->id]['how_long_took_avg'] = array(
      '#type' => 'value',
      '#value' => $module->average_execution_time_formatted
    );

    $form['rows'][$module->id]['how_long_took_avg_json_id'] = array(
      '#type' => 'hidden',
      '#value' => $module->average_execution_time_formatted_json_id
    );

    $form['rows'][$module->id]['output_link'] = array(
      '#type' => 'markup',
      '#value' => $module->output_link
    );

    $form['rows'][$module->id]['output_link_json_id'] = array(
      '#type' => 'hidden',
      '#value' => $module->output_link_json_id
    );

    $form['rows'][$module->id]['exception_link']=array(
      '#type' => 'markup',
      '#value' => $module->exception_link,
    );

    $form['rows'][$module->id]['exception_link_json_id'] = array(
      '#type' => 'hidden',
      '#value' => $module->exception_link_json_id
    );

    $form['rows'][$module->id]['enabled'] = array(
      '#type' => 'checkbox',
      '#default_value' => $module->enabled,
    );

    $form['rows'][$module->id]['detached']=array(
      '#type' => 'checkbox',
      '#default_value' => $module->detached,
    );

    $form['rows'][$module->id]['weight']=array(
      '#type' => 'textfield',
      '#size' => 5,
      '#default_value' => $module->weight,
      '#attributes' => array('class' => 'weight'),
    );

    $form['rows'][$module->id]['rule']=array(
      '#type'      => 'textfield',
      '#size'      => 10,
      '#default_value' => $module->rule,
      '#attributes' => array(
        'class' => 'supercron-rule',
      )
    );
  }

  $form['supercron-lower-warning'] = array(
    '#type'      => 'markup',
    '#value' => '<div id="supercron-lower-warning">' . $stats['lower_warning'] . '</div>'
  );

  // Throttle module integration
  $form['throttle'] = array(
    '#type'      => 'checkbox',
    '#title'     => t('Do not perform cron tasks when the site is under heavy load'),
    '#description'   => module_exists('throttle') ? '' : t('This features depends on the Throttle module being installed and enabled.'),
    '#default_value' => module_exists('throttle') ? variable_get(SUPERCRON_THROTTLE_SUPPORT_VARIABLE, FALSE) : FALSE,
    '#disabled'    => !module_exists('throttle')
  );

   // AutoCron integration point 1 of 2
  $form['crashsafety'] = array(
    '#type'      => 'checkbox',
    '#title'     => t('Protect against cron daemon crashes'),
    '#description'   => t('If cron tasks have not been executed in %hours hours (for example, if the crontab service crashed on this server), simulate a cron call with the next site visit.', array('%hours' => SUPERCRON_AUTOCRON_AUTOENABLED_TRESHOLD_DEFAULT_HOURS)),
    '#default_value' => variable_get(SUPERCRON_AUTOCRON_AUTOENABLED, TRUE),
      );
    


  // Option for user to clear cron-successfully logs
  $dblog_exists = module_exists('dblog');
  $form['clear-log'] = array(
    '#type'      => 'checkbox',
    '#title'     => t('Clear SuperCron log before every call'),
    '#description'   => t('This feature can only work when the dblog module is enabled.'),
    '#default_value' => $dblog_exists ? variable_get(SUPERCRON_CLEAR_LOGS, FALSE) : TRUE,
    '#disabled'      => !$dblog_exists,
  );

  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Save changes'),
  );

  // Rule toolbox
  module_load_include('inc', 'supercron', 'supercron.admin.rule');
  $form['#prefix'] = drupal_get_form('supercron_settings_rule_toolbox');

  return $form;
}

function supercron_administration_get_results() {
  drupal_json(supercron_get_formatted_stats());
}

/**
 * Theming function for supercron settings form.
 */
function _supercron_theme_make_span($id, $content, $extras = '') {
  $value = '<span id="' . $id . '"';
  if ($extras) {
    $value .= " {$extras}";
  }

  return $value . '>' . htmlentities($content) . '</span>';
}

function theme_supercron_settings($form) {
  if (user_access(SUPERCRON_EXCEPTION_FEED_PERMISSION)) {
    drupal_add_feed(url(supercron_rss_url()), t('SuperCron Exceptions'));
  }

  drupal_add_js("var path_to_supercron_status_callback = '" .  url(SUPERCRON_AJAX_CALLBACK) . "';", 'inline');
  $module_path = drupal_get_path('module', 'supercron');
  drupal_add_js("{$module_path}/js/supercron-auto-update.js");
  drupal_add_css("{$module_path}/supercron.css");
  drupal_add_js("{$module_path}/js/supercron.admin.js");

  $header[] = '';
  $header[] = '';
  $header[] = t('Name');
  $header[] = t('Last called');
  $header[] = t('Execution time') . '<br/>' . t('(Last call)');
  $header[] = t('Execution time') . '<br/>' . t('(Average)');
  $header[] = t('Output');
  $header[] = t('Exceptions');
  $header[] = t('Enabled');
  $header[] = t('Multithread') . _supercron_help_link('supercron', 'multithread');
  $header[] = t('Weight');
  $header[] = t('Rule') . _supercron_help_link('supercron', 'rule');

  foreach (element_children($form['rows']) as $id) {
    $module_fields = &$form['rows'][$id];

    $id_prefix = $module_fields['json_id']['#value'] . '-';

    $row = array();
    $row[] = '';
    $row[] = drupal_render($module_fields['call']);
    $row[] = check_plain($module_fields['data']['#value']);
    $row[] = _supercron_theme_make_span($module_fields['timestamp_json_id']['#value'], check_plain($module_fields['timestamp']['#value']));
    $row[] = _supercron_theme_make_span($module_fields['how_long_took_json_id']['#value'], $module_fields['how_long_took']['#value']);
    $row[] = _supercron_theme_make_span($module_fields['how_long_took_avg_json_id']['#value'], $module_fields['how_long_took_avg']['#value']);
    $row[] = _supercron_theme_make_span($module_fields['output_link_json_id']['#value'], drupal_render($module_fields['output_link']));
    $row[] = _supercron_theme_make_span($module_fields['exception_link_json_id']['#value'], drupal_render($module_fields['exception_link']));
    $row[] = drupal_render($module_fields['enabled']);
    $row[] = drupal_render($module_fields['detached']);
    $row[] = drupal_render($module_fields['weight']);

    $row[] = drupal_render($module_fields['rule']);

    $rows[] = array('data' => $row, 'class' => 'draggable');
    unset($form['rows'][$id]);
  }

  drupal_add_tabledrag('supercron-table', 'order', 'sibling', 'weight');

  $output  = '';
  $output .= drupal_render($form['invoke-all']);
  $output .= theme('table', $header, $rows, array('id' => 'supercron-table'));
  $output .= drupal_render($form);

  return $output;
}

function supercron_settings_validate($form, &$form_state) {
  $v = &$form_state['values'];

  // Valid Rules
  foreach (array_keys($v['rows']) as $i) {
    $rule = $v['rows'][$i]['rule'];
    if (!empty($rule) && !supercron_rule_is_valid($rule)) {
      form_error(
        $form['rows'][$i]['rule'],
        t('<strong>@rule</strong> is an invalid rule', array('@rule' => $rule))
      );
    }
  }
}

/**
 * Submit handler for supercron settings form.
 */
function supercron_settings_submit($form, &$form_state) {
  $v = &$form_state['values'];
  $weights = array();

  foreach (array_keys($v['rows']) as $module_id) {
    $row = $v['rows'][$module_id];
    
    $record = new stdClass();
    $record->id       = $module_id;
    $record->weight   = $row['weight'];
    $record->enabled  = $row['enabled'];
    $record->detached = $row['detached'];
    $record->rule     = $row['rule'];
    drupal_write_record('supercron_enabled_mods', $record, 'id');
  }

  variable_set(SUPERCRON_THROTTLE_SUPPORT_VARIABLE, $form_state['values']['throttle']);
  variable_set(SUPERCRON_AUTOCRON_AUTOENABLED, $form_state['values']['crashsafety']);

  if ($form_state['values'][SUPERCRON_SCRIPT_PATH] == SUPERCRON_SCRIPT_PATH_CUSTOM) {
    variable_set(SUPERCRON_SCRIPT_PATH, $form_state['values'][SUPERCRON_SCRIPT_PATH . '_text']);
  }
  else {
    variable_set(SUPERCRON_SCRIPT_PATH, $form_state['values'][SUPERCRON_SCRIPT_PATH]);
  }
  
}

function supercron_exception($id) {
  $module = db_result(db_query_range(
    "SELECT module_name FROM {supercron_enabled_mods} WHERE id = %d",
    $id, 0, 1
  ));

  drupal_set_title(drupal_get_title() . $module);

  if (module_exists('dblog')) {
    $result = db_query("SELECT * FROM {watchdog} WHERE type = 'cron exception' ORDER BY wid DESC");
    $exceptions = array();
    while ($row = db_fetch_array($result)) {
      $variables = unserialize($row["variables"]);
      if ($variables["@module"] == $module) {
        $exceptions[$row["wid"]]= array(
          'message'   => $variables["@message"],
          'location'  => $row["location"],
          'timestamp' => $row["timestamp"]
        );
      }
    }
  }

  $header = array(t('Log ID'), t('Message'), t('Location link'), t('Created'));
  $rows = array();

  if (empty($exceptions)) {
    $message = 'No catched cron exceptions in !module module';
    $message = t($message, array('!module' => $module));
    $rows[] = array(array('data' => $message, 'colspan' => 4));
  }
  else {
    foreach ($exceptions as $wid => $exception) {
      $rows[] = array(
        check_plain($wid),
        check_plain($exception["message"]),
        check_plain($exception["location"]),
        check_plain(strftime("%Y-%m-%d %H:%M:%S", $exception["timestamp"])),
      );
    }
  }

  return theme('table', $header, $rows);
}

function supercron_output($id) {
  $module = db_fetch_array(db_query_range(
    "SELECT * FROM {supercron_enabled_mods} WHERE id = %d",
    (int)$id, 0, 1
  ));

  if ($module) {
    // Update page title
    drupal_set_title(drupal_get_title() . $module['module_name']);

    return $module['output'];
  }

  return '';
}
