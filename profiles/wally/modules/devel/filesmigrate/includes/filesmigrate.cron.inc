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
  $db_files = db_query("SELECT fid, filename, filepath FROM {files} WHERE STATUS = 1 AND filepath RLIKE '^sites/default/files/[^/]*$' LIMIT 0 , %d", $files_limit);

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
      $return[] = t('Couldn\'t copy file "@filename" to "@new_path', array('@filename' => $orig_file['filename'], '@new_path' => $new_path));
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
    file_delete($db_file['filepath']);
    // Clear imagecaches files
    imagecache_image_flush($db_file['filepath']);
    $delete_count++;
  }
  
  // Clear the package populate cache to clear references to old paths
  module_load_include('inc', 'wallytoolbox', 'includes/wally.object-cache');
  wallytoolbox_object_cache_clear_all();
  
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

/**
 * Checks whether a directory exists and is writable.
 *
 * Furthermore, the directory can optionally be created if it does not exist,
 * and/or be set to writable if it is currently not. Directories need to have
 * execute permission to be considered a directory by FTP servers.
 *
 * @param $directory
 *   A string representing the directory path.
 * @param $mode
 *   An optional bitmask containing the actions, if any, to be carried out on
 *   the directory. Any combination of the actions FILE_CREATE_DIRECTORY and
 *   FILE_MODIFY_PERMISSIONS is allowed.
 * @param $form_item
 *   An optional string containing the name of a form item that any errors
 *   will be attached to. Useful when the function validates a directory path
 *   entered as a form value. An error will consequently prevent form submit
 *   handlers from running, and instead display the form along with the
 *   error messages.
 *
 * @return
 *   FALSE if the directory does not exist or is not writable, even after
 *   any optional actions have been carried out. Otherwise, TRUE is returned.
 */
function _filesmigrate_file_check_directory_recursive(&$directory, $mode = 0, $form_item = NULL) {
  $directory = rtrim($directory, '/\\');

  // Check if directory exists.
  if (!is_dir($directory)) {
    if (($mode & FILE_CREATE_DIRECTORY) && @mkdir($directory, 0775, TRUE)) {
      drupal_set_message(t('The directory %directory has been created.', array('%directory' => $directory)));
      @chmod($directory, 0775); // Necessary for non-webserver users.
    }
    else {
      if ($form_item) {
        form_set_error($form_item, t('The directory %directory does not exist.', array('%directory' => $directory)));
      }
      return FALSE;
    }
  }

  // Check to see if the directory is writable.
  if (!is_writable($directory)) {
    if (($mode & FILE_MODIFY_PERMISSIONS) && @chmod($directory, 0775)) {
      drupal_set_message(t('The permissions of directory %directory have been changed to make it writable.', array('%directory' => $directory)));
    }
    else {
      form_set_error($form_item, t('The directory %directory is not writable', array('%directory' => $directory)));
      watchdog('file system', 'The directory %directory is not writable, because it does not have the correct permissions set.', array('%directory' => $directory), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  if ((file_directory_path() == $directory || file_directory_temp() == $directory) && !is_file("$directory/.htaccess")) {
    $htaccess_lines = "SetHandler Drupal_Security_Do_Not_Remove_See_SA_2006_006\nOptions None\nOptions +FollowSymLinks";
    if (($fp = fopen("$directory/.htaccess", 'w')) && fputs($fp, $htaccess_lines)) {
      fclose($fp);
      chmod($directory .'/.htaccess', 0664);
    }
    else {
      $variables = array('%directory' => $directory, '!htaccess' => '<br />'. nl2br(check_plain($htaccess_lines)));
      form_set_error($form_item, t("Security warning: Couldn't write .htaccess file. Please create a .htaccess file in your %directory directory which contains the following lines: <code>!htaccess</code>", $variables));
      watchdog('security', "Security warning: Couldn't write .htaccess file. Please create a .htaccess file in your %directory directory which contains the following lines: <code>!htaccess</code>", $variables, WATCHDOG_ERROR);
    }
  }

  return TRUE;
}
