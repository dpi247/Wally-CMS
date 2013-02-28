<?php

/**
 * Menu callback to load a view via AJAX.
 */
function wallyedit_views_ajax() {
  if (isset($_REQUEST['view_name']) && isset($_REQUEST['view_display_id'])) {
    $name = $_REQUEST['view_name'];
    $display_id = $_REQUEST['view_display_id'];
    $args = explode('/', $_REQUEST['view_args']);
    $path = isset($_REQUEST['view_path']) ? $_REQUEST['view_path'] : NULL;
    $dom_id = isset($_REQUEST['view_dom_id']) ? preg_replace('/[^a-zA-Z0-9_-]+/', '-', $_REQUEST['view_dom_id']) : NULL;
    $pager_element = isset($_REQUEST['pager_element']) ? intval($_REQUEST['pager_element']) : NULL;

    $ajax_arguments = array(
      'view_name' => $name,
      'view_display_id' => $display_id,
      'view_args' => $_REQUEST['view_args'],
      'view_path' => $path,
      'view_dom_id' => $dom_id,
      'pager_element' => $pager_element,
    );
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
      $view->ajax_path = array($_GET['q'], $ajax_arguments);
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

        $object->status = TRUE;
        $object->display .= $view->preview($display_id);
        // Get the title after the preview call, to let it set
        // up both the view's current display and arguments
        $object->title = $view->get_title();
        // Register the standard JavaScript callback.
        $object->__callbacks = array('Drupal.Views.Ajax.ajaxViewResponse');
        // Allow other modules to extend the data returned.
        drupal_alter('ajax_data', $object, 'views', $view);
      }
    }
    $messages = theme('status_messages');
    $object->messages = $messages ? '<div class="views-messages">' . $messages . '</div>' : '';

    wallyedit_views_ajax_render($object);
  }
}
/**
* Simple render function to make sure output is what we want.
*
* This function renders an object into JSON, and that object contains
* commands to the ajax response parser on the other side. The actual
* commands that can be sent are completely dependent upon the client
* javascript parser, which can be anything, but this function assumes
* that 'display', at least, will be displayed in some kind of ajax
* spot or popup.
*/
function wallyedit_views_ajax_render($output = NULL, $title = NULL, $url = NULL, $js = NULL) {
  if (empty($output)) {
    $output->display = t('Server reports invalid input error.');
    $output->title = t('Error');
  }
  elseif (!is_object($output)) {
    $temp = new stdClass();
    $temp->display = $output;
    $temp->title = $title;
    $temp->url = $url;
    $output = $temp;
  }
  if (!empty($js)) {
    $output->js = $js;
  }

  drupal_json($output);
  exit;
}
