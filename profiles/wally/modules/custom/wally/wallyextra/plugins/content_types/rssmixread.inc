<?php
/*
 * Plugin declaration function - returns a plugin definition array that
 * describes the content type.
 */
 
/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'no title override' => FALSE,
  'defaults' => _wallyextra_rssmixread_defaultformvalues(),
  'js' => array('misc/autocomplete.js', 'misc/textarea.js', 'misc/collapse.js'),
  // Make sure the edit form is only used for some subtypes.
  'edit form' => '',
  'add form' => '',
  'edit text' => t('Edit'),
  'all contexts' => TRUE,
);

/**
 * Building the configuration form of a RSS mix reader.
 */
function wallyextra_rssmixread_content_type_edit_form(&$form, &$form_state) {
  $settings = wallyextra_rssmixread_content_type_get_conf($form_state['subtype'], $form_state['conf']);
  $form_state['settings'] = $settings;
  $creation_infos = explode('_', $form_state['conf']['caid']);
  $creation_date = $creation_infos[1];

  if ($settings['rssmixread_type'] == 'fixed' && ($creation_date+1)>=time()) {
    return; // no form for this case.
  }
  
  $form['uris'] = array (
    '#type'             => 'textarea',
    '#id'               => 'uri_area',
    '#title'            => t('URL of the RSS feeds'),
    '#description'      => t('Please enter one URL by line.'),
    '#default_value'    => $settings['uris'],
    '#required'         => TRUE,
  );

  $form['maxitem_global'] = array (
    '#type'             => 'textfield',
    '#title'            => t('Max item displayed in the final mixed feed'),
    '#default_value'    => $settings['maxitem_global'],
    '#required'         => TRUE,
  );
  
  $form['maxitem_byfeed'] = array (
    '#type'             => 'textfield',
    '#title'            => t('Max item to fetch by feed'),
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
    '#title'            => t('Schema used for mixing the feeds'),
    '#default_value'    => $settings['mix_schema'],
    '#options'          => $mix_schemas,
  );
  
  $form['own_schema'] = array (
    '#type'             => 'textfield',
    '#title'            => t('Define your own schema used for mixing the feeds'),
    '#description'      => t('Examples:
        1,2,3 / 1-2,3,2,1-3,2,2-3 / any,2,1-2,3,any,2 / ... see <a href="http://rossel.audaxis.com/xwiki/wiki/drupal/view/Main/User_RSS_mix" target="_new">RSS mix documentation</a>.
      '),
    '#default_value'    => $settings['own_schema'],
    '#required'         => FALSE,
  );
  
  $form['get_distant_img'] = array (
    '#type'             => 'checkbox',
    '#default_value'    => $settings['get_distant_img'],
    '#title'            => 'Copy RSS feeds images to locale folder.',
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
  
  if (!user_access('administer rssmixread content') || $settings['rssmixread_type'] == 'fixed') {
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
 * Validation for the configuration form of a RSS mix reader.
 */
function wallyextra_rssmixread_content_type_edit_form_validate(&$form, &$form_state) {
  if (isset($form_state['values']['uris'])) {
    $uri_strings = explode("\n", str_replace(array("\n", "\r\n"), "\n", trim($form_state['values']['uris'])));
    foreach ($uri_strings as $uri_string) {
      $uri_response = drupal_http_request($uri_string);
      if ($uri_response->status_message != 'OK') {
        form_set_error('uris', t('The URI '.check_plain($uri_string).' doesn\'t respond.'));
      }
    }
  }

  if (isset($form_state['values']['maxitem_global']) && !is_numeric($form_state['values']['maxitem_global'])) {
    form_set_error('maxitem_global', t('The total max item isn\'t a numeric value'));
  } elseif (isset($form_state['values']['maxitem_global']) && $form_state['values']['maxitem_global'] <= 0) {
    form_set_error('maxitem_global', t('The total max item value must be greater than 0'));
  }
  
  if (isset($form_state['values']['maxitem_byfeed']) && !is_numeric($form_state['values']['maxitem_byfeed'])) {
    form_set_error('maxitem_byfeed', t('The max item by feed isn\'t a numeric value'));
  } elseif (isset($form_state['values']['maxitem_byfeed']) && $form_state['values']['maxitem_byfeed'] <= 0) {
    form_set_error('maxitem_byfeed', t('The max item by feed value must be greater than 0'));
  }
  
  if (isset($form_state['values']['delta_t']) && !is_numeric($form_state['values']['delta_t'])) {
    form_set_error('delta_t', t('The refresh time isn\'t a numeric value'));
  } elseif (isset($form_state['values']['delta_t']) && $form_state['values']['delta_t'] < 0) {
    form_set_error('delta_t', t('The refresh time must be greater or equal to 0'));
  }
  
  if ($form_state['values']['own_schema'] && !empty($form_state['values']['own_schema'])) {
    $temp_schema = explode(',', $form_state['values']['own_schema']);
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
function wallyextra_rssmixread_content_type_edit_form_submit(&$form, &$form_state) {
  if ($form_state['settings']['rssmixread_type'] == 'fixed') {
    _wallyextra_rssmixread_content_type_edit_save($form_state['settings']['content'], $form_state);
  } else if (!empty($form_state['values']['reusable'])) {
    // If the 'reusable' checkbox was checked, we will create a new
    // rssmixread content and give it the proper values.
    $content = wallyextra_newctoolsexport('wallyextra_rssmixread_content', 'rssmixread');
    _wallyextra_rssmixread_content_type_edit_save($content, $form_state);
    $form_state['conf']['name'] = $content->name;
  } else {
    // Otherwise, just save values into $conf normally.
    cache_clear_all($form_state['values']['caid'], $table = 'cache');
    $form_state['conf']['uris'] = $form_state['values']['uris'];
    $form_state['conf']['maxitem_global'] = $form_state['values']['maxitem_global'];
    $form_state['conf']['maxitem_byfeed'] = $form_state['values']['maxitem_byfeed'];
    $form_state['conf']['delta_t'] = $form_state['values']['delta_t'];
    $form_state['conf']['mix_schema'] = $form_state['values']['mix_schema'];
    $form_state['conf']['own_schema'] = $form_state['values']['own_schema'];
    $form_state['conf']['get_distant_img'] = $form_state['values']['get_distant_img'];
    $form_state['conf']['override_theme'] = $form_state['values']['override_theme'];
    $form_state['conf']['override_theme_text'] = $form_state['values']['override_theme_text'];
    $form_state['conf']['caid'] = $form_state['values']['caid'];
  }
}

/**
 * Export the configuration of the reusable block.
 */
function _wallyextra_rssmixread_content_type_edit_save(&$content, $form_state) {
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

  ctools_export_crud_save('wallyextra_rssmixread_content', $content);
}

/**
 * Return all rssmixread content types available.
 */
function wallyextra_rssmixread_content_type_content_types() {
  ctools_include('export');
  $types = array();
  $types['rssmixread'] = _wallyextra_rssmixread_default_ctools_content_type();
  
  foreach (ctools_export_crud_load_all('wallyextra_rssmixread_content') as $name => $content) {
    $types[$name] = _wallyextra_rssmixread_ctools_content_type($content);
  }
  return $types;
}

/**
 * Return the rssmixread content types with the specified $subtype_id.
 */
function wallyextra_rssmixread_content_type_content_type($subtype_id) {
  if ($subtype_id == 'rssmixread') {
    return _wallyextra_rssmixread_default_ctools_content_type();
  }
  else {
    ctools_include('export');
    $content = ctools_export_crud_load('wallyextra_rssmixread_content', $subtype_id);
    if ($content) {
      return _wallyextra_rssmixread_ctools_content_type($content);
    }
  }
}

/**
 * Settings for the default rssmixread content type.
 *
 * The default is the one that allows the user to actually create a type.
 */
function _wallyextra_rssmixread_default_ctools_content_type() {
  $info = array(
    'name' => 'rssmixread',
    'title' => t('RSS Mix Reader'),
    'category' => t('Wally Extra'),
    'edit form' => 'wallyextra_rssmixread_content_type_edit_form',
    'description' => t('Fetch some RSS feeds and mix them together in a single feed'),
    'all contexts' => TRUE,
    'defaults' => _wallyextra_rssmixread_defaultformvalues(),
  );

  return $info;
}

/**
 * Return an info array for a specific rssmixread content type.
 */
function _wallyextra_rssmixread_ctools_content_type($content) {
  $info = array(
    'name' => $content->name,
    'title' => check_plain($content->admin_title),
    'description' => check_plain($content->admin_description),
    'category' => $content->category ? check_plain($content->category) : t('Wally reusable blocks'),
    'edit form' => 'wallyextra_rssmixread_content_type_edit_form',
    'all contexts' => TRUE,
    'icon' => 'icon_block_custom.png',
    // Store this here to make it easy to access.
    'content' => $content,
  );

  return $info;
}

/**
 * Rendering of the RSS mix reader ctools.
 */
function wallyextra_rssmixread_content_type_render($subtype, $conf, $panel_args, $context) {
  $settings = wallyextra_rssmixread_content_type_get_conf(wallyextra_rssmixread_content_type_content_type($subtype), $conf);

  $feed = array();
  $block = new stdClass();
  $block->module = 'wallyextra';
  $block->title = 'RSS mix reader.';
  $cid = $settings['caid'];
  $cached_feed = cache_get($cid);
  if (!is_object($cached_feed) || !isset($cached_feed) || empty($cached_feed) || true) {
    $rss_uris = explode("\n", str_replace(array("\n", "\r\n"), "\n", trim($settings['uris'])));
    $override_theme = $settings['override_theme'];
    if (isset($settings['own_schema']) && !empty($settings['own_schema'])) {
      $mix_schema = explode(',', $settings['own_schema']);
    } else {
      if ($settings['mix_schema'] == 'alt') {
        $mix_schema = array();
        for ($i=0; $i<sizeof($rss_uris); $i++) $mix_schema[] = $i+1;
      } else {
        $mix_schema = explode(',', $settings['mix_schema']);
      }
    }
    $override_theme_text = $settings['override_theme_text'];
    $maxitem_global = $settings['maxitem_global'];
    $maxitem_byfeed = $settings['maxitem_byfeed'];

    foreach ($rss_uris as $rss_uri) {
      $feed_array[] = _wallyextra_rssmixread_createarrayfromuri($rss_uri, $maxitem_byfeed, $settings);
    }
    
    for ($i=0; $i<$maxitem_global; $i++) {
      $schema_elem = $mix_schema[$i%sizeof($mix_schema)];
      $available_items = _wallyextra_rssmixread_getavailableitemsfromschema($schema_elem, $feed_array);
      $most_recent_index = _wallyextra_rssmixread_getmostrecentfromavailables($available_items);
      $item = array_shift($feed_array[$most_recent_index]);
      array_push($feed, $item);
    }
    
    cache_set($cid, $feed, 'cache', time() + mktime(0, $settings['delta_t']) + 1);
  } else {
    $feed = $cached_feed->data;
  }

  if ($settings['override_theme_text'] != "") {// && function_exists($settings['override_theme_text'])){
    $block->content = theme($settings['override_theme_text'], $subtype, $context, $feed, $settings ) ;
    return $block;
  } else {
    $block->content = theme("wallyct_rss", $subtype, $context, $feed, $settings); 
    return $block;  
  }
}

/**
 * Given a $subtype and a $conf, return the actual settings to use.
 *
 * The actual settings may be stored directly in the pane or this may
 * be a pointer to re-usable content that may be in the database or in
 * an export. We have to determine from the subtype whether or not it
 * is local or shared rssmixread content.
 * 
 * @param $subtype
 *   The given subtype.
 * @param $conf
 *   The configuration informations.
 * 
 * @return
 *   The usable settings.
 */
function wallyextra_rssmixread_content_type_get_conf($subtype, $conf) {
  if ($subtype['name'] != 'rssmixread') {
    $settings = $subtype['content']->settings;
    $settings['rssmixread_type'] = 'fixed';
    $settings['content'] = $subtype['content'];
  } else {
    // This means they created it as rssmixread content and then set it as
    // reusable. Since we're not allowed to change the subtype, we're
    // still stored as though we are local, but are pointing off to
    // non-local.
    if (!empty($conf['name'])) {
      ctools_include('export');
      $content = ctools_export_crud_load('wallyextra_rssmixread_content', $conf['name']);
      if ($content) {
        $settings = $content->settings;
      } else {
        $content = wallyextra_newctoolsexport('wallyextra_rssmixread_content', 'rssmixread');
        $content->name = $conf['name'];
        $settings = _wallyextra_rssmixread_defaultformvalues();
      }
      $settings['rssmixread_type'] = 'fixed';
      $settings['content'] = $content;
    } else {
      // This means that it is created as rssmixread and has not been set to
      // reusable.
      $settings = $conf;
      $settings['rssmixread_type'] = 'local';
    }
  }

  return $settings;
}

/**
 * Callback to provide the administrative title of the rssmixread content.
 */
function wallyextra_rssmixread_content_type_admin_title($subtype, $conf) {
  $settings = wallyextra_rssmixread_content_type_get_conf(wallyextra_rssmixread_content_type_content_type($subtype), $conf);

  $output = t('RSS mix reader');
  $title = !empty($settings['admin_title']) ? $settings['admin_title'] : (!empty($settings['title'])) ? $settings['title'] : $settings['caid'] ;
  
  if ($title) {
    if ($settings['rssmixread_type'] != 'fixed') {
      $output = t('RSS mix reader: @title', array('@title' => $title));
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
 * 
 * @todo
 *   Check utility of this function (also in some other plugins of wallyextra)
 */
function wallyextra_rssmixread_content_type_admin_info($subtype, $conf) {
  $settings = wallyextra_rssmixread_content_type_get_conf(wallyextra_rssmixread_content_type_content_type($subtype), $conf);

  $block = new stdClass();
  $block->title = (!empty($settings['title'])) ? filter_xss_admin($settings['title']) : filter_xss_admin($settings['caid']);
  
  // We don't want to render php output on preview here, because if something is
  // wrong the whole display will be borked. So we check to see if the php
  // evaluator filter is being used, and make a temporary change to the filter
  // so that we get the printed php, not the eval'ed php.
  $php_filter = FALSE;
  if (!empty($settings['format'])) {
	  foreach (filter_list_format($settings['format']) as $filter) {
	    if ($filter->module == 'php') {
	      $php_filter = TRUE;
	      break;
	    }
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
    if (!empty($settings['body'])) {
    	$block->content = filter_xss_admin(check_markup($settings['body'], $settings['format']));
  	} else {
  		$block->content = t("n/a");
  	}

  }
  return $block;
}

/**
 * Definition of the default values of the edit form.
 * 
 * @return
 *   The default values.
 */
function _wallyextra_rssmixread_defaultformvalues() {
  $default_form_values = array();
  $default_form_values['uris'] = '';
  $default_form_values['maxitem_global'] = 10;
  $default_form_values['maxitem_byfeed'] = 10;
  $default_form_values['delta_t'] = 20;
  $default_form_values['own_schema'] = '';
  $default_form_values['mix_schema'] = 'alt';
  $default_form_values['get_distant_img'] = 0;
  $default_form_values['override_theme'] = 0;
  $default_form_values['override_theme_text'] = '';
  $default_form_values['caid'] = 'rssmix_'.time();

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
 * @param $settings
 *   The settings of the widget.
 * 
 * @return
 *   The array.
 */
function _wallyextra_rssmixread_createarrayfromuri($rss_uri, $maxitem, $settings) {
  $feed_array = array();
  $feed_xml = drupal_http_request($rss_uri);

  if ($feed_xml) {
    $dom = new DomDocument();
    $dom->LoadXML($feed_xml->data);
    $channels = $dom->getElementsByTagName('channel');
    if ($channels->item(0)) {
      foreach ($channels->item(0)->getElementsByTagName('item') as $item) {
        $temp_feed_array = wallytoolbox_rss_item_to_array($channels->item(0), $item);
        if ($settings['get_distant_img'] == 1) {
          foreach ($temp_feed_array['Package']['EmbeddedContent']['EmbeddedObjects']['Object'] as $object) {
            $new_file_path = 'RSS/';
            $new_file_path .= isset($options['override_title']) ? $settings['override_title_text'] : $settings['caid'];
            _wimprthelp_create_file_from_uri($object['FileURI']['value'], $new_file_path);
          }
        }
        $feed_array[] = $temp_feed_array;
        if (sizeof($feed_array) >= $maxitem) break;
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
function _wallyextra_rssmixread_getavailableitemsfromschema($schema_elem, $feed_array) {
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
 *   The array containing the available feeds.
 * 
 * @return
 *   The ID of the RSS feed list containing the most recent feed.
 */
function _wallyextra_rssmixread_getmostrecentfromavailables($available_items) {
  $most_recent = array(
    'Package' => array('PublicationDate' => '1970-01-01T00:00:00+00:00'),
    'arrayID' => 0,
  );

  foreach ($available_items as $available_item) {
    if (strtotime($available_item['Package']['PublicationDate']) > strtotime($most_recent['Package']['PublicationDate']))
      $most_recent = $available_item;
  }

  return $most_recent['arrayID'];
}
