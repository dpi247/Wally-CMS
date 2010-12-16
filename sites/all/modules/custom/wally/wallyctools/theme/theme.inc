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

function wallyctools_views_admin_links_alter(&$vars,$view){
  dsm($vars);
  dsm($view->display_handler->handlers['filter']);
  $handlers=$view->display_handler->handlers;
foreach($handlers['argument'] as $key=>$value){
    if($key=='field_destination_tid'){
      $destination_tid=$value->value[0];
      break;
    }
  }
foreach($handlers['filter'] as $key=>$value){
    if($key=='field_destination_target'){
      dsm($value);
      $target=$value->value;
      break;
    }
  }
  $vars['admin_links_raw'] = array(
        'title' => t('Reorder'),
        'alt' => t("Reorder the view"),
        'href' => "wallyctools/reorder/$destination_tid/$target",
        'fragment' => 'views-tab-' . $view->current_display,
        'query' => drupal_get_destination(),
      );
}

  


/**
* Theme preprocess function for views-view-row-unformatted.tpl.php
*/
function template_preprocess_wallyctools_row_redacblock(&$vars) {
  
  $view     = &$vars['view'];
  $options  = $view->style_handler->options;
  $handler  = $view->style_handler;
  
  
  
  $options = $vars['options'];

  // Make sure the variables are defined.
  $vars['node'] = '';
  $vars['comments'] = '';

  $nid = $vars['row']->{$vars['field_alias']};

  if (!is_numeric($nid)) {
    return;
  }

  $node = node_load($nid);

  $vars['destination']=$view->display_handler->handlers['argument']['field_testdes_tid']->value[0]  ;
  $vars['target']=$view->display_handler->handlers['filter']['field_testdes_target']->value;
  $vars['layout']=wallyctools_api_getlayoutfromdestinationandtarget($node,$vars['destination'],$vars['target']);
  
  if (empty($node)) {
    return;
  }

  $node->view = $vars['view'];
  $node->node_view_layout=str_replace('_','-',$vars['layout']);
  $node->node_view_target=$vars['target'];
  $node->node_view_destination=$vars['destination'];
  
    
  
  $vars['node'] = node_view($node, $options['build_mode'] == 'teaser', FALSE, $options['links']);

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
      "nid            ","title   ","time rank    " 
    );

    $output = theme('table',$header,$table_rows,array('id'=>'example-table'));
    $output .= drupal_render($form);

    // Call add_tabledrag to add and setup the JS for us
    // The key thing here is the first param - the table ID
    // and the 4th param, the class of the form item which holds the weight
    drupal_add_tabledrag('example-table', 'order', 'self', 'weight',NULL,NULL,FALSE);     

    return $output;
  }