<?php

/**
 * Manage multiple destinations
 */
function cckdestinations_page_multiple_destinations_form($form_state, $op = NULL, $key = NULL) {
  $form = array();

  $current_multiple_dests = cckdestinations_get_multiple_dests();
  
  $form['rows'] = array();
  
  foreach ($current_multiple_dests as $cur_multi_dest) {
    $form['rows'][$cur_multi_dest['id']]['name'] = array(
      '#type' => 'markup',
      '#value' => $cur_multi_dest['virtual'],
    );
    
    $form['rows'][$cur_multi_dest['id']]['reals'] = array(
      '#type' => 'markup',
      '#value' => $cur_multi_dest['reals'],
    );
  }
  
  $form['#theme'] = 'cckdestinations_page_multiple_destinations_form';
  
  return $form;
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
    '#default_value' => variable_get('cckdestinations_rank_limit', 10),
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
      $defaults = array(
        'sort_method' => 'absolute',
        'behavior' => 'top',
        'reserved' => 5,
        'delta_t' => 1,
      );
      $dest_settings = variable_get('cckdestinations_destination_'.$tid.'_'.$redacblock['name'], $defaults);

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
    '#default_value' => variable_get('cckdestinations_layouts', 'small'.PHP_EOL.'medium'.PHP_EOL.'large'),
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
    variable_set('cckdestinations_rank_limit', $values['cckdestinations_rank']['cckdestinations_rank_limit']);
    variable_set('cckdestinations_layouts', $values['cckdestinations_layout']['cckdestinations_layouts']);
    
    foreach ($values['cckdestinations_rank'] as $dest_name => $dest) {
      if (is_array($dest)) {
        foreach ($dest as $block_name => $block) {
          $temp_dest = array(
            'sort_method' => $block[$dest_name.'_'.$block_name.'_sort_method'],
            'behavior' => $block[$dest_name.'_'.$block_name.'_behavior'],
            'reserved' => $block[$dest_name.'_'.$block_name.'_reserved'],
            'delta_t' => $block[$dest_name.'_'.$block_name.'_delta_t'],
          );
          variable_set($dest_name.'_'.$block_name, $temp_dest);
        }
      }
    }
    
    drupal_set_message(t('The configuration options have been saved.'));
  } elseif ($values['op'] == $values['buttons']['reset']) {
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
    '#default_value' => variable_get('cckdestinations_taxonomy_limit', 10),
    '#size' => 80,
    '#maxlength' => 512,
  );

  $form['cckdestinations_display_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Displayed matching terms'),
    '#description' => t('The autocomplete field will display this amount of matching terms.'),
    '#default_value' => variable_get('cckdestinations_display_limit', 10),
    '#size' => 80,
    '#maxlength' => 512,
  );

  return system_settings_form($form);
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
  if (!isset($form['rows']) || empty(element_children($form['rows']))) {
    drupal_set_message(t('No multiple destination defined'));
    $output = '';
  } else {
    $rows = array();
    foreach(element_children($form['rows']) as $row_id) {
      $rows[] = array(
        drupal_render($form['rows'][$row_id]['name']),
        drupal_render($form['rows'][$row_id]['reals']),
        _cckdestinations_multiple_destinations_table_operations($row_id),
      );
    }
  
    //Make sure the header count matches the column count
    $header = array(
      t('Virtual destinations'), t('Mapped real destinations'), t('Operations'),
    );
  
    $output = theme('table', $header, $rows);
  }

  $output .= drupal_render($form);

  return $output;
}

function _cckdestinations_multiple_destinations_table_operations($id) {
  $return = l('Edit', 'admin/wally/cckdestinations/settings/multipledests/edit/'.$id['rss_key']).' ';
  $return .= l('Delete', 'admin/wally/cckdestinations/settings/multipledests/delete/'.$id['rss_key']);
  
  return $return;
}
