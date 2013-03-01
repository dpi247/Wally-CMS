<?php

function wallyedit_callback_node_edition($prenode, $js = FALSE, $current_profile = 0, $preview = FALSE, $action = NULL, $form_id = 'wallyedit_form_node_edition_form'){
  if ($current_profile == 0) {
    drupal_set_message(t('Profile not set.'), 'error');
    drupal_goto('node/'.$prenode->cache_name.'/edit');
  }
  if ($_POST['form_id'] == 'views_bulk_operations_form_prenode_selectbox_vbo_default') {
    module_load_include('inc', 'wallyedit', 'wallyedit.forms');
    $view = views_get_view('prenode_selectbox_vbo', TRUE);
    
    //Construct ref parents
    if (sizeof($prenode->form_state['parents']) > 3) {
      $tmp_parents = array_slice($prenode->form_state['parents'], 0, -1);
      $tmp_parents[sizeof($tmp_parents)] = 'type';
      $field_type = wydit_get_target_from_parents($prenode->form_state, $tmp_parents);
      $prenode->form_state['ref_parents'] = array_merge(array($field_type), array_slice($prenode->form_state['parents'], sizeof($prenode->form_state['parents']) - 1));
    } else {
      $prenode->form_state['ref_parents'] = $prenode->form_state['parents'];
    }
    //Add a filter and args to the view
    wydit_process_node_selector_view($view, $prenode->form_state['ref_parents'], $prenode->cache_name, $prenode->form_state['parents']);
    
    $view->execute_display('default');
  }

  module_load_include('inc', 'wallyedit', 'wallyedit.forms');
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

  if ($form_state['locked']) {
    $locked = theme('wallyedit_lock', $form_state);
  } else {
    $locked = '';
  }

  if ($form_state['type'] != 'page') {
    variable_set('menu_override_parent_selector', TRUE);
  }
  $form = ctools_build_form($form_id, $form_state, $current_profile);
  if ($preview && sizeof($form_state['parents']) == 1) {
    $form_prev = drupal_get_form('wydit_form_preview_form', $form_state['cache name']);
  } else {
    $form_prev = '';
  }
  if ($form_state['type'] != 'page') {
    variable_set('menu_override_parent_selector', FALSE);
  }

  if($js) {
    //AJAX rendering
    $commands[] = ctools_ajax_command_html('#page-title h2', wydit_get_fil_ariane($form_state));
    $commands[] = ctools_ajax_command_html('#wallyedit_locked', $locked);
    $commands[] = ctools_ajax_command_html('#wallyedit_container', $form);
    $commands[] = ctools_ajax_command_html('#wallyedit_preview_container', $form_prev);
    $commands[] = ctools_ajax_command_html('#wallyedit_messages', theme_status_messages());
    ctools_ajax_render($commands);
    return FALSE;
  } else {
    drupal_set_title(wydit_get_fil_ariane($form_state));
    return theme('wallyedit_page_editmode', $locked, $form, $form_prev, theme('status_messages'));
  }
}
