<?php
/*
  Plugin declaration function - returns a plugin definition array that
 * describes the content type.
 */
 
function wallycontenttypes_taxonomylist_ctools_content_types() {
    return array(
      'single' => TRUE,
      'title' => t('Package related Taxonomy List (simple/tree)'),
      'icon' => 'icon_node.png',
      'description' => t('Fetch all taxonomy terms related to package node (List/Tree Mode).'),
      'required context' => new ctools_context_required(t('Node'), 'node'),
      'category' => t('Packages'),
   'defaults' => array(),
    );
}

function wallycontenttypes_taxonomylist_content_type_edit_form(&$form, &$form_state) {

  $conf = $form_state['conf'];
      
  $form['taxonomyfield'] = array (
    '#type'             => 'select',
    '#title'            => t('Taxonomy field to extract'),
    '#options'          => array(
                            "field_authors"=>t('Authors'),
                            "field_persons"=>t('Persons'),
                            "field_locations"=>t('Locations'),
                            "field_entities"=>t('Entities'),
                            "field_free_tags"=>t('Free Tags'),
                            "field_tags"=>t('Classified Tags'),
                          ),
    '#required'         => TRUE,
  );

  $form['taxonomystyle'] = array (
    '#type'             => 'select',
    '#title'            => t('Taxonomy Style'),
    '#options'          => array( 'simple'=>t('Simple List'), 'tree'=>t('Tree List') ),
    '#required'         => TRUE,
  );

  return $form;  
}

function wallycontenttypes_taxonomylist_content_type_edit_form_submit(&$form, &$form_state) {
  $form_state['conf']['taxonomyfield']=$form['taxonomyfield']['#value'];
  $form_state['conf']['taxonomystyle']=$form['taxonomystyle']['#value'];
}

function wallycontenttypes_taxonomylist_content_type_render($subtype, $conf, $panel_args, $context) {
   
  $node = isset($context->data) ? drupal_clone($context->data) : NULL;
  $block = new stdClass();
  
  if (empty($node)) {
    $block->title = "Error.";
    $block->content = t('wallycontenttype: A context is required.<br/> Please specify a node context in the settings of this pane');
  }
  else {

    $taxonomyfield = $conf['taxonomyfield'];
         
    $block->title = "Taxonomy Tag List.";
    if ($conf['taxonomystyle']=="tree") {
      $block->content = theme("wallyct_taxotermlist_tree",$node->$taxonomyfield, $node, $conf); 
    } else {
      $block->content = theme("wallyct_taxotermlist",$node->$taxonomyfield, $node, $conf); 
    } 

  }
  return $block;
}
