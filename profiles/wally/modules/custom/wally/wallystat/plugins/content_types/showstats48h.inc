<?php
/**
* Callback function to supply a list of content types.
*/
function wallystat_showstats48h_ctools_content_types() {

  return array(
    'single' => TRUE,
    'title' => t('[Node] Number of views during the last 48 hours'),
    'category' => t('Dashboard'),
    'description' => t('Display a chart of the number of visits during the last 48 hours'),
    'required context' => new ctools_context_required(t('Node'), 'node'),
    'defaults' => array(),
  );
}

function wallystat_showstats48h_content_type_edit_form(&$form, &$form_state) {
  // The current configuration
  
}

function wallystat_showstats48h_content_type_edit_form_submit(&$form, &$form_state) {
  // For each part of the form defined in the 'defaults' array set when you
  // defined the content type, copy the value from the form into the array
  // of items to be saved. We don't ever want to use
  // $form_state['conf'] = $form_state['values'] because values contains
  // buttons, form id and other items we don't want stored. CTools will handle
  // the actual form submission.
  
}

/**
* Output function for the 'shortcut' content type.
*/
function wallystat_showstats48h_content_type_render($subtype, $conf, $panel_args, $context,$a) {

  $block->title = t('Number of visists during the last 48 hours');
  $block->content = theme('wallystat_show_stats_48h', $context->data->nid);
  return $block;
}

