<?php
/*
  Plugin declaration function - returns a plugin definition array that
 * describes the content type.
 */
 
/**
 * Definition of the RSS reader ctools.
 */
function wallycontenttypes_rssread_ctools_content_types() {
  return array(
    'single' => TRUE,
    'title' => t('Simple RSS Reader'),
    'icon' => 'icon_node.png',
    'description' => t('Fetch a RSS feed'),
//      'required context' => new ctools_context_required(t('Node'), 'node'),
    'category' => t('Wally Tools'),
    'defaults' => array(),
  );
}

/**
 * Building the configuration form of a RSS reader.
 */
function wallycontenttypes_rssread_content_type_edit_form(&$form, &$form_state) {
  $conf = $form_state['conf'];
  $default_form_values = _wallyconttentypes_rssread_getdefaultformvalues($conf);
              
  $form['uri'] = array (
    '#type'             => 'textfield',
    '#title'            => t('URL of the RSS FEED'),
    '#default_value'    => $default_form_values['uri'],
    '#required'         => TRUE,
  );

  $form['maxitem'] = array (
    '#type'             => 'textfield',
    '#title'            => t('Max Item to fetch'),
    '#default_value'    => $default_form_values['maxitem'],
    '#required'         => TRUE,
  );
  
  $form['delta_t'] = array (
    '#type'             => 'textfield',
    '#title'            => t('Refresh time (minutes)'),
    '#default_value'    => $default_form_values['delta_t'],
    '#required'         => TRUE,
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
 * Validation for the configuration form of a RSS reader.
 */
function wallycontenttypes_rssread_content_type_edit_form_validate(&$form, &$form_state) {
  $uri_response = drupal_http_request($form['uri']['#value']);
  if ($uri_response->status_message != 'OK') {
    form_set_error('uri', t('The URI '.check_plain($uri_string).' doesn\'t respond.'));
  }

  if (!is_numeric($form['maxitem']['#value'])) {
    form_set_error('maxitem', t('The max item isn\'t a numeric value'));
  } elseif ($form['maxitem']['#value'] <= 0) {
    form_set_error('maxitem', t('The max item value must be greater than 0'));
  }
  
  if (!is_numeric($form['delta_t']['#value'])) {
    form_set_error('delta_t', t('The refresh time isn\'t a numeric value'));
  } elseif ($form['delta_t']['#value'] < 0) {
    form_set_error('delta_t', t('The refresh time must be greater or equal to 0'));
  }
}

/**
 * Submitting the configuration form of a RSS reader.
 */
function wallycontenttypes_rssread_content_type_edit_form_submit(&$form, &$form_state) {
  cache_clear_all($cid = $form['cid']['#value'], $table = 'cache');
  $form_state['conf']['uri']=$form['uri']['#value'];
  $form_state['conf']['override_theme']=$form['override_theme']['#value'];
  $form_state['conf']['override_theme_text']=$form['override_theme_text']['#value'];
  $form_state['conf']['maxitem']=$form['maxitem']['#value'];
  $form_state['conf']['delta_t'] = $form['delta_t']['#value'];
  $form_state['conf']['last_build'] = $form['last_build']['#value'];
  $form_state['conf']['cid'] = $form['cid']['#value'];
}

/**
 * Rendering of the RSS reader ctools.
 */
function wallycontenttypes_rssread_content_type_render($subtype, $conf, $panel_args, $context) {
  $feed = array();
  $block = new stdClass();
  $block->module = 'wallycontenttypes';
  $block->title = 'RSS reader.';
  $cid = $conf['cid'];
  
  if ((time() - $conf['last_build']) > mktime(0, $conf['delta_t'])) {
    $rss_uri = $conf['uri'];
    $override_theme = $conf['override_theme'];
    $override_theme_text = $conf['override_theme_text'];
    $maxitem = $conf['maxitem'];

    $feed = _wallycontenttypes_rssread_createarrayfromuri($rss_uri, $maxitem);
    
    $conf['last_build'] = time();
    cache_set($cid, $feed);
  } else {
    $cached_feed = cache_get($cid);
    $feed = $cached_feed->data;
  }

  $block->content = theme("wallyct_rss", $feed, $conf); 
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
function _wallyconttentypes_rssread_getdefaultformvalues($conf) {
  $default_form_values = array();
  if (!empty($conf)) {
    $default_form_values = $conf;
  } else {
    $default_form_values['uri'] = '';
    $default_form_values['maxitem'] = 10;
    $default_form_values['delta_t'] = 20;
    $default_form_values['override_theme'] = 0;
    $default_form_values['override_theme_text'] = '';
    $default_form_values['cid'] = 'rsssimple_'.time();
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
function _wallycontenttypes_rssread_createarrayfromuri($rss_uri, $maxitem) {
  $feed_array = array();
  $feed_xml = drupal_http_request($rss_uri);
  if ($feed_xml) {
    $dom = new DomDocument();
    $dom->LoadXML($feed_xml->data);
    $channels = $dom->getElementsByTagName('channel');
    if ($channels->item(0)) {
      foreach ($channels->item(0)->getElementsByTagName('item') as $item) {
        $feed_array[] = wallytoolbox_rss_item_to_array($channels->item(0), $item);
        if (sizeof($feed_array) >= $maxitem) break;
      }
    }
  }

  return $feed_array;
}
