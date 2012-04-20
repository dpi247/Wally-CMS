<?php

function wallyctools_page_reorder_settings_form($form_state) {
  $form = array();
  
  $form['wallyctools_reorder_check_conflict'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check conflicts before applying reorder'),
    '#default_value' => variable_get('wallyctools_reorder_check_conflict', 1),
  );
  
  return system_settings_form($form);
}

function wallyctools_page_destinations_tree_caching_form($form_state) {
  $form = array();
  
  $form['refresh_cache'] = array(
    '#type' => 'submit',
    '#value' => t('Force refresh of the cached destination tree'),
  );
  
  return $form;
}

function wallyctools_page_destinations_tree_caching_form_submit($form, $form_state) {
  $dest_vid = variable_get('wallymport_destinationpath', 0);
  $depth = 0;
  if (wallyctools_build_term_handler_mapping($dest_vid, $depth, TRUE)) {
    drupal_set_message(t('Cached destination tree rebuild'));
  } else {
    drupal_set_message(t('An error occurred'), 'error');
  }
}
