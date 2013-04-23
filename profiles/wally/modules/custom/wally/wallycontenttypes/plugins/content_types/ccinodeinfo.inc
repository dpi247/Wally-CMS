<?php

/**
 * Callback function to supply a list of content types.
 */
function wallycontenttypes_ccinodeinfo_ctools_content_types() {
  ctools_include('context');
  return array(
    'single' => TRUE,
    'title' => t('[node] CCI Info'),
    'category' => t('Dashboard'),
    'description' => t('CCI Info'),
    'required context' => new ctools_context_required(t('Node'), 'node'),
    'defaults' => array(),
  );
}

function wallycontenttypes_ccinodeinfo_content_type_edit_form(&$form, &$form_state) {
  // The current configuration
 
}

function wallycontentypes_ccinodeinfo_content_type_edit_form_submit(&$form, &$form_state) {
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
function wallycontenttypes_ccinodeinfo_content_type_render($subtype, $conf, $panel_args, $context,$a) {

  $node = $context->data;
  $block->title = t('CCI infos');
  $block->content .= '<h3>'.t('Fields').'</h3>';
  $header = array(t('Field'), t('Value'));
  $rows = array(
    array(t('External Reference'), $node->field_externalreference[0]['value']),
    array(t('Product ID'), $node->field_productid[0]['value']),
    array(t('Publication date'), $node->field_publicationdate[0]['value']),
    array(t('Embargo Date'), $node->field_embargodatetime[0]['value']),
    array(t('Unpublish Date'), $node->field_unpublishdate[0]['value']),
  );
  $block->content .= theme('table', $header, $rows);
  $block->content .= '<h3>'.t('Destinations').'</h3>';
  $header = array(t('Term'), t('Target'), t('Layout'), t('Rank'), t('Time Rank'));
  $rows = array();
 
  foreach ($node->field_destinations as $dest){
    $term = taxonomy_get_term($dest['tid']);
    $rows[] = array($term->name, $dest['target'], $dest['layout'], $dest['rank'], $dest['time_rank']);
  }
  $block->content .= theme('table', $header, $rows);
  return $block;
}

