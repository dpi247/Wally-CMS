<?php

function theme_wallyedit_admin($rows=array(),$headers){
  drupal_set_title("Wallyedit Admin");

  foreach($rows as $row){
    $collumn[]=array('data'=>$row['sid']);
    $collumn[]=array('data'=>$row['name']);
    $collumn[]=array('data'=>$row['data'][$row['data']['type']]['#node']->type);
    $collumn[]=array('data'=>l('edit',$row['edit_action_url']));
    $collumns[]=$collumn;
  }
  
  $output=theme('table',$headers, $collumns);
  return $output;
}

function theme_wallyedit_page_createmode($form,$prenode, $form_prev){
  drupal_add_css(drupal_get_path('module', 'wallyedit') . '/css/wallyedit.css');
  $output='<div id="wallyedit_messages">';
  $output.='</div>';
  
  $output.='<div id="wallyedit_container">';
  $output.=$form;
  $output.='</div>';
  $output.='<div id="wallyedit_preview_container">';
  $output.= $form_prev;
  $output.='</div>';
  return $output;
}

function theme_wallyedit_page_editmode($form,$prenode, $form_prev){
  drupal_add_css(drupal_get_path('module', 'wallyedit') . '/css/wallyedit.css');
  $output='<div id="wallyedit_messages">';
  $output.='</div>';
  
  $output.='<div id="wallyedit_container">';
  $output.=$form;
  $output.='</div>';
  $output.='<div id="wallyedit_preview_container">';
  $output.= $form_prev;
  $output.='</div>';

  return $output;
}

function theme_wallyedit_preview_id($cache_name, $dest = 'disabled'){
  $output.='<div id="wallyedit_preview">';
  $output.= theme('wallyedit_preview_iframe', $cache_name, $dest);
  $output.='</div>';
  
  return $output;
}

function theme_wydit_selector_theme($form){
  $header=array();
  $header[] = t('Order');
  $header[] = t('Title');
  $header[] = t('Type');
  $header[] = t('Action(s)');
  
  foreach($form as $key=>$element){
    if(is_integer($key)){
      $row = array();
      
      /*
      $row[] = array('data' =>drupal_render($element['weight']));
      $row[] = array('data' => drupal_render($element['title']));
      $row[] = array('data' => drupal_render($element['type']));
      $row[] = array('data' => drupal_render($element['edit_action']).' '.drupal_render($element['delete_action']));
      */
      
      $row[] =drupal_render($element['weight_'.$key]);
      $row[] = drupal_render($element['title']);
      $row[] = drupal_render($element['type']);
      $row[] = drupal_render($element['edit_action']).' '.drupal_render($element['delete_action']);
          
      $rows[] = array('data'=>$row, 'class'=>'draggable');
    }
  }
  
  $table_id='table_order_'.$form['#parents'][count($form['#parents'])-1];

  drupal_add_tabledrag($table_id, 'order', 'sibiling', 'order_weightss', NULL, NULL,FALSE);    
  $output = theme('table', $header, $rows,array('id'=>$table_id));
  
  $output .= drupal_render($form['add_existing_action']);
  $output .= drupal_render($form["add_new_action"]);
  
  return  $output;
}

function wallyedit_preprocess_wallyedit_form_display_fields_form(&$vars){
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
//  drupal_add_tabledrag('content-field-overview', 'match', 'parent', 'group-parent', 'group-parent', 'field-name', FALSE, 1);
  drupal_add_tabledrag('content-field-overview', 'match', 'parent', 'group-parent', 'group-parent', 'field-name', TRUE, 1);
//  drupal_add_tabledrag('content-field-overview', 'order', 'sibling', 'field-weight', NULL, NULL, FALSE);
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
