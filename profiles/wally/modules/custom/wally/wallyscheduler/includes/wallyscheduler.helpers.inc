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
function _wallyscheduler_switch_check(){
	
	
}



/*
 * Replace destination table with values from the values contained in the table  reffered by the snapshot
 *
 */

function _wallyscheduler_clean_process_switch($snapshot_id){

	$snapshot=wallyscheduler_get_snapshot($snapshot_id);
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
	$type_name='wally_articlepackage';
	module_load_include('inc', 'content', 'includes/content.crud');
	$content_fields = content_field_instance_read(array('type_name' => $type_name));
	$field=$content_fields['12'];
	$schema=content_database_info($field);
    
    //adding special cck fields
    $schema['fields']["nid"]=$schema['columns']['tid'];
	$schema['fields']["vid"]=$schema['columns']['tid'];
    $schema['fields']["delta"]=$schema['columns']['tid'];

    $schema['fields']["tid"]=$schema['columns']['tid'];
    $schema['fields']["target"]=$schema['columns']['target'];
    $schema['fields']["layout"]=$schema['columns']['layout'];
    $schema['fields']["rank"]=$schema['columns']['rank'];
    $schema['fields']["time_rank"]=$schema['columns']['time_rank'];
    
    
    return $schema;
}


function _wallyscheduler_process_switch($snapshot_id){

	$snapshot=wallyscheduler_get_snapshot($snapshot_id);
	
	//put site offline
	variable_set("site_offline",TRUE);
	
	//call hook_wallyscheduler_snapshot_switch_before
    module_invoke_all('wallyscheduler_process_switch_before',$snapshot);
	
    
    //make the switch
    db_query("TRUNCATE TABLE {wallyscheduler_destinations} ;");
    db_query("INSERT {wallyscheduler_destinations} SELECT * FROM {". $snapshot->destination_table."};");
    
	
	//call hook_wallyscheduler_snapshot_switch_after
    module_invoke_all('wallyscheduler_process_switch_after',$snapshot);
    
    
    //@todo: cache_clear of remote site ???? how to ??? (MD5 callback+ secret passphrase on settings.php ?)
    
    //Put the table id in the variable that define the current prodution snaphsot
    wally_variable_set('wallyscheduler_production',$snapshot->sid);
	

    //put site online
    variable_set("site_offline",FALSE);
    

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
	}
	else{
		watchdog('wallyscheduler', t("Failed to create !machine_name at !time",array('!machine_name'=>$snapshot_object->machine_name,'!time'=>date("Y-m-d h:i",$snapshot_object->time_of_execution))));
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


function wallyscheduler_get_snapshot($snapshot_id){
	$result=db_query("SELECT * FROM {wallyscheduler_snapshot_list} WHERE sid=%d ;",array($snapshot_id));
	$snapshot=db_fetch_object($result);
	return $snapshot;
	
	
}