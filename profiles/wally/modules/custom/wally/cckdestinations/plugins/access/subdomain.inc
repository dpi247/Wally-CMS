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
  'title' => t("Sub domain"),
  'description' => t('Control access by the existence of a substring in the domain of the url.'),
  'callback' => 'cckdestinations_subdomain_ctools_access_check',
  'default' => array('type' => array()),
  'settings form' => 'cckdestinations_subdomain_ctools_access_settings',
  'settings form submit' => 'cckdestinations_subdomain_ctools_access_settings_submit',
  'summary' => 'cckdestinations_subdomain_ctools_access_summary',
  //TODO: what restriction is for ?
  //'restrictions' => 'cckdestinations_subdomain_ctools_access_restrictions',
);

/**
 * Settings form for the 'by node_type' access plugin
 */
function cckdestinations_subdomain_ctools_access_settings(&$form, &$form_state, $conf) {
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
function cckdestinations_subdomain_ctools_access_check($conf, $context) {
  // As far as I know there should always be a context at this point, but this
  // is safe.
  $domain = $_SERVER['HTTP_HOST'];
  
  switch ($conf['method']){
    case'strstr':
    	dsm($domain,domain);
    	dsm( $conf['token'],token);
    	var_dump(strstr($domain, $conf['token']));
      return strstr($domain, $conf['token']);
    break;
    case'reg':
      return preg_match($conf['token'], $domain) > 0;
    break;
  }
  dsm('rr');
  return false;
}

//TODO: what restriction is for ?
/*function cckdestinations_subdomain_ctools_access_restrictions($conf, &$context) {
  if (isset($context->restrictions['type'])) {
    $context->restrictions['type'] = array_unique(array_merge($context->restrictions['type'], array_keys(array_filter($conf['type']))));
  }
  else {
    $context->restrictions['type'] = array_keys(array_filter($conf['type']));
  }
}
*/
/**
 * Provide a summary description based upon the checked subdomains.
 */
function cckdestinations_subdomain_ctools_access_summary($conf, $context) {
  return t('Subdomain match "@token" with method "@method"', array('@token' => $conf['token'], '@method' => $conf['method']));
}

