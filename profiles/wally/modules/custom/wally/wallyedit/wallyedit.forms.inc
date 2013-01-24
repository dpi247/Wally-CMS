<?php

module_load_include('inc', 'wallyedit', 'includes/Prenode');
module_load_include('inc', 'wallyedit', 'includes/config_api');
module_load_include('inc', 'node', 'node.pages');

/**
 **************************
 * MAIN NODE EDITION FORM *
 **************************
 */

function wallyedit_form_node_edition_form(&$form_state) {
  if (empty($_POST)) {
    global $user;
    wally_variable_del('wallyedit_current_tab_'.$user->uid);
  }

  $cache_name = $form_state['cache name'];
  $current_profile = $form_state['profile_id'];
  
  list($tmp_fs, $type_name) = wydit_get_tmp_fs_from_form_state($form_state);

  $parents = $form_state['parents'];
  $current_form_state = $form_state;
  $current_form_state[$type_name] = $tmp_fs;
  $current_form_state['type'] = $type_name;

  if (!($node = wydit_get_target_from_parents($form_state, array_merge($parents, array('#node'))))) {
    $node = new stdClass();
    $node->type = $type_name;
  }

  $a = array('#tree' => TRUE);
  $a = drupal_retrieve_form($type_name.'_node_form', $a, $node);
  $a['#tree'] = TRUE;
  drupal_prepare_form($type_name.'_node_form', $a, $current_form_state);

  unset($a['#type']);
  unset($a['form_id']);
  unset($a['#parents']);
  $form = array();
  $form[$type_name] = array('#type' => 'fieldset');
  foreach($a as $element_name => $value){
    $form[$type_name][$element_name] = $value;
  }

  $temp_b = $form[$type_name];
  wydit_prepare_primary_package_form($form[$type_name], $current_form_state, $type_name, $parents, $temp_b);

  if (!$form_state['locked']) {
    $form['save_global'] = array(
      '#type' => 'submit',
      '#wydit_op_type' => 'action',
      '#name' => 'save_global',
      '#value'=> t('Save'),
      '#attributes' => array('class' => 'form-submit ctools-use-ajax'),
      '#next_parents' => (sizeof($parents) == 1) ? $parents : array_slice($parents, 0, sizeof($parents) - 2),
    );
  
    if (sizeof($parents) == 1) {
      $form['publish_global'] = array(
        '#type' => 'submit',
        '#wydit_op_type' => 'action',
        '#name' => 'publish_global',
        '#value' => t('Save and Publish'),
        '#attributes' => array('class' => 'form-submit ctools-use-ajax'),
        '#next_parents' => $parents,
      );
      
      if (isset($current_form_state[$type_name]['#node']->nid) && $current_form_state[$type_name]['#node']->nid == $cache_name) {
        $form['reset_global'] = array(
          '#type' => 'submit',
          '#wydit_op_type' => 'pre_treat',
          '#name' => 'reset_global',
          '#value'=> t('Reset'),
          '#attributes' => array('class' => 'form-submit ctools-use-ajax'),
          '#next_parents' => $parents,
          '#skip validation' => TRUE,
        );
  
        $form['delete_global'] = array(
          '#type' => 'submit',
          '#wydit_op_type' => 'pre_treat',
          '#name' => 'delete_global',
          '#value'=> t('Delete node'),
          '#attributes' => array('class' => 'form-submit ctools-use-ajax'),
          '#next_parents' => $parents,
          '#skip validation' => TRUE,
        );
      }
    } else {
      $form['cancel_global'] = array(
        '#type' => 'submit',
        '#wydit_op_type' => 'pre_treat',
        '#name' => 'cancel_global',
        '#value' => t('Cancel'),
      	'#attributes' => array('class' => 'form-submit ctools-use-ajax'),
        '#next_parents' => array_slice($parents, 0, sizeof($parents) - 2),
        '#skip validation' => TRUE,
      );
    }
  }

  $form['profile_id'] = array('#value' => $current_profile);
  $form['type'] = array('#value' => $type_name);
  $form['current_tab'] = array('#value' => (sizeof($parents) == 1) ? $form_state['current_tab'] : NULL);
  $form['#tree'] = TRUE;
  $form['#theme'] = array('wallyedit_node_form');
  $form['#validate'] = array('wallyedit_form_node_form_validate');
  $form['#submit'] = array('wallyedit_form_node_form_submit');
  if ($current_form_state['action']) {
    $form['#action'] = $current_form_state['action'];
  }
  
  if (!empty($prenode->locked)) {
    $form['#prefix'] = theme('wallyedit_lock', $prenode);
  }

  return $form;
}

function wallyedit_form_node_form_validate(&$form, &$form_state) {
  $commands = array();

  list($action, $b) = explode('__', $form_state['clicked_button']['#name'], 2);
  $function_name = 'wydit_form_validate_action_'.$action;
  if (function_exists($function_name)) {
    $args = array($form_state);
    list($apply_commands, $commands) = call_user_func_array($function_name, $args);
    if (!empty($commands) && $apply_commands) {
      wydit_ctools_include();
      $commands[] = ctools_ajax_command_html('#wallyedit_messages', theme_status_messages());
      ctools_ajax_render($commands);
    }
  }

  //This is the important line !!!
  wydit_form_node_main_treatment_validate($form_state, $commands);

  return;
}

function wallyedit_form_node_form_submit(&$form, &$form_state) {
  wydit_ctools_include();
  
  // Some fields have a part of their process in the validation step
  if ($form_state['clicked_button']['#skip validation']) {
    wydit_form_node_main_treatment_forced_validate($form, $form_state);
    // We only want the process part, not the true validation, so we skip the messages
    drupal_get_messages();
  }

  if ($form_state['clicked_button']['#wydit_op_type'] == 'pre_treat') {
    list($action, $b) = explode('__', $form_state['clicked_button']['#name'], 2);
    $pre_treat_function_name = 'wydit_form_submit_action_'.$action;
    if (function_exists($pre_treat_function_name)) {
      $pre_treat_function_name($form_state);
      return;
    }
  } elseif ($form_state['clicked_button']['#wydit_op_type'] == 'action') {
    //This is the important line !!!
    wydit_form_node_main_treatment_submit($form_state);

    $form_state['parents'] = $form_state['clicked_button']['#next_parents'];
    list($action, $b) = explode('__', $form_state['clicked_button']['#name'], 2);
    $function_name = 'wydit_form_submit_action_'.$action;
    if (function_exists($function_name)) {
      $args = array(&$form_state);
      $commands = call_user_func_array($function_name, $args);
    }

    $commands[] = ctools_ajax_command_html('#wallyedit_messages', theme_status_messages());
    if ($form_state['type'] != 'page') {
      variable_set('menu_override_parent_selector', FALSE);
    }
    ctools_ajax_render($commands);
  }
  
  return;
}

/**
 ****************************
 * END OF NODE EDITION FORM *
 ****************************
 */

/**
 *************
 * SUB FORMS *
 *************
 */

function wydit_prepare_primary_package_form(&$form, &$form_state, $type, $parents, $global_form = array(), $global_path = NULL) {
  $fields = wyditadmin_get_existing_fields($type);
  $flat_fields = wyditadmin_get_fields_flat($form_state['profile_id'], $type);
  $content_fields = content_types($type);

  $groups_names = array();
  if (module_exists('field_group')) {
    $groups = fieldgroup_groups($type);
    $groups_names = array_keys($groups);
  }

  foreach(element_children($form) as $element) {
    if (in_array($element, $groups_names)) {
      $parents_new = $parents;
    } else {
      $parents_new = array_merge($parents, array($element));
    }
    $form[$element]['#parents'] = $parents_new;

    if(isset($flat_fields[$element]) && $flat_fields[$element]['display'] == 0) {
      if ($children = element_children($form[$element])) {
        foreach ($children as $child) {
          if ($form[$element][$child]['#type'] == 'text_textarea') {
            $form[$element][$child]['#wysiwyg'] = FALSE;
          }
          $form[$element][$child]['#required'] = FALSE;
        }
      } else {
        if ($form[$element]['#type'] == 'text_textarea') {
          $form[$element]['#wysiwyg'] = FALSE;
        }
        $form[$element]['#required'] = FALSE;
      }
    }
    
    if($form[$element]['#type'] == 'value' && $form[$element]['#value']) {
      continue;
    }

    if($form[$element]['#type'] == 'fieldset') {
      if (!isset($form[$element]['#title'])) {
        $form[$element]['#title'] = wydit_convert_type_to_title($form[$element]['type']['#value']);
      }

      $temp_b = $form_state;
      wydit_prepare_primary_package_form($form[$element], $form_state, $type, $parents, $global_form, $global_path.'/'.$element);

      if(count(element_children($form[$element])) == 0) {
        unset($form[$element]);
      } else {
        $has_another_type_than_value = FALSE;
        foreach(element_children($form[$element]) as $subfield_key => $subfield) {
          if($form[$element][$subfield]['#type'] != 'value'){
            $has_another_type_than_value = TRUE;
          }
        }
        if(!$has_another_type_than_value) {
          unset($form[$element]['#type']);
        }
      }
      continue;
    }

    if ($fields[$element]['widget'] == 'text_textarea') {
      if (isset($fields[$element]['text_processing']) && $fields[$element]['text_processing'] == 0) {
        if ($children = element_children($form[$element])) {
          foreach ($children as $child) {
            $form[$element][$child]['#wysiwyg'] = FALSE;
          }
        } else {
          $form[$element]['#wysiwyg'] = FALSE;
        }
      } else {
        foreach (element_children($form[$element]) as $index) {
          $filter_key = (count($form[$element][$index]['#columns']) == 2) ? $form[$element][$index]['#columns'][1] : 'format';
          $format = isset($form[$element][$index]['#value'][$filter_key]) ? $form[$element][$index]['#value'][$filter_key] : FILTER_FORMAT_DEFAULT;
          $filter_parents = $form[$element]['#parents'];
          $filter_parents[] = 0;
          $filter_parents[] = $filter_key;
        
          $form[$element][$filter_key] = filter_form($format, 1, $filter_parents);
        }
      }
    }

    if(isset($flat_fields[$element])) {
      switch ($flat_fields[$element]['display']) {
        case 1:
          if (!isset($form[$element]['#title'])) {
            $form[$element]['#title'] = $form['#title'];
          }
          if($fields[$element]['multiple'] == 1) {
            $form[$element][$fields[$element]['name'].'_add_more']['#ahah']['path'] = str_replace('//', '/', 'wydit/js_add_more/'.$type.'/'.$element.'/'.$global_path.'/'.$element);
          }
          break;

        case 2:
          $parents_new[] = '0';
          $form[$element]['#parents'] = array($element);
          $form_state[$type][$element][0]['profile_id'] = $form_state['profile_id'];
          wydit_prepare_inline_form_element($form[$element], $form_state[$type][$element][0], $element, $fields, $parents_new, $global_path.'/'.$element.'/0');
          $form[$element]['#theme'] = 'wallyedit_inline_form';
          break;

        case 3:
          wydit_prepare_selectbox_form_element($form[$element], $form_state[$type][$element], $element, $fields, $parents_new);
          break;

        default:
      }
    }
  }

  unset($form['buttons']);
  unset($form['#submit']);
  unset($form['#disabled']);
}

function wydit_prepare_inline_form_element(&$form_element, $form_state, $element, $fields, $parents, $global_path = NULL){
  $type_name = $form_state['type'];
  $current_profile = $form_state['profile_id'];
  $fields = wyditadmin_get_existing_fields($type_name);
  $flat_fields = wyditadmin_get_fields_flat($current_profile, $type_name);

  $node = $form_state['#node'];
  $form_state[$type_name]['#node'] = $node;
  $form_state['#node'] = $form_state[$type_name]['#node'];

  $a = drupal_retrieve_form($type_name.'_node_form', $form_state, $form_state[$type_name]['#node']);
  drupal_prepare_form($type_name.'_node_form', $a, $form_state);
  
  $form = array(
    '#title' => $form_element['#title'],
  	'#type' => 'fieldset',
    '#tree' => TRUE,
    '#parents' => $parents,
  );
  unset($a['#type']);
  unset($a['form_id']);
  unset($a['#validate']);
  unset($a['#parents']);

  foreach(element_children($a) as $element_name) {
    $a['#parents'] = $parents;
    $form[$element_name] = $a[$element_name];

    //WARNNING: FIELDS DOESN'T EXIST CAUSE THERE ARE FIELDS FROM THE MAIN CONTENT, SHOULD BRING FIELD INFO FROM NODEREFERENCE CONTENT TYPE
    //EDIT2: WTF ???
    if(isset($fields[$element_name]) && $flat_fields[$element_name]['display'] == 1) {
      $form[$element_name]['#parents'] = $parents;
    }
  }

  wydit_prepare_primary_package_form($form, $form_state, $type_name, $parents, NULL, $global_path);

  $form['profile_id'] = array(
    '#type' => 'value',
    '#value' => $current_profile,
  );

  $form_element = array($form);
}

function wydit_prepare_selectbox_form_element(&$form_element, &$form_state, $element, $fields, $parents) {
  $form = array(
  	'#type' => 'fieldset',
    '#theme' => 'wydit_selector_theme',
    '#title' => $form_element['#title'],
  );

  foreach($form_state as $key => $embed) {
    if(is_array($embed) && isset($embed['#node'])) {
      $embed_node = $embed['#node'];
      if (isset($embed_node->title) && !empty($embed_node->title)) {
        $form[$key]['title'] = array(
          '#type' => 'markup',
          '#value' => $embed_node->title,
        );

        $form[$key]['type'] = array(
          '#type' => 'markup',
          '#value' => $embed_node->type,
        );

        $form[$key]['weight'] = array(
          '#type' => 'weight',
          '#default_value' => $key,
          '#attributes' => array('class'=>'order_weight'),
          '#parents' => array_merge($parents, array('weight_'.$key)),
        );

        $form[$key]['edit'] = array(
          '#type' => 'submit',
          '#wydit_op_type' => 'action',
          '#value' => 'edit',
          '#name' => 'edit__'.$parents[count($parents)-1].'__'.$key,
          '#attributes' => array('class' => 'form-submit ctools-use-ajax'),
          '#next_parents' => array_merge($parents, array($key)),
          '#skip validation' => TRUE,
        );

        $form[$key]['delete'] = array(
          '#type' => 'submit',
          '#wydit_op_type' => 'action',
          '#value' => 'remove',
          '#name' => 'remove__'.$parents[count($parents)-1].'__'.$key,
          '#attributes' => array('class' => 'form-submit ctools-use-ajax'),
          '#next_parents' => array_slice($parents, 0, sizeof($parents) - 1),
          '#skip validation' => TRUE,
        );
      } else {
        unset($form_state[$key]);
      }
    }
  }

	$form['add_existing'] = array(
	  '#type' => 'submit',
	  '#wydit_op_type' => 'action',
	  '#value' => t('Add existing one'),
	  '#name' => 'add_existing__'.$parents[count($parents)-1],
	  '#attributes' => array('class' => 'form-submit ctools-use-ajax'),
	  '#next_parents' => $parents,
	  '#skip validation' => TRUE,
	);

	$old_keys = empty($form_state) ? array(-1) : array_keys($form_state);
  $new_key = max($old_keys) + 1;
  $form['add_new'] = array(
    '#type' => 'submit',
    '#wydit_op_type' => 'action',
    '#value' => t('Add new one'),
    '#name' => 'add_new__'.$parents[count($parents)-1],
    '#attributes' => array('class' => 'form-submit ctools-use-ajax'),
    '#next_parents' => array_merge($parents, array($new_key)),
    '#skip validation' => TRUE,
  );

  if ($element == 'field_embededobjects') {
    if (module_exists('jquery_update')) {
      $form['batch_import'] = array(
        '#type' => 'submit',
        '#wydit_op_type' => 'action',
        '#value' => t('Batch import'),
        '#name' => 'batch_import__'.$parents[count($parents)-1],
        '#attributes' => array('class' => 'form-submit ctools-use-ajax'),
        '#next_parents' => $parents,
        '#skip validation' => TRUE,
      );
    } else {
      drupal_set_message(t('Install jquery update module to use the batch upload feature.'), 'notice');
    }
  }
  
  $form_element = array_merge(array('#parents' => $form_element['#parents']), $form);
}

function wydit_form_preview_form(&$form_state, $cache_name = NULL) {
  ctools_include('ajax');
  if(empty($form_state['cache name'])) {
    $form_state['cache name'] = $cache_name;
  }
  
  drupal_add_js(array('cache_name' => $form_state['cache name']), 'setting');
  drupal_add_js(drupal_get_path('module', 'wallyedit').'/js/refresh_preview.js');

  $prenode = new Prenode($form_state['cache name']);
  $tmp_form_state = $prenode->getFormState();
  $type_name = $tmp_form_state['type'];
  if(!empty($prenode->form_state)) {
    $options = array('disabled' => t('Preview disabled'), 'no_dest' => 'Node');
    if (isset($tmp_form_state[$type_name]['#node']->field_destinations)) {
      foreach($tmp_form_state[$type_name]['#node']->field_destinations as $delta => $dest) {
        if($delta !== 'field_destinations_add_more' && $dest['tid'] && !empty($dest['tid']) && $dest['target'] && !empty($dest['target'])) {
          $dest_name = taxonomy_get_term($dest['tid'])->name;
          $option = $dest_name.' - Target : '.$dest['target'].' - Layout : '.$dest['layout'].' - Rank : ';
          if (strstr($dest['rank'], 'DNC')) {
            $option .= substr($dest['rank'], 3, 1).' (not changed)';
          } else {
            $option .= $dest['rank'];
          }
          $options += array($delta => $option);
        }
      }
    }
    $form['select_preview'] = array(
      '#type' => 'select',
      '#title' => t('Preview destination'),
      '#default_value' => 'disabled',
      '#options' => $options, 
      '#attributes' => array('prenode' => $tmp_form_state['cache name']),
    );
    $form['#suffix'] = theme('wallyedit_preview_id', array('cache name' => $tmp_form_state['cache name'], 'dest' => 'disabled'));
  } else {
    $form['#suffix'] = theme('wallyedit_nopreview');
  }
  
  $form['preview'] = array(
    '#type' => 'submit',
    '#value' => t('Reload destinations list'),
    '#attributes' => array('class' => 'ctools-use-ajax'),
  );
  
  return $form;
}

function wydit_form_preview_form_submit($form, &$form_state) {
  $form_state['cache name'] = $form['select_preview']['#attributes']['prenode'];
  $form_prev = drupal_rebuild_form('wydit_form_preview_form', $form_state);
  $form_prev = drupal_render_form('wydit_form_preview_form', $form_prev);
  
  $commands[] = ctools_ajax_command_html('#wallyedit_messages', theme_status_messages());
  $commands[] = ctools_ajax_command_html('#wallyedit_preview_container', $form_prev);
  ctools_ajax_render($commands);
}

/**
 ********************
 * END OF SUB FORMS *
 ********************
 */

/**
 ********************
 * TRANSITION FORMS *
 ********************
 */

function wydit_form_nodetypeselector_form(&$form_state) {
  wydit_ctools_include();

  $parents = wydit_get_parents_from_form_state($form_state);
  if (sizeof($parents) > 3) {
    $tmp_parents = array_slice($parents, 0, sizeof($parents) - 1);
    $tmp_parents[sizeof($tmp_parents) - 1] = 'type';
    $field_type = wydit_get_target_from_parents($form_state, $tmp_parents);
    $ref_parents = array_merge(array($field_type), array_slice($parents, sizeof($parents) - 2));
  } else {
    $ref_parents = $parents;
  }
  $field_infos = wyditadmin_get_existing_fields($ref_parents[0], $ref_parents[1]);
  $content_types = node_get_types();

  foreach($field_infos['referenceable_types'] as $type => $value) {
    if($value) {
      $options[$type] = $content_types[$type]->name;
    }
  }
  
  $form = array();

  $form['type'] = array(
    '#type' => 'radios',
    '#title' => t('Select the type of node to add'),
    '#options' => $options,
  );

  $form['cancel_global'] = array(
    '#type' => 'submit',
    '#wydit_op_type' => 'pre_treat',
    '#name' => 'cancel_global',
    '#value' => t('Cancel'),
    '#attributes' => array('class' => 'form-submit ctools-use-ajax'),
    '#next_parents' => array_slice($parents, 0, -2),
  );
  
  $form['next'] = array(
    '#type' => 'submit',
    '#wydit_op_type' => 'action',
    '#name' => 'next',
    '#value' => t('Next'),
    '#attributes' => array('class' => 'form-submit ctools-use-ajax'),
    '#next_parents' => $parents,
  );
  
  // If we got only one choice, directly apply it
  if (sizeof($options) == 1) {
    $next_form_state = $form_state;
    $next_form_state['clicked_button'] = $form['next'];
    $next_form_state['values']['type'] = array_shift(array_keys($options));
    wallyedit_form_node_form_submit($form, $next_form_state);
  }

  $form['profile_id'] = array('#value' => $current_profile);
  $form['#tree'] = TRUE;
  $form['#no_cache'] = TRUE;
  $form['#validate'] = array('wallyedit_form_node_form_validate');
  $form['#submit'] = array('wallyedit_form_node_form_submit');
  if ($form_state['action']) {
    $form['#action'] = $form_state['action'];
  }

  return $form;
}

function wydit_form_batchimport_form(&$form_state) {
  wydit_ctools_include();
  
  $parents = wydit_get_parents_from_form_state($form_state);
  
  $form = array();
  $form['#attributes'] = array('enctype' => 'multipart/form-data');
  
  $form['zipped_file'] = array(
    '#type' => 'file',
    '#title' => t('Choose a zip file to upload'),
    '#name' => 'zipped_file',
  );
  
  $form['batch_upload_zip'] = array(
    '#type' => 'submit',
    '#wydit_op_type' => 'pre_treat',
    '#value' => t('Upload'),
    '#name' => 'batch_upload_zip',
    '#attributes' => array('class' => 'form-submit ctools-use-ajax'),
    '#next_parents' => $parents,
  );

  $form['cancel_global'] = array(
    '#type' => 'submit',
    '#wydit_op_type' => 'pre_treat',
    '#name' => 'cancel_global',
    '#value' => t('Cancel'),
    '#attributes' => array('class' => 'form-submit ctools-use-ajax'),
    '#next_parents' => array_slice($parents, 0, -1),
  );
  
  $form['profile_id'] = array('#value' => $current_profile);
  $form['#tree'] = TRUE;
  $form['#no_cache'] = TRUE;
  $form['#validate'] = array('wallyedit_form_node_form_validate');
  $form['#submit'] = array('wallyedit_form_node_form_submit');
  if ($form_state['action']) {
    $form['#action'] = $form_state['action'];
  }
  
  return $form;
}

function wydit_form_batchimportselector_form ($form_state) {
  wydit_ctools_include();
  $parents = wydit_get_parents_from_form_state($form_state);
  $form = array();
  
  $post_file = $_FILES['zipped_file'];
  if (file_exists($post_file['tmp_name'])) {
    $filename = $post_file['name'];
    $tmp_path = $post_file['tmp_name'];
    $object_file = new stdClass();
    $object_file->name = $filename;
    $object_file->filename = $tmp_path;
    $object_file->basename = $tmp_path;
    $return = array();
    $zipped_objects = wydit_preprocess_batch_upload_zip($object_file, $return);
  
    if ($return['error']) {
      drupal_set_message($return['error_msg'], 'error');
    } else {
      $allowed_extensions = array(
        'photo' => array(),
      );
      $photo_field = content_fields('field_photofile', 'wally_photoobject');
      if ($photo_field) {
        $allowed_extensions['photo'] = explode(' ', strtolower($photo_field['widget']['file_extensions']));
      }
      
      foreach ($zipped_objects as $zip_id => $zipped_object) {
        $form_pic = array();
        
        switch ($zipped_object->type) {
          case 'photo':
            $file_ext = strtolower(pathinfo($zipped_object->filename, PATHINFO_EXTENSION));
            if (in_array($file_ext, $allowed_extensions['photo'])) {
              $form_pic['thumb'] = array(
                '#type' => 'markup',
                '#value' => '<img src="/'.$zipped_object->filename.'" width="100px" />',
                '#weight' => 1,
              );
              
              $form_pic['title'] = array(
                '#type' => 'textfield',
                '#title' => t('Title'),
                '#default_value' => $zipped_object->name,
                '#size' => 30,
                '#keep_size' => TRUE,
                '#required' => TRUE,
                '#weight' => 2,
              );
              
              $form_pic['caption'] = array(
                '#type' => 'textfield',
                '#title' => t('Summary'),
                '#default_value' => $zipped_object->name,
                '#size' => 60,
                '#keep_size' => TRUE,
                '#weight' => 3,
              );
            } else {
              drupal_set_message(t('The extension !extension is not allowed', array('!extension' => $file_ext)), 'error');
            }

            break;
        }
        
        if (!empty($form_pic)) {
          $form_pic['#type'] = 'fieldset';
          $form_pic['#title'] = $zipped_object->basename;
          $form_pic['#collapsible'] = FALSE;
          
          $form_pic['selected'] = array(
            '#type' => 'checkbox',
            '#title' => '',
            '#default_value' => 1,
            '#weight' => 0,
          );
          
          $form_pic['path'] = array(
            '#type' => 'hidden',
            '#value' => $zipped_object->filename,
          );
          
          $form_pic['basename'] = array(
            '#type' => 'hidden',
            '#value' => $zipped_object->basename,
          );
          
          $form_pic['type'] = array(
            '#type' => 'hidden',
            '#value' => $zipped_object->type,
          );
          
          $form['picture_'.$zip_id] = $form_pic;
        }
      }
    }
    
    $temp_subdir = strstr($filename, '.zip') ? str_replace('.zip', '', $filename) : $filename;
    $temp_subdir = file_directory_path().'/tmp_batch_upload/'.$temp_subdir;
    $form['zip_tmp_dir'] = array(
      '#type' => 'hidden',
      '#value' => $temp_subdir,
    );
  }
  
  $form['save_batch_upload'] = array(
    '#type' => 'submit',
    '#wydit_op_type' => 'pre_treat',
    '#value' => t('Save'),
    '#name' => 'save_batch_upload',
    '#attributes' => array('class' => 'form-submit ctools-use-ajax'),
    '#next_parents' => array_slice($parents, 0, -1),
  );
  
  $form['cancel_global'] = array(
    '#type' => 'submit',
    '#wydit_op_type' => 'pre_treat',
    '#name' => 'cancel_global',
    '#value' => t('Cancel'),
    '#attributes' => array('class' => 'form-submit ctools-use-ajax'),
    '#next_parents' => array_slice($parents, 0, -1),
  );
  
  $form['profile_id'] = array('#value' => $current_profile);
  $form['#tree'] = TRUE;
  $form['#no_cache'] = TRUE;
  $form['#validate'] = array('wallyedit_form_node_form_validate');
  $form['#submit'] = array('wallyedit_form_node_form_submit');
  if ($form_state['action']) {
    $form['#action'] = $form_state['action'];
  }
  
  return $form;
}

/**
 * Confirm form to delete a node
 */
function wydit_form_delete_node_and_prenode_form (&$form_state) {
  $redirect = isset($_GET['destination']) ? $_GET['destination'] : '';
  $question = t('Are you sure you want to delete node @nid?', array('@nid' => $form_state['cache name']));
  
  $form = array();
  
  $form['question'] = array(
    '#type' => 'markup',
    '#value' => $question.'</br>',
  );
  
  $form['confirm_delete'] = array(
    '#type' => 'submit',
    '#name' => 'confirm_delete',
    '#value' => t('Confirm'),
    '#attributes' => array('class' => 'form-submit ctools-use-ajax'),
  );
  
  $form['cancel'] = array(
    '#type' => 'submit',
    '#name' => 'cancel',
    '#value' => t('Cancel'),
    '#attributes' => array('class' => 'form-submit ctools-use-ajax'),
  );
  
  $form['#action'] = '/node/'.$form_state['cache name'].'/deleteglobal/ajax/'.$form_state['profile_id'];

  return $form;
}

/**
 ***************************
 * END OF TRANSITION FORMS *
 ***************************
 */

/**
 ***************************************
 * FORM ACTIONS FOR VALIDATE FUNCTIONS *
 ***************************************
 */

function wydit_form_node_main_treatment_validate($form_state, $commands = array()) {
  if(form_get_errors()) {
    wydit_ctools_include();
    $commands[] = ctools_ajax_command_html('#wallyedit_messages', theme('status_messages'));
    if ($form_state['type'] != 'page') {
      variable_set('menu_override_parent_selector', FALSE);
    }
    ctools_ajax_render($commands);
  }
}

function wydit_form_validate_action_cancel_global($form_state) {
  $commands = array();

  drupal_get_messages();
  form_set_error(NULL, '', TRUE);

  return array(FALSE, $commands);
}

function wydit_form_validate_action_next($form_state) {
  $commands = array();

  $values = $form_state['values'];
  if (empty($values['type'])) {
    form_set_error('type', t('You have to choose a type!'));
  }

  return array(FALSE, $commands);
}

function wydit_form_validate_action_batch_upload_zip($form_state) {
  $commands = array();

  if (!isset($_FILES['zipped_file']) || empty($_FILES['zipped_file']) || $_FILES['zipped_file']['error'] > 0) {
    form_set_error('zipped_file', t('No files to import'));
  }

  $authorized_types = array(
    'application/zip',
    'application/x-zip-compressed',
    'multipart/x-zip',
    'application/x-compressed',
    'application/octet-stream',
  );
  if (!in_array($_FILES['zipped_file']['type'], $authorized_types) && pathinfo($_FILES['zipped_file']['name'], PATHINFO_EXTENSION) != 'zip') {
    form_set_error('zipped_file', t('The uploaded file is not a zip file.'));
  }

  return array(FALSE, $commands);
}

function wydit_form_node_main_treatment_forced_validate($elements, &$form_state) {
  // Recurse through all children.
  foreach (element_children($elements) as $key) {
    if (isset($elements[$key]) && $elements[$key]) {
      wydit_form_node_main_treatment_forced_validate($elements[$key], $form_state);
    }
  }
  if ($elements['#force_validate']) {
    _form_validate($elements, $form_state);
  }
}

/**
 ********************************
 * END OF FORM VALIDATE ACTIONS *
 ********************************
 */

/**
 *************************************
 * FORM ACTIONS FOR SUBMIT FUNCTIONS *
 *************************************
 */

function wydit_form_submit_action_delete_global(&$form_state) {
  if ($form_state['type'] != 'page') {
    variable_set('menu_override_parent_selector', TRUE);
  }
  $confirm_form = ctools_build_form('wydit_form_delete_node_and_prenode_form', $form_state);
  $commands[] = ctools_ajax_command_html('#wallyedit_container', $confirm_form);
  $commands[] = ctools_ajax_command_html('#wallyedit_preview_container', '');
  $commands[] = ctools_ajax_command_html('#wallyedit_messages', theme('status_messages'));
  if ($form_state['type'] != 'page') {
    variable_set('menu_override_parent_selector', FALSE);
  }
  ctools_ajax_render($commands);
}

function wydit_form_submit_action_cancel_global($form_state) {
  $commands = array();

  $parents = $form_state['parents'];
  if (sizeof($parents) == 3 || sizeof($parents) == 5) {
    module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
    $node_parents = array_merge($parents, array('#node'));
    if (!wallytoolbox_check_is_node(wydit_get_target_from_parents($form_state, $node_parents))) {
      $form_state = wydit_remove_element($form_state, $parents);
    }
  }

  $form_state['parents'] = $form_state['clicked_button']['#next_parents'];
  unset($form_state['values'][$form_state['type']]);
  if (wydit_clean_and_save_form_state($form_state)) {
    $cache_name = $form_state['cache name'];
    $current_profile = $form_state['profile_id'];
    $new_form_state = wydit_get_canonical_form_state($form_state, TRUE);
    $new_form_state['action'] = '/node/'.$cache_name.'/edit2/ajax/'.$current_profile;
  
    if ($form_state['type'] != 'page') {
      variable_set('menu_override_parent_selector', TRUE);
    }
    $edit_form = ctools_build_form('wallyedit_form_node_edition_form', $new_form_state, $current_profile);
    $commands[] = ctools_ajax_command_html('#wallyedit_container', $edit_form);
    if (sizeof($form_state['parents']) == 1) {
      $form_prev = drupal_get_form('wydit_form_preview_form', $cache_name);
    } else {
      $form_prev = '';
    }
    
    $commands[] = ctools_ajax_command_html('#page-title h2', wydit_get_fil_ariane($form_state));
    $commands[] = ctools_ajax_command_html('#wallyedit_preview_container', $form_prev);
    $commands[] = ctools_ajax_command_html('#wallyedit_messages', theme('status_messages'));
    if ($form_state['type'] != 'page') {
      variable_set('menu_override_parent_selector', FALSE);
    }
  } else {
    drupal_set_message(t('Unable to save prenode, please try again.'), 'error');
    wydit_form_reset_form_to_first_level($commands, $form_state);
  }

  ctools_ajax_render($commands);
}

function wydit_form_submit_action_reset_global(&$form_state) {
  // Store prenodes in a specific table so we can inspect them later
  db_query("DELETE FROM {wallyedit_stored_prenodes} WHERE pid = '%s'", $form_state['cache name']);
  db_query("INSERT INTO {wallyedit_stored_prenodes} (pid, created, data) VALUES ('%s', %d, %b)", $form_state['cache name'], time(), serialize($form_state));
  // Clean the rest
  prenode_delete($form_state);
  $commands[] = ctools_ajax_command_redirect('node/'.$form_state[$form_state['type']]['#node']->nid);
  if ($form_state['type'] != 'page') {
    variable_set('menu_override_parent_selector', FALSE);
  }
  ctools_ajax_render($commands);
}

// important: add fields infos to form_state
function wydit_form_node_main_treatment_submit(&$form_state) {
  wydit_ctools_include();
  $parents = wydit_get_parents_from_form_state($form_state);
  
  //*********
  //Prepare form_state for input element
  //*********
  if (!($node = wydit_get_target_from_parents($form_state, array_merge($parents, array('#node'))))) {
    $node = new stdClass();
    $type_name = $form_state['values']['type'];
    $node->type = $type_name;
  } else {
    $type_name = $node->type;
    $fields = wyditadmin_get_existing_fields($type_name);
    $values = wydit_get_target_from_parents($form_state['values'], $parents);

    foreach($fields as $field) {
      if($field['multiple'] == 1) {
        $field_name = $field['name'];
        
        // Clean empty multiple values (cfr hook_content_is_empty)
        $function_is_empty = $field['module'].'_content_is_empty';
        if(function_exists($function_is_empty) && is_array($values[$field_name])) {
          foreach($values[$field_name] as $key => $value) {
            if(is_numeric($key)) {
              if($function_is_empty($values[$field_name][$key], $field)) {
                $form_state['values'] = wydit_remove_element($form_state['values'], array_merge($parents, array($field_name, $key)));
              }
            }
          }
        }
        
        // ReOrder by _weight
        if (isset($values[$field_name]) && !empty($values[$field_name])) {
          $tmp_items = $values[$field_name];
          if (isset($tmp_items[$field_name.'_add_more'])) {
            $add_more = $tmp_items[$field_name.'_add_more'];
            unset($tmp_items[$field_name.'_add_more']);
          }
          $sorted_items = _content_sort_items($field, $tmp_items);
          if ($add_more) {
            $sorted_items = array_merge($sorted_items, array($field_name.'_add_more' => $add_more));
          }
          wydit_add_elem_at_target($form_state['values'], $sorted_items, array_merge($parents, array($field_name)));
        }
      }
    }
  }
  
  $a = array();
  if (!($b = wydit_get_target_from_parents($form_state['values'], $parents))) {
    wydit_populate_form_state($a[$type_name], $node, $form_state['profile_id']);
  } else {
    $a[$type_name]['#node'] = (object)(wydit_merge_arrays((array)$node, (array)node_submit($b), $node->type));
    $sub_form_state = array();
    $sub_form_state[$type_name] = wydit_get_target_from_parents($form_state, $parents);
    $sub_form_state['values'] = wydit_get_target_from_parents($form_state['values'], $parents);

    $flat_fields = wyditadmin_get_fields_flat($form_state['profile_id'], $type_name);
    foreach($flat_fields as $field) {
      $field_name = $field['name'];
      if($field['display'] == 2 || $field['display'] == 3) {
        if(isset($sub_form_state[$type_name][$field_name])) {
          foreach($sub_form_state[$type_name][$field_name] as $key => $value) {
            // Avoid the weight_.$key element
            if(is_numeric($key)) {
              // Determine index, if no weight -inline element- use $key value
              if(isset($sub_form_state['values'][$field_name]['weight_'.$key])) {
                $index = $sub_form_state['values'][$field_name]['weight_'.$key];
              } else {
                $index = $key;
              }

              $a[$type_name][$field_name][$index]['#node'] = (object)(wydit_merge_arrays((array)$sub_form_state[$type_name][$field_name][$key]['#node'], (array)node_submit($sub_form_state['values'][$field_name][$key]), $sub_form_state[$type_name][$field_name][$key]['type']));
              $subtype = $a[$type_name][$field_name][$index]['#node']->type;
              $a[$type_name][$field_name][$index]['type'] = $subtype;

              $subfields = wyditadmin_get_fields_flat($form_state['profile_id'], $a[$type_name][$field_name][$key]['#node']->type);

              foreach($subfields as $subfield) {
                $subfield_name = $subfield['name'];
                if(($subfield['display'] == 2 || $subfield['display'] = 3) && isset($sub_form_state[$type_name][$field_name][$key][$subfield_name])) {
                  foreach($sub_form_state[$type_name][$field_name][$key][$subfield_name] as $subkey => $subvalue) {
                    $a[$type_name][$field_name][$key][$subfield_name][$subkey] = $sub_form_state[$type_name][$field_name][$key][$subfield_name][$subkey];
                  }
                }
              }
            }
            //ReOrder by index
            ksort($a[$type_name][$field_name]);
          }
        }
      }
    }
  }
  
  $a[$type_name]['type'] = $type_name;
  wydit_add_elem_at_target($form_state, $a[$type_name], $parents);
}

function wydit_form_submit_action_publish_global($form_state) {
  $commands = array();

  if (!$form_state['locked'] && wydit_publish_prenode($form_state[$form_state['type']], $form_state['profile_id'], $form_state['values'])) {
    drupal_set_message(t('Node has been published'));
    prenode_delete($form_state);
    drupal_set_message(t('PreNode has been cleared'));
    $commands[] = ctools_ajax_command_redirect('node/'.$form_state[$form_state['type']]['#node']->nid);
  } else {
    drupal_set_message(t('Node not published'), 'error');
    $commands[] = ctools_ajax_command_redirect('node/'.$form_state[$form_state['type']]['#node']->nid.'/edit');
  }

  return $commands;
}

function wydit_form_submit_action_edit($form_state) {
  $commands = array();

  $form_state['parents'] = $form_state['clicked_button']['#next_parents'];
  if (wydit_clean_and_save_form_state($form_state)) {
    $current_profile = $form_state['profile_id'];
    $new_form_state = wydit_get_canonical_form_state($form_state, TRUE);
  
    if ($form_state['type'] != 'page') {
      variable_set('menu_override_parent_selector', TRUE);
    }
    $edit_form = ctools_build_form('wallyedit_form_node_edition_form', $new_form_state, $current_profile);
    if ($form_state['type'] != 'page') {
      variable_set('menu_override_parent_selector', FALSE);
    }
    $commands[] = ctools_ajax_command_html('#page-title h2', wydit_get_fil_ariane($form_state));
    $commands[] = ctools_ajax_command_html('#wallyedit_container', $edit_form);
    $commands[] = ctools_ajax_command_html('#wallyedit_preview_container', '');
  } else {
    drupal_set_message(t('Unable to save prenode, please try again.'), 'error');
    wydit_form_reset_form_to_first_level($commands, $form_state);
  }

  return $commands;
}

function wydit_form_submit_action_add_new($form_state) {
  $commands = array();
  
  $form_state['parents'] = $form_state['clicked_button']['#next_parents'];
  if (wydit_clean_and_save_form_state($form_state)) {
    $cache_name = $form_state['cache name'];
    $current_profile = $form_state['profile_id'];
    $new_form_state = wydit_get_canonical_form_state($form_state, TRUE);
    $new_form_state['action'] = '/node/'.$cache_name.'/wydit_typeselector/ajax/'.$current_profile;
  
    if ($form_state['type'] != 'page') {
      variable_set('menu_override_parent_selector', TRUE);
    }
    $form_type_selector = ctools_build_form('wydit_form_nodetypeselector_form', $new_form_state);
    if ($form_state['type'] != 'page') {
      variable_set('menu_override_parent_selector', FALSE);
    }
    $commands[] = ctools_ajax_command_html('#wallyedit_container', $form_type_selector);
    $commands[] = ctools_ajax_command_html('#wallyedit_preview_container', '');
  } else {
    drupal_set_message(t('Unable to save prenode, please try again.'), 'error');
    wydit_form_reset_form_to_first_level($commands, $form_state);
  }

  return $commands;
}

function wydit_form_submit_action_add_existing($form_state) {
  $commands = array();

  $form_state['parents'] = $parents = $form_state['clicked_button']['#next_parents'];
  $form_state_copy = $form_state;
  if (wydit_clean_and_save_form_state($form_state_copy)) {
    $current_profile = $form_state['profile_id'];
    $type_name = $form_state['type'];
    $cache_name = $form_state['cache name'];
    if (sizeof($parents) > 3) {
      $tmp_parents = array_slice($parents, 0, -1);
      $tmp_parents[sizeof($tmp_parents)] = 'type';
      $field_type = wydit_get_target_from_parents($form_state, $tmp_parents);
      $ref_parents = array_merge(array($field_type), array_slice($parents, sizeof($parents) - 1));
    } else {
      $ref_parents = $parents;
    }
  
    $view = views_get_view('prenode_selectbox_vbo');
    $view->set_display('default');
    wydit_process_node_selector_view($view, $ref_parents, $cache_name, $parents);
  
    $output = $view->preview();
  
    $commands[] = ctools_ajax_command_html('#wallyedit_container', $output);
    $commands[] = ctools_ajax_command_html('#wallyedit_preview_container', '');
  } else {
    drupal_set_message(t('Unable to save prenode, please try again.'), 'error');
    wydit_form_reset_form_to_first_level($commands, $form_state);
  }

  return $commands;
}

function wydit_form_submit_action_batch_import($form_state) {
  $commands = array();
  
  $form_state['parents'] = $parents = $form_state['clicked_button']['#next_parents'];
  $form_state_copy = $form_state;
  if (wydit_clean_and_save_form_state($form_state_copy)) {
    $cache_name = $form_state['cache name'];
    $current_profile = $form_state['profile_id'];
    $new_form_state = wydit_get_canonical_form_state($form_state, TRUE);
    $new_form_state['action'] = '/node/'.$cache_name.'/wydit_batchimport/ajax/'.$current_profile;
    
    if ($form_state['type'] != 'page') {
      variable_set('menu_override_parent_selector', TRUE);
    }
    $form_batch_import = ctools_build_form('wydit_form_batchimport_form', $new_form_state);
    if ($form_state['type'] != 'page') {
      variable_set('menu_override_parent_selector', FALSE);
    }
    $commands[] = ctools_ajax_command_html('#wallyedit_container', $form_batch_import);
    $commands[] = ctools_ajax_command_html('#wallyedit_preview_container', '');
  } else {
    drupal_set_message(t('Unable to save prenode, please try again.'), 'error');
    wydit_form_reset_form_to_first_level($commands, $form_state);
  }

  return $commands;
}

function wydit_form_submit_action_next($form_state) {
  $commands = array();

  $cache_name = $form_state['cache name'];
  $type_name = $form_state['type'];
  $current_profile = $form_state['profile_id'];
  $form_state['parents'] = $form_state['clicked_button']['#next_parents'];
  $form_state_copy = $form_state;
  if (wydit_clean_and_save_form_state($form_state_copy)) {
    $new_form_state = wydit_get_canonical_form_state($form_state, TRUE);
    if (isset($form_state[$type_name]['#node']->nid) && $form_state[$type_name]['#node']->nid == $cache_name) {
      $new_form_state['action'] = '/node/'.$cache_name.'/edit2/ajax/'.$current_profile;
    } else {
      $new_form_state['action'] = '/node/add2/'.$type_name.'/ajax/'.$current_profile.'/'.$cache_name;
    }
    if ($form_state['type'] != 'page') {
      variable_set('menu_override_parent_selector', TRUE);
    }
    $new_form = ctools_build_form('wallyedit_form_node_edition_form', $new_form_state);
    if ($form_state['type'] != 'page') {
      variable_set('menu_override_parent_selector', FALSE);
    }
    
    $commands[] = ctools_ajax_command_html('#page-title h2', wydit_get_fil_ariane($form_state));
    $commands[] = ctools_ajax_command_html('#wallyedit_container', $new_form);
    $commands[] = ctools_ajax_command_html('#wallyedit_preview_container', '');
  } else {
    drupal_set_message(t('Unable to save prenode, please try again.'), 'error');
    wydit_form_reset_form_to_first_level($commands, $form_state);
  }

  return $commands;
}

function wydit_form_submit_action_batch_upload_zip($form_state) {
  $commands = array();
  
  $form_state['parents'] = $parents = $form_state['clicked_button']['#next_parents'];
  $form_state_copy = $form_state;
  if (wydit_clean_and_save_form_state($form_state_copy)) {
    $cache_name = $form_state['cache name'];
    $current_profile = $form_state['profile_id'];
    $new_form_state = wydit_get_canonical_form_state($form_state, TRUE);
    $new_form_state['action'] = '/node/'.$cache_name.'/wydit_batchimportselector/ajax/'.$current_profile;
    
    if ($form_state['type'] != 'page') {
      variable_set('menu_override_parent_selector', TRUE);
    }
    $form_batch_import_selector = ctools_build_form('wydit_form_batchimportselector_form', $new_form_state);
    if ($form_state['type'] != 'page') {
      variable_set('menu_override_parent_selector', FALSE);
    }
    $commands[] = ctools_ajax_command_html('#wallyedit_container', $form_batch_import_selector);
    $commands[] = ctools_ajax_command_html('#wallyedit_preview_container', '');
    $commands[] = ctools_ajax_command_html('#wallyedit_messages', theme('status_messages'));
  } else {
    drupal_set_message(t('Unable to save prenode, please try again.'), 'error');
    wydit_form_reset_form_to_first_level($commands, $form_state);
  }

  ctools_ajax_render($commands);
}

function wydit_form_submit_action_save_batch_upload($form_state) {
  $commands = array();
  
  $parents = $form_state['parents'];
  $values = &$form_state['values'];
  $input = $form_state['input'];
  $i=0;
  foreach ($input as $in_name => $in_val) {
    if (substr($in_name, 0, 8) == 'picture_' && $in_val['selected']) {
      $new_object = array();
      
      switch ($in_val['type']) {
        case 'photo':
          $new_object['type'] = 'wally_photoobject';
          $new_object['title'] = $in_val['title'];
          $new_object['field_externalreference'] = array(
            array('value' => 'WallyEdit_batch_'.time().'_'.$in_val['basename']),
          );
          $new_object['field_summary'] = array(
            array(
              'value' => $in_val['caption'],
              'format' => variable_get('filter_default_format', 1),
            ),
          );
          if ($photofile = wydit_create_file('wally_photoobject', 'field_photofile', $in_val['path'], $in_val['basename'])) {
            $new_object['field_photofile'][] = (array)$photofile;
            $new_object['field_photofile'][0]['data'] = array(
              'title' => $in_val['title'],
              'alt' => $in_val['caption'],
            );
          }
          
          break;
      }
      
      if (!empty($new_object)) {
        wydit_add_elem_at_target($values, $new_object, $parents, TRUE);
        $object_new_object = array('#node' => (object)$new_object);
        wydit_add_elem_at_target($form_state, $object_new_object, $parents, TRUE);
      }
    }
  }

  // Remove tmp files
  module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
  rrmdir($input['zip_tmp_dir']);

  $form_state['parents'] = $parents = $form_state['clicked_button']['#next_parents'];
  $form_state_copy = $form_state;
  if (wydit_clean_and_save_form_state($form_state_copy)) {
    $cache_name = $form_state['cache name'];
    $current_profile = $form_state['profile_id'];
    $type_name = $form_state['type'];
    $new_form_state = wydit_get_canonical_form_state($form_state, TRUE);
  
    if (sizeof($new_form_state['parents']) == 1) {
      $commands[] = ctools_ajax_command_redirect('node/'.$cache_name.'/edit2/0/'.$current_profile);
    } else {
      if (isset($form_state[$type_name]['#node']->nid) && $form_state[$type_name]['#node']->nid == $cache_name) {
        $new_form_state['action'] = '/node/'.$cache_name.'/edit2/ajax/'.$current_profile;
      } else {
        $new_form_state['action'] = '/node/add2/'.$type_name.'/ajax/'.$current_profile.'/'.$cache_name;
      }
      $form = $form_prev = '';
      
      if ($form_state['type'] != 'page') {
        variable_set('menu_override_parent_selector', TRUE);
      }
      $form = ctools_build_form('wallyedit_form_node_edition_form', $new_form_state, $current_profile);
      if ($form_state['type'] != 'page') {
        variable_set('menu_override_parent_selector', FALSE);
      }
      
      $commands[] = ctools_ajax_command_html('#page-title h2', wydit_get_fil_ariane($form_state));
      $commands[] = ctools_ajax_command_html('#wallyedit_container', $form);
      $commands[] = ctools_ajax_command_html('#wallyedit_messages', theme_status_messages());
      $commands[] = ctools_ajax_command_html('#wallyedit_preview_container', $form_prev);
    }
  } else {
    drupal_set_message(t('Unable to save prenode, please try again.'), 'error');
    wydit_form_reset_form_to_first_level($commands, $form_state);
  }
  
  ctools_ajax_render($commands);
}

function wydit_form_submit_action_save_global($form_state) {
  $commands = array();

  $form_state['parents'] = $form_state['clicked_button']['#next_parents'];
  $form_state_copy = $form_state;
    
  if (wydit_clean_and_save_form_state($form_state_copy)) {
    $cache_name = $form_state['cache name'];
    $current_profile = $form_state['profile_id'];
    $new_form_state = wydit_get_canonical_form_state($form_state, TRUE);
  
    $form = $form_prev = '';
  
    if (sizeof($new_form_state['parents']) == 1) {
      $commands[] = ctools_ajax_command_redirect('node/'.$cache_name.'/edit2/0/'.$current_profile);
    } else {
      if ($form_state['type'] != 'page') {
        variable_set('menu_override_parent_selector', TRUE);
      }
      $form = ctools_build_form('wallyedit_form_node_edition_form', $new_form_state, $current_profile);
      if ($form_state['type'] != 'page') {
        variable_set('menu_override_parent_selector', FALSE);
      }
      
      $commands[] = ctools_ajax_command_html('#page-title h2', wydit_get_fil_ariane($form_state));
      $commands[] = ctools_ajax_command_html('#wallyedit_container', $form);
      $commands[] = ctools_ajax_command_html('#wallyedit_preview_container', $form_prev);
    }
  } else {
    drupal_set_message(t('Unable to save prenode, please try again.'), 'error');
    wydit_form_reset_form_to_first_level($commands, $form_state);
  }

  return $commands;
}

function wydit_form_submit_action_remove($form_state) {
  $commands = array();

  $form_state['parents'] = $form_state['clicked_button']['#next_parents'];
  $current_profile = $form_state['profile_id'];
  $split_name = explode('__', $form_state['clicked_button']['#name']);
  $form_state_copy = $form_state = wydit_remove_element($form_state, array_merge($form_state['parents'], array_splice($split_name, 1, 2)));
  if (wydit_clean_and_save_form_state($form_state_copy)) {
    $new_form_state = wydit_get_canonical_form_state($form_state, TRUE);
    if ($form_state['type'] != 'page') {
      variable_set('menu_override_parent_selector', TRUE);
    }
    $new_form = ctools_build_form('wallyedit_form_node_edition_form', $new_form_state, $current_profile);
    if ($form_state['type'] != 'page') {
      variable_set('menu_override_parent_selector', FALSE);
    }
    if (sizeof($form_state['parents']) == 1) {
      $form_prev = drupal_get_form('wydit_form_preview_form', $form_state['cache name']);
    } else {
      $form_prev = '';
    }
    
    $commands[] = ctools_ajax_command_html('#page-title h2', wydit_get_fil_ariane($form_state));
    $commands[] = ctools_ajax_command_html('#wallyedit_container', $new_form);
    $commands[] = ctools_ajax_command_html('#wallyedit_preview_container', $form_prev);
  } else {
    drupal_set_message(t('Unable to save prenode, please try again.'), 'error');
    wydit_form_reset_form_to_first_level($commands, $form_state);
  }

  return $commands;
}

/**
 ******************************
 * END OF FORM SUBMIT ACTIONS *
 ******************************
 */

/**
 *************************************
 * FORM AND FORM_STATE MANIPULATIONS *
 *************************************
 */

/**
 * Return the target associated with the hierarchical structure of the array based on the path formed by parents.
 * 
 * @param: $array
 *   - the array
 * @param: $parents
 *   - the parents is an array representing the path in the array
 */
function wydit_get_target_from_parents(&$array, $parents = NULL) {
  if($parents == NULL) {
    $parents = $array['parents'];
  }

  $return = NULL;
  $temp = $array;
  foreach($parents as $index => $cursor_name) {
    if(isset($temp[$cursor_name])) {
      $temp = &$temp[$cursor_name];
    } else {
      return $return;
    }
  }

  if ($temp) {
    $return = $temp;
  }
  return $return;
}

/**
 *  Add "$element" in the structure of the $array based on the path of target 
 * 
 * @param: $array
 *   - the targeted array.
 * @param: $element
 *   - the element to add to the array.
 * @param: $target
 *   - the place where add element in the array structure.
 * @param: $append
 *   - a boolean to indicate if the element should replace or added next the element specified by target
 * 
 * @return nothing, the form_state is passed by reference.
 */
function wydit_add_elem_at_target(&$array, $element, $target, $append = FALSE){
  if (count($target) > 0) {
    $key = array_shift($target);
    if (!isset($array[$key])) {
      $array[$key] = array();
    }
    wydit_add_elem_at_target($array[$key], $element, $target, $append);
  } else {
    if($append === TRUE){
      $array[] = $element;
    } else {
      $array = $element;
    }
  }
}

/**
 * Remove an element from an array. The element to remove is found following the given path
 * Thx to JDE
 */
function wydit_remove_element($array, $path, $level = 0) {
  foreach($array as $key => $value) {
    if($key == $path[$level]) { // On atteint l'elem de $a correspondant au $path dans $b pour ce niveau de traitement
      if($level != (count($path) - 1)) { // On est pas encore au dernier niveau de traitement
        if(is_array($value)) { // Logique car il y a encore des niveaux à traiter dans $b;
          $tmp_val = wydit_remove_element($value, $path, $level+1);
          $array[$key] = $tmp_val ? $tmp_val : array();
        }
      } else {
        unset($array[$path[$level]]);
      }
    }
  }
  return $array;
}

function wydit_merge_arrays() {
  if (func_num_args() < 2) {
    trigger_error(__FUNCTION__ .' needs two or more array arguments', E_USER_WARNING);
    return;
  }
  $arrays = func_get_args();
  $type = array_pop($arrays);
  if (is_array($type)) {
    array_push($arrays, $type);
    $type = FALSE;
  }
  $merged = array();
  while ($arrays) {
    $array = array_shift($arrays);
    if (!is_array($array)) {
      trigger_error(__FUNCTION__ .' encountered a non array argument', E_USER_WARNING);
      return;
    }
    if (!$array) {
      continue;
    }
    foreach ($array as $key => $value) {
      if (is_string($key) || is_numeric($key)) {
        $multiple = FALSE;
        if ($type) {
          module_load_include('inc', 'wallyedit', 'includes/config_api');
          if ($field_infos = wyditadmin_get_existing_fields($type, $key)) {
            $multiple = $field_infos['multiple'];
          }
        }
        if (is_array($value) && array_key_exists($key, $merged) && is_array($merged[$key]) && !$multiple) {
          $merged[$key] = call_user_func(__FUNCTION__, $merged[$key], $value, $type);
        } else {
          $merged[$key] = $value;
        }
      } else {
        $merged[] = $value;
      }
    }
  }
  return $merged;
}

function wydit_clean_and_save_form_state(&$form_state) {
  $form_state = wydit_clean_form_state($form_state);
  return wydit_save_form_state($form_state);
}

/**
 * Here we sync the internal values of the main #node object
 */
function wydit_clean_form_state($form_state){
  $a = wydit_get_canonical_form_state($form_state);
  $type_name = $form_state['type'];
  $current_profile = $form_state['profile_id'];

  $fields = wyditadmin_get_fields_flat($current_profile, $type_name);

  foreach($fields as $field) {
    $field_name = $field['name'];
    if($field['display'] == 2 || $field['display'] == 3) {
      $a[$type_name][$field_name] = array();
      if(isset($form_state[$type_name][$field_name])) {
        foreach($form_state[$type_name][$field_name] as $key => $value) {
          if(is_numeric($key)) {
            if(isset($form_state[$type_name][$field_name][$key]['#node']) && isset($form_state['values'][$type_name][$field_name][$key])){
              $a[$type_name][$field_name][$key]['#node'] = (object)(wydit_merge_arrays((array)$form_state[$type_name][$field_name][$key]['#node'], (array)node_submit($form_state['values'][$type_name][$field_name][$key]), $form_state[$type_name][$field_name][$key]['type']));
              $a[$type_name][$field_name][$key]['type'] = $a[$type_name][$field_name][$key]['#node']->type;
            } else {
              $a[$type_name][$field_name][$key]['#node'] = $form_state[$type_name][$field_name][$key]['#node'];
              $a[$type_name][$field_name][$key]['type'] = $form_state[$type_name][$field_name][$key]['#node']->type;
            }
            $subfields = wyditadmin_get_fields_flat($current_profile, $a[$type_name][$field_name][$key]['#node']->type);
            
            foreach($subfields as $subfield) {
              $subfield_name = $subfield['name'];
              if(($subfield['display'] == 2 || $subfield['display'] == 3) && isset($form_state[$type_name][$field_name][$key][$subfield_name])) {
                foreach($form_state[$type_name][$field_name][$key][$subfield_name] as $subkey => $subvalue) {
                  $a[$type_name][$field_name][$key][$subfield_name][$subkey] = $form_state[$type_name][$field_name][$key][$subfield_name][$subkey];
                }
              }
            }
          }
        }
      }
    }
  }

  return $a;
}

function wydit_save_form_state($clean_form_state) {
  global $user;
  $clean_form_state['current_tab'] = wally_variable_get('wallyedit_current_tab_'.$user->uid, NULL);
  return prenode_save($clean_form_state);
}

function wydit_form_reset_form_to_first_level($form_state, &$commands) {
  $type_name = $form_state['type'];

  if (!isset($form_state['cache name']) && isset($form_state[$type_name]['#node']->nid)) {
    $redirect = '/node/'.$form_state[$type_name]['#node']->nid.'/edit';
  } elseif (isset($form_state[$type_name]['#node']->nid) && $form_state[$type_name]['#node']->nid == $form_state['cache name']) {
    $redirect = '/node/'.$form_state['cache name'].'/edit';
  } else {
    $redirect = '/node/add/'.$type_name;
  }

  $commands[] = ctools_ajax_command_redirect($redirect);
}

function wydit_get_canonical_form_state($form_state, $reset_input = FALSE) {
  $new_form_state = array();
  $type_name = $form_state['type'];
  $new_form_state['cache name'] = $form_state['cache name'];
  $new_form_state['profile_id'] = $form_state['profile_id'];
  $new_form_state['type'] = $type_name;
  $new_form_state['parents'] = wydit_get_parents_from_form_state($form_state);
  $new_form_state['locked'] = $form_state['locked'];
  $new_form_state['current_tab'] = $form_state['current_tab'];
  $new_form_state[$type_name] = $form_state[$type_name];
  if ($reset_input) {
    //Need that to avoid re-processing of the form (infinite loop)
    $new_form_state['input'] = NULL;
  }
  return $new_form_state;
}

function wydit_publish_prenode(&$form_state, $current_profile, $values = array()) {
  $type_name = $form_state['type'];
  $fields = wyditadmin_get_fields_flat($current_profile, $type_name);

  foreach($fields as $field) {
    if($field['display'] == 2 || $field['display'] == 3) {
      $field_name = $field['name'];
      if(isset($form_state[$field_name])) {
        foreach($form_state[$field_name] as $key => $value) {
          if ($nid = wydit_publish_prenode($form_state[$field_name][$key], $current_profile)) {
            unset($form_state[$field_name][$key]);
            $form_state[$field_name][$key]['nid'] = $nid;
          }
        }
      }
      $form_state['#node']->$field_name = $form_state[$field_name] ? $form_state[$field_name] : array();
    }
  }

  if (is_array($form_state['#node']->path) && isset($form_state['#node']->path['pathauto_perform_alias'])) {
    $form_state['#node']->pathauto_perform_alias = $form_state['#node']->path['pathauto_perform_alias'];
    unset($form_state['#node']->path);
  }

  switch ($form_state['type']) {
    case 'wally_articlepackage':
      $main_object_type = 'field_mainstory';
      break;
    case 'wally_gallerypackage':
      $main_object_type = 'field_mainobject';
      break;
    case 'wally_pollpackage':
      $main_object_type = 'field_mainpoll';
      break;
    default:
      $main_object_type = FALSE;
      break;
  }
  if ($main_object_type && (!isset($form_state['#node']->$main_object_type) || empty($form_state['#node']->$main_object_type))) {
    return FALSE;
  }

  return wydit_publish_prenode_save($form_state, $values);
}

function wydit_publish_prenode_save(&$form_state, $values) {
  global $user;
  module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
  $b = (object)array_merge($values, (array)$form_state['#node']);
  if (wallytoolbox_check_is_node($b)) {
    $b->doNotPullUp = TRUE;
    $b->uid = $user->uid;
    node_save($b);
    $form_state['#node'] = $b;
    return $b->nid;
  }
  return FALSE;
}

/**
 * Convert a type name is a real name which can be used as title.
 * 
 * @param string $type
 *   The type name to be converted.
 * 
 * @return
 *   The converted name.
 */
function wydit_convert_type_to_title($type) {
  $expl_type = explode('_', $type);
  $expl_type = explode('object', $expl_type[1]);
  return ucfirst($expl_type[0]);
}

function wydit_get_inline_type($referencable_types) {
  foreach($referencable_types as $key => $value) {
    if($value) {
      return $value;
    }
  }
  return NULL;
}

function wydit_get_fil_ariane($form_state) {
  $array_title = array();
  $delimiter = ' > ';

  $types = node_get_types();
  $parents = wydit_get_parents_from_form_state($form_state);
  $pack_type = $parents[0];

  $array_title[] = $types[$pack_type]->name.' : '.$form_state[$pack_type]['#node']->title;

  for($i = 3; $i <= sizeof($parents); $i += 2) {
    $father_type = wydit_get_target_from_parents($form_state, array_merge(array_slice($parents, 0, $i - 2), array('type')));
    $elem_type = wydit_get_target_from_parents($form_state, array_merge(array_slice($parents, 0, $i), array('type')));
    $elem_node = wydit_get_target_from_parents($form_state, array_merge(array_slice($parents, 0, $i), array('#node')));
    $field_infos = wyditadmin_get_existing_fields($father_type, $parents[$i - 2]);
    
    $array_title[] = $field_infos['label'];
    $array_title[] = $types[$elem_type]->name.($elem_node->title ? ' : '.$elem_node->title : '');
  }

  $max_key = max(array_keys($array_title));
  foreach ($array_title as $id => $elem) {
    if ($id == $max_key) {
      break;
    }
    $array_title[$id] = '<span style = "font-size: 17px; color: grey;">'.$elem.'</span>';
  }

  return implode($delimiter, $array_title);
}

function wydit_populate_form_state(&$form_state, $node, $current_profile) {
  if (isset($node->type) && !empty($node->type)) {
    $type_name = $node->type;
    $form_state['#node'] = $node;
    $form_state['type'] = $type_name;

    $type = wydit_get_infos_type($type_name);
    $fields = $type['fields'];
    $flat_fields = wyditadmin_get_fields_flat($current_profile, $type_name);
    $existing_fields = wyditadmin_get_existing_fields($type_name);

    foreach($flat_fields as $field_name => $infos) {
      $orig_field_infos = $existing_fields[$field_name];

      if($infos['display'] == 2 || $infos['display'] == 3) {
        $form_state[$field_name] = array();
        if($infos['display'] == 2) {
          if($inline_type = wydit_get_inline_type($orig_field_infos['referenceable_types'])) {
            $main_node = new stdClass;
            $main_node->type = $inline_type;
            $form_state[$field_name][0]['#node'] = $main_node;
            $form_state[$field_name][0]['type'] = $main_node->type;
          }
        }

        if ($node->{$field_name}) {
          foreach($node->{$field_name} as $key => $value){
            if ($temp_node = node_load($value['nid'])) {
              wydit_populate_form_state($form_state[$field_name][$key], $temp_node, $current_profile);
            }
          }
        }
      }
    }
  }
}

function wydit_get_tmp_fs_from_form_state(&$form_state) {
  $i = 5;

  $parents = wydit_get_parents_from_form_state($form_state);
  while ($i) {
    while (sizeof($parents) > 1 && !is_numeric($parents[sizeof($parents) - 1])) {
      array_pop($parents);
    }

    if (($tmp_fs = wydit_get_target_from_parents($form_state, $parents)) && wydit_get_target_from_parents($form_state, array_merge($parents, array('type')))) {
      $type_name = $tmp_fs['type'];
    }

    if ($tmp_fs && $type_name) {
      $form_state['parents'] = $parents;
      return array($tmp_fs, $type_name);
    } else {
      array_pop($parents);
      $i--;
    }
  }
  
  if (isset($form_state['type']) && isset($form_state[$form_state['type']])) {
    $type_name = $form_state['type'];
    $form_state['parents'] = array($type_name);
    $tmp_fs = $form_state[$type_name];
    return array($tmp_fs, $type_name);
  }

  drupal_set_message(t('An error occured'), 'error');
  drupal_goto('node/'.$form_state['cache name']);
}

function wydit_get_parents_from_form_state ($form_state) {
  return (empty($form_state['parents']) || empty($form_state['parents'][0])) ? array($form_state['type']) : $form_state['parents'];
}

/**
 ********************************************
 * END OF FORM AND FORM_STATE MANIPULATIONS *
 ********************************************
 */

/**
 *****************
 * MISCELLANEOUS *
 *****************
 */

function wydit_preprocess_batch_upload_zip($file, &$return){
  $zipped_objects = array();
  $zip_error = FALSE;
  $error_msg = FALSE;

  // Unzip input file to the temp directory
  $zip = new ZipArchive;
  $res = $zip->open($file->filename);
  if ($res === TRUE) {
    $file_name = strstr($file->name, '.zip') ? str_replace('.zip', '', $file->name) : $file->name;
    $temp_subdir = file_directory_path().'/tmp_batch_upload/'.$file_name;
    module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
    file_check_directory_recursive($temp_subdir, 1);

    if (is_dir($temp_subdir)) {
      $zip->extractTo($temp_subdir);
      $zipped_files = file_scan_directory($temp_subdir, '.*');
      if (is_array($zipped_files)) {
        foreach ($zipped_files as $zipped_file) {
          if (strstr(mime_content_type($zipped_file->filename), 'image')) {
            $zipped_file->type = 'photo';
          } else {
            $zipped_file->type = 'other';
          }
          $zipped_objects[] = $zipped_file;
        }
      }
      
      if (count($zipped_objects) == 0) {
        $zip_error = TRUE;
        $error_msg = t('No files found in the import: !file.', array('!file' => $file->basename));
        $return = array('error' => TRUE, 'error_msg' => $error_msg);
      }
      
      $zip->close();
    } else {
      $zip_error = TRUE;
      $error_msg = t('Input file !file could not be unzipped to !subdir. Error code !res', array('!file' => $file->filename, '!subdir' => $temp_subdir, '!res' => $res));
      $return = array('error' => TRUE, 'error_msg' => $error_msg);
    }
  } else {
    $zip_error = TRUE;
    $error_msg = t('Cant UnZip file: !file', array('!file' => $file->filename));
    $return = array('error' => TRUE, 'error_msg' => $error_msg);
  }

  return $zipped_objects;
}

function wydit_create_file($type, $field_name, $src, $file_name) {
  $field = content_fields($field_name, $type);
  $dest = filefield_widget_file_path($field);
  $dest_path = $dest.'/'.$file_name;
  module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
  $orig_src = $src;
  
  if (!file_check_directory_recursive($dest, 1) || !file_copy($src, $dest_path)) {
    watchdog('WallyEdit file', t('File copy from @from to @to failed', array('@from' => $orig_src, '@to' => $dest)), array(), WATCHDOG_ERROR);
  } else {
    global $user;
    $uid = $user->uid;
    
    $file = new stdClass();
    $file->filename = $file_name;
    $file->filepath = $src;
    $file->filemime = file_get_mimetype($src);
    $file->filesize = filesize($src);
    $file->uid = $uid;
    $file->status = FILE_STATUS_TEMPORARY;
    $file->timestamp = time();
    drupal_write_record('files', $file);
    $return = field_file_load($src);
    return $return;
  }
  
  return FALSE;
}

/**
 ************************
 * END OF MISCELLANEOUS *
 ************************
 */
