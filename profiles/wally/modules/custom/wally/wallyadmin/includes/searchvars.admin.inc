<?php

/**
 * HTML code for the search vars admin page.
 */
function search_vars_admin() {
  $return = '';
  $return .= drupal_get_form('search_vars_admin_form');
  
  return $return;
}

/**
 * Form for the search vars admin page.
 */
function search_vars_admin_form(&$form_state) {
    $form['search_vars'] = array(
      '#title' => t('Type the text to search'),
      '#type' => 'textfield',
    );
    
    $form['search_submit'] = array(
      '#type' => 'submit',
      '#value' => 'Submit'
    ); 
    
return $form;
}

function search_vars_admin_form_submit(&$form, &$form_state) {
  $form['#redirect'] = 'admin/wally/searchvars/key/'.$form_state['values']['search_vars'];
}

/**
 * HTML code for the selection of variables to export.
 */
 
function export_vars_admin($key) {
  $return = '';
  $return .= drupal_get_form('export_vars_admin_form', $key);
  
  return $return;
}

/**
 * Form for the selected variables admin page.
 */
function export_vars_admin_form(&$form_state, $key) {
  $list = db_query('SELECT name FROM {variable} WHERE `name` LIKE "%%%s%%" ORDER BY `name`', $key);
  while ($elem = db_fetch_array($list)) {
    $form['blocks_vars'][$elem['name']] = array(
      '#type' => 'checkbox',
      '#title' => $elem['name'],
      '#default_value' => 0,
    );
  }
    
  if(!isset($form['blocks_vars'])){
    drupal_set_message('No result for '.$key);
  }else{
    $form['submit'] = array(
      '#type' => 'submit',
      '#title' => t('Export selected variables'),
      '#value' => t('Export selected variables'),
    );
  }
  $form['#prefix'] =  $form_state['storage']['output'];
  
return $form;
}

/**
 * Validation for form for the delete selected cache admin page.
 */
function export_vars_admin_form_validate($form, &$form_state) {
  foreach ($form['blocks_vars'] as $name => $value) {
    if (substr($name, 0, 1) != '#' && $value['#value'] == 1)
      return;
  }
  form_set_error('blocks_vars', t('Select at least one block.'));
}

/**
 * Submit actions for form for the delete selected cache admin page.
 */
function export_vars_admin_form_submit(&$form, &$form_state) {
  $code = '';
  foreach ($form['blocks_vars'] as $name => $value) {
    if (substr($name, 0, 1) != '#' && $value['#value'] == 1) {
      $code .= $name.' '.serialize(variable_get($name, NULL)).PHP_EOL;
    }
  }
  $toExport = array(
    '#title' => t('Exported'),
    '#type' => 'textarea',
    '#id' => 'export-textarea',
    '#name' => 'export-textarea',
    '#rows' => 30,
    '#value' => $code,
    '#parents' => array('dummy'),
    '#required' => FALSE,
  );
  
  $form_state['storage']['output'] = theme('textarea', $toExport);
}

function import_vars_admin(){
  $return = '';
  $return = drupal_get_form('import_vars_admin_form');
  
  return $return;
}

/**
 * Form for the search cache admin page.
 */
function import_vars_admin_form(&$form_state) {
  $form['import_vars'] = array(
    '#title' => t('Import'),
    '#type' => 'textarea',
    '#id' => 'export-textarea',
    '#name' => 'export-textarea',
    '#rows' => 30,
    '#value' => $code,
    '#parents' => array('dummy'),
    '#required' => FALSE,
  );
  $form['import_submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit'
  ); 
  
  return $form;
}

/**
 * Submit actions for form for the delete selected cache admin page.
 */
function import_vars_admin_form_submit(&$form, &$form_state) {
  $code = explode(PHP_EOL, $form['#post']['export-textarea']);
  foreach($code as $elem){
    if(!empty($elem)){
      $exp_elem = explode(' ', $elem, 2);
      variable_set($exp_elem[0], unserialize($exp_elem[1]));
      drupal_set_message('Variable "'.$exp_elem[0].'" imported.');
    }
  }
}
