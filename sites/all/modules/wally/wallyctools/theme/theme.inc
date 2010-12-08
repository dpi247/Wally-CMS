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
  $vars['destination']=$view->display_handler->handlers['argument']['field_testdes_tid']->value[0]  ;
  $vars['target']=$view->display_handler->handlers['filter']['field_testdes_target']->value;
  
  
}



/**
* Theme preprocess function for views-view-row-unformatted.tpl.php
*/
function template_preprocess_wallyctools_row_redacblock(&$vars) {
  
  $view     = &$vars['view'];
  $options  = $view->style_handler->options;
  $handler  = $view->style_handler;
  $vars['destination']=$view->display_handler->handlers['argument']['field_testdes_tid']->value[0]  ;
  $vars['target']=$view->display_handler->handlers['filter']['field_testdes_target']->value;
  $vars['layout']=wallyctools_api_getlayoutfromdestinationandtarget($node,$vars['destination'],$vars['target']);
  
  
  
  
  $options = $vars['options'];

  // Make sure the variables are defined.
  $vars['node'] = '';
  $vars['comments'] = '';

  $nid = $vars['row']->{$vars['field_alias']};

  if (!is_numeric($nid)) {
    return;
  }

  $node = node_load($nid);

  
  if (empty($node)) {
    return;
  }

  $node->view = $vars['view'];
  $node->node_view_layout=$vars['layout'];
  $node->node_view_target=$vars['target'];
  $node->node_view_destination=$vars['destination'];
  
    
  
  $vars['node'] = node_view($node, $options['build_mode'] == 'teaser', FALSE, $options['links']);

  if (!empty($options['comments']) && function_exists('comment_render')) {
    $vars['comments'] = comment_render($node);
  }
}