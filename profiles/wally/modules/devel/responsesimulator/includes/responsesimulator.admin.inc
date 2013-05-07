<?php

/**
 * Display table containing the current HTTP responses.
 */
function responsesimulator_page_admin_http_responses_list() {
  $header = array(
    array('data' => t('Name')),
    array('data' => t('HTTP Code')),
    array('data' => t('URL')),
    array('data' => t('Operations')),
  );

  $db_results = db_query("SELECT * FROM {responsesimulator_responses}");
  $rows = array();
  while ($db_result = db_fetch_array($db_results)) {
    $db_result['settings'] = unserialize($db_result['settings']);
    $temp_row = array();
    $temp_row['name'] = check_plain($db_result['name']);
    $temp_row['httpcode'] = check_plain($db_result['httpcode']);
    $temp_row['url'] = _responsesimulator_page_admin_http_responses_get_url($db_result);
    $temp_row['operations'] = _responsesimulator_page_admin_http_responses_table_actions($db_result);
    $rows[] = $temp_row;
  }

  return theme_table($header, $rows);
}

/**
 * Add/Edit an HTTP response
 */
function responsesimulator_page_admin_http_responses_add($form_state, $rid = NULL, $remove_file = NULL) {
  $form = array(
    '#tree' => TRUE,
  );

  $defaults = _responsesimulator_page_admin_add_defaults();
  if ($rid) {
    if ($http_response = responsesimulator_get_defined_http_response($rid)) {
      $defaults = array_merge($defaults, $http_response);
      $form['rid'] = array(
        '#type' => 'hidden',
        '#value' => $defaults['rid'],
      );
    } else {
      drupal_set_message(t('No matching HTTP response.'), 'warning');
      drupal_goto('admin/settings/responsesimulator');
    }
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('Must be a machine readable name (no spaces, no capital letters, no punctuation, "-" allowed).'),
    '#default_value' => $defaults['name'],
    '#required' => TRUE,
  );

  $form['httpcode'] = array(
    '#type' => 'textfield',
    '#title' => t('HTTP Code'),
    '#description' => t('Must be a numerical value.'),
    '#default_value' => $defaults['httpcode'],
    '#required' => TRUE,
  );

  $settings = isset($defaults['settings']) ? $defaults['settings'] : array();

  $form['settings'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
  );

  // @todo Ajouter la possibilité de choisir entre un fichier ou une textarea
  /*
  ctools_include('dependent');
  ctools_add_js('dependent');
  */

  $form['settings']['file'] = array(
    '#type' => 'fieldset',
    '#title' => t('Use a file as HTTP response\'s contents'),
    '#collapsible' => FALSE,
  );

  if (!$remove_file && isset($settings['file']) && !empty($settings['file'])) {
    $form['settings']['file']['current'] = array(
      '#type' => 'markup',
      '#value' => l($settings['file']->filename, $settings['file']->filepath),
      '#prefix' => t('Current file : '),
      '#suffix' => '<br>',
    );

    $form['settings']['file']['remove'] = array(
      '#type' => 'markup',
      '#value' => ' '.l(t('Remove this file'), $_GET['q'].'/1'),
    );
  } else {
    if ($remove_file && empty($form_state['post'])) {
      drupal_set_message(t('The file will not be deleted until the form is saved.'), 'warning');
    }

    $response_files_dir = 'sites/default/files/httpresponses';
    if (!file_check_directory($response_files_dir, FILE_CREATE_DIRECTORY)) {
      drupal_set_message(t('To upload files, the directory "sites/default/files/httpresponses" must be writable.'), 'warning');
    } else {
      $form['#attributes'] = array('enctype' => 'multipart/form-data');

      $form['settings']['file']['add'] = array(
        '#type' => 'file',
        '#title' => t('Upload your file'),
        '#size' => 40,
      );
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['cancel'] = array(
    '#type' => 'markup',
    '#value' => l(t('Cancel'), 'admin/settings/responsesimulator'),
  );

  return $form;
}

function responsesimulator_page_admin_http_responses_add_validate($form, $form_state) {
  $values = $form_state['values'];

  if (!preg_match('/^[a-z0-9\-]*$/', $values['name'])) {
    form_set_error('name', t('Please enter a machine readale name (no spaces, no capital letters, no punctuation, "-" allowed).'));
  }

  if (!is_numeric($values['httpcode'])) {
    form_set_error('name', t('Please enter a numerical value.'));
  }
}

function responsesimulator_page_admin_http_responses_add_submit($form, $form_state) {
  $values = $form_state['values'];

  $values['settings']['file'] = array();
  if (isset($_FILES['files']['name']['settings']) && !empty($_FILES['files']['name']['settings'])) {
    $validators = array();
    $file = file_save_upload('settings', $validators, 'sites/default/files/httpresponses');
    file_set_status($file, FILE_STATUS_PERMANENT);
    $values['settings']['file'] = $file;
  }

  if (isset($values['rid']) && !empty($values['rid'])) {
    // Old file handling
    if ($old_http_resp = responsesimulator_get_defined_http_response($values['rid'])) {
      if (isset($old_http_resp['settings']['file']) && !empty($old_http_resp['settings']['file'])) {
        if (!isset($values['settings']['file']) || empty($values['settings']['file'])) {
          // If we have no new file but an old one, set the new with the old
          $values['settings']['file'] = $old_http_resp['settings']['file'];
        } else {
          // If we have a new file and an old one, delete the old one
          $old_file = $old_http_resp['settings']['file'];
          if ($file->fid != $old_file->fid) {
            db_query("DELETE FROM {files} WHERE fid = %d", $old_file->fid);
            file_delete($old_file->filepath);
          }
        }
      }
    }

    drupal_write_record('responsesimulator_responses', $values, array('rid'));
    drupal_set_message(t('Updated HTTP response "@name"', array('@name' => $values['name'])));
  } else {
    drupal_write_record('responsesimulator_responses', $values);
    drupal_set_message(t('Created HTTP response "@name"', array('@name' => $values['name'])));
  }

  drupal_goto('admin/settings/responsesimulator');
}

/**
 * Delete an HTTP response
 */
function responsesimulator_page_admin_http_responses_delete($form_state, $rid) {
  if ($http_response = responsesimulator_get_defined_http_response($rid)) {
    $question = t('Are you sure you want to delete this HTTP response?');

    $add_form = array();
    $add_form['rid'] = array(
      '#type' => 'hidden',
      '#value' => $rid,
    );

    $form = confirm_form($add_form, $question, 'admin/settings/responsesimulator');
    $form['#redirect'] = 'admin/settings/responsesimulator';

    return $form;
  } else {
    drupal_set_message(t('No matching HTTP response.'), 'warning');
    drupal_goto('admin/settings/responsesimulator');
  }
}

function responsesimulator_page_admin_http_responses_delete_submit($form, $form_state) {
  $values = $form_state['values'];
  $rid = $values['rid'];

  if ($http_resp = db_fetch_array(db_query('SELECT * FROM {responsesimulator_responses} WHERE rid = %d', $rid))) {
    $http_resp['settings'] = unserialize($http_resp['settings']);
    if (isset($http_resp['settings']['file']) && !empty($http_resp['settings']['file'])) {
      $file = $http_resp['settings']['file'];
      db_query("DELETE FROM {files} WHERE fid = %d", $file->fid);
      file_delete($file->filepath);
    }
    db_query('DELETE FROM {responsesimulator_responses} WHERE rid = %d', $rid);
    drupal_set_message(t('Deleted HTTP response.'));
  } else {
    drupal_set_message(t('No matching HTTP response.'), 'warning');
  }
}

/**
 * Get the URL for this HTTP response
 */
function _responsesimulator_page_admin_http_responses_get_url($http_resp) {
  if (isset($http_resp['settings']['file']) && !empty($http_resp['settings']['file'])) {
    $filepath = $http_resp['settings']['file']->filepath;
    $url = l(check_url(url($filepath, array('absolute' => TRUE))), $filepath, array('absolute' => TRUE));
  } else {
    $url = l(check_url(url('responsesimulator/'.$http_resp['name'], array('absolute' => TRUE))), 'responsesimulator/'.$http_resp['name'], array('absolute' => TRUE));
  }
  return $url;
}

/**
 * Get links to available operations
 */
function _responsesimulator_page_admin_http_responses_table_actions($resp_infos) {
  if ($resp_infos['erasable']) {
    $return = l(t('Edit'), 'admin/settings/responsesimulator/add/'.$resp_infos['rid']).' - ';
    $return .= l(t('Delete'), 'admin/settings/responsesimulator/delete/'.$resp_infos['rid']).' - ';
  } else {
    $return = '';
  }
  $return .= l(t('Test'), 'responsesimulator/'.$resp_infos['name']);

  return $return;
}

/**
 * Get default values for the add/edit form
 */
function _responsesimulator_page_admin_add_defaults() {
  return array(
    'name' => '',
    'httpcode' => 200,
    'settings' => array(
      'attachments' => array(),
    ),
  );
}
