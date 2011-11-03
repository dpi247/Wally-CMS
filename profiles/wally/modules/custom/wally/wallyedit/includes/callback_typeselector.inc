<?php

function wallyedit_callback_typeselector($prenode,$js=FALSE){
  global $custom_theme;
  $custom_theme=variable_get('admin_theme','rubik');

  _wydit_ctools_include();

  //@todo: DIRTY HACK TO REMOVE !!!
  $parents = unserialize($_POST['parents']);
  $cache_name = $_POST['cache name'];
  $coming_form = $_POST['coming_form'];

  $type_name = $parents[0];
  module_load_include('inc','wallyedit','includes/page_form_display_tabs');
  $fields = wyditadmin_get_existing_fields($type_name);

  //@todo: remove hard coding of parents
  $field_infos = $fields[$parents[1]];

  $form_state=array(
    'parents'=>$parents,
    'cache name'=>$prenode->cache_name,
    'profile_id'=>$prenode->profile_id,
    'args' => array(
      'coming_form' => $coming_form,
    ),
    'type' => $type_name,
    'fields_infos' => $field_infos,
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
