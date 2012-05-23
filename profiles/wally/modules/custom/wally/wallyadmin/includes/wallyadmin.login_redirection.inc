<?php

function wallyadmin_page_form_login_redirection_form ($form_state) {
  $form = array();
  
  $roles = user_roles();
  foreach ($roles as $rid => $rlabel) {
    if ($rlabel != 'anonymous user' && $rlabel != 'authenticated user') {
      $form['wallyadmin_redirection_'.$rid] = array(
        '#type' => 'textfield',
        '#title' => $rlabel,
        '#description' => t('Enter the redirection path to use for this role login'),
        '#default_value' => wally_variable_get('wallyadmin_redirection_'.$rid, ''),
      );
    }
  }
  
  return wally_system_settings_form($form, TRUE);
}
