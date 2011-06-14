<?php

function wallyedit_callback_selectbox($prenode,$js=FALSE){
  global $custom_theme;
  $custom_theme=variable_get('admin_theme','rubik');

  ctools_include('form');
  ctools_include('modal');
  ctools_include('ajax');
  ctools_add_js('ajax-responder');
  ctools_modal_add_js();
  
  $form_state=array();;
  
  module_load_include('inc','views','/includes/ajax');
  
  $view = views_get_view('test_vbo');
  $view->set_display('default');
  // set the view arguments here, if needed
  $view->set_arguments( array( 'VIEW', 'ARGUMENTS') );

  // Override the view path, otherwise the path will be 'ACTION_URL'
  $view->override_path = 'views/ajax';
  $output = $view->preview();
  return drupal_json(array('view_output' => $output));
}
