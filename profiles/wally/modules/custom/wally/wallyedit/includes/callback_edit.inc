<?php

function wallyedit_callback_edit($prenode,$js=FALSE){
  global $custom_theme;
  $custom_theme=variable_get('admin_theme','rubik');

  ctools_include('form');
  ctools_include('modal');
  ctools_include('ajax');
  ctools_add_js('ajax-responder');
  ctools_modal_add_js();
  
  dsm('EDIT PAGE');
  
  $form_state=$prenode->getFormState();
  //$form_state=array();
  $form = ctools_build_form('wallyedit_form_editnode_form', $form_state);
  
  
  if($js){
   
  }
  else{
  return '';  
//    return theme('wallyedit_page_node_edition',$form,$prenode);
  }
}
