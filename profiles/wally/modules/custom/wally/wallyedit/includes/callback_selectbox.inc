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

  
    
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
 // $form = ctools_build_form('wallyedit_editmode_selectbox_form', $form_state);
  
     
     /*
  
  if($js){

views_add_js('ajax_view');
    $views= views_embed_view('test_vbo', 'default', 6,64);
    
  
  $args=array(6,64);
  $view = views_get_view('test_vbo');

  
  
    $conf=array(
    "override_pager_settings"=>0,
  'use_pager'=>0,
  'nodes_per_page'=>20,
  "pager_id"=>'',
  );
  $block=views_content_views_content_type_render('test_vbo',$conf,array(),array());
    
     views_ajax();
  //  $commands[] = ctools_ajax_command_html('#wallyedit_container',$block->content);  
    
   // $commands[] = ctools_ajax_command_html('#wallyedit_messages', theme_status_messages());
   // ctools_ajax_render($commands); 
     
  }
  else{
  return '';  
//    return theme('wallyedit_page_node_edition',$form,$prenode);
  }
  */
}
