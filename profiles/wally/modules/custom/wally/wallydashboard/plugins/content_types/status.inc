<?php
/**
* Callback function to supply a list of content types.
*/
function wallydashboard_status_ctools_content_types() {

  return array(
    'single' => TRUE,
    'title' => t('Status'),
    'category' => t('Dashboard'),
    'description' => t('Display status'),
    'defaults' => array(),
  );
}

function wallydashboard_status_content_type_edit_form(&$form, &$form_state) {
  // The current configuration
  
  
  $options = array(
    'Apache Solr' => 'Apache Solr',
    'Access to update.php' => 'Access to update.php',
    'CTools CSS Cache' => 'CTools CSS Cache',
    'Configuration file' => 'Configuration file',
    'Cron maintenance tasks' => 'Cron maintenance tasks',
    'Database updates' => 'Database updates',
    'Database updates' => 'Database updates',
    'Varnish status' => 'Varnish status',
    'Memcache admin' => 'Memcache admin',
  );
  
  $form['requirements'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Select the status you want to observe in the dashboard'),
    '#options' => $options,
  );
}

function wallydashboard_status_content_type_edit_form_submit(&$form, &$form_state) {
  // For each part of the form defined in the 'defaults' array set when you
  // defined the content type, copy the value from the form into the array
  // of items to be saved. We don't ever want to use
  // $form_state['conf'] = $form_state['values'] because values contains
  // buttons, form id and other items we don't want stored. CTools will handle
  // the actual form submission.
  foreach ($form_state['values']['requirements'] as $key => $value){
    if ($value !== 0){
      $form_state['conf']['requirements'][$key] = $key;
    }
  }
}

/**
* Output function for the 'shortcut' content type.
*/
function wallydashboard_status_content_type_render($subtype, $conf, $panel_args, $context,$a) {

  $block->title = t('Status');
  include_once './includes/install.inc';
  drupal_load_updates();
  $requirements = module_invoke_all('requirements', 'runtime');
  usort($requirements, '_system_sort_requirements');
  dsm($requirements);
  $select_requirements = array();
  foreach ($requirements as $require){
    if (array_key_exists($require['title'], $conf['requirements'])){
      $select_requirements[] = $require;
    }
  }
  $block->content = theme('wallydashboard_status', $select_requirements);


  return $block;
}

