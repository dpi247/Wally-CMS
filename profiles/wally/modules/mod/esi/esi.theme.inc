<?php

/**
 * @file
 *   Theme handlers for the ESI module.
 */

/**
 * Replacement for core theme function: thene_blocks
 */
function esi__theme_blocks($region) {
  $output = '';

  // load our helper file.
  require_once(drupal_get_path('module', 'esi') . '/esi.inc');

  if ($list = block_list($region)) {
    foreach ($list as $key => $block) {
      // if ESI's enabled on the block, add an ESI tag instead of block content.
      $config = esi_get_settings($block->module . '_' . $block->delta);
      $output .= ($config['scope'] && variable_get('esi_mode', ESI_MODE))
        ? theme('esi_tag', 'block', $block)
        : theme('block', $block);
    }
  }

  // Add any content assigned to this region through drupal_set_content() calls.
  $output .= drupal_get_content($region);

  return $output;
}

function esi_theme_block() {
  $args = func_get_args();
  array_unshift($args, 'esi_alt_block');
  return call_user_func_array('esi_theme_callback_logic', $args);
}

function esi_theme_panels_pane() {
  $args = func_get_args();
  array_unshift($args, 'esi_alt_panels_pane');
  return call_user_func_array('esi_theme_callback_logic', $args);
}

function esi_theme_panels_stylizer_stylizer_style_render_pane() {
  $args = func_get_args();
  array_unshift($args, 'esi_alt_panels_stylizer_stylizer_style_render_pane');
  return call_user_func_array('esi_theme_callback_logic', $args);
}

function esi_theme_panels_rounded_corners_style_render_pane() {
  $args = func_get_args();
  array_unshift($args, 'esi_alt_panels_rounded_corners_style_render_pane');
  return call_user_func_array('esi_theme_callback_logic', $args);
}

function esi_theme_callback_logic() {
  $args = func_get_args();
  if (isset($args[1]->content) && esi_theme_is_esied($args[1]->content)) {
    return $args[1]->content;
  }
  return call_user_func_array('theme', $args);
}

/**
 * Create the ESI-tag for a particular block.
 */
function theme_esi_tag($type, $data) {
  global $theme_key, $base_root, $base_path;
  require_once(drupal_get_path('module', 'esi') . '/esi.inc');
  $added_to_url = array();

  if ($type == 'block') {
    $src = "esi/block/{$theme_key}:{$data->region}:{$data->module}:{$data->delta}";
    $css_id = "{$theme_key}-{$data->region}-{$data->module}-{$data->delta}";
    $config = esi_get_settings($data->module . '_' . $data->delta);
    $scope = $config['scope'];
  }
  elseif ($type == 'panel') {
    // Build the esi tag.
    $src = "esi/panels_pane/{$theme_key}:{$data->did}:{$data->pid}";
    $css_id = "{$theme_key}-{$data->did}-{$data->pid}";
    $scope = isset($data->cache['settings']['scope']) ? $data->cache['settings']['scope'] : (int)variable_get('esi_panel_default_scope', ESI_PANEL_DEFAULT_SCOPE);
    if (!empty($data->configuration['context']) && is_string($data->configuration['context'])) {
      $display = panels_get_current_page_display();
      // The cache-key variable is set by the panel-context render function.
      list($context, $task_name, $handler_name) = explode(':', $display->cache_key);

      // Add the URL the panel was requested from.
      $src .= '/' . base64_encode($_GET['q']);
      $added_to_url['page'] = TRUE;

      // Add the page-manager task name.
      $src .= '/' . $task_name;
      $added_to_url['task'] = TRUE;

      // Add the name of the context which is supplied to this pane.
      $src .= '/' . $data->configuration['context'];
      $added_to_url['context'] = TRUE;
    }
  }
  if ($type == 'external') {
    $src = $data->cache['settings']['external_text'];
    if (!empty($data->cache['settings']['context'])) {
      $full = base64_encode($base_root . request_uri());
      $internal = empty($_GET['q']) ? variable_get('site_frontpage', 'node') : $_GET['q'];
      $internal = base64_encode($internal);
      $parsed = parse_url($src);
      $glue_string = empty($parsed['query']) ? '?' : '&';
      $src .= $glue_string . 'esi_full=' . $full . '&esi_internal=' . $internal;
      $added_to_url['page'] = TRUE;
      $added_to_url['fullpage'] = TRUE;
    }
    $css_id = "{$theme_key}-{$data->did}-{$data->pid}";
  }

  $user_cache = '/CACHE=USER';
  $role_cache = '/CACHE=ROLE';
  $no_cache = '/CACHE=NOCACHE';
  
  if (variable_get('esi_role_user_direct_injection', ESI_ROLE_USER_DIRECT_INJECTION)) {
    $role_cache = '/CACHE=' . _esi__get_roles_hash();
    $user_cache = '/CACHE=' . esi_get_user_hash();
  }

  if (!empty($scope)) {
    switch ($scope) {
      case 0:
      	break;
     	// No Cache 	
      case 1:
        $src .= $no_cache;
        $added_to_url['cache'] = TRUE;
        break;
      // Global Scope.
      case 2:
        break;

      // Page Scope.
      case 3:
        if (empty($added_to_url['page'])) {
          $src .= '/' . base64_encode($_GET['q']);
          $added_to_url['page'] = TRUE;
        }
        break;

      // User Role.
      case 4:
        $src .= $role_cache;
        $added_to_url['cache'] = TRUE;
        break;

      // User Role/Page.
      case 5:
        if (empty($added_to_url['page'])) {
          $src .= '/' . base64_encode($_GET['q']);
          $added_to_url['page'] = TRUE;
        }
        $src .= $role_cache;
        $added_to_url['cache'] = TRUE;
        break;

      // User ID.
      case 6:
        $src .= $user_cache;
        $added_to_url['cache'] = TRUE;
        break;

      // User ID/Page.
      case 7:
        if (empty($added_to_url['page'])) {
          $src .= '/' . base64_encode($_GET['q']);
          $added_to_url['page'] = TRUE;
        }
        $src .= $user_cache;
        $added_to_url['cache'] = TRUE;
        break;
    }
  }

  $ajax_src = esi_build_uri($src);
  $fragments = @parse_url($src);
  if (!isset($fragments['host'])) {
    $src = $base_path . $src;
  }

  $setting = variable_get('esi_mode', ESI_MODE);
  switch ($setting) {
    case ESI__CONFIG_ESI:
      $output = '<esi:include src="' . $src . '" />';
      if (!empty($css_id) && variable_get('esi_ajax_fallback', ESI_AJAX_FALLBACK)) {
        drupal_add_js(array('esi' => array($css_id => $ajax_src)), 'setting');
        $output .= '<esi:remove><div id="' . $css_id . '" class="esi-ajax"></div></esi:remove>';
      }
      break;

    case ESI__CONFIG_SSI:
      $output ='<!--# include virtual="' . $src . '" -->';
      break;

    case ESI__CONFIG_AJAX:
      if (!empty($css_id)) {
        drupal_add_js(array('esi' => array($css_id => $ajax_src)), 'setting');
        $output = '<div id="' . $css_id . '" class="esi-ajax"></div>';
      }
      break;
  }
  return $output;
}
