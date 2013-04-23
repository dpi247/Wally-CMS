<?php 
/**
 * Callback function to supply a list of content types.
 */
function wallydashboard_top404_ctools_content_types() {

	return array(
			'single' => TRUE,
			'title' => t('Top 404'),
			'category' => t('Dashboard'),
			'description' => t('Display the top 404 page not found.'),
			'defaults' => array(),
	);
}
/**
 * Implementation of content_type_form()
 */
function wallydashboard_top404_content_type_edit_form(&$form, &$form_state) {
	// select the numbers of top 404 logs lines
	$options = array();
	$defaultValue = 5;
	
	for($i=1;$i<=10;$i++)
	{
		$options[$i]=$i;
	}
	
	if($form_state['conf']['top404_nbrLine']!=NULL)
	{
		$defaultValue = $form_state['conf']['top404_nbrLine'];
	}
	$form['top404'] = array(
				'#type' => 'select',
				'#title' => t('Choose the numbers of lines displayed'),
				'#options' => $options,
				'#default_value' => $defaultValue,
		);
	
}
/**
 * Implementation of content_type_form_submit()
 */
function wallydashboard_top404_content_type_edit_form_submit(&$form, &$form_state) {
	
	$form_state['conf']['top404_nbrLine'] =  $form_state['values']['top404'];
	
}
/**
 * Implementation of content_type_render()
 */
function wallydashboard_top404_content_type_render($subtype, $conf, $panel_args, $context) {
	
	module_load_include('inc', 'dblog','dblog.admin');
	$nbrLinesTop404 = $conf['top404_nbrLine']; //numbers of lines displayed
	$block = new stdClass();
	$rows = array();
	$header = array('Date','Message','User','Number of request');
	
	$sql = "SELECT w.wid, w.uid, w.timestamp, w.message, w.variables,
			u.name, COUNT(w.message) AS nbr 
			FROM {watchdog} w 
			INNER JOIN {users} u ON w.uid = u.uid
			WHERE w.type = 'page not found' GROUP BY w.message ORDER BY nbr DESC LIMIT %d";
	
	$result = db_query($sql,$nbrLinesTop404);
	
	$i=0;
	
	while ($top404 = db_fetch_object($result)) {
		$rows[$i] = array(
					format_date($top404->timestamp, 'small'),
					//use _dblog_format_message() from dblog module file : dblog.admin.inc
					l(truncate_utf8(_dblog_format_message($top404), 56, TRUE, TRUE), 'admin/reports/event/'. $top404->wid, array('html' => TRUE)),
					theme('username', $top404), //link for username
					$top404->nbr,
				);
		$i++;	
	}

	//if no logs 404
	if (!$rows) {
		
		$rows[] = array(array('data' => t('No logs 404 messages available.'), 'colspan' => 6));
	
	}
	
	$output = theme('table', $header, $rows);
	
	$block->title=t('Top 404 page not found');
	$block->content=$output;
	return $block;
}

