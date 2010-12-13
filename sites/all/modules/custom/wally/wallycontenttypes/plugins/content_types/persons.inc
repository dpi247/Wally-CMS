<?php
/*
  Plugin declaration function - returns a plugin definition array that
 * describes the content type.
 */
 
function wallycontenttypes_persons_ctools_content_types() {
    return array(
      'single' => TRUE,
      'title' => t('Package related Persons'),
      'icon' => 'icon_node.png',
      'description' => t('Fetch all persons related to package node.'),
      'required context' => new ctools_context_required(t('Node'), 'node'),
      'category' => t('Packages'),
   'defaults' => array(),
    );
}

function wallycontenttypes_persons_content_type_edit_form(&$form, &$form_state) {
}

function wallycontenttypes_persons_content_type_render($subtype, $conf, $panel_args, $context) {

  $node = isset($context->data) ? drupal_clone($context->data) : NULL;

  $block = new stdClass();
  $block->module = 'wallycontenttypes';
  $block->delta  = $node->nid;

  
  if (empty($node)) {
    $block->title = "Error.";
    $block->content = t('wallycontenttype: A context is required.<br/> Please specify a node context in the settings of this pane');
  }
  else {
    // Load associated Wally_peron nodes.  
    $node->field_persons_nodes = wallycontenttypes_field_tidload("field_persons", $node->field_persons, "wally_persontype");

    $block->title = "Person List.";
    $block->content = theme("wallyct_personslist_detail",$node->field_persons_nodes, $node);
  }

  return $block;
}

