<?php

function wallyedit_page_node_creation($type, $js = FALSE, $current_profile = 0){
  if ($current_profile == 0) {
    drupal_set_message(t('Profile not set.'), 'error');
    drupal_goto('node/add/'.str_replace('-','_',$type));
  }

  global $custom_theme;
  $custom_theme=variable_get('admin_theme','rubik');

  ctools_include('form');
  ctools_include('modal');
  ctools_include('ajax');
  ctools_add_js('ajax-responder');
  ctools_modal_add_js();
  
  $node->type = str_replace('-','_',$type);
  $prenode = new Prenode(NULL, FALSE, $current_profile);
  $prenode->setNode($node);

  //Cuz we set the node after the build we need to rebuild the form_state
  $form_state = $prenode->getFormState(TRUE);
  if ($_POST['form_id']=='views_bulk_operations_form__1') {
    $view = views_get_view('prenode_selectbox_vbo');
    $view->execute_display('default');
  }

  $form = ctools_build_form('wallyedit_form_page_createmode_form', $form_state);

  if($js) {
    //AJAX rendering in the submit function   
  } else {
    return theme('wallyedit_page_createmode', $form);
  }
}
