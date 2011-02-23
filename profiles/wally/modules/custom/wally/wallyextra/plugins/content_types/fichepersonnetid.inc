<?php
/*
  Plugin declaration function - returns a plugin definition array that
 * describes the content type.
 */
function wallycontenttypes_fichepersonnetid_ctools_content_types() {
    return array(
      'single' => TRUE,
      'title' => t('Tid related Persons'),
      'icon' => 'icon_node.png',
      'description' => t('Fetch all persons related to a person taxonomy term id.'),
    	// @todo: contextualiser sur une page de taxonomie
      'all contexts' => TRUE,
      'category' => t('Wally Extra'),
   'defaults' => array(),
    );
}

function wallycontenttypes_fichepersonnetid_content_type_edit_form(&$form, &$form_state) {
}

function wallycontenttypes_fichepersonnetid_content_type_render($subtype, $conf, $panel_args, $context) {

	$data = $context["argument_terms_1"]->data; 
	
	$block = new stdClass();
  $block->module = 'wallycontenttypes';
  $block->delta  = $data->tid;
	$tid = $data->tid; 
  $nodes = wallycontenttypes_fieldTIDload("", array("1"=>array("value"=>$tid)), "wally_persontype");
  
  if (empty($nodes)) {
    $block->title = "";
    $block->content = "";
  }
  else {
    // Load associated Wally_peron nodes.  
    $block->title = "Person List.";
    $block->content = theme("wallyct_personslist_detail",$nodes, NULL);
  }
  return $block;
}

