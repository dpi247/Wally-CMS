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
  $parents = unserialize($_POST['parents']);
  $cache_name = $_POST['cache name'];
  $coming_form = $_POST['coming_form'];

  $form_state=array(
    'parents'=>$parents,
    'cache name'=>$prenode->cache_name,
    'profile_id'=>$prenode->profile_id,
    'args' => array(
      'coming_form' => $coming_form,
    ),
    'type' => $parents[0],
    'fields_infos' => array('referenceable_types' => array()),
  );

  $form = ctools_build_form('wallyedit_form_nodetypeselector_form', $form_state);

  if($js){
    $commands[] = ctools_ajax_command_html('#wallyedit_container', $form);
    $commands[] = ctools_ajax_command_html('#wallyedit_messages', theme_status_messages());
    ctools_ajax_render($commands);
  }
  else {
    return '';
  }
}
