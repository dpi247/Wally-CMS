<?php

/* 
* Implementation of views_plugin_style
*
*/
module_load_include('inc', 'views', 'plugins/views_plugin_style');
//module_load_include('inc', 'views', 'plugins/views_plugin_style_default');
class wallyctools_views_plugin_style_redacblock extends views_plugin_style {
 /**
  * Set default options
  */
  function additional_theme_functions() {
    $funcs = array();
    if (!empty($this->definition['additional themes'])) {
      foreach ($this->definition['additional themes'] as $theme => $type) {
        $funcs[] = views_theme_functions($theme, $this->view, $this->display);
      }
    }

    if($this->view->split_index_count){
      $vars['destination']=$this->view->display_handler->handlers['argument']['field_destinations_tid']->value[0]  ;
      $vars['target']=$this->view->display_handler->handlers['filter']['field_destinations_target']->value[0];

      foreach($funcs as $func){
        $funcs[]=$func.'___'.$vars['destination'].'___'.$vars['target'].'____split';
        if($this->view->split_index_count==$this->view->split_total_count){
          $funcs[]=$func.'___'.$vars['destination'].'___'.$vars['target'].'____split_____split-last';;
        } 
        if($this->view->split_index_count=="1"){
          $funcs[]=$func.'___'.$vars['destination'].'___'.$vars['target'].'____split_____split-first';
        } 
      }
    }

    return $funcs;
  }
  
  function option_definition() {
    $options = parent::option_definition();

    return $options;
  }
 
  /**
   * Provide a form for setting options.
   *
   * @param array $form
   * @param array $form_state
   */ 
  function options_form(&$form, &$form_state) {
    
  }

  /**
   * Allow the style to do stuff before each row is rendered.
   *
   * @param $result
   *   The full array of results from the query.
   */
  function pre_render($result) {
    if (!empty($this->row_plugin)) {
      $this->row_plugin->pre_render($result);
    }
    
    $view = &$this->view;
    if (isset($view->wallyedit_preview) && empty($view->result)) {
      $view->result[] = array();
    }
  }
  
  function render() {
    if (empty($this->row_plugin)) {
      vpr('views_plugin_style_default: Missing row plugin');
      return;
    }
    
    static $inserted_previews = array();
    static $saved_results = array();

    $rows = '';

    if ($this->view->wallyedit_preview) {
      $preview_infos = $this->view->wallyedit_preview;
      $view_infos = wallyctools_get_view_info_from_redacblock($preview_infos['target']);
      if ($view_infos['view_id'] == $this->view->current_display) {
        module_load_include('inc', 'wallyedit', 'includes/Prenode');
        $prenode = new Prenode($preview_infos['cache name']);
        $node = $prenode->ReturnPopulated();
        $node->node_view_destination = $preview_infos['tid'];
  
        if (!isset($saved_results[$this->view->current_display])) {
          $saved_results[$this->view->current_display] = array();
        }

        // Remove previewed node from results
        $this->_clean_results($node, $saved_results[$this->view->current_display]);

        if (!isset($inserted_previews[$this->view->current_display]) || !$inserted_previews[$this->view->current_display]) {
          if($this->view->pager['current_page'] == 0) {//useful if more than one view in a page.
            $inserted_previews[$this->view->current_display] = FALSE;
          }
  
          // Determine new time_rank
          $this->_insert_node_in_results($node);

          foreach($this->view->result as $key => $row) {
            if($node->field_destinations[$preview_infos['key']]['time_rank'] >= $row->node_data_field_destinations_field_destinations_time_rank){
              $inserted_previews[$this->view->current_display] = TRUE;
              $node->done = TRUE;
              $result = array_merge(array_slice($this->view->result, 0, $key), array(0 => $node), array_slice($this->view->result, $key));
              $this->view->result = $result;
              break;
            }
          }

          if($inserted_previews[$this->view->current_display]) {
            array_push($saved_results[$this->view->current_display], array_pop($this->view->result));
          }
        }
      }
    }

    foreach ($this->view->result as $row_index => $row) {
      $this->view->row_index = $row_index;
      $rows[] = $this->row_plugin->render($row);
    }

    $output .= theme($this->theme_functions(), $this->view, $this->options, $rows, $title);
    return $output;
  }
  
  function _clean_results($node, &$saved_results) {
    // Replace saved results at the beginning of the current results
    while (!empty($saved_results)) {
      array_unshift($this->view->result, array_shift($saved_results));
    }
    foreach($this->view->result as $key => $result) {
      if($result->nid == $node->nid) {
        $result = array_merge(array_slice($this->view->result, 0, $key), array_slice($this->view->result, $key + 1));
        $this->view->result = $result;
        break;
      }
    }
    // Remove and save additionnal results
    while (sizeof($this->view->result) >= $this->view->pager['items_per_page']) {
      array_push($saved_results, array_pop($this->view->result));
    }
  }
  
  function _insert_node_in_results(&$node) {
    $current_results = $this->view->result;
    $item = $preview_infos = $this->view->wallyedit_preview;

    if (!strstr($node->field_destinations[$preview_infos['key']]['rank'], 'DNC')) {
      $defaults = array(
        'sort_method' => 'absolute',
        'behavior' => 'top',
        'reserved' => 5,
        'delta_t' => 1,
      );
      $dest_settings = wally_variable_get('cckdestinations_destination_'.$item['tid'].'_'.$item['target'], $defaults);
      $sort_method = $dest_settings['sort_method'];
  
      if ($sort_method == 'interval') {
        $item['time_rank'] = $pub_date + ($item['rank'] * 60 * 60 * $dest_settings['delta_t']);
      } else {
        // Set params for this destination
        $behavior = $dest_settings['behavior'];
        $reserved = $dest_settings['reserved'] - $this->view->pager['offset'];
        $temp_time_rank = 0;
        $rank = $item['rank'];

        if ($rank == 0) {
          if ($behavior == 'bottom') {
            $rank = $reserved + 1;
          } else {
            $rank = 1;
          }
        } elseif ($rank > $reserved) {
          $rank = $reserved + 1;
        }
        --$rank;
  
        $current_results[$rank - 1]->node_data_field_destinations_field_destinations_time_rank = isset($current_results[$rank - 1]) ? $current_results[$rank - 1]->node_data_field_destinations_field_destinations_time_rank : $current_results[$rank]->node_data_field_destinations_field_destinations_time_rank + 30;
        $item['time_rank'] = ($current_results[$rank - 1]->node_data_field_destinations_field_destinations_time_rank + $current_results[$rank]->node_data_field_destinations_field_destinations_time_rank) / 2;
      }
  
      $node->field_destinations[$preview_infos['key']]['time_rank'] = $item['time_rank'];
    }
  }
}
