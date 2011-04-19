<?php


/**
 * Theme preprocess function for views-view-json.tpl.php
 *
 * @param array $vars
 */
function template_preprocess_wallyctools_style_redacblock(&$vars) {
  //  $vars['rows']       = !empty($view->result) || !empty($view->style_plugin->definition['even empty']) ? $view->style_plugin->render($view->result) : '';
  $view     = &$vars['view'];
  $options  = $view->style_handler->options;
  $handler  = $view->style_handler;
  $vars['destination']=$view->display_handler->handlers['argument']['field_destinations_tid']->value[0]  ;
  $vars['target']=$view->display_handler->handlers['filter']['field_destinations_target']->value;
}

function wallyctools_views_admin_links_alter(&$vars,$view){
  $handlers=$view->display_handler->handlers;
  foreach($handlers['argument'] as $key=>$value){
    if($key=='field_destinations_tid'){
      $destination_tid=$value->value[0];
      break;
    }
  }
  foreach($handlers['filter'] as $key=>$value){
    if($key=='field_destinations_target'){
      $target=$value->value;
      break;
    }
  }
  foreach ($view->argument as $arg_name => $argument) {
    if (strstr($arg_name, 'field_destination')) {
      $vars['admin_links_raw'] = array(
        'title' => t('Reorder'),
        'alt' => t("Reorder the view"),
        'href' => "admin/wallyctools/reorder/$destination_tid/$target",
        'fragment' => 'views-tab-' . $view->current_display,
        'query' => drupal_get_destination(),
      );
      break;
    }
  }
}


/**
* Theme preprocess function for views-view-row-unformatted.tpl.php
*/
function template_preprocess_wallyctools_row_redacblock(&$vars) {
  $view = &$vars['view'];
  $options = $vars['options'];

  // Make sure the variables are defined.
  $vars['node'] = '';
  $vars['comments'] = '';

  $nid = $vars['row']->{$vars['field_alias']};

  if (!is_numeric($nid)) {
    return;
  }
  
  if($view->wallyedit_preview){
    if(empty($vars['row']->done)){
      $node = node_load($nid);
    }else{
      $node = $vars['row'];
    }
    $vars['layout']=$view->wallyedit_preview['layout'];
    $vars['target']=$view->wallyedit_preview['target'];
    $vars['destination']=$view->wallyedit_preview['tid'];
  }else{
    $node = $vars['row'];
    $vars['destination']=$view->display_handler->handlers['argument']['field_destinations_tid']->value[0];
    $vars['target']=$view->display_handler->handlers['filter']['field_destinations_target']->value;
    $vars['layout']=wallyctools_api_getlayoutfromdestinationandtarget($node,$vars['destination'],$vars['target']);
  }

  if (empty($node)) {
    return;
  }

  $node->view = $vars['view'];
  $node->node_view_layout=str_replace('_','-',$vars['layout']);
  $node->node_view_target=$vars['target'];
  $node->node_view_destination=$vars['destination'];
  
  if (isset($options['build_mode'])) {
		if ($options['build_mode'] == 'teaser') {
  		$teaser = TRUE; 
		} else {
  		$teaser = FALSE; 
		}
  } else {
  	$teaser = FALSE; 
  }

  if (!isset($options['links'])) {
  	$links = TRUE; 
  } else {
  	$links = $options['links']; 
  }
  
	$vars['node'] = node_view($node, $teaser, FALSE, $links);
    
  if (!empty($options['comments']) && function_exists('comment_render')) {
    $vars['comments'] = comment_render($node);
  }
}


function theme_wallyctools_form_reorderdestinationview_form($form){
  //loop through each "row" in the table array
  foreach($form['rows'] as $id => $row){
    //we are only interested in numeric keys
    if (intval($id)){ 
      $this_row = $row['data']['#value'];

      //Add the weight field to the row
      $this_row[] = drupal_render($form['rows'][$id]['weight']);

      //Add the row to the array of rows
      $table_rows[] = array('data'=>$this_row, 'class'=>'draggable');
    }
  }
 
  //Make sure the header count matches the column count
  $header=array(
   "title" ,'order'
  );
  drupal_add_tabledrag('example-table', 'order', 'self', 'weight',NULL,NULL,TRUE);     
  
  $output = theme('table',$header,$table_rows,array('id'=>'example-table'));
  $output .= drupal_render($form);

  // Call add_tabledrag to add and setup the JS for us
  // The key thing here is the first param - the table ID
  // and the 4th param, the class of the form item which holds the weight

  return $output;
}
