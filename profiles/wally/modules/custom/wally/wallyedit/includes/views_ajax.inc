<?php

/**
 * Menu callback to load a view via AJAX.
 */
function wallyedit_views_ajax() {
  if (isset($_REQUEST['view_name']) && isset($_REQUEST['view_display_id'])) {
    $name = $_REQUEST['view_name'];
    $display_id = $_REQUEST['view_display_id'];
    $path = isset($_REQUEST['view_path']) ? $_REQUEST['view_path'] : NULL;
    $dom_id = isset($_REQUEST['view_dom_id']) ? intval($_REQUEST['view_dom_id']) : NULL;
    $pager_element = isset($_REQUEST['pager_element']) ? intval($_REQUEST['pager_element']) : NULL;
    $args = explode('/', $_REQUEST['view_args']);

    views_include('ajax');

    $object = new stdClass();
    $object->status = FALSE;
    $object->display = '';

    $view_path = explode('/', $_REQUEST['view_path']);
    if ($view_path[0] == 'admin' || (variable_get('node_admin_theme', '0') && $view_path[0] == 'node' && ($view_path[1] == 'add' || $view_path[1] == 'add2' || $view_path[2] == 'edit' || $view_path[2] == 'edit2'))) {
      global $custom_theme;
      $custom_theme = variable_get('admin_theme', 'rubik');
    }

    // Load the view.
    if ($view = views_get_view($name)) {
      if ($view->access($display_id)) {
        $cache_name = $args[0];
        $ref_parents = explode('__', str_replace('-', '_', $args[1]));
        $parents = explode('__', str_replace('-', '_', $args[2]));
        wydit_process_node_selector_view($view, $ref_parents, $cache_name, $parents);

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
          $object->display .= $view->preview($display_id);
        } else {
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
