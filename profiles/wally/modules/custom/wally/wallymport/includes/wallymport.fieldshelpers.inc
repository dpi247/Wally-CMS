<?php

function _wimprthelp_getfieldnamefromvocabularyname($name){
  $result_set = NULL;
  switch ($name){
    case 'Tags'     : $result_set = 'field_free_tags';break;
    case 'Concepts' : $result_set = 'field_concepts';break;
    case 'IPTC'     : $result_set = 'field_iptc';break;
  }
  return $result_set;
}

function _wimprthelp_getTaxonomyVidFromDamName($damname){
  $vid = NULL;
  switch ($damname){
    case 'Tags'      : $vid = variable_get('wallymport_freetagtaxonomy', NULL);break;
    case 'Concepts'  : $vid = variable_get('wallymport_concept', NULL);break;
    case 'IPTC'      : $vid = variable_get('wallymport_iptc', NULL);break;
    case 'Persons'   : $vid = variable_get('wallymport_person', NULL);break;
    case 'Authors'   : $vid = variable_get('wallymport_author', NULL);break;
    case 'Entities'  : $vid = variable_get('wallymport_entity', NULL);break;
    case 'Locations' : $vid = variable_get('wallymport_location', NULL);break;
  }
  return $vid;
}

function _wimprthelp_array_to_ccklocation($location) {
  $node = array();

  // Get all country list from location module
  $countries = location_get_iso3166_list();

  // Is it an existant location - get the LID?
  if (isset($location["lid"])) {
    $node['lid'] = $location["lid"];
  }

  if ($location["name"]) {
    $node['name'] = $location["name"];
  } else {
    // TO DO FATAL :: No location NAME - crital
    drupal_set_message('No location NAME found.','error');
    return NULL;
  }

  // Street lines
  if (isset($location["street"])) $node['street'] = $location["street"];
  if (isset($location["additional"])) $node['additional'] = $location["additional"];

  // City
  $node['city'] = $location["city"];

  // Province
  if (isset($location['province_name'])) {
    if ($node['province']=location_province_code($location['country_name'],$location['province_name'])) {
      $node['province_name'] = $location['province_name'];
    } else {
      drupal_set_message('Country PROVINCE not found for '.$location['country_name'].'/'.$location['province_name'].'.','error');
    }
  }

  // Postal Code
  $node['postal_code'] = $location["postal_code"];

  // Country
  if (isset($location['country'])) {
    if (array_key_exists($location['country'],$countries)) {
      $node['country'] = $location['country'];
      $node['country_name'] = $countries[$location['country']];
    } else {
      drupal_set_message('Country ISO CODE not found for '.$location['country'].'.','error');
    }
  } else {
    if (isset($location['country_name'])) {
      if ($node['country']=array_search($location['country_name'],$countries)) {
        $node['country_name'] = $location['country_name'];
      } else {
        drupal_set_message('Country ISO not found for '.$location['country_name'].'.','error');
      }
    }
  }
  location_standardize_country_code($node['country']);

  // Long-Lattitude
  if (isset($location["latitude"]) && isset($location["longitude"])) {
    if (is_numeric($location["latitude"]) && is_numeric($location["longitude"])) {
      $node['latitude'] = $location["latitude"];
      $node['longitude'] = $location["longitude"];
      $node['locpick']["user_latitude"] = $location["latitude"];
      $node['locpick']["user_longitude"] = $location["longitude"];
    } else {
      drupal_set_message('longitude & Lattitude not numeric.','error');
    }
  } else {
    drupal_set_message('longitude & Lattitude not set.','error');
  }

  $settings = variable_get('location_settings_node_wally_locationtype', array());
  $node['location_settings'] = $settings;

  $node['source'] = 0;
  $node['is_primary'] = 0;

  return (array) $node;
}

/**
 * Translates datetime into CCK datetime field
 *
 * @param $date
 *   - ISO Datetime (e.g. '2010-10-26T21:32:52')
 *
 * @return
 *   CCK datetime field (array)
 */
function _wimprthelp_date_to_ccktime($date) {
  list($thedate, $thetime) = explode('T', $date);
  if (count(explode('+',$thetime)) == 2){
    list($datetime, $offset) = explode('+', $date);
    list ($offsethour, $offsetmin) = explode(':', $offset);
    $time_offset = $offsethour*3600 + $offsetmin * 60;
    $time = strtotime($datetime) - $time_offset;
  } elseif (count(explode('-',$thetime)) == 2){
    list($thetime, $offset) = explode('-', $thetime);
    $date = $thedate.'T'.$thetime;
    list ($offsethour, $offsetmin) = explode(':', $offset);
    $time_offset = $offsethour*3600 + $offsetmin * 60; 
    $time = strtotime($date) + $time_offset;
  } else {
    //We suppose the xml date is at the same offset than the site
    $timezone = new DateTimeZone(variable_get('date_default_timezone_name', 'UTC'));
    $time_offset = $timezone->getOffset(new DateTime("now")); // Offset in seconds
    $time = strtotime($date) - $time_offset;
  }

  $result['value'] = date('Y-m-d H:i:s', $time);
  $result['timezone'] = variable_get('date_default_timezone_name', 'UTC');
  $result['timezone_db'] = 'UTC';
  $result['date_type'] = 'datetime';
  return $result;
}

/**
 * Translates datetimes into CCK datetimes field for polldates (
 *
 * @param $date
 *   - ISO Datetime (e.g. '2010-10-26T21:32:52')
 *
 * @return
 *   CCK polldate field (array)
 */
function _wimprthelp_polldate_to_ccktime($date1,$date2) {
  //@todo: handle timezone

  $date1 = _wimprthelp_date_to_ccktime($date1);
  $date2 = _wimprthelp_date_to_ccktime($date2);
  $result['value'] = $date1['value'];
  $result['value2'] = $date2['value'];
  $result['timezone'] = variable_get('date_default_timezone_name', 'UTC');
  $result['timezone_db'] = 'UTC';
  $result['date_type'] = 'datetime';
  return $result;
}

/**
 * Translates datetime into timestamp
 *
 * @param $date
 *   - ISO Datetime (e.g. '2010-10-26T21:32:52')
 *
 * @return
 *   timestamp
 */
function _wimprthelp_date_to_timestamp($date) {
  $result = strtotime($date);
  return $result;
}

/**
 * return filtred html text from html text
 *
 * @param $text
 *   - An non filtred html content
 *
 * @return
 *   An filtred html content
 */
function _wimprthelp_html_text($text){
  return $text;
}

/**
 * return plain text from potential html text
 *
 * @param $text
 *   - An non filtred html content
 *
 * @return
 *   An plain text
 */
function _wimprthelp_plain_text($text) {
  if (module_exists('filter')) {
    return filter_xss($text, array());
  } else {
    return $text;
  }
}

/**
 * Return acreated drupal file object from uri. The images supported
 * formats are jpg (or jpeg), png and gif. 
 *
 * @param $file_uri
 *   - A uri to the filet
 *
 * @return
 *   A Drupal File Object
 */
function _wimprthelp_create_file_from_uri($file_uri, $new_file_path = '', $importer = NULL) {
  if (empty($file_uri)) {
    return NULL;
  }

  global $proceed_file_path;
  global $base_path;
  global $proceed_folder_path;

  if ($importer == NULL){
    $temp_folder = $proceed_folder_path;
    $uid = 1;
  } elseif($importer->zip == 0){
    //If files are not zipped : The img files is search in the source directory path folder Be careful of name collision!
    $temp_folder = $importer->source_directory_path;
    $uid = $importer->default_user ? $importer->default_user : 1;
  } else {
    //if files are zipped
    $temp_folder = $importer->temp_subdir;
    $uid = $importer->default_user ? $importer->default_user : 1;
  }

  $uri = parse_url($file_uri);
  $badcar = array('+', ' ');
  $new_file_path = str_replace($badcar, '_', rtrim($new_file_path, '/'));
  $new_file_path .= '/';

  switch($uri['scheme']){
    case'embed':
      // "+" sign in filename is unsupported by imagecache.
      $good_uri_host = str_replace($badcar, '_', $uri['host']);
      $file_temp_path = $temp_folder.'/'.$uri['host'];
      $file_dir = file_directory_path().'/'.$new_file_path;
      _wimprthelp_file_check_directory_recursive($file_dir, 1);
      $file_dest_path = $file_dir.'/'.rand().'_'.$good_uri_host;

      if (!@_copy_file($file_temp_path, $file_dest_path)) {
        // Everything for owner, read and execute for others
        //chmod($file_dest_path, octdec("0777"));
        //drupal_set_message('Cant copy image file.'.$file_temp_path, "error");
      	watchdog('Wallymport file', t('File copy form @from to @to failed', array('@from' => $file_temp_path, '@to' => $file_dest_path)), array(), WATCHDOG_ERROR);
      } else {
        if ($importer && $importer->wallymport_preview) {
          $status = FILE_STATUS_TEMPORARY;
        } else {
          $status = FILE_STATUS_PERMANENT;
        }
        $return = _wimprthelp_create_file($file_dest_path, $uid, $status);
        return $return;
      }
      break;

    case 'http':
      $expl_path = explode('/', $uri['path']);
      $file_name = $expl_path[count($expl_path)-1];
      $http_req = drupal_http_request($file_uri, array(), 'GET', NULL, wally_variable_get('wallytoolbox_http_request_retry', 1), wally_variable_get('wallytoolbox_http_request_timeout', 3));
      if ($http_req && $http_req->code == '200' OR $http_reql->redirect_code=='200') {
        if (stristr($http_req->headers['Content-Type'], 'image') || stristr($http_req->headers['Content-type'], 'image')) {
          if (stristr($file_name,'.jpg') || stristr($file_name,'.jpeg') || stristr($file_name,'.gif') || stristr($file_name,'.png')) {
            $good_file_name = str_replace($badcar, '_', $file_name);
          } elseif ($http_req->headers['Content-Disposition']) {
            $content_disp = $http_req->headers['Content-Disposition'];
            $sub_cont_disp = substr(strstr($content_disp, 'filename="'), strlen('filename="'));
            $good_file_name = str_replace($badcar, "_", substr($sub_cont_disp, 0, strlen($sub_cont_disp) - 1));
          } else {
            break;
          }
          $file_temp_dir = file_directory_path().'/'.$new_file_path;
          _wimprthelp_file_check_directory_recursive($file_temp_dir, 1);
          $file_temp_path = $file_temp_dir.'/'.md5($file_uri).'_'.$good_file_name;
          if(!@_copy_file($file_uri, $file_temp_path)) {
            chmod($file_temp_path, octdec("0777"));
            drupal_set_message(t('Cant copy image file : @file', array('@file' => $file_temp_path)), 'error');
          } else {
            if ($importer && $importer->wallymport_preview) {
              $status = FILE_STATUS_TEMPORARY;
            } else {
              $status = FILE_STATUS_PERMANENT;
            }
            $return = _wimprthelp_create_file($file_temp_path, $uid, $status);
            return $return;
          }
        }
      }
      break;
  }

  return $return;
}

function _wimprthelp_create_file($filepath, $uid, $status = FILE_STATUS_PERMANENT) {
  $file = new stdClass();
  $file->filename = basename($filepath);
  $file->filepath = $filepath;
  $file->filemime = file_get_mimetype($filepath);
  $file->filesize = filesize($filepath);
  $file->uid = $uid;
  $file->status = $status;
  $file->timestamp = time();
  drupal_write_record('files', $file);
  $return = field_file_load($filepath);
  return $return;
}

function _wimprthelp_check_is_node($node) {
  if(isset($node->type) && !empty($node->type) &&
     isset($node->title) && !empty($node->title) &&
     isset($node->field_externalreference) && !empty($node->field_externalreference[0]['value'])) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Delete the Drupal file and remove the physical file of a photo.
 * 
 * @param array $photofile
 *   A Drupal file array.
 */
function _wimprthelp_delete_photofile($photofile) {
  if (isset($photofile['filepath'])) {
    $filepath = $photofile['filepath'];
    file_delete($filepath);
    db_query('DELETE FROM files WHERE filepath = "%s"', $filepath);
  }
}

/**
 * Unsets all persistent variables beginning with the given string.
 *
 * @param $name
 *   The name of the variable to undefine.
 *
 * @see variable_get(), variable_set(), variable_del()
 */
function _wimprthelp_variable_del_like($name, $type = 'both') {
  if ($type == 'system' || $type == 'both') {
    global $conf, $db_prefix;

    $db_res = db_query("SELECT name FROM {variable} WHERE name LIKE '%s%'", $name);

    while ($res = db_fetch_array($db_res)) {
      db_query("DELETE FROM {variable} WHERE name = '%s'", $res['name']);
      unset($conf[$res['name']]);
    }

    // The write-through rebuild optimization isn't compatible with SimpleTest.
    // Because array-based prefixes don't work with SimpleTest, we can assume
    // that a non-string prefix indicates lack of SimpleTest operations.
    if (is_string($db_prefix) && strpos($db_prefix, 'simpletest') === 0) {
      cache_clear_all('variables', 'cache');
    }

    variable_cache_rebuild();
  }

  if ($type == 'wally' || $type == 'both') {
    global $wally_vars;

    $db_res = db_query("SELECT name FROM {wally_variable_fast} WHERE name LIKE '%s%'", $name);

    while ($res = db_fetch_array($db_res)) {
      db_query("DELETE FROM {wally_variable_fast} WHERE name = '%s'", $res['name']);
      unset($wally_vars[$res['name']]);
    }
  }
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
function _wimprthelp_file_check_directory_recursive(&$directory, $mode = 0, $form_item = NULL) {
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

/**
 * Recursively remove a dir.
 */
function _wimprthelp_rrmdir($dir) {
  if ($dir && isset($dir) && is_dir($dir)) {
    $objects = scandir($dir);
    foreach ($objects as $object) {
      if ($object != "." && $object != "..") {
        if (filetype($dir."/".$object) == "dir") _wimprthelp_rrmdir($dir."/".$object); else unlink($dir."/".$object);
      }
    }
    reset($objects);
    rmdir($dir);
  }
}

function _wimprthelp_validate_xml($dom, $xsd_path) {
  set_error_handler('_wimprthelp_validate_xml_error', E_WARNING);
  return $dom->schemaValidate($xsd_path);
}

function _wimprthelp_validate_xml_error($errno, $errstr, $errfile = '', $errline = 0, $errcontext = array()) {
  if (strpos($errstr, 'DOMDocument::schemaValidate') === 0) {
    switch ($errno) {
      case 1:
        $mess_type = 'error';
        break;
      case 2:
        $mess_type = 'warning';
        break;
      default:
        $mess_type = 'status';
        break;
    }
    drupal_set_message(check_plain($errstr), $mess_type);
  }
}
