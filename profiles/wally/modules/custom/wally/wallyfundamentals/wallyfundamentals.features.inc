<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function wallyfundamentals_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "wallyctools" && $api == "wallyctools_redacblock") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_views_api().
 */
function wallyfundamentals_views_api() {
  return array(
    'api' => '3',
  );
}
