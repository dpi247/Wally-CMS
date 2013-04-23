<?php

/**
 * Form for the variables export page.
 */
function export_vars_admin_form(&$form_state) {
  $form = array();
  
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

function export_vars_admin_form_submit(&$form, &$form_state) {
  $form['#redirect'] = 'admin/wally/searchvars/export/confirm/'.$form_state['values']['search_vars'];
}

/**
 * Choose which variables to export amongst the matches
 */
function export_vars_confirm_form($form_state, $key) {
  $form = array(
    '#method' => 'POST',
    '#attributes' => array(
      'enctype' => 'multipart/form-data',
    ),
  );
  
  $system_db_vars = db_query('SELECT name FROM {variable} WHERE name LIKE "%%%s%%" ORDER BY name', $key);
  $system_vars = array();
  while ($system_db_var = db_fetch_array($system_db_vars)) {
    $system_vars[$system_db_var['name']] = $system_db_var['name'];
  }
  
  if (!empty($system_vars)) {
    $form['system_vars'] = array(
      '#type' => 'checkboxes',
      '#title' => t('System variables'),
      '#options' => $system_vars,
      '#default_value' => $system_vars,
    );
  }
  
  $wally_db_vars = db_query('SELECT name FROM {wally_variable_fast} WHERE name LIKE "%%%s%%" ORDER BY name', $key);
  $wally_vars = array();
  while ($wally_db_var = db_fetch_array($wally_db_vars)) {
    $wally_vars[$wally_db_var['name']] = $wally_db_var['name'];
  }
  
  if (!empty($wally_vars)) {
    $form['wally_vars'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Wally fast variables'),
      '#options' => $wally_vars,
      '#default_value' => $wally_vars,
    );
  }

  if (empty($form)) {
    drupal_set_message(t('No result for @key', array('@key' => $key)));
  } else {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Export selected variables'),
    );
  }
  
  return $form;
}

function export_vars_confirm_form_validate($form, &$form_state) {
  $empty = TRUE;
  $values = $form_state['values'];
  $vars = array_merge(isset($values['system_vars']) ? $values['system_vars'] : array(), isset($values['wally_vars']) ? $values['wally_vars'] : array());
  foreach ($vars as $key => $value) {
    if ($key == $value) {
      $empty = FALSE;
    }
  }
  if ($empty) {
    form_set_error('', t('Select at least one variable to export.'));
  }
}

function export_vars_confirm_form_submit(&$form, &$form_state) {
  $dom = _wallyadmin_export_generate_dom($form_state['values']);

  $site_name = str_replace(' ', '-', strtolower(variable_get('site_name', '')));
  $date = date('Y-m-d');
  $filename = 'vars-export-'.$site_name.'-'.$date;
  $files = array(
    array(
      'name' => $filename,
      'extension' => '.xml',
      'content' => $dom->saveXML(),
    ),
    array(
      'name' => '',
      'extension' => '.xml',
      'content' => '',
    ),
  );

  module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
  wallytoolbox_export_files_to_tar($files, $filename.'.tar');
}

/**
 * Form for the variables import page.
 */
function import_vars_admin_form(&$form_state) {
  $form = array(
    '#method' => 'POST',
    '#attributes' => array(
      'enctype' => 'multipart/form-data',
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
    '#prefix' => t('Choose a tar file to import:').' <input type="file" name="uploadedfile" /><br/>',
  );
  
  return $form;
}

function import_vars_admin_form_validate($form, $form_state) {
  if (!isset($_FILES['uploadedfile']) || empty($_FILES['uploadedfile']) || $_FILES['uploadedfile']['error'] > 0) {
    form_set_error('', t('No files to import'));
  }
}

function import_vars_admin_form_submit($form, $form_state) {
  $post_file = $_FILES['uploadedfile'];

  if (file_exists($post_file['tmp_name'])) {
    $filename = $post_file['name'];
    $tmp_path = $post_file['tmp_name'];
    module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
    $return = wallytoolbox_tar_extract_dom($filename, $tmp_path);
    if ($return['error']) {
      drupal_set_message($return['error_msg'], 'error');
    } else {
      _wallyadmin_import_import_from_dom($return['content']);
      drupal_set_message(t('Import done'));
    }
  }
}

function _wallyadmin_export_generate_dom($values) {
  $dom = new DomDocument('1.0', 'UTF-8');
  $vars_export = $dom->appendChild($dom->createElement('VarsExport'));
  $dom_system_vars = $vars_export->appendChild($dom->createElement('SystemVars'));
  $dom_wally_vars = $vars_export->appendChild($dom->createElement('WallyVars'));

  if (isset($values['system_vars']) && is_array($values['system_vars'])) {
    foreach ($values['system_vars'] as $key => $value) {
      if ($key == $value) {
        $system_var = variable_get($key, 0);
        $dom_system_var = $dom_system_vars->appendChild($dom->createElement('Variable'));
        $dom_system_var->appendChild($dom->createCDATASection(serialize($system_var)));
        $dom_system_var->setAttribute('name', $key);
      }
    }
  }

  if (isset($values['wally_vars']) && is_array($values['wally_vars'])) {
    foreach ($values['wally_vars'] as $key => $value) {
      if ($key == $value) {
        $wally_var = wally_variable_get($key, 0);
        $dom_wally_var = $dom_wally_vars->appendChild($dom->createElement('Variable'));
        $dom_wally_var->appendChild($dom->createCDATASection(serialize($wally_var)));
        $dom_wally_var->setAttribute('name', $key);
      }
    }
  }

  return $dom;
}

function _wallyadmin_import_import_from_dom($dom) {
  $dom_system_vars = $dom->getElementsByTagName('SystemVars');
  if ($dom_system_vars->item(0)) {
    $dom_system_var = $dom_system_vars->item(0)->getElementsByTagName('Variable');
    if ($dom_system_var->item(0)) {
      foreach ($dom_system_var as $dom_var) {
        $key = $dom_var->getAttributeNode('name')->value;
        $value = unserialize($dom_var->nodeValue);
        variable_set($key, $value);
      }
    }
  }

  $dom_wally_vars = $dom->getElementsByTagName('WallyVars');
  if ($dom_wally_vars->item(0)) {
    $dom_wally_var = $dom_wally_vars->item(0)->getElementsByTagName('Variable');
    if ($dom_wally_var->item(0)) {
      foreach ($dom_wally_var as $dom_var) {
        $key = $dom_var->getAttributeNode('name')->value;
        $value = unserialize($dom_var->nodeValue);
        wally_variable_set($key, $value);
      }
    }
  }
}
