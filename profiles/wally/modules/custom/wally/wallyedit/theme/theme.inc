<?php


function theme_wallyedit_admin($rows=array(),$headers){
  drupal_set_title("Wallyedit Admin");

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
  
  return $output;
}

function theme_wallyedit_page_createmode($form){
  drupal_add_css(drupal_get_path('module', 'wallyedit') . '/css/wallyedit.css');
  $output='<div id="wallyedit_messages">';
  $output.='</div>';
  
  $output.='<div id="wallyedit_container">';
  $output.=$form;
  $output.='</div>';
  return $output;
}

function theme_wallyedit_page_editmode($form, $form_prev) {
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
  
  foreach($form as $key=>$element) {
    if(is_integer($key)){
      $row = array();
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

function theme_wallyedit_form_tabs_content_types_overview_form ($form) {
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

function theme_wallyedit_form_tabs_overview_form ($form) {
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

function theme_wallyedit_form_fields_overview_form ($form) {
  return '';
}

function wallyedit_preprocess_wallyedit_form_fields_overview_form (&$variables) {
  $form = $variables['form'];
  $type_name = $form['type']['#value'];
  $profile_id = $form['profile_id']['#value'];
  $form_state = array();
  $options_onglet_infos = wallyedit_get_onglets($form_state, $profile_id, $type_name);
  foreach ($options_onglet_infos as $key => $value) {
    $options_onglet[$key] = $value['label'];
  }

  $variables['element_onglets'] = $options_onglet;// + array(-1 => t('Disabled'));
  $variables['element_listing'] = array();

  foreach (element_children($variables['form']['level1']) as $i) {
    $element = &$variables['form']['level1'][$i];

    // Only take form elements that are blocks.
    if (isset($element['weight'])) {
      // Fetch region for current block.
      $onglet = $element['onglet']['#default_value'];
      // Set special classes needed for table drag and drop.
      $variables['form']['level1'][$i]['onglet']['#attributes']['class'] = 'my-element-onglet-lev1 my-element-onglet-lev1-'. $onglet;
      $variables['form']['level1'][$i]['weight']['#attributes']['class'] = 'element-weight-lev1 element-weight-lev1-'. $onglet;

      $variables['element_listing'][$onglet][$i] = new stdClass();
      $variables['element_listing'][$onglet][$i]->row_class = isset($element['#attributes']['class']) ? $element['#attributes']['class'] : '';
      $variables['element_listing'][$onglet][$i]->block_modified = isset($element['#attributes']['class']) && strpos($element['#attributes']['class'], 'block-modified') !== FALSE ? TRUE : FALSE;
      $variables['element_listing'][$onglet][$i]->block_title =  drupal_render($element['field_name']);
      $variables['element_listing'][$onglet][$i]->region_select = drupal_render($element['onglet']);
      $variables['element_listing'][$onglet][$i]->weight_select = drupal_render($element['weight']);
      $variables['element_listing'][$onglet][$i]->wallyedit_select = drupal_render($element['wallyedit']);
      $variables['element_listing'][$onglet][$i]->configure_link = drupal_render($element['configure']);
      $variables['element_listing'][$onglet][$i]->printed = FALSE;

      if($variables['form']['level1'][$i]['level2']){
        $variables['element_listing'][$onglet][$i]->has_level2=TRUE;
        foreach(element_children($variables['form']['level1'][$i]['level2']) as $index_j=>$j){
          $variables['form']['level1'][$i]['level2'][$j]['weight']['#attributes']['class'] = 'element-weight-lev2 element-weight-lev2-'. $i;

          $subelement=&$variables['form']['level1'][$i]['level2'][$j];

          $variables['element_listing'][$onglet][$i]->level2[$j] = new stdClass();
          $variables['element_listing'][$onglet][$i]->level2[$j]->row_class = isset($subelement['#attributes']['class']) ? $subelement['#attributes']['class'] : '';
          $variables['element_listing'][$onglet][$i]->level2[$j]->block_modified = isset($subelement['#attributes']['class']) && strpos($subelement['#attributes']['class'], 'block-modified') !== FALSE ? TRUE : FALSE;
          $variables['element_listing'][$onglet][$i]->level2[$j]->block_title =  drupal_render($subelement['field_name']);
          $variables['element_listing'][$onglet][$i]->level2[$j]->weight_select = drupal_render($subelement['weight']);
          $variables['element_listing'][$onglet][$i]->level2[$j]->wallyedit_select = drupal_render($subelement['wallyedit']);
          $variables['element_listing'][$onglet][$i]->level2[$j]->configure_link = drupal_render($subelement['configure']);
          $variables['element_listing'][$onglet][$i]->level2[$j]->printed = FALSE;
        }
      }
      $last_region = $onglet;
    }
  }

  // Add table javascript.
  drupal_add_js('misc/tableheader.js');
  drupal_add_js(drupal_get_path('module', 'wallyedit') .'/js/wallyedit.js');
  drupal_add_css(drupal_get_path('module', 'wallyedit') .'/css/wallyedit.css');

  $variables['form_submit'] = drupal_render($variables['form']);
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

function wallyedit_preprocess_wallyedit_form_display_tabs_form(&$vars){
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
