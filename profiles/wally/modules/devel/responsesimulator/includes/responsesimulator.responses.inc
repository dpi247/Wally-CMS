<?php

/**
 * Handle an HTTP response
 *
 * @param string $resp_name
 *   The machine name of the requested HTTP response
 */
function responsesimulator_process_response($resp_name) {
  drupal_get_messages();

  if ($http_resp = responsesimulator_get_defined_http_response($resp_name)) {
    if (isset($http_resp['settings']['text'])) {
      print $http_resp['settings']['text'];
    }

    switch ($http_resp['httpcode']) {
      case '301':
      case '302':
        drupal_goto('', NULL, NULL, $http_resp['httpcode']);
        break;

      default:
        header('Cache-Control: no-cache', TRUE, $http_resp['httpcode']);
        break;
    }
  } else {
    drupal_not_found();
  }
}
