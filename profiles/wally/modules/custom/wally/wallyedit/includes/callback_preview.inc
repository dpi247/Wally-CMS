<?php

function wallyedit_callback_preview($prenode, $dest = 'disabled'){
  if($prenode->form_state['DoNotPopulate'] === TRUE){
    $node = $prenode->form_state[$prenode->form_state['type']]['#node'];
  }else{
    $node = $prenode->ReturnPopulated();
  } 
  if($dest == 'disabled'){
    return null;
  }elseif($dest == 'no_dest'){
    module_load_include('inc', 'ctools', 'page_manager/plugins/tasks/node_view');
    $node->DoNotPopulate = true;
    return page_manager_node_view($node);
  }else{
    ctools_include('context');
    ctools_include('context-task-handler');
    ctools_include('plugins', 'panels');
     // Load my task plugin
    $task = page_manager_get_task('term_view');
    // Load the term into a context.
    $args = array($node->field_destinations[$dest]['tid'], 0);
    $contexts = ctools_context_handler_get_task_contexts($task, '', array($node->field_destinations[$dest]['tid'], 0));
    if (empty($contexts)) {
      return drupal_not_found();
    }

    $handlers = page_manager_load_sorted_handlers($task, $subtask ? $subtask['name'] : '', TRUE);

    foreach ($handlers as $handler) {
      if ($function = page_manager_get_renderer($handler)) {
        if ($info = $function($handler, $contexts, $args)) {
          //$test = TRUE;
          // Add my contexts
          $contexts = ctools_context_handler_get_handler_contexts($contexts, $handler);

          
          /*
          // Test.
          if ($test && !ctools_context_handler_select($handler, $contexts)) {
            return;
          }*/

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
          if (empty($renderer->prep_run)) {
            $renderer->prepare();
          }
          $renderer->rendered['panes'] = array();
          foreach ($renderer->prepared['panes'] as $pid => $pane) {
            if($pane->type !=  'redacblock'){
              $content = $renderer->render_pane($pane);
            }else{
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
              $options=array(
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
              $content =  $style->view->preview();
            }
            if ($content) {
              $renderer->rendered['panes'][$pid] = $content;
            }
          }
          $renderer->render_regions();
          if ($renderer->admin && !empty($renderer->plugins['layout']['admin theme'])) {
            $theme = $renderer->plugins['layout']['admin theme'];
          }else{
            $theme = $renderer->plugins['layout']['theme'];
          }
          $renderer->rendered['layout'] = theme($theme, check_plain($renderer->display->css_id), $renderer->rendered['regions'], $renderer->display->layout_settings, $renderer->display, $renderer->plugins['layout'], $renderer);
          return $renderer->prefix . $renderer->rendered['layout'] . $renderer->suffix;
        }
      }
    }
    
    //Following code should be useless, i only renders the view without the blocks around.
    $view = views_get_view('default_destination_view');
    $view->set_display('redacblock_1');
    module_load_include('inc', 'wallyctools', 'plugins/views/wallyctools_views_plugin_style_redacblock');

    $style = new wallyctools_views_plugin_style_redacblock();
    $style->definition = array();
    $style->definition['uses row plugin'] = TRUE;
    $view->wallyedit_preview = $node->field_destinations[$dest];
    $view->wallyedit_preview['cache_name'] = $prenode->cache_name;
    $view->result = array();
    $style->init($view, $view->display[$view->current_display]);

    return $style->view->preview();
  }
}
