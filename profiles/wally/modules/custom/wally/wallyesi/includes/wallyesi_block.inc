<?php

/**
 * Form-submit handler for ESI settings in block-config
 */
function wallyesi__block_config_save($form, $form_state) {
  drupal_set_message('ESI settings saved');
  $module = $form_state['values']['module'];
  $delta  = $form_state['values']['delta'];
  
  $config = new stdClass;
  $config->enabled = $form_state['values']['esi_config']['enabled'];
  $config->ttl     = $form_state['values']['esi_config']['ttl'];
  _wallyesi__block_settings($module, $delta, $config);
}

function _wallyesi__block_settings($module, $delta, $config = NULL) {
  $key = "block_{$module}_{$delta}";
  $global = variable_get('esi_block_config', array());
  if($config) {
    $global[$key] = $config;
    variable_set('esi_block_config', $global);
  }
  elseif(array_key_exists($key, $global)) {
    $config = $global[$key];
  }
  else {
    $config = new stdClass;
    $config->enabled = ESI__BLOCK_CONFIG_DEFAULT__IS_ENABLED;
    $config->ttl     = ESI__BLOCK_CONFIG_DEFAULT__TTL;
  }

  return $config;
}
