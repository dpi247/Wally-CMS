<?php
/** Implementation of views_row_plugin
 *
 */
class wallyctools_views_plugin_row_redacblock extends views_plugin_row {
  var $base_table = 'node';
  var $base_field = 'nid';
  function theme_functions() {
    $suggestions=views_theme_functions($this->definition['theme'], $this->view, $this->display);
    $vars['destination']=$this->view->display_handler->handlers['argument']['field_destinations_tid']->value[0]  ;
    $vars['target']=$this->view->display_handler->handlers['filter']['field_destinations_target']->value[0];
    foreach($suggestions as $sugestion){
      $suggestions[]=$sugestion.'___'.$vars['destination'];
      $suggestions[]=$sugestion.'___'.$vars['target'];
      $suggestions[]=$sugestion.'___'.$vars['destination'].'___'.$vars['target'];
      $suggestions[]=$sugestion.'___'.$vars['destination'].'___'.$vars['target'];
      if($this->view->split_index_count){
        $suggestions[]=$sugestion.'___'.$vars['destination'].'___'.$vars['target'].'____split';
        if($this->view->split_index_count==$this->view->split_total_count){
          $suggestions[]=$sugestion.'___'.$vars['destination'].'___'.$vars['target'].'____split_____split-last';;
        }
        if($this->view->split_index_count=="1"){
          $suggestions[]=$sugestion.'___'.$vars['destination'].'___'.$vars['target'].'____split_____split-first';
        }
      }
    }
    return $suggestions;
  }

  /**
   * Render a row object. This usually passes through to a theme template
   * of some form, but not always.
   */
  function render($row) {
    return theme($this->theme_functions(), $this->view, $this->options, $row, $this->field_alias);
  }


}