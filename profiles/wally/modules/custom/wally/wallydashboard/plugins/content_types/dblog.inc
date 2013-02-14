<?php 
/**
 * Callback function to display dblog in the dashboard.
 */
function wallydashboard_dblog_ctools_content_types() {

	return array(
			'single' => TRUE,
			'title' => t('Db log'),
			'category' => t('Dashboard'),
			'description' => t('Display Db log.'),
			'defaults' => array(),
	);
}
/**
 * Implementation of content_type_form()
 */
function wallydashboard_dblog_content_type_edit_form(&$form, &$form_state) {
	// select the numbers of logs lines
	$options = array();
	$defaultValue = 5;
	
	for($i=1;$i<=20;$i++)
	{
		$options[$i]=$i;
	}

	if($form_state['conf']['dblog_nbrLine']!=NULL)
	{
		$defaultValue = $form_state['conf']['dblog_nbrLine'];
	}
	$form['dblog'] = array(
				'#type' => 'select',
				'#title' => t('Choose the numbers of lines'),
				'#options' => $options,
				'#default_value' => $defaultValue,
	);
	

}
/**
 * Implementation of content_type_form_submit()
 */
function wallydashboard_dblog_content_type_edit_form_submit(&$form, &$form_state) {
	
    $form_state['conf']['dblog_nbrLine'] = $form_state['values']['dblog'];
}

/**
 * Implementation of content_type_render()
 */
function wallydashboard_dblog_content_type_render($subtype, $conf, $panel_args, $context) {
	
	//call dblog module to use _dblog_format_message() and dblog_build_filter_query()
	module_load_include('inc', 'dblog','dblog.admin');
	
	$nbrLine = $conf['dblog_nbrLine'];
	$block = new stdClass();
	$filter = dblog_build_filter_query();
	$rows = array();
	
	$icons = array(
			WATCHDOG_DEBUG    => '',
			WATCHDOG_INFO     => '',
			WATCHDOG_NOTICE   => '',
			WATCHDOG_WARNING  => theme('image', 'misc/watchdog-warning.png', t('warning'), t('warning')),
			WATCHDOG_ERROR    => theme('image', 'misc/watchdog-error.png', t('error'), t('error')),
			WATCHDOG_CRITICAL => theme('image', 'misc/watchdog-error.png', t('critical'), t('critical')),
			WATCHDOG_ALERT    => theme('image', 'misc/watchdog-error.png', t('alert'), t('alert')),
			WATCHDOG_EMERG    => theme('image', 'misc/watchdog-error.png', t('emergency'), t('emergency')),
	);
	$classes = array(
			WATCHDOG_DEBUG    => 'dblog-debug',
			WATCHDOG_INFO     => 'dblog-info',
			WATCHDOG_NOTICE   => 'dblog-notice',
			WATCHDOG_WARNING  => 'dblog-warning',
			WATCHDOG_ERROR    => 'dblog-error',
			WATCHDOG_CRITICAL => 'dblog-critical',
			WATCHDOG_ALERT    => 'dblog-alert',
			WATCHDOG_EMERG    => 'dblog-emerg',
	);
	
	$output = drupal_get_form('dblog_filter_form');
	
	$header = array(
			' ',
			array('data' => t('Type'), 'field' => 'w.type'),
			array('data' => t('Date'), 'field' => 'w.wid', 'sort' => 'desc'),
			t('Message'),
			array('data' => t('User'), 'field' => 'u.name'),
			array('data' => t('Operations')),
	);
	
	$sql = "SELECT w.wid, w.uid, w.severity, w.type, w.timestamp, w.message, w.variables,
			 w.link, u.name FROM {watchdog} w 
			INNER JOIN {users} u ON w.uid = u.uid";
	
	$tablesort = tablesort_sql($header);
	
	if (!empty($filter['where'])) {
		//filter
		$result = pager_query($sql ." WHERE ". $filter['where'] . $tablesort, $nbrLine, 0, NULL, $filter['args']);
	}
	else {
		//no filter
		$result = pager_query($sql . $tablesort, $nbrLine);
	}
	
	while ($dblog = db_fetch_object($result)) {
		$rows[] = array('data' =>
				array(
						// Cells
						$icons[$dblog->severity],
						t($dblog->type),
						format_date($dblog->timestamp, 'small'),
						l(truncate_utf8(_dblog_format_message($dblog), 56, TRUE, TRUE), 'admin/reports/event/'. $dblog->wid, array('html' => TRUE)),
						theme('username', $dblog),
						filter_xss($dblog->link),
				),
				// Attributes for tr
				'class' => "dblog-". preg_replace('/[^a-z]/i', '-', $dblog->type) .' '. $classes[$dblog->severity]
		);
	}
	
	if (!$rows) {
		$rows[] = array(array('data' => t('No log messages available.'), 'colspan' => 6));
	}
	
	$output .= theme('table', $header, $rows, array('id' => 'admin-dblog'));
	$output .= theme('pager', NULL, $nbrLine, 0);

	$block->title=t('db log');
	$block->content=$output;
	return $block;
}

