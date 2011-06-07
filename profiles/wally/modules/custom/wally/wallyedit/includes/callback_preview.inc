<?php

function wallyedit_callback_preview($prenode, $dest = 'disabled'){
  if ($prenode->form_state['DoNotPopulate'] === TRUE) {
    $node = $prenode->form_state[$prenode->form_state['type']]['#node'];
  } else {
    $node = $prenode->ReturnPopulated();
  }
  
  if ($dest == 'disabled') {
    return null;
  } elseif($dest == 'no_dest') {
    module_load_include('inc', 'ctools', 'page_manager/plugins/tasks/node_view');
    $node->DoNotPopulate = true;
    return page_manager_node_view($node);
  } else {
    // While we ordinarily should never actually get feeds through here,
    // just in case
    // Load my task plugin
    $task = page_manager_get_task('term_view');

    // Load the term into a context.
    ctools_include('context');
    ctools_include('context-task-handler');
    $contexts = ctools_context_handler_get_task_contexts($task, '', array($node->field_destinations[$dest]['tid'], 0));

    if (empty($contexts)) {
      return drupal_not_found();
    }

    $output = wallyedit_callback_preview_ctools_context_handler_render($prenode, $node, $dest, $task, '', $contexts, array($node->field_destinations[$dest]['tid'], 0, 'page'));
    
    if ($output !== FALSE) {
      return $output;
    }
  }

  // Otherwise, fall back.
  module_load_include('inc', 'taxonomy', 'taxonomy.pages');
  return taxonomy_term_page($terms, $depth, $op);
}

function wallyedit_callback_preview_ctools_context_handler_render($prenode, $node, $dest, $task, $subtask, $contexts, $args, $page = TRUE) {
  // Load the landlers, choosing only enabled handlers.
  $handlers = page_manager_load_sorted_handlers($task, $subtask ? $subtask['name'] : '', TRUE);

  // Try each handler.
  foreach ($handlers as $handler) {
    if ($function = page_manager_get_renderer($handler)) {
      if ($function = 'panels_panel_context_render')
        $function = 'wallyedit_callback_preview_'.$function;
      if ($info = $function($prenode, $node, $dest, $handler, $contexts, $args)) {
        drupal_alter('ctools_render', $info, $page, $args, $contexts, $task, $subtask, $handler);
        // If we don't own the page, let the caller deal with rendering.
        if (!$page) {
          return $info;
        }

        if ($subtask) {
          $task_name = page_manager_make_task_name($task['name'], $subtask['name']);
        }
        else {
          $task_name = $task['name'];
        }

        page_manager_get_current_page(array(
          'name' => $task_name,
          'task' => $task,
          'subtask' => $subtask,
          'contexts' => $contexts,
          'arguments' => $args,
          'handler' => $handler,
        ));

        if (!empty($info['response code']) && $info['response code'] != 200) {
          switch ($info['response code']) {
            case 403:
              return MENU_ACCESS_DENIED;
            case 404:
              return MENU_NOT_FOUND;
            case 301:
            case 302:
            case 303:
            case 304:
            case 305:
            case 307:
              $info += array(
                'query' => '',
                'fragment' => '',
              );
              return drupal_goto($info['destination'], $info['query'], $info['fragment'], $info['response code']);
            // @todo -- should other response codes be supported here?
          }
        }

        /*
        // Only do this if something hasn't already changed the active menu,
        // such as a book.
        if (menu_get_active_menu_name() == 'navigation') {
          $item = menu_get_item();
          $mlink = db_fetch_object(db_query("SELECT * FROM {menu_links} WHERE link_path = '%s'", $item['href']));

          if ($mlink && isset($mlink->menu_name)) {
            menu_set_active_menu_name($mlink->menu_name);
          }
        }
        */
        foreach (ctools_context_handler_get_task_arguments($task, $subtask) as $id => $argument) {
          $plugin = ctools_get_argument($argument['name']);
          $cid = ctools_context_id($argument, 'argument');
          if (!empty($contexts[$cid]) && ($function = ctools_plugin_get_function($plugin, 'breadcrumb'))) {
            $function($argument['settings'], $contexts[$cid]);
          }
        }

        if (isset($info['title'])) {
          drupal_set_title($info['title']);
        }

        // Only directly output if $page was set to true.
        if (!empty($info['no_blocks'])) {
          print theme('page', $info['content'], FALSE);
          return;
        }
        else {
          return $info['content'];
        }
      }
    }
  }

  return FALSE;
}

function wallyedit_callback_preview_panels_panel_context_render($prenode, $node, $dest, $handler, $base_contexts, $args) {
  // Go through arguments and see if they match.
  ctools_include('context');
  ctools_include('context-task-handler');
  ctools_include('plugins', 'panels');

  // Add my contexts
  $contexts = ctools_context_handler_get_handler_contexts($base_contexts, $handler);

  if (isset($handler->handler)) {
    ctools_context_handler_pre_render($handler, $contexts, $args);
  }

  // Load the display
  $display = panels_panel_context_get_display($handler);

  $display->context = $contexts;
  $display->args = $args;
  $display->css_id = $handler->conf['css_id'];
  $task_name = page_manager_make_task_name($handler->task, $handler->subtask);

  $display->cache_key = 'panel_context:' . $task_name . ':' . $handler->name;

  // Check to see if there is any CSS.
  if (!empty($handler->conf['css'])) {
    ctools_include('css');
    $css_id = 'panel_context:' . $handler->name;
    $filename = ctools_css_retrieve($css_id);
    if (!$filename) {
      $filename = ctools_css_store($css_id, $handler->conf['css']);
    }
    ctools_css_add_css($filename);
  }

  // With an argument, this actually sets the display.
  panels_get_current_page_display($display);

  $renderer = panels_get_renderer($handler->conf['pipeline'], $display);

  $info = array(
    'content' => wallyedit_callback_preview_panels_render_display($prenode, $node, $dest, $display, $renderer),
    'no_blocks' => !empty($handler->conf['no_blocks']),
  );

  $info['title'] = $display->get_title();

  return $info;
}

function wallyedit_callback_preview_panels_render_display($prenode, $node, $dest, &$display, $renderer = NULL) {
  ctools_include('display-render', 'panels');
  ctools_include('plugins', 'panels');
  ctools_include('context');

  if (!empty($display->context)) {
    if ($form_context = ctools_context_get_form($display->context)) {
      $form_context->form['#theme'] = 'panels_render_display_form';
      $form_context->form['#display'] = &$display;
      $form_context->form['#form_context_id'] = $form_context->id;
      return drupal_render_form($form_context->form_id, $form_context->form);
    }
  }
  return wallyedit_callback_preview_display_render($prenode, $node, $dest, $display, $renderer);
}

function wallyedit_callback_preview_display_render($prenode, $node, $dest, &$display, $renderer = NULL) {
  $layout = panels_get_layout($display->layout);
  if (!$layout) {
    return NULL;
  }

  // If we were not given a renderer object, load it.
  if (!is_object($renderer)) {
    // If the renderer was not specified, default to $this->renderer
    // which is either standard or was already set for us.
    $renderer = panels_get_renderer_handler(!empty($renderer) ? $renderer : $display->renderer, $this);
    if (!$renderer) {
      return NULL;
    }
  }

  $output = '';
  // Let modules act just prior to render.
  foreach (module_implements('panels_pre_render') as $module) {
    $function = $module . '_panels_pre_render';
    $output .= $function($display, $renderer);
  }

  $output .= wallyedit_callback_preview_renderer_render($prenode, $node, $dest, $renderer);

  // Let modules act just after render.
  foreach (module_implements('panels_post_render') as $module) {
    $function = $module . '_panels_post_render';
    $output .= $function($display, $renderer);
  }
  return $output;
}

function wallyedit_callback_preview_renderer_render($prenode, $node, $dest, &$renderer) {
  // Attach out-of-band data first.
  $renderer->add_meta();

  if (empty($renderer->display->cache['method']) || !empty($renderer->display->skip_cache)) {
    return wallyedit_callback_preview_renderer_render_layout($prenode, $node, $dest, $renderer);
  }
  else {
    $cache = panels_get_cached_content($renderer->display, $renderer->display->args, $renderer->display->context);
    if ($cache === FALSE) {
      $cache = new panels_cache_object();
      $cache->set_content(wallyedit_callback_preview_renderer_render_layout($prenode, $node, $dest, $renderer));
      panels_set_cached_content($cache, $renderer->display, $renderer->display->args, $renderer->display->context);
    }
    return $cache->content;
  }
}

function wallyedit_callback_preview_renderer_render_layout($prenode, $node, $dest, &$renderer) {
  if (empty($renderer->prep_run)) {
    $renderer->prepare();
  }
  wallyedit_callback_preview_renderer_render_panes($prenode, $node, $dest, $renderer);
  $renderer->render_regions();

  if ($renderer->admin && !empty($renderer->plugins['layout']['admin theme'])) {
    $theme = $renderer->plugins['layout']['admin theme'];
  }
  else {
    $theme = $renderer->plugins['layout']['theme'];
  }
  $renderer->rendered['layout'] = theme($theme, check_plain($renderer->display->css_id), $renderer->rendered['regions'], $renderer->display->layout_settings, $renderer->display, $renderer->plugins['layout'], $renderer);
  return $renderer->prefix . $renderer->rendered['layout'] . $renderer->suffix;
}

function wallyedit_callback_preview_renderer_render_panes($prenode, $node, $dest, &$renderer) {
  ctools_include('content');

  // First, render all the panes into little boxes.
  $renderer->rendered['panes'] = array();
  foreach ($renderer->prepared['panes'] as $pid => $pane) {
    $content = wallyedit_callback_preview_renderer_render_pane($prenode, $node, $dest, $renderer, $pane);
    if ($content) {
      $renderer->rendered['panes'][$pid] = $content;
    }
  }
  return $renderer->rendered['panes'];
}

function wallyedit_callback_preview_renderer_render_pane($prenode, $node, $dest, &$renderer, &$pane) {
  if ($pane->type !=  'redacblock') {
    return $renderer->render_pane($pane);
  } else {
    return wallyedit_callback_preview_renderer_render_redacblock($prenode, $node, $dest, $renderer, $pane);
  }
}

function wallyedit_callback_preview_renderer_render_redacblock($prenode, $node, $dest, &$renderer, &$pane) {
  if (is_array($pane->type)) {
    $plugin = $pane->type;
  }else{
    $plugin = ctools_get_content_type($pane->type);
  }
  $pane_context = ctools_content_select_context($plugin, $pane->subtype, $pane->configuration, $renderer->display->context);
  $conf = wallyctools_redacblock_content_type_get_conf(wallyctools_redacblock_content_type_content_type($pane->subtype), $pane->configuration);
  $redacblock_name=$conf['content']->name;
  $view = views_get_view('default_destination_view');
  $view->set_display('redacblock_1');
  /*$options=array(
    'field_destinations_target' => array(
      'operator' => '=',
      'value' => $redacblock_name,
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'case' => 1,
      'id' => 'field_destinations_target',
      'table' => 'node_data_field_destinations',
      'field' => 'field_destinations_target',
      'relationship' => 'none',
    ),
  );
  $options=array(
    'operator' => '=',
    'value' => $redacblock_name,
    'group' => '0',
    'exposed' => FALSE,
                'expose' => array(
                  'operator' => FALSE,
                  'label' => '',
  ),
                'case' => 1,
                'id' => 'field_destinations_target',
                'table' => 'node_data_field_destinations',
                'field' => 'field_destinations_target',
                'relationship' => 'none',
  );*/
  $options=array(
    'operator' => '=',
    'value' => $redacblock_name,
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'relationship' => 'none',
  );
  $view->add_item('redacblock_1', 'filter', 'node_data_field_destinations', 'field_destinations_target', $options);
  $view->is_cacheable = FALSE;
  $view->init_handlers();
  $view->set_arguments(array($node->field_destinations[$dest]['tid']));
  if ($conf['url']) {
    $view->override_path = $conf['url'];
  }
  $block = new stdClass();
  $block->module = 'views';
  $block->delta  = $view->name .'-'.  $view->current_display;
  if (!empty($conf['link_to_view'])) {
    $block->title_link = $view->get_url();
  }
  if (!empty($conf['more_link'])) {
    $block->more = array('href' => $view->get_url());
    $view->display_handler->set_option('use_more', FALSE);
  }
  if ($conf['override_pager_settings']) {
    if (method_exists($view, 'init_pager')) {
      // Views 3 version
      $view->set_items_per_page($conf['nodes_per_page']);
      $view->set_offset($conf['offset']);
      $pager = $view->display_handler->get_option('pager');
      if ($conf['use_pager'] && ($pager['type'] == 'none' || $pager['type'] == 'some')) {
        $pager['type'] = 'full';
      }
      elseif (!$conf['use_pager'] && $pager['type'] != 'none' && $pager['type'] != 'some') {
        $pager['type'] = $view->get_items_per_page() ? 'some' : 'none';
      }
      if ($conf['use_pager']) {
        if (!isset($pager['options']['id']) || $pager['options']['id'] != $conf['pager_id']) {
          $pager['options']['id'] = $conf['pager_id'];
        }
      }
      $view->display_handler->set_option('pager', $pager);
    }
    else {
      if (!$view->display_handler->get_option('use_pager') || empty($conf['use_pager'])) {
        $view->display_handler->set_option('use_pager', $conf['use_pager']);
      }
      $view->display_handler->set_option('pager_element', $conf['pager_id']);
      $view->display_handler->set_option('items_per_page', $conf['nodes_per_page']);
      $view->display_handler->set_option('offset', $conf['offset']);
    }
  }
  $view->display[$view->current_display]->display_options['style_plugin'] = 'redacblock_style';
  module_load_include('inc', 'wallyctools', 'plugins/views/wallyctools_views_plugin_style_redacblock');
  $style = new wallyctools_views_plugin_style_redacblock();
  $style->definition = array();
  $style->definition['uses row plugin'] = TRUE;
  $view->wallyedit_preview = $node->field_destinations[$dest];
  $view->wallyedit_preview['key'] = $dest;
  $view->wallyedit_preview['cache_name'] = $prenode->cache_name;
  $style->init($view, $view->display[$view->current_display]);
  $style->view->style_plugin = $style;
  $content = new stdClass();
  $content->content = $style->view->preview();
  
  if (!empty($content->content)) {
    if (!empty($pane->style['style'])) {
      $style = panels_get_style($pane->style['style']);

      if (isset($style) && isset($style['render pane'])) {
        $output = theme($style['render pane'], $content, $pane, $renderer->display, $style);

        // This could be null if no theme function existed.
        if (isset($output)) {
          return $output;
        }
      }
    }

    // fallback
    return theme('panels_pane', $content, $pane, $renderer->display);
  }
}
