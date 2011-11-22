<?php
// $Id: media_googledoc.variables.inc $

/**
 * @file media_googledoc/includes/media_googledoc.variables.inc
 * Contains the variables and defaults used by Media: googledoc.
 */

/**
 * The variable namespace for Media: googledoc.
 */
define('MEDIA_GOOGLEDOC_NAMESPACE', 'media_googledoc__');

/**
 * This is the main URL for GOOGLEDOC.
 */
define('MEDIA_GOOGLEDOC_MAIN_URL', 'https://spreadsheets.google.com/');

/**
 * Wrapper for variable_get() that uses the Media: GOOGLEDOC variable registry.
 *
 * @param string $name
 *  The variable name to retrieve. Note that it will be namespaced by
 *  pre-pending MEDIA_GOOGLEDOC_NAMESPACE, as to avoid variable collisions
 *  with other modules.
 * @param unknown $default
 *  An optional default variable to return if the variable hasn't been set
 *  yet. Note that within this module, all variables should already be set
 *  in the media_GOOGLEDOC_variable_default() function.
 * @return unknown
 *  Returns the stored variable or its default.
 *
 * @see media_GOOGLEDOC_variable_set()
 * @see media_GOOGLEDOC_variable_del()
 * @see media_GOOGLEDOC_variable_default()
 */
function media_googledoc_variable_get($name, $default = NULL) {
  // Allow for an override of the default.
  // Useful when a variable is required (like $path), but namespacing still desired.
  if (!isset($default)) {
    $default = media_googledoc_variable_default($name);
  }
  // Namespace all variables
  $variable_name = MEDIA_GOOGLEDOC_NAMESPACE . $name;
  return variable_get($variable_name, $default);
}

/**
 * Wrapper for variable_set() that uses the Media: googledoc variable registry.
 *
 * @param string $name
 *  The variable name to set. Note that it will be namespaced by
 *  pre-pending MEDIA_googledoc_NAMESPACE, as to avoid variable collisions with
 *  other modules.
 * @param unknown $value
 *  The value for which to set the variable.
 * @return unknown
 *  Returns the stored variable after setting.
 *
 * @see media_googledoc_variable_get()
 * @see media_googledoc_variable_del()
 * @see media_googledoc_variable_default()
 */
function media_googledoc_variable_set($name, $value) {
  $variable_name = MEDIA_GOOGLEDOC_NAMESPACE . $name;
  return variable_set($variable_name, $value);
}

/**
 * Wrapper for variable_del() that uses the Media: googledoc variable registry.
 *
 * @param string $name
 *  The variable name to delete. Note that it will be namespaced by
 *  pre-pending MEDIA_GOOGLEDOC_NAMESPACE, as to avoid variable collisions with
 *  other modules.
 *
 * @see media_googledoc_variable_get()
 * @see media_googledoc_variable_set()
 * @see media_googledoc_variable_default()
 */
function media_googledoc_variable_del($name) {
  $variable_name = MEDIA_GOOGLEDOC_NAMESPACE . $name;
  variable_del($variable_name);
}

/**
 * The default variables within the Media: googledoc namespace.
 *
 * @param string $name
 *  Optional variable name to retrieve the default. Note that it has not yet
 *  been pre-pended with the MEDIA_GOOGLEDOC_NAMESPACE namespace at this time.
 * @return unknown
 *  The default value of this variable, if it's been set, or NULL, unless
 *  $name is NULL, in which case we return an array of all default values.
 *
 * @see media_googledoc_variable_get()
 * @see media_googledoc_variable_set()
 * @see media_googledoc_variable_del()
 */
function media_googledoc_variable_default($name = NULL) {
  static $defaults;

  if (!isset($defaults)) {//TODO get default variables from parent module ?
    $defaults = array(
      'full_width' => 470,
      'full_height' => 550,
      'preview_width' => 300,
      'preview_height' => 385,
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
 * Return the fully namespaced variable name.
 *
 * @param string $name
 *  The variable name to retrieve the namespaced name.
 * @return string
 *  The fully namespace variable name, prepended with
 *  MEDIA_GOOGLEDOC_NAMESPACE.
 */
function media_googledoc_variable_name($name) {
  return MEDIA_GOOGLEDOC_NAMESPACE.$name;
}
