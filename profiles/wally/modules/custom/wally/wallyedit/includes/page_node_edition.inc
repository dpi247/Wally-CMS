<?php

function wallyedit_page_node_edition($prenode, $js = FALSE, $current_profile = 0){
  if ($current_profile == 0) {
    drupal_set_message(t('Profile not set.'), 'error');
    drupal_goto('node/'.$prenode->cache_name.'/edit');
  }
  
  global $custom_theme;
  $custom_theme=variable_get('admin_theme','rubik');

  ctools_include('form');
  ctools_include('modal');
  ctools_include('ajax');
  ctools_add_js('ajax-responder');
  ctools_modal_add_js();

  $form_state = $prenode->getFormState();
  $form_state["action"] = '/node/'.$form_state['cache name'].'/edit2/ajax/'.$current_profile;

  if($_POST['form_id']=='views_bulk_operations_form__1'){
    $view = views_get_view('prenode_selectbox_vbo');
    $view->execute_display('default');
  }

  $form = ctools_build_form('wallyedit_form_page_editmode_form', $form_state, $current_profile);
  $form_prev = drupal_get_form('wydit_subform_global_preview_form', $form_state['cache name']);

  if($js){
    //AJAX rendering int the submit function   
  } else{
    return theme('wallyedit_page_editmode', $form, $form_prev, $form_state['cache name']);
  }
}
