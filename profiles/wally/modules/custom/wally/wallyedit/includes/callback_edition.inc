<?php

function wallyedit_callback_node_edition($prenode, $js = FALSE, $current_profile = 0, $preview = FALSE, $action = NULL, $form_id = 'wallyedit_form_node_edition_form'){
  if ($current_profile == 0) {
    drupal_set_message(t('Profile not set.'), 'error');
    drupal_goto('node/'.$prenode->cache_name.'/edit');
  }

  global $custom_theme;
  $custom_theme = variable_get('admin_theme', 'rubik');
  
  if ($_POST['form_id'] == 'views_bulk_operations_form__1') {
    $view = views_get_view('prenode_selectbox_vbo');
    $view->execute_display('default');
  }

  wydit_ctools_include();

  $form_state = $prenode->getFormState();
  if ($action) {
    $args = array(
      '@cache_name' => $form_state['cache name'],
      '@js' => 'ajax',
      '@current_profile' => $current_profile,
      '@type' => str_replace('_', '-', $form_state['type']),
    );
    $form_state['action'] = wydit_string_args_translate($action, $args);
  }

  $form = ctools_build_form($form_id, $form_state, $current_profile);
  if ($preview && sizeof($form_state['parents']) == 1) {
    $form_prev = drupal_get_form('wydit_form_preview_form', $form_state['cache name']);
  } else {
    $form_prev = '';
  }

  if($js) {
    //AJAX rendering
    $commands[] = ctools_ajax_command_html('#page-title h2', wydit_get_fil_ariane($form_state));
    $commands[] = ctools_ajax_command_html('#wallyedit_container', $form);
    $commands[] = ctools_ajax_command_html('#wallyedit_preview_container', $form_prev);
    $commands[] = ctools_ajax_command_html('#wallyedit_messages', theme_status_messages());
    ctools_ajax_render($commands);
    return FALSE;
  } else {
    drupal_set_title(wydit_get_fil_ariane($form_state));
    return theme('wallyedit_page_editmode', $form, $form_prev, $form_state['cache name']);
  }
}