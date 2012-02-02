<?php
// $Id: object-cache.inc,v 1.8.2.3 2010/07/22 22:33:34 merlinofchaos Exp $

/**
 * @file
 * The non-volatile object cache is used to store an object while it is
 * being edited, so that we don't have to save until we're completely
 * done. The cache should be 'cleaned' on a regular basis, meaning to
 * remove old objects from the cache, but otherwise the data in this
 * cache must remain stable, as it includes unsaved changes.
 */

/**
 * Get an object from the non-volatile wallyedit cache.
 *
 * This function caches in memory as well, so that multiple calls to this
 * will not result in multiple database reads.
 *
 * @param $name
 *   The name of the object being stored.
 * @param $skip_cache
 *   Skip the memory cache, meaning this must be read from the db again.
 *
 * @return
 *   The data that was cached.
 */
function wallyedit_object_cache_get($name, $skip_cache = FALSE) {
  $cache = &ctools_static('wallyedit_object_cache', array());
  if ($skip_cache) {
    unset($cache[$name]);
  }

  if (!array_key_exists($name, $cache)) {
    $data = db_fetch_object(db_query("SELECT * FROM {wallyedit_object_cache} WHERE name = '%s'", $name));
    if ($data) {
      $cache[$name] = unserialize(db_decode_blob($data->data));
    }
  }
  return isset($cache[$name]) ? $cache[$name] : NULL;
}

/**
 * Store an object in the non-volatile wallyedit cache.
 *
 * @param $name
 *   The name of the object being stored.
 * @param $cache
 *   The object to be cached. This will be serialized prior to writing.
 */
function wallyedit_object_cache_set($name, $cache) {
  wallyedit_object_cache_clear($name);
  db_query("INSERT INTO {wallyedit_object_cache} (name, data, updated) VALUES ('%s', %b, %d)", $name, serialize($cache), time());
}

/**
 * Remove an object from the non-volatile wallyedit cache
 *
 * @param $name
 *   The name of the object being removed.
 */
function wallyedit_object_cache_clear($name) {
  db_query("DELETE FROM {wallyedit_object_cache} WHERE name = '%s'", $name);
  // Ensure the static cache is emptied of this name set.
  $cache = &ctools_static('wallyedit_object_cache', array());
  unset($cache[$name]);
}

/**
 * Remove all objects from the non-volatile wallyedit cache.
 */
function wallyedit_object_cache_clear_all() {
  db_query("TRUNCATE TABLE {wallyedit_object_cache}");
  // Ensure the static cache is emptied.
  $cache = &ctools_static('wallyedit_object_cache', array());
  $cache = array();
}

/**
 * Remove all objects in the object cache that are older than the
 * specified age.
 *
 * @param $age
 *   The minimum age of objects to remove, in seconds. For example, 86400 is
 *   one day. Defaults to 7 days.
 */
function wallyedit_object_cache_clean($age = NULL) {
  if (empty($age)) {
    $age = 86400 * variable_get('wallyedit_outdated_prenodes', 7); // 7 days
  }
  db_query("DELETE FROM {wallyedit_object_cache} WHERE updated < %d", time() - $age);
}
