<?php
/** Implementation of views_row_plugin
*
*/
class wallyctools_views_plugin_row_redacblock extends views_plugin_row {
  function theme_functions() {
    $suggestions=views_theme_functions($this->definition['theme'], $this->view, $this->display);
    $vars['destination']=$this->view->display_handler->handlers['argument']['field_destinations_tid']->value[0]  ;
    $vars['target']=$this->view->display_handler->handlers['filter']['field_destinations_target']->value;
    foreach($suggestions as $sugestion){
      $suggestions[]=$sugestion.'___'.$vars['destination'];
      $suggestions[]=$sugestion.'___'.$vars['target'];
      $suggestions[]=$sugestion.'___'.$vars['destination'].'___'.$vars['target'];
    }
    
    return $suggestions;
  }
  
  
  /*
 // Basic properties that let the row style follow relationships.
  var $base_table = 'node';
  var $base_field = 'nid';

  function init(&$view, &$display, $options = NULL) {
    parent::init($view, $display, $options);
    // Handle existing views with the deprecated 'teaser' option.
    if (isset($this->options['teaser'])) {
      $this->options['build_mode'] = $this->options['teaser'] ? 'teaser' : 'full';
    }
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['build_mode'] = array('default' => 'teaser');
    $options['links'] = array('default' => TRUE);
    $options['comments'] = array('default' => FALSE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // CCK holds the registry of available build modes, but can hardly
    // push them as options for the node row style, so we break the normal
    // rule of not directly relying on non-core modules.
    if ($modes = module_invoke('content', 'build_modes')) {
      $options = array();
      foreach ($modes as $key => $value) {
        if (isset($value['views style']) && $value['views style']) {
          $options[$key] = $value['title'];
        }
      }
    }
    else {
      $options = array(
        'teaser' => t('Teaser'),
        'full' => t('Full node')
      );
    }
    $form['build_mode'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#title' => t('Build mode'),
      '#default_value' => $this->options['build_mode'],
     );
    $form['links'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display links'),
      '#default_value' => $this->options['links'],
    );
    $form['comments'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display node comments'),
      '#default_value' => $this->options['comments'],
    );
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
  
  /**
   * Render a row object. This usually passes through to a theme template
   * of some form, but not always.
   */
  function render($row) {
    return theme($this->theme_functions(), $this->view, $this->options, $row, $this->field_alias);
  }
  
  
}