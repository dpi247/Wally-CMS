<?php

function wallyscheduler_form_createsnapshot_form($form, &$form_state){
  $form=array();
  $format = 'Y-m-d H:i';
  $date =date( $format,time()+60*15);
  $form['timestamp_of_execution'] = array(
  		'#type' => 'date_select',
  		'#title' => t('Executin time'),
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
  
  return $form;

}
function wallyscheduler_form_createsnapshot_form_validate($form_state){
	
	//@todo: check that the created snapshot isn't too close of an existing snapshot
	//avoid too many switch (min 15 min interval)
}
function wallyscheduler_form_createsnapshot_form_submit($form, &$form_state){
	module_load_include('inc', 'wallyscheduler', 'includes/wallyscheduler.helpers');
	global $user;
	
	$snapshot=new stdClass();
	
	$snapshot->timestamp_of_execution=strtotime($form_state['values']['timestamp_of_execution']);
	
	$pseudo_machine_name=str_replace(' ','_',  $snapshot->timestamp_of_execution);
	$pseudo_machine_name=str_replace(':','-', $pseudo_machine_name);
	$pseudo_machine_name="wallyscheduler_".$pseudo_machine_name;
	foreach(array('machine_name','destination_table') as $key){
		if($form_state['values']['$key']==""){
			$snapshot->{$key}=$pseudo_machine_name;
		}
	}
	
	$snapshot->uid=$user->uid;
	$snapshot->created=time();
	$snapshot->updated=time();
	
	//save snapshot
	wallyscheduler_snapshot_create($snapshot);
	
	
	
	
}