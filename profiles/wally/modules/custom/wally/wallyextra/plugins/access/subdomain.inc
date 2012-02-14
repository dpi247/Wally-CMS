<?php
// $Id: subdomain.inc,v 1.7.2.2 2009/11/13 00:44:16 merlinofchaos Exp $

/**
 * @file
 * Plugin to provide access control based upon node type.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Sub domain'),
  'description' => t('Control access by the existence of a substring in the domain of the url.'),
  'callback' => 'wallyextra_subdomain_ctools_access_check',
  'default' => array('type' => array()),
  'settings form' => 'wallyextra_subdomain_ctools_access_settings',
  'summary' => 'wallyextra_subdomain_ctools_access_summary',
);

/**
 * Settings form for the 'by node_type' access plugin
 */
function wallyextra_subdomain_ctools_access_settings(&$form, &$form_state, $conf) {
  $form['settings']['token'] = array(
    '#title' => t('token'),
    '#type' => 'textfield',
    '#description' => t('The token to search in the domain.'),
    '#default_value' => $conf['token'],
  );
  
  $options = array(
    'strstr' => 'strstr ('.t('Case Sensitive').')',
    'reg' => t('Regular Expression'),
  );
  $form['settings']['method'] = array(
    '#title' => t('method'),
    '#type' => 'radios',
    '#options' => $options,
    '#description' => t('The methode used to search the token, strstr is faster, reg more powerfull ....'),
    '#default_value' => $conf['method'],
  );
}

/**
 * Check for access.
 */
function wallyextra_subdomain_ctools_access_check($conf, $context) {
  // As far as I know there should always be a context at this point, but this
  // is safe.
  $domain = $_SERVER['HTTP_HOST'];
  
  switch ($conf['method']){
    case'strstr':
      return strstr($domain, $conf['token']);
    break;
    case'reg':
      return preg_match($conf['token'], $domain) > 0;
    break;
  }
  
  return false;
}

/**
 * Provide a summary description based upon the checked subdomains.
 */
function wallyextra_subdomain_ctools_access_summary($conf, $context) {
  return t('Subdomain match "@token" with method "@method"', array('@token' => $conf['token'], '@method' => $conf['method']));
}
