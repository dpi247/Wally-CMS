<?php

function wallyrules_flush_home_page_cache_form($settings, &$form, &$form_state) {
  $settings += array('caches_to_flush' => array());

  $form['settings']['caches_to_flush'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Choose which type of caches to flush'),
    '#default_value' => $settings['caches_to_flush'],
    '#options' => array(
      'drupal_cache' => t('Drupal cache'),
      'varnish_cache' => t('Varnish cache'),
    ),
  );
}

function wallyrules_flush_home_page_cache_validate(&$form, &$form_state) {
  foreach($form_state['values']['settings']['caches_to_flush'] as $cache_to_flush) {
    if ($cache_to_flush !== 0)
      return;
  }
  form_set_error('caches_to_flush', t('Select at least one cache to flush.'));
}

function wallyrules_flush_node_page_cache_form($settings, &$form, &$form_state) {
  $settings += array('caches_to_flush' => array());

  $form['settings']['caches_to_flush'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Choose which type of caches to flush'),
    '#default_value' => $settings['caches_to_flush'],
    '#options' => array(
      'drupal_cache' => t('Drupal cache'),
      'varnish_cache' => t('Varnish cache'),
    ),
  );
}

function wallyrules_flush_node_page_cache_validate(&$form, &$form_state) {
  foreach($form_state['values']['settings']['caches_to_flush'] as $cache_to_flush) {
    if ($cache_to_flush !== 0)
      return;
  }
  form_set_error('caches_to_flush', t('Select at least one cache to flush.'));
}

function wallyrules_flush_destinations_page_cache_form($settings, &$form, &$form_state) {
  $settings += array('caches_to_flush' => array());

  $form['settings']['caches_to_flush'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Choose which type of caches to flush'),
    '#default_value' => $settings['caches_to_flush'],
    '#options' => array(
      'drupal_cache' => t('Drupal cache'),
      'varnish_cache' => t('Varnish cache'),
    ),
  );
}

function wallyrules_flush_destinations_page_cache_validate(&$form, &$form_state) {
  foreach($form_state['values']['settings']['caches_to_flush'] as $cache_to_flush) {
    if ($cache_to_flush !== 0)
      return;
  }
  form_set_error('caches_to_flush', t('Select at least one cache to flush.'));
}

function wallyrules_flush_custom_page_cache_form($settings, &$form, &$form_state) {
  $settings += array('urls_to_flush' => '', 'caches_to_flush' => array());
  
  $form['settings']['urls_to_flush'] = array(
    '#type' => 'textarea',
    '#title' => t('Custom URLs to flush'),
    '#default_value' => $settings['urls_to_flush'],
    '#description' => t('Enter INTERNAL URLs, one URL per line'),
  );

  $form['settings']['caches_to_flush'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Choose which type of caches to flush'),
    '#default_value' => $settings['caches_to_flush'],
    '#options' => array(
      'drupal_cache' => t('Drupal cache'),
      'varnish_cache' => t('Varnish cache'),
    ),
  );
}

function wallyrules_flush_custom_page_cache_validate(&$form, &$form_state) {
  if ($form_state['values']['settings']['urls_to_flush'] == '') {
    form_set_error('urls_to_flush', 'Enter at least one URL');
  }
  foreach($form_state['values']['settings']['caches_to_flush'] as $cache_to_flush) {
    if ($cache_to_flush !== 0)
      return;
  }
  form_set_error('caches_to_flush', t('Select at least one cache to flush.'));
}

function wallyrules_flush_blocks_cache_form($settings, &$form, &$form_state) {  
  foreach (module_list() as $module) {
    $module_blocks = module_invoke($module, 'block', 'list');
    if (isset($module_blocks) && !empty($module_blocks)) {
      $blocks_options = array();
      foreach ($module_blocks as $module_id => $module_block) {
        if (is_numeric($module_id))
          $module_id = 'id_'.$module_id;
        $blocks_options[$module_id] = $module_block['info'];
      }
      
      $form['settings']['blocks_to_flush'][$module] = array(
        '#type' => 'checkboxes',
        '#title' => $module,
        '#default_value' => isset($settings['blocks_to_flush'][$module]) ? $settings['blocks_to_flush'][$module] : array(),
        '#options' => $blocks_options,
      );
    }
  }
}

function wallyrules_flush_blocks_cache_validate(&$form, &$form_state) {
  foreach($form_state['values']['settings']['blocks_to_flush'] as $module_to_flush) {
    foreach ($module_to_flush as $block_to_flush) {
      if ($block_to_flush !== 0)
        return;
    }
  }
  
  form_set_error('caches_to_flush', t('Select at least one block to flush.'));
}
