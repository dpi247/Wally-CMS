<?php 

class panels_renderer_cpsprint extends panels_renderer_standard {
  /**
  * Receive and store the display object to be rendered.
  *
  * This is a psuedo-constructor that should typically be called immediately
  * after object construction.
  *
  * @param array $plugin
  *   The definition of the renderer plugin.
  * @param panels_display $display
  *   The panels display object to be rendered.
  */
  function init($plugin, &$display) {
    $this->plugin = $plugin;
    $layout = panels_get_layout($display->layout);
    
    //Change de layout .tpl in case of sendprint
    if (arg(3) == 'sendprint'){
      $layout['file'] = $layout['theme'].'xml.inc';
      $layout['theme'] = $layout['theme'].'xml'; 
    }
    
    $this->display = &$display;
    $this->plugins['layout'] = $layout;
    if (!isset($layout['panels'])) {
      $this->plugins['layout']['panels'] = panels_get_regions($layout, $display);
    }
  
    if (empty($this->plugins['layout'])) {
      watchdog('panels', "Layout: @layout couldn't been found, maybe the theme is disabled.", array('@layout' => $display->layout));
    }
    //Add the panel information to the panel pane
    foreach ($this->display->content as $content_key => $content_value){
      $this->display->content[$content_key]->configuration['cpsprint'] = TRUE; 
      $this->display->content[$content_key]->configuration['panel'] = $content_value->panel;
    }
  }
  /**
  * Render a pane using its designated style.
  *
  * This method also manages 'title pane' functionality, where the title from
  * an individual pane can be bubbled up to take over the title for the entire
  * display.
  *
  * @param stdClass $pane
  *  A Panels pane object, as loaded from the database.
  */
  function render_pane(&$pane) {
    $content = $this->render_pane_content($pane);
    //return content without any theme function
    if (!empty($content->content)) {
      return $content->content;
    }
  }
}