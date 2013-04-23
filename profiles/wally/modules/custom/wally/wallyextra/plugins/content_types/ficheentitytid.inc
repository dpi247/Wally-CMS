<?php
/*
  Plugin declaration function - returns a plugin definition array that
 * describes the content type.
 */
function wallyextra_ficheentitytid_ctools_content_types() {
    return array(
      'single' => TRUE,
      'title' => t('Tid related Entity'),
      'icon' => 'icon_node.png',
      'description' => t('Fetch all entities related to a entity taxonomy term id.'),
    	// @todo: contextualiser sur une page de taxonomie
      'all contexts' => TRUE,
      'category' => t('Wally Extra'),
   'defaults' => array(),
    );
}

function wallyextra_ficheentitytid_content_type_edit_form(&$form, &$form_state) {
}

function wallyextra_ficheentitytid_content_type_render($subtype, $conf, $panel_args, $context) {

	$data = $context["argument_terms_1"]->data; 
	
	$block = new stdClass();
  $block->module = 'wallyextra';
  $block->delta  = $data->tid;
	$tid = $data->tid; 
  $nodes = wallycontenttypes_fieldTIDload("", array("1"=>array("value"=>$tid)), "wally_entitytype");
  if (empty($nodes)) {
    $block->title = "";
    $block->content = "";
  } else {
    // Load associated Wally_peron nodes.  
    $block->title = "Entity List.";
    $block->content = theme("wallyct_entitylist_detail",$nodes, NULL);
  }
  return $block;
}

