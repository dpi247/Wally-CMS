<?php
// $Id: pub.admin.inc,v 1.0 2011/02/10 ODM $
 
function pub_page_admin() {
  $return = '';
  $return .= drupal_get_form('pub_page_admin_form');
  
  return $return;
}

/**
 * Settings form.
 */
function pub_page_admin_form($form_state) {  
  $form['website'] = array(
    '#type' => 'textfield',
    '#title' => t('Website name'),
    '#description' => t('The website name used to the pub requests.'),
    '#default_value' => variable_get('pub_website', ''),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  
  $form['pub_submit'] = array(
    '#type' => 'submit',
    '#title' => t('Save'),
    '#value' => t('Save'),
  );
  
  return $form;
}

/**
 * Submit function for the settings form.
 */
function pub_page_admin_form_submit($form, &$form_state) {
  variable_set('pub_website', $form['website']['#value']);
  drupal_set_message('Configuration saved', 'notice');
}

/**
 * Executes the pub cron run when called.
 * 
 * @return
 *   Returns FALSE in case of error, redirect if successful.
 */
function pub_cron_run() {
  // Try to allocate enough time to run all the hook_cronrsstonode implementations.
  if (function_exists('set_time_limit')) {
    @set_time_limit(60);
  }

  // Fetch the cron semaphore
  $semaphore = variable_get('cronpub_semaphore', FALSE);

  if ($semaphore) {
    if (time() - $semaphore > 3600) {
      // Either cron has been running for more than an hour or the semaphore
      // was not reset due to a database error.
      watchdog('Cron pub', 'Cron has been running for more than an hour and is most likely stuck.', array(), WATCHDOG_ERROR);

      // Release cron semaphore
      variable_del('cronpub_semaphore');
    } else {
      // Cron is still running normally.
      watchdog('Cron pub', 'Attempting to re-run cron while it is already running.', array(), WATCHDOG_WARNING);
    }

    // Return FALSE and set error message;
    drupal_set_message('The cron didn\'t run successfully because the semaphore wasn\'t free.', "error");
    return FALSE;
  } else {
    // Register shutdown callback
    register_shutdown_function('pub_cron_cleanup');

    // Lock cron semaphore
    variable_set('cronrsstonode_semaphore', time());

    // Call the function calling the RSS feeds
    $return = _pub_callalltags();

    // Record cron time
    variable_set('cronpub_last', time());
    watchdog('Cron pub', 'Cron run completed.', array(), WATCHDOG_NOTICE);

    // Release cron semaphore
    variable_del('cronpub_semaphore');

    // Redirect to admin page if success
    drupal_goto('admin/rossel');
  }
}

/**
 * Shutdown function for pub cron cleanup.
 */
function pub_cron_cleanup() {
  // See if the semaphore is still locked.
  if (variable_get('cronpub_semaphore', FALSE)) {
    watchdog('Cron pub', 'Cron run exceeded the time limit and was aborted.', array(), WATCHDOG_WARNING);

    // Release cron semaphore
    variable_del('cronpub_semaphore');
  }
}

function pub_blocks_admin() {
  $return = '';
  $return .= drupal_get_form('pub_blocks_admin_form');
  
  return $return;
}

function pub_blocks_admin_form(&$form_state) {
  $form['action_choice'] = array(
    '#type' => 'radios',
    '#title' => t('Action'),
    '#options' => array(
      'edit' => t('Edit'),
      'delete' => t('Delete')
    ),
    '#required' => TRUE,
  );
  
  $form['blocks'] = array(
    '#title' => t('Choose blocks'),
    '#type' => 'radios',
    '#options' => array(),
  );
  
  $pub_list = db_query('SELECT * FROM `zz_pub_UMI` ORDER BY `position`');
  while ($pub_elem = db_fetch_array($pub_list)) {
    $form['blocks'][$pub_elem['position']] = array(
      '#type' => 'checkbox',
      '#title' => $pub_elem['position'],
      '#default_value' => 0,
    );
  }
  
  $form['blocks_submit'] = array(
    '#type' => 'submit',
    '#title' => t('Go'),
    '#value' => t('Go'),
  );
  
  return $form;
}

function pub_blocks_admin_form_validate($form, &$form_state) {
  foreach ($form['blocks'] as $name => $value) {
    if (substr($name, 0, 1) != '#' && $value['#value'] == 1)
      return;
  }
  form_set_error('blocks', t('Select at least one block.'));
}

function pub_blocks_admin_form_submit($form, &$form_state) {
  switch ($form['action_choice']['#value']) {
    case 'edit':
      foreach ($form['blocks'] as $name => $value) {
        if ($value['#value'] == 1) {
          drupal_goto('admin/build/block/configure/pub/'.$name);
        }
      }
      break;
    case 'delete':
      foreach ($form['blocks'] as $name => $value) {
        if (substr($name, 0, 1) != '#' && $value['#value'] == 1) {
          db_query('DELETE FROM `zz_pub_UMI` WHERE `position` = "%s"', $name);
        }
      }
      break;
    default:
  }
}

function pub_cache_admin() {
  $return = '';
  $return .= drupal_get_form('pub_cache_admin_form');
  
  return $return;
}

function pub_cache_admin_form(&$form_state) {
  $form['blocks_cache'] = array(
    '#title' => t('Choose blocks'),
    '#type' => 'radios',
    '#options' => array(),
  );
  
  $pub_list = db_query('SELECT * FROM `cache` WHERE `cid` LIKE "pub_%" ORDER BY `cid`');
  while ($pub_elem = db_fetch_array($pub_list)) {
    $form['blocks_cache'][$pub_elem['cid']] = array(
      '#type' => 'checkbox',
      '#title' => $pub_elem['cid'],
      '#default_value' => 0,
    );
  }
  
  $form['blocks_submit'] = array(
    '#type' => 'submit',
    '#title' => t('Go'),
    '#value' => t('Go'),
  );
  
  return $form;
}

function pub_cache_admin_form_validate($form, &$form_state) {
  foreach ($form['blocks_cache'] as $name => $value) {
    if (substr($name, 0, 1) != '#' && $value['#value'] == 1)
      return;
  }
  form_set_error('blocks_cache', t('Select at least one block.'));
}

function pub_cache_admin_form_submit($form, &$form_state) {
  foreach ($form['blocks_cache'] as $name => $value) {
    if (substr($name, 0, 1) != '#' && $value['#value'] == 1) {
      db_query('DELETE FROM `cache` WHERE `cid` = "%s"', $name);
    }
  }
}

function _pub_callalltags() {
  $xml_list = drupal_http_request('http://gps1.intranet.rossel/private/script/ZZsqls.cgi?key=dumpTagsAdtech&format=xml');
  $dom = new DomDocument();
  $dom->LoadXML($xml_list->data);
  
  $pub_list = db_query('SELECT * FROM `zz_pub_UMI`');
  $pub_array = array();
  while ($pub_elem = db_fetch_array($pub_list)) {
    $pub_array[] = $pub_elem;
  }
  
  foreach($dom->getElementsByTagName('row') as $row) {
    $tag_elem = array();
    foreach($row->attributes as $attrName => $attrValue) {
      $tag_elem[$attrName] = $attrValue->value;
    }
    unset($tag_elem['website']);
    foreach($pub_array as $pub_elem) {
      if ($tag_elem['position'] == $pub_elem['position']) {
        drupal_write_record('zz_pub_UMI', $tag_elem, array('position'));
        break;
      }
    }
    drupal_write_record('zz_pub_UMI', $tag_elem);
  }
  
  return TRUE;
}
