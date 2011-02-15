<?php

function wallyedit_callback_typeselector($prenode,$js=FALSE){
  global $custom_theme;
  $custom_theme=variable_get('admin_theme','rubik');

  ctools_include('form');
  ctools_include('modal');
  ctools_include('ajax');
  ctools_add_js('ajax-responder');
  ctools_modal_add_js();
  
  
  //@todo: DIRTY HACK TO REMOVE !!!
  $parents=unserialize($_POST['parents']);
  $cache_name=$_POST['cache_name'];
  $form_state=array(
    'parents'=>$parents,
    'cache name'=>$cache_name,
  );
  //dsm($form_state);
  // $form_state=array();;
    
  $form = ctools_build_form('wallyedit_form_nodetypeselector_form', $form_state);
  
  
  if($js){
    $commands[] = ctools_ajax_command_html('#wallyedit_messages', theme_status_messages());
    ctools_ajax_render($commands); 
     
  }
  else{
  return '';  
//    return theme('wallyedit_page_node_edition',$form,$prenode);
  }
}
