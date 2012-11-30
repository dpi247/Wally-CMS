<?php


/*
Un truc comme ca devrait permetre de r�utiliser le schema ... pour cr�er des tables snapshot ;)
  function drupal_install_schema($module) {
  $schema = drupal_get_schema_unprocessed($module);
  _drupal_initialize_schema($module, $schema);

  $ret = array();
  foreach ($schema as $name => $table) {
    db_create_table($ret, $name, $table);
  }
  return $ret;
}
*/

/*
 * Check in the if it is time to switch the destination table
 */
function _wallyscheduler_process_switchcheck(){
	
	
  $snapshot_id = db_result(db_query("SELECT sid
			FROM {wallyscheduler_snapshot_list}
			WHERE timestamp_of_execution < %d
			AND timestamp_of_execution IN (select max(timestamp_of_execution) 
									       FROM {wallyscheduler_snapshot_list} 
										   WHERE timestamp_of_execution < %d )", 
	        array(time(), time())));
	
	
  //if the snapshot id is not the one in the stored variable, we make the switch
  if (wally_variable_get('wallyscheduler_production',NULL) != $snapshot_id){
    _wallyscheduler_process_switch($snapshot_id);
  }
}

/*
 * Replace destination table with values from the values contained in the table  reffered by the snapshot
 *
 */

function _wallyscheduler_clean_process_switch($snapshot_id){

	$snapshot = wallyscheduler_get_snapshot($snapshot_id);
    //make site offline
	
	//invoke_hook_wallyscheduler_switch_before
	
	//make switch
	
	//invoke_hook_wallyscheduler_switch_after
	
	//make site online
	
}

/*
 * Clean old snaphot table tables and their relatice entries in the wallyscheduler_snapshot_list table
 *
 */
function _wallyscheduler_clean_old_snapshot(){
	
	
	//@todo: Dont delete the snapshot used in production ! 
	
}


/*
 * Get the default schema of field destination used by cck module
 *
*/

function wallyscheduler_get_destinations_original_schema(){
	$type_name = 'wally_articlepackage';
	module_load_include('inc', 'content', 'includes/content.crud');
	$content_fields = content_field_instance_read(array('type_name' => $type_name));
	$field = $content_fields['12'];
	$schema = content_database_info($field);
    
    //adding special cck fields
    $schema['fields']["nid"] = $schema['columns']['tid'];
	$schema['fields']["vid"] = $schema['columns']['tid'];
    $schema['fields']["delta"] = $schema['columns']['tid'];

    $schema['fields']["field_destinations_tid"] = $schema['columns']['tid'];
    $schema['fields']["field_destinations_target"] = $schema['columns']['target'];
    $schema['fields']["field_destinations_layout"] = $schema['columns']['layout'];
    $schema['fields']["field_destinations_rank"] = $schema['columns']['rank'];
    $schema['fields']["field_destinations_time_rank"] = $schema['columns']['time_rank'];
     
    return $schema;
}


function _wallyscheduler_process_switch($snapshot_id){

	$snapshot=wallyscheduler_get_snapshot($snapshot_id);
	
	//put site offline
	variable_set("site_offline",TRUE);
	
	//call hook_wallyscheduler_snapshot_switch_before
    module_invoke_all('wallyscheduler_process_switch_before',$snapshot);
	
    
    //make the switch
    db_query("TRUNCATE TABLE {wallyscheduler_content_field_destinations} ;");
    db_query("INSERT {wallyscheduler_content_field_destinations} SELECT * FROM {". $snapshot->destination_table."};");
    
	
	//call hook_wallyscheduler_snapshot_switch_after
    module_invoke_all('wallyscheduler_process_switch_after',$snapshot);
    
    watchdog('wallyscheduler',t('Switch: prodution now reflect : !machine_name',array('!machine_name'=>$snapshot->machine_name)));
    
    // cache_clear of remote site 
    wallyscheduler_remote_cache_clear();
    
    //Put the table id in the variable that define the current prodution snaphsot
    wally_variable_set('wallyscheduler_production',$snapshot->sid);
	

    //put site online
    variable_set("site_offline",FALSE);
    

}
function wallyscheduler_remote_cache_clear(){
	
	$challenge = time();
	$secret = wally_variable_get('wallytoolbox_remotecache_secret',NULL);
	$production_site_url = wally_variable_get('wallyscheduler_production_site_url','');
	
	$url = 'http://'.$production_site_url.'/wallytoolbox/remote_cacheclear/'.$challenge."/".md5($challenge.$secret);
	$call = drupal_http_request($url);
	if ($call->code == '200'){
	  watchdog('wallyscheduler','Remotly cache clear of production site' );
	  $return = TRUE;
	} else {
	  watchdog('wallyscheduler','Remotly cache clear of production site FAILED', array(), WATCHDOG_WARNING );
	  $return = FALSE;
	}
	return $return;
}
function _wallyscheduler_process_overwrite($snapshot_id){

	$snapshot=wallyscheduler_get_snapshot($snapshot_id);


	//call hook_wallyscheduler_snapshot_switch_before
	module_invoke_all('wallyscheduler_process_overwrite_before',$snapshot);


	//make the switch
	db_query("TRUNCATE TABLE {".$snapshot->destination_table."} ;");
	db_query("INSERT {". $snapshot->destination_table."} SELECT * FROM {wallyscheduler_content_field_destinations};");
	watchdog('wallyscheduler', t("Overrwrite !machine_name at !time with working copy",array('!machine_name'=>$snapshot->machine_name,'!time'=>date("Y-m-d h:i",$snapshot->time_of_execution))));


	//call hook_wallyscheduler_snapshot_switch_after
	module_invoke_all('wallyscheduler_process_overwrite_after',$snapshot);



}


function _wallyscheduler_process_delete($snapshot_id){

	$snapshot=wallyscheduler_get_snapshot($snapshot_id);

	//call hook_wallyscheduler_snapshot_switch_before
	module_invoke_all('wallyscheduler_process_delete_before',$snapshot);

	//make the switch
	db_query("DROP TABLE {".$snapshot->destination_table."} ;");
	db_query("DELETE FROM  {wallyscheduler_snapshot_list}  WHERE sid = %d",$snapshot->sid);
	watchdog('wallyscheduler', t("Delete snapshot !machine_name ",array('!machine_name'=>$snapshot->machine_name,'!time'=>date("Y-m-d h:i",$snapshot->time_of_execution))));

	//call hook_wallyscheduler_snapshot_switch_after
	module_invoke_all('wallyscheduler_process_delete_after',$snapshot);



}

function _wallyscheduler_process_rollback($snapshot_id){

	$snapshot=wallyscheduler_get_snapshot($snapshot_id);

	//desactive l'import

	module_invoke_all('wallyscheduler_process_rollback_before',$snapshot);
	
	//setter la variable de rollback
	wally_variable_get('wallyscheduler_workingcopy',$snapshot->sid);

	module_invoke_all('wallyscheduler_process_rollback_after',$snapshot);
	
	
	//sauvegarde de la table actuelle des destination dans la table de roll back

	//import de la table du snapshot dans la table de destination


}

function _wallyscheduler_process_rollbackrevert($snapshot_id){

	$snapthod_id=wally_variable_get('wallyscheduler_workingcopy',NULL);
	
	$snapshot= wallyscheduler_get_snapshot($snapshot_id);

	module_invoke_all('wallyscheduler_process_rollbackrevert_after',$snapshot);
	
	//setter la variable de rollback
	wally_variable_get('wallyscheduler_workingcopy',$snapshot->machine_name);

	module_invoke_all('wallyscheduler_process_rollbackrevert_after',$snapshot);
	
	//sauvegarde de la table actuelle des destination dans la table originale ($snapshot->table)

	//import de la table dr rollback dans la table de destination

	//réactiver l'import
	
}


/*
 * save a snapshot object in DB
*
*/

function wallyscheduler_snapshot_create(&$snapshot_object){
	module_invoke_all('wallyscheduler_snapshot_create_before',$snapshot_object);
	wallyscheduler_snapshot_save(& $snapshot_object);
	module_invoke_all('wallyscheduler_snapshot_create_after',$snapshot_object);
	if($snapshot_object->sid){
      watchdog('wallyscheduler', t("Working copy saved as !machine_name at !time",array('!machine_name'=>$snapshot_object->machine_name,'!time'=>date("Y-m-d h:i",$snapshot_object->time_of_execution))));
      return TRUE;
	}
	else{
		watchdog('wallyscheduler', t("Failed to create !machine_name at !time",array('!machine_name'=>$snapshot_object->machine_name,'!time'=>date("Y-m-d h:i",$snapshot_object->time_of_execution))));
	  return FALSE;
	}
}
/*
 * save a snapshot object in DB
*
*/

function wallyscheduler_snapshot_save(&$snapshot_object){
  drupal_write_record("wallyscheduler_snapshot_list", $snapshot_object);
  
  db_query("CREATE TABLE {". $snapshot_object->destination_table."} LIKE {content_field_destinations};");
  db_query("INSERT {". $snapshot_object->destination_table."} SELECT * FROM {content_field_destinations};");

}
function wallyscheduler_table_exist($destination_table){
	if(db_result(db_query("SHOW TABLES LIKE '".$destination_table."'"))){
		return TRUE;
	}
	return FALSE;
}

function wallyscheduler_get_snapshot($snapshot_id){
	$result=db_query("SELECT * FROM {wallyscheduler_snapshot_list} WHERE sid=%d ;",array($snapshot_id));
	$snapshot=db_fetch_object($result);
	return $snapshot;
	
	
}