<?php

/**
 * @file
 *   Admin settings for ESI.
 */

/**
 * Page callback function for the admin settings form.
 */
function esi_admin_page() {
  $output = '';
  $output .= drupal_get_form('esi_admin_settings_form');
  return $output;
}

/**
 * Admin settings form
 */
function esi_admin_settings_form() {
  $form = array();
  require_once(drupal_get_path('module', 'esi') . '/esi.inc');

  // Global Settings.
  $form['global'] = array(
    '#type' => 'fieldset',
    '#title' => t('Global Settings'),
  );
  $options = array(
    0 => 'Disabled',
    ESI__CONFIG_ESI => 'ESI',
    ESI__CONFIG_SSI => 'SSI',
    ESI__CONFIG_AJAX => 'AJAX',
  );
  $form['global']['esi_mode'] = array(
    '#title' => t('Mode'),
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => variable_get('esi_mode', ESI_MODE),
  );
  $form['global']['esi_ajax_fallback'] = array(
    '#title' => t('Use AJAX if ESI is disabled.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('esi_ajax_fallback', ESI_AJAX_FALLBACK),
  );
  // CDN module.
  $hook_file_url_alter = module_implements('file_url_alter');
  if (!empty($hook_file_url_alter)) {
    $form['global']['esi_cdn_ajax'] = array(
      '#title' => t('Use a CDN for AJAXed fragments.'),
      '#type' => 'checkbox',
      '#default_value' => variable_get('esi_cdn_ajax', ESI_CDN_AJAX),
    );
  }
  $form['global']['esi_page_cache'] = array(
    '#title' => t("Use Core's page cache to store ESI fragments."),
    '#type' => 'checkbox',
    '#default_value' => variable_get('esi_page_cache', ESI_PAGE_CACHE),
  );
  $seed = (int)variable_get('esi_seed_rotation_interval', ESI_SEED_ROTATION_INTERVAL);
  $form['global']['esi_seed_rotation_interval'] = array(
    '#title' => t('Default seed key rotation interval'),
    '#type' => 'select',
    '#options' => esi_max_age_options($seed),
    '#default_value' => $seed,
  );
  $form['global']['esi_role_user_direct_injection'] = array(
    '#title' => t('Place user & roles keys in the URL'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('esi_role_user_direct_injection', ESI_ROLE_USER_DIRECT_INJECTION),
    '#description' => t('By default CACHE=* will get replaced client side (if ajax) or server side (if using varnish rules). If dynamic replacement is not an option; this can be done when the page is generated.'),
  );

  // Block Settings.
  $form['blocks'] = array(
    '#type' => 'fieldset',
    '#title' => t('Block settings'),
  );
  $max_age = (int)variable_get('esi_block_default_max_age', ESI_BLOCK_DEFAULT_MAX_AGE);
  $form['blocks']['esi_block_default_max_age'] = array(
    '#title' => t('Cache Maximum Age (TTL)'),
    '#type' => 'select',
    '#options' => esi_max_age_options($max_age),
    '#default_value' => $max_age,
    '#description' => t('External page caches (proxy/browser) will not deliver cached paged older than this setting; time to live in short.'),
  );
  $form['blocks']['esi_block_default_scope'] = array(
    '#title'          => t('Default Block Cache Scope'),
    '#type'           => 'select',
    '#options'        => array(
      0 => 'Disabled',
      1 => 'Not Cached',
      2 => 'Global',
      3 => 'Page',
      4 => 'User Role',
      5 => 'User Role/Page',
      6 => 'User ID',
      7 => 'User ID/Page',
    ),
    '#default_value'  => variable_get('esi_block_default_scope', ESI_BLOCK_DEFAULT_SCOPE),
    '#description'    => t('Disabled - Do not use ESI. <br />Not Cached - Use ESI, but never cache the content. <br />Global - Content is same on every page. <br />Page - Content changes based on the URL. <br />User Role - Content changes based on the user role. <br />User Role/Page - Content changes based on the user role as well as the URL. <br />User ID - Content changes based on the UID; otherwise it is the same as global. <br />User ID/Page - Content changes based on the UID and based on the URL.'),
  );

  // Panel Settings.
  $form['panels'] = array(
    '#type' => 'fieldset',
    '#title' => t('Panels settings'),
  );
  $max_age = (int)variable_get('esi_panel_default_max_age', ESI_PANEL_DEFAULT_MAX_AGE);
  $form['panels']['esi_panel_default_max_age'] = array(
    '#title' => t('Cache Maximum Age (TTL)'),
    '#type' => 'select',
    '#options' => esi_max_age_options($max_age),
    '#default_value' => $max_age,
    '#description' => t('External page caches (proxy/browser) will not deliver cached paged older than this setting; time to live in short.'),
  );
  $form['panels']['esi_panel_default_scope'] = array(
    '#title'          => t('Default Panel Cache Scope'),
    '#type'           => 'select',
    '#options'        => array(
      0 => 'Disabled',
      1 => 'Not Cached',
      2 => 'Global',
      3 => 'Page',
      4 => 'User Role',
      5 => 'User Role/Page',
      6 => 'User ID',
      7 => 'User ID/Page',
    ),
    '#default_value'  => variable_get('esi_panel_default_scope', ESI_PANEL_DEFAULT_SCOPE),
    '#description'    => t('Disabled - Do not use ESI. <br />Not Cached - Use ESI, but never cache the content. <br />Global - Content is same on every page. <br />Page - Content changes based on the URL. <br />User Role - Content changes based on the user role. <br />User Role/Page - Content changes based on the user role as well as the URL. <br />User ID - Content changes based on the UID; otherwise it is the same as global. <br />User ID/Page - Content changes based on the UID and based on the URL.'),
  );

  // Roles config
  $form['roles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Role settings'),
  );
  $form['roles']['esi_include_roles'] = array(
    '#title' => t('Roles included in User Role scope'),
    '#type' => 'checkboxes',
    '#options' => user_roles(FALSE),
    '#default_value' => variable_get('esi_include_roles', array()),
    '#description' => t('Choose the roles that will be included in the roles hash. Only select roles that may affect caching. If you select no roles, all roles will be candidates.'),
  );

  // Cache Flusing
  $form['cache_page'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Clear ESI Cache (cache_page cache)'),
    '#description'  => t('This only clears out the cache in the cache_page cache. Varnish and browser caches are not cleared by this action.'),
  );
  $form['cache_page']['flush'] = array(
    '#type'   => 'submit',
    '#value'  => t('Flush ESI: cache_page Cache'),
    '#submit' => array('esi_admin_flush_cache_button'),
  );

  $widget_vis = "
$(esi_update_visibility);
$(function(){ $('#edit-esi-mode-0-wrapper').parent().change(function (){esi_update_visibility();})});
function esi_update_visibility() {
  var block_age_fieldset = $('#edit-esi-block-default-max-age').parents('fieldset');
  var panel_age_fieldset = $('#edit-esi-panel-default-max-age').parents('fieldset');
  // Disabled.
  if ($('#edit-esi-mode-0:checked').val() !== undefined) {
    $('#edit-esi-ajax-fallback-wrapper').hide();
    esi_update_cdn_ajax_visibility();
    $('#edit-esi-page-cache-wrapper').hide();
    $('#edit-esi-seed-rotation-interval-wrapper').hide();
    $('#edit-esi-role-user-direct-injection-wrapper').hide();
    $(block_age_fieldset[0]).hide();
    $(panel_age_fieldset[0]).hide();
    esi_update_cache_visibility();
  }
  // ESI.
  else if ($('#edit-esi-mode-1:checked').val() !== undefined) {
    $('#edit-esi-ajax-fallback-wrapper').show();
    esi_update_cdn_ajax_visibility();
    $('#edit-esi-page-cache-wrapper').show();
    $('#edit-esi-seed-rotation-interval-wrapper').show();
    $('#edit-esi-role-user-direct-injection-wrapper').show();
    $(block_age_fieldset[0]).show();
    $(panel_age_fieldset[0]).show();
    esi_update_cache_visibility();
  }
  // SSI.
  else if ($('#edit-esi-mode-2:checked').val() !== undefined) {
    $('#edit-esi-ajax-fallback-wrapper').hide();
    esi_update_cdn_ajax_visibility();
    $('#edit-esi-page-cache-wrapper').show();
    $('#edit-esi-seed-rotation-interval-wrapper').show();
    $('#edit-esi-role-user-direct-injection-wrapper').show();
    $(block_age_fieldset[0]).show();
    $(panel_age_fieldset[0]).show();
    esi_update_cache_visibility();
  }
  // AJAX.
  else if ($('#edit-esi-mode-3:checked').val() !== undefined) {
    $('#edit-esi-ajax-fallback-wrapper').hide();
    esi_update_cdn_ajax_visibility();
    $('#edit-esi-page-cache-wrapper').show();
    $('#edit-esi-seed-rotation-interval-wrapper').show();
    $('#edit-esi-role-user-direct-injection-wrapper').show();
    $(block_age_fieldset[0]).show();
    $(panel_age_fieldset[0]).show();
    esi_update_cache_visibility();
  }
}

$(esi_update_cache_visibility);
$(function(){ $('#edit-esi-page-cache').change(function (){esi_update_cache_visibility();})});
function esi_update_cache_visibility() {
  var page_cache_fieldset = $('#edit-flush').parents('fieldset');
  // Not Disabled and Using the Page cache.
  if ($('#edit-esi-mode-0:checked').val() === undefined && $('#edit-esi-page-cache:checked').val() !== undefined) {
    $(page_cache_fieldset[0]).show();
  }
  else {
    $(page_cache_fieldset[0]).hide();
  }
}

$(esi_update_cdn_ajax_visibility);
$(function(){ $('#edit-esi-ajax-fallback').change(function (){esi_update_cdn_ajax_visibility();})});
function esi_update_cdn_ajax_visibility() {
  // AJAX mode or ESI mode with AJAX fallback checked.
  if ($('#edit-esi-mode-3:checked').val() !== undefined || ($('#edit-esi-mode-1:checked').val() !== undefined && $('#edit-esi-ajax-fallback:checked').val() !== undefined)) {
    $('#edit-esi-cdn-ajax-wrapper').show();
  }
  else {
    $('#edit-esi-cdn-ajax-wrapper').hide();
  }
}
";
  drupal_add_js($widget_vis, 'inline');

  $form['#submit'][] = 'esi_admin_settings_form_submit';
  return system_settings_form($form);
}

function esi_admin_settings_form_validate($form, &$form_state) {
  // Remove non variable form info.
  unset($form_state['values']['flush']);
}

/**
 * Clear the theme cache if the ESI Mode changed.
 */
function esi_admin_settings_form_submit($form, &$form_state) {
  global $conf;

  if ($conf['esi_mode'] != $form_state['values']['esi_mode']) {
    cache_clear_all('esi:', 'cache_page', TRUE);
    cache_clear_all('esi_esi_cache:', 'cache', TRUE);
    module_invoke('system', 'theme_data');
    drupal_rebuild_theme_registry();
  }
  elseif ($conf['esi_page_cache'] != $form_state['values']['esi_page_cache']) {
    cache_clear_all('esi:', 'cache_page', TRUE);
  }

}

/**
 * Cache clear button.
 */
function esi_admin_flush_cache_button() {
  cache_clear_all('esi:', 'cache_page', TRUE);
  drupal_set_message(t("ESI's drupal cache (cache_page table) cleared."));
}

/**
 * Cache clear callback for admin_menu/flush-cache/esi.
 */
function esi_admin_flush_cache() {
  esi_admin_flush_cache_button();
  drupal_goto();
}
