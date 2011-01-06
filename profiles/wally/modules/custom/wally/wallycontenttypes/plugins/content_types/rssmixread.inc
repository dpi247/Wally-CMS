<?php
/*
  Plugin declaration function - returns a plugin definition array that
 * describes the content type.
 */
 
/**
 * Definition of the RSS mix reader ctools.
 */
function wallycontenttypes_rssmixread_ctools_content_types() {  
  return array(
    'single' => TRUE,
    'title' => t('RSS Mix Reader'),
    'icon' => 'icon_node.png',
    'description' => t('Fetch some RSS feeds and mix them together in a single feed'),
//      'required context' => new ctools_context_required(t('Node'), 'node'),
    'category' => t('Wally Tools'),
    'defaults' => array(),
  );
}

/**
 * Building the configuration form of a RSS mix reader.
 */
function wallycontenttypes_rssmixread_content_type_edit_form(&$form, &$form_state) {    
  $conf = $form_state['conf'];
  $default_form_values = _wallyconttentypes_rssmixread_getdefaultformvalues($conf);
  
  $form['uris'] = array (
    '#type'             => 'textarea',
    '#id'               => 'uri_area',
    '#title'            => t('URL of the RSS feeds'),
    '#description'      => t('Please enter one URL by line.'),
    '#default_value'    => $default_form_values['uris'],
    '#required'         => TRUE,
  );

  $form['maxitem_global'] = array (
    '#type'             => 'textfield',
    '#title'            => t('Max item displayed in the final mixed feed'),
    '#default_value'    => $default_form_values['maxitem_global'],
    '#required'         => TRUE,
  );
  
  $form['maxitem_byfeed'] = array (
    '#type'             => 'textfield',
    '#title'            => t('Max item to fetch by feed'),
    '#default_value'    => $default_form_values['maxitem_byfeed'],
    '#required'         => TRUE,
  );
  
  $form['delta_t'] = array (
    '#type'             => 'textfield',
    '#title'            => t('Refresh time (minutes)'),
    '#default_value'    => $default_form_values['delta_t'],
    '#required'         => TRUE,
  );
  
  $mix_schemas = array(
    '1,2,3'             => 'Alternate',
    'any'               => 'Most Recent'
  );
  
  $form['mix_schema'] = array(
    '#type'             => 'select',
    '#title'            => t('Schema used for mixing the feeds'),
    '#default_value'    => $default_form_values['mix_schema'],
    '#options'          => $mix_schemas,
  );
  
  $form['own_schema'] = array (
    '#type'             => 'textfield',
    '#title'            => t('Define your own schema used for mixing the feeds'),
    '#description'      => t('Examples:
        1,2,3 / 1-2,3,2,1-3,2,2-3 / any,2,1-2,3,any,2 / ...
      '),
    '#default_value'    => $default_form_values['own_schema'],
    '#required'         => FALSE,
  );

  $form['override_theme'] = array (
    '#type'             => 'checkbox',
    '#default_value'    => $default_form_values['override_theme'],
    '#id'               =>  'override-theme-checkbox',
    '#title'            => 'Override Theming Function',
  );

  $form['override_theme_text'] = array (
    '#type'             => 'textfield',
    '#default_value'    => $default_form_values['override_theme_text'],
    '#id'               => 'override-theme-textfield',
    '#dependency'       => array('override-theme-checkbox'=>array('1')),
    '#dependency_type'  => 'disable',
    '#process'          => array('ctools_dependent_process'),
    '#title'            => 'Theming Function Name',
  );
  
  $form['last_build'] = array (
    '#type'             => 'hidden',
    '#value'            => 0,
  );
  
  $form['cid'] = array (
    '#type'             => 'hidden',
    '#value'            => $default_form_values['cid'],
  );

  return $form;  
}

/**
 * Validation for the configuration form of a RSS mix reader.
 */
function wallycontenttypes_rssmixread_content_type_edit_form_validate(&$form, &$form_state) {
  $uri_strings = explode("\n", str_replace(array("\n", "\r\n"), "\n", trim($form['uris']['#value'])));
  foreach ($uri_strings as $uri_string) {
    $uri_response = drupal_http_request($uri_string);
    if ($uri_response->status_message != 'OK') {
      form_set_error('uris', t('The URI '.check_plain($uri_string).' doesn\'t respond.'));
    }
  }

  if (!is_numeric($form['maxitem_global']['#value'])) {
    form_set_error('maxitem_global', t('The total max item isn\'t a numeric value'));
  }
  
  if (!is_numeric($form['maxitem_byfeed']['#value'])) {
    form_set_error('maxitem_byfeed', t('The max item by feed isn\'t a numeric value'));
  }
  
  if (!is_numeric($form['delta_t']['#value'])) {
    form_set_error('delta_t', t('The refresh time isn\'t a numeric value'));
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
 * Submitting the configuration form of a RSS mix reader.
 */
function wallycontenttypes_rssmixread_content_type_edit_form_submit(&$form, &$form_state) {
  cache_clear_all($cid = $form['cid']['#value'], $table = 'cache');
  $form_state['conf']['uris'] = $form['uris']['#value'];
  $form_state['conf']['maxitem_global'] = $form['maxitem_global']['#value'];
  $form_state['conf']['maxitem_byfeed'] = $form['maxitem_byfeed']['#value'];
  $form_state['conf']['delta_t'] = $form['delta_t']['#value'];
  $form_state['conf']['own_schema'] = $form['own_schema']['#value'];
  $form_state['conf']['mix_schema'] = $form['mix_schema']['#value'];
  $form_state['conf']['override_theme'] = $form['override_theme']['#value'];
  $form_state['conf']['override_theme_text'] = $form['override_theme_text']['#value'];
  $form_state['conf']['last_build'] = $form['last_build']['#value'];
  $form_state['conf']['cid'] = $form['cid']['#value'];
}

/**
 * Rendering of the RSS mix reader ctools.
 */
function wallycontenttypes_rssmixread_content_type_render($subtype, $conf, $panel_args, $context) {
  $feed = array();
  $block = new stdClass();
  $block->module = 'wallycontenttypes';
  $block->title = 'RSS mix reader.';
  $cid = $conf['cid'];
  
  if ((time() - $conf['last_build']) > mktime(0, $conf['delta_t'])) {
    $rss_uris = explode("\n", str_replace(array("\n", "\r\n"), "\n", trim($conf['uris'])));
    $override_theme = $conf['override_theme'];
    if (isset($conf['own_schema']) && !empty($conf['own_schema'])) {
      $mix_schema = explode(',', $conf['own_schema']);
    } else {
      $mix_schema = explode(',', $conf['mix_schema']);
    }
    $override_theme_text = $conf['override_theme_text'];
    $maxitem_global = $conf['maxitem_global'];
    $maxitem_byfeed = $conf['maxitem_byfeed'];

    foreach ($rss_uris as $rss_uri) {
      $feed_array[] = _wallycontenttypes_rssmixread_createarrayfromuri($rss_uri, $maxitem_byfeed);
    }
    
    for ($i=0; $i<10; $i++) {
      $schema_elem = $mix_schema[$i%sizeof($mix_schema)];
      $available_items = _wallycontenttypes_rssmixread_getavailableitemsfromschema($schema_elem, $feed_array);
      $most_recent_index = _wallycontenttypes_rssmixread_getmostrecentfromavailables($available_items);
      $item = array_shift($feed_array[$most_recent_index]);
      array_push($feed, $item);
    }
    
    $conf['last_build'] = time();
    cache_set($cid, $feed);
  } else {
    $cached_feed = cache_get($cid);
    $feed = $cached_feed->data;
  }

  $block->content = theme("wallyct_rssmix",$feed, $conf); 
  return $block;
}

/**
 * Return the default values of the configuration form, allow the user
 * to update settings without having to reenter all values.
 * 
 * @param $conf
 *   The configuration informations.
 * 
 * @return
 *   The default values.
 */
function _wallyconttentypes_rssmixread_getdefaultformvalues($conf) {
  $default_form_values = array();
  if (!empty($conf)) {
    $default_form_values = $conf;
  } else {
    $default_form_values['uris'] = '';
    $default_form_values['maxitem_global'] = 0;
    $default_form_values['maxitem_byfeed'] = 0;
    $default_form_values['delta_t'] = 20;
    $default_form_values['own_schema'] = '';
    $default_form_values['mix_schema'] = '1,2,3';
    $default_form_values['override_theme'] = 0;
    $default_form_values['override_theme_text'] = '';
    $default_form_values['cid'] = 'rssmix_'.time();
  }
  return $default_form_values;
}

/**
 * Build a array based on an feed. Each item array is given by the
 * rss_item_to_array function of wallytoolbox.
 * 
 * @param $rss_uri
 *   The feed URI.
 * @param $maxitem
 *   A limitation in the number of returned feeds.
 * 
 * @return
 *   The array.
 */
function _wallycontenttypes_rssmixread_createarrayfromuri($rss_uri, $maxitem) {
  $feed_xml = drupal_http_request($rss_uri);
  if ($feed_xml) {
    $dom = new DomDocument();
    $dom->LoadXML($feed_xml->data);
    $feed_array = array();
    $channels = $dom->getElementsByTagName('channel');
    if ($channels->item(0)) {
      foreach ($channels->item(0)->getElementsByTagName('item') as $item) {
        $feed_array[] = wallytoolbox_rss_item_to_array($channels->item(0), $item);
        if (sizeof($feed_array) == $maxitem) break;
      }
    }
  }

  return $feed_array;
}

/**
 * Select the most recent feed of every RSS feed list that matches the
 * schema.
 * 
 * @param $schema_elem
 *   The schema containing the IDs of the allowed RSS feed lists.
 * @param $feed_array
 *   Array of all the RSS feed lists.
 * 
 * @return
 *   The array containing the selected feeds and their respective RSS
 *  feed list ID.
 */
function _wallycontenttypes_rssmixread_getavailableitemsfromschema($schema_elem, $feed_array) {
  $available_items = array();
  $schema_elems = explode('-', $schema_elem);

  for ($i=0; $i<sizeof($feed_array); $i++) {
    if (in_array($i+1, $schema_elems) || $schema_elems[0] == 'any')
      $available_items[] = $feed_array[$i][0] + array('arrayID' => $i);
  }
  
  return $available_items;
}

/**
 * Select the most recent feed (based on the publication date) in the
 * given array.
 * 
 * @param $available_items
 *   The array containing the available feeds.
 * 
 * @return
 *   The ID of the RSS feed list containing the most recent feed.
 */
function _wallycontenttypes_rssmixread_getmostrecentfromavailables($available_items) {
  $most_recent = array( 'Package' => array('PublicationDate' => '1970-01-01T00:00:00+00:00'));
  
  foreach ($available_items as $available_item) {
    if (strtotime($available_item['Package']['PublicationDate']) > strtotime($most_recent['Package']['PublicationDate']))
      $most_recent = $available_item;
  }
  
  return $most_recent['arrayID'];
}
