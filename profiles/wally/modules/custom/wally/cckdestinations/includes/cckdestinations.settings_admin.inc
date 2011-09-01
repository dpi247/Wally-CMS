<?php
// $Id: yaxim.admin.inc,v 1.0 2010/08/23 14:39:00 rso Exp $

/**
 * Settings form.
 */
function cckdestinations_page_admin_form($form_state) {
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
      '#title' => $hierarchy['term_name'],
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    
    foreach ($hierarchy['redacblocks'] as $redacblock) {
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
        '#default_value' => variable_get('cckdestinations_destination_'.$item['tid'].'_'.$item['target'].'_sort_method', 'absolute'),
      );
      
      $radio_name = 'cckdestinations_rank[cckdestinations_destination_'.$tid.']['.$redacblock['name'].'][cckdestinations_destination_'.$tid.'_'.$redacblock['name'].'_sort_method]';
      
      $form['cckdestinations_rank']['cckdestinations_destination_'.$tid][$redacblock['name']]['cckdestinations_destination_'.$tid.'_'.$redacblock['name'].'_behavior'] = array(
        '#type' => 'select',
        '#title' => t('Behavior'),
        '#default_value' => variable_get('cckdestinations_destination_'.$tid.'_'.$redacblock['name'].'_behavior', 'bottom'),
        '#options' => $behavior_options,
        '#process' => array('ctools_dependent_process'),
        '#dependency' => array('radio:'.$radio_name => array('absolute')),
      );
      
      $form['cckdestinations_rank']['cckdestinations_destination_'.$tid][$redacblock['name']]['cckdestinations_destination_'.$tid.'_'.$redacblock['name'].'_reserved'] = array(
        '#type' => 'textfield',
        '#title' => t('Reserved positions'),
        '#description' => t('Must be smaller than the global rank\'s range'),
        '#default_value' => variable_get('cckdestinations_destination_'.$tid.'_'.$redacblock['name'].'_reserved', 5),
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
        '#default_value' => variable_get('cckdestinations_destination_'.$tid.'_'.$redacblock['name'].'_delta_t', 1),
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
  
  $form['cckdestinations_select_to_auto'] = array(
    '#type' => 'fieldset',
    '#title' => t('Taxonomy autocomplete settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $form['cckdestinations_select_to_auto']['cckdestinations_taxonomy_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Limit of terms'),
    '#description' => t('If the amount of proposed terms is greater thant this limit, the select box will be replaced by a suggestion autocomplete text field.'),
    '#default_value' => variable_get('cckdestinations_taxonomy_limit', 10),
    '#size' => 80,
    '#maxlength' => 512,
  );
  
  $form['cckdestinations_select_to_auto']['cckdestinations_display_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Displayed matching terms'),
    '#description' => t('The autocomplete field will display this amount of matching terms.'),
    '#default_value' => variable_get('cckdestinations_display_limit', 10),
    '#size' => 80,
    '#maxlength' => 512,
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

/**
 * Validate function for the settings form.
 */
function cckdestinations_page_admin_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  
  if ($values['op'] == $values['buttons']['submit']) {
    $rank_limit = $values['cckdestinations_rank']['cckdestinations_rank_limit'];
    if (!is_numeric($rank_limit) || $rank_limit < 0) {
      form_set_error('cckdestinations_rank][cckdestinations_rank_limit', t('The given maximum rank value must be a numeric value greater or equal than 0.'));
    }
    
    $tax_limit = $values['cckdestinations_select_to_auto']['cckdestinations_taxonomy_limit'];
    if (!is_numeric($tax_limit) || $tax_limit < 0) {
      form_set_error('cckdestinations_select_to_auto][cckdestinations_taxonomy_limit', t('The given term limit value must be a numeric value greater or equal than 0.'));
    }
    $display_limit = $values['cckdestinations_select_to_auto']['cckdestinations_display_limit'];
    if (!is_numeric($tax_limit) || $tax_limit < 0) {
      form_set_error('cckdestinations_select_to_auto][cckdestinations_display_limit', t('The given display limit value must be a numeric value greater or equal than 0.'));
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

/**
 * Submit function for the settings form.
 */
function cckdestinations_page_admin_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  if ($values['op'] == $values['buttons']['submit']) {
    variable_set('cckdestinations_rank_limit', $values['cckdestinations_rank']['cckdestinations_rank_limit']);
    variable_set('cckdestinations_layouts', $values['cckdestinations_layout']['cckdestinations_layouts']);
    variable_set('cckdestinations_taxonomy_limit', $values['cckdestinations_select_to_auto']['cckdestinations_taxonomy_limit']);
    variable_set('cckdestinations_display_limit', $values['cckdestinations_select_to_auto']['cckdestinations_display_limit']);

    foreach ($values['cckdestinations_rank'] as $dest) {
      if (is_array($dest)) {
        foreach ($dest as $block) {
          foreach ($block as $name => $value)
            variable_set($name, $value);
        }
      }
    }
    
    drupal_set_message(t('The configuration options have been saved.'));
  } elseif ($values['op'] == $values['buttons']['reset']) {
    wallytoolbox_variable_del_like('cckdestinations_');
    drupal_set_message(t('The configuration options have been reset to their default values.'));
  }
}

