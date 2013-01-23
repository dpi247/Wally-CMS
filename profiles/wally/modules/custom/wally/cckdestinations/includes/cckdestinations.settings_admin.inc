<?php

/**
 * Manage multiple destinations
 */
function cckdestinations_page_multiple_destinations_form($form_state, $op = NULL, $key = NULL) {
  $form = array();

  $current_multiple_dests = cckdestinations_get_multiple_dests();
  $dest_vid = variable_get('wallymport_destinationpath', 0);

  $form['rows'] = array();

  foreach ($current_multiple_dests as $cur_multi_dest) {
    $form['rows'][$cur_multi_dest['id']]['name'] = array(
      '#type' => 'markup',
      '#value' => $cur_multi_dest['virtual'],
    );

    $form['rows'][$cur_multi_dest['id']]['reals'] = array(
      '#type' => 'markup',
      '#value' => $cur_multi_dest['reals'],
      '#theme' => 'cckdestinations_page_multiple_destinations_table_reals',
    );

    $form['rows'][$cur_multi_dest['id']]['parent'] = array(
      '#type' => 'markup',
      '#value' => $cur_multi_dest['parent'],
      '#theme' => 'cckdestinations_page_multiple_destinations_table_parent',
    );
  }

  if ($op == 'edit' && $key && isset($current_multiple_dests[$key])) {
    $edit_reals = $current_multiple_dests[$key]['reals'];
    foreach ($edit_reals as $edit_real) {
      $defaults_reals[$edit_real['tid']] = (object)$edit_real;
    }
    $default_reals = cckdestinations_autocomplete_merge_tags($defaults_reals, $dest_vid);
    $defaults = array(
      'name' => $key,
      'reals' => $default_reals,
      'parent' => $current_multiple_dests[$key]['parent'],
    );

    $form['edit_id'] = array(
      '#type' => 'hidden',
      '#value' => $current_multiple_dests[$key]['id'],
    );
  } else {
    $defaults = array(
      'name' => '',
      'reals' => '',
      'parent' => '',
    );
  }

  $form['new'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add new'),
  );

  $form['new']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $defaults['name'],
    '#required' => TRUE,
    '#weight' => -17,
  );

  $form['new']['reals'] = array(
    '#type' => 'cckdestinations_autocomplete',
    '#title' => t('Real destinations'),
    '#description' => t('You can enter more than one value'),
    '#default_value' => $defaults,
    '#required' => TRUE,
    '#vid' => variable_get('wallymport_destinationpath', 0),
    '#multiple' => TRUE,
    '#weight' => -16,
  );
  
  $form['new']['parent'] = _taxonomy_term_select(t('Parent'), 'parent', $defaults['parent'], $dest_vid, t('Parent term') .'.', 0, '<'. t('root') .'>');

  $form['new']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['#theme'] = 'cckdestinations_page_multiple_destinations_form';
  $form['#redirect'] = 'admin/wally/cckdestinations/settings/multipledests';

  return $form;
}

function cckdestinations_page_multiple_destinations_form_validate($form, $form_state) {
  $values = $form_state['values'];
  $dest_vid = variable_get('wallymport_destinationpath', '');
  if (wallytoolbox_taxonomy_get_path_by_tid_or_term($values['name'], $dest_vid)) {
    form_set_error('name', t('This is name already exists as real destination'));
  }

  if (is_array($values['reals'])) {
    $intersect = cckdestinations_multiple_dests_get_target_intersect($values['reals']);
    unset($intersect['none']);
    if (empty($intersect)) {
      form_set_error('reals', t('The given destinations have no common target'));
    }
  }
}

function cckdestinations_page_multiple_destinations_form_submit($form, $form_state) {
  $values = $form_state['values'];
  $mapping = array(
    'virtual' => $values['name'],
    'parent' => is_numeric($values['parent']) ? $values['parent'] : 0,
  );

  foreach ($values['reals'] as $tid) {
    $term = (array)taxonomy_get_term($tid);
    $term['hierarchic_name'] = wallytoolbox_taxonomy_get_path_by_tid_or_term($tid);
    $mapping['reals'][$tid] = $term;
  }
  if (isset($form['edit_id'])) {
    $mapping['id'] = $form['edit_id']['#value'];
    drupal_write_record('cckdestinations_multiple_destinations', $mapping, array('id'));
  } else {
    drupal_write_record('cckdestinations_multiple_destinations', $mapping);
  }
}

/**
 * Delete an existing multiple destination
 */
function cckdestinations_page_multiple_destinations_delete_form($form_state, $key) {
  $question = t('Are you sure you want to delete virtual destination "@name"?', array('@name' => $key));

  $add_form = array();
  $add_form['key'] = array(
    '#type' => 'value',
    '#value' => $key,
  );

  $form = confirm_form($add_form, $question, 'admin/wally/cckdestinations/settings/multipledests');
  $form['#redirect'] = 'admin/wally/cckdestinations/settings/multipledests';

  return $form;
}

function cckdestinations_page_multiple_destinations_delete_form_submit(&$form, &$form_state) {
  $values = $form_state['values'];
  $key = $values['key'];

  if (db_result(db_query('SELECT * FROM {cckdestinations_multiple_destinations} WHERE virtual = "%s"', $key))) {
    db_query('DELETE FROM {cckdestinations_multiple_destinations} WHERE virtual = "%s"', $key);
    drupal_set_message(t('Virtual destination deleted'));
  } else {
    drupal_set_message(t('No virtual destination matching the requested key.'), 'warning');
  }
}

/**
 * Ranks, behaviors and layouts
 */
function cckdestinations_page_ranks_layouts_form($form_state) {
  $form = array();
  $form['#tree'] = TRUE;
  
  $form['cckdestinations_rank'] = array(
    '#type' => 'fieldset',
    '#title' => t('Rank and behavior settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  // Make the other form items dependent upon it.
  ctools_include('dependent');
  ctools_add_js('dependent');
  
  $form['cckdestinations_rank']['cckdestinations_rank_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Global rank\'s range'),
    '#default_value' => wally_variable_get('cckdestinations_rank_limit', 10),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  
  $hierarchies = wallyctools_get_hierarchies();
  $behavior_options = array(
    'bottom' => t('Bottom'),
    'top' => t('Top'),
  );

  foreach ($hierarchies as $tid => $hierarchy) {
    $form['cckdestinations_rank']['cckdestinations_destination_'.$tid] = array(
      '#type' => 'fieldset',
      '#title' => $hierarchy['term_name'].' ('.wallytoolbox_taxonomy_get_path_by_tid_or_term($tid).')',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    
    foreach ($hierarchy['redacblocks'] as $redacblock) {
      $defaults = cckdestination_get_default_rank_settings();
      $dest_settings = wally_variable_get('cckdestinations_destination_'.$tid.'_'.$redacblock['name'], $defaults);

      $form['cckdestinations_rank']['cckdestinations_destination_'.$tid][$redacblock['name']] = array(
        '#type' => 'fieldset',
        '#title' => $redacblock['title'],
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      
      $form['cckdestinations_rank']['cckdestinations_destination_'.$tid][$redacblock['name']]['cckdestinations_destination_'.$tid.'_'.$redacblock['name'].'_sort_method'] = array(
        '#type' => 'radios',
        '#title' => t('Sorting method'),
        '#description' => t('Based on the rank, two sorting methods can be applied : the rank is an absolute position (a rank of 2 means the second place) or the rank represent an time interval added to the node publication date.'),
        '#options' => array('absolute' => t('Absolute'),'interval' => t('Time interval')),
        '#default_value' => $dest_settings['sort_method'],
      );
      
      $radio_name = 'cckdestinations_rank[cckdestinations_destination_'.$tid.']['.$redacblock['name'].'][cckdestinations_destination_'.$tid.'_'.$redacblock['name'].'_sort_method]';
      
      $form['cckdestinations_rank']['cckdestinations_destination_'.$tid][$redacblock['name']]['cckdestinations_destination_'.$tid.'_'.$redacblock['name'].'_behavior'] = array(
        '#type' => 'select',
        '#title' => t('Behavior'),
        '#default_value' => $dest_settings['behavior'],
        '#options' => $behavior_options,
        '#process' => array('ctools_dependent_process'),
        '#dependency' => array('radio:'.$radio_name => array('absolute')),
      );
      
      $form['cckdestinations_rank']['cckdestinations_destination_'.$tid][$redacblock['name']]['cckdestinations_destination_'.$tid.'_'.$redacblock['name'].'_reserved'] = array(
        '#type' => 'textfield',
        '#title' => t('Reserved positions'),
        '#description' => t('Must be smaller than the global rank\'s range'),
        '#default_value' => $dest_settings['reserved'],
        '#size' => 80,
        '#maxlength' => 512,
        '#required' => TRUE,
      	'#process' => array('ctools_dependent_process'),
        '#dependency' => array('radio:'.$radio_name => array('absolute')),
      );
      
      $form['cckdestinations_rank']['cckdestinations_destination_'.$tid][$redacblock['name']]['cckdestinations_destination_'.$tid.'_'.$redacblock['name'].'_delta_t'] = array(
        '#type' => 'textfield',
        '#title' => t('Time interval "between" two ranks'),
        '#description' => t('The step of time, in hours, that will be set between two rank.'),
        '#default_value' => $dest_settings['delta_t'],
        '#size' => 80,
        '#maxlength' => 512,
        '#required' => TRUE,
        '#process' => array('ctools_dependent_process'),
        '#dependency' => array('radio:'.$radio_name => array('interval')),
      );
    }
  }
  
  $form['cckdestinations_layout'] = array(
    '#type' => 'fieldset',
    '#title' => t('Layout settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $form['cckdestinations_layout']['cckdestinations_layouts'] = array(
    '#type' => 'textarea',
    '#title' => t('Available layouts'),
    '#default_value' => wally_variable_get('cckdestinations_layouts', 'small'.PHP_EOL.'medium'.PHP_EOL.'large'),
    '#required' => TRUE,
  );
  
  $form['buttons']['submit'] = array(
  	'#type' => 'submit',
  	'#value' => t('Save configuration')
  );
  
  $form['buttons']['reset'] = array(
  	'#type' => 'submit',
  	'#value' => t('Reset to defaults')
  );
  
  return $form;
}

function cckdestinations_page_ranks_layouts_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  
  if ($values['op'] == $values['buttons']['submit']) {
    $rank_limit = $values['cckdestinations_rank']['cckdestinations_rank_limit'];
    if (!is_numeric($rank_limit) || $rank_limit < 0) {
      form_set_error('cckdestinations_rank][cckdestinations_rank_limit', t('The given maximum rank value must be a numeric value greater or equal than 0.'));
    }
    
    foreach ($values['cckdestinations_rank'] as $dest_name => $dest) {
      if (is_array($dest)) {
        foreach ($dest as $block_name => $block) {
          $sort_method = $block[$dest_name.'_'.$block_name.'_sort_method'];
          $reserved = $block[$dest_name.'_'.$block_name.'_reserved'];
          $delta_t = $block[$dest_name.'_'.$block_name.'_delta_t'];
          if ($sort_method == 'interval' && (!is_numeric($delta_t) || $delta_t < 0)) {
            form_set_error('cckdestinations_rank]['.$dest_name.']['.$block_name.']['.$dest_name.'_'.$block_name.'_delta_t', t('The time interval must be a numeric value greater than 0.'));
          } elseif ($sort_method == 'absolute' && (!is_numeric($reserved) || $reserved < 0 || $reserved > $rank_limit)) {
            form_set_error('cckdestinations_rank]['.$dest_name.']['.$block_name.']['.$dest_name.'_'.$block_name.'_reserved', t('The reserved positions value must be a numeric value smaller than 0 and than the global rank\'s range.'));
          }
        }
      }
    }
  }
}

function cckdestinations_page_ranks_layouts_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  if ($values['op'] == $values['buttons']['submit']) {
    wally_variable_set('cckdestinations_rank_limit', $values['cckdestinations_rank']['cckdestinations_rank_limit']);
    wally_variable_set('cckdestinations_layouts', $values['cckdestinations_layout']['cckdestinations_layouts']);
    
    foreach ($values['cckdestinations_rank'] as $dest_name => $dest) {
      if (is_array($dest)) {
        foreach ($dest as $block_name => $block) {
          $temp_dest = array(
            'sort_method' => $block[$dest_name.'_'.$block_name.'_sort_method'],
            'behavior' => $block[$dest_name.'_'.$block_name.'_behavior'],
            'reserved' => $block[$dest_name.'_'.$block_name.'_reserved'],
            'delta_t' => $block[$dest_name.'_'.$block_name.'_delta_t'],
          );
          wally_variable_set($dest_name.'_'.$block_name, $temp_dest);
        }
      }
    }
    
    drupal_set_message(t('The configuration options have been saved.'));
  } elseif ($values['op'] == $values['buttons']['reset']) {
    module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
    wallytoolbox_variable_del_like('cckdestinations_');
    drupal_set_message(t('The configuration options have been reset to their default values.'));
  }
}

function cckdestinations_page_taxonomy_autocomplete_form($form_state) {
  $form = array();

  $form['cckdestinations_taxonomy_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Limit of terms'),
    '#description' => t('If the amount of proposed terms is greater thant this limit, the select box will be replaced by a suggestion autocomplete text field.'),
    '#default_value' => wally_variable_get('cckdestinations_taxonomy_limit', 10),
    '#size' => 80,
    '#maxlength' => 512,
  );

  $form['cckdestinations_display_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Displayed matching terms'),
    '#description' => t('The autocomplete field will display this amount of matching terms.'),
    '#default_value' => wally_variable_get('cckdestinations_display_limit', 10),
    '#size' => 80,
    '#maxlength' => 512,
  );

  return wally_system_settings_form($form, TRUE);
}

function cckdestinations_page_taxonomy_autocomplete_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  
  if ($values['op'] == $values['submit']) {
    $tax_limit = $values['cckdestinations_taxonomy_limit'];
    if (!is_numeric($tax_limit) || $tax_limit < 0) {
      form_set_error('cckdestinations_taxonomy_limit', t('The given term limit value must be a numeric value greater or equal than 0.'));
    }
    $display_limit = $values['cckdestinations_display_limit'];
    if (!is_numeric($display_limit) || $display_limit < 0) {
      form_set_error('cckdestinations_display_limit', t('The given display limit value must be a numeric value greater or equal than 0.'));
    }
  }
}

function theme_cckdestinations_page_multiple_destinations_form($form) {
  $rows_id = element_children($form['rows']);
  if (!isset($form['rows']) || empty($rows_id)) {
    drupal_set_message(t('No multiple destination defined'));
    $output = '';
  } else {
    $rows = array();
    foreach($rows_id as $row_id) {
      $rows[] = array(
        drupal_render($form['rows'][$row_id]['name']),
        drupal_render($form['rows'][$row_id]['reals']),
        drupal_render($form['rows'][$row_id]['parent']),
        _cckdestinations_multiple_destinations_table_operations($form['rows'][$row_id]['name']['#value']),
      );
    }

    //Make sure the header count matches the column count
    $header = array(
      t('Virtual destinations'), t('Mapped real destinations'), t('Parent'), t('Operations'),
    );
  
    $output = theme('table', $header, $rows);
  }

  $output .= drupal_render($form);

  return $output;
}

function theme_cckdestinations_page_multiple_destinations_table_reals($element) {
  $items = $element['#value'];
  $ret = '';
  foreach ($items as $item) {
    $ret .= '<b>'.$item['name'].'</b>';
    $ret .= ' ('.$item['hierarchic_name'].')<br>';
  }
  return $ret;
}

function theme_cckdestinations_page_multiple_destinations_table_parent($element) {
  $parent_tid = $element['#value'];
  if (is_numeric($parent_tid) && $parent_tid != 0) {
    $ret = wallytoolbox_taxonomy_get_path_by_tid_or_term($parent_tid);
  } else {
    $ret = '';
  }
  return $ret;
}

function _cckdestinations_multiple_destinations_table_operations($id) {
  $return = l('Edit', 'admin/wally/cckdestinations/settings/multipledests/edit/'.$id).' ';
  $return .= l('Delete', 'admin/wally/cckdestinations/settings/multipledests/delete/'.$id);
  
  return $return;
}
