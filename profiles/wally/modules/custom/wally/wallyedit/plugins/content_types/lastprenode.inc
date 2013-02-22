<?php
/**
* Callback function to supply a list of content types.
*/
function wallyedit_lastprenode_ctools_content_types() {

  return array(
    'single' => TRUE,
    'title' => t('Last prenode'),
    'category' => t('Dashboard'),
    'description' => t('Display last prenode.'),
    'defaults' => array(),
  );
}

function wallyedit_lastprenode_content_type_edit_form(&$form, &$form_state) {
  // The current configuration
  $options = array();
  for ($i=1; $i < 21; $i++){
    $options[$i] = $i;
  }
  $form['item_number'] = array(
    '#type' => 'select',
    '#title' => t('Select the number of items to display'),
    '#options' => $options,
    '#default_value' => $form_state['conf']['item_number'],
  );
}

function wallyedit_lastprenode_content_type_edit_form_submit(&$form, &$form_state) {
  // For each part of the form defined in the 'defaults' array set when you
  // defined the content type, copy the value from the form into the array
  // of items to be saved. We don't ever want to use
  // $form_state['conf'] = $form_state['values'] because values contains
  // buttons, form id and other items we don't want stored. CTools will handle
  // the actual form submission.
  $form_state['conf']['item_number'] = $form_state['values']['item_number'];
}

/**
* Output function for the 'shortcut' content type.
*/
function wallyedit_lastprenode_content_type_render($subtype, $conf, $panel_args, $context,$a) {
  module_load_include('inc', 'wallyedit', 'includes/page_admin');
  $block->title = t('Last prenode');
  $block->content = '<div class="wallyedit_lastprenode">'.wallyedit_page_list_prenodes_dashboard($conf['item_number']).'</div>';
  return $block;
}