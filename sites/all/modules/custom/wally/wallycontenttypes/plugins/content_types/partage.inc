<?php
/*
  Plugin declaration function - returns a plugin definition array that
 * describes the content type.
 */
 
function wallycontenttypes_partage_ctools_content_types() {
    return array(
      'single' => TRUE,
      'title' => t('Partage to face book and all'),
      'icon' => 'icon_node.png',
      'description' => t('Partage to facebook and all.'),
      'required context' => new ctools_context_required(t('Node'), 'node'),
      'category' => t('Packages'),
   'defaults' => array(),
    );
}

function wallycontenttypes_partage_content_type_edit_form(&$form, &$form_state) {
}

function wallycontenttypes_partage_content_type_edit_form_submit(&$form, &$form_state) {
}

function wallycontenttypes_partage_content_type_render($subtype, $conf, $panel_args, $context) {
   
  $node = isset($context->data) ? drupal_clone($context->data) : NULL;

  $block = new stdClass();
  $block->module = 'wallycontenttypes';
  $block->delta  = $node->nid;
  
  if (empty($node)) {
    $block->title = "Error.";
    $block->content = t('wallycontenttype: A context is required.<br/> Please specify a node context in the settings of this pane');
  }
  else {

    $block->title = "Paratager vers facebook et autres";
    $block->content = theme("wallyct_tofacebook",$node, $conf); 
  }
  return $block;
}
