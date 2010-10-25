<?php

/*
 * 
 * 
 * 
 */
function _wimprthelp_array_to_ccklocation($location) {
	$node = array();

	// Get all country list from location module
	$countries = location_get_iso3166_list();

	// Is it an existant location - get the LID?
	if (isset($location["lid"])) {
		$node['lid'] = $location["lid"];
	} 

	if ($location["name"]) {
		$node['name'] = $location["name"];
	} else {
		// TO DO FATAL :: No location NAME - crital
		drupal_set_message('No location NAME found.','error');
		return null; 
	}
		
	// Street lines
	if (isset($location["street"])) $node['street'] = $location["street"];
	if (isset($location["additional"])) $node['additional'] = $location["additional"];

	// City
	$node['city'] = $location["city"];

	// Province 
	if (isset($location['province_name'])) {
		if ($node['province']=location_province_code($location['country_name'],$location['province_name'])) {
			$node['province_name'] = $location['province_name'];
		} else {
			drupal_set_message('Country PROVINCE not found for '.$location['country_name'].'/'.$location['province_name'].'.','error');
		}
	}

	// Postal Code
	$node['postal_code'] = $location["postal_code"];
	
	// Country 
	if (isset($location['country'])) {
		if (array_key_exists($location['country'],$countries)) {
			$node['country'] = $location['country'];
			$node['country_name'] = $countries[$location['country']];
		} else {
			drupal_set_message('Country ISO CODE not found for '.$location['country'].'.','error');
		}
	} else {
		if (isset($location['country_name'])) {
			if ($node['country']=array_search($location['country_name'],$countries)) {
				$node['country_name'] = $location['country_name'];
			} else {
				drupal_set_message('Country ISO not found for '.$location['country_name'].'.','error');
			}
		}
	}
	location_standardize_country_code($node['country']); 
    
	// Long-Lattitude
	if (isset($location["latitude"]) && isset($location["longitude"])) {
		if (is_numeric($location["latitude"]) && is_numeric($location["longitude"])) {
			$node['latitude'] = $location["latitude"];
			$node['longitude'] = $location["longitude"];
			$node['locpick']["user_latitude"] = $location["latitude"];
			$node['locpick']["user_longitude"] = $location["longitude"];
		} else {
			drupal_set_message('longitude & Lattitude not numeric.','error');
		}
	} else {
		drupal_set_message('longitude & Lattitude not set.','error');
	}   
	
	$settings = variable_get('location_settings_node_wally_locationtype', array());
    $node['location_settings'] = $settings;

	$node['source'] = 0;
	$node['is_primary'] = 0;

	return (array) $node; 
		
}


/**
 * Translates datetime into CCK datetime field
 *
 * @param $date
 *   - ISO Datetime (e.g. '2010-10-26T21:32:52')
 *
 * @return
 *   CCK datetime field (array)
 */
function _wimprthelp_date_to_ccktime($date) {
  //@todo: handle timezone
  $result['value']=str_replace('T', ' ', $date);
  $result['timezone']=variable_get('date_default_timezone_name', 'UTC');
  $result['timezone_db']='UTC';
  $result['date_type']='datetime';
  return $result;
}

/**
 * Translates datetime into timestamp
 *
 * @param $date
 *   - ISO Datetime (e.g. '2010-10-26T21:32:52')
 *
 * @return
 *   timestamp
 */
function _wimprthelp_date_to_timestamp($date) {
  $result=strtotime($date);
  return $result;
}

/**
 * return filtred html text from html text
 *
 * @param $text
 *   - An non filtred html content
 *
 * @return
 *   An filtred html content
 */
function _wimprthelp_html_text($text){
  return $text;
}

/**
 * return plain text from potential html text
 *
 * @param $text
 *   - An non filtred html content
 *
 * @return
 *   An plain text
 */
function _wimprthelp_plain_text($text){
  return check_plain($text);
}


/**
 * return acreated  drupal file object from uri
 *
 * @param $file_uri
 *   - A uri to the filet
 *
 * @return
 *   A Drupal File Object
 */
function _wimprthelp_create_file_from_uri($file_uri){
  global $proceed_file_path;
  global $base_path;
  
  $uri=parse_url($file_uri);

  switch($uri['scheme']){
    case'embed':
      $file_path= variable_get('wallymport_source', 'sites/default/files/import').'/'.$uri['host'];
      return _wimprthelp_create_file($file_path);
    break;
  }

  return $file_uri;
}


function _wimprthelp_create_file($filepath){
  $uid=variable_get('wallymport_defaultuser',1);
  $file = new stdClass();
  $file->filename = basename($filepath);
  $file->filepath = $filepath;
  $file->filemime = file_get_mimetype($filepath);
  $file->filesize = filesize($filepath);
  $file->uid = $uid;
  $file->status = FILE_STATUS_PERMANENT;
  $file->timestamp = time();
  drupal_write_record('files', $file);
  return field_file_load($filepath);
}


