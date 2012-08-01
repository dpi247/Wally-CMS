<?php 

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Twitter'),
  'description' => t('Post tweets using defined accounts'),
  'defaults' => wallychannels_twitter_channels_settings_default(),
  'types' => array(
    'wally_articlepackage',
    'wally_gallerypackage',
    'wally_pollpackage',
  ),
  'form settings submit' => 'wallychannels_twitter_channels_settings_form_submit',
);

function wallychannels_twitter_channels_process($settings, $node) {
  $main_target = explode('/', $settings['field']);
  $text_field = _wallychannels_get_value_from_text_target($node, $main_target);
  if (empty($text_field)) {
    $fallback_target = explode('/', $settings['fallback_field']);
    $text_field = _wallychannels_get_value_from_text_target($node, $fallback_target);
  }

  if (!empty($text_field)) {
    $short_url = '';
    if ($settings['short_url']) {
      global $base_url;
      $path = $base_url.'/'.$node->path;
      $headers = array('Content-Type' => 'application/json');
      $data = drupal_to_js(array('longUrl' => $path));
      $google_api_url = 'https://www.googleapis.com/urlshortener/v1/url';
      $google_api_url .= (isset($settings['api_key']) && !empty($settings['api_key'])) ? '?key='.$settings['api_key'] : '';
      $short_url_req = drupal_http_request($google_api_url, $headers, 'POST', $data);
      
      if ($short_url_req->code == 200) {
        $req_datas = json_decode($short_url_req->data);
        $short_url = $req_datas->id;
      }
    }
    
    if (strlen($text_field) > (140 - 1 - strlen($short_url))) {
      $text_field = substr($text_field, 0, 140 - 4 - strlen($short_url)).'... '.$short_url;
    } else {
      $text_field = $text_field.' '.$short_url;
    }

    module_load_include('php', 'wallychannels', 'plugins/channels/twitteroauth/twitteroauth');
    $twitteroauth = new TwitterOAuth($settings['consumer_key'], $settings['consumer_secret'], $settings['oauth_token'], $settings['oauth_token_secret']);
    $params = array('status' => $text_field);
    $twitteroauth->post('statuses/update', $params);

    return TRUE;
  }

  return FALSE;
}

function wallychannels_twitter_channels_access($settings) {
  if ($settings['oauth_token_secret']) {
    return TRUE;
  }

  return FALSE;
}

function wallychannels_twitter_channels_settings_form(&$form, &$form_state) {
  $conf = wallychannels_twitter_channels_settings_default($form_state);
  if (isset($_GET['oauth_token']) && isset($_GET['oauth_verifier'])) {
    module_load_include('php', 'wallychannels', 'plugins/channels/twitteroauth/twitteroauth');
    $conf['oauth_token'] = $_GET['oauth_token'];
    $conf['oauth_verifier'] = $_GET['oauth_verifier'];
    $twitteroauth = new TwitterOAuth($conf['consumer_key'], $conf['consumer_secret'], $conf['oauth_token'], $conf['oauth_verifier']);
    $token = $twitteroauth->getAccessToken($conf['oauth_verifier']);
    $conf['oauth_token'] = $token['oauth_token'];
    $conf['oauth_token_secret'] = $token['oauth_token_secret'];
    drupal_set_message(t('Authentication granted, please save this form again to save access token.'), 'warning');
  }

  // @todo : ajouter un bouton pour reseter les oauth_token et oauth_verifier

  $form['consumer_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Consumer Key'),
    '#required' => TRUE,
    '#default_value' => $conf['consumer_key'],
  );
  
  $form['consumer_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Consumer Secret'),
    '#required' => TRUE,
    '#default_value' => $conf['consumer_secret'],
  );
  
  $form['oauth_token'] = array(
    '#type' => 'hidden',
    '#title' => '',
    '#default_value' => $conf['oauth_token'],
  );
  
  $form['oauth_token_secret'] = array(
    '#type' => 'hidden',
    '#title' => '',
    '#default_value' => $conf['oauth_token_secret'],
  );

  $form['oauth_verifier'] = array(
    '#type' => 'hidden',
    '#title' => '',
    '#default_value' => $conf['oauth_verifier'],
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
  
  ctools_include('dependent');
  ctools_add_js('dependent');

  $form['short_url'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add link to your node'),
    '#description' => t('This will add a link to your node, in a short-URL format, at the end of your tweet.'),
    '#default_value' => $conf['short_url'],
  );
  
  $form['api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Google API key'),
    '#default_value' => $conf['api_key'],
    '#dependency' => array('edit-settings-short-url' => array(1)),
    '#process' => array('ctools_dependent_process'),
  );
}

function wallychannels_twitter_channels_settings_form_submit(&$form, &$form_state) {
  $values = $form_state['values']['settings'];
  if (empty($values['oauth_token']) || empty($values['oauth_token_secret']) || empty($values['oauth_verifier'])) {
    global $base_url;
    module_load_include('php', 'wallychannels', 'plugins/channels/twitteroauth/twitteroauth');
    $twitteroauth = new TwitterOAuth($values['consumer_key'], $values['consumer_secret']);
    $current_path = $_GET['q'];
    if (strstr($current_path, '/add/')) {
      $new_id = db_result(db_query("SELECT MAX(cid) FROM {wallychannels_channels}")) + 1;
      $destination = str_replace('/add/', '/list/'.$new_id.'/edit/', $current_path);
    } else {
      $destination = $current_path;
    }
    $token = $twitteroauth->getRequestToken($base_url.'/'.$destination);
    $form_state['redirect_after_save'] = $twitteroauth->getAuthorizeURL($token);
  }
}

function wallychannels_twitter_channels_settings_default($form_state = array()) {
  if (isset($form_state) && !empty($form_state)) {
    return $form_state;
  }

  $defaults = array(
    'consumer_key' => '',
    'consumer_secret' => '',
    'oauth_token' => '',
    'oauth_token_secret' => '',
    'oauth_verifier' => '',
    'field' => 'field_mainstory/field_extractshort',
    'fallback_field' => 'field_mainstory/field_extractmedium',
    'short_url' => 0,
  );

  return $defaults;
}

function wallychannels_twitter_channels_admin_preview($settings) {
  $output = '';
  
  $output .= '<b>'.t('Authenticated :').'</b> '.($settings['oauth_token_secret'] ? t('Yes') : t('No')).'<br>';
  $output .= '<b>'.t('Main field :').'</b> '.$settings['field'].'<br>';
  $output .= '<b>'.t('Fallback field :').'</b> '.$settings['fallback_field'].'<br>';
  $output .= '<b>'.t('Short URL :').'</b> '.($settings['short_url'] ? t('Yes') : t('No')).'<br>';

  return $output;
}
