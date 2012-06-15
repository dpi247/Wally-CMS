<?php

/**
 * @file
 * ESI cache plugin.  Substitutes esi-tags for a panel-pane's content.
 */

// Plugin definition
$plugin = array(
  'title' => t("ESI"),
  'description' => t('ESI caching is a proxy-based cache. Panes are replaced by <esi> tags and requested separately by the proxy.'),
  'cache get' => 'esi_esi_cache_get_cache',
  'cache set' => 'esi_esi_cache_set_cache',
  'cache clear' => 'esi_esi_cache_clear_cache',
  'settings form' => 'esi_esi_cache_settings_form',
  'settings form submit' => 'esi_esi_cache_settings_form_submit',
  'defaults' => array(
    // TODO: review settings.
    'max_age' => (int)variable_get('esi_panel_default_max_age', ESI_PANEL_DEFAULT_MAX_AGE),
    'scope' => (int)variable_get('esi_panel_default_scope', ESI_PANEL_DEFAULT_SCOPE),
  ),
);

/**
 * Get cached content.
 */
function esi_esi_cache_get_cache($conf, $display, $args, $contexts, $pane = NULL) {
  // Bail out if ESI is disabled OR if pane's cache method is not ESI.
  if (!variable_get('esi_mode', ESI_MODE) || (isset($pane) && (empty($pane->cache['method']) || $pane->cache['method'] != 'esi'))) {
    return FALSE;
  }

  // Generate the cached object without using cache_get().
  $cache = new stdClass;
  $cache->data = new panels_cache_object();
  $cache->data->content = new stdClass;
  $cache->data->head = NULL;
  $cache->data->css = array();
  $cache->data->js = array();
  $cache->data->tokens = array();
  $cache->data->ready = TRUE;

  // Fill in the content with the ESI code
  $mode = $pane->cache['settings']['external'] === 1 ? 'external' : 'panel';
  $cache->data->content->content = theme('esi_tag', $mode, $pane);

  // Add in extras if missing.
  if (!isset($cache->data->content->module)) {
    $cache->data->content->module = isset($pane->type) ? $pane->type : 'esi';
  }
  if (!isset($cache->data->content->delta)) {
    $cache->data->content->delta = isset($pane->subtype) ? $pane->subtype : 'cache';
  }

  // Return the cache object.
  return $cache->data;
}

/**
 * Set cached content.
 */
function esi_esi_cache_set_cache($conf, $content, $display, $args, $contexts, $pane = NULL) {
}

/**
 * Clear cached content.
 *
 * Cache clears are always for an entire display, regardless of arguments.
 */
function esi_esi_cache_clear_cache($display) {
  // TODO: wipe the varnish cache.
  cache_clear_all('esi:', 'cache_page', TRUE);
  cache_clear_all('esi_esi_cache:', 'cache', TRUE);
}

/**
 * Figure out an id for our cache based upon input and settings.
 */
function esi_esi_cache_get_id($conf, $display, $args, $contexts, $pane) {
  // Build the cache-key using display ID, pane ID and pane context (if set).
  $key = array();
  $key[] = 'esi_esi_cache';
  $key[] = isset($display->id) ? $display->id : isset($display->css_id) ? $display->css_id : $display->did;
  $key[] = $pane->pid;
  if (!empty($pane->configuration['context']) && is_string($pane->configuration['context'])) {
    $key[] = $pane->configuration['context'];
  }

  return implode(':', $key);
}

function esi_esi_cache_settings_form($conf, $display, $pid) {
  // The ESI callback URL provides the display ID, pane ID, and the string
  // identifier for the context passed to the pane.
  // E.g. /esi/panel_panes/21/2
  // E.g. /esi/panel_panes/21/3/argument_current_page_content_1

  ctools_include('dependent');
  require_once(drupal_get_path('module', 'esi') . '/esi.inc');

  // The contexts-definition for this pane is in $display->configuration['context']
  $form['scope'] = array(
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
    '#default_value'  => isset($conf['scope']) ? $conf['scope'] : (int)variable_get('esi_panel_default_scope', ESI_PANEL_DEFAULT_SCOPE),
    '#description'    => t('Disabled - Do not use ESI. <br />Not Cached - Use ESI, but never cache the content. <br />Global - Content is same on every page. <br />Page - Content changes based on the URL. <br />User Role - Content changes based on the user role. <br />User Role/Page - Content changes based on the user role as well as the URL. <br />User ID - Content changes based on the UID; otherwise it is the same as global. <br />User ID/Page - Content changes based on the UID and based on the URL.'),
  );

  $max_age = isset($conf['max_age']) ? $conf['max_age'] : (int)variable_get('esi_panel_default_max_age', ESI_PANEL_DEFAULT_MAX_AGE);
  $form['max_age'] = array(
    '#title' => t('Cache Maximum Age (TTL)'),
    '#type' => 'select',
    '#options' => esi_max_age_options($max_age),
    '#default_value' => $max_age,
    '#description' => t('External page caches (proxy/browser) will not deliver cached paged older than this setting; time to live in short.'),
  );
  $form['aligner_start'] = array('#value' => '<div class="option-text-aligner">');
  $form['external'] = array(
    '#type' => 'checkbox',
    '#default_value' => isset($conf['external']) ? $conf['external'] : '',
    '#title' => t('External Link'),
    '#id' => 'external-checkbox',
  );
  $form['external_text'] = array(
    '#type' => 'textfield',
    '#default_value' => isset($conf['external_text']) ? $conf['external_text'] : '',
    '#size' => 80,
    '#id' => 'external-textfield',
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('external-checkbox' => array(1)),
    '#dependency_type' => 'disable',
  );
  $form['aligner_stop'] = array('#value' => '</div><div style="clear: both; padding: 0; margin: 0"></div>');
  $form['context'] = array(
    '#type' => 'checkbox',
    '#default_value' => isset($conf['context']) ? $conf['context'] : '',
    '#title' => t('Pass the current url to the external url as a base64 encoded string.'),
    '#id' => 'context-checkbox',
  );

  // Add some JS inline.
  $widget_vis = '
<script type="text/javascript">
<!--//--><![CDATA[//><!--' . "
function esi_update_visibility() {
  var esi_scope = $('#edit-settings-scope').val();
  if (esi_scope == '0' || esi_scope == '1') {
    $('#edit-settings-max-age-wrapper').hide();
  }
  else {
    $('#edit-settings-max-age-wrapper').show();
  }
}
$(esi_update_visibility);
$(function(){ $('#edit-settings-scope').change(function (){esi_update_visibility();})});
//--><!]]>
</script>";
  $form['js'] = array(
    '#type' => 'markup',
    '#value' => $widget_vis,
  );

  return $form;
}
