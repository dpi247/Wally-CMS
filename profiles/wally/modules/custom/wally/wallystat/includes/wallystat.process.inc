<?php

function _wallystat_process_cron() {
  $current_tstamp = time();

  // Reset counters if needed
  _wallystat_counters_reset($current_tstamp);

  // Count the number of terms and nodes
  $term_counts = $node_counts = array();

  $cache_inc = variable_get('cache_inc', './includes/cache.inc');
  switch (true) { // memcache.inc needs less bootstrap than cache.inc
    case strstr($cache_inc, '/memcache.inc') :
      $cached_lock = cache_get('wallystat_lock');
      if ($cached_lock && isset($cached_lock->data) && !empty($cached_lock->data) && is_numeric($cached_lock->data)) {
        $lock = $cached_lock->data;
      } else {
        $lock = 1;
      }
      $next_lock = $lock + 1;
      cache_set('wallystat_lock', $next_lock);

      $cached_entries_index = cache_get('wallystat_entries_index_'.$lock);
      if ($cached_entries_index && isset($cached_entries_index->data) && !empty($cached_entries_index->data)) {
        $ser_entries_index = trim($cached_entries_index->data, '-');
      } else {
        $ser_entries_index = '';
      }
      cache_clear_all('wallystat_entries_index_'.$lock, 'cache');

      if (!empty($ser_entries_index)) {
        $entries_index = explode('--', $ser_entries_index);
        foreach ($entries_index as $entry_id) {
          $res = (array)cache_get($entry_id, 'cache_wallystat_tempdata');
          $res['cid'] = $entry_id;
          _wallystat_process_entry($res, $term_counts, $node_counts, $current_tstamp);
        }
      }
      break;
    case strstr($cache_inc, '/cache.inc') :
    default :
      // Collect all temp datas
      $tmp_datas = db_query("SELECT cid, data, created FROM {cache_wallystat_tempdata}");

      while ($res = db_fetch_array($tmp_datas)) {
        _wallystat_process_entry($res, $term_counts, $node_counts, $current_tstamp);
      }
      break;
  }

  // Update tables with new datas
  _wallystat_counters_update($term_counts, $node_counts);

  // Flush old datas if needed
  _wallystat_counters_clean($current_tstamp);
}

function _wallystat_counters_reset($current_tstamp) {
  $last_tstamp = db_result(db_query_range("SELECT timestamp FROM {wallystat_node_counter} ORDER BY timestamp DESC", 0, 1));
  if ($placeholder = _wallystat_get_placeholder($last_tstamp, $current_tstamp)) {
    db_query('UPDATE {wallystat_node_counter} SET '.$placeholder);
    print t('Updated : !placeholder for !table', array('!placeholder' => $placeholder, '!table' => 'wallystat_node_counter')).'<br/>';
  }
  
  $last_tstamp = db_result(db_query_range("SELECT timestamp FROM {wallystat_term_counter} ORDER BY timestamp DESC", 0, 1));
  if ($placeholder = _wallystat_get_placeholder($last_tstamp, $current_tstamp)){
    db_query('UPDATE {wallystat_term_counter} SET '.$placeholder);
    print t('Updated : !placeholder for !table', array('!placeholder' => $placeholder, '!table' => 'wallystat_term_counter')).'<br/>';
  }
}

function _wallystat_process_entry($res, &$term_counts, &$node_counts, $current_tstamp) {
  $dtstamp = $res['created'];
  if ($dtstamp < $current_tstamp) {
    list($type, $a) = explode('_', $res['cid'], 2);
    switch ($type) {
      case 'wallystat' :
        $data = is_string($res['data']) ? unserialize($res['data']) : $res['data'];
        $trunc_dtstamp = $dtstamp - (date('i', $dtstamp) * 60 + date('s', $dtstamp));

        // Create merged arrays containing all structured datas
        _wallystat_merge_datas($term_counts, $node_counts, $data, $trunc_dtstamp);
        break;

      case 'error' :
        // @todo : manage errors (watchdog ?)
        break;
    }
    cache_clear_all($res['cid'], 'cache_wallystat_tempdata');
  }
}

function _wallystat_merge_datas(&$term_counts, &$node_counts, $data, $trunc_dtstamp) {
  if (isset($data['terms'])) {
    //explode terms to get an array of terms
    $terms = explode(' ', $data['terms']);
    //foreach term add 1 in the array
    foreach ($terms as $term) {
      if (isset($term_counts[$trunc_dtstamp][$term])) {
        $term_counts[$trunc_dtstamp][$term]++;
      } else {
        $term_counts[$trunc_dtstamp][$term] = 1;
      }
    }
  }

  for ($i = 1; $i <= 10; $i++) {
    $id = ($i == 10) ? 'param'.$i : 'param0'.$i;
    $param_array[$id] = $data[$id];
  }
  $param_key = implode('_', $param_array);

  $nid = $data['nid'];
  $param_exist = FALSE;
  if (isset($node_counts[$trunc_dtstamp][$nid])) {
    foreach ($node_counts[$trunc_dtstamp][$nid] as $key => $cur_node_count) {
      if ($cur_node_count['param_key'] == $param_key) {
        $param_exist = TRUE;
        $node_counts[$trunc_dtstamp][$nid][$key]['node_count']++;
        break;
      }
    }
  }
  if (!$param_exist || !isset($node_counts[$nid])) {
    $new_entry = array(
      'param_key' => $param_key,
      'node_count' => 1,
      'data' => $data,
    );
    $node_counts[$trunc_dtstamp][$nid][] = $new_entry;
  }
}

function _wallystat_counters_update($term_counts, $node_counts) {
  foreach ($term_counts as $timestamp => $terms) {
    foreach ($terms as $tid => $term_count) {
      db_query("UPDATE {wallystat_term_counter} SET daycount = daycount + %d, weekcount = weekcount + %d, monthcount = monthcount + %d, yearcount = yearcount + %d, totalcount = totalcount + %d, timestamp = %d WHERE tid = %d",
                $term_count, $term_count, $term_count, $term_count, $term_count, $timestamp, $tid);
      if (!db_affected_rows()) {
        // If no affected rows, insert a new record
        db_query("INSERT INTO {wallystat_term_counter} (tid, daycount, weekcount, monthcount, yearcount, totalcount, timestamp) VALUES (%d, %d, %d, %d, %d, %d, %d)",
                  $tid, $term_count, $term_count, $term_count, $term_count, $term_count, $timestamp);
      }
    }
  }

  foreach ($node_counts as $timestamp => $nodes) {
    foreach ($nodes as $nid => $node_datas) {
      foreach ($node_datas as $node_data) {
        $data = $node_data['data'];
        $node_count = $node_data['node_count'];

        db_query("UPDATE {wallystat_node_counter} SET daycount = daycount + %d, weekcount = weekcount + %d, monthcount = monthcount + %d, yearcount = yearcount + %d, totalcount = totalcount + %d, timestamp = %d, title = '%s'
                  WHERE nid = %d AND param01 = '%s' AND param02 = '%s' AND param03 = '%s' AND param04 = '%s' AND param05 = '%s' AND param06 = '%s' AND param07 = '%s' AND param08 = '%s' AND param09 = '%s' AND param10 = '%s'", 
                  $node_count, $node_count, $node_count, $node_count, $node_count, $timestamp, $data['title'], $nid, $data['param01'], $data['param02'], $data['param03'], $data['param04'], $data['param05'], $data['param06'], $data['param07'], $data['param08'], $data['param09'], $data['param10']);
  
        if (!db_affected_rows()) {
          // If no affected rows, insert a new record
          db_query("INSERT INTO {wallystat_node_counter} (nid, title, daycount, weekcount, monthcount, yearcount, totalcount, timestamp, param01, param02, param03, param04, param05, param06, param07, param08, param09, param10)
                    VALUES (%d, '%s', %d, %d, %d, %d, %d, %d, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')", 
                    $nid, $data['title'], $node_count, $node_count, $node_count, $node_count, $node_count, $timestamp, $data['param01'], $data['param02'], $data['param03'], $data['param04'], $data['param05'], $data['param06'], $data['param07'], $data['param08'], $data['param09'], $data['param10']);
        }
  
        db_query("UPDATE {wallystat_node_hourly_counter} SET count = count + %d, title = '%s'
                  WHERE nid = %d AND timestamp = %d AND param01 = '%s' AND param02 = '%s' AND param03 = '%s' AND param04 = '%s' AND param05 = '%s' AND param06 = '%s' AND param07 = '%s' AND param08 = '%s' AND param09 = '%s' AND param10 = '%s'",
                  $node_count, $data['title'], $nid, $timestamp, $data['param01'], $data['param02'], $data['param03'], $data['param04'], $data['param05'], $data['param06'], $data['param07'], $data['param08'], $data['param09'], $data['param10']);
        if (!db_affected_rows()) {
          // If no affected rows, insert a new record
          db_query("INSERT INTO {wallystat_node_hourly_counter} (nid, title, timestamp, count, param01, param02, param03, param04, param05, param06, param07, param08, param09, param10)
                    VALUES (%d, '%s', %d, %d, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')",
                    $nid, $data['title'], $timestamp, $node_count, $data['param01'], $data['param02'], $data['param03'], $data['param04'], $data['param05'], $data['param06'], $data['param07'], $data['param08'], $data['param09'], $data['param10']);
        }
      }
    }
  }
}

function _wallystat_counters_clean($current_tstamp) {
  // Time limit of 48h
  $limit_tstamp = $current_tstamp - 49*60*60;
	db_query("DELETE FROM {wallystat_node_hourly_counter} WHERE timestamp < %d", $limit_tstamp);
}

/**
 * Internal function to get the placeholder to put in the query
 * reseting some of the count fields depending on the difference of 
 * time between two timestamps
 */
function _wallystat_get_placeholder($last_tstamp, $current_tstamp){
  if (!is_numeric($last_tstamp) || !is_numeric($current_tstamp))
    return FALSE;
  
  $placeholder = array();
  if (date('z', $current_tstamp) != date('z', $last_tstamp))
    $placeholder[] = 'daycount = 0';
  
  if (date('W', $current_tstamp) != date('W', $last_tstamp))
    $placeholder[] = 'weekcount = 0';
  
  if (date('m', $current_tstamp) != date('m', $last_tstamp))
    $placeholder[] = 'monthcount = 0';
  
  if (date('Y', $current_tstamp) != date('Y', $last_tstamp))
    $placeholder[] = 'yearcount = 0';
  
  if (!empty($placeholder)){
    return implode(', ', $placeholder);
  } else {
    return FALSE;
  }
}

function _wallystat_cron_cleanup() {
  // See if the semaphore is still locked.
  if (wally_variable_get('wallystat_cron_semaphore', FALSE)) {
    // Release cron semaphore
    wally_variable_del('wallystat_cron_semaphore');
  }
}
