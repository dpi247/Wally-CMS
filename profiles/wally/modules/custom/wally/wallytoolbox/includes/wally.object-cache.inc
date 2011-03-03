<?php
// $Id: wally.object-cache.inc,v 1.0 2011/03/02 ODM $

/**
 * @file
 * The non-volatile object cache is used to store an object while it is
 * being edited, so that we don't have to save until we're completely
 * done. The cache should be 'cleaned' on a regular basis, meaning to
 * remove old objects from the cache, but otherwise the data in this
 * cache must remain stable, as it includes unsaved changes.
 */

/**
 * Get an node from the non-volatile wally cache.
 *
 * This function caches in memory as well, so that multiple calls to this
 * will not result in multiple database reads.
 *
 * @param $nid
 *   The node ID of the node to get.
 * @param $skip_cache
 *   Skip the memory cache, meaning this must be read from the db again.
 *
 * @return
 *   The data that was cached.
 */
function wallytoolbox_object_cache_get($nid, $skip_cache = FALSE) {
  $cache = &ctools_static(__FUNCTION__, array());
  if ($skip_cache) {
    unset($cache[$nid]);
  }

  if (!array_key_exists($nid, $cache)) {
    $data = db_fetch_object(db_query("SELECT * FROM {wallytoolbox_object_cache} WHERE nid = '%s'", $nid));
    if ($data) {
      $cache[$nid] = unserialize($data->data);
    }
  }
  
  watchdog('WallyToolbox', 'Node %nid get from cache.', array('%nid' => $nid), WATCHDOG_INFO);
  
  return isset($cache[$nid]) ? $cache[$nid] : FALSE;
}

/**
 * Store an object in the non-volatile ctools cache.
 *
 * @param $nid
 *   The node ID of the node to get.
 * @param $node
 *   The node to be cached. This will be serialized prior to writing.
 * 
 * @return
 *   Result of the database query.
 */
function wallytoolbox_object_cache_set($node) {
  $cache = &ctools_static('wallytoolbox_object_cache_get', array());
  
  $nid = $node->nid;
  
  wallytoolbox_object_cache_clear($nid);

  $object = array(
    'nid' => $nid,
    'type' => $node->type,
    'updated' => time(),
    'data' => $node,
  );
  
  watchdog('WallyToolbox', 'Node %nid cached.', array('%nid' => $nid), WATCHDOG_INFO);
  
  return drupal_write_record('wallytoolbox_object_cache', $object);
}

/**
 * Remove a node from the non-volatile wally cache
 *
 * @param $nid
 *   The node ID of the node to delete.
 * 
 * @return
 *   Result of the database query.
 */
function wallytoolbox_object_cache_clear($nid) {  
  // Ensure the static cache is emptied of this $nid set.
  $cache = &ctools_static('wallytoolbox_object_cache_get', array());
  unset($cache[$nid]);
  
  watchdog('WallyToolbox', 'Node %nid removed from cache.', array('%nid' => $nid), WATCHDOG_INFO);
  
  return db_query("DELETE FROM {wallytoolbox_object_cache} WHERE nid = '%s'", $nid);
}

/**
 * Remove cache for all nodes which have a reference to the given node.
 * 
 * @param $node
 *   The node being referenced.
 */
function wallytoolbox_object_referencer_cache_clear($node) {
  $ref_nodes = wallytoolbox_get_node_by_reference($node);
  foreach ($ref_nodes as $ref_node) {
    wallytoolbox_object_cache_clear($ref_node['nid']);
  }
}

/**
 * Get the cache status of a group of nodes.
 *
 * @param $nids
 *   An array of node IDs.
 *
 * @return
 *   An associative array of "nid" => "updated" entries with 0 as
 * updated value if the node is not found.
 */
function wallytoolbox_object_cache_test_nodes($nids) {
  $result = db_query("SELECT nid, updated FROM {wallytoolbox_object_cache} WHERE nid = '%s'", $nids);

  $return = array();
  foreach ($nids as $nid) {
    $return[$nid] = 0;
  }
  while ($test = db_fetch_object($result)) {
    $return[$test->nid] = $test->updated;
  }

  return $return;
}

/**
 * Remove all nodes from the non-volatile wally.
 * 
 * @return
 *   Result of the database query.
 */
function wallytoolbox_object_cache_clear_all() {
  // Ensure the static cache is emptied of this obj:name set.
  $cache = &ctools_static('ctools_object_cache_get', array());
  foreach ($cache as $key=>$value) {
    unset($cache[$key]);
  }
  
  watchdog('WallyToolbox', 'All cache cleared.', array(), WATCHDOG_INFO);
  
  return db_query("TRUNCATE TABLE {wallytoolbox_object_cache}");
}

/**
 * Remove all nodes in the object cache that are older than the
 * specified age.
 *
 * @param $age
 *   The minimum age of objects to remove, in seconds. For example, 86400 is
 *   one day. Defaults to 7 days.
 * 
 * @return
 *   Result of the database query.
 */
function wallytoolbox_object_cache_clean($age = NULL) {
  if (empty($age)) {
    $age = 86400 * 7; // 7 days
  }
  
  watchdog('WallyToolbox', 'Old cache cleared.', array(), WATCHDOG_INFO);
  
  return db_query("DELETE FROM {wallytoolbox_object_cache} WHERE updated < %d", time() - $age);
}

