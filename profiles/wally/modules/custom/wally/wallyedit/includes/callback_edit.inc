<?php

function wallyedit_callback_edit($prenode, $js = FALSE, $current_profile = 0){
  global $custom_theme;
  $custom_theme=variable_get('admin_theme','rubik');

  ctools_include('form');
  ctools_include('modal');
  ctools_include('ajax');
  ctools_add_js('ajax-responder');
  ctools_modal_add_js();
  
  $form_state=$prenode->getFormState();
  $form = ctools_build_form('wallyedit_form_editmode_edit_form', $form_state, $current_profile);
  
  if($js) {
    
  } else{
    return '';
  }
}
