<?php

function wallyedit_callback_delete($prenode, $js = FALSE, $current_profile = 0){
  if ($current_profile == 0) {
    drupal_set_message(t('Profile not set.'), 'error');
    drupal_goto('node/'.$prenode->cache_name.'/edit');
  }

  $js = $js ? 'ajax' : 0;
  if ($_POST['confirm_delete']) {
    $form_state = $prenode->getFormState();
    prenode_delete($form_state);
    node_delete($form_state['cache name']);
    if ($js) {
      wydit_ctools_include();
      $commands[] = ctools_ajax_command_redirect('');
      ctools_ajax_render($commands);
    } else {
      drupal_goto();
    }
  } else {
    module_load_include('inc', 'wallyedit', '/includes/callback_edition');
    if ($return = wallyedit_callback_node_edition($prenode, $js, $current_profile, TRUE, '/node/@cache_name/edit2/@js/@current_profile')) {
      return $return;
    }
  }
}
