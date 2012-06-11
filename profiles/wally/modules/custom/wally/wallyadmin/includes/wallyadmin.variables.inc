<?php 

/**
 * Returns a persistent variable.
 *
 * Case-sensitivity of the _wally_variable_* functions depends on the database
 * collation used. To avoid problems, always use lower case for persistent
 * variable names.
 *
 * @param $name
 *   The name of the variable to return.
 * @param $default
 *   The default value to use if this variable has never been set.
 * @return
 *   The value of the variable.
 *
 * @see _wally_variable_del(), _wally_variable_set(), _wally_variable_get_all()
 */
function _wally_variable_get($name, $default, $regenerate = FALSE) {
  global $wally_vars;

  if (!isset($wally_vars[$name]) || $regenerate) {
    $result = db_query('SELECT value FROM {wally_variable_fast} WHERE name = "%s"', $name);
    if($variable = db_fetch_array($result)) {
      $wally_vars[$name] = unserialize($variable['value']);
    }
  }

  return isset($wally_vars[$name]) ? $wally_vars[$name] : $default;
}

/**
 * Returns all persistents variables.
 *
 * @return
 *   The value of the variables.
 *
 * @see _wally_variable_get(), _wally_variable_del(), _wally_variable_set()
 */
function _wally_variable_get_all() {
  global $wally_vars;

  if (!isset($wally_vars[$name]) || $regenerate) {
    $result = db_query('SELECT * FROM {wally_variable_fast}');
    while($variable = db_fetch_array($result)) {
      $wally_vars[$name] = unserialize($variable['value']);
    }
  }

  return isset($wally_vars[$name]) ? $wally_vars[$name] : $default;
}

/**
 * Sets a persistent variable.
 *
 * Case-sensitivity of the _wally_variable_* functions depends on the database
 * collation used. To avoid problems, always use lower case for persistent
 * variable names.
 *
 * @param $name
 *   The name of the variable to set.
 * @param $value
 *   The value to set. This can be any PHP data type; these functions take care
 *   of serialization as necessary.
 *
 * @see _wally_variable_del(), _wally_variable_get(), _wally_variable_get_all()
 */
function _wally_variable_set($name, $value) {
  global $wally_vars;

  $serialized_value = serialize($value);
  db_query("UPDATE {wally_variable_fast} SET value = '%s' WHERE name = '%s'", $serialized_value, $name);
  if (!db_affected_rows()) {
    @db_query("INSERT INTO {wally_variable_fast} (name, value) VALUES ('%s', '%s')", $name, $serialized_value);
  }

  $wally_vars[$name] = $value;
}

/**
 * Unsets a persistent variable.
 *
 * Case-sensitivity of the _wally_variable_* functions depends on the database
 * collation used. To avoid problems, always use lower case for persistent
 * variable names.
 *
 * @param $name
 *   The name of the variable to undefine.
 *
 * @see _wally_variable_get(), _wally_variable_set(), _wally_variable_get_all()
 */
function _wally_variable_del($name) {
  global $wally_vars;

  db_query("DELETE FROM {wally_variable_fast} WHERE name = '%s'", $name);

  unset($wally_vars[$name]);
}

/**
 * Add default buttons to a form and set its prefix.
 * 
 * The only difference with system_settings_form is that the cache is not flushed and
 * the theme not rebuild on submit.
 *
 * @ingroup forms
 * @see wally_system_settings_form_submit(), system_settings_form()
 * @param $form
 *   An associative array containing the structure of the form.
 * @param $fast
 *   Use Wally's variable functions or standards
 * @return
 *   The form structure.
 */
function _wally_system_settings_form($form, $fast = FALSE) {
  $form['buttons']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));
  $form['buttons']['reset'] = array('#type' => 'submit', '#value' => t('Reset to defaults'));
  $form['fast'] = array('#type' => 'hidden', '#value' => $fast ? 1 : 0);

  if (!empty($_POST) && form_get_errors()) {
    drupal_set_message(t('The settings have not been saved because of the errors.'), 'error');
  }
  $form['#submit'][] = 'wally_system_settings_form_submit';
  $form['#theme'] = 'system_settings_form';
  return $form;
}

/**
 * Execute the system_settings_form.
 * 
 * The only difference with system_settings_form is that the cache is not flushed and
 * the theme not rebuild on submit.
 *
 * If you want node type configure style handling of your checkboxes,
 * add an array_filter value to your form.
 * 
 * @see wally_system_settings_form(), system_settings_form_submit()
 */
function _wally_system_settings_form_submit($form, &$form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';

  // Exclude unnecessary elements.
  unset($form_state['values']['submit'], $form_state['values']['reset'], $form_state['values']['form_id'], $form_state['values']['op'], $form_state['values']['form_token'], $form_state['values']['form_build_id']);

  foreach ($form_state['values'] as $key => $value) {
    if ($op == t('Reset to defaults')) {
      $form_state['values']['fast'] ? wally_variable_del($key) : variable_del($key);
    } else {
      if (is_array($value) && isset($form_state['values']['array_filter'])) {
        $value = array_keys(array_filter($value));
      }
      $form_state['values']['fast'] ? wally_variable_set($key, $value) : variable_set($key, $value);
    }
  }
  if ($op == t('Reset to defaults')) {
    drupal_set_message(t('The configuration options have been reset to their default values.'));
  } else {
    drupal_set_message(t('The configuration options have been saved.'));
  }
}
