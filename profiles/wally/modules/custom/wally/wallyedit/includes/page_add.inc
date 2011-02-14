<?php

function wallyedit_page_add($type,$js=FALSE){
  global $custom_theme;
  $custom_theme=variable_get('admin_theme','rubik');

  ctools_include('form');
  ctools_include('modal');
  ctools_include('ajax');
  ctools_add_js('ajax-responder');
  ctools_include('object-cache');
  ctools_modal_add_js();
 
  
  $node->type=str_replace('-','_',$type);
  
  $prenode=new Prenode(arg(4));
  $prenode->setNode($node);

  //Cuz we set the node after the build we need to rebuild the form_state
  $form_state=$prenode->getFormState(TRUE);
  
  
  $form = ctools_build_form('wallyedit_form_page_createmode_form', $form_state);
  
  if($js){
    
    //AJAX rendering int the submit function   
  }
  else{
    return theme('wallyedit_page_createmode',$form,$prenode);
  }
}
