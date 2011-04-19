<?php

function wallyedit_callback_preview($prenode, $dest = 'no_dest'){
  $node = $prenode->ReturnPopulated();
  if($dest == 'no_dest'){
    return theme('node', $node);
  }else{
    $full_dest = $prenode->node->field_destinations[$dest];
    $view = views_get_view('default_destination_view');
    $view->set_display('redacblock_1');
    module_load_include('inc', 'wallyctools', 'plugins/views/wallyctools_views_plugin_style_redacblock');
    module_load_include('inc', 'views', 'plugins/views_plugin_style');
    $test = new wallyctools_views_plugin_style_redacblock();
    $test->definition = array();
    $view->wallyedit_preview = $node->field_destinations[$dest];
    $test->init($view, $view->display[$view->current_display]);
  
    return $test->view->preview();
  }
  
}
