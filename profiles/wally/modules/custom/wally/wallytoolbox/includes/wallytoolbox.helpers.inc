<?php

/**
 * Menu callback; clears memcached bin(s), then redirects to the previous page.
 */
function wallytoolbox_flush_memcached($bins = NULL) {
  module_load_include('inc', 'memcache', 'dmemcache');
  
  if ($bins) {
    dmemcache_flush($bins);
    drupal_set_message(t('Memcached bin "@bin" cleared.', array('@bin' => $bins)));
  } else {
    $bins = variable_get('memcache_bins', array('cache' => 'default'));
    foreach ($bins as $bin => $value) {
      dmemcache_flush($bin);
      drupal_set_message(t('Memcached bin "@bin" cleared.', array('@bin' => $bin)));
    }
  }

  drupal_goto();
}

/**
 * Unsets all persistent variables beginning with the given string.
 *
 * @param $name
 *   The name of the variable to undefine.
 *
 * @see variable_get(), variable_set(), variable_del()
 */
function wallytoolbox_variable_del_like($name, $type = 'both') {
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
 * Get all persistent variables beginning with the given string.
 *
 * @param $name
 *   The name of the variable to get.
 *
 * @see variable_get(), variable_set(), variable_del()
 */
function wallytoolbox_variable_get_like($get_name, $remove_common = FALSE, $type = 'both') {
  switch ($type) {
    case 'system' :
      global $conf;
      $variables = $conf;
      break;
    case 'wally' :
      $variables = wally_variable_get_all();
      break;
    case 'both' :
    default :
      global $conf;
      $variables = array_merge($conf, wally_variable_get_all());
      break;
  }

  $return = array();

  foreach ($variables as $name => $value) {
    if (strpos($name, $get_name) === 0) {
      if ($remove_common) {
        $name = str_replace($get_name, '', $name);
      }
      $return[$name] = $value;
    }
  }

  return empty($return) ? FALSE : $return;
}

/**
 * Replace arguments in a string by their value (like the "t" function does)
 * 
 * @param $string
 *   A string containing the string to convert.
 * @param $args
 *   An associative array of replacements to make. Incidences
 *   of any key in this array are replaced with the corresponding value. Based
 *   on the first character of the key, the value is escaped and/or themed:
 *    - !variable: inserted as is
 *    - @variable: escape plain text to HTML (check_plain)
 *    - %variable: escape text and theme as a placeholder for user-submitted
 *      content (check_plain + theme_placeholder)
 */
function wallytoolbox_string_args_translate($string, $args) {
  foreach ($args as $key => $value) {
    switch ($key[0]) {
      case '@':
        // Escaped only.
        $args[$key] = check_plain($value);
        break;

      case '%':
      default:
        // Escaped and placeholder.
        $args[$key] = theme('placeholder', $value);
        break;

      case '!':
        // Pass-through.
    }
  }
  return strtr($string, $args);
}


/**
 * Save or update a file in the Drupal file table and his associate file in the directory structure using a subdirectory and a subsubdirectory
 * 
 * We use sub and subsub directory to avoid to much files in a same directory.
 * Tippically the subtype should be a type eg. Flows and subsub should be a type instance like Flow A, Flow B, etc.
 * In extra we also use a third level with the day of the month...
 * 
 * @param string $file_uri
 *   A string containing the file uri.
 * @param string $subdirectory
 *   A string containing the file uri.
 * @param string $subsubdirectory
 *   A string containing the file uri.
 * @param string $file_temp_directory
 *   A string containing the root of the Drupal file system (commonly: sites/default/files).
 * @param boolean $replace
 *   If the given URI already exists, replace current with new one or not
 */
function wallytoolbox_create_or_update_file($file_uri, $subdirectory, $subsubdirectory, $file_temp_dir = 'sites/default/files', $requestor = NULL, $replace = TRUE) {
  // $new_file_path = relatively to drupal files directory
  $new_file_path = $subdirectory.'/'.$subsubdirectory.'/'.date('Ymd', time());

  module_load_include('inc', 'filefield', 'field_file');
  $select_fid_query = "SELECT fid FROM {wallytoolbox_external_files} WHERE uri = '%s'";
  $select_fid_query .= $requestor ? "AND requestor = '%s'" : "AND requestor IS NULL";
  $db_files = db_query($select_fid_query, $file_uri, $requestor);
  while ($existing_fid = db_fetch_array($db_files)) {
    $existing_file = field_file_load($existing_fid['fid']);
    if ($replace) {
      if (!field_file_delete($existing_file)) {
        db_query('DELETE FROM {files} WHERE fid = %d', $existing_fid['fid']);
        _field_file_cache(NULL, $existing_file); // delete the file from the cache
      }
      db_query("DELETE FROM {wallytoolbox_external_files} WHERE fid = %d", $existing_fid['fid']);
    } else {
      return $existing_file;
    }
  }

  module_load_include('inc', 'wallymport', 'includes/wallymport.fieldshelpers');
  $new_file = _wimprthelp_create_file_from_uri($file_uri, $new_file_path);
  $new_file_obj = new stdClass();
  $new_file_obj->fid = $new_file['fid'];
  $new_file_obj->uri = $file_uri;
  if ($requestor) $new_file_obj->requestor = $requestor;
  drupal_write_record('wallytoolbox_external_files', $new_file_obj);
  return $new_file;
}

/**
 * A true boolean conversion function.
 * 
 * @param $v
 *   The value to be converted
 * @param $del
 *   default value return if conversion fail.
 *
 * @return
 *   boolean "true" or "false"
 */
function wallytoolbox_toboolean($v, $def=NULL) {
  if ($v===FALSE) return FALSE;
  if ($v===TRUE) return TRUE;
  if (strtolower($v)=="true") return TRUE;
  if (strtolower($v)=="false") return FALSE;
  if ($v==0) return FALSE;
  if ($v==1) return TRUE;
  return $def;
}

/**
 * Helper fonction to calculate time consuming part of a script.
 *
 * @param $step
 *   The step of the process we evaluate. also used in the dsm output
 * @param $print
 *   Yes if the output should be a dsm false if you want the consumint time as a return.
 * @param $namespace
 *   To avoid collision between two debug timer use a namespace
 * @param $restart
 *   Restart the static variable begin time.
 *   
 * @return
 *   the difference between the time between the  first call of this fuction for this namespace and the current call of this function for this namespace
 */
function wallytoolbox_set_microtime_step($step = '', $print = TRUE, $namespace = "default", $restart = FALSE) {
  static $begin_time ;
  static $first;
  
  if(!isset($begin_time[$namespace])) {
    $begin_time[$namespace] = 0;
    $first[$namespace] = TRUE;
  }
  
  if ($first[$namespace] || $restart) {
    $begin_time[$namespace] = microtime(TRUE);
    $first[$namespace] = FALSE;
  } else {
    $end_time = microtime(TRUE);
    $t = $end_time - $begin_time[$namespace];
    if ($print) {
      $micro = sprintf('%06d', ($t - floor($t)) * 1000000);
      $d = new DateTime(date('Y-m-d H:i:s.'.$micro, $t));
      $formatted = $d->format('i:s.u');
      dsm($formatted, $step);
    }
    return $t;
  }
}

function wallytoolbox_check_is_node($node) {
  if(isset($node->type) && !empty($node->type) && isset($node->title) && !empty($node->title)) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Get numeric keys of a structured array tree element.
 */
function wallytoolbox_element_children_numeric($element) {
  return array_filter(array_keys((array) $element), 'wallytoolbox_element_child_numeric');
}

/**
 * Check if the key is a numeric child.
 */
function wallytoolbox_element_child_numeric($key) {
  return is_numeric($key);
}

/**
 *  Add "$element" in the structure of the $array based on the path of target 
 * 
 * @param: $array
 *   - the targeted array.
 * @param: $element
 *   - the element to add to the array.
 * @param: $target
 *   - the place where add element in the array structure.
 * @param: $append
 *   - a boolean to indicate if the element should replace or added next the element specified by target
 * 
 * @return nothing, the form_state is passed by reference.
 */
function wallytoolbox_add_elem_at_target(&$array, $element, $target, $append = FALSE){
  if (count($target) > 0) {
    $key = array_shift($target);
    if (!isset($array[$key])) {
      $array[$key] = array();
    }
    wallytoolbox_add_elem_at_target($array[$key], $element, $target, $append);
  } else {
    if($append === TRUE){
      $array[] = $element;
    } else {
      $array = $element;
    }
  }
}

/**
 * Return the target associated with the hierarchical structure of the array based on the path formed by parents.
 * 
 * @param: $array
 *   - the array
 * @param: $parents
 *   - the parents is an array representing the path in the array
 */
function wallytoolbox_get_target_from_parents(&$array, $parents = NULL) {
  if($parents == NULL) {
    $parents = $array['parents'];
  }

  $return = NULL;
  $temp = $array;
  foreach($parents as $index => $cursor_name) {
    if(isset($temp[$cursor_name])) {
      $temp = &$temp[$cursor_name];
    } else {
      return $return;
    }
  }

  if ($temp) {
    $return = $temp;
  }
  return $return;
}

/**
 * Remove an element from an array. The element to remove is found following the given path
 * Thx to JDE
 */
function wallytoolbox_remove_element($array, $path, $level = 0) {
  foreach($array as $key => $value) {
    if($key == $path[$level]) { // On atteint l'elem de $a correspondant au $path dans $b pour ce niveau de traitement
      if($level != (count($path) - 1)) { // On est pas encore au dernier niveau de traitement
        if(is_array($value)) { // Logique car il y a encore des niveaux à traiter dans $b;
          $array[$key] = wallytoolbox_remove_element($value, $path, $level+1);
        }
      } else {
        unset($array[$path[$level]]);
      }
    }
  }
  return $array;
}

/**
 * Check wheter or not an HTTP request is successfull :
 *   - Code 200
 *   - Not empty
 * 
 * @param object $request
 *   The result of the HTTP request
 * 
 * @return bool
 *   TRUE or FALSE
 */
function wallytoolbox_is_http_request_successfull($request) {
  if (is_object($request) &&
      ((isset($request->code) && $request->code == 200) || (isset($request->redirect_code) && $request->redirect_code == 200)) &&
      isset($request->data) && !empty($request->data)) {
    return TRUE;
  }

  return FALSE;
}

function wallytoolbox_merge_arrays() {
  if (func_num_args() < 2) {
    trigger_error(__FUNCTION__ .' needs two or more array arguments', E_USER_WARNING);
    return;
  }
  $arrays = func_get_args();
  $merged = array();
  while ($arrays) {
    $array = array_shift($arrays);
    if (!is_array($array)) {
      trigger_error(__FUNCTION__ .' encountered a non array argument', E_USER_WARNING);
      return;
    }
    if (!$array) {
      continue;
    }
    foreach ($array as $key => $value) {
      if (is_string($key) || is_numeric($key)) {
        if (is_array($value) && array_key_exists($key, $merged) && is_array($merged[$key]) && !array_key_exists($key.'_add_more', $value)) {
          $merged[$key] = call_user_func(__FUNCTION__, $merged[$key], $value);
        } else {
          $merged[$key] = $value;
        }
      } else {
        $merged[] = $value;
      }
    }
  }
  return $merged;
}

/**
 * Transform a DOMDocument, DOMNode, DOMElement, wathever, in an array.
 */
function dom2array($node) {
  $res = array();
  if($node->nodeType == XML_TEXT_NODE){
      $res = $node->nodeValue;
  }
  else{
      if($node->hasAttributes()){
          $attributes = $node->attributes;
          if(!is_null($attributes)){
              $res['@attributes'] = array();
              foreach ($attributes as $index=>$attr) {
                  $res['@attributes'][$attr->name] = $attr->value;
              }
          }
      }
      if($node->hasChildNodes()){
          $children = $node->childNodes;
          for($i=0;$i<$children->length;$i++){
              $child = $children->item($i);
              $res[$child->nodeName][] = dom2array($child);
          }
      }
  }
  return $res;
}

/**
 * Autoindent an XML string
 */
function formatXmlString($xml) {  
  // add marker linefeeds to aid the pretty-tokeniser (adds a linefeed between all tag-end boundaries)
  $xml = preg_replace('/(>)(<)(\/*)/', "$1\n$2$3", $xml);
  
  // now indent the tags
  $token      = strtok($xml, "\n");
  $result     = ''; // holds formatted version as it is built
  $pad        = 0; // initial indent
  $matches    = array(); // returns from preg_matches()
  
  // scan each line and adjust indent based on opening/closing tags
  while ($token !== false) : 
    // test for the various tag states
    
    // 1. open and closing tags on same line - no change
    if (preg_match('/.+<\/\w[^>]*>$/', $token, $matches)) : 
      $indent=0;
    // 2. closing tag - outdent now
    elseif (preg_match('/^<\/\w/', $token, $matches)) :
      $pad--;
    // 3. opening tag - don't pad this one, only subsequent tags
    elseif (preg_match('/^<\w[^>]*[^\/]>.*$/', $token, $matches)) :
      $indent=1;
    // 4. no indentation needed
    else :
      $indent = 0; 
    endif;
    
    // pad the line with the required number of leading spaces
    $line    = str_pad($token, strlen($token)+$pad, ' ', STR_PAD_LEFT);
    $result .= $line . "\n"; // add to the cumulative result, with linefeed
    $token   = strtok("\n"); // get the next token
    $pad    += $indent; // update the pad size for subsequent lines    
  endwhile; 
  
  return $result;
}

/**
 * Recursively remove a dir.
 */
function rrmdir($dir) { 
  if ($dir && isset($dir) && is_dir($dir)) { 
    $objects = scandir($dir); 
    foreach ($objects as $object) { 
      if ($object != "." && $object != "..") { 
        if (filetype($dir."/".$object) == "dir") rrmdir($dir."/".$object); else unlink($dir."/".$object); 
      } 
    } 
    reset($objects); 
    rmdir($dir); 
  } 
}

function wallytoolbox_export_files_to_tar($files, $filename) {
  // Clear out output buffer to remove any garbage from tar output.
  if (ob_get_level()) {
    ob_end_clean();
  }

  drupal_set_header('Content-type: application/x-tar');
  drupal_set_header('Content-Disposition: attachment; filename="'. $filename .'"');

  foreach ($files as $file) {
    print wallytoolbox_tar_create($file['name'].$file['extension'], $file['content']);
  }
}

/**
 * Tar creation function. Written by dmitrig01.
 *
 * @param $name
 *   Filename of the file to be tarred.
 * @param $contents
 *   String contents of the file.
 *
 * @return
 *   A string of the tar file contents.
 */
function wallytoolbox_tar_create($name, $contents) {
  $tar = '';
  $binary_data_first = pack("a100a8a8a8a12A12",
    $name,
    '100644 ', // File permissions
    '   765 ', // UID,
    '   765 ', // GID,
    sprintf("%11s ", decoct(strlen($contents))), // Filesize,
    sprintf("%11s", decoct(time())) // Creation time
  );
  $binary_data_last = pack("a1a100a6a2a32a32a8a8a155a12", '', '', '', '', '', '', '', '', '', '');

  $checksum = 0;
  for ($i = 0; $i < 148; $i++) {
    $checksum += ord(substr($binary_data_first, $i, 1));
  }
  for ($i = 148; $i < 156; $i++) {
    $checksum += ord(' ');
  }
  for ($i = 156, $j = 0; $i < 512; $i++, $j++) {
    $checksum += ord(substr($binary_data_last, $j, 1));
  }

  $tar .= $binary_data_first;
  $tar .= pack("a8", sprintf("%6s ", decoct($checksum)));
  $tar .= $binary_data_last;
  
  $buffer = str_split($contents, 512);
  foreach ($buffer as $item) {
    $tar .= pack("a512", $item);
  }

  return $tar;
}

function wallytoolbox_tar_extract_dom($filename, $tmp_path) {
  set_error_handler('_wallytoolbox_extract_tar_error');
  include_once 'Archive/Tar.php';
  restore_error_handler();
  
  $return = array(
    'error' => FALSE,
    'error_msg' => '',
    'content' => NULL,
  );

  if (class_exists('Archive_Tar')) {
    $ext = pathinfo($filename, PATHINFO_EXTENSION);
    $dir = '/tmp/';

    //Check file extension
    if($ext == 'tar') {
      //Open file to read
      $tar = new Archive_Tar($tmp_path, TRUE);

      //Extract the file using Pear library
      if($tar->extract($dir)) {
        $list_content = $tar->listContent();
        $local_path = $dir.$list_content[0]['filename'];
        $handle = fopen($local_path, 'r');

        if (!$handle) {
          $return['error'] = TRUE;
          $return['error_msg'] = t('The XML file (!xml_file) isn\'t readable', array('!xml_file' => $local_path));
        } else {
          $contents = fread($handle, filesize($local_path));
          $dom = new DomDocument();
          if ($dom->LoadXML(trim($contents))) {
            $return['content'] = $dom;
          } else {
            $return['error'] = TRUE;
            $return['error_msg'] = t('Unable to create DOM Document.');
          }
          $return['content'] = $dom;
        }
      } else {
        $return['error'] = TRUE;
        $return['error_msg'] = t('Unable to extract tar file contents');
      }
    } else {
      $return['error'] = TRUE;
      $return['error_msg'] = t('Given file is not a tar file');
    }
  } else {
    $return['error'] = TRUE;
    $return['error_msg'] = t('Class "Archive_Tar" not found');
  }

  return $return;
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
function file_check_directory_recursive(&$directory, $mode = 0, $form_item = NULL) {
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
 * Transform a stdClass object to another class
 */
function dirty_cast($obj, $to_class, $require = FALSE) {
  if ($require) {
    require_once $require;
  }
  
  if(class_exists($to_class)) {
    $obj_in = serialize($obj);
    $obj_out = 'O:' . strlen($to_class) . ':"' . $to_class . '":' . substr($obj_in, $obj_in[2] + 7);
    return unserialize($obj_out);
  }
  else {
    return false;
  }
}

/**
 * Recursively compare two arrays.
 * For two arrays ($a and $b), arrayRecursiveDiff($a, $b) is not equal to arrayRecursiveDiff($b, $a)
 */
function arrayRecursiveDiff($aArray1, $aArray2) {
  $aReturn = array();

  foreach ($aArray1 as $mKey => $mValue) {
    if (array_key_exists($mKey, $aArray2)) {
      if (is_array($mValue)) {
        $aRecursiveDiff = arrayRecursiveDiff($mValue, $aArray2[$mKey]);
        if (count($aRecursiveDiff)) { $aReturn[$mKey] = $aRecursiveDiff; }
      } elseif (is_object($mValue) && is_object($aArray2[$mKey])) {
        $aRecursiveDiff = arrayRecursiveDiff((array)$mValue, (array)$aArray2[$mKey]);
        if (count($aRecursiveDiff)) { $aReturn[$mKey] = $aRecursiveDiff; }
      } else {
        if ($mValue != $aArray2[$mKey]) {
          $aReturn[$mKey] = $mValue;
        }
      }
    } else {
      $aReturn[$mKey] = $mValue;
    }
  }
  return $aReturn;
}

/**
 * Return all available destinations in an array following the
 * structure :
 * 
 * array[destination_path_tid][position][layout][rank] = TRUE;
 */
function wallytoolbox_destinations_items_array(){
  $all_dests = cckdestinations_get_classes_and_options();
  $rank_limit = wally_variable_get('cckdestinations_rank_limit', '10');
  for ($i=0; $i<=$rank_limit; $i++) {
    $ranks_list[] = $i;
  }
  $array_dests = array();

  foreach ($all_dests['tid']['#options'] as $tid=>$term_infos) {
    $array_dests[$tid] = array();
    foreach ($all_dests['target']['#options'] as $target_name=>$target_infos) {
      if (substr($target_name, 0, strlen($tid)) == $tid && !is_numeric(substr($target_name, strlen($tid), 1))) {
        $target = substr($target_name, strlen($tid));
        $array_dests[$tid][$target] = array();
        foreach ($all_dests['layout']['#options'] as $layout_name=>$layout_infos) {
          if (substr($layout_name, 0, strlen($target_name)) == $target_name) {
            $layout = $layout_infos[substr($layout_name, strlen($target_name))];
            $array_dests[$tid][$target][$layout] = array();
            foreach ($ranks_list as $rank_elem) {
              $array_dests[$tid][$target][$layout][$rank_elem] = TRUE;
            }
          }
        }
      }
    }
  }
  
  return $array_dests; 
}

/**
 * Flush the page cache for the given internal url.
 */
function _wallytoolbox_flush_all_pages($internal_urls = '*', $flush_aliases = FALSE, $flush_type = 'drupal') {
  global $base_url;
  $varnish_pages_to_flush = array();
  
  if (is_string($internal_urls))
    $internal_urls = array($internal_urls);
  
  foreach ($internal_urls as $internal_url) {
    $url_to_flush = array();
    $url_to_flush[] = $internal_url;
    
    if ($internal_url != '*' && $flush_aliases) {
      $aliases = wallytoolbox_get_all_aliases($internal_url);
      foreach ($aliases as $alias)
        $url_to_flush[] = $alias;
    }
  
    foreach($url_to_flush as $url) {
      if ($flush_type == 'drupal' || $flush_type == 'both') {
        if ($url == '*') {
          cache_clear_all($url, 'cache_page', TRUE);
        } else {
          cache_clear_all($base_url.'/'.$url, 'cache_page');
        }
      }
      if ($flush_type == 'varnish' || $flush_type == 'both') {
        if ($url == '*' && module_exists('warnish')) {
          warnish_purge_all_pages();
        } else {
          $varnish_pages_to_flush[] = '/'.$url;
        }
      }
    }
    
    if (strstr($internal_url, 'home')) {
      if ($flush_type == 'drupal' || $flush_type == 'both') {
        cache_clear_all($base_url.'/'.variable_get('site_frontpage', 'home'), 'cache_page', TRUE);
        cache_clear_all($base_url.'/', 'cache_page');
      }
      if ($flush_type == 'varnish' || $flush_type == 'both') {
        $varnish_pages_to_flush[] = variable_get('site_frontpage', 'home');
        $varnish_pages_to_flush[] = '/';
      }
    }
  }
  
  if (!empty($varnish_pages_to_flush) && module_exists('warnish'))
    warnish_purge_page($varnish_pages_to_flush);
}

/**
 * Return an array of all existing aliases for an internal url.
 */
function _wallytoolbox_get_all_aliases($internal_url) {
  $db_aliases = db_query('SELECT dst FROM {url_alias} WHERE src = "%s" ORDER BY pid DESC', $internal_url);
  $return_aliases = array();

  while ($db_alias = db_fetch_array($db_aliases)) {
    $return_aliases[] = $db_alias['dst'];
  }

  if(count($return_aliases) == 0){
    $return_aliases[] = $internal_url;
  }

  return $return_aliases;
}

/**
 * Get the latest created alias for an internal URL
 * @param $internal_url {String} Internal URL, it can't begin with "/" or "http://".
 * @return {String} The alias if found, $internal_url if not.
 */
function wallytoolbox_get_main_alias($internal_url){
  if(is_null($internal_url) || trim($internal_url) == "")
    return $internal_url;
  $db_aliases = @db_query('SELECT dst FROM {url_alias} WHERE src = "%s" ORDER BY pid DESC LIMIT 0,1', $internal_url);
  if(!$db_aliases)
    return $internal_url;
  $db_alias = db_fetch_array($db_aliases);
  if(!$db_alias)
    return $internal_url;
  return $db_alias['dst'];
}

/**
 * Delete the Drupal file and remove the physical file of a photo.
 * 
 * @param array $photofile
 *   A Drupal file array.
 */
function wallytoolbox_delete_photofile($photofile) {
  if (isset($photofile['filepath'])) {
    $filepath = $photofile['filepath'];
    file_delete($filepath);
    db_query('DELETE FROM files WHERE filepath = "%s"', $filepath);
  }
}

function wallytoolbox_validate_xml($dom, $xsd_path) {
  set_error_handler('_wallytoolbox_validate_xml_error', E_WARNING);
  return $dom->schemaValidate($xsd_path);
}

function _wallytoolbox_extract_tar_error($errno, $errstr, $errfile = '', $errline = 0, $errcontext = array()) {
  if ($errno === 2) {
    drupal_set_message(t('The "Archive_Tar" package need to be installed on your system to use this functionality (see !link for further informations)', array('!link' => l('here', 'http://pear.php.net/package/Archive_Tar/redirected'))), 'error');
  }
}

function _wallytoolbox_validate_xml_error($errno, $errstr, $errfile = '', $errline = 0, $errcontext = array()) {
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
