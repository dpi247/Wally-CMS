<?php
// $Id: content.node_form.inc,v 1.7.2.21 2009/07/18 00:40:20 markuspetrux Exp $

// PATCH DLA - EXXODUS - AUDAXIS
//
// Change the default beavure of CCK multiple value. By default multiple field show 2 empty field
// 1 for the current data (emppty and 1 for more ...)
// I patch the code to show up only 1 field if empty, 2 if there is already a value in the 1st fieldd

/**
 * @file
 * Create fields' form for a content type.
 *
 * Each field defines its own component of the content entry form, via its
 * chosen widget.
 */
function wallyedit_form(&$form, &$form_state) {
  $type = content_types($form['type']['#value']);
  foreach ($type['fields'] as $field_name => $field) {
    $form['#field_info'][$field['field_name']] = $field;
    $form += (array) content_field_form($form, $form_state, $field);
  }
  return $form;
}

/**
 * Menu callback for AHAH addition of new empty widgets.
 */
function wallyedit_add_more_js($type_name_url, $field_name) {
  module_load_include('module', 'content');
  module_load_include('inc', 'content', 'includes/content.node_form');
  
  $type = content_types($type_name_url);
  $field = content_fields($field_name, $type['type']);

  if (($field['multiple'] != 1) || empty($_POST['form_build_id'])) {
    // Invalid request.
    drupal_json(array('data' => ''));
    exit;
  }
  
  // Retrieve the cached form.
  $form_state = array('submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  $form = form_get_cache($form_build_id, $form_state);
  if (!$form) {
    // Invalid form_build_id.
    drupal_json(array('data' => ''));
    exit;
  }
  
  // We don't simply return a new empty widget to append to existing ones, because
  // - ahah.js won't simply let us add a new row to a table
  // - attaching the 'draggable' behavior won't be easy
  // So we resort to rebuilding the whole table of widgets including the existing ones,
  // which makes us jump through a few hoops.

  // The form that we get from the cache is unbuilt. We need to build it so that
  // _value callbacks can be executed and $form_state['values'] populated.
  // We only want to affect $form_state['values'], not the $form itself
  // (built forms aren't supposed to enter the cache) nor the rest of $form_data,
  // so we use copies of $form and $form_data.
  $form_copy = $form;
  $form_state_copy = $form_state;

  $form_copy['#post'] = array();
  
  form_builder($_POST['form_id'], $form_copy, $form_state_copy);
  // Just grab the data we need.
 
  $form_state['values'] = $form_state_copy['values'];
  
  // Reset cached ids, so that they don't affect the actual form we output.
  form_clean_id(NULL, TRUE);

  $q = $_GET['q'];
  $path = array_slice(explode('/', $q), 4);
  $path = array_merge(array($type_name_url), $path);
  
  $form_element = $form;
  foreach($path as $path_item){
    $form_element = $form_element[$path_item];
  }
  $parents = $form_element['#parents'];
  
  $form_state_ref = $form_state['values'];
  $_POST_ref = $_POST;
  foreach($parents as $path_item) {
    $form_state_array[] = $form_state_ref[$path_item];
    $form_state_ref = &$form_state_ref[$path_item];
    $_POST_ref = &$_POST_ref[$path_item];
  }

  /**********************/
  // Sort the $form_state['values'] we just built *and* the incoming $_POST data
  // according to d-n-d reordering.
  
  unset($form_state_ref[$field['field_name'].'_add_more']);
  foreach ($_POST_ref as $delta => $item) {
    $form_state_ref[$delta]['_weight'] = $item['_weight'];
  }
  
  $node_t = (object)$form_state_array[count($form_state_array) - 2];
  $form_t = array();
  $form_t['#node'] = $node_t;
  
  $form_state_ref = _content_sort_items($field, $form_state_ref);
  $_POST_ref = _content_sort_items($field, $_POST_ref);

  // Build our new form element for the whole field, asking for one more element.
  $form_state_ref['item_count'] = array($field_name => count($_POST_ref) + 1);
  
  $temp = array();
  $max_id = max(array_keys($form_state_ref));
  for ($i = 0; $i <= $max_id; $i++) {
    if (!isset($form_state_ref[$i])) {
      $form_state_ref[$i]['#fake_item'] = TRUE;
    }
  }
  $temp['values'][$field_name] = $form_state_ref;
  $form_element = content_field_form( $form_t, $temp, $field);

  // Let other modules alter it.
  drupal_alter('form', $form_element, array(), 'content_add_more_js');

  // Add the new element at the right place in the (original, unbuilt) form.
  $form_element[$field_name]['#parents'] = $parents;
  $redirect_path = $path;
  unset($redirect_path[0]);
  $form_element[$field_name][$field_name.'_add_more']['#submit'][0] = 'wallyedit_add_more_submit_proxy';
  $form_element[$field_name][$field_name.'_add_more']['#ahah']['path'] = 'wallyedit/js_add_more/'.$type_name_url.'/'.$field_name.'/'.implode('/', $redirect_path);
  
  wydit_add_to_form_state($form, $form_element[$field_name], $path);
  
  /**********************/
  // Save the new definition of the form.
  $form_state['values'] = array();
  form_set_cache($form_build_id, $form, $form_state);

  // Build the new form against the incoming $_POST values so that we can
  // render the new element.
  $delta = max(array_keys($_POST_ref)) + 1;
  $_POST_ref[$delta]['_weight'] = $delta;
  
  wydit_add_to_form_state($_POST, $_POST_ref, $path, 1);
  
  $form_state = array('submitted' => FALSE);
  $form += array(
    '#post' => $_POST,
    '#programmed' => FALSE,
  );
  $form = form_builder($_POST['form_id'], $form, $form_state);
  
  // Render the new output.
  $field_form = $form;
  foreach($path as $path_item){
    $field_form = $field_form[$path_item];
  }

  //$field_form[$field_name]['#parents']=$parents;
  $field_form['#parents'] = $parents;
  $field_form[$field_form['#field_name'].'_add_more']['#submit'][0] = 'wallyedit_add_more_submit_proxy';
  $field_form[$field_form['#field_name'].'_add_more']['#ahah']['path'] = 'wallyedit/js_add_more/'.$type_name_url.'/'.$field_name.'/'.implode('/', $parents);
  
  // We add a div around the new content to receive the ahah effect.
  $field_form[$delta]['#prefix'] = '<div class="ahah-new-content">'. (isset($field_form[$delta]['#prefix']) ? $field_form[$delta]['#prefix'] : '');
  $field_form[$delta]['#suffix'] = (isset($field_form[$delta]['#suffix']) ? $field_form[$delta]['#suffix'] : '') .'</div>';
  // Prevent duplicate wrapper.
  unset($field_form['#prefix'], $field_form['#suffix']);

  // If a newly inserted widget contains AHAH behaviors, they normally won't
  // work because AHAH doesn't know about those - it just attaches to the exact
  // form elements that were initially specified in the Drupal.settings object.
  // The new ones didn't exist then, so we need to update Drupal.settings
  // by ourselves in order to let AHAH know about those new form elements.
  $javascript = drupal_add_js(NULL, NULL);
  $output_js = isset($javascript['setting']) ? '<script type="text/javascript">jQuery.extend(Drupal.settings, '. drupal_to_js(call_user_func_array('array_merge_recursive', $javascript['setting'])) .');</script>' : '';

  $output = theme('status_messages') . drupal_render($field_form) . $output_js;

  // Using drupal_json() breaks filefield's file upload, because the jQuery
  // Form plugin handles file uploads in a way that is not compatible with
  // 'text/javascript' response type.
  $GLOBALS['devel_shutdown'] =  FALSE;
  print drupal_to_js(array('status' => TRUE, 'data' => $output));
  exit;
}
