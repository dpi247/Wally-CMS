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
  'no title override' => TRUE,
  'defaults' => _wallyextra_flowmix_default_form_values(),
  'js' => array('misc/autocomplete.js', 'misc/textarea.js', 'misc/collapse.js'),
  // Make sure the edit form is only used for some subtypes.
  'add form' => array(
    'wallyflowmix_contenttype_summary_form'        => t('Summary'),
    'wallyflowmix_contenttype_basicsetttings_form' => t('Create Flow 1/3: Basic settings'),
    'wallyflowmix_contenttype_pluginselector_form' => t('Create Flow 2/3: Plugin Selector'),
    'wallyflowmix_contenttype_pluginspecific_form' => t('Create Flow 2bis/3: Plugin specific settings'),
    'wallyflowmix_contenttype_finalstep_form'      => t('Create Flow 3/3:  Final Step'),
  ),
  'edit form' => array(
    'wallyflowmix_contenttype_summary_form'        =>t('Summary'),
    'wallyflowmix_contenttype_basicsetttings_form' =>t('Create Flow 1/3: Basic settings'),
    'wallyflowmix_contenttype_pluginselector_form' =>t('Create Flow 2/3: Plugin Selector'),
    'wallyflowmix_contenttype_pluginspecific_form' =>t('Create Flow 2bis/3: Plugin specific settings'),
    'wallyflowmix_contenttype_finalstep_form'      =>t('Create Flow 3/3:  Final Step'),
  ),
  'edit text' => t('Edit'),
  'all contexts' => TRUE,
);

/**
 * Rendering of the flow mix plugin.
 */
function wallyextra_flowmix_content_type_render($subtype, $conf, $panel_args, $context) {
  $settings = wallyextra_flowmix_content_type_get_conf(wallyextra_flowmix_content_type_content_type($subtype), $conf);
  $settings = (array)$settings['flow_obj'];

  $feed = array();
  $block = new stdClass();
  $block->module = 'wallyextra';
  $block->title = 'Flow mix.';
  $cid = $settings['caid'];
  $default_theme = 'wallyct_flowmix';

  $feed = wallyextra_get_cache_or_create($cid, '_wallyextra_flowmix_setcache', $settings, $subtype, $context, $default_theme, drupal_get_path('module', 'wallyextra').'/plugins/content_types/flowmix.inc', FALSE);
  
  if (is_string($feed))
    $block->content = $feed;
  elseif ($settings['override_theme_text'] != '')
    $block->content = theme($settings['override_theme_text'], $subtype, $context, $feed, $settings ) ;
  else
    $block->content = theme($default_theme, $subtype, $context, $feed, $settings); 
  
  return $block;
}

/**
 * Given a $subtype and a $conf, return the actual settings to use.
 *
 * The actual settings may be stored directly in the pane or this may
 * be a pointer to re-usable content that may be in the database or in
 * an export. We have to determine from the subtype whether or not it
 * is local or shared flowmix content.
 * 
 * @param $subtype
 *   The given subtype.
 * @param $conf
 *   The configuration informations.
 * 
 * @return
 *   The usable settings.
 */
function wallyextra_flowmix_content_type_get_conf($subtype, $conf) {
  if ($subtype['name'] != 'flowmix') {
    $settings = $subtype['content']->settings;
    $settings['flowmix_type'] = 'fixed';
    $settings['content'] = $subtype['content'];
  } else {
    // This means they created it as flowmix content and then set it as
    // reusable. Since we're not allowed to change the subtype, we're
    // still stored as though we are local, but are pointing off to
    // non-local.
    if (!empty($conf['name'])) {
      ctools_include('export');
      $content = ctools_export_crud_load('wallyextra_flowmix_content', $conf['name']);
      if ($content) {
        $settings = $content->settings;
      } else {
        $content = wallyextra_newctoolsexport('wallyextra_flowmix_content', 'flowmix');
        $content->name = $conf['name'];
        $settings = _wallyextra_flowmix_default_form_values();
      }
      $settings['flowmix_type'] = 'fixed';
      $settings['content'] = $content;
    } else {
      // This means that it is created as flowmix and has not been set to
      // reusable.
      $settings = $conf;
      $settings['flowmix_type'] = 'local';
    }
  }

  return $settings;
}

/**
 * Callback to provide the administrative title of the flowmix content.
 */
function wallyextra_flowmix_content_type_admin_title($subtype, $conf) {
  $settings = wallyextra_flowmix_content_type_get_conf(wallyextra_flowmix_content_type_content_type($subtype), $conf);

  $output = t('Flow mix');
  if (!empty($settings['content']->admin_title)) {
    $title = $settings['content']->admin_title;
  } elseif (!empty($settings['title'])) {
    $title = $settings['title'];
  } else {
    $title = $settings['caid'];
  }

  if ($title) {
    if ($settings['flowmix_type'] != 'fixed') {
      $output = t('Flow mix: @title', array('@title' => $title));
    }
    else {
      $output = t('Reusable flow mix: @title', array('@title' => $title));
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
function wallyextra_flowmix_content_type_admin_info($subtype, $conf) {
  $settings = wallyextra_flowmix_content_type_get_conf(wallyextra_flowmix_content_type_content_type($subtype), $conf);

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
 * Custom refresh action for flow mix blocks
 */
function wallyextra_flowmix_refresh_block($settings) {
  $old_feed = cache_get($settings['caid']);
  if (is_object($old_feed) && isset($old_feed) && !empty($old_feed)) {
    _wallyextra_flowmix_setcache($settings, $old_feed->data);
  } else {
    _wallyextra_flowmix_setcache($settings);
  }
}

/**
 * Export the configuration of the reusable block.
 */
function _wallyextra_flowmix_content_type_edit_save(&$content, $values) {
  // Apply updates to the content object.
  $content->name = $values['name'];
  $content->category = $values['category'];
  $content->admin_title = $values['admin_title'];
  $content->admin_description = $values['admin_description'];
  foreach (array_keys(_wallyextra_flowmix_defaultformvalues()) as $key) {
    if (isset($values[$key])) {
      $content->settings[$key] = $values[$key];
    }
  }

  ctools_include('export');
  ctools_export_crud_save('wallyextra_flowmix_content', $content);
}

/**
 * Return all flowmix content types available.
 */
function wallyextra_flowmix_content_type_content_types() {
  ctools_include('export');
  $types = array();
  $types['flowmix'] = _wallyextra_flowmix_default_ctools_content_type();
  
  foreach (ctools_export_crud_load_all('wallyextra_flowmix_content') as $name => $content) {
    $types[$name] = _wallyextra_flowmix_ctools_content_type($content);
  }
  return $types;
}

/**
 * Return the flowmix content types with the specified $subtype_id.
 */
function wallyextra_flowmix_content_type_content_type($subtype_id) {
  if ($subtype_id == 'flowmix') {
    return _wallyextra_flowmix_default_ctools_content_type();
  }
  else {
    ctools_include('export');
    $content = ctools_export_crud_load('wallyextra_flowmix_content', $subtype_id);
    if ($content) {
      return _wallyextra_flowmix_ctools_content_type($content);
    }
  }
}

/**
 * Settings for the default flowmix content type.
 *
 * The default is the one that allows the user to actually create a type.
 */
function _wallyextra_flowmix_default_ctools_content_type() {
  $info = array(
    'name' => 'flowmix',
    'title' => t('Flow mix'),
    'category' => t('Wally Extra'),
    'edit form' => 'wallyextra_flowmix_content_type_edit_form',
    'description' => t('Fetch some flows and mix them together in a single feed'),
    'all contexts' => TRUE,
    'defaults' => _wallyextra_flowmix_default_form_values(),
  'add form' => array(
    'wallyflowmix_contenttype_summary_form'        => t('Summary'),
    'wallyflowmix_contenttype_basicsetttings_form' => t('Create Flow 1/3: Basic settings'),
    'wallyflowmix_contenttype_pluginselector_form' => t('Create Flow 2/3: Plugin Selector'),
    'wallyflowmix_contenttype_pluginspecific_form' => t('Create Flow 2bis/3: Plugin specific settings'),
    'wallyflowmix_contenttype_finalstep_form'      => t('Create Flow 3/3:  Final Step'),
  ),
  'edit form' => array(
    'wallyflowmix_contenttype_summary_form'        =>t('Summary'),
    'wallyflowmix_contenttype_basicsetttings_form' =>t('Create Flow 1/3: Basic settings'),
    'wallyflowmix_contenttype_pluginselector_form' =>t('Create Flow 2/3: Plugin Selector'),
    'wallyflowmix_contenttype_pluginspecific_form' =>t('Create Flow 2bis/3: Plugin specific settings'),
    'wallyflowmix_contenttype_finalstep_form'      =>t('Create Flow 3/3:  Final Step'),
  ),
  );

  return $info;
}

/**
 * Return an info array for a specific flowmix content type.
 */
function _wallyextra_flowmix_ctools_content_type($content) {
  $info = array(
    'name' => $content->name,
    'title' => check_plain($content->admin_title),
    'description' => check_plain($content->admin_description),
    'category' => $content->category ? check_plain($content->category) : t('Wally reusable blocks'),
    'edit form' => 'wallyextra_flowmix_content_type_edit_form',
    'all contexts' => TRUE,
    'icon' => 'icon_block_custom.png',
    // Store this here to make it easy to access.
    'content' => $content,
  'add form' => array(
    'wallyflowmix_contenttype_summary_form'        => t('Summary'),
    'wallyflowmix_contenttype_basicsetttings_form' => t('Create Flow 1/3: Basic settings'),
    'wallyflowmix_contenttype_pluginselector_form' => t('Create Flow 2/3: Plugin Selector'),
    'wallyflowmix_contenttype_pluginspecific_form' => t('Create Flow 2bis/3: Plugin specific settings'),
    'wallyflowmix_contenttype_finalstep_form'      => t('Create Flow 3/3:  Final Step'),
  ),
  'edit form' => array(
    'wallyflowmix_contenttype_summary_form'        =>t('Summary'),
    'wallyflowmix_contenttype_basicsetttings_form' =>t('Create Flow 1/3: Basic settings'),
    'wallyflowmix_contenttype_pluginselector_form' =>t('Create Flow 2/3: Plugin Selector'),
    'wallyflowmix_contenttype_pluginspecific_form' =>t('Create Flow 2bis/3: Plugin specific settings'),
    'wallyflowmix_contenttype_finalstep_form'      =>t('Create Flow 3/3:  Final Step'),
  ),
  );

  return $info;
}

/**
 * Refresh the cache for the selected flow mix entry in the cache table
 * 
 * @param $settings
 *   The array containing the settings for this object.
 * 
 * @return
 *   The array containing the differents items of the flow mix feed 
 * 	 which will be displayed.
 */
function _wallyextra_flowmix_setcache($settings, $old_feed = array()) {
  $override_theme = $settings['override_theme'];
  $feed = array();
  $settings['flows'][1]->conf = $settings['flows'][0]->conf;
  $settings['flows'][0]->conf = array('maxitem' => 5);

  if (isset($settings['own_schema']) && !empty($settings['own_schema'])) {
    $mix_schema = explode(',', $settings['own_schema']);
  } else {
    if ($settings['mix_schema'] == 'alt') {
      $mix_schema = array();
      for ($i=0; $i<sizeof($flow_uris); $i++) $mix_schema[] = $i+1;
    } else {
      $mix_schema = explode(',', $settings['mix_schema']);
    }
  }
  
  foreach ($settings['flows'] as $flow) {
    $flow = (array)$flow;
    $temp_feed_array = _wallyextra_flowmix_create_array_from_uri($flow);
    if ($temp_feed_array)
      $feed_array[] = $temp_feed_array;
  }

  $maxitem_global = $settings['maxitem_global'];
  for ($i=0; $i<$maxitem_global; $i++) {
    $schema_elem = $mix_schema[$i%sizeof($mix_schema)];
    $available_items = _wallyextra_flowmix_get_available_items_from_schema($schema_elem, $feed_array);
    $most_recent_index = _wallyextra_flowmix_get_most_recent_from_availables($available_items);
    $item = array_shift($feed_array[$most_recent_index]);
    if (isset($item)){
      array_push($feed, $item);
    }
  }

  $modif = FALSE;
  for ($i=0; $i<sizeof($feed); $i++) {
    $elem_package = $feed[$i];
    $feed[$i]['feed_md5'] = md5($elem_package['PublicationDate'].$elem_package['PackageTitle']['value'].$elem_package['ExternalURI']['value'].$elem_package['MainStory']['TextBody']['value']);
    if (!isset($old_feed[$i]['feed_md5']) || ($feed[$i]['feed_md5'] != $old_feed[$i]['feed_md5']))
      $modif = TRUE;
  }
  
  if ($modif) {
    $internal_url = drupal_get_normal_path(trim($_SERVER[REQUEST_URI], '/'));
    wallytoolbox_flush_all_pages($internal_url, TRUE);
    cache_set($settings['caid'], $feed, 'cache', CACHE_PERMANENT);//, NULL, 'flowmix');
  }

  return $feed;
}

/**
 * Build a array based on an feed.
 * 
 * @param $flow_uri
 *   The feed URI.
 * @param $maxitem
 *   A limitation in the number of returned feeds.
 * @param $settings
 *   The settings of the widget.
 * 
 * @return
 *   The array.
 */
function _wallyextra_flowmix_create_array_from_uri($flow) {
  ctools_include('plugins');
  if ($function = ctools_plugin_get_function(wallytoolbox_get_plugin_by_name($flow['plugin_name']), 'build')) {
    while(is_string($flow['conf']))
      $flow['conf'] = unserialize($flow['conf']);
    while(is_string($flow['feeds_md5']))
      $flow['feeds_md5'] = unserialize($flow['feeds_md5']);
    $struct_array = $function($flow);
    if ($struct_array['Packages']['__log']['success'] && !empty($struct_array['Packages']['Package'])) {
      return $struct_array['Packages']['Package'];
    }
  }
  
  return FALSE;
}

/**
 * Select the most recent feed of every flows list that matches the
 * schema.
 * 
 * @param $schema_elem
 *   The schema containing the IDs of the allowed flows list(s).
 * @param $feed_array
 *   Array of all flows lists.
 * 
 * @return
 *   The array containing the selected feeds and their respective flows list ID.
 */
function _wallyextra_flowmix_get_available_items_from_schema($schema_elem, $feed_array) {
  $available_items = array();
  $schema_elems = explode('-', $schema_elem);
  for ($i=0; $i<sizeof($feed_array); $i++) {
    if (in_array($i+1, $schema_elems) || $schema_elems[0] == 'any') {
      if (isset($feed_array[$i][0]) && !empty($feed_array[$i][0]) && is_array($feed_array[$i][0])) {
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
 *   The ID of the flow list containing the most recent feed.
 */
function _wallyextra_flowmix_get_most_recent_from_availables($available_items) {
  $most_recent = array(
    'PublicationDate' => '1970-01-01T00:00:00+00:00',
    'arrayID' => 0,
  );

  foreach ($available_items as $available_item) {
    if (strtotime($available_item['PublicationDate']) > strtotime($most_recent['PublicationDate']))
      $most_recent = $available_item;
  }

  return $most_recent['arrayID'];
}

function _wallyextra_flowmix_default_form_values(){
  return array();
}
