<?php
// $Id: media_ustream.variables.inc,v 1.0 2011/01/07 ODM $

/**
 * @file Contains the variables and defaults used by Media: YouTube.
 */

/**
 * The variable namespace for Media: ustream.
 */
define('MEDIA_ustream_NAMESPACE', 'media_ustream__');

/**
 * This is the main URL for ustream.
 */
define('MEDIA_ustream_MAIN_URL', 'http://www.ustream.com/');

/**
 * This is the URL to the API of ustream.
 */

//define('MEDIA_ustream_API_INFO', 'http://developers.ustream.com/');
//define('MEDIA_ustream_API_APPLICATION_URL', 'http://developers.ustream.com/account.php');

/**
 * This defines the version of the content data array that we serialize
 * in emvideo_ustream_data(). If we change the expected keys of that array,
 * we must increment this value, which will allow older content to be updated
 * to the new version automatically.
 */
define('MEDIA_ustream_DATA_VERSION', 4);

define('MEDIA_ustream_STATUS_UPDATE_NONE', 0);
define('MEDIA_ustream_STATUS_UPDATE_FROM_UNAVAILABLE', 1);
define('MEDIA_ustream_STATUS_UPDATE_ALWAYS', 2);

define('MEDIA_ustream_STATUS_UNAVAILABLE', 0x0000);
define('MEDIA_ustream_STATUS_AVAILABLE', 0x0001);
define('MEDIA_ustream_STATUS_PRIVATE', 0x0002);

/**
 * Wrapper for variable_get() that uses the Media: ustream variable registry.
 *
 * @param string $name
 *  The variable name to retrieve. Note that it will be namespaced by
 *  pre-pending MEDIA_ustream_NAMESPACE, as to avoid variable collisions
 *  with other modules.
 * @param unknown $default
 *  An optional default variable to return if the variable hasn't been set
 *  yet. Note that within this module, all variables should already be set
 *  in the media_ustream_variable_default() function.
 * @return unknown
 *  Returns the stored variable or its default.
 *
 * @see media_ustream_variable_set()
 * @see media_ustream_variable_del()
 * @see media_ustream_variable_default()
 */
function media_ustream_variable_get($name, $default = NULL) {
  // Allow for an override of the default.
  // Useful when a variable is required (like $path), but namespacing still desired.
  if (!isset($default)) {
    $default = media_ustream_variable_default($name);
  }
  // Namespace all variables
  $variable_name = MEDIA_ustream_NAMESPACE . $name;
  return variable_get($variable_name, $default);
}

/**
 * Wrapper for variable_set() that uses the Media: ustream variable registry.
 *
 * @param string $name
 *  The variable name to set. Note that it will be namespaced by
 *  pre-pending MEDIA_ustream_NAMESPACE, as to avoid variable collisions with
 *  other modules.
 * @param unknown $value
 *  The value for which to set the variable.
 * @return unknown
 *  Returns the stored variable after setting.
 *
 * @see media_ustream_variable_get()
 * @see media_ustream_variable_del()
 * @see media_ustream_variable_default()
 */
function media_ustream_variable_set($name, $value) {
  $variable_name = MEDIA_ustream_NAMESPACE . $name;
  return variable_set($variable_name, $value);
}

/**
 * Wrapper for variable_del() that uses the Media: ustream variable registry.
 *
 * @param string $name
 *  The variable name to delete. Note that it will be namespaced by
 *  pre-pending MEDIA_ustream_NAMESPACE, as to avoid variable collisions with
 *  other modules.
 *
 * @see media_ustream_variable_get()
 * @see media_ustream_variable_set()
 * @see media_ustream_variable_default()
 */
function media_ustream_variable_del($name) {
  $variable_name = MEDIA_ustream_NAMESPACE . $name;
  variable_del($variable_name);
}

/**
 * The default variables within the Media: ustream namespace.
 *
 * @param string $name
 *  Optional variable name to retrieve the default. Note that it has not yet
 *  been pre-pended with the MEDIA_ustream_NAMESPACE namespace at this time.
 * @return unknown
 *  The default value of this variable, if it's been set, or NULL, unless
 *  $name is NULL, in which case we return an array of all default values.
 *
 * @see media_ustream_variable_get()
 */
function media_ustream_variable_default($name = NULL) {
  static $defaults;

  if (!isset($defaults)) {
    $defaults = array(
      // The ustream API developer key.
      'api_key' => '',

      // Default ustream user & password.
      'ustream_username' => '',
      'ustream_password' => '',

      // Use Longtail/JW FLV Media Player to display videos?
      'use_jw_flv' => FALSE,

      // Zend path.
      'zend_path' => '',

      // Status message to display when a video is unavailable.
      'status_display_'. EMFIELD_STATUS_UNAVAILABLE => '',

      // Status message to display for unavailable duplicate videos.
      'status_display_duplicates' => '',
      'check_for_duplicates' => FALSE,

      // Status message to display for rejected videos.
      'status_display_rejected' => '',
      'check_for_rejected' => FALSE,

      // Status message to display for failed video uploads.
      'status_display_upload_failed' => '',
      'check_for_failed' => FALSE,

      'cron_time' => 10,
      'cron_limit' => 50,
      
      // "textarea" of the allowed URLs from which the videos may come.
      'allowed_url' => '',
    );
  }

  if (!isset($name)) {
    return $defaults;
  }

  if (isset($defaults[$name])) {
    return $defaults[$name];
  }
}

/**
 * Return the fully namespace variable name.
 *
 * @param string $name
 *  The variable name to retrieve the namespaced name.
 * @return string
 *  The fully namespace variable name, prepended with
 *  MEDIA_ustream_NAMESPACE.
 */
function media_ustream_variable_name($name) {
  return MEDIA_ustream_NAMESPACE . $name;
}
