<?php 

function wallyadmin_theme_registry_form($form_state) {
  $redirect = isset($_GET['destination']) ? $_GET['destination'] : 'admin';
  $question = t('Are you sure you want to rebuild theme registry?');
  $form = confirm_form(array(), $question, $redirect);
  $form['#redirect'] = $redirect;
  return $form;
}

function wallyadmin_theme_registry_form_submit($form, $form_state) {
  wallyadmin_rebuild_theme_registry();
}

/**
 * Ability to rebuild theme registry even if devel module is not installed (i.e. in production).
 */
function wallyadmin_rebuild_theme_registry() {
  drupal_rebuild_theme_registry();
  drupal_set_message('Theme registry rebuild.');
}
