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