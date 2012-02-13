<?php
function _wallyesi__ctoolscontenttype_settings($type, $subtype, $pid,$config = NULL) {
  $key = "ctoolscontenttype_{$type}_{$subtype}_{$pid}";
  
  $global = variable_get('esi_ctoolscontenttype_config', array());
  if($config) {
    $global[$key] = $config;
    variable_set('esi_ctoolscontenttype_config', $global);
  }
  elseif(array_key_exists($key, $global)) {
    $config = $global[$key];
  }
  else {
    $config = new stdClass;
    $config->enabled = 0;
    $config->ttl     = 300;
  }
  return $config;
}

/**
 * Form-submit handler for ESI settings in block-config
 */
function wallyesi__ctoolscontenttype_config_save($form, $form_state) {
  //drupal_set_message('ESI settings saved');
  $type = $form_state['pane']->type;
  $subtype  = $form_state['pane']->subtype;

  $config = new stdClass;
  $config->enabled = $form_state['values']['esi_config']['enabled'];
  $config->ttl     = $form_state['values']['esi_config']['ttl'];

  _wallyesi__ctoolscontenttype_settings($type, $subtype,$form_state['pane']->pid, $config);
}


