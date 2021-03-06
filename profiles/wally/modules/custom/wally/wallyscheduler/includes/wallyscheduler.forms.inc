<?php

function wallyscheduler_form_createsnapshot_form($form){
  $form = array();
  $format = 'Y-m-d H:i';
  
  $timezone = date_default_timezone_name(FALSE);
  date_default_timezone_set($timezone);
  $date = date($format, time() + 60*15);
  
  $form['timestamp_of_execution'] = array(
  		'#type' => 'date_select',
  		'#title' => t('Execution time'),
  		'#required' => TRUE,
  		'#default_value' => $date,
  		'#date_format' => $format,
  		'#date_label_position' => 'within', // See other available attributes and what they do in date_api_elements.inc
  		'#date_timezone' => 'America/Chicago', // Optional, if your date has a timezone other than the site timezone.
  		'#date_increment' => 15,
  );
  
  $form['machine_name'] = array(
  		'#type' => 'textfield',
  		'#title' => t('machine name'),
  		'#required' => false,
  		'#size' => 60,
  		'#maxlength' => 254,
  );
  
  $form['destination_table'] = array(
  		'#type' => 'textfield',
  		'#title' => t('Table'),
  		'#size' => 60,
  		'#maxlength' => 254,
  );
  
  $form['submit'] = array(
  		'#type' => 'submit',
  		'#value' => t('Create Snapshot'),
  );
  
  $form['#redirect']="admin/wally/wallyscheduler/dashboard";
  
  return $form;

}
function wallyscheduler_form_createsnapshot_form_validate($form,$form_state){
	module_load_include('inc', 'wallyscheduler', 'includes/wallyscheduler.helpers');
	
	//@todo: check that the created snapshot isn't too close of an existing snapshot
	//avoid too many switch (min 15 min interval)
	$snapshot = new stdClass();
	
	$timezone = date_default_timezone_name(FALSE);
	date_default_timezone_set($timezone);
	$snapshot->timestamp_of_execution = strtotime($form_state['values']['timestamp_of_execution']);
	$pseudo_machine_name = str_replace(' ','_',  $snapshot->timestamp_of_execution);
	$pseudo_machine_name = str_replace(':','-', $pseudo_machine_name);
	$pseudo_machine_name = "wallyscheduler_".$pseudo_machine_name;
	foreach (array('machine_name','destination_table') as $key){
		if ($form_state['values']['$key'] == ""){
			$snapshot->{$key} = $pseudo_machine_name;
		}
	}
	if (wallyscheduler_table_exist($snapshot->destination_table)){
		form_set_error("timestamp_of_execution",'Un snaphot est déjà défini à cet instant. veuilez soit supprimer le timestamp existant, soit l\' overwriter ...');
	}
	
}
function wallyscheduler_form_createsnapshot_form_submit($form, &$form_state){
	module_load_include('inc', 'wallyscheduler', 'includes/wallyscheduler.helpers');
	global $user;
	
	$snapshot = new stdClass();
	
	$timezone = date_default_timezone_name(FALSE);
	date_default_timezone_set($timezone);
	$snapshot->timestamp_of_execution = strtotime($form_state['values']['timestamp_of_execution']);
	
	$pseudo_machine_name = str_replace(' ','_',  $snapshot->timestamp_of_execution);
	$pseudo_machine_name = str_replace(':','-', $pseudo_machine_name);
	$pseudo_machine_name = "wallyscheduler_".$pseudo_machine_name;
	foreach (array('machine_name','destination_table') as $key){
		if ($form_state['values']['$key'] == ""){
			$snapshot->{$key} = $pseudo_machine_name;
		}
	}
	
	$snapshot->uid = $user->uid;
	$snapshot->created = time();
	$snapshot->updated = time();
	
	//save snapshot
	if (wallyscheduler_snapshot_create($snapshot)){
		drupal_set_message(t("The snapshot has been created"));
	} else {
		drupal_set_message(t("The snapshot has not been created"),'warning');
	}
	
}


function wallyscheduler_form_overwritesnapshot_form($form, &$form_state,$b){
	module_load_include('inc', 'wallyscheduler', 'includes/wallyscheduler.helpers');
	
	$form = array();
	
	$snapshot_id = arg(4);
	$snapshot = wallyscheduler_get_snapshot($snapshot_id);
	if (!isset($snapshot->sid)){
		drupal_not_found();
	}
	$form['markup'] = array(
			'#type' => 'markup',
			'#value' => t('Are you sure you want to override this snaphot with the current working copy ?'),
			'#prefix'=>'<div>',
			'#suffix'=>'</div>',
	);
	$form['sid'] = array(
			'#type' => 'value',
			'#value' => $snapshot->sid,
	);
	$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Yes ! Overwrite the Snapshot'),
	);
    $form['#redirect']="admin/wally/wallyscheduler/dashboard";
	return $form;

}


function wallyscheduler_form_overwritesnapshot_form_submit($form, &$form_state,$b){
	module_load_include('inc', 'wallyscheduler', 'includes/wallyscheduler.helpers');

	$form = array();

	$snapshot_id = $form_state['values']['sid'];
	$snapshot = wallyscheduler_get_snapshot($snapshot_id);
	
	_wallyscheduler_process_overwrite($snapshot_id);
	drupal_set_message(t("The snapshot has been overrwrited."));

}




function wallyscheduler_form_deletesnapshot_form($form, &$form_state,$b){
	module_load_include('inc', 'wallyscheduler', 'includes/wallyscheduler.helpers');

	$form = array();

	$snapshot_id = arg(4);
	$snapshot = wallyscheduler_get_snapshot($snapshot_id);
	if (!isset($snapshot->sid)){
		drupal_not_found();
	}
	$form['markup'] = array(
			'#type' => 'markup',
			'#value' => t('Are you sure you want to delete this snaphot ?'),
			'#prefix'=>'<div>',
			'#suffix'=>'</div>',
	);
	$form['sid'] = array(
			'#type' => 'value',
			'#value' => $snapshot->sid,
	);
	$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('Yes ! Delete the Snapshot'),
	);
	$form['#redirect']="admin/wally/wallyscheduler/dashboard";
	return $form;

}


function wallyscheduler_form_deletesnapshot_form_submit($form, &$form_state,$b){
	module_load_include('inc', 'wallyscheduler', 'includes/wallyscheduler.helpers');

	$form = array();

	$snapshot_id = $form_state['values']['sid'];
	$snapshot = wallyscheduler_get_snapshot($snapshot_id);

	_wallyscheduler_process_delete($snapshot_id);
	drupal_set_message(t("The snapshot has been deleted."));

}

function wallyscheduler_form_config_form($form, &$form_state){

  $form = array();

  $form['production_site_url'] = array(
	'#type'          => 'textfield',
	'#title'         => t('Production site\'s url'),
	'#default_value' => wally_variable_get('wallyscheduler_production_site_url',''),
  );
  
  $options = array();
  for ($i = 1; $i < 60; $i++){
    $options[$i] = $i;
  }
  $form['snapshot_expire_time'] = array(
    '#type'          => 'select',
    '#title'         => t('Time to live of an old snapshot (in day)'),
    '#description'   => t('Choose the number of day before an old snapshot is destroy'),
    '#options'       => $options,
    '#default_value' => wally_variable_get('wallyscheduler_snaphot_expire_time', 30),
  );
  
  $form['submit'] = array(
	'#type' => 'submit',
	'#value' => t('Save'),		
  );
  
  $form['#redirect'] = "admin/wally/wallyscheduler/dashboard";
	
  return $form;

}


function wallyscheduler_form_config_form_submit($form, &$form_state){
	
  //Set the production url
  $production_site_url = $form_state['values']['production_site_url'];
  wally_variable_set('wallyscheduler_production_site_url', $production_site_url);
	
  //Set the snapshot expire time
  $snapshot_expire_time = $form_state['values']['snapshot_expire_time'];
  wally_variable_set('wallyscheduler_snaphot_expire_time', $snapshot_expire_time);
}
