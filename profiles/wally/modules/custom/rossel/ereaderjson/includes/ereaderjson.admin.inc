<?php 

/**
 * List of existing products
 */
function ereaderjson_page_list_product(){
  $content = '';
  $products = ereaderjson_getproducts();

  if ($products != NULL){
    $header = array(t('Name'), t('Description'), t('Product link'), t('Operations'));
    $rows = array();
    foreach ($products as $product){
    	$rows[] = array(
        $product->name,
        $product->description,
        '<a href="/ereader/product/'.$product->pid.'">'.t('View').'</a>',
        '<a href="/admin/rossel/ereader/settings/add/'.$product->pid.'">'.t('Edit').'</a>'."&nbsp".'<a href="/admin/rossel/ereader/settings/delete/'.$product->pid.'">'.t('Delete').'</a>'."&nbsp",
      );
    }
    $content .= theme_table($header,$rows);
  } else {
    $content = t('There is no product. To add one, click !link_here', array('!link_here' => '<a href="/admin/rossel/ereader/settings/add">'.t('here').'</a>'));
  }
  return $content;
}

/**
 * Create/update product form.
 */
function ereaderjson_page_product_form($form_state, $edit_id = NULL){
	module_load_include('inc', 'content', 'includes/content.node_form');

	if ($edit_id != NULL){
		$product = ereaderjson_getproduct($edit_id);
		$dummy_form["#node"] = new stdClass();
		$dummy_form["#node"]->field_destinations = unserialize($product->settings);
	}

	$destinations = content_fields('field_destinations');
	$form['ereaderjson'] = (array) content_field_form($dummy_form, $dummy_form_state, $destinations);

	$form['ereaderjson']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('The unique Name of the product.'),
    '#size' => 80,
    '#maxlength' => 100,
    '#required' => TRUE,
  );

  $form['ereaderjson']['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#size' => 80,
    '#maxlength' => 1000,
  );

  $form['ereaderjson']['#tree'] = TRUE;
  $form['ereaderjson']['field_destinations'][0]['#type_name'] = 'ereaderjson';
  $form['ereaderjson']['field_destinations']['field_destinations_add_more']['#submit'][0] = 'cckdestinations_add_more_submit_proxy';
  $form['ereaderjson']['field_destinations']['field_destinations_add_more']['#ahah']['path'] = str_replace('//', '/', 'cckdestinations/js_add_more/ereaderjson/field_destinations/ereaderjson/field_destinations');
  $form['ereaderjson']['field_destinations']['#parents'] = array('ereaderjson', 'field_destinations');
  $form['ereaderjson']['#field_info']['field_destinations']['display_settings']['parent'] = 'ereaderjson';

  $form['buttons']['submit'] = array('#type' => 'submit', '#value' => t('Save') );

  if ($edit_id != NULL){
    $form['ereaderjson']['pid'] = array('#type' => 'value',);

    foreach ($form['ereaderjson'] as $key => $value) {
      if (isset($product->$key)) {
        $form['ereaderjson'][$key]['#default_value'] = $product->$key;
      }
    }

    $form['ereaderjson']['update'] = array(
      '#type' => 'hidden',
      '#value' => 'pid',
    );

    $form['delete'] = array(
      '#type' => 'markup',
      '#value' => '<a href = "admin/rossel/ereader/settings/delete/'.$edit_id.'">'.t('Delete').'</a>',
    );
  }

  return $form;
}

/**
 * Hook_submit pour la création/update d'un product
 */
function ereaderjson_page_product_form_submit($form, &$form_state){
	$record = array(); 
	$record['name'] = $form_state['values']['ereaderjson']['name'];
	$record['description'] = $form_state['values']['ereaderjson']['description'];
	$record['pid'] = $form_state['values']['ereaderjson']['pid'];
	
	foreach ($form_state['values']['ereaderjson']['field_destinations'] as $key=>$item) {
		if (cckdestinations_content_is_empty($item, $field)) unset($form_state['values']['ereaderjson']['field_destinations'][$key]);
	}

	usort($form_state['values']['ereaderjson']['field_destinations'], '_content_sort_items_helper');
	$record["settings"] = serialize($form_state['values']['ereaderjson']['field_destinations']);

  $result = drupal_write_record('ereaderjson_product', $record, $form_state['values']['ereaderjson']['update']);

  if ($result != FALSE) {
    $message = t('The product as been created/updated');
  } else {
    $message = t('The product as not been created/udpated');
  }
  drupal_set_message($message);
  $form_state['redirect'] = 'admin/rossel/ereader/settings';
}

/**
 * Delete importer form.
 */
function ereaderjson_page_productdelete_form($form_state, $delete_id = NULL) {
  if ($delete_id != NULL) {
    $question = t('Are you sure you want to delete this product?');
    
    $add_form = array();
    $add_form['pid'] = array(
      '#type' => 'value',
      '#value' => $delete_id,
    );
    
    $form = confirm_form($add_form, $question, 'admin/rossel/ereader/settings/list');
    $form['#redirect'] = 'admin/rossel/ereader/settings/list';
    return $form;
  } else {
    drupal_set_message(t('No product selected.'), 'error');
    drupal_goto('admin/rossel/ereader/settings/list');
  }
}

/**
 * Submit of the importer delete form
 */
function ereaderjson_page_productdelete_form_submit($form, $form_state) {
  $result = db_query("DELETE FROM {ereaderjson_product} WHERE pid = %d", $form_state['values']['pid']);
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
function ereaderjson_page_general_settings_form($form_state) {
  $form = array();
  
  $menu_names = menu_get_names();
  $menu_options = array();
  foreach ($menu_names as $menu_name) {
    $menu_options[$menu_name] = $menu_name;
  }
  $form['ereader_product_main_menu'] = array(
    '#type' => 'select',
    '#title' => t('Select main menu'),
    '#options' => $menu_options,
    '#default_value' => variable_get('ereader_product_main_menu', 0),
  );
  
  return system_settings_form($form);
}
