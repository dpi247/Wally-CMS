<?php 

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
/*$plugin = array(
  'title' => t('Google+'),
  'description' => t('Post messages using defined accounts'),
  'defaults' => wallychannels_googleplus_channels_settings_default(),
  'types' => array(
    'wally_articlepackage',
    'wally_gallerypackage',
    'wally_pollpackage',
  ),
);*/

function wallychannels_googleplus_channels_process($settings) {
  module_load_include('php', 'wallychannels', 'plugins/channels/google-api-php-client/src/apiClient');
  module_load_include('php', 'wallychannels', 'plugins/channels/google-api-php-client/src/contrib/apiPlusService');
  
  global $base_url;
  $client = new apiClient();
  $client->setApplicationName("Google+ PHP Starter Application");
  // Visit https://code.google.com/apis/console to generate your
  // oauth2_client_id, oauth2_client_secret, and to register your oauth2_redirect_uri.
  $client->setClientId('453334009800.apps.googleusercontent.com');
  $client->setClientSecret('Vtz0ybhEjrvO19J32HKVcIqg');
  $client->setRedirectUri("http://localhost/wally/index.php?q=devel/php");
  //$client->setDeveloperKey('AIzaSyBI0K4JDJER_LS4hYIoR3OdM8IEbvWAlyk');
  $plus = new apiPlusService($client);
  
  //header('Location:'. $client->createAuthUrl());
  $_GET['code'] = "4/tRp2a2HKmHz28eDQDvIYFM5kEnqa.csKrfD2kUskTsNf4jSVKMpZCNqSHcQI";
  $client->authenticate();
  $client->setAccessToken($client->getAccessToken());
  $me = $plus->people->get('me');
  
  dsm($me);
  // @todo : filtrer les "#"
  return TRUE;
}

function wallychannels_googleplus_channels_settings_form(&$form, &$form_state) {
  $conf = wallychannels_googleplus_channels_settings_default($form_state);

  $form['user'] = array(
    '#type' => 'textfield',
    '#title' => t('User Name'),
    '#required' => TRUE,
    '#default_value' => $conf['user'],
  );

  $form['password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#required' => TRUE,
    '#default_value' => $conf['password'],
  );

  $avail_fields = array();
  $types = array(
    'wally_articlepackage',
    'wally_gallerypackage',
    'wally_pollpackage',
  );
  foreach ($types as $type) {
    $avail_fields = array_merge($avail_fields, _wallychannels_get_text_fields($type));
  }

  $form['field'] = array(
    '#type' => 'select',
    '#title' => t('Field used'),
    '#required' => TRUE,
    '#options' => $avail_fields,
    '#default_value' => $conf['field'],
  );

  $form['fallback_field'] = array(
    '#type' => 'select',
    '#title' => t('Fallback field'),
    '#description' => t('This field will be used if the first is empty or not present'),
    '#options' => $avail_fields,
    '#default_value' => $conf['fallback_field'],
  );
}

function wallychannels_googleplus_channels_settings_default($form_state = array()) {
  if (isset($form_state) && !empty($form_state)) {
    return $form_state;
  }

  $defaults = array(
    'user' => '',
    'password' => '',
    'field' => 'field_mainstory/field_extractshort',
    'fallback_field' => 'field_mainstory/field_extractmedium',
  );

  return $defaults;
}

function wallychannels_googleplus_channels_admin_preview($settings) {
  $output = '';
  
  $output .= '<b>'.t('User :').'</b> '.$settings['user'].'<br>';
  $output .= '<b>'.t('Main field :').'</b> '.$settings['field'].'<br>';
  $output .= '<b>'.t('Fallback field :').'</b> '.$settings['fallback_field'].'<br>';

  return $output;
}
