<?php

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