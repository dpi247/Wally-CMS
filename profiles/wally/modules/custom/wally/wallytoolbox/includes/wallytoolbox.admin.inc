<?php

/**
 * WallyToolbox "Supercron" settings form
 */
function wallytoolbox_supercron_settings_form ($form_state, $page = NULL, $block_type = NULL) {
  $form = array();
  
  $form['invoke-all'] = array(
    '#type' => 'item',
    '#value' => l('<img src="/'.drupal_get_path('module', 'supercron').'/images/cog_go.png" alt="'.t('Run all crons').'" title="'.t('Run all crons').'"> '.t('Run all'), 'admin/wally/wallytoolbox/supercron/call/all', array('attributes' => array(), 'html' => TRUE)),
  );
  
  $form['go_to'] = array(
    '#type' => 'fieldset',
    '#title' => t('Change list criterias'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  
  $pages = page_manager_get_tasks_by_type('page');
  $page_options = array();
  foreach ($pages as $page_name => $page_infos) {
    $page_options[$page_name] = $page_name;
  }
  $form['go_to']['page'] = array(
    '#type' => 'select',
    '#title' => t('Page'),
    '#options' => $page_options,
    '#default_value' => $page,
  );
  
  $form['go_to']['block_type'] = array(
    '#type' => 'textfield',
    '#title' => t('Block type'),
    '#default_value' => $block_type,
  );
  
  $form['go_to']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update list'),
    '#submit' => array('wallytoolbox_supercron_settings_form_go_to_submit'),
  );

  if ($page) {
    $blocks_list = db_query('SELECT * FROM {wallytoolbox_content_types_cache} WHERE cid LIKE "%s:%:%s:%" ORDER BY weight ASC, cid ASC', $page, $block_type);

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
      
      while (is_string($block['settings'])) {
        $block['settings'] = unserialize($block['settings']);
      }
      $form['rows'][$block['cid']]['settings'] = array(
        '#type' => 'hidden',
        '#value' => $block['settings'],
      );
    }
  }
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes'),
  );
  
  $form['force_refresh'] = array(
    '#type' => 'submit',
    '#value' => t('Force datas refresh'),
    '#submit' => array('wallytoolbox_supercron_settings_form_force_refresh_submit'),
  );
  
  return $form;
}

/**
 * Theming for the wallytoolbox "Supercron" settings form
 */
function theme_wallytoolbox_supercron_settings_form($form) {
  $header[] = '';
  $header[] = '';
  $header[] = t('Page');
  $header[] = t('Variant');
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
    $row[] = l('<img src="/'.drupal_get_path('module', 'supercron').'/images/cog_go.png" alt="'.t('Run').'" title="'.t('Run').'">', 'admin/wally/wallytoolbox/supercron/call/'.$cid, array('attributes' => array(), 'html' => TRUE));
    $row[] = check_plain($expl_cid[0]);
    $row[] = check_plain($expl_cid[1]);
    $row[] = check_plain($expl_cid[2]);
    $row[] = check_plain($expl_cid[3]);
    $row[] = check_plain($expl_cid[4]);
    $row[] = check_plain($expl_cid[5]);
    $row[] = $form_row['last_run']['#value'] ? date('Y-m-d H:i:s', $form_row['last_run']['#value']) : t('never');
    $row[] = l(t('Output'), 'admin/wally/wallytoolbox/supercron/output/'.$cid);
    $row[] = drupal_render($form_row['enabled']);
    $row[] = drupal_render($form_row['rule']);
    $row[] = drupal_render($form_row['weight']);

    $rows[] = array('data' => $row, 'class' => 'draggable');
    unset($form['rows'][$cid]);
  }

  drupal_add_tabledrag('wallytoolbox-supercron-table', 'order', 'sibling', 'weight');

  $output .= drupal_render($form['invoke-all']);
  $output .= drupal_render($form['go_to']);
  $output .= theme('table', $header, $rows, array('id' => 'wallytoolbox-supercron-table'));
  $output .= drupal_render($form);

  return $output;
}

/**
 * Validation for the wallytoolbox "Supercron" settings form
 */
function wallytoolbox_supercron_settings_form_validate($form, &$form_state) {
  foreach ($form_state['values']['rows'] as $row) {
    if (isset($row['rule']) && !is_numeric($row['rule'])) {
      form_set_error('rows]['.$row['cid'].'][rule', t('The refresh time isn\'t a numeric value'));
    } elseif (isset($row['rule']) && $row['rule'] < 0) {
      form_set_error('rows]['.$row['cid'].'][rule', t('The refresh time must be greater or equal to 0'));
    }
  }
}

/**
 * Submit for the wallytoolbox "Supercron" settings form
 */
function wallytoolbox_supercron_settings_form_submit($form, &$form_state) {
  foreach ($form_state['values']['rows'] as $row) {
    $row['rule'] *= 60;
    drupal_write_record('wallytoolbox_content_types_cache', $row, 'cid');
  }
  
  drupal_set_message(t('Configuration saved'));
}

/**
 * Specific submit for the wallytoolbox "Supercron" settings form to change page and block type
 */
function wallytoolbox_supercron_settings_form_go_to_submit($form, &$form_state) {
  $values = $form_state['values'];
  $goto = 'admin/wally/wallytoolbox/supercron/'.$values['page'];
  $goto .= !empty($values['block_type']) ? '/'.$values['block_type'] : '';
  drupal_goto($goto);
}

/**
 * Specific submit for the wallytoolbox "Supercron" settings form to force the refresh of the cached datas
 */
function wallytoolbox_supercron_settings_form_force_refresh_submit($form, $form_state) {
  wallytoolbox_get_handlers(TRUE);
  drupal_set_message(t('Datas refreshed'));
  drupal_goto('admin/wally/wallytoolbox/supercron');
}

/**
 * Print the outputs of a wallytoolbox "Supercron" entry.
 */
function wallytoolbox_supercron_output($cid) {
  $output = db_fetch_array(db_query('SELECT output FROM {wallytoolbox_content_types_cache} WHERE cid = "%s"', $cid));
  drupal_set_title(drupal_get_title().' '.$cid);
  return unserialize($output['output']);
}

/**
 * Calls a single wallytoolbox "Supercron" entry (or all).
 */
function wallytoolbox_supercron_call($cid) {
  if ($cid == 'all') {
    $blocks_list = db_query('SELECT * FROM {wallytoolbox_content_types_cache} WHERE enabled = 1 ORDER BY weight ASC, cid ASC');
  } else {
    $blocks_list = db_query('SELECT * FROM {wallytoolbox_content_types_cache} WHERE cid = "%s"', $cid);
  }
  
  while ($block = db_fetch_array($blocks_list)) {
    while (is_string($block['settings']))
      $block['settings'] = unserialize($block['settings']);
    $output = module_invoke_all('refresh_block', $block['cid'], $block['settings']);
    if (!empty($output)) {
      $block['output'] .= serialize($output);
      db_query('UPDATE {wallytoolbox_content_types_cache} SET output = "%s" WHERE cid = "%s"', $block['output'], $block['cid']);
    }
    $block['last_run'] = time();
    db_query('UPDATE {wallytoolbox_content_types_cache} SET last_run = %d WHERE cid = "%s"', $block['last_run'], $block['cid']);
  }

  drupal_goto('admin/wally/wallytoolbox/supercron');
}
