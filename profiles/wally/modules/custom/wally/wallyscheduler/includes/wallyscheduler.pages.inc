<?php
function wallyscheduler_page_dashboard(){
	$variables=array();
	
	//build status
	$header_status=$rows_status=array();
	wallyscheduler_page_dashboard_get_status($header_status,$rows_status);
	$variables['header_status']=$header_status;
	$variables['rows_status']=$rows_status;
	
	
	//build Future table
	$header_future_snapshots=$rows_future_snapshots=array();
	wallyscheduler_page_dashboard_get_future_snaphots($header_future_snapshots,$rows_future_snapshots);
	$variables['header_future_snapshots']=$header_future_snapshots;
	$variables['rows_future_snapshots']=$rows_future_snapshots;
	
	
	//build history table
	$header_history=$rows_history=array();
	wallyscheduler_page_dashboard_get_history($header_history,$rows_history);
	$variables['header_history']=$header_future_snapshots;
	$variables['rows_history']=$rows_history;
	
	
	return theme("wallyscheduler_dashboard",$variables);
}
function wallyscheduler_page_dashboard_get_status(& $header,& $rows){
	module_load_include('inc', 'wallyscheduler', 'includes/wallyscheduler.helpers');
	
	$actions_list=array(
	  array('title'=>t('Create a snapshot.'),'href'=>"admin/wally/wallyscheduler/create_snapshot"),
	);
	$actions_working=theme('links',$actions_list);
	
	if(! $working_copy=wally_variable_get('wallyscheduler_workingcopy',NULL)){
		$working_copy='Live';
	}
	
	$rows[] = array('data' =>
			array(
					// Cells
					'Working copy',
					$working_copy,
					$actions_working,
			),
			// Attributes for tr
			'class' => "wallyscheduler-status wallyscheduler-status-workingcopy" 
	);
	
	
	
    if(! $working_copy=wally_variable_get('wallyscheduler_production',NULL)){
		$production='LIVE';
	}
	else{
		$production=wallyscheduler_get_snapshot($working_copy)->machine_name;
	}$rows[] = array('data' =>
			array(
					// Cells
					'Production',
					$production,
					'',
			),
			// Attributes for tr
			'class' => "wallyscheduler-status wallyscheduler-status-productioncopy" 
			);
	
}
function wallyscheduler_page_dashboard_get_future_snaphots(&$header,&$rows){
	$header = array(
			' ',
			array('data' => t('machine name'), 'field' => 's.machine_name'),
			array('data' => t('Table'), 'field' => 's.destination_table', 'sort' => 'desc'),
			array('data' => t('Time of execution'), 'field' => 's.timestamp_of_execution', 'sort' => 'desc'),
			array('data' => t('Name'), 'field' => 'u.name', 'sort' => 'desc'),
			array('data' => t('Operations')),
	);
	$now=time();
	$sql = "SELECT s.sid,s.machine_name, s.destination_table, s.timestamp_of_execution, u.name FROM {wallyscheduler_snapshot_list} s INNER JOIN {users} u ON s.uid = u.uid WHERE s.timestamp_of_execution >".$now;
	$tablesort = tablesort_sql($header);
	
	$result = pager_query($sql . $tablesort, 50);
	
	while ($snapshot = db_fetch_object($result)) {
		$row_count=db_query("SELECT count(sid) FROM {%s}",$snapshot->table);
		if($row_count>0){
			$row_class="wallyscheduler_row_not_empty";
		}
		else{
			$row_class="wallyscheduler_row_empty";
		}
		
		$actions_list=array(
		  array('title'=>t('Overwrite with working copy'),'href'=>'admin/wally/wallyscheduler/overwrite_with_working_copy/'.$snapshot->sid),
		  array('title'=>t('Delete'),'href'=>'admin/wally/wallyscheduler/delete_snapshot/'.$snapshot->sid),
		  //array('title'=>t('Rollback'),'href'=>'admin/wallyscheduler/rollback/'.$snapshot->sid),
		);
		$actions=theme('links',$actions_list);
	
		$rows[] = array('data' =>
				array(
						// Cells
						$snapshot->machine_name,
						$snapshot->table,
						format_date($snapshot->timestamp_of_execution, 'small'),
						theme('username', $snapshot),
						$actions,
				),
				// Attributes for tr
			'class' => "wallyscheduler-future " 
			);
	}
	
	if (!$rows) {
		$rows[] = array(array('data' => t('No snapshot availlable available.'), 'colspan' => 6));
	}
	
	
}

function wallyscheduler_page_dashboard_get_history(& $header,& $rows){
	module_load_include('inc', 'dblog', 'dblog.admin');
	
	$header = array(
			array('data' => t('Type'), 'field' => 'w.type'),
			array('data' => t('Date'), 'field' => 'w.wid', 'sort' => 'desc'),
			t('Message'),
			array('data' => t('User'), 'field' => 'u.name'),
			array('data' => t('Operations')),
	);
	
	$sql = "SELECT w.wid, w.uid, w.severity, w.type, w.timestamp, w.message, w.variables, w.link, u.name FROM {watchdog} w INNER JOIN {users} u ON w.uid = u.uid WHERE w.type=\"wallyscheduler\"";
	$tablesort = tablesort_sql($header);
	$result = pager_query($sql . $tablesort, 50);
	
	while ($dblog = db_fetch_object($result)) {
		
		$rows[] = array('data' =>
				array(
						// Cells
						t($dblog->type),
						format_date($dblog->timestamp, 'small'),
						l(truncate_utf8(_dblog_format_message($dblog), 56, TRUE, TRUE), 'admin/reports/event/'. $dblog->wid, array('html' => TRUE)),
						theme('username', $dblog),
						filter_xss($dblog->link),
				),
				// Attributes for tr
			'class' => "wallyscheduler-history " 
			);
	}
	
	if (!$rows) {
		$rows[] = array(array('data' => t('No log messages available.'), 'colspan' => 6));
	}
}


