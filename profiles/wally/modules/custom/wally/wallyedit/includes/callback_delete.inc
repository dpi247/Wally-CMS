<?php

function wallyedit_callback_delete($prenode, $current_profile = 0){
  if ($current_profile == 0) {
    drupal_set_message(t('Profile not set.'), 'error');
    drupal_goto('node/'.$prenode->cache_name.'/edit');
  }
  
  global $custom_theme;
  $custom_theme=variable_get('admin_theme','rubik');

  _wydit_ctools_include();
  
  $form_state = $prenode->getFormState();
  $form_state["action"] = '/node/'.$form_state['cache name'].'/edit2/ajax/'.$current_profile;
  
  if ($_POST['confirm_delete']) {
    prenode_delete($form_state);
    node_delete($form_state['cache name']);
    $commands[] = ctools_ajax_command_redirect('');
  } else {
    if($_POST['form_id']=='views_bulk_operations_form__1'){
      $view = views_get_view('prenode_selectbox_vbo');
      $view->execute_display('default');
    }

    $form = ctools_build_form('wallyedit_form_page_editmode_form', $form_state, $current_profile);
    $form_prev = drupal_get_form('wydit_subform_global_preview_form', $form_state['cache name']);
  
    //AJAX rendering int the submit function
    $commands[] = ctools_ajax_command_html('#wallyedit_container', $form);
    $commands[] = ctools_ajax_command_html('#wallyedit_preview_container', $form_prev);
    $commands[] = ctools_ajax_command_html('#wallyedit_messages', theme_status_messages());
  }
  
  ctools_ajax_render($commands);
}