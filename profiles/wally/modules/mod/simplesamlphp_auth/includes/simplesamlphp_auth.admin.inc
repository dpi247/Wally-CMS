<?php
// $Id: yaxim.admin.inc,v 1.0 2010/08/23 14:39:00 rso Exp $

/**
 * Settings form.
 */
function simplesamlphp_auth_settings_form() {
  $form = array();

  $form['simplesamlphp_auth_installdir'] = array(
		'#type' => 'textfield',
		'#title' => t('Instalation directory (default: /var/simplesamlphp)'),
		'#default_value' => variable_get('simplesamlphp_auth_installdir', '/var/simplesamlphp'),
		'#description' => t('The base directory of simpleSAMLphp. Absolute path with no trailing slash.'),
  );

  $form['simplesamlphp_auth_user_name'] = array(
    '#type' => 'select',
    '#title' => t('Which attributes from simpleSAMLphp should be used as user\'s name'),
    '#default_value' => variable_get('simplesamlphp_auth_user_name', 'smartname-fullname-eduPersonPrincipalName'),
    '#options' => array(
      'eduPersonPrincipalName' => t('eduPersonPrincipalName - globally unique'),
      'smartname-fullname-eduPersonPrincipalName' => t('smartname-fullname [eduPersonPrincipalName] - globally unique'),
      'smartname-fullname-drupaluid' => t('smartname-fullname [drupaluid] - globally unique'),
      'smartname-fullname' => t('smartname-fullname - NOT globally unique'),
    ),
		'#description' => t('If the user attribute is multivalued, the first value will be used.'),
  );

  $form['simplesamlphp_auth_unique_id'] = array(
		'#type' => 'textfield',
		'#title' => t('Which attribute from simpleSAMLphp should be used as unique identifier for the user'),
		'#default_value' => variable_get('simplesamlphp_auth_unique_id', 'eduPersonPrincipalName'),
		'#description' => t('If the user attribute is multivalued, the first value will be used.'),
  );

  $form['simplesamlphp_auth_mailattr'] = array(
		'#type' => 'textfield',
		'#title' => t('Which attribute from simpleSAMLphp should be used as user mail address'),
		'#default_value' => variable_get('simplesamlphp_auth_mailattr', 'mail'),
		'#description' => t('If the user attribute is multivalued, the first value will be used.'),
  );

  $form['simplesamlphp_auth_logout_page'] = array(
		'#type' => 'textfield',
		'#title' => t('Web page to send the user to when he logs out'),
		'#default_value' => variable_get('simplesamlphp_auth_logout_page', '/'),
		'#description' => t('The path to the web page'),
  );

  $form['simplesamlphp_auth_rolepopulation'] = array(
		'#type' => 'textfield',
		'#title' => t('Automatic role population from simpleSAMLphp attributes'),
		'#default_value' => variable_get('simplesamlphp_auth_rolepopulation', ''),
		'#description' => t('Example: <i>roleid1:condition1|roleid2:contition2... <br />1:eduPersonPrincipalName,@=,uninett.no;affiliation,=,employee|2:mail,=,andreas@uninett.no</i>'),
  );
  
  $form['simplesamlphp_auth_login_post_url'] = array(
		'#type' => 'textfield',
		'#title' => t('SSO server called for the connection.'),
		'#default_value' => variable_get('simplesamlphp_auth_login_post_url', 'http://sso1.rossel.be:8080/opensso'),
  );
  
  $form['simplesamlphp_auth_login_return_url'] = array(
		'#type' => 'textfield',
		'#title' => t('Return URL after successful login.'),
		'#default_value' => variable_get('simplesamlphp_auth_login_return_url', 'home'),
  );
  
  $form['simplesamlphp_auth_roles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Roles which can bypass the authentification'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  foreach(user_roles() as $role) {
    $form['simplesamlphp_auth_roles']['simplesamlphp_auth_bypass_role_'.str_replace(' ', '_', $role)] = array(
      '#type' => 'checkbox',
      '#title' => t(ucwords($role)),
      '#default_value' => variable_get('simplesamlphp_auth_bypass_role_'.str_replace(' ', '_', $role), 0),
    );
  }

  return system_settings_form($form);
}
