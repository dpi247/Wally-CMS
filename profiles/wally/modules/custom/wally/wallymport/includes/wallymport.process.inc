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
  $semaphore = variable_get('wallymport_process_folder_semaphore', FALSE);
  $semaphore_period = variable_get('wallymport_process_folder_semaphore_period', 5);

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
    variable_set('wallymport_process_folder_semaphore', time());

    $importer = wallymport_getimporterbyname($importer_name);
    if ($importer != NULL){
      $returns = wallymport_process_folder($importer);
    } else {
      //Unknown importer
      $returns[] = array('error' => TRUE, 'error_msg' => t('Unknown importer'));
    }

    // Record cron time
    variable_set('wallymport_process_folder_last', time());

    // Release cron semaphore
    variable_del('wallymport_process_folder_semaphore');
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
  $semaphore = variable_get('wallymport_process_folder_semaphore', FALSE);
  $semaphore_period = variable_get('wallymport_process_folder_semaphore_period', 5);
  $file_semaphore = variable_get('wallymport_process_file_semaphore_'.$file_name, FALSE);
  $file_semaphore_period = variable_get('wallymport_process_file_semaphore_period', 1);

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
    variable_set('wallymport_process_file_semaphore_'.$file_name, time());

    $importer = wallymport_getimporterbyname($importer_name);
    if ($importer != NULL){
      $returns = wallymport_process_file($importer, $file_name);
    } else {
      //Unknown importer
      $returns[] = array('error' => TRUE, 'error_msg' => t('Unknown importer'));
    }

    // Release cron semaphore
    variable_del('wallymport_process_file_semaphore_'.$file_name);
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
  if (variable_get('wallymport_process_folder_semaphore', FALSE)) {
    watchdog('Wallymport', 'Process folder task run exceeded the time limit and was aborted.', array(), WATCHDOG_WARNING);

    // Release semaphore
    variable_del('wallymport_process_folder_semaphore');
  }
}

/**
 * Process all zip files from the import folder. 
 * Page Callback of the processing of the import folder and call for each XML element the wallymport_process_xmlfile().
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
      $return = NULL;
      $xml_file = wallymport_preprocess_file($file, $importer, $return);
      $file_location = '';
      if ($xml_file != NULL){
        //Process the file
        $return = wallymport_process_xmlfile($xml_file, $importer);
        $file_location = wallymport_movefiles($importer, $file, $xml_file, $return);
        $return['log']['extref'] = $return['log']['node']['current']->field_externalreference[0]['value'];
        unset($return['log']['node']);
      } else {
        $file_location = $importer->error_directory_path.'/'.$file->name;
      }
      $returns[] = $return;
      wallymport_save_log($importer, time(), $file_location, serialize($return));
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
 * Page Callback of the processing of the import folder and call for each XML element the wallymport_process_xmlfile().
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
    $returns[] = array('error' => TRUE, 'error_msg' => $error_msg);
  }
  if ($returns == NULL){
    $return = NULL;
    $xml_file = wallymport_preprocess_file($file, $importer, $return);
    $file_location = '';
    if ($xml_file != NULL){
      //Process the file
      $return = wallymport_process_xmlfile($xml_file, $importer);
      $file_location = wallymport_movefiles($importer, $file, $xml_file, $return);
      $return['log']['extref'] = $return['log']['node']['current']->field_externalreference[0]['value'];
      unset($return['log']['node']);
    } else {
      $file_location = $importer->error_directory_path.'/'.$file->name;
    }
    $returns[] = $return;
    wallymport_save_log($importer, time(), $file_location, serialize($return));
  }
  return $returns;
}

/**
 * Move files to their new directory and delete tmp files
 *
 * @param $importer
 *   $importer choosed to process de file
 * @param $file
 *   The file to move
 * @param $xml_file
 *   The tmp file to delete
 * @param $return
 *   information about the import
 */
function wallymport_movefiles($importer, $file, $xml_file, $return){
  $file_location = $importer->source_directory_path.'/'.$file->name;
  if ($return['error'] == FALSE){
    if (!file_move($file->filename, $importer->done_directory_path)) {
      $error_msg = t('An error occured during file copy to done directory.');
      drupal_set_header('HTTP/1.1 517 '.$error_msg);
    } else {
      $file_location = $importer->done_directory_path.'/'.$file->name;
      //If the is no zip, get all media file path form log and replace them in the done directory file
      if ($importer->zip == 0){
        foreach ($return['log']['filetomove'] as $media_file){
          if (!file_move($media_file, $importer->done_directory_path)){
            $error_msg = t('An error occured during file copy to done dorectory (file : '.$media_file.')');
          }
        }
      }
    }
  } else {
    if (!file_move($file->filename, $importer->error_directory_path)) {
      $error_msg = t('An error occured during file copy to error directory.');
      drupal_set_header('HTTP/1.1 517 '.$error_msg);
    } else {
      $file_location = $importer->error_directory_path.'/'.$file->name;
      //If the is no zip, get all media file path form log and replace them in the error directory file
      if ($importer->zip == 0){
        foreach ($return['log']['filetomove'] as $media_file){
          if (!file_move($media_file, $importer->error_directory_path)){
            $error_msg = t('An error occured during file copy to done dorectory (file : '.$media_file.')');
          }
        }
      }
    }
  }
  //save log in db
  file_delete($xml_file);
  return $file_location;
}

/**
 * Return the form containing the preview iframe and the select box to choose the destination preview
 */
function wallymport_preview_form(&$form_state, $cache_name, $dest = 'disabled') {
  global $base_url;
  $prev_js = '$(document).ready(function() {
            var temp = "'.$base_url.'/node/'.$cache_name.'/preview2/"+$("#edit-select-preview").val();
            $("#prev_iframe").attr("src", temp);
            
            $("#edit-select-preview").change(function () {
              temp = "'.$base_url.'/node/'.$cache_name.'/preview2/"+$(this).val();
              $("#prev_iframe").attr("src", temp);
            });
          });';

  drupal_add_js($prev_js, 'inline');
  drupal_get_messages();
  $prenode = new Prenode($cache_name);

  if (!empty($prenode->form_state)){
    $options = array('no_dest' => 'Node', );
    foreach ($prenode->form_state[$prenode->form_state['type']]['#node']->field_destinations as $delta => $dest){
      if ($delta !== 'field_destinations_add_more'){
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
    $form['#suffix'] = theme('wallyedit_preview_id', array('cache name' => $cache_name, 'dest' => 'disabled')).drupal_get_js();
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
    
    if ($nb_file > 1){
      $error_msg = 'There is more than one file in the POST';
      $returns[] = array('error' => TRUE, 'error_msg' => $error_msg);
    } elseif ($nb_file == 0){
      $error_msg = 'There is no file in the POST';
      $returns[] = array('error' => TRUE, 'error_msg' => $error_msg);
    } else {
      $return = NULL;
      $xml_file = wallymport_preprocess_file($object_file, $importer, $return);
      $file_location = '';
      if ($xml_file != NULL & $return == NULL){
        //Proccess xml
        $return = wallymport_process_xmlfile($xml_file, $importer);
        $file_location = wallymport_movefiles($importer, $file, $xml_file, $return);
        $return['log']['extref'] = $return['log']['node']['current']->field_externalreference[0]['value'];
        unset($return['log']['node']);
      } else {
        $file_location = $importer->error_directory_path.'/'.$file->name;
      }
      $returns[] = $return;
      wallymport_save_log($importer, time(), $file_location, serialize($return));
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
    $content .= theme('status_messages').theme('wallymport_logs', $return).$return['log']['content'].$js;
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
function wallymport_preprocess_file($file, &$importer, &$return) {
  if ($importer->zip == 1){
    $xml_file = wallymport_preprocess_zip($file, $importer, $return);
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
function wallymport_preprocess_zip($file, &$importer, &$return){
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
        $return = array('error' => TRUE, 'error_msg' => $error_msg);
      } elseif (count($import_files) > 1) {
        $zip_error = TRUE;
        $error_msg = t('There is more than one XML file found in the import: !file', array('!file' => $file->basename));
        $return = array('error' => TRUE, 'error_msg' => $error_msg);
      } else {
        // Yeah .. ready to ROCK & ROLL
        $import_file = reset($import_files);
        $xml_file = $import_file->filename;
      }
      $zip->close();
    } else {
      $zip_error = TRUE;
      $error_msg = t('Input file !file could not be unzipped to !subdir. Error code !res', array('!file' => $file->filename, '!subdir' => $temp_subdir, '!res' => $res));
      $return = array('error' => TRUE, 'error_msg' => $error_msg);
    }
  } else {
    $zip_error = TRUE;
    $error_msg = t('Cant UnZip file: !file', array('!file' => $file->filename));
    $return = array('error' => TRUE, 'error_msg' => $error_msg);
  }
  if ($zip_error) {
    if (!file_move($file->filename, $importer->error_directory_path)) {
      $error_msg .= ' '.t('An error occured during file copy to error directory.');
    }
    drupal_set_header('HTTP/1.1 517 '.$error_msg);
  }

  return $xml_file;
}

/**
 * Process a xml file
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
function wallymport_process_xmlfile($xml_file, $importer){
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
    
    //If you edit the root of the dom, do not forget to reload the dom ($content = dom->saveXML(); $dom->LoadXML(content))
    module_invoke_all('wallymport_process', $dom, $log, $importer, 'before_xsd_validation', '');
    
    if (!$dom->schemaValidate($importer->xsd_path)){
      dsm('XML not VALIDATED');
      $error_msg = t('The XML file (!xml_file) isn\'t validate by the XSD file', array('!xml_file' => $xml_file));
      $return = array('error' => TRUE, 'error_msg' => $error_msg);
    } else {
      module_load_include('inc', 'wallymport', 'includes/wallymport.fields');

      foreach ($dom->childNodes as $child){
        $return = call_user_func("wimprt_process_entrypoint_generic", $dom, &$log, $importer);
      }
      if ($return['log']['error'] == TRUE){
        $return['error'] = TRUE;
        $return['error_msg'] = $return['log']['error_msg'];
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
function wimprt_set_callback($dom, $importer, $callback = NULL){
  $function_exist = FALSE;
  foreach ($importer->importer_functions as $importer_function){
    if (function_exists($importer_function.'_set_callback')){
      $callback = call_user_func($importer_function."_set_callback", $dom, $callback);
      $function_exist = TRUE;
      break;
    } 
  }
  if (!$function_exist) {
    if ($callback == NULL){
      $callback = strtolower($dom->nodeName);
    }
  }
  return strtolower($callback);
}

/**
 * Call the fonction who will load or create the node
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
function wimprt_node_create_or_load($dom, $importer, $callback, &$log){
  $node = new stdClass;
  foreach ($importer->importer_functions as $importer_function){
    if (function_exists($importer_function.'_node_create_or_load_'.$callback)){
      $node = call_user_func($importer_function."_node_create_or_load_".$callback, $dom, &$log, $importer);
      break;
    }
  }
  return $node;
}
