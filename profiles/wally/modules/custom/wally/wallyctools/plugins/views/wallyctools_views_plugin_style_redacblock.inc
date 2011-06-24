<?php
/* 
* Implementation of views_plugin_style
*
*/
module_load_include('inc', 'views', 'plugins/views_plugin_style');
//module_load_include('inc', 'views', 'plugins/views_plugin_style_default');
class wallyctools_views_plugin_style_redacblock extends views_plugin_style {
 /*
  *  Set default options
  */
  function additional_theme_functions() {
  $funcs = array();
  if (!empty($this->definition['additional themes'])) {
    foreach ($this->definition['additional themes'] as $theme => $type) {
      $funcs[] = views_theme_functions($theme, $this->view, $this->display);
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
  
  function _clean_results($node){
     $extracted = false;
    
    foreach($this->view->result as $key=>$result){
      if($result->nid == $node->nid){
        $extracted = true;
        $result = array_merge(array_slice($this->view->result, 0, $key), array_slice($this->view->result, $key+2));
        $this->view->result = $result;
        break;
      }
    }
    if(!$extracted && $this->view->pager['use_pager']==0)
      array_pop($this->view->result);
  }
  
  function render() {
    if (empty($this->row_plugin)) {
      vpr('views_plugin_style_default: Missing row plugin');
      return;
    }
    $rows = '';
    $prenode = new Prenode($this->view->wallyedit_preview['cache name']);
    $node = $prenode->ReturnPopulated();
    $node->node_view_destination = $this->view->wallyedit_preview['tid'];
    if($this->view->pager['current_page']==0)//useful if more than one view in a page.
      $node->done = false;

    $this->_clean_results($node);
    while(!$node->done){
      foreach($this->view->result as $key=>$row) {
        $cur_dest = $node->field_destinations[$this->view->wallyedit_preview['key']];
        if (!isset($cur_dest['time_rank']) || empty($cur_dest['time_rank']) || $cur_dest['time_rank'] == 0) {
          $items = array($cur_dest);
          _cckdestinations_set_new_time_ranks($node, $items);
          $cur_dest = $items[0];
        }
        $node->field_destinations[$this->view->wallyedit_preview['key']] = $cur_dest;
        if($node->field_destinations[$this->view->wallyedit_preview['key']]['time_rank'] >= $row->node_data_field_destinations_field_destinations_time_rank){
          $node->done = true;
          $result = array_merge(array_slice($this->view->result, 0, $key), array(0=>$node, ), array_slice($this->view->result, $key));
          $this->view->result = $result;
          break;
        }
      }
      $remaining_rows = $this->view->total_rows-($this->view->pager['items_per_page']*($this->view->pager['current_page']+1));
      if(!$node->done){
        if($remaining_rows<=0){ //At the end, node still not inserted and no remaining pages
          $node->done = true;
          $this->view->result[] = $node;
        }else{
          $this->view->pager['current_page'] += 1;
          $this->view->executed = FALSE;
          $this->view->pre_execute();
          $this->view->execute();
          $this->_clean_results($node);
        }
      }
    }
    foreach ($this->view->result as $row) {
      $rows.= $this->row_plugin->render($row);
    }
    return $rows;
    //return theme($this->theme_functions(), $this->view, $this->options, $rows);
  }
}
