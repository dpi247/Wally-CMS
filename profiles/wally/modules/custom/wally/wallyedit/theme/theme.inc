<?php

function wallyedit_preprocess_wallyedit_form_fields_overview_form(&$variables) {
  drupal_add_css(drupal_get_path('module', 'wallyedit') . '/css/wallyedit.css');
  drupal_add_js(drupal_get_path('module', 'wallyedit') . '/js/wallyedit.js');

  $form = $variables['form'];
  $type_name = $form['type']['#value'];
  $profile_id = (int) $form['profile_id']['#value'];

  $profile = db_fetch_array(db_query('SELECT * FROM {wallyedit_profiles} WHERE pid = %d', $profile_id));
  $variables['caption'] = $caption = t('Fields management for profile "!profile_label" and content type "!content_type"', array('!profile_label' => $profile['label'], '!content_type' => $type_name));

  $tabs = wyditadmin_get_tabs($profile_id, $type_name);

  foreach($tabs as $tab){
    $tabs_infos[$tab['tid']]=$tab;
  }

  $variables['tabs_infos'] = $tabs_infos;// + array(-1 => t('Disabled'));

  $variables['tabs_elements'] = array();
  foreach (element_children($form['fields']) as $i) {
    $element = &$variables['form']['fields'][$i];
    // Only take form elements that are blocks.
    if (isset($element['weight'])) {
      // Fetch region for current block.
      $tab = $element['tab']['#default_value'];
      // Set special classes needed for table drag and drop.
      $variables['form']['fields'][$i]['tab']['#attributes']['class'] = 'my-element-tab-lev1 my-element-tab-lev1-'. $tab;
      $variables['form']['fields'][$i]['weight']['#attributes']['class'] = 'element-weight-lev1 element-weight-lev1-'. $tab;

      $variables['tabs_elements'][$tab][$i] = new stdClass();
      $variables['tabs_elements'][$tab][$i]->row_class = isset($element['#attributes']['class']) ? $element['#attributes']['class'] : '';
      $variables['tabs_elements'][$tab][$i]->block_modified = isset($element['#attributes']['class']) && strpos($element['#attributes']['class'], 'block-modified') !== FALSE ? TRUE : FALSE;
      $variables['tabs_elements'][$tab][$i]->label =  drupal_render($element['label']);
      $variables['tabs_elements'][$tab][$i]->region_select = drupal_render($element['tab']);
      $variables['tabs_elements'][$tab][$i]->group_select = drupal_render($element['group']);
      $variables['tabs_elements'][$tab][$i]->weight_select = drupal_render($element['weight']);
      $variables['tabs_elements'][$tab][$i]->wallyedit_select = drupal_render($element['wallyedit']);
      $variables['tabs_elements'][$tab][$i]->printed = FALSE;

      $last_region = $tab;
    }
  }
  $variables['form_submit'] = drupal_render($variables['form']);
}

function wallyedit_preprocess_wallyedit_node_form(&$vars) {
  module_load_include('inc', 'wallyedit', 'includes/config_api');
  
  $profile_id = $vars['profile_id'] = $vars['form']['profile_id']['#value'];
  $node_type = $vars['node_type'] = $vars['form']['type']['#value'];
  
  $js_settings = array(
    'profile_id' => $profile_id,
    'current_tab' => $vars['form']['current_tab']['#value'],
  );

  drupal_add_css(drupal_get_path('module', 'wallyedit').'/css/wallyedit.css');
  drupal_add_js(drupal_get_path('module', 'wallyedit').'/js/tab_slider.js');
  drupal_add_js($js_settings, 'setting');

  $global_form = &$vars['form'];
  $form = &$vars['form'][$node_type];
  $onglets_struct = wyditadmin_get_fields_tree($profile_id, $node_type);
  $type = $vars['type'] = wydit_get_infos_type($node_type);
  $extra_fields = array();
  wallyedit_get_extra_fields($extra_fields, $form, array_keys($type['fields']));
  $cck_fields = $vars['cck_fields'] = $type['fields'] + $extra_fields;
  $flat_fields = wyditadmin_get_fields_flat($profile_id, $node_type);

  $hardcoded_fields = array(
    'title',
    'revision_information',
    'author',
    'options',
    'comment_settings',
    'menu',
    'path',
    'attachments',
  );
  
  foreach ($cck_fields as $field_name => $field) {
    if (isset($field['display_settings']['parent']) && in_array($field['display_settings']['parent'], $hardcoded_fields)) {
      foreach($onglets_struct as $onglet => $onglet_content) {
        foreach($onglet_content['elements'] as $group_id => $group_content) {
          foreach($group_content['fields'] as $element_name => $element_content) {
            if ($element_name == $field_name) {
              $onglets_struct[$onglet]['elements'][$group_id]['fields'][$field['display_settings']['parent']] = $element_content;
              break 3;
            }
          }
        }
      }
    }
  }
  $vars['onglets_struct'] = $onglets_struct;

  if (module_exists('fieldgroup')) {
    // Flatten the form so we do not dependent on the groups
    $groups = fieldgroup_groups($node_type);
    foreach ($groups as $group) {
      if (isset($form[$group['group_name']])) {
        foreach ($group['fields'] as $field) {
          if (isset($form[$group['group_name']][$field['field_name']])) {
            $form[$field['field_name']] = $form[$group['group_name']][$field['field_name']];
          }
        }
        unset($form[$group['group_name']]);
      }
    }
  }

  $no_display = '';
  $no_display_struct = array();
  foreach($onglets_struct['no_tab']['elements'] as $group_id => $group_content) {
    foreach($onglets_struct['no_tab']['elements'][$group_id]['fields'] as $element_name => $element_content) {
      $no_display .= drupal_render($form[$element_name]);
    }
    $no_display_struct = array_merge($no_display_struct, array_keys($group_content['fields']));
  }
  foreach ($hardcoded_fields as $field) {
    if (isset($form[$field])) {
      list($tmp_no_display, $rendered) = _wallyedit_preprocess_no_display($form[$field], $no_display_struct, $flat_fields);
      $no_display .= $tmp_no_display;
    }
  }

  $no_display .= drupal_render($global_form['profile_id']);
  $no_display .= drupal_render($global_form['type']);
  $vars['no_display'] = $no_display;

  $buttons = '';
  $buttons .= drupal_render($global_form['save_global']);
  $buttons .= drupal_render($global_form['publish_global']);
  $buttons .= drupal_render($global_form['cancel_global']);
  $buttons .= drupal_render($global_form['reset_global']);
  $buttons .= drupal_render($global_form['delete_global']);
  $vars['buttons'] = $buttons;
  $profile_selector = '';
  $profile_selector .= drupal_render($global_form['choose_profile']);
  $profile_selector .= drupal_render($global_form['confirm_profile']);
  $vars['profile_selector'] = $profile_selector;
}

function _wallyedit_preprocess_no_display(&$subform, $struct) {
  $no_display = '';
  $rendered_elem = 0;
  $children = element_children($subform);
  foreach ($children as $elem) {
    if (in_array($elem, $struct, TRUE)) {
      $no_display .= drupal_render($subform[$elem]);
      $rendered_elem++;
    } else {
      list($tmp_no_display, $rendered) = _wallyedit_preprocess_no_display($subform[$elem], $struct);
      if ($rendered) {
        $rendered_elem++;
      }
      $no_display .= $tmp_no_display;
    }
  }
  $rendered_subform = FALSE;
  if (sizeof($children) && $rendered_elem == sizeof($children)) {
    $no_display .= drupal_render($subform);
    $rendered_subform = TRUE;
  }
  return array($no_display, $rendered_subform);
}

function wallyedit_preprocess_wallyedit_inline_form(&$vars){
  $vars['profile_id']=$vars['form'][0]['profile_id']['#value'];
  $vars['node_type']=$vars['form'][0]['type']['#value'];
}

function wallyedit_preprocess_wallyedit_form_display_tabs_form(&$vars) {
  $form = &$vars['form'];

  $vars['help'] = theme('advanced_help_topic', 'content', 'manage-fields') . t('Add fields and groups to the content type, and arrange them on content display and input forms.');
  if (module_exists('fieldgroup')) {
    $vars['help'] .= '<br/>'. t('You can add a field to a group by dragging it below and to the right of the group.');
  }
  if (!module_exists('advanced_help')) {
    $vars['help'] .= '<br/>' . t('Note: Installing the <a href="!adv_help">Advanced help</a> module will let you access more and better help.', array('!adv_help' => 'http://drupal.org/project/advanced_help'));
  }

  $order = _content_overview_order($form, $form['#field_rows'], $form['#group_rows']);
  $rows = array();

  // Identify the 'new item' keys in the form, they look like
  // _add_new_field, add_new_group.
  $keys = array_keys($form);
  $add_rows = array();
  foreach ($keys as $key) {
    if (substr($key, 0, 4) == '_add') {
      $add_rows[] = $key;
    }
  }
  while ($order) {
    $key = reset($order);
    $element = &$form[$key];
    
    // Only display the 'Add' separator if the 'add' rows are still
    // at the end of the table.
    if (!isset($added_separator)) {
      $remaining_rows = array_diff($order, $add_rows);
      if (empty($remaining_rows) && empty($element['#depth'])) {
        $row = new stdClass();
        $row->row_type = 'separator';
        $row->class = 'tabledrag-leaf region';
        $rows[] = $row;
        $added_separator = TRUE;
      }
    }

    if (isset($element)) {
      $row = new stdClass();
      // Add target classes for the tabledrag behavior.
      $element['weight']['#attributes']['class'] = 'field-weight';
      $element['parent']['#attributes']['class'] = 'group-parent';
      $element['hidden_name']['#attributes']['class'] = 'field-name';
      // Add target classes for the update selects behavior.
      switch ($element['#row_type']) {
        case 'add_new_field':
          $element['type']['#attributes']['class'] = 'content-field-type-select';
          $element['widget_type']['#attributes']['class'] = 'content-widget-type-select';
          break;
        case 'add_existing_field':
          $element['field_name']['#attributes']['class'] = 'content-field-select';
          $element['widget_type']['#attributes']['class'] = 'content-widget-type-select';
          $element['label']['#attributes']['class'] = 'content-label-textfield';
          break;
      }
      foreach (element_children($element) as $child) {
        $row->{$child} = drupal_render($element[$child]);
      }
      $row->label_class = 'label-'. strtr($element['#row_type'], '_', '-');
      $row->row_type = $element['#row_type'];
      $row->indentation = theme('indentation', isset($element['#depth']) ? $element['#depth'] : 0);
      $row->class = 'draggable';
      $row->class .= isset($element['#disabled_row']) ? ' menu-disabled' : '';
      $row->class .= isset($element['#add_new']) ? ' content-add-new' : '';
      $row->class .= isset($element['#leaf']) ? ' tabledrag-leaf' : '';
      $row->class .= isset($element['#root']) ? ' tabledrag-root' : '';

      $rows[] = $row;
    }
    array_shift($order);
  }
  $vars['rows'] = $rows;
  $vars['submit'] = drupal_render($form);

  // Add tabledrag behavior.
  drupal_add_tabledrag('content-field-overview', 'match', 'parent', 'group-parent', 'group-parent', 'field-name', TRUE, 1);
  drupal_add_tabledrag('content-field-overview', 'order', 'sibling', 'field-weight');

  // Add settings for the update selects behavior.
  $js_fields = array();
  foreach (array_keys(content_existing_field_options($form['#type_name'])) as $field_name) {
    $field = content_fields($field_name);
    $js_fields[$field_name] = array('label' => $field['widget']['label'], 'type' => $field['type'], 'widget' => $field['widget']['type']);
  }
  
  drupal_add_js(array('contentWidgetTypes' => content_widget_type_options(), 'contentFields' => $js_fields), 'setting');
  drupal_add_js(drupal_get_path('module', 'content') .'/content.js');
}

function theme_wallyedit_fieldset($variables) {
  $element = $variables['element'];
  
  $output .= $element['#children'];
  if (isset($element['#value'])) {
    $output .= $element['#value'];
  }
  
  return $output;
}

function theme_wallyedit_admin($rows = array(), $headers) {
  drupal_set_title("Wallyedit Admin");
  $output = '';

  if (!empty($rows)) {
    foreach($rows as $row){
      $data = array();
      foreach($row as $elem){
        $data[] = $elem;
      }
      $collumn = array('data'=> $data);
      $collumns[] = $collumn;
    }
    
    $output = theme('table',$headers, $collumns);
    $output .= theme('pager');
  }
  
  return $output;
}

function theme_wallyedit_page_editmode($locked, $form, $form_prev, $messages) {
  drupal_add_css(drupal_get_path('module', 'wallyedit').'/css/wallyedit.css');
  
  $output = '<div id="wallyedit_messages">';
  $output .= $messages;
  $output .= '</div>';
  $output .= '<div id="wallyedit_locked">';
  $output .= $locked;
  $output .= '</div>';
  $output .= '<div id="wallyedit_container">';
  $output .= $form;
  $output .= '</div>';
  $output .= '<div id="wallyedit_preview_container">';
  $output .= $form_prev;
  $output .=' </div>';

  return $output;
}

function theme_wallyedit_preview_id($cache_name, $dest = 'disabled') {
  $output.='<div id="wallyedit_preview">';
  $output.= theme('wallyedit_preview_iframe', $cache_name, $dest);
  $output.='</div>';
  
  return $output;
}

function theme_wydit_selector_theme($form) {
  $header = array();
  $header[] = t('Order');
  $header[] = t('Title');
  $header[] = t('Type');
  $header[] = t('Action(s)');

  foreach($form as $key => $element) {
    if(is_integer($key)){
      $row = array();

      $row[] = drupal_render($element['title']);
      $row[] = drupal_render($element['weight']);
      $row[] = drupal_render($element['type']);
      $row[] = drupal_render($element['edit']).' '.drupal_render($element['delete']);

      $rows[] = array('data'=>$row, 'class'=>'draggable');
    }
  }

  $table_id = 'table_order_'.$form['#parents'][count($form['#parents']) - 1];
  drupal_add_tabledrag($table_id,  'order', 'self', 'order_weight', NULL, NULL, TRUE);

  $output = theme('table', $header, $rows, array('id' => $table_id));
  $output .= drupal_render($form['add_existing']);
  $output .= drupal_render($form['add_new']);
  $output .= drupal_render($form['batch_import']);

  return  $output;
}

function theme_wallyedit_form_tabs_content_types_overview_form($form) {
  if (!isset($form['rows']) || empty($form['rows'])) {
    drupal_set_message(t('No content types!'), 'error');
    drupal_goto('admin/content/wallydisplaytabs');
  }

  if ($profile = db_fetch_array(db_query('SELECT * FROM {wallyedit_profiles} WHERE pid = %d', $form['profile_id']['#value']))) {
    
  } else {
    drupal_set_message(t('Profile not found!'), 'error');
    drupal_goto('admin/content/wallydisplaytabs');
  }

  $table_rows = array();
  //loop through each "row" in the table array
  foreach(element_children($form['rows']) as $key){
    $content_form = $form['rows'][$key];
    $this_row = array(
      $content_form['name']['#value'],
      $content_form['type']['#value'],
      $content_form['description']['#value'],
      drupal_render($content_form['edition']),
      $content_form['operations']['#value'],
    );
    //Add the row to the array of rows
    $table_rows[] = $this_row;
  }
  unset($form['rows']);

  //Make sure the header count matches the column count
  $header = array(t('Name'), t('Type'), t('Description'), t('Edition type'), array('data' => t('Operations'), 'colspan' => '2'));
  $caption = t('Choose the content type to edit for profile "!profile_label"', array('!profile_label' => $profile['label']));
  $output = theme('table', $header, $table_rows, array(), $caption);
  $output .= drupal_render($form);

  return $output;
}

function theme_wallyedit_form_groups_overview_form($form) {
  if (!isset($form['rows']) || empty($form['rows'])) {
    drupal_set_message(t('No groups!'), 'error');
    drupal_goto('admin/content/wallydisplaytabs');
  }
  
  $content_names = node_get_types('names');
  $content_name = $content_names[$form['content_key']['#value']];

  $group_rows = array();
  foreach (element_children($form['rows']) as $group_id) {
    $group_form = $form['rows'][$group_id];
    $row = array(
      $group_form['label']['#value'],
      $group_form['operations']['#value'],
      //Add the weight field to the row
      drupal_render($form['rows'][$group_id]['weight']),
    );
    $groups_rows[] = array('data' => $row, 'class' => 'draggable');
  }

  $groups_header = array(t('Label'), t('Operations'), t('Order'));
  drupal_add_tabledrag('example-table', 'order', 'self', 'weight', NULL, NULL, TRUE);
  $groups_caption = t('Groups list for content type "@content_name"', array('@content_name' => $content_name));
  $output = theme('table', $groups_header, $groups_rows, array('id' => 'example-table'), $groups_caption);
  $output .= drupal_render($form);
  
  return $output;
}

function theme_wallyedit_form_tabs_overview_form($form) {
  if (!isset($form['rows']) || empty($form['rows'])) {
    drupal_set_message(t('No tabs!'), 'error');
    drupal_goto('admin/content/wallydisplaytabs');
  }

  if ($profile = db_fetch_array(db_query('SELECT * FROM {wallyedit_profiles} WHERE pid = %d', $form['profile_id']['#value']))) {
    
  } else {
    drupal_set_message(t('Profile not found!'), 'error');
    drupal_goto('admin/content/wallydisplaytabs');
  }
  
  $content_names = node_get_types('names');
  $content_name = $content_names[$form['content_key']['#value']];

  $tabs_rows = array();
  foreach (element_children($form['rows']) as $tab_id) {
    $tab_form = $form['rows'][$tab_id];
    $row = array(
      $tab_form['label']['#value'],
      $tab_form['operations']['#value'],
      //Add the weight field to the row
      drupal_render($form['rows'][$tab_id]['weight']),
    );
    $tabs_rows[] = array('data' => $row, 'class' => 'draggable');
  }

  $tabs_header = array(t('Label'), t('Operations'), t('Order'));
  drupal_add_tabledrag('example-table', 'order', 'self', 'weight', NULL, NULL, TRUE);
  $tabs_caption = t('Tabs list for profile "@profile" and content type "@content_name"', array('@profile' => $profile['label'], '@content_name' => $content_name));
  $output = theme('table', $tabs_header, $tabs_rows, array('id' => 'example-table'), $tabs_caption);
  $output .= drupal_render($form);
  
  return $output;
}

/**
 * Draw the "this task is locked from editing" box.
 */
function theme_wallyedit_lock($form_state) {
  $account  = user_load($form_state['locked']['uid']);
  $name     = theme('username', $account);
  $lock_age = format_interval(time() - $form_state['locked']['updated']);
  $break    = url(wydit_get_url_action('edit_prenode', $form_state['cache name']), array('query' => 'break=1'));

  ctools_add_css('ctools');
  $output = '<div class="ctools-locked">';
  $output .= t('This node is being edited by user !user, and is therefore locked from editing by others. This lock is !age old. Click here to <a href="!break">break this lock</a>.', array('!user' => $name, '!age' => $lock_age, '!break' => $break));
  $output .= '</div>';
  return $output;
}
