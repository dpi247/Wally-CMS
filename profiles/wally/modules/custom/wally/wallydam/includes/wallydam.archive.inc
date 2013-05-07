<?php 

/**
 *
 * Check if the url exist in the cache and return its content, return FALSE if the url does not exists
 * @param string $url
 */
function wallydam_archive_cacheget($url){
  $return = FALSE;
  if ($cache){
    $return = 'html';
  }
  return $return;
}
/**
 * 
 * Call the DAM, if a zip is returned, import the zip to a node and render de node.
 * @param string $url
 * @param string $last_alias
 * @return String or FALSE if the import fails
 */
function wallydam_archive_getpackagefromurl($url, &$last_alias){

  //Call to the DAM
  $file_name = wallydam_archive_geturl($url);
  
  $content = FALSE;
  if ($file_name){
    $importer_name = wally_variable_get('wallydam_archive_importer', '');
    //Import the zip
    $node = wallydam_archive_importnode($importer_name, $file_name);
    if ($node){
      //Render node
      $content = wallydam_archive_rendernode($node);
      //a modifier, récupérer le last alias du noeud
      $last_alias = $url;
    }
    
  }
  
  return $content;
}
/**
 * 
 * Set the content in cache
 * @param string $url
 * @param string $content
 */
function wallydam_archive_cacheset($url, $content){
  //met en cache
}
/**
 * 
 * Call the dam and save the zip in the importer folder
 * @param string $url
 */
function wallydam_archive_geturl($url){
  
  $retry = wally_variable_get('wallytoolbox_http_request_retry', 1);
  $timeout = wally_variable_get('wallytoolbox_http_request_timeout', 3);
  $request = wally_variable_get('wallydam_archive_geturl', '');
  $args = '?url='.$url;
  $headers = array();
  $data = NULL;
 
  //GET
  $result = drupal_http_request($request.$args, $headers, 'GET', $data, $retry, $timeout);

  $file_name = FALSE;
  if ($result->code == 200){  
    $importer_name = wally_variable_get('wallydam_archive_importer', '');
    $importer = wallymport_getimporterbyname($importer_name);
    
    $file_name = 'dam_archive_'.rand().'.zip';
    
    //Save the file in the importer folder
    file_put_contents($importer->source_directory_path.'/'.$file_name, $result->data);
  }

  return $file_name;
}
/**
 * 
 * Import the zip to a node
 * @param string $importer_name
 * @param string $file_name
 * @return A node object or FALSE if the import fails
 */
function wallydam_archive_importnode($importer_name, $file_name) {
  // Fetch the cron semaphores
  module_load_include('inc', 'wallymport', 'includes/wallymport.process');
 
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
    $importer->wallymport_preview = TRUE;
    $importer->wallymport_preview_norender = TRUE;
    
    if ($importer != NULL){
      $returns = wallymport_process_file($importer, $file_name);
    } else {
      //Unknown importer
      $returns[] = array('error' => TRUE, 'error_msg' => t('Unknown importer'));
    }

    // Release cron semaphore
    wally_variable_del('wallymport_process_file_semaphore_'.$file_name);
  }
  
  $node = FALSE;
  //Keep the node even if it has no destination
  if (isset($returns[0]) && !$returns[0]['error'] || $returns[0]['error_msg'] == 'No destination AT ALL for current package. Package rejected.'){
    $node = $returns[0]['log']['tempnode'];
  }
  return $node;

}
/**
 * 
 * Render the node in the archive page.
 * @param object $node
 * @return string $content
 */
function wallydam_archive_rendernode($node){

  $node->preview = TRUE;

  module_load_include('inc', 'page_manager', 'plugins/tasks/page');
  $page_archive = page_manager_page_load('archive');
  if ($page_archive->default_handlers['page_archive_panel_context']->conf['display'] != NULL){
    //If the page is in file
    $display = $page_archive->default_handlers['page_archive_panel_context']->conf['display'];
  } else {
    //if the page is in db
    $task = page_manager_get_task($page_archive->task);
    $subtask = page_manager_get_task_subtask($task, 'archive');
    $handlers = page_manager_load_sorted_handlers($task, $subtask ? $subtask['name'] : '', TRUE);
    $display = panels_load_display($handlers['page_archive_panel_context']->conf['did']);
  }
  //add the node to the context
  $display->context['context_node_1']->type = 'node';
  $display->context['context_node_1']->data = $node;
  $display->context['context_node_1']->title = $node->title;
  $display->context['context_node_1']->page_title = '';
  $display->context['context_node_1']->identifier = 'Node';
  $display->context['context_node_1']->keyword = 'node';
  $display->context['context_node_1']->restriction = array('type' => array(0 => 'wally_articlepackage'));
  $display->context['context_node_1']->empty = FALSE;
  $display->context['context_node_1']->plugin = 'node';
  $content = panels_render_display($display);
  return $content;
}



