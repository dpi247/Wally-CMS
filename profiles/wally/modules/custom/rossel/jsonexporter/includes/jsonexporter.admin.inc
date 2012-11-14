<?php 

/**
 * List of existing products
 */
function jsonexporter_page_list_product(){
  $content = '';
  $products = jsonexporter_getproducts();

  if ($products != NULL){
    $header = array(t('Name'), t('Description'), t('Product link'), t('Operations'));
    $rows = array();
    foreach ($products as $product){
    	$rows[] = array(
        $product->name,
        $product->description,
        '<a href="/jsonexporter/product/'.$product->pid.'">'.t('View').'</a>',
        '<a href="/admin/rossel/jsonexporter/settings/add/'.$product->pid.'">'.t('Edit').'</a>'."&nbsp".'<a href="/admin/rossel/jsonexporter/settings/delete/'.$product->pid.'">'.t('Delete').'</a>'."&nbsp",
      );
    }
    $content .= theme_table($header,$rows);
  } else {
    $content = t('There is no product. To add one, click !link_here', array('!link_here' => '<a href="/admin/rossel/jsonexporter/settings/add">'.t('here').'</a>'));
  }
  return $content;
}

/**
 * Create/update product form.
 */
function jsonexporter_page_product_form($form_state, $edit_id = NULL){
	module_load_include('inc', 'content', 'includes/content.node_form');

	if ($edit_id != NULL){
	  // Get default values
	  $product = jsonexporter_getproduct($edit_id);
	  $dummy_form["#node"] = new stdClass();
	  $dummy_form["#node"]->field_destinations = $product->settings;
	  $dummy_form['presets'] =$product->presets;
	}

	// Get widget
	$destinations = content_fields('field_destinations');
	$form['jsonexporter'] = (array) content_field_form($dummy_form, $dummy_form_state, $destinations);

	$form['jsonexporter']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('The unique Name of the product.'),
    '#size' => 80,
    '#maxlength' => 100,
    '#required' => TRUE,
  );

  $form['jsonexporter']['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#size' => 80,
    '#maxlength' => 1000,
  );

  // Configurations to retrieve elements in the form, necessary for the "add new" and "remove" operations
  $form['jsonexporter']['#tree'] = TRUE;
  $form['jsonexporter']['field_destinations'][0]['#type_name'] = 'jsonexporter';
  $form['jsonexporter']['field_destinations']['field_destinations_add_more']['#submit'][0] = 'cckdestinations_add_more_submit_proxy';
  $form['jsonexporter']['field_destinations']['field_destinations_add_more']['#ahah']['path'] = str_replace('//', '/', 'cckdestinations/js_add_more/jsonexporter/field_destinations/jsonexporter/field_destinations');
  $form['jsonexporter']['field_destinations']['#parents'] = array('jsonexporter', 'field_destinations');
  $form['jsonexporter']['#field_info']['field_destinations']['display_settings']['parent'] = 'jsonexporter';

  $presets_raw=imagecache_presets();
  $presets=array();
  foreach($presets_raw as $preset){
    $presets[$preset['presetid']]=$preset['presetname'];
  }
  $form['jsonexporter']['presets'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Availllable Imagecache\'s presets'),
    '#default_value' => $dummy_form['presets'],
    '#options' =>$presets,
  );

  $form['buttons']['submit'] = array('#type' => 'submit', '#value' => t('Save') );

  if ($edit_id != NULL){
    $form['jsonexporter']['pid'] = array('#type' => 'value',);

    foreach ($form['jsonexporter'] as $key => $value) {
      if (isset($product->$key)) {
        $form['jsonexporter'][$key]['#default_value'] = $product->$key;
      }
    }

    $form['jsonexporter']['update'] = array(
      '#type' => 'hidden',
      '#value' => 'pid',
    );

    $form['delete'] = array(
      '#type' => 'markup',
      '#value' => '<a href = "admin/rossel/jsonexporter/settings/delete/'.$edit_id.'">'.t('Delete').'</a>',
    );
  }

  drupal_add_css(drupal_get_path('module', 'jsonexporter').'/css/product_admin.css');

  return $form;
}

/**
 * Hook_submit pour la création/update d'un product
 */
function jsonexporter_page_product_form_submit($form, &$form_state){
	$record = array(); 
	$record['name'] = $form_state['values']['jsonexporter']['name'];
	$record['description'] = $form_state['values']['jsonexporter']['description'];
	$record['pid'] = $form_state['values']['jsonexporter']['pid'];
	
	foreach ($form_state['values']['jsonexporter']['field_destinations'] as $key => $item) {
	  // Clear empty values
		if (cckdestinations_content_is_empty($item, $field)) unset($form_state['values']['jsonexporter']['field_destinations'][$key]);
	}

	usort($form_state['values']['jsonexporter']['field_destinations'], '_content_sort_items_helper');
	$record['settings'] = serialize($form_state['values']['jsonexporter']['field_destinations']);
	$record['presets'] = serialize($form_state['values']['jsonexporter']['presets']);
	
  $result = drupal_write_record('jsonexporter_product', $record, $form_state['values']['jsonexporter']['update']);

  if ($result != FALSE) {
    $message = t('The product as been created/updated');
  } else {
    $message = t('The product as not been created/udpated');
  }
  drupal_set_message($message);
  $form_state['redirect'] = 'admin/rossel/jsonexporter/settings';
}

/**
 * Delete importer form.
 */
function jsonexporter_page_productdelete_form($form_state, $delete_id = NULL) {
  if ($delete_id != NULL) {
    $question = t('Are you sure you want to delete this product?');
    
    $add_form = array();
    $add_form['pid'] = array(
      '#type' => 'value',
      '#value' => $delete_id,
    );
    
    $form = confirm_form($add_form, $question, 'admin/rossel/jsonexporter/settings/list');
    $form['#redirect'] = 'admin/rossel/jsonexporter/settings/list';
    return $form;
  } else {
    drupal_set_message(t('No product selected.'), 'error');
    drupal_goto('admin/rossel/jsonexporter/settings/list');
  }
}

/**
 * Submit of the importer delete form
 */
function jsonexporter_page_productdelete_form_submit($form, $form_state) {
  $result = db_query("DELETE FROM {jsonexporter_product} WHERE pid = %d", $form_state['values']['pid']);
  if ($result != FALSE){
    $message = t('The product as been deleted');
  } else {
    $message = t('The product as <b>not been</b> deleted');
  }
  drupal_set_message($message);
}

/**
 * General settings form
 */
function jsonexporter_page_general_settings_form($form_state) {
  $form = array();
  
  $menu_names = menu_get_names();
  $menu_options = array();
  foreach ($menu_names as $menu_name) {
    $menu_options[$menu_name] = $menu_name;
  }
  $form['jsonexporter_product_main_menu'] = array(
    '#type' => 'select',
    '#title' => t('Select main menu'),
    '#options' => $menu_options,
    '#default_value' => variable_get('jsonexporter_product_main_menu', 0),
  );
  
  return system_settings_form($form);
}
