<?php 

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Facebook'),
  'description' => t('Post messages to Facebook using defined accounts'),
  'defaults' => wallychannels_facebook_channels_settings_default(),
  'types' => array(
    'wally_articlepackage',
    'wally_gallerypackage',
    'wally_pollpackage',
  ),
);

function wallychannels_facebook_channels_process($settings, $node) {
  $main_target = explode('/', $settings['field']);
  $text_field = _wallychannels_get_value_from_text_target($node, $main_target);
  if (empty($text_field)) {
    $fallback_target = explode('/', $settings['fallback_field']);
    $text_field = str_replace('#', '', _wallychannels_get_value_from_text_target($node, $fallback_target));
  }
  
  $text_field = strip_tags($text_field);

  if (!empty($text_field) && $settings['access_token']) {
    global $base_url;
    $path = $base_url.'/'.$node->path;

    module_load_include('php', 'wallychannels', 'plugins/channels/wallyfacebook');
    // Create our Application instance
    $facebook = new WallyFacebook(array(
      'appId'  => $settings['app_id'],
      'secret' => $settings['app_secret'],
    ));
    $facebook->setAccessToken($settings['access_token']);
    $facebook->setTokenExpires($settings['token_expires']);

    $datas = array(
      'link' => $path,
      'message' => $text_field,
    );
    try {
      // Proceed knowing you have a logged in user who's authenticated.
      $ret_obj = $facebook->api('/me/feed', 'POST', $datas);
    } catch (FacebookApiException $e) {
      drupal_set_message($e->__toString(), 'error');
    }

    return TRUE;
  }

  return FALSE;
}

function wallychannels_facebook_channels_access($settings) {
  if (!empty($settings['access_token']) && !empty($settings['token_expires']) && (time() < $settings['token_expires'])) {
    return TRUE;
  }

  return FALSE;
}

function wallychannels_facebook_channels_settings_form(&$form, &$form_state) {
  $conf = wallychannels_facebook_channels_settings_default($form_state);
  
  if (isset($_GET['code'])) {
    module_load_include('php', 'wallychannels', 'plugins/channels/wallyfacebook');
    // Create our Application instance
    $facebook = new WallyFacebook(array(
      'appId'  => $conf['app_id'],
      'secret' => $conf['app_secret'],
    ));
    $conf['code'] = $_GET['code'];
    try {
      // Proceed knowing you have a logged in user who's authenticated.
      $conf['access_token'] = $facebook->getAccessToken();
      $conf['token_expires'] = $facebook->getTokenExpires();
      drupal_set_message(t('Authentication granted, please save this form again to save access token.'), 'warning');
    } catch (FacebookApiException $e) {
      drupal_set_message($e->__toString(), 'error');
    }
  }

  $form['app_id'] = array(
    '#type' => 'textfield',
    '#title' => t('App ID'),
    '#required' => TRUE,
    '#default_value' => $conf['app_id'],
  );

  $form['app_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('App Secret'),
    '#required' => TRUE,
    '#default_value' => $conf['app_secret'],
  );
  
  // @todo : ajouter un bouton pour reseter le code et l'access_token
  
  $form['code'] = array(
    '#type' => 'hidden',
    '#title' => '',
    '#default_value' => $conf['code'],
  );
  
  $form['access_token'] = array(
    '#type' => 'hidden',
    '#title' => '',
    '#default_value' => $conf['access_token'],
  );
  
  $form['token_expires'] = array(
    '#type' => 'hidden',
    '#title' => '',
    '#default_value' => $conf['token_expires'],
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

function wallychannels_facebook_channels_settings_form_submit(&$form, &$form_state) {
  $values = $form_state['values']['settings'];
  if (empty($values['access_token']) || empty($values['token_expires']) || (time() > $values['token_expires'])) {
    global $base_url;
    module_load_include('php', 'wallychannels', 'plugins/channels/wallyfacebook');
    // Create our Application instance
    $facebook = new WallyFacebook(array(
      'appId'  => $values['app_id'],
      'secret' => $values['app_secret'],
    ));
    $current_path = $_GET['q'];
    if (strstr($current_path, '/add/')) {
      $new_id = db_result(db_query("SELECT MAX(cid) FROM {wallychannels_channels}")) + 1;
      $destination = str_replace('/add/', '/list/'.$new_id.'/edit/', $current_path);
    } else {
      $destination = $current_path;
    }
    $form_state['redirect_after_save'] = $facebook->getLoginUrl(array('scope' => 'publish_stream'), $base_url.'/'.$destination);
  }
}

function wallychannels_facebook_channels_settings_default($form_state = array()) {
  if (isset($form_state) && !empty($form_state)) {
    return $form_state;
  }

  $defaults = array(
    'app_id' => '',
    'app_secret' => '',
    'code' => '',
    'access_token' => '',
    'token_expires' => '',
    'field' => 'field_mainstory/field_extractshort',
    'fallback_field' => 'field_mainstory/field_extractmedium',
  );

  return $defaults;
}

function wallychannels_facebook_channels_admin_preview($settings) {
  $output = '';

  $output .= '<b>'.t('Authenticated :').'</b> '.((empty($settings['access_token']) || empty($settings['token_expires']) || (time() > $settings['token_expires'])) ? t('No') : t('Yes')).'<br>';
  $output .= '<b>'.t('Authentication expires :').'</b> '.((empty($settings['token_expires']) || (time() > $settings['token_expires'])) ? t('Expired') : date('d-m-Y H:i:s', $settings['token_expires'])).'<br>';
  $output .= '<b>'.t('Main field :').'</b> '.$settings['field'].'<br>';
  $output .= '<b>'.t('Fallback field :').'</b> '.$settings['fallback_field'].'<br>';

  return $output;
}

function wallychannels_facebook_channels_reset_authentication(&$settings) {
  $settings['access_token'] = '';
  $settings['token_expires'] = '';
}
