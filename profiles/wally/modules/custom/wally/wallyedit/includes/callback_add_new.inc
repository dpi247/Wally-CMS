<?php

function wallyedit_callback_add_new($prenode, $js = FALSE, $current_profile = 0){
  global $custom_theme;
  $custom_theme=variable_get('admin_theme','rubik');

  _wydit_ctools_include();
  
  $form_state=$prenode->getFormState();
  $form = ctools_build_form('wallyedit_form_editmode_add_form', $form_state, $current_profile);
  
  if($js) {
    $commands[] = ctools_ajax_command_html('#wallyedit_messages', theme_status_messages());
    ctools_ajax_render($commands); 
  } else {
    return '';
  }
}
