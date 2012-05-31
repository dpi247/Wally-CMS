<?php

function wallyedit_callback_preview($prenode, $dest = 'disabled'){
  if ($prenode->form_state['DoNotPopulate'] === TRUE) {
    $node = $prenode->form_state[$prenode->form_state['type']]['#node'];
  } else {
    $node = $prenode->ReturnPopulated();
  }

  $node->preview=TRUE;
  if ($dest == 'disabled') {
    return NULL;
  } elseif ($dest == 'no_dest') {
    module_load_include('inc', 'ctools', 'page_manager/plugins/tasks/node_view');
    $node->DoNotPopulate = TRUE;
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
    $preview_context = new stdClass();
    $preview_context->wallyedit_preview = $node->field_destinations[$dest];
    $preview_context->wallyedit_preview['key'] = $dest;
    $preview_context->wallyedit_preview['cache name'] = $prenode->cache_name;
    $contexts['wallyedit_preview'] = $preview_context;
    
    if (empty($contexts)) {
      return drupal_not_found();
    }
    $output = ctools_context_handler_render($task, '', $contexts, array(array($node->field_destinations[$dest]['tid']), 0, 'page'));
    
    // Hide messages before return
    //drupal_get_messages();
    
    if ($output !== FALSE) {
      return $output;
    }
  }

  // Otherwise, fall back.
  module_load_include('inc', 'taxonomy', 'taxonomy.pages');
  return taxonomy_term_page($terms, $depth, $op);
}
