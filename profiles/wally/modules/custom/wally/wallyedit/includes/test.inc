<?php
function wallyedit_test($a){

 global $custom_theme;
  $custom_theme=variable_get('admin_theme','rubik');

     ctools_include('object-cache');
    
     
    $form_state = array('ajax' => TRUE);
    $form_state += array(
      're_render' => FALSE,
      'no_redirect' => TRUE,
    );
     $form_state = array(
      'cache name' => 6,
    );
    if($cache = ctools_object_cache_get('prenode', $form_state['cache name'])){
     $form_state=$cache;
    }
    return ''; 
}
