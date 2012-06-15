<?php

/**
 * @file
 *   Helper functions for the ESI module.
 */

/**
 * Get the current seed key.
 */
function _esi__get_seed_key() {
  $seed = variable_get('esi_seed_key', FALSE);
  if (!$seed) {
    $seed = _esi__rotate_seed_key();
  }
  return $seed;
}

/**
 * Rotate the seed key.
 */
function _esi__rotate_seed_key() {
  $seed = _esi__get_random_seed();
  variable_set('esi_seed_key', $seed);
  variable_set('esi_seed_key_last_changed', time());
  return $seed;
}

/**
 * Get a random 32-character string
 */
function _esi__get_random_seed() {
  $seed = '';
  for ($i=0; $i<32; $i++) {
    // get a random character from the printable ASCII character set: 32-176
    $seed += chr(mt_rand(32, 176));
  }
  return $seed;
}

/**
 * Get the hash for a set of roles.
 *
 * @param array $rids
 *   An array of role-ids
 */
function _esi__get_roles_hash($rids = NULL) {
  static $roles = array();

  // Get all roles.
  if (empty($rids)) {
    global $user;
    $rids = !empty($user->roles) ? array_keys($user->roles) : 0;
  }
  // Get roles saved from admin/settings/esi.
  $include_roles = array_filter(variable_get('esi_include_roles', array()));
  if (!empty($include_roles)) {
    $included_rids = array_intersect($rids, $include_roles);
  }
  else {
    $included_rids = $rids;
  }

  // Create a hash of the roles.
  $hash = implode(':', $included_rids);
  if (!isset($roles[$hash])) {
    $seed = _esi__get_seed_key();
    $roles[$hash] = md5($seed . md5($hash));
    // Invloke hook_esi_roles_hash().
    drupal_alter('esi_roles_hash', $roles[$hash], $included_rids, $rids, $seed);
  }
  return $roles[$hash];
}

/**
 * Get the hash for a user.
 *
 * @param int $uid
 *   User ID.
 */
function esi_get_user_hash($uid = NULL) {
  static $users = array();

  // Get current user if uid was not passed in.
  if (empty($uid)) {
    global $user;
    $uid = isset($user->uid) ? $user->uid : 0;
  }

  // Create a hash of the user.
  if (!isset($users[$uid])) {
    $seed = _esi__get_seed_key();
    $users[$uid] = md5($seed . md5($uid));
    // Invloke hook_esi_user_hash().
    drupal_alter('esi_user_hash', $users[$uid], $uid, $seed);
  }
  return $users[$uid];
}

/**
 * Get the themed HTML for a particular block
 *
 * @param $theme
 *   The name of the theme: "garland".
 * @param String $region
 *   The name of the region the block is in: e.g. "left"
 * @param String $module
 * @param String $delta
 */
function _esi__get_block($theme, $region, $module, $delta) {
  $block = db_fetch_object(db_query("SELECT * FROM {blocks} WHERE module = '%s' AND delta = '%s' AND theme = '%s'", $module, $delta, $theme));
  $block->context = $region;
  $block->status = 1;
  $array = module_invoke($block->module, 'block', 'view', $block->delta);
  if (isset($array) && is_array($array)) {
    foreach ($array as $k => $v) {
      $block->$k = $v;
    }
  }

  if (!empty($block->title)) {
    // Check plain here to allow module generated titles to keep any markup.
    $block->subject = $block->title == '<none>' ? '' : check_plain($block->title);
  }
  if (!isset($block->subject)) {
    $block->subject = '';
  }

  return $block;
}

/**
 * Get / Set the config for a particular block.
 *
 * @param $id
 *   The ID for the configuration.
 * @param $config
 *   (optinal) If this is set, it will save the settings passed in.
 * @return
 *   The saved configuration for the given ID.
 */
function esi_get_settings($id, $config = NULL) {
  // TODO: replace with something more suited to in-code config than variables.
  $settings = variable_get('esi_block_config', array());

  if ($config) {
    while (!lock_acquire('esi_block_config')) {
      lock_wait('esi_block_config');
    }
    // Now that we have a lock, grab the settings from the database.
    $settings = unserialize(db_result(db_query("SELECT value FROM {variable} WHERE name = 'esi_block_config'")));
    $settings[$id] = $config;
    variable_set('esi_block_config', $settings);
    lock_release('esi_block_config');
  }
  elseif (!empty($settings[$id])) {
    $config = $settings[$id];
  }

  // Set defaults.
  if (!is_array($config)) {
    $config = array();
  }
  if (!isset($config['max_age'])) {
    $config['max_age'] = (int)variable_get('esi_block_default_max_age', ESI_BLOCK_DEFAULT_MAX_AGE);
  }
  if (!isset($config['scope'])) {
    $config['scope'] = (int)variable_get('esi_block_default_scope', ESI_BLOCK_DEFAULT_SCOPE);
  }

  // Return the configuration for this ID.
  return $config;
}

/**
 * Get a list of possible max age (ttl) choices.
 */
function esi_max_age_options($current_max_age) {
  $options = drupal_map_assoc(array(0, 5, 15, 30, 60, 120, 180, 240, 300, 600, 900, 1200, 1800, 3600, 7200, 14400, 28800, 43200, 64800, 86400, 86400*2, 86400*3, 86400*4, 86400*5, 86400*6, 86400*7), 'format_interval');

  // If the given max age isn't one of our options, add the current max age as a custom option.
  if (!isset($options[$current_max_age])) {
    $options[$current_max_age] = t('Custom: @time', array('@time' => format_interval($current_max_age)));
    ksort($options);
  }
  $options[0] = '<' . t('none') . '>';

  return $options;
}

/**
 * Checks what Drupal distribution is in use. Drupal, Pressflow, or Cocomore.
 *
 * Taken from cdn.requirements.inc.
 * @see _cdn_requirements_is_patched_distribution().
 *
 */
function _esi_get_drupal_distribution() {
  static $distribution;
  if (!empty($distribution)) {
    return $distribution;
  }

  $profile = realpath('.') . '/profiles/default/default.profile';
  if (!file_exists($profile)) {
    $distribution = 'drupal';
    return $distribution;
  }

  include_once $profile;
  $default = default_profile_details();
  if ($default['name'] == 'Pressflow') {
    $distribution = 'pressflow';
  }
  elseif (stristr($default['name'], 'cocomore')){
    $distribution = 'cocomore';
  }
  else {
    $distribution = 'drupal';
  }
  return $distribution;
}

function esi_build_uri($path) {
  static $hook_file_url_alter = array();

  // If the current path is an absolute path, return immediately.
  $fragments = parse_url($path);
  if (isset($fragments['host'])) {
    return $path;
  }

  // If Use a CDN for AJAXed fragments is disabled then do not CDN path.
  if (!variable_get('esi_cdn_ajax', ESI_CDN_AJAX)) {
    return base_path() . $path;
  }

  $original_path = $path;
  // CDN Support.
  if (module_exists('cdn')) {
    $status = variable_get(CDN_STATUS_VARIABLE, CDN_DISABLED);
    if ($status == CDN_ENABLED || ($status == CDN_TESTING && user_access(CDN_PERM_ACCESS_TESTING))) {
      // Alter URL when the file_create_url() patch is not there.
      if (variable_get(CDN_THEME_LAYER_FALLBACK_VARIABLE, FALSE)) {
        cdn_file_url_alter($path);

      }
      // Use the patched version of file_create_url().
      else {
        $path = file_create_url($path);
      }
      // Return here if the path was changed above.
      if (strcmp($original_path, $path) != 0) {
        return $path;
      }
    }
  }

  // Other modules besides CDN might want to use hook_file_url_alter.
  if (empty($hook_file_url_alter)) {
    $hook_file_url_alter = module_implements('file_url_alter');
  }
  if (!empty($hook_file_url_alter)) {
    $path = file_create_url($path);
    // Return here if the path was changed above.
    if (strcmp($original_path, $path) != 0) {
      return $path;
    }
  }

  // If nothing was altered then use the drupal default.
  return base_path() . $path;
}
