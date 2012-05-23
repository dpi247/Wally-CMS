<?php

/**
 * Record a prenode in the index table.
 *
 * @param $pid
 *   The ID of the prenode being stored.
 *
 * @return
 *   The prenode.
 */
function wallyedit_index_prenodes_get($pid) {
  return db_fetch_array(db_query("SELECT * FROM {wallyedit_prenodes} WHERE pid = '%s'", $pid));
}

/**
 * Store a prenode in the index table.
 *
 * @param $prenode
 *   The prenode to be indexed.
 */
function wallyedit_index_prenodes_set($form_state) {
  $pid = $form_state['cache name'];
  $type_name = $form_state['type'];
  $title = $form_state[$type_name]['#node']->title;

  wallyedit_index_prenodes_clear($pid);
  db_query("INSERT INTO {wallyedit_prenodes} (pid, updated, title, type) VALUES ('%s', %d, '%s', '%s')", $pid, time(), $title, $type_name);
}

/**
 * Remove a prenode from the index table.
 *
 * @param $pid
 *   The ID of the prenode being removed.
 */
function wallyedit_index_prenodes_clear($pid) {
  db_query("DELETE FROM {wallyedit_prenodes} WHERE pid = '%s'", $pid);
}

/**
 * Remove all prenodes from the index table.
 */
function wallyedit_index_prenodes_clear_all() {
  db_query("TRUNCATE TABLE {wallyedit_prenodes}");
}

/**
 * Remove all prenodes in the index table that are older than the
 * specified age.
 *
 * @param $age
 *   The minimum age of prenodes to remove, in seconds. For example, 86400 is
 *   one day. Defaults to 7 days.
 */
function wallyedit_index_prenodes_clean($age = NULL) {
  if (empty($age)) {
    $age = 86400 * wally_variable_get('wallyedit_outdated_prenodes', 7); // 7 days
  }
  db_query("DELETE FROM {wallyedit_prenodes} WHERE updated < %d", time() - $age);
}
