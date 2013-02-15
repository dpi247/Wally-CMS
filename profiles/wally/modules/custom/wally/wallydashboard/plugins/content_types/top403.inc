<?php 
/**
 * Callback function to supply a list of content types.
 */
function wallydashboard_top403_ctools_content_types() {

	return array(
			'single' => TRUE,
			'title' => t('Top 403'),
			'category' => t('Dashboard'),
			'description' => t('Display the top 403 access denied.'),
			'defaults' => array(),
	);
}
/**
 * Implementation of content_type_form()
 */
function wallydashboard_top403_content_type_edit_form(&$form, &$form_state) {
	// select the numbers of top 403 logs lines
	$options = array();
	$defaultValue = 5;

	for($i=1;$i<=10;$i++)
	{
		$options[$i]=$i;
	}

	if($form_state['conf']['top403_nbrLine']!=NULL)
	{
		$defaultValue = $form_state['conf']['top403_nbrLine'];
	}
	$form['top403'] = array(
			'#type' => 'select',
			'#title' => t('Choose the numbers of lines displayed'),
			'#options' => $options,
			'#default_value' => $defaultValue,
	);

}
/**
 * Implementation of content_type_form_submit()
 */
function wallydashboard_top403_content_type_edit_form_submit(&$form, &$form_state) {

	$form_state['conf']['top403_nbrLine'] =  $form_state['values']['top403'];

}

/**
 * Implementation of content_type_render()
 */
function wallydashboard_top403_content_type_render($subtype, $conf, $panel_args, $context) {

	module_load_include('inc', 'dblog','dblog.admin');
	$nbrLinesTop403 = $conf['top403_nbrLine']; //numbers of lines displayed
	$block = new stdClass();
	$rows = array();
	$header = array('Date','Message','User','Number of request');

	$sql = "SELECT w.wid, w.uid, w.timestamp, w.message, w.variables,
			u.name, COUNT(w.message) AS nbr
			FROM {watchdog} w
			INNER JOIN {users} u ON w.uid = u.uid
			WHERE w.type = 'access denied' GROUP BY w.message ORDER BY nbr DESC LIMIT %d";

	$result = db_query($sql,$nbrLinesTop403);

	$i=0;

	while ($top403 = db_fetch_object($result)) {
		$rows[$i] = array(
				format_date($top403->timestamp, 'small'),
				//use _dblog_format_message() from dblog module file : dblog.admin.inc
				l(truncate_utf8(_dblog_format_message($top403), 56, TRUE, TRUE), 'admin/reports/event/'. $top403->wid, array('html' => TRUE)),
				theme('username', $top403), //link for username
				$top403->nbr,
		);
		$i++;
	}

	//if no logs 403
	if (!$rows) {

		$rows[] = array(array('data' => t('No logs 403 messages available.'), 'colspan' => 6));

	}

	$output = theme('table', $header, $rows);

	$block->title=t('Top 403 access denied');
	$block->content=$output;
	return $block;
}

