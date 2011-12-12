<?php

/**
 * Menu callback to load a view via AJAX.
 */
function wallyedit_views_ajax($url_type_name, $cache_name, $url_clicked_parent) {
  if (isset($_REQUEST['view_name']) && isset($_REQUEST['view_display_id'])) {
    $name = $_REQUEST['view_name'];
    $display_id = $_REQUEST['view_display_id'];
    $args = isset($_REQUEST['view_args']) && $_REQUEST['view_args'] !== '' ? explode('/', $_REQUEST['view_args']) : array();
    $path = isset($_REQUEST['view_path']) ? $_REQUEST['view_path'] : NULL;
    $dom_id = isset($_REQUEST['view_dom_id']) ? intval($_REQUEST['view_dom_id']) : NULL;
    $pager_element = isset($_REQUEST['pager_element']) ? intval($_REQUEST['pager_element']) : NULL;
    views_include('ajax');
    $object = new stdClass();

    $object->status = FALSE;
    $object->display = '';

    $arg = explode('/', $_REQUEST['view_path']);

    if ($arg[0] == 'admin' || (variable_get('node_admin_theme', '0') && $arg[0] == 'node' && ($arg[1] == 'add' || $arg[2] == 'edit'))) {
      global $custom_theme;
      $custom_theme = variable_get('admin_theme', '0');
      drupal_add_css(drupal_get_path('module', 'system') .'/admin.css', 'module');
    }
    // Load the view.
    if ($view = views_get_view($name)) {
      if ($view->access($display_id)) {
        $type_name = str_replace('-', '_', $url_type_name);
        $clicked_parent = str_replace('-', '_', $url_clicked_parent);
        module_load_include('inc','wallyedit','includes/page_form_display_tabs');
        $fields = wyditadmin_get_existing_fields($type_name);
  
        //@todo: remove hard coding of parents
        $field_infos=$fields[$clicked_parent];
        $referencables=$field_infos['referenceable_types'];
        foreach($referencables as $value){
          if($value){
            $allowed_types[$value]=$value;
          }
        }
         
  if(count($allowed_types)>1){
    $view->add_item('default','filter','node','type',array('operator' => 'in','value' => $allowed_types, 'exposed' => TRUE,));
    
   }else{
    $view->add_item('default','filter','node','type',array('operator' => 'in','value' => $allowed_types, 'exposed' => FALSE,));
           }
        
        // set the view arguments here, if needed : SET the cache name and the target
        $view->set_arguments(array($cache_name, $clicked_parent));

        // Override the view path, otherwise the path will be 'ACTION_URL' and AJAX will not be take into account
        $view->override_path = 'wallyedit/views/ajax/'.$url_type_name.'/'.$cache_name.'/'.$url_clicked_parent;

        // Fix 'q' for paging.
        if (!empty($path)) {
          $_GET['q'] = $path;
        }

        // Override the display's pager_element with the one actually used.
        if (isset($pager_element)) {
          $view->display[$display_id]->handler->set_option('pager_element', $pager_element);
        }
        // Reuse the same DOM id so it matches that in Drupal.settings.
        $view->dom_id = $dom_id;

        $errors = $view->validate();
        if ($errors === TRUE) {
          $object->status = TRUE;
          $object->title = $view->get_title();
          $object->display .= $view->preview($display_id, $args);
        }
        else {
          foreach ($errors as $error) {
            drupal_set_message($error, 'error');
          }
        }
        // Register the standard JavaScript callback.
        $object->__callbacks = array('Drupal.Views.Ajax.ajaxViewResponse');
        // Allow other modules to extend the data returned.
        drupal_alter('ajax_data', $object, 'views', $view);
      }
    }
    $messages = theme('status_messages');
    $object->messages = $messages ? '<div class="views-messages">' . $messages . '</div>' : '';

    views_ajax_render($object);
  }
}
