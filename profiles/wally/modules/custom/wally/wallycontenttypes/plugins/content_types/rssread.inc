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
  'defaults' => _wallycontenttypes_rssread_defaultformvalues(),
  'js' => array('misc/autocomplete.js', 'misc/textarea.js', 'misc/collapse.js'),
  // Make sure the edit form is only used for some subtypes.
  'edit form' => '',
  'add form' => '',
  'edit text' => t('Edit'),
  'all contexts' => TRUE,
);

/**
 * Building the configuration form of a RSS reader.
 */
function wallycontenttypes_rssread_content_type_edit_form(&$form, &$form_state) {
  $settings = wallycontenttypes_rssread_content_type_get_conf($form_state['subtype'], $form_state['conf']);
  $form_state['settings'] = $settings;
  $creation_infos = explode('_', $form_state['conf']['caid']);
  $creation_date = $creation_infos[1];

  if ($settings['rssread_type'] == 'fixed' && ($creation_date+1)>=time()) {
    return; // no form for this case.
  }
              
  $form['uri'] = array (
    '#type'             => 'textfield',
    '#title'            => t('URL of the RSS FEED'),
    '#default_value'    => $settings['uri'],
    '#required'         => TRUE,
  );

  $form['maxitem'] = array (
    '#type'             => 'textfield',
    '#title'            => t('Max Item to fetch'),
    '#default_value'    => $settings['maxitem'],
    '#required'         => TRUE,
  );
  
  $form['delta_t'] = array (
    '#type'             => 'textfield',
    '#title'            => t('Refresh time (minutes)'),
    '#default_value'    => $settings['delta_t'],
    '#required'         => TRUE,
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
  
  if (!user_access('administer rssread content') || $settings['rssread_type'] == 'fixed') {
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
 * Validation for the configuration form of a RSS reader.
 */
function wallycontenttypes_rssread_content_type_edit_form_validate(&$form, &$form_state) {
  if (isset($form['uri']['#value'])) {
    $uri_response = drupal_http_request($form['uri']['#value']);
    if ($uri_response->status_message != 'OK') {
      form_set_error('uri', t('The URI '.check_plain($uri_string).' doesn\'t respond.'));
    }
  }

  if (isset($form['maxitem']['#value']) && !is_numeric($form['maxitem']['#value'])) {
    form_set_error('maxitem', t('The max item isn\'t a numeric value'));
  } elseif (isset($form['maxitem']['#value']) && $form['maxitem']['#value'] <= 0) {
    form_set_error('maxitem', t('The max item value must be greater than 0'));
  }
  
  if (isset($form['delta_t']['#value']) && !is_numeric($form['delta_t']['#value'])) {
    form_set_error('delta_t', t('The refresh time isn\'t a numeric value'));
  } elseif (isset($form['delta_t']['#value']) && $form['delta_t']['#value'] < 0) {
    form_set_error('delta_t', t('The refresh time must be greater or equal to 0'));
  }
}

/**
 * Submitting the configuration form of a RSS reader.
 */
function wallycontenttypes_rssread_content_type_edit_form_submit(&$form, &$form_state) {
  if ($form_state['settings']['rssread_type'] == 'fixed') {
    _wallycontenttypes_rssread_content_type_edit_save($form_state['settings']['content'], $form_state);
  } else if (!empty($form_state['values']['reusable'])) {
    // If the 'reusable' checkbox was checked, we will create a new
    // rssread content and give it the proper values.
    $content = wallycontenttypes_newctoolsexport('wallycontenttypes_rssread_content', 'rssread');
    $content->name = $form_state['values']['name'];
    _wallycontenttypes_rssread_content_type_edit_save($content, $form_state);
    $form_state['conf']['name'] = $content->name;
  } else {
    // Otherwise, just save values into $conf normally.
    cache_clear_all($cid = $form['caid']['#value'], $table = 'cache');
    $form_state['conf']['uri']=$form['uri']['#value'];
    $form_state['conf']['override_theme']=$form['override_theme']['#value'];
    $form_state['conf']['override_theme_text']=$form['override_theme_text']['#value'];
    $form_state['conf']['maxitem']=$form['maxitem']['#value'];
    $form_state['conf']['delta_t'] = $form['delta_t']['#value'];
    $form_state['conf']['caid'] = $form['caid']['#value'];
  }
}

/**
 * Export the configuration of the reusable block.
 */
function _wallycontenttypes_rssread_content_type_edit_save(&$content, $form_state) {
  // Apply updates to the content object.
  $content->category = $form_state['values']['category'];
  $content->admin_title = $form_state['values']['name'];
  $content->admin_description = $form_state['values']['admin_description'];
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    if (isset($form_state['values'][$key])) {
      $content->settings[$key] = $form_state['values'][$key];
    }
  }

  ctools_export_crud_save('wallycontenttypes_rssread_content', $content);
}

/**
 * Return all rssread content types available.
 */
function wallycontenttypes_rssread_content_type_content_types() {
  ctools_include('export');
  $types = array();
  $types['rssread'] = _wallycontenttypes_rssread_default_ctools_content_type();
  
  foreach (ctools_export_crud_load_all('wallycontenttypes_rssread_content') as $name => $content) {
    $types[$name] = _wallycontenttypes_rssread_ctools_content_type($content);
  }
  return $types;
}

/**
 * Return the rssread content types with the specified $subtype_id.
 */
function wallycontenttypes_rssread_content_type_content_type($subtype_id) {
  if ($subtype_id == 'rssread') {
    return _wallycontenttypes_rssread_default_ctools_content_type();
  }
  else {
    ctools_include('export');
    $content = ctools_export_crud_load('wallycontenttypes_rssread_content', $subtype_id);
    if ($content) {
      return _wallycontenttypes_rssread_ctools_content_type($content);
    }
  }
}

/**
 * Settings for the default rssread content type.
 *
 * The default is the one that allows the user to actually create a type.
 */
function _wallycontenttypes_rssread_default_ctools_content_type() {
  $info = array(
    'name' => 'rssread',
    'title' => t('RSS Reader'),
    'category' => t('Wally Tools'),
    'edit form' => 'wallycontenttypes_rssread_content_type_edit_form',
    'description' => t('Fetch a RSS feed'),
    'all contexts' => TRUE,
    'defaults' => _wallycontenttypes_rssread_defaultformvalues(),
  );

  return $info;
}

/**
 * Return an info array for a specific rssread content type.
 */
function _wallycontenttypes_rssread_ctools_content_type($content) {
  $info = array(
    'name' => $content->name,
    'title' => check_plain($content->admin_title),
    'description' => check_plain($content->admin_description),
    'category' => $content->category ? check_plain($content->category) : t('Wally reusable blocks'),
    'edit form' => 'wallycontenttypes_rssread_content_type_edit_form',
    'all contexts' => TRUE,
    'icon' => 'icon_block_custom.png',
    // Store this here to make it easy to access.
    'content' => $content,
  );

  return $info;
}

/**
 * Rendering of the RSS reader ctools.
 */
function wallycontenttypes_rssread_content_type_render($subtype, $conf, $panel_args, $context) {
  $settings = wallycontenttypes_rssread_content_type_get_conf(wallycontenttypes_rssread_content_type_content_type($subtype), $conf);
  
  $feed = array();
  $block = new stdClass();
  $block->module = 'wallycontenttypes';
  $block->title = 'RSS reader.';
  $cid = $conf['caid'];
  $cached_feed = cache_get($cid);
  if ($cached_feed == 0 || !isset($cached_feed) || empty($cached_feed)) {
    $rss_uri = $settings['uri'];
    $override_theme = $settings['override_theme'];
    $override_theme_text = $settings['override_theme_text'];
    $maxitem = $settings['maxitem'];

    $feed = _wallycontenttypes_rssread_createarrayfromuri($rss_uri, $maxitem);

    cache_set($cid, $feed, 'cache', time() + mktime(0, $settings['delta_t']) + 1);
  } else {
    $feed = $cached_feed->data;
  }

  $block->content = theme("wallyct_rss", $feed, $settings); 
  return $block;
}

/**
 * Given a $subtype and a $conf, return the actual settings to use.
 *
 * The actual settings may be stored directly in the pane or this may
 * be a pointer to re-usable content that may be in the database or in
 * an export. We have to determine from the subtype whether or not it
 * is local or shared rssred content.
 * 
 * @param $subtype
 *   The given subtype.
 * @param $conf
 *   The configuration informations.
 * 
 * @return
 *   The usable settings.
 */
function wallycontenttypes_rssread_content_type_get_conf($subtype, $conf) {
  if ($subtype['name'] != 'rssread') {
    $settings = $subtype['content']->settings;
    $settings['rssread_type'] = 'fixed';
    $settings['content'] = $subtype['content'];
  } else {
    // This means they created it as rssread content and then set it as
    // reusable. Since we're not allowed to change the subtype, we're
    // still stored as though we are local, but are pointing off to
    // non-local.
    if (!empty($conf['name'])) {
      ctools_include('export');
      $content = ctools_export_crud_load('wallycontenttypes_rssread_content', $conf['name']);
      if ($content) {
        $settings = $content->settings;
      } else {
        $content = wallycontenttypes_newctoolsexport('wallycontenttypes_rssread_content', 'rssread');
        $content->name = $conf['name'];
        $settings = _wallycontenttypes_rssread_defaultformvalues();
      }
      $settings['rssread_type'] = 'fixed';
      $settings['content'] = $content;
    } else {
      // This means that it is created as rssread and has not been set to
      // reusable.
      $settings = $conf;
      $settings['rssread_type'] = 'local';
    }
  }

  return $settings;
}

/**
 * Callback to provide the administrative title of the rssread content.
 */
function wallycontenttypes_rssread_content_type_admin_title($subtype, $conf) {
  $settings = wallycontenttypes_rssread_content_type_get_conf(wallycontenttypes_rssread_content_type_content_type($subtype), $conf);

  $output = t('RSS reader');
  $title = !empty($settings['admin_title']) ? $settings['admin_title'] : $settings['title'];
  if ($title) {
    if ($settings['rssread_type'] != 'fixed') {
      $output = t('RSS reader: @title', array('@title' => $title));
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
function wallycontenttypes_rssread_content_type_admin_info($subtype, $conf) {
  $settings = wallycontenttypes_rssread_content_type_get_conf(wallycontenttypes_rssread_content_type_content_type($subtype), $conf);

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
function _wallycontenttypes_rssread_defaultformvalues() {
  $default_form_values = array();
  $default_form_values['uri'] = '';
  $default_form_values['maxitem'] = 10;
  $default_form_values['delta_t'] = 20;
  $default_form_values['override_theme'] = 0;
  $default_form_values['override_theme_text'] = '';
  $default_form_values['caid'] = 'rsssimple_'.time();

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
