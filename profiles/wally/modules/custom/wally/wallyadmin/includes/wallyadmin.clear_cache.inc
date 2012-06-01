<?php

function wallyadmin_clear_cache_form($form_state) {
  $redirect = isset($_GET['destination']) ? $_GET['destination'] : 'admin';
  $question = t('Are you sure you want to clear all caches?');
  $form = confirm_form(array(), $question, $redirect);
  $form['#redirect'] = $redirect;
  return $form;
}

function wallyadmin_clear_cache_form_submit($form, $form_state) {
  wallyadmin_clear_cache();
}

/**
 * Ability to flush all caches even if devel module is not installed (i.e. in production).
 */
function wallyadmin_clear_cache() {
  drupal_flush_all_caches();
  drupal_set_message('Cache cleared.');
}
