<?php

/**
 * WallyToolbox "Supercron" settings form
 */
function wallytoolbox_supercron_settings_form ($form_state, $page = NULL, $block_type = NULL) {
  $form = array();
  
  $form['invoke-all'] = array(
    '#type' => 'item',
    '#value' => l('<img src="/'.drupal_get_path('module', 'supercron').'/images/cog_go.png" alt="'.t('Run all crons').'" title="'.t('Run all crons').'"> '.t('Run all'), 'admin/wally/wallytoolbox/supercron/call/all', array('html' => TRUE, 'query' => 'destination='.$_GET['q'])),
  );
  
  $form['go_to'] = array(
    '#type' => 'fieldset',
    '#title' => t('Change list criterias'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  
  $pages = page_manager_get_tasks_by_type('page');

  $page_options = array();
  $page_options['any'] = t('Any');
  foreach ($pages as $page_name => $page_infos) {
    $page_options[$page_name] = $page_infos['admin title'] ? $page_infos['admin title'] : $page_infos['title'];
  }
  $form['go_to']['page'] = array(
    '#type' => 'select',
    '#title' => t('Page'),
    '#options' => $page_options,
    '#default_value' => $page,
  );
  
  ctools_include('plugins');
  $existing_plugins = ctools_get_plugins('ctools', 'content_types');
  $options_plugins = array();
  $options_plugins['any'] = t('Any');
  foreach ($existing_plugins as $exst_plug) {
    $options_plugins[$exst_plug['name']] = $exst_plug['name'];
  }
  ksort($options_plugins);
  $form['go_to']['block_type'] = array(
    '#type' => 'select',
    '#title' => t('Block type'),
    '#options' => $options_plugins,
    '#default_value' => $block_type,
  );
  
  $form['go_to']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update list'),
    '#validate' => array(),
    '#submit' => array('wallytoolbox_supercron_settings_form_go_to_submit'),
  );

  if ($page != 'any' || $block_type != 'any') {
    if ($page == 'any') {
      $page = '%%';
    }
    if ($block_type == 'any') {
      $block_type == '%%';
    }
    if ($block_type) {
      $blocks_list = db_query('SELECT * FROM {wallytoolbox_content_types_cache} WHERE cid LIKE "%s:%:%s:%" ORDER BY weight ASC, cid ASC', $page, $block_type);
    } else {
      $blocks_list = db_query('SELECT * FROM {wallytoolbox_content_types_cache} WHERE cid LIKE "%s:%" ORDER BY weight ASC, cid ASC', $page);
    }

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

      $form['rows'][$block['cid']]['rule']=array(
        '#type' => 'textfield',
        '#size' => 10,
        '#default_value' => round($block['rule']/60, 2),
        '#description' => t('It will be called every minute if set between 0 and 1.'),
      );
      
      $form['rows'][$block['cid']]['weight']=array(
        '#type' => 'textfield',
        '#size' => 5,
        '#default_value' => $block['weight'],
        '#description' => t('The lowest weight will be executed first'),
      );
      
      while (is_string($block['settings'])) {
        $block['settings'] = unserialize($block['settings']);
      }
      $form['rows'][$block['cid']]['settings'] = array(
        '#type' => 'hidden',
        '#value' => $block['settings'],
      );
    }
  } else {
    drupal_set_message(t('One of the search criterias must be other than "Any".'), 'warning');
  }
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes'),
  );
  
  $form['force_refresh'] = array(
    '#type' => 'submit',
    '#value' => t('Force datas refresh'),
    '#validate' => array(),
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
  $header[] = t('Title');
  $header[] = t('Last run');
  $header[] = t('Output');
  $header[] = t('Enabled');
  $header[] = t('Run frequency (min)');
  $header[] = t('Weight');
  
  $pages = page_manager_get_tasks_by_type('page');
  ctools_include('context');

  foreach (element_children($form['rows']) as $cid) {
    $form_row = &$form['rows'][$cid];
    $expl_cid = explode(':', $cid);
    
    $actions = l('<img src="/'.drupal_get_path('module', 'wallytoolbox').'/images/cog_go.png" alt="'.t('Run').'" title="'.t('Run').'">', 'admin/wally/wallytoolbox/supercron/call/'.$cid, array('html' => TRUE, 'query' => 'destination='.$_GET['q']));
    $actions .= ($expl_cid[2] != $expl_cid[3]) ? ' <div class="form-item" style="padding:0;float:right;">
        <img src="/'.drupal_get_path('module', 'wallytoolbox').'/images/help.png" alt="'.t('Help').'" title="'.t('Help').'">
        <div class="description" style="max-width:none;">'.t('This is a reusable block, any changes to its properties will be applied to each instance of the block.').'</div>
      </div>' : '';
    
    $page = page_manager_get_page_cache($expl_cid[0]);

    $row = array();
    $row[] = '';
    $row[] = $actions;
    $row[] = $pages[$expl_cid[0]]['admin title'] ? $pages[$expl_cid[0]]['admin title'] : $pages[$expl_cid[0]]['title'];
    $row[] = $page->handlers[$expl_cid[1]]->conf['title'];
    $row[] = $expl_cid[2];
    $row[] = $expl_cid[3];
    $row[] = $expl_cid[4];
    $row[] = $expl_cid[5];
    $row[] = _wallytoolbox_get_setting($form_row['settings']['#value'], 'title', '');
    $row[] = $form_row['last_run']['#value'] ? date('Y-m-d H:i:s', $form_row['last_run']['#value']) : t('never');
    $row[] = l(t('Output'), 'admin/wally/wallytoolbox/supercron/output/'.$cid);
    $row[] = drupal_render($form_row['enabled']);
    $row[] = drupal_render($form_row['rule']);
    $row[] = drupal_render($form_row['weight']);

    $rows[] = array('data' => $row);
    unset($form['rows'][$cid]);
  }

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
    list($a, $b, $type, $subtype, $e, $pid) = explode(':', $row['cid']);

    if ($row['rule'] != $form['rows'][$row['cid']]['rule']['#default_value']) {
      _wallytoolbox_set_setting($row['settings'], 'wally_rule', $row['rule']);
      if ($type == $subtype) {
        db_query("UPDATE {panels_pane} SET configuration = '%s' WHERE pid = %d", serialize($row['settings']), $pid);
      } else {
        db_query("UPDATE {panels_pane} SET configuration = '%s' WHERE type = '%s' AND subtype = '%s'", serialize($row['settings']), $type, $subtype);
      }
    }
    
    $row['output'] = unserialize($row['output']);
    $row['rule'] *= 60;
    if ($type == $subtype) {
      drupal_write_record('wallytoolbox_content_types_cache', $row, 'cid');
    } else {
      $query_values = array();
      $query_args = array();
      if ($row['enabled'] != $form['rows'][$row['cid']]['enabled']['#default_value']) {
        $query_values[] = 'enabled = %d';
        $query_args[] = $row['enabled'];
      }
      if ($row['weight'] != $form['rows'][$row['cid']]['weight']['#default_value']) {
        $query_values[] = 'weight = %d';
        $query_args[] = $row['weight'];
      }
      if ($row['rule']/60 != $form['rows'][$row['cid']]['rule']['#default_value']) {
        $query_values[] = 'rule = %d';
        $query_args[] = $row['rule'];
      }
      $query_args[] = $type;
      $query_args[] = $subtype;
      if (!empty($query_values)) {
        db_query("UPDATE {wallytoolbox_content_types_cache} SET ".implode($query_values, ', ')." WHERE cid LIKE '%%:%%:%s:%s:%%:%%'", $query_args);
      }
    }
  }
  
  drupal_set_message(t('Configuration saved'));
}

/**
 * Specific submit for the wallytoolbox "Supercron" settings form to change page and block type
 */
function wallytoolbox_supercron_settings_form_go_to_submit(&$form, &$form_state) {
  $values = $form_state['values'];
  $goto = 'admin/wally/wallytoolbox/supercron/'.$values['page'];
  $goto .= !empty($values['block_type']) ? '/'.$values['block_type'] : '';
  $form['#redirect'] = $goto;
}

/**
 * Specific submit for the wallytoolbox "Supercron" settings form to force the refresh of the cached datas
 */
function wallytoolbox_supercron_settings_form_force_refresh_submit($form, $form_state) {
  wallytoolbox_refresh_cache_list();
  drupal_set_message(t('Datas refreshed'));
}

/**
 * Print the outputs of a wallytoolbox "Supercron" entry.
 */
function wallytoolbox_supercron_output($cid) {
  $output = db_fetch_array(db_query('SELECT output FROM {wallytoolbox_content_types_cache} WHERE cid = "%s"', $cid));
  drupal_set_title(drupal_get_title().' '.$cid);
  $unser_output = unserialize($output['output']);
  $to_print = '';
  foreach ($unser_output as $out_time => $out) {
    $item_to_print = '<b>On '.date('Y-m-d H:i:s', $out_time).' :</b><br>';
    foreach ($out as $out_id => $out_message) {
      if (is_array($out_message)) {
        foreach ($out_message as $out_mess) {
          $item_to_print .= $out_mess.'<br>';
        }
      } else {
        $item_to_print .= $out_message.'<br>';
      }
    }
    $item_to_print .= '<br>';
    $to_print = $item_to_print.$to_print;
  }
  return $to_print;
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
  
  module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
  
  $processed = $output = array();

  while ($block = db_fetch_array($blocks_list)) {
    $cid = $block['cid'];
    list($a, $b, $type, $subtype, $c, $d) = explode(':', $cid);
    $multiple_id = ($type == $subtype) ? $cid : $type.'::'.$subtype;

    if (!in_array($multiple_id, $processed)) {
      $processed[] = $multiple_id;

      if (isset($block['settings']) && is_string($block['settings']) && !empty($block['settings'])) {
        $block['settings'] = unserialize($block['settings']);
      }
      if (isset($block['callback']) && is_string($block['callback']) && !empty($block['callback'])) {
        $block['callback'] = unserialize($block['callback']);
      }
      
      wallytoolbox_set_microtime_step('', FALSE, 'wtb_cron_block', TRUE);
      $tmp_output = _wallytoolbox_process_block_refresh($block);
      if (is_string($tmp_output)) {
        $output[] = $tmp_output;
      } elseif (is_array($tmp_output)) {
        $output += $tmp_output;
      }
      $delta_t = wallytoolbox_set_microtime_step('', FALSE, 'wtb_cron_block');
      $micro = sprintf('%06d', ($delta_t - floor($delta_t)) * 1000000);
      $d = new DateTime(date('Y-m-d H:i:s.'.$micro, $delta_t));
      $formatted = $d->format('i:s.u');
      
      list($a, $b, $type, $subtype, $c, $d) = explode(':', $block['cid']);
      
      if (!empty($output)) {
        $output[] = 'Runtime : '.$formatted;
        $block['output'] = unserialize($block['output']);
        $block['output'][time()] = $output;
        $block['output'] = array_slice($block['output'], -50, 50, TRUE);
        if ($type == $subtype) {
          db_query('UPDATE {wallytoolbox_content_types_cache} SET output = "%s" WHERE cid = "%s"', serialize($block['output']), $block['cid']);
        } else {
          db_query('UPDATE {wallytoolbox_content_types_cache} SET output = "%s" WHERE cid LIKE "%%:%%:%s:%s:%%:%%"', serialize($block['output']), $type, $subtype);
        }
      }
      $block['last_run'] = time();
      if ($type == $subtype) {
        db_query('UPDATE {wallytoolbox_content_types_cache} SET last_run = %d WHERE cid = "%s"', $block['last_run'], $block['cid']);
      } else {
        db_query('UPDATE {wallytoolbox_content_types_cache} SET last_run = %d WHERE cid LIKE "%%:%%:%s:%s:%%:%%"', $block['last_run'], $type, $subtype);
      }
    }
  }

  $redirect = isset($_GET['destination']) ? $_GET['destination'] : 'admin/wally/wallytoolbox/supercron';
  drupal_goto($redirect);
}

function _wallytoolbox_process_block_refresh(&$block) {
  // When called from supercron the theme is set to admin theme, we have to force front theme to get correct templates suggestions.
  global $custom_theme;
  $custom_theme = variable_get('theme_default', 'garland');
  
  $block_infos = explode(':', $block['cid']);
  $block_infos = array(
    'page' => $block_infos[0],
    'handler' => $block_infos[1],
    'type' => $block_infos[2],
    'subtype' => $block_infos[3],
    'panel' => $block_infos[4],
    'pid' => $block_infos[5],
  );

  $output = array();
  if (isset($block['callback']) && !empty($block['callback'])) {
    $callback_datas = $block['callback'];
    if (isset($callback_datas['plugin']) && !empty($callback_datas['plugin'])) {
      ctools_include('plugins');
      if ($plugin = ctools_get_plugins('ctools', 'content_types', $callback_datas['plugin'])) {
        require_once './'.$plugin['path'].'/'.$plugin['file'];
      }
    }
    if (function_exists($callback_datas['callback'])) {
      $datas = call_user_func_array($callback_datas['callback'], array($block['settings']));
      if ($datas && !empty($datas) && isset($callback_datas['cid']) && !empty($callback_datas['cid'])) {
        $table = (isset($callback_datas['table']) && !empty($callback_datas['table'])) ? $callback_datas['table'] : 'cache_wally_blocks';
        cache_set($callback_datas['cid'], $datas, $table);
        $output[] = 'Process complete.';
      } else {
        $output[] = 'Process uncomplete, something went wrong during callback process.';
      }
    }
  } else {
    // Invoke hook_refresh_block
    $output[] = module_invoke_all('refresh_block', $block_infos, $block['settings']);
  }
  
  return $output;
}

function wallytoolbox_page_settings_form ($form_state) {
  $form = array();
  
  $form['images_flush'] = array(
    '#type' => 'fieldset',
    '#title' => t('Files flush settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  
  $form['images_flush']['wallytoolbox_files_days_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Remove files older than (in days)'),
    '#description' => t('The cron will remove files older than this amount of days'),
    '#default_value' => wally_variable_get('wallytoolbox_files_days_limit', 7),
  );
  
  return wally_system_settings_form($form, TRUE);
}

