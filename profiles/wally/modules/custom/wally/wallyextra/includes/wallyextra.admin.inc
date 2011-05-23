<?php

/**
 * Wallyextra "Supercron" settings form
 */
function wallyextra_supercron_settings ($form_state) {
  $blocks_list = db_query('SELECT * FROM {wallyextra_cache_torefresh} ORDER BY weight ASC, cid ASC');
  
  $form = array();
  
  $form['invoke-all']=array(
    '#type' => 'item',
    '#value' => l('<img src="/'.drupal_get_path('module', 'supercron').'/images/cog_go.png" alt="Run" title="Run"> '.t('Run all'), 'admin/wally/wallyextra/supercron/call/all', array('attributes' => array(), 'html' => TRUE)),
  );
  
  $form['rows']['#tree'] = TRUE;
  
  while ($block = db_fetch_array($blocks_list)) {
    $form['rows'][$block['cid']]['cid'] = array(
      '#type' => 'hidden',
      '#value' => $block['cid'],
    );
    
    $form['rows'][$block['cid']]['last_run'] = array(
      '#type' => 'value',
      '#value' => $block['last_run'],
    );
    
    $form['rows'][$block['cid']]['output'] = array(
      '#type' => 'hidden',
      '#value' => $block['output'],
    );
    
    $form['rows'][$block['cid']]['enabled'] = array(
      '#type' => 'checkbox',
      '#default_value' => $block['enabled'],
    );

    $form['rows'][$block['cid']]['weight']=array(
      '#type' => 'textfield',
      '#size' => 5,
      '#default_value' => $module->weight,
      '#attributes' => array('class' => 'weight'),
    );
    
    $form['rows'][$block['cid']]['rule']=array(
      '#type'          => 'textfield',
      '#size'          => 10,
      '#default_value' => round($block['rule']/60, 2),
      '#description'   => t('It will be called every minute if set between 0 and 1.'),
    );
    
    while (is_string($block['settings']))
      $block['settings'] = unserialize($block['settings']);
    $form['rows'][$block['cid']]['settings'] = array(
      '#type' => 'hidden',
      '#value' => $block['settings'],
    );
  }
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes'),
  );
  
  return $form;
}

/**
 * Theming for the wallyextra "Supercron" settings form
 */
function theme_wallyextra_supercron_settings($form) {
  $header[] = '';
  $header[] = '';
  $header[] = t('Page');
  $header[] = t('Type');
  $header[] = t('Subtype');
  $header[] = t('Panel');
  $header[] = t('PID');
  $header[] = t('Last run');
  $header[] = t('Output');
  $header[] = t('Enabled');
  $header[] = t('Run frequency (min)');
  $header[] = '';

  foreach (element_children($form['rows']) as $cid) {
    $form_row = &$form['rows'][$cid];
    $expl_cid = explode(':', $cid);
    
    $row = array();
    $row[] = '';
    $row[] = l('<img src="/'.drupal_get_path('module', 'supercron').'/images/cog_go.png" alt="Run" title="Run">', 'admin/wally/wallyextra/supercron/call/'.$cid, array('attributes' => array(), 'html' => TRUE));
    $row[] = check_plain($expl_cid[0]);
    $row[] = check_plain($expl_cid[1]);
    $row[] = check_plain($expl_cid[2]);
    $row[] = check_plain($expl_cid[3]);
    $row[] = check_plain($expl_cid[4]);
    $row[] = date('Y-m-d H:i:s', $form_row['last_run']['#value']);
    $row[] = l('Output', 'admin/wally/wallyextra/supercron/output/'.$cid);
    $row[] = drupal_render($form_row['enabled']);
    $row[] = drupal_render($form_row['rule']);
    $row[] = drupal_render($form_row['weight']);

    $rows[] = array('data' => $row, 'class' => 'draggable');
    unset($form['rows'][$cid]);
  }

  drupal_add_tabledrag('wallyextra-supercron-table', 'order', 'sibling', 'weight');

  $output  = '';
  $output .= drupal_render($form['invoke-all']);
  $output .= theme('table', $header, $rows, array('id' => 'wallyextra-supercron-table'));
  $output .= drupal_render($form);

  return $output;
}

/**
 * Validation for the wallyexra "Supercron" settings form
 */
function wallyextra_supercron_settings_validate($form, &$form_state) {
  foreach ($form_state['values']['rows'] as $row) {
    if (isset($row['rule']) && !is_numeric($row['rule'])) {
      form_set_error('rows]['.$row['cid'].'][rule', t('The refresh time isn\'t a numeric value'));
    } elseif (isset($row['rule']) && ($row['rule'] < 0 || $row['rule'] >= 60)) {
      form_set_error('rows]['.$row['cid'].'][rule', t('The refresh time must be greater or equal to 0 and smaller than 60 minutes'));
    }
  }
}

/**
 * Submit for the wallyexra "Supercron" settings form
 */
function wallyextra_supercron_settings_submit($form, &$form_state) {
  foreach ($form_state['values']['rows'] as $row) {
    $row['rule'] *= 60;
    drupal_write_record('wallyextra_cache_torefresh', $row, 'cid');
  }
  
  drupal_set_message(t('Configuration saved'));
}

/**
 * Print the outputs of a Wallyextra "Supercron" entry.
 */
function wallyextra_supercron_output($cid) {
  $output = db_fetch_array(db_query('SELECT output FROM {wallyextra_cache_torefresh} WHERE cid = "%s"', $cid));
  drupal_set_title(drupal_get_title().' '.$cid);
  return unserialize($output['output']);
}

/**
 * Calls a single Wallyextra "Supercron" entry (or all).
 */
function wallyextra_supercron_call($cid) {
  if ($cid == 'all') {
    $blocks_list = db_query('SELECT * FROM {wallyextra_cache_torefresh} WHERE enabled = 1 ORDER BY weight ASC, cid ASC');
  } else {
    $blocks_list = db_query('SELECT * FROM {wallyextra_cache_torefresh} WHERE cid = "%s"', $cid);
  }
  
  while ($block = db_fetch_array($blocks_list)) {
    while (is_string($block['settings']))
      $block['settings'] = unserialize($block['settings']);
    $output = module_invoke_all('wallyextra_refresh_block', $block['cid'], $block['settings']);
    $block['output'] .= serialize($output);
    $block['last_run'] = time();
    drupal_write_record('wallyextra_cache_torefresh', $block, 'cid');
  }

  drupal_goto('admin/wally/wallyextra/supercron');
}
