<?php 

/**
 * Check if Wallymport can process folder.
 * 
 * @param $importer_name
 * The importer who will process the file
 * 
 * @return
 *   Response about the success or failure of import.
 */
function wallymport_check_process_folder($importer_name) {
  // Fetch the cron semaphore
  $semaphore = wally_variable_get('wallymport_process_folder_semaphore', FALSE);
  $semaphore_period = wally_variable_get('wallymport_process_folder_semaphore_period', 5);

  // Try to allocate enough time to run import process.
  if (function_exists('set_time_limit')) {
    @set_time_limit(60 * $semaphore_period);
  }
  $returns = array();

  if ($tmp_return = wimprt_check_semaphore('wallymport_process_folder_semaphore', $semaphore_period, 'folder')) {
    $returns[] = $tmp_return;
  } else {
    // Register shutdown callback
    register_shutdown_function('wallymport_process_folder_cleanup');

    // Lock cron semaphore
    wally_variable_set('wallymport_process_folder_semaphore', time());

    $importer = wallymport_getimporterbyname($importer_name);
    if ($importer != NULL){
      $returns = wallymport_process_folder($importer);
    } else {
      //Unknown importer
      $returns[] = array('error' => TRUE, 'error_msg' => t('Unknown importer'));
    }

    // Record cron time
    wally_variable_set('wallymport_process_folder_last', time());

    // Release cron semaphore
    wally_variable_del('wallymport_process_folder_semaphore');
  }

  $content = '';
  foreach ($returns as $return){
    $return['date'] = time();
    $content .= theme('wallymport_logs', $return);
  } 
  return $content;
}

/**
 * Check if Wallymport can process folder.
 * 
 * @param $importer_name
 * The importer who will process the file
 * 
 * @return
 *   Response about the success or failure of import.
 */
function wallymport_check_process_file($importer_name, $file_name) {
  // Fetch the cron semaphores
  $semaphore = wally_variable_get('wallymport_process_folder_semaphore', FALSE);
  $semaphore_period = wally_variable_get('wallymport_process_folder_semaphore_period', 5);
  $file_semaphore = wally_variable_get('wallymport_process_file_semaphore_'.$file_name, FALSE);
  $file_semaphore_period = wally_variable_get('wallymport_process_file_semaphore_period', 1);

  // Try to allocate enough time to run import process.
  if (function_exists('set_time_limit')) {
    @set_time_limit(60 * $file_semaphore_period);
  }
  $returns = array();

  if ($tmp_return = wimprt_check_semaphore('wallymport_process_folder_semaphore', $semaphore_period, 'folder')) {
    $returns[] = $tmp_return;
  } elseif ($tmp_return = wimprt_check_semaphore('wallymport_process_file_semaphore_'.$file_name, $file_semaphore_period, 'file')) {
    $returns[] = $tmp_return;
  } else {
    // Register shutdown callback
    register_shutdown_function('wallymport_process_folder_cleanup');

    // Lock cron semaphore
    wally_variable_set('wallymport_process_file_semaphore_'.$file_name, time());

    $importer = wallymport_getimporterbyname($importer_name);
    if ($importer != NULL){
      $returns = wallymport_process_file($importer, $file_name);
    } else {
      //Unknown importer
      $returns[] = array('error' => TRUE, 'error_msg' => t('Unknown importer'));
    }

    // Release cron semaphore
    wally_variable_del('wallymport_process_file_semaphore_'.$file_name);
  }

  $content = '';
  foreach ($returns as $return){
    $return['date'] = time();
    $content .= theme('wallymport_logs', $return);
  }
  return $content;
}

/**
 * Shutdown function for Wallymport process folder cleanup.
 */
function wallymport_process_folder_cleanup() {
  // See if the semaphore is still locked.
  if (wally_variable_get('wallymport_process_folder_semaphore', FALSE)) {
    watchdog('Wallymport', 'Process folder task run exceeded the time limit and was aborted.', array(), WATCHDOG_WARNING);

    // Release semaphore
    wally_variable_del('wallymport_process_folder_semaphore');
  }
}

/**
 * Process all zip files from the import folder. 
 * Page Callback of the processing of the import folder and call for each XML element the wallymport_page_process_file().
 * 
 * @param $importer
 *   Importer choose to process file
 * 
 * @return errors, logs
 */
function wallymport_process_folder($importer){

  $returns = NULL;
  
  $folder = $importer->source_directory_path;
  $format = 'xml';
  if ($importer->zip == 1){
    $format = 'zip';
  }
  $files = file_scan_directory($folder, '.*\.'. $format .'$', array('.', '..', 'CVS'), 0, FALSE, 'name', 0);// get list of all files
  
  if (count($files) == 0 ) {
    $error_msg = t('No input files found.');
    drupal_set_header('HTTP/1.1 517 '.$error_msg);
    $returns[] = array('error' => TRUE, 'error_msg' => $error_msg);
  }
  ksort($files);
  if ($returns == NULL){
    // Process each files from folder.
    foreach ($files as $file) {
      //Unzip the file if its zipped
      $xml_file = wallymport_preprocess_file($file, $importer, $returns);
      if ($xml_file != NULL){
        //Process the file
        $return = wallymport_page_process_file($xml_file, $importer);
        if ($return['error'] == FALSE){
          if (!file_move($file->filename, $importer->done_directory_path)) {
            $error_msg = t('An error occured during file copy to done directory.');
            drupal_set_header('HTTP/1.1 517 '.$error_msg);
          } else {
            $file_location = $importer->done_directory_path.'/'.$file->name;
          }
        } else {
          if (!file_move($file->filename, $importer->error_directory_path)) {
            $error_msg = t('An error occured during file copy to error directory.');
            drupal_set_header('HTTP/1.1 517 '.$error_msg);
          } else {
            $file_location = $importer->error_directory_path.'/'.$file->name;
          }
        }
        
        if ($error_msg) {
          $return['error'] = TRUE;
          $return[0]['log'][] = $error_msg;
        }
        
        file_delete($xml_file);
        $returns[] = $return;
        $return['log']['extref'] = $return['log']['node']['current']->field_externalreference[0]['value'];
        unset($return[0]['log']['node']);
        wallymport_save_log($importer, time(), $file_location, serialize($return));
      }
    }
  }
  return $returns;
}

/**
 * Save logs in db
 * 
 * @param $importer_name
 *   Importer_name of the importer choose to process file
 * @param $date
 * @param $file_location
 * @param $log
 * @return errors, logs
 */
function wallymport_save_log($importer, $date, $file_location, $log) {
  if ($importer->keep_log == 1) {
    $logs = new stdClass();
    $logs->importer_name = $importer->name;
    $logs->date = $date;
    $logs->location = $file_location;
    $logs->log = $log;
    drupal_write_record('wallymport_logs', $logs);
  }
}

/**
 * Process all zip files from the import folder. 
 * Page Callback of the processing of the import folder and call for each XML element the wallymport_page_process_file().
 * 
 * @param $importer_name
 *   Importer_name of the importer choose to process file
 * @param $file_name
 *   File name
 * @return errors, logs
 */
function wallymport_process_file($importer, $file_name){
  $returns = NULL;
  
  $folder = $importer->source_directory_path;

  $file = new stdClass();  
  $file->name = $file_name;
  $file->filename = $folder."/".$file_name;
  $file->basename = $file_name;

  if (!file_exists($file->filename)) {
    $error_msg = t('Input file doesn\'t exists');
    $return = array('error' => TRUE, 'error_msg' => $error_msg);
    $returns[] = $return;
    drupal_set_header('HTTP/1.1 517 '.$error_msg);
    wallymport_save_log($importer, time(), $file->filename, serialize($return));
  }

  if ($returns == NULL) {
    $xml_file = wallymport_preprocess_file($file, $importer, $returns);
    if ($xml_file != NULL & $returns == NULL){
      //Process the file
      $return = wallymport_page_process_file($xml_file, $importer);
      if ($return['error'] == FALSE){
        if (!file_move($file->filename, $importer->done_directory_path)) {
          $error_msg = t('An error occured during file copy to done directory.');
          drupal_set_header('HTTP/1.1 517 '.$error_msg);
        } else {
          $file_location = $importer->done_directory_path.'/'.$file->name;
        }
      } else {
        if (!file_move($file->filename, $importer->error_directory_path)) {
          $error_msg = t('An error occured during file copy to error directory.');
          drupal_set_header('HTTP/1.1 517 '.$error_msg);
        } else {
          $file_location = $importer->error_directory_path.'/'.$file->name;
        }
      }
      file_delete($xml_file);
      $returns[] = $return;
      $return['log']['extref'] = $return['log']['node']['current']->field_externalreference[0]['value'];
      unset($return[0]['log']['node']);
      wallymport_save_log($importer, time(), $file_location, serialize($return));
    }
  }

  return $returns;
}

/**
 * Return the form containing the preview iframe and the select box to choose the destination preview
 */
function wallymport_preview_form(&$form_state, $cache_name, $dest = 'disabled') {
  $force_refresh_js = '
    Drupal.behaviors.forceRefreshPreview = function(context) {
      $("#edit-select-preview").trigger("change");
    };';
  drupal_add_js(array('cache_name' => $cache_name), 'setting');
  drupal_add_js(drupal_get_path('module', 'wallyedit').'/js/refresh_preview.js');
  drupal_add_js($force_refresh_js, 'inline');
  drupal_get_messages();

  $prenode = new Prenode($cache_name);
  if (!empty($prenode->form_state)) {
    $options = array('no_dest' => t('Preview of the package\'s page'), );
    foreach ($prenode->form_state[$prenode->form_state['type']]['#node']->field_destinations as $delta => $dest) {
      if ($delta !== 'field_destinations_add_more') {
        $dest_name = taxonomy_get_term($dest['tid'])->name;
        $options += array($delta => $dest_name.' - Target : '.$dest['target'].' - Layout : '.$dest['layout'].' - Rank : '.$dest['rank'], );
      }
    }
    $form['select_preview'] = array(
      '#type' => 'select',
      '#title' => t('Preview destination'),
      '#default_value' => 'no_dest',
      '#options' => $options,
      '#attributes' => array('prenode' => $cache_name),
    );
    $form['#suffix'] = theme('wallyedit_preview_id', array('cache name' => $cache_name, 'dest' => 'no_dest')).drupal_get_js();
  } else {
    $form['#suffix'] = theme('wallyedit_nopreview');
  }

  return $form;
}

/**
 * Import files receive by POST
 * 
 * @param $importer_name
 *  $importer choosed to process de file
 *  
 * @return $result of the post
 * 
 * */
function wallymport_post($importer_name){
  $returns = NULL;
  $importer = wallymport_getimporterbyname($importer_name);
  if ($importer == NULL){
    $error_msg  = t('This importer does not exist');
    $returns[] = array('error' => TRUE, 'error_msg' => $error_msg);
  } elseif ($importer->processed_by_post != 1){
    $error_msg = t('This importer does not accept POST');
    $returns[] = array('error' => TRUE, 'error_msg' => $error_msg);
  } else {
    $nb_file = 0;
    $object_file = new stdClass();
    
    //Get the file
    foreach ($_FILES as $key => $input){
      $object_file->name = $input['name'];
      $object_file->filename = $input['tmp_name'];
      $object_file->basename = $input['tmp_name'];
      $nb_file ++;
    }
    if ($nb_file == 0){
      $temp = variable_get('wallymport_preview_temp', file_directory_temp());
      $file_name = 'wally_curl_input_'.rand();
      $file_path = $temp.'/'.$file_name.'.'.$format;
      $fwrite = TRUE;

      $input = fopen('php://input', 'r');
      $file = fopen($file_path, 'w');
      while (!feof($input)) {
        $fwrite = fwrite($file, fread($input, 1024));
        if (!fwrite)
        break;
      }
      fclose($input);
      //$object_file=$file;
      $object_file->name = $file_name;
      $object_file->filename = $file_path;
      $object_file->basename = $file_path;

      fclose($file);

      $nb_file = 1;
    }

    if ($nb_file > 1){
      $error_msg = 'There is more than one file in the POST';
      $returns[] = array('error' => TRUE, 'error_msg' => $error_msg);
    } elseif ($nb_file == 0){
      $error_msg = 'There is no file in the POST';
      $returns[] = array('error' => TRUE, 'error_msg' => $error_msg);
    } else {
      $xml_file = wallymport_preprocess_file($object_file, $importer, $returns);
      if ($xml_file != NULL & $returns == NULL){
        //Proccess xml
        $returns[] = wallymport_page_process_file($xml_file, $importer);
        //Move file 
        if (!$returns[0]['error']){
          if (!file_move($object_file->filename, $importer->done_directory_path)) {
            $returns[0]['error_msg'][] = t('An error occured during file copy to done directory.');
          } 
        } else {
          if (!file_move($object_file->filename, $importer->error_directory_path)) {
            $returns[0]['error_msg'][] = t('An error occured during file copy to done directory.');
          }
        }
        file_delete($xml_file);
      }
    }
  }
  $js = '<script>
$("div.wallymport_result ul").hide();
$("div#wallyedit_preview h1").hide();
$("div.wallymport_result .logs").click(function(){
  $("div.wallymport_result ul").toggle();
});
</script>';
  $content = '';
  foreach ($returns as $return){
    $return['date'] = time();
    $content .= theme('status_messages').theme('wallymport_logs', $return).$return['content'].$js;
  }
  print $content;
}

/**
 * Process one file.
 * 
 * @param stdObject $file
 *   File object to be processed.
 * @param importer
 *   The importer object used for the import
 * @param array $returns
 *   Results of the import.
 * @return $xml_file (name of the xml file to process)
 */
function wallymport_preprocess_file($file, &$importer, &$returns){
  if ($importer->zip == 1){
    $xml_file = wallymport_preprocess_zip($file, $importer, $returns);
  } else {
    $xml_file = $file->filename;
  }
  return $xml_file;
}

/**
 * Process one zip file.
 * 
 * @param stdObject $file
 *   File object to be processed.
 * @param importer
 *   The importer object used for the import
 * @param array $returns
 *   Results of the import.
 * @return $xml_file (name of the xml file to process)
 */
function wallymport_preprocess_zip($file, &$importer, &$returns){
  $xml_file = NULL;
  $zip_error = FALSE;
  $error_msg = FALSE;
  // Unzip input file to the temp directory
  $zip = new ZipArchive;

  $res = $zip->open($file->filename);
  if ($res === TRUE) {
    module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
    $file_name = strstr($file->name, '.zip') ? str_replace('.zip', '', $file->name) : $file->name;
    $temp_subdir = $importer->temp_directory_path.'/'.$file_name;
    file_check_directory_recursive($temp_subdir, 1);
    if (is_dir($temp_subdir)) {
      $importer->temp_subdir = $temp_subdir;
      $zip->extractTo($temp_subdir);
      // Search for XMLs
      $import_files = file_scan_directory($temp_subdir, '.*\.xml$', array('.', '..', 'CVS'), 0, FALSE, 'name', 0);
      
      if (count($import_files) == 0) {
        $zip_error = TRUE;
        $error_msg = t('No XML file found in the import: !file. The ZIP must contains only files, no folder.', array('!file' => $file->basename));
        $returns[] = array('error' => TRUE, 'error_msg' => $error_msg);
      } elseif (count($import_files) > 1) {
        $zip_error = TRUE;
        $error_msg = t('There is more than one XML file found in the import: !file CODE X9', array('!file' => $file->basename));
        $returns[] = array('error' => TRUE, 'error_msg' => $error_msg);
      } else {
        // Yeah .. ready to ROCK & ROLL
        $import_file = reset($import_files);
        $xml_file = $import_file->filename;
      }
      $zip->close();
    } else {
      $zip_error = TRUE;
      $error_msg = t('Input file !file could not be unzipped to !subdir. Error code !res CODE X10', array('!file' => $file->filename, '!subdir' => $temp_subdir, '!res' => $res));
      $returns[] = array('error' => TRUE, 'error_msg' => $error_msg);
    }
  } else {
    $zip_error = TRUE;
    $error_msg = t('Cant UnZip file: !file CODE X11', array('!file' => $file->filename));
    $returns[] = array('error' => TRUE, 'error_msg' => $error_msg);
  }
  if ($zip_error) {
    if (!file_move($file->filename, $importer->error_directory_path)) {
      $error_msg .= ' '.t('An error occured during file copy to error directory CODE X12.');
    }
    drupal_set_header('HTTP/1.1 517 '.$error_msg);
  }

  return $xml_file;
}

/**
 * Process a xml file or a folder containg an xml file.
 * The function load the XML file, validate it with the xsd defined in the module's settings
 * and call the recursive process wimprt_process_packages()
 *
 * @param $xml_file
 *   The path to a XML file to be proccesed.
 * @param importer
 *   The importer object used for the import
 * @return
 *   array with logs
 */
function wallymport_page_process_file($xml_file, $importer){
  $handle = fopen($xml_file, "r");
  if (!$handle) {
    $error_msg = t('The XML file (!xml_file) isn\'t readable', array('!xml_file' => $xml_file));
    $return = array('error' => TRUE, 'error_msg' => $error_msg);
  } else {
    $contents = fread($handle, filesize($xml_file));
    $contents = wallymport_convertEntities($contents);

    $dom = new DomDocument();
    $dom->LoadXML($contents);
    $log = array();
    $return = array('error' => FALSE);
    module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
    if (!wallytoolbox_validate_xml($dom, $importer->xsd_path)) {
      dsm('XML not VALIDATED');
      $error_msg = t('The XML file (!xml_file) isn\'t validate by the XSD file', array('!xml_file' => $xml_file));
      $return = array('error' => TRUE, 'error_msg' => $error_msg);
    } else {
      module_load_include('inc', 'wallymport', 'includes/wallymport.fields');

      foreach ($dom->childNodes as $child){
        $type = $child->nodeName;
        $function_exist = FALSE;
        foreach ($importer->importer_functions as $importer_function){
          if (function_exists($importer_function.'_process_'.$type)){
            $return = call_user_func($importer_function."_process_".$type, $dom, &$log, $importer);
            $function_exist = TRUE;
            break;
          }
        }
        if ($function_exist == FALSE & function_exists('wimprt_process_'.$type)){
          $return = call_user_func("wimprt_process_".$type, $dom, &$log, $importer);
        }
        if ($return['log']['error'] == TRUE){
          $return = array('error' => TRUE, 'error_msg' => $return[0]['log']['error_msg']);
        }
      }
    }
  }
  return $return;
}

/**
 * Convert HTML Entites in acceptable XML Entites (See http://www.developpez.net/forums/d238024/java/general-java/xml/caracteres-speciaux/)
 *
 * @param $str
 *   the string that represent the xml
 *
 * @return
 *   the converted string
 */
function wallymport_convertEntities($str){
  $xml = array('&#8364;','&#8217;','&#233;', '&#34;','&#38;','&#38;','&#60;','&#62;','&#160;','&#161;','&#162;','&#163;','&#164;','&#165;','&#166;','&#167;','&#168;','&#169;','&#170;','&#171;','&#172;','&#173;','&#174;','&#175;','&#176;','&#177;','&#178;','&#179;','&#180;','&#181;','&#182;','&#183;','&#184;','&#185;','&#186;','&#187;','&#188;','&#189;','&#190;','&#191;','&#192;','&#193;','&#194;','&#195;','&#196;','&#197;','&#198;','&#199;','&#200;','&#201;','&#202;','&#203;','&#204;','&#205;','&#206;','&#207;','&#208;','&#209;','&#210;','&#211;','&#212;','&#213;','&#214;','&#215;','&#216;','&#217;','&#218;','&#219;','&#220;','&#221;','&#222;','&#223;','&#224;','&#225;','&#226;','&#227;','&#228;','&#229;','&#230;','&#231;','&#232;','&#233;','&#234;','&#235;','&#236;','&#237;','&#238;','&#239;','&#240;','&#241;','&#242;','&#243;','&#244;','&#245;','&#246;','&#247;','&#248;','&#249;','&#250;','&#251;','&#252;','&#253;','&#254;','&#255;');
  $html = array('&euro;','&rsquo;','&eacute;','&quot;','&amp;','&amp;','&lt;','&gt;','&nbsp;','&iexcl;','&cent;','&pound;','&curren;','&yen;','&brvbar;','&sect;','&uml;','&copy;','&ordf;','&laquo;','&not;','&shy;','&reg;','&macr;','&deg;','&plusmn;','&sup2;','&sup3;','&acute;','&micro;','&para;','&middot;','&cedil;','&sup1;','&ordm;','&raquo;','&frac14;','&frac12;','&frac34;','&iquest;','&Agrave;','&Aacute;','&Acirc;','&Atilde;','&Auml;','&Aring;','&AElig;','&Ccedil;','&Egrave;','&Eacute;','&Ecirc;','&Euml;','&Igrave;','&Iacute;','&Icirc;','&Iuml;','&ETH;','&Ntilde;','&Ograve;','&Oacute;','&Ocirc;','&Otilde;','&Ouml;','&times;','&Oslash;','&Ugrave;','&Uacute;','&Ucirc;','&Uuml;','&Yacute;','&THORN;','&szlig;','&agrave;','&aacute;','&acirc;','&atilde;','&auml;','&aring;','&aelig;','&ccedil;','&egrave;','&eacute;','&ecirc;','&euml;','&igrave;','&iacute;','&icirc;','&iuml;','&eth;','&ntilde;','&ograve;','&oacute;','&ocirc;','&otilde;','&ouml;','&divide;','&oslash;','&ugrave;','&uacute;','&ucirc;','&uuml;','&yacute;','&thorn;','&yuml;');
  $str = str_replace($html, $xml, $str);
  $str = str_ireplace($html, $xml, $str);
  return $str;
}

/**
 * Determine the next callback function from the Dom element
 *
 * If a callback is passed don't change else try to determine the next callback:
 * - first try callbak associated with the xsi:type attribute.
 * - second try the tag name if the xsi:type attribute doesn't exist.
 *
 * @param $dom
 *   The DomDocument who is being proceed (A DomElement)
 * @param $callback
 *   The callback passing as argument to the caller function
 *   
 * @return
 *   The next callback
 */
function wimprt_set_callback($dom, $callback = NULL){
  if (is_object($dom) && get_class($dom) == 'DOMElement'){
    if ($callback == NULL){
      if ($dom->hasAttribute('xsi:type')){
        $callback = $dom->getAttributeNode('xsi:type')->value;
      }
      else{
        $callback = $dom->nodeName;
      }
    }
  }
  return strtolower($callback);
}

/**
 * Determine if the node from the processed element should be create or update based on the dom.
 *
 * @param $dom
 *   The DomDocument who is being proceed (A DomElement)
 * @param importer
 *   The importer object used for the import
 * @param $objecttype
 *   Node type
 * @param $callback
 *   The callback of the process function to allow specific call of wimprt_create_or_load_object_CALLBACK
 * @param $log
 *
 * @return
 *   A node object
 */
function wimprt_create_or_load($dom, $importer, $objecttype, $callback,&$log){
  $node = new stdClass;
  $function_exist = FALSE;
  foreach ($importer->importer_functions as $importer_function){
    if (function_exists($importer_function.'_create_or_load_'.$objecttype."_".$callback)){
      $node = call_user_func($importer_function."_create_or_load_".$objecttype."_".$callback, $dom, &$log, $importer);
      $function_exist = TRUE;
      break;
    }
  }
  if ($function_exist == FALSE & function_exists("wimprt_create_or_load_".$objecttype."_".$callback)){
    $node = call_user_func("wimprt_create_or_load_".$objecttype."_".$callback, $dom, &$log, $importer);
  }
  return $node;
}
