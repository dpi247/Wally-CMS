<?php
// $Id: supercron.crontab-admin.inc,v 1.1 2010/03/18 23:02:38 63reasons Exp $

/**
 * @file
 * Contains administrative configuration helper for Cron tasks
 */

include_once(drupal_get_path('module', 'supercron') . '/supercron-main.inc');

function supercron_invocation_form($form_state) {
  global $base_path;
  $module_path = drupal_get_path('module', 'supercron');
  drupal_add_js("{$module_path}/js/ZeroClipboard.js");
  drupal_add_js("{$module_path}/js/supercron.crontab.js");
  drupal_add_js(array('supercron_module_path' => drupal_get_path('module', 'supercron')), 'setting');

  $exefound = TRUE;
  $exe = variable_get(SUPERCRON_PHP_BINARY_PATH, '');
  if (!$exe) $exe = isset($_SERVER['PHP_PEAR_PHP_BIN']) ? $_SERVER['PHP_PEAR_PHP_BIN'] : '';
  if (!$exe) $exe = isset($_SERVER['_']) ? $_SERVER['_'] : '';

  if (!$exe) {
    $exe = SUPERCRON_PHP_BINARY_PATH_PLACEHOLDER;
    $exefound = FALSE;
  }

  $supercron_script_path = variable_get('supercron_script_path', SUPERCRON_SCRIPT_PATH_MODULE_ROOT);
  $form['invocation'][SUPERCRON_SCRIPT_PATH] = array(
    '#type'      => 'radios',
    '#title'     => t('Path to your supercron script'),
    '#required'  => TRUE,
    '#default_value' => !in_array($supercron_script_path, array(
      SUPERCRON_SCRIPT_PATH_DRUPAL_ROOT,
      SUPERCRON_SCRIPT_PATH_MODULE_ROOT
    ))
    ? SUPERCRON_SCRIPT_PATH_CUSTOM
    : $supercron_script_path,
    '#options'   => array(
      SUPERCRON_SCRIPT_PATH_DRUPAL_ROOT => t('Drupal root (@path)', array('@path' => supercron_standard_path())),
      SUPERCRON_SCRIPT_PATH_MODULE_ROOT => t('Module root (@path)', array('@path' => supercron_base_path())),
      SUPERCRON_SCRIPT_PATH_CUSTOM      => t('Other place')
    ),
  );

  $form['invocation'][SUPERCRON_SCRIPT_PATH . '_text'] = array(
    '#type'      => 'textfield',
    '#default_value' => !in_array($supercron_script_path, array(
      SUPERCRON_SCRIPT_PATH_DRUPAL_ROOT,
      SUPERCRON_SCRIPT_PATH_MODULE_ROOT
    )) ? $supercron_script_path : '',
  );

  $form['invocation']['help'] = array(
    '#type'  => 'markup',
    '#value' => '<div>If you place supercron.php anywhere but in the Drupal root, make sure to edit the value of <tt>$drupal_dir</tt> in <tt>supercron.php</tt> so that it contains the path to the Drupal root directory.</div>'
  );

  $form['invocation']['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Save changes'),
  );

  $exe .= ' -q ' . supercron_get_script_path();
  $form['invocation']['general'] = array(
    '#type'  => 'textfield',
    '#title' => t('Crontab command line for SuperCron'),
    '#size'  => 100,
    '#value' => $exe,
    '#field_suffix' => l(t('Copy to clipboard'), '', array('attributes' => array(
      'class' => 'supercron-crontab-command',
      'rel' => $exe,
    ))),
  );
  $modules = supercron_list_of_cron_modules();
  $safety = _supercron_get_safety_variable();
  $markup = '';

  foreach ($modules as $module) {
    $value = $exe . '?safety=' . urlencode($safety) . '&module=' . urlencode($module);
    $form['commands'][] = array(
      '#type'  => 'textfield',
      '#title' => $module,
      '#value' => $value,
      '#field_suffix' => l(t('Copy to clipboard'), '#', array('attributes' => array(
        'class' => 'supercron-crontab-command',
        'rel' => $value,
      ))),
    );
  }

  if (!$exefound) {
    $message = t('Replace %placeholder by the path to your php interpreter executable');

    $form['replace'] = array(
      '#type'  => 'markup',
      '#value' => t($message, array('%placeholder' => SUPERCRON_PHP_BINARY_PATH_PLACEHOLDER)),
    );
  }

  return $form;
}

function supercron_invocation_form_submit($form, &$form_state) {
  if ($form_state['values'][SUPERCRON_SCRIPT_PATH] == SUPERCRON_SCRIPT_PATH_CUSTOM) {
    variable_set(SUPERCRON_SCRIPT_PATH, $form_state['values'][SUPERCRON_SCRIPT_PATH . '_text']);
  }
  else {
    variable_set(SUPERCRON_SCRIPT_PATH, $form_state['values'][SUPERCRON_SCRIPT_PATH]);
  }
}


/**
 * Theming function for invocation form.
 */
function theme_supercron_invocation_form($form) {
  $output = '';

  $output .= drupal_render($form[SUPERCRON_SCRIPT_PATH]);
  $output .= drupal_render($form[SUPERCRON_SCRIPT_PATH . '_text']);
  $output .= drupal_render($form['invocation']);

  if (!empty($form['commands'])) {
    $header[] = t('Module');
    $header[] = t('Command line');
    $rows = array();

    foreach (element_children($form['commands']) as $key) {
      $command = &$form['commands'][$key];
      $module  = $command['#title'];
      $command['#title'] = '';
      $rows[] = array($module, drupal_render($command));
    }

    $caption  = 'Expert users who do not wish to use multitasking can use';
    $caption  .= ' these command lines to call individual cron hooks one at a time.';
    $caption  = t($caption);

    $output .= theme('table', $header, $rows, array(), $caption);
  }

  return $output . drupal_render($form);
}
