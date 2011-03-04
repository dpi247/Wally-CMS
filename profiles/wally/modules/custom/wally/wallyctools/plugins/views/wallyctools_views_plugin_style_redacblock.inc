<?php
/* 
* Implementation of views_plugin_style
*
*/

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
  /*
  function render() {
     if (empty($this->row_plugin)) {
       vpr('views_plugin_style_default: Missing row plugin');
       return;
     }
     $rows = array();
     foreach ($this->view->result as $row) {

       $rows[] = $this->row_plugin->render($row);
     }
     return theme($this->theme_functions(), $this->view, $this->options, $rows);
   }
   */
   
}
