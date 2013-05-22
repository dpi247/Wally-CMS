<?php
// $Id: pub.admin.inc,v 1.0 2011/02/10 ODM $

/**
 * Settings form.
 */
function pub_page_admin_form($form_state) {
  $form = array();

  $form['pub_website'] = array(
    '#type' => 'textfield',
    '#title' => t('Website name'),
    '#description' => t('The website name used to the ads requests.'),
    '#default_value' => variable_get('pub_website', ''),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  
  $form['pub_provider'] = array(
    '#type' => 'textfield',
    '#title' => t('Ads provider'),
    '#description' => t('The ads provider (Examples : AdTech, AOS).'),
    '#default_value' => variable_get('pub_provider', 'AdTech'),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  
  $form['pub_get_xml'] = array(
    '#type' => 'textfield',
    '#title' => t('URL used for getting blocks informations'),
    '#default_value' => variable_get('pub_get_xml', 'http://gps1.intranet.rossel/private/script/ZZsqls.cgi?key=dumpTagsAdtech&format=xml'),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  
  $form['pub_webserv'] = array(
    '#type' => 'textfield',
    '#title' => t('URL of the webservice returning the ads'),
    '#default_value' => variable_get('pub_webserv', 'http://172.21.132.180:8080/Wally-Tags/rest/wally/script/detail'),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  
  $form['pub_header'] = array(
    '#type' => 'textfield',
    '#title' => t('URL of the webservice returning the header javascript content'),
    '#default_value' => variable_get('pub_header', ''),
    '#size' => 80,
    '#maxlength' => 512,
  );
  
  return system_settings_form($form);
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
    register_shutdown_function('wallytoolbox_cron_cleanup', 'cronpub_semaphore', 'Pub', t('Pub cron run exceeded the time limit and was aborted'), array(), FALSE);

    // Lock cron semaphore
    variable_set('cronpub_semaphore', time());

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
 * Form for the pub blocks admin page.
 */
function pub_blocks_admin_form(&$form_state) {
  /*$form['action_choice'] = array(
    '#type' => 'radios',
    '#title' => t('Action'),
    '#options' => array(
      'edit' => t('Edit'),
      'delete' => t('Delete')
    ),
    '#required' => TRUE,
  );*/
  
  $form['instruction'] = array(
    '#title' => t('Click on a block name to edit it, check one (or more) box and click "Delete" button to delete them'),
    '#type' => 'radios',
    '#options' => array(),
  );
  
  $form['blocks'] = array(
    '#title' => t('Choose blocks'),
    '#type' => 'radios',
    '#options' => array(),
  );
  
  $pub_list = db_query('SELECT * FROM {zz_pub_UMI} ORDER BY position');
  while ($pub_elem = db_fetch_array($pub_list)) {
    $form['blocks'][$pub_elem['position']] = array(
      '#type' => 'checkbox',
      '#title' => '<a href="/admin/build/block/configure/pub/'.$pub_elem['position'].'?destination=admin/rossel/pubblocks" title="'.t("Edit this block").'">'.$pub_elem['position'].'</a>',
      '#default_value' => 0,
    );
  }
  
  $form['blocks_submit'] = array(
    '#type' => 'submit',
    '#title' => t('Delete'),
    '#value' => t('Delete'),
    '#name'=>'delete',
  );
  
  $form['new_block'] = array(
    '#title' => t('Create a new block'),
    '#type' => 'radios',
    '#options' => array(),
  );

  $options_builder = array(
    'adtech' => 'AdTech',
    'purl' => 'Purl',
    'openx' => 'OpenX',
  );
  $form['new_block']['builder'] = array(
    '#type' => 'select',
    '#title' => t('Builder'),
    '#default_value' => 'adtech',
    '#options' => $options_builder,
    '#required' => TRUE,
  );
  
  $form['new_block']['page'] = array(
    '#type' => 'textfield',
    '#title' => t('Page'),
    '#default_value' => '',
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => FALSE,
  );
  
  $form['new_block']['position'] = array(
    '#type' => 'textfield',
    '#title' => t('Position'),
    '#default_value' => '',
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => FALSE,
  );
  
  $form['new_block']['_PlacementID'] = array(
    '#type' => 'textfield',
    '#title' => t('Placement ID'),
    '#default_value' => '',
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => FALSE,
  );
  
  $form['new_block']['_SizeID'] = array(
    '#type' => 'textfield',
    '#title' => t('Size ID'),
    '#default_value' => '',
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => FALSE,
  );

  $form['new_block']['kvregion_ls'] = array(
    '#type' => 'textfield',
    '#title' => t('kvregion_ls'),
    '#default_value' => '',
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => FALSE,
  );
  
  $form['new_block']['kvregion_sp'] = array(
    '#type' => 'textfield',
    '#title' => t('kvregion_sp'),
    '#default_value' => '',
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => FALSE,
  );
  
  $form['new_block']['kvville_sp'] = array(
    '#type' => 'textfield',
    '#title' => t('kvville_sp'),
    '#default_value' => '',
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => FALSE,
  );
  
  $form['new_block_submit'] = array(
    '#type' => 'submit',
    '#title' => t('Create new block'),
    '#value' => t('Create new block'),
  	'#name'=>'create',
  );
  
  return $form;
}

/**
 * Validation for the form for the pub blocks admin page.
 */
function pub_blocks_admin_form_validate($form, &$form_state) {
  $op=$form_state['clicked_button']['#name'];
  switch ($op) {
    case 'delete':
      foreach ($form['blocks'] as $name => $value) {
        if (substr($name, 0, 1) != '#' && $value['#value'] == 1)
          return;
      }
      form_set_error('blocks', t('Select at least one block.'));
      break;
    case 'create':
      $existing_position = db_result(db_query('SELECT count(pid) FROM {zz_pub_UMI} where position="%s"',$form_state["values"]["position"]));
      
      if($form_state['values']['position']==""){
        form_set_error('position', t('The position field could not be empty.'));
      }
      elseif($existing_position){
        form_set_error('position', t('This position already exist.'));
      }
      break;
    default:
  }
}

/**
 * Submit actions for the form for the pub blocks admin page.
 */
function pub_blocks_admin_form_submit($form, &$form_state) {
  $op=$form_state['clicked_button']['#name'];
  
  switch ($op) {
    case 'delete':
  /*switch ($form['action_choice']['#value']) {
    case 'edit':
      foreach ($form['blocks'] as $name => $value) {
        if ($value['#value'] == 1) {
          drupal_goto('admin/build/block/configure/pub/'.$name);
        }
      }
      break;
    case 'delete':*/
      foreach (element_children($form['blocks']) as $name) {
        db_query('DELETE FROM {zz_pub_UMI} WHERE position = "%s"', $name);
      }
      /*break;
    default:
  }*/
      break;
    case 'create':
      $new_block = array();
      foreach (element_children($form['new_block']) as $name) {
        $new_block[$name] = $form['new_block'][$name]['#value'];
      }
      drupal_write_record('zz_pub_UMI', $new_block);
      break;
    default:
  }
}

/**
 * Form for the pub blocks cache admin page.
 */
function pub_cache_admin_form(&$form_state) {
  $form['blocks_cache'] = array(
    '#title' => t('Choose blocks'),
    '#type' => 'radios',
    '#options' => array(),
  );
  
  $pub_list = db_query('SELECT * FROM {cache} WHERE cid LIKE "pub_%" ORDER BY cid');
  while ($pub_elem = db_fetch_array($pub_list)) {
    $expl_id = explode('-', $pub_elem['cid']);
    $form['blocks_cache'][$expl_id[0]] = array(
      '#type' => 'checkbox',
      '#title' => $expl_id[0],
      '#default_value' => 0,
    );
  }
  
  $analytic_list = db_query('SELECT * FROM {cache} WHERE cid LIKE "analytic_%" ORDER BY cid');
  while ($analytic_elem = db_fetch_array($analytic_list)) {
    $expl_id = explode('-', $analytic_elem['cid']);
    $form['blocks_cache'][$expl_id[0]] = array(
      '#type' => 'checkbox',
      '#title' => $expl_id[0],
      '#default_value' => 0,
    );
  }
  
  $xiti_list = db_query('SELECT * FROM {cache} WHERE cid LIKE "xiti_%" ORDER BY cid');
  while ($xiti_elem = db_fetch_array($xiti_list)) {
    $expl_id = explode('-', $xiti_elem['cid']);
    $form['blocks_cache'][$expl_id[0]] = array(
      '#type' => 'checkbox',
      '#title' => $expl_id[0],
      '#default_value' => 0,
    );
  }
  
  $metriweb_list = db_query('SELECT * FROM {cache} WHERE cid LIKE "metriweb_%" ORDER BY cid');
  while ($metriweb_elem = db_fetch_array($metriweb_list)) {
    $expl_id = explode('-', $metriweb_elem['cid']);
    $form['blocks_cache'][$expl_id[0]] = array(
      '#type' => 'checkbox',
      '#title' => $expl_id[0],
      '#default_value' => 0,
    );
  }
  
  $form['blocks_submit'] = array(
    '#type' => 'submit',
    '#title' => t('Delete cache'),
    '#value' => t('Delete cache'),
  );
  
  return $form;
}

/**
 * Validation for form for the pub blocks cache admin page.
 */
function pub_cache_admin_form_validate($form, &$form_state) {
  foreach ($form['blocks_cache'] as $name => $value) {
    if (substr($name, 0, 1) != '#' && $value['#value'] == 1)
      return;
  }
  form_set_error('blocks_cache', t('Select at least one block.'));
}

/**
 * Submit actions for form for the pub blocks cache admin page.
 */
function pub_cache_admin_form_submit($form, &$form_state) {
  foreach ($form['blocks_cache'] as $name => $value) {
    if (substr($name, 0, 1) != '#' && $value['#value'] == 1) {
      cache_clear_all($name, 'cache', TRUE);
    }
  }
}

/**
 * Get the most recent blocks list and save them in the 'zz_pub_UMI' table.
 * 
 * @return
 *   TRUE in case success, FALSE otherwise.
 */
function _pub_callalltags() {
  $xml_list = drupal_http_request(variable_get('pub_get_xml', 'http://gps1.intranet.rossel/private/script/ZZsqls.cgi?key=dumpTagsAdtech&format=xml'));
  $website = variable_get('pub_website', '');
  if ($website == 'LESOIR') {
    $website = 'LESOIRV6_FR';
  }

  if ($xml_list->code == 200) {
    $dom = new DomDocument();
    $dom->LoadXML($xml_list->data);
    
    $pub_list = db_query('SELECT * FROM {zz_pub_UMI}');
    $pub_array = array();
    while ($pub_elem = db_fetch_array($pub_list)) {
      $pub_array[] = $pub_elem;
    }
    
    foreach($dom->getElementsByTagName('row') as $row) {
      if ($row->getAttribute('website') == $website) {
        $update = FALSE;
        $tag_elem = array();
        foreach($row->attributes as $attrName => $attrValue) {
          $tag_elem[$attrName] = $attrValue->value;
        }
        unset($tag_elem['website']);
        
        if (strstr($tag_elem['position'], '_PURL')) {
          $tag_elem['builder'] = 'purl';
        }
        
        foreach($pub_array as $pub_elem) {
          if ($tag_elem['position'] == $pub_elem['position']) {
            drupal_write_record('zz_pub_UMI', $tag_elem, array('position'));
            $update = TRUE;
            break;
          }
        }
        
        if (!$update) {
          drupal_write_record('zz_pub_UMI', $tag_elem);
        }
      }
    }
    
    return TRUE;
  } else {
    return FALSE;
  }
}
