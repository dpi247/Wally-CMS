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
function wallytoolbox_object_cache_get($nid, $revision, $view_name, $display) {
  $cid = $nid.'_'.$revision.'_'.$view_name.'_'.$display;
  $cached_datas = cache_get($cid, 'cache_wally_object');
  
  if (!is_object($cached_datas) || !isset($cached_datas) || empty($cached_datas)) {
    return FALSE;
  }

  return $cached_datas->data;
}

/**
 * Store an object in the non-volatile wally cache.
 *
 * @param $node
 *   The node to be cached. This will be serialized prior to writing.
 * 
 * @return
 *   Result of the database query.
 */
function wallytoolbox_object_cache_set($node) {
  $nid = $node->nid;
  $revision = $node->revision_uid;
  $view_name = $node->view->name;
  $display = $node->view->current_display;
  
  $cid = $nid.'_'.$revision.'_'.$view_name.'_'.$display;
  
  return cache_set($cid, $node, 'cache_wally_object');
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
  return cache_clear_all($nid, 'cache_wally_object', TRUE);
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
    wallytoolbox_object_cache_clear($ref_node['nid'].'_');
  }
}

/**
 * Remove all nodes from the non-volatile wally.
 * 
 * @return
 *   Result of the database query.
 */
function wallytoolbox_object_cache_clear_all() {
  return cache_clear_all('*', 'cache_wally_object', TRUE);
}
