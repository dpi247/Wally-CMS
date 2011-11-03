<?php

function wallyedit_callback_selectbox($prenode,$js=FALSE){
  global $custom_theme;
  $custom_theme=variable_get('admin_theme','rubik');

  _wydit_ctools_include();
  
  $form_state=array();;
  
  module_load_include('inc','views','/includes/ajax');
  
  $view = views_get_view('prenode_selectbox_vbo');
  $view->set_display('default');

  // Override the view path, otherwise the path will be 'ACTION_URL'
  $view->override_path = 'views/ajax';
  $output = $view->preview();
  return drupal_json(array('view_output' => $output));
}
