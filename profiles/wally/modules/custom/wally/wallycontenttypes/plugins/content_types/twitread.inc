<?php
/*
  Plugin declaration function - returns a plugin definition array that
 * describes the content type.
 */
 
/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'no title override' => TRUE,
  'defaults' => _wallycontenttypes_twitread_defaultformvalues(),
  'js' => array('misc/autocomplete.js', 'misc/textarea.js', 'misc/collapse.js'),
  // Make sure the edit form is only used for some subtypes.
  'edit form' => '',
  'add form' => '',
  'edit text' => t('Edit'),
  'all contexts' => TRUE,
);

/**
 * Building the configuration form of a Twitter reader.
 */
function wallycontenttypes_twitread_content_type_edit_form(&$form, &$form_state) {
  $settings = wallycontenttypes_twitread_content_type_get_conf($form_state['subtype'], $form_state['conf']);
  $form_state['settings'] = $settings;
  $creation_infos = explode('_', $form_state['conf']['caid']);
  $creation_date = $creation_infos[1];

  if ($settings['twitread_type'] == 'fixed' && ($creation_date+1)>=time()) {
    return; // no form for this case.
  }
  
  $form['uris'] = array (
    '#type'             => 'textarea',
    '#id'               => 'uri_area',
    '#title'            => t('IDs of the Twitter feeds'),
    '#description'      => t('Please enter one ID by line.'),
    '#default_value'    => $settings['uris'],
    '#required'         => TRUE,
  );

  $form['maxitem_global'] = array (
    '#type'             => 'textfield',
    '#title'            => t('Max item displayed in the final mixed Twitter feed'),
    '#default_value'    => $settings['maxitem_global'],
    '#required'         => TRUE,
  );
  
  $form['maxitem_byfeed'] = array (
    '#type'             => 'textfield',
    '#title'            => t('Max item to fetch by Twitter feed'),
    '#default_value'    => $settings['maxitem_byfeed'],
    '#required'         => TRUE,
  );
  
  $form['delta_t'] = array (
    '#type'             => 'textfield',
    '#title'            => t('Refresh time (minutes)'),
    '#default_value'    => $settings['delta_t'],
    '#required'         => TRUE,
  );
  
  $mix_schemas = array(
    'alt'               => 'Alternate',
    'any'               => 'Most Recent'
  );
  
  $form['mix_schema'] = array(
    '#type'             => 'select',
    '#title'            => t('Schema used for mixing the Twitter feeds'),
    '#default_value'    => $settings['mix_schema'],
    '#options'          => $mix_schemas,
  );
  
  $form['own_schema'] = array (
    '#type'             => 'textfield',
    '#title'            => t('Define your own schema used for mixing the Twitter feeds'),
    '#description'      => t('Examples:
        1,2,3 / 1-2,3,2,1-3,2,2-3 / any,2,1-2,3,any,2 / ...
      '),
    '#default_value'    => $settings['own_schema'],
    '#required'         => FALSE,
  );

  $form['override_theme'] = array (
    '#type'             => 'checkbox',
    '#default_value'    => $settings['override_theme'],
    '#id'               =>  'override-theme-checkbox',
    '#title'            => 'Override Theming Function',
  );

  $form['override_theme_text'] = array (
    '#type'             => 'textfield',
    '#default_value'    => $settings['override_theme_text'],
    '#id'               => 'override-theme-textfield',
    '#dependency'       => array('override-theme-checkbox'=>array('1')),
    '#dependency_type'  => 'disable',
    '#process'          => array('ctools_dependent_process'),
    '#title'            => 'Theming Function Name',
  );
  
  $form['caid'] = array (
    '#type'             => 'hidden',
    '#value'            => $settings['caid'],
  );
  
  if (!user_access('administer twitread content') || $settings['twitread_type'] == 'fixed') {
    return;
  }
  
  // Make the other form items dependent upon it.
  ctools_include('dependent');
  ctools_add_js('dependent');

  $form['reusable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Make this content reusable'),
    '#default_value' => FALSE,
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Machine name'),
    '#description' => t('The machine readable name of this content. It must be unique, and it must contain only alphanumeric characters and underscores. Once created, you will not be able to change this value!'),
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-reusable' => array(1)),
  );

  $form['category'] = array(
    '#type' => 'textfield',
    '#title' => t('Category'),
    '#description' => t('What category this content should appear in. If left blank the category will be "Wally reusable blocks".'),
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-reusable' => array(1)),
  );

  $form['admin_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Administrative description'),
    '#description' => t('A description of what this content is, does or is for, for administrative use.'),
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-reusable' => array(1)),
  );
}

/**
 * Validation for the configuration form of a Twitter reader.
 * 
 * @todo
 *   Change uris' validation in accordance with Twitter API.
 */
function wallycontenttypes_twitread_content_type_edit_form_validate(&$form, &$form_state) {
  /*if (isset($form['uris']['#value'])) {
    $uri_strings = explode("\n", str_replace(array("\n", "\r\n"), "\n", trim($form['uris']['#value'])));
    foreach ($uri_strings as $uri_string) {
      $uri_response = drupal_http_request($uri_string);
      if ($uri_response->status_message != 'OK') {
        form_set_error('uris', t('The URI '.check_plain($uri_string).' doesn\'t respond.'));
      }
    }
  }*/

  if (isset($form['maxitem_global']['#value']) && !is_numeric($form['maxitem_global']['#value'])) {
    form_set_error('maxitem_global', t('The total max item isn\'t a numeric value'));
  } elseif (isset($form['maxitem_global']['#value']) && $form['maxitem_global']['#value'] <= 0) {
    form_set_error('maxitem_global', t('The total max item value must be greater than 0'));
  }
  
  if (isset($form['maxitem_byfeed']['#value']) && !is_numeric($form['maxitem_byfeed']['#value'])) {
    form_set_error('maxitem_byfeed', t('The max item by feed isn\'t a numeric value'));
  } elseif (isset($form['maxitem_byfeed']['#value']) && $form['maxitem_byfeed']['#value'] <= 0) {
    form_set_error('maxitem_byfeed', t('The max item by feed value must be greater than 0'));
  }
  
  if (isset($form['delta_t']['#value']) && !is_numeric($form['delta_t']['#value'])) {
    form_set_error('delta_t', t('The refresh time isn\'t a numeric value'));
  } elseif (isset($form['delta_t']['#value']) && $form['delta_t']['#value'] < 0) {
    form_set_error('delta_t', t('The refresh time must be greater or equal to 0'));
  }
  
  if ($form['own_schema']['#value'] && !empty($form['own_schema']['#value'])) {
    $temp_schema = explode(',', $form['own_schema']['#value']);
    foreach ($temp_schema as $temp_elem) {
      if ($temp_elem != 'any' && !is_numeric($temp_elem)) {
        $temp_sub_schema = explode('-', $temp_elem);
        foreach ($temp_sub_schema as $temp_sub_elem) {
          if (!is_numeric($temp_sub_elem)) {
            form_set_error('own_schema', t('The requested schema isn\'t readable.'));
          }
        }
      }
    }
  }
}

/**
 * Submitting the configuration form of a Twitter reader.
 */
function wallycontenttypes_twitread_content_type_edit_form_submit(&$form, &$form_state) {
  if ($form_state['settings']['twitread_type'] == 'fixed') {
    _wallycontenttypes_twitread_content_type_edit_save($form_state['settings']['content'], $form_state);
  } else if (!empty($form_state['values']['reusable'])) {
    // If the 'reusable' checkbox was checked, we will create a new
    // twitread content and give it the proper values.
    $content = wallycontenttypes_newctoolsexport('wallycontenttypes_twitread_content', 'twitread');
    _wallycontenttypes_twitread_content_type_edit_save($content, $form_state);
    $form_state['conf']['name'] = $content->name;
  } else {
    // Otherwise, just save values into $conf normally.
    cache_clear_all($form['caid']['#value'], $table = 'cache');
    $form_state['conf']['uris'] = $form['uris']['#value'];
    $form_state['conf']['maxitem_global'] = $form['maxitem_global']['#value'];
    $form_state['conf']['maxitem_byfeed'] = $form['maxitem_byfeed']['#value'];
    $form_state['conf']['delta_t'] = $form['delta_t']['#value'];
    $form_state['conf']['mix_schema'] = $form['mix_schema']['#value'];
    $form_state['conf']['own_schema'] = $form['own_schema']['#value'];
    $form_state['conf']['override_theme'] = $form['override_theme']['#value'];
    $form_state['conf']['override_theme_text'] = $form['override_theme_text']['#value'];
    $form_state['conf']['caid'] = $form['caid']['#value'];
  }
}

/**
 * Export the configuration of the reusable block.
 */
function _wallycontenttypes_twitread_content_type_edit_save(&$content, $form_state) {
  // Apply updates to the content object.
  $content->name = $form_state['values']['name'];
  $content->category = $form_state['values']['category'];
  $content->admin_title = $form_state['values']['name'];
  $content->admin_description = $form_state['values']['admin_description'];
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    if (isset($form_state['values'][$key])) {
      $content->settings[$key] = $form_state['values'][$key];
    }
  }

  ctools_export_crud_save('wallycontenttypes_twitread_content', $content);
}

/**
 * Return all twitread content types available.
 */
function wallycontenttypes_twitread_content_type_content_types() {
  ctools_include('export');
  $types = array();
  $types['twitread'] = _wallycontenttypes_twitread_default_ctools_content_type();
  
  foreach (ctools_export_crud_load_all('wallycontenttypes_twitread_content') as $name => $content) {
    $types[$name] = _wallycontenttypes_twitread_ctools_content_type($content);
  }
  return $types;
}

/**
 * Return the twitread content types with the specified $subtype_id.
 */
function wallycontenttypes_twitread_content_type_content_type($subtype_id) {
  if ($subtype_id == 'twitread') {
    return _wallycontenttypes_twitread_default_ctools_content_type();
  }
  else {
    ctools_include('export');
    $content = ctools_export_crud_load('wallycontenttypes_twitread_content', $subtype_id);
    if ($content) {
      return _wallycontenttypes_twitread_ctools_content_type($content);
    }
  }
}

/**
 * Settings for the default twitread content type.
 *
 * The default is the one that allows the user to actually create a type.
 */
function _wallycontenttypes_twitread_default_ctools_content_type() {
  $info = array(
    'name' => 'twitread',
    'title' => t('Twitter Reader'),
    'category' => t('Wally Tools'),
    'edit form' => 'wallycontenttypes_twitread_content_type_edit_form',
    'description' => t('Fetch some Twitter feed and mix them together in a single feed'),
    'all contexts' => TRUE,
    'defaults' => _wallycontenttypes_twitread_defaultformvalues(),
  );

  return $info;
}

/**
 * Return an info array for a specific twitread content type.
 */
function _wallycontenttypes_twitread_ctools_content_type($content) {
  $info = array(
    'name' => $content->name,
    'title' => check_plain($content->admin_title),
    'description' => check_plain($content->admin_description),
    'category' => $content->category ? check_plain($content->category) : t('Wally reusable blocks'),
    'edit form' => 'wallycontenttypes_twitread_content_type_edit_form',
    'all contexts' => TRUE,
    'icon' => 'icon_block_custom.png',
    // Store this here to make it easy to access.
    'content' => $content,
  );

  return $info;
}

/**
 * Rendering of the Twitter reader ctools.
 */
function wallycontenttypes_twitread_content_type_render($subtype, $conf, $panel_args, $context) {
  $settings = wallycontenttypes_twitread_content_type_get_conf(wallycontenttypes_twitread_content_type_content_type($subtype), $conf);


  $feed = array();
  $block = new stdClass();
  $block->module = 'wallycontenttypes';
  $block->title = 'Twitter reader.';
  $cid = $settings['caid'];
  $cached_feed = cache_get($cid);
  if ($cached_feed == 0 || !isset($cached_feed) || empty($cached_feed) || true) {
    $twit_uris = explode("\n", str_replace(array("\n", "\r\n"), "\n", trim($settings['uris'])));
    $override_theme = $settings['override_theme'];
    if (isset($settings['own_schema']) && !empty($settings['own_schema'])) {
      $mix_schema = explode(',', $settings['own_schema']);
    } else {
      if ($settings['mix_schema'] == 'alt') {
        $mix_schema = array();
        for ($i=0; $i<sizeof($twit_uris); $i++) $mix_schema[] = $i+1;
      } else {
        $mix_schema = explode(',', $settings['mix_schema']);
      }
    }
    $override_theme_text = $settings['override_theme_text'];
    $maxitem_global = $settings['maxitem_global'];
    $maxitem_byfeed = $settings['maxitem_byfeed'];

    //foreach ($twit_uris as $twit_uri) {
      $feed_array[] = _wallycontenttypes_twitread_createarrayfromuri('drupal', $maxitem_byfeed);
    //}
    
    for ($i=0; $i<$maxitem_global; $i++) {
      $schema_elem = $mix_schema[$i%sizeof($mix_schema)];
      $available_items = _wallycontenttypes_twitread_getavailableitemsfromschema($schema_elem, $feed_array);
      $most_recent_index = _wallycontenttypes_twitread_getmostrecentfromavailables($available_items);
      $item = array_shift($feed_array[$most_recent_index]);
      array_push($feed, $item);
    }
    
    cache_set($cid, $feed, 'cache', time() + mktime(0, $settings['delta_t']) + 1);
  } else {
    $feed = $cached_feed->data;
  }
//dsm($feed_array);
  $block->content = theme("wallyct_twit", $feed_array[0], $settings); 
  return $block;
}

/**
 * Given a $subtype and a $conf, return the actual settings to use.
 *
 * The actual settings may be stored directly in the pane or this may
 * be a pointer to re-usable content that may be in the database or in
 * an export. We have to determine from the subtype whether or not it
 * is local or shared twitread content.
 * 
 * @param $subtype
 *   The given subtype.
 * @param $conf
 *   The configuration informations.
 * 
 * @return
 *   The usable settings.
 */
function wallycontenttypes_twitread_content_type_get_conf($subtype, $conf) {
  if ($subtype['name'] != 'twitread') {
    $settings = $subtype['content']->settings;
    $settings['twitread_type'] = 'fixed';
    $settings['content'] = $subtype['content'];
  } else {
    // This means they created it as twitread content and then set it as
    // reusable. Since we're not allowed to change the subtype, we're
    // still stored as though we are local, but are pointing off to
    // non-local.
    if (!empty($conf['name'])) {
      ctools_include('export');
      $content = ctools_export_crud_load('wallycontenttypes_twitread_content', $conf['name']);
      if ($content) {
        $settings = $content->settings;
      } else {
        $content = wallycontenttypes_newctoolsexport('wallycontenttypes_twitread_content', 'twitread');
        $content->name = $conf['name'];
        $settings = _wallycontenttypes_twitread_defaultformvalues();
      }
      $settings['twitread_type'] = 'fixed';
      $settings['content'] = $content;
    } else {
      // This means that it is created as twitread and has not been set to
      // reusable.
      $settings = $conf;
      $settings['twitread_type'] = 'local';
    }
  }

  return $settings;
}

/**
 * Callback to provide the administrative title of the twitread content.
 */
function wallycontenttypes_twitread_content_type_admin_title($subtype, $conf) {
  $settings = wallycontenttypes_twitread_content_type_get_conf(wallycontenttypes_twitread_content_type_content_type($subtype), $conf);

  $output = t('Twitter reader');
  $title = !empty($settings['admin_title']) ? $settings['admin_title'] : $settings['title'];
  if ($title) {
    if ($settings['twitread_type'] != 'fixed') {
      $output = t('Twitter reader: @title', array('@title' => $title));
    }
    else {
      $output = $title;
    }
  }

  return $output;
}

/**
 * Callback to provide administrative info. In this case we'll render the
 * content as long as it's not PHP, which is too risky to render here.
 */
function wallycontenttypes_twitread_content_type_admin_info($subtype, $conf) {
  $settings = wallycontenttypes_twitread_content_type_get_conf(wallycontenttypes_twitread_content_type_content_type($subtype), $conf);

  $block = new stdClass();
  $block->title = filter_xss_admin($settings['title']);
  // We don't want to render php output on preview here, because if something is
  // wrong the whole display will be borked. So we check to see if the php
  // evaluator filter is being used, and make a temporary change to the filter
  // so that we get the printed php, not the eval'ed php.
  $php_filter = FALSE;
  foreach (filter_list_format($settings['format']) as $filter) {
    if ($filter->module == 'php') {
      $php_filter = TRUE;
      break;
    }
  }
  // If a php filter is active, just print the source, but only if the current
  // user has access to the actual filter.
  if ($php_filter) {
    if (!filter_access($settings['format'])) {
      return NULL;
    }
    $block->content = '<pre>'. check_plain($settings['body']) .'</pre>';
  }
  else {
    // We also need to filter through XSS admin because <script> tags can
    // cause javascript which will interfere with our ajax.
    $block->content = filter_xss_admin(check_markup($settings['body'], $settings['format']));
  }
  return $block;
}

/**
 * Definition of the default values of the edit form.
 * 
 * @return
 *   The default values.
 */
function _wallycontenttypes_twitread_defaultformvalues() {
  $default_form_values = array();
  $default_form_values['uris'] = '';
  $default_form_values['maxitem_global'] = 10;
  $default_form_values['maxitem_byfeed'] = 10;
  $default_form_values['delta_t'] = 20;
  $default_form_values['own_schema'] = '';
  $default_form_values['mix_schema'] = 'alt';
  $default_form_values['override_theme'] = 0;
  $default_form_values['override_theme_text'] = '';
  $default_form_values['caid'] = 'twitread_'.time();

  return $default_form_values;
}

/**
 * Build a array based on an feed. Each item array is given by the
 * twitter_item_to_array function of wallytoolbox.
 * 
 * @param $twit_uri
 *   The Twitter feed URI.
 * @param $maxitem
 *   A limitation in the number of returned feeds.
 * 
 * @return
 *   The array.
 */
function _wallycontenttypes_twitread_createarrayfromuri($twit_uri, $maxitem) {
  $feed_array = array();
  
  module_load_include('lib.php', 'twitter');
  $twitter_user = new Twitter('Wally_audaxis', 'Wa11y_Audax1s!');
  /*dsm('just name');
  dsm($twitter_user->user_timeline($twit_uri));
  dsm('@ name');
  dsm($twitter_user->user_timeline('@exxxxxodus'));
  dsm('# fail');
  dsm($twitter_user->user_timeline('#fail'));*/
  
  return $twitter_user->user_timeline('exxxxxodus');
  
  /*require_once(drupal_get_path('module', 'twitter').'/twitter.inc');
  twitter_account_save('exxxxxodus');*/
  
  /*require_once(drupal_get_path('module', 'wallycontenttypes').'/libraries/twitteroauth/twitteroauth.php');
  $consumer_key = 'jWj0NclKM9AqaRZqjek4oA';
  $consumer_secret = 'tggmaR3ZT9vbLqmmYAnk1LERPgkqQZjBAIYpBieSyz0';
  $connection = new TwitterOAuth($consumer_key, $consumer_secret);
  dsm($connection);
  $oauth_callback = '';
  $request_token = $connection->getRequestToken($oauth_callback);
  dsm($request_token);*/
  
  /*switch ($connection->http_code) {
    case 200:
      // Build authorize URL and redirect user to Twitter.
      $url = $connection->getAuthorizeURL($request_token);
      $parameters = array(
        'oauth_consumer_key' => $consumer_key,
      );
      dsm($connection->get($url, $parameters));
      //header('Location: ' . $url); 
    break;
    default:
      // Show notification if something went wrong.
      dsm('Could not connect to Twitter. Refresh the page or try again later.');
  }*/
  
  /*$url = 'http://api.twitter.com/1/statuses/user_timeline.json';
  $headers = array(
    'Accept'        => '*//*',
    'Connection'    => 'close',
    'User-Agent'    => 'OAuth gem v0.3.4.1',
    'Authorization' => 'OAuth oauth_nonce=\"D8Wy8afUyHqXByAwAHHMPmdwxkgM6JHcjCatwesBF1F\", oauth_signature_method=\"HMAC-SHA1\", oauth_timestamp=\"1295867722\", oauth_consumer_key=\"'.$consumer_key.'\", oauth_token=\"'.$request_token["oauth_token"].'\", oauth_signature=\"ROxIrkb1VemnSm%2Fr5Q3Lh01rgOQ%3D\", oauth_version=\"1.0\"',
    'Host'          => 'api.local.twitter.com:9000',
  );
  dsm(drupal_http_request($url, $headers));*/
  
  /*$feed_xml = drupal_http_request($twit_uri);
  if ($feed_xml) {
    $dom = new DomDocument();
    $dom->LoadXML($feed_xml->data);
    $channels = $dom->getElementsByTagName('channel');
    if ($channels->item(0)) {
      foreach ($channels->item(0)->getElementsByTagName('item') as $item) {
        $feed_array[] = wallytoolbox_twit_item_to_array($channels->item(0), $item);
        if (sizeof($feed_array) >= $maxitem) break;
      }
    }
  }*/

  return $feed_array;
}

/**
 * Select the most recent feed of every Twitter feed list that matches the
 * schema.
 * 
 * @param $schema_elem
 *   The schema containing the IDs of the allowed Twitter feed lists.
 * @param $feed_array
 *   Array of all the Twitter feed lists.
 * 
 * @return
 *   The array containing the selected feeds and their respective Twitter
 *   feed list ID.
 */
function _wallycontenttypes_twitread_getavailableitemsfromschema($schema_elem, $feed_array) {
  $available_items = array();
  $schema_elems = explode('-', $schema_elem);
  for ($i=0; $i<sizeof($feed_array); $i++) {
    if (in_array($i+1, $schema_elems) || $schema_elems[0] == 'any') {
      if (is_array($feed_array[$i][0]) && isset($feed_array[$i][0]) && !empty($feed_array[$i][0])) {
        $available_items[] = $feed_array[$i][0] + array('arrayID' => $i);
      }
    }
  }
  
  return $available_items;
}

/**
 * Select the most recent feed (based on the publication date) in the
 * given array.
 * 
 * @param $available_items
 *   The array containing the available Twitter feeds.
 * 
 * @return
 *   The ID of the Twitter feed list containing the most recent feed.
 */
function _wallycontenttypes_twitread_getmostrecentfromavailables($available_items) {
  $most_recent = array( 'Package' => array('PublicationDate' => '1970-01-01T00:00:00+00:00'));
  
  foreach ($available_items as $available_item) {
    if (strtotime($available_item['Package']['PublicationDate']) > strtotime($most_recent['Package']['PublicationDate']))
      $most_recent = $available_item;
  }
  
  return $most_recent['arrayID'];
}
