<?php

/**
 * Actual cron process
 * 
 * @return array The resulting logs
 */
function _filesmigrate_cron() {
  $return = array();

  _filesmigrate_copy_files($return);

  if (_filesmigrate_has_to_delete()) {
    _filesmigrate_delete_files($return);
  }

  return $return;
}

/**
 * Copy old files to subdirectory
 * 
 * @param array &$return
 *   Logs array
 * 
 * @return Nothing
 */
function _filesmigrate_copy_files(&$return) {
  $files_limit = wally_variable_get('filesmigrate_files_limit', 50);

  // Get all files that are at the "files root"
  $db_files = db_query("
      SELECT f.fid, f.filename, f.filepath
      FROM {files} f
      WHERE f.status = %d
      AND f.filepath RLIKE '^sites/default/files/[^/]*$'
      AND f.fid NOT IN (SELECT e.fid FROM {filesmigrate_error_files} e)
      LIMIT 0 , %d", FILE_STATUS_PERMANENT, $files_limit);

  $copied_count = 0;
  while ($db_file = db_fetch_array($db_files)) {
    if (_filesmigrate_copy_and_store_file($return, $db_file)) {
      $copied_count++;
    }
  }

  $return[] = t('!file_amount file(s) copied.', array('!file_amount' => $copied_count));
}

/**
 * Copy a file to subdirectory and store his old path
 * 
 * @param array &$return
 *   Logs array
 * @param array $orig_file
 *   Informations about the file to be copied
 *   
 * @return boolean The result of the copy
 */
function _filesmigrate_copy_and_store_file(&$return, $orig_file) {
  if ($new_path = _filesmigrate_get_new_path($orig_file)) {
    if (!@copy($orig_file['filepath'], $new_path)) {
      //$return[] = t('Couldn\'t copy file "@filepath" to "@new_path"', array('@filepath' => $orig_file['filepath'], '@new_path' => $new_path));
      // Store error file infos
      $stored_file = new stdClass();
      $stored_file->fid = (int)$orig_file['fid'];
      drupal_write_record('filesmigrate_error_files', $stored_file);
    } else {
      // Store old file infos
      $stored_file = new stdClass();
      $stored_file->filepath = $orig_file['filepath'];
      $stored_file->moved = time();
      drupal_write_record('filesmigrate_temp_files', $stored_file);
      
      // Set new path in the files table
      db_query("UPDATE {files} SET filepath = '%s' WHERE fid = '%d'", $new_path, $orig_file['fid']);
      
      return TRUE;
    }
  } else {
    $return[] = t('Couldn\'t create subdirectory for file "@filepath"', array('@filepath' => $orig_file['filepath']));
  }
  return FALSE;
}

/**
 * Return the new file path
 * 
 * @param array $orig_file
 *   Informations about the file to be copied
 * 
 * @return mixed The destination directory path if it exists and is writable, FALSE if not
 */
function _filesmigrate_get_new_path($orig_file) {
  // @todo choix du nouveau path des fichiers, avec token
  $subdirectory = $orig_file['fid'] % 1000;
  $new_path = file_directory_path().'/migrates/'.$subdirectory;
  module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
  if (file_check_directory_recursive($new_path, FILE_CREATE_DIRECTORY)) {
    return $new_path.'/'.$orig_file['filename'];
  }
  return FALSE;
}

/**
 * Delete old files
 * 
 * @param array &$return
 *   Logs array
 */
function _filesmigrate_delete_files(&$return) {
  $db_files = db_query("SELECT * FROM {filesmigrate_temp_files}");
  
  db_query("TRUNCATE TABLE {filesmigrate_temp_files}");
  
  $delete_count = 0;
  while ($db_file = db_fetch_array($db_files)) {
    if (file_delete($db_file['filepath'])) {
      // Clear imagecaches files
      imagecache_image_flush($db_file['filepath']);
      $delete_count++;
    } else {
      //$return[] = t('Couldn\'t delete file "@filepath"', array('@filepath' => $db_file['filepath']));
      //drupal_write_record('filesmigrate_temp_files', $db_file);
    }
  }
  
  // Clear caches
  // Don't clear cache_form - in-progress form submissions may break.
  // Ordered so clearing the page cache will always be the last action.
  $core = array('cache', 'cache_block', 'cache_filter', 'cache_page');
  $cache_tables = array_merge(module_invoke_all('flush_caches'), $core);
  $cache_tables[] = 'cache_wally_blocks';
  foreach ($cache_tables as $table) {
    cache_clear_all('*', $table, TRUE);
  }
  
  $return[] = t('!file_amount file(s) deleted.', array('!file_amount' => $delete_count));
}

/**
 * Check if the delete process has to run
 * 
 * @return boolean Whether the removal process has to run or not
 */
function _filesmigrate_has_to_delete() {
  $delete_freq = (int)wally_variable_get('filesmigrate_delete_freq', 60) * 60;
  $last_run = (int)wally_variable_get('filesmigrate_last_delete', time() - $delete_freq);

  if ($delete_freq == (1440 * 60) && date('j') != date('j', $last_run)) {
    wally_variable_set('filesmigrate_last_delete', time());
    return TRUE;
  } elseif ($last_run <= (time() - $delete_freq)) {
    wally_variable_set('filesmigrate_last_delete', time());
    return TRUE;
  }
  return FALSE;
}
