<?php

function wallyedit_page_node_edition($prenode,$js=FALSE){
  global $custom_theme;
  $custom_theme=variable_get('admin_theme','rubik');

  ctools_include('form');
  ctools_include('modal');
  ctools_include('ajax');
  ctools_add_js('ajax-responder');
  ctools_modal_add_js();
  dsm(EDIT);
  $form_state=$prenode->getFormState();
  $form_state["action"]= '/node/'.$form_state['cache name'].'/edit2/ajax';
  
  
  if($_POST['form_id']=='views_bulk_operations_form__1'){
    
    $view = views_get_view('test_vbo');
    $view->execute_display('default');
  
  }
  $form = ctools_build_form('wallyedit_form_page_editmode_form', $form_state);
  
  if($js){
    //AJAX rendering int the submit function   
  }
  else{
    return theme('wallyedit_page_editmode',$form,$prenode);
  }
}
