<?php
/**
 * @file
 *
 * This file contains custom code that manages redirecting SELECT queries to a slave, even if they
 * weren't marked as slave queries by db_select_slave()
 *
 * It is based conceptually on HyperDB for Wordpress, and borrows some of its code and ideas.
 *
 * Basically, we parse each query and note what kind of query it is, and what tables it involves if
 * it changes anything. If a table is written to then subsequently selected from, we'll make sure the
 * query goes to the master to avoid any lag issues.
 *
 * The actual sending of the query to the slave server is managed by Pressflow's _db_query() function, which
 * supports this side of things
 *
 * @author Neil Bertram <neil@catalyst.net.nz>
 */

/**
 * This function is called from a core hack in database.mysql-common.php
 * 
 * It will look at queries as they pass through and determine whether they can be run on the MySQL slave
 * or not, based on what's in the query.
 *
 * @param string $query the SQL query, still with raw placeholders and no args substituted
 * @param string $prefixed_query the SQL query, with prefixes and placeholders substituted
 * @param array $args any arguments that were substituted into the query
 * @return bool TRUE if this query can be safely run on the slave
 */
function _db_is_slave_query(&$query, &$prefixed_query, &$args) {
  static $query_counts, $srtm, $query_cache;
  if (!is_array($query_counts)) {
    $query_counts = array();
    $srtm = array();
    $query_cache = array();
  }
  if (isset($query_cache[$query])) {
    // We've already calculated for this table
    return $query_cache[$query];
  }
  if (count($query_cache) > 250) {
    // To avoid issues when the page is making lots of distinct queries, we flush the cache if it gets too big
    $query_cache = array();
  }

  // Short-circuit these special cases
  if ($query == 'SELECT LAST_INSERT_ID()') {
    return FALSE;
  }
  else if ($query == 'BEGIN' || $query == 'START TRANSACTION') {
    $srtm = TRUE; // force all queries to go to master after this for this request
    return FALSE;
  }
  if ($srtm === TRUE) {
    // we're locked to master for the rest of this request
    return FALSE;
  }

  // Find any mentioned tables
  $tables = array();
  if (preg_match_all('/\{(.?)\}/', $query, $m)) {
    $tables = $m[1];
    
  }
  // Default to master
  $force_slave = FALSE;

  // Look for queries that could possibly be done on slave:
  //  1) Must know the primary table
  //  2) Must not have that table marked in the "send request to master" $srtm array
  //  3) Must not be a write query of any kind
  if (!_db_is_write_query($query)) {
    // Allow this to run on the slave
    $force_slave = TRUE;
  }

  $query_cache[$query] = $force_slave;
  if (!$force_slave && !empty($tables)) {
    // We need to mark this table dirty in the srtm array
    foreach ($tables as $table) {
      $srtm[$table] = $table;
    }
  }

  return $force_slave;
}

/**
 * Determine the likelihood that this query could alter anything
 *
 * @param string $query the SQL query to check
 * @return bool TRUE if the query may be performing a write
 */
function _db_is_write_query ($query) {
  // Quick and dirty: only SELECT statements are considered read-only.
  $query = ltrim($query, "\t (");
  $word = strtoupper(substr(trim($query), 0, 6));
  return 'SELECT' != $word;
}
