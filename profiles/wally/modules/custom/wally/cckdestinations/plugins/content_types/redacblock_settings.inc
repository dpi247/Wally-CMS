<?php 
/**
 * Callback function to supply a list of content types.
 */
function cckdestinations_redacblock_settings_ctools_content_types() {

	return array(
			'single' => TRUE,
			'title' => t('Redac Block Settings'),
			'category' => t('Dashboard'),
			'description' => t('Display settings of each redac block.'),
			'required context' => new ctools_context_required(t('Term'), array('taxonomy_term', 'term')),
			'defaults' => array(),
			'all contexts' => TRUE,			
	);
}

/**
 * Implementation of content_type_form()
 */
function cckdestinations_redacblock_settings_content_type_edit_form(&$form, &$form_state) {

}

/**
 * Implementation of content_type_form_submit()
 */
function cckdestinations_redacblock_settings_content_type_edit_form_submit(&$form, &$form_state) {

}

/**
 * Implementation of content_type_render()
 */
function cckdestinations_redacblock_settings_content_type_render($subtype, $conf, $panel_args, $context) {

	$dataContext = $context["argument_term_1"]->data; 
	$defaults = cckdestination_get_default_rank_settings();
	$tid = $dataContext->tid;
	
	$header = array(t('Name'),t('sort method'),t('behavior'),t('reserved position'),t('delta t'), t('Reorder'));
	$rows  = array();
	$i = 0;
	
	//get all destination and all redac block
	$hierarchies = wallyctools_get_hierarchies();

	//get all redac block for the current tid
	$redacBlocks = $hierarchies[$tid]['redacblocks'];
	
	//build a tab with redac block name
	foreach($redacBlocks as $cle => $valeur){
	  $redacBlocksName[$i] = $valeur['name'];
	  $i++;
	}

	for ($i = 0;$i < count($redacBlocksName);$i++){
	  //get settings of each redac block
	  $dest_settings[$i] = wally_variable_get('cckdestinations_destination_'.$tid.'_'.$redacBlocksName[$i], $defaults);
	  $delta_t = t('/');
	  if ($dest_settings[$i]['sort_method'] != 'absolute'){
		$delta_t = $dest_settings[$i]['delta_t'];
	  }
	  $rows[$i] = array(
		$redacBlocksName[$i],
		$dest_settings[$i]['sort_method'],
		$dest_settings[$i]['behavior'],
		$dest_settings[$i]['reserved'],
		$delta_t,
		'<a href="/admin/wally/wallyctools-operations/reorder/25/'.$tid.'/'.$redacBlocksName[$i].'">'.t('Reorder').'</a>',
	  );
	}
	
	//if rows is empty
	if (!$rows) {
	  $rows[] = array(array('data' => t('No informations available.'), 'colspan' => 6));
	}
	
	$output = theme('table', $header, $rows);
	
	$block = new stdClass();
	$block->title = t('Redac block settings');
	$block->content = $output;
	return $block;
}