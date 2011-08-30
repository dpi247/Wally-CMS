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
  'defaults' => _wallyextra_flowmix_content_type_form_summary_defaults(),
  'js' => array('misc/autocomplete.js', 'misc/textarea.js', 'misc/collapse.js'),
  // Make sure the edit form is only used for some subtypes.
  'add form' => array(
    'wallyextra_flowmix_content_type_form_summary_form'        => t('Summary'),
    'wallyextra_flowmix_content_type_form_basicsetttings_form' => t('Create Flow 1/2: Basic settings'),
    'wallyextra_flowmix_content_type_form_pluginselector_form' => t('Create Flow 2/2: Plugin Selector'),
    'wallyextra_flowmix_content_type_form_pluginspecific_form' => t('Create Flow 2bis/2: Plugin specific settings'),
  ),
  'edit form' => array(
    'wallyextra_flowmix_content_type_form_summary_form'        =>t('Summary'),
    'wallyextra_flowmix_content_type_form_basicsetttings_form' =>t('Create Flow 1/2: Basic settings'),
    'wallyextra_flowmix_content_type_form_pluginselector_form' =>t('Create Flow 2/2: Plugin Selector'),
    'wallyextra_flowmix_content_type_form_pluginspecific_form' =>t('Create Flow 2bis/2: Plugin specific settings'),
  ),
  'edit text' => t('Edit'),
  'all contexts' => TRUE,
);

/**
 * Rendering of the flow mix plugin.
 */
function wallyextra_flowmix_content_type_render($subtype, $conf, $panel_args, $context) {
  $settings = (array)wallyextra_flowmix_content_type_get_conf(wallyextra_flowmix_content_type_content_type($subtype), $conf);

  $feed = array();
  $block = new stdClass();
  $block->module = 'wallyextra';
  $block->title = 'Flow mix.';
  $cid = $settings['caid'];
  $default_theme = 'wallyct_flowmix';
  $ajax = TRUE;

  $feed = wallyextra_get_cache_or_create($cid, '_wallyextra_flowmix_setcache', $settings, $subtype, $context, $default_theme, drupal_get_path('module', 'wallyextra').'/plugins/content_types/flowmix.inc', $ajax);
  
  if (is_string($feed))
    $block->content = $feed;
  elseif ($settings['override_theme'] && $settings['override_theme_text'] != '')
    $block->content = theme($settings['override_theme_text'], $subtype, $context, $feed, $settings);
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
  $settings = new stdClass();
  $conf = $conf['flow_obj'];

  if ($subtype['name'] == 'flowmix') {
    $settings = $conf;
    $settings->flowmix_type = 'local';
  } else {
    $settings = $subtype['content']->settings;
    $settings->flowmix_type = 'fixed';
  }

  return $settings;
}

/**
 * Callback to provide the administrative title of the flowmix content.
 */
function wallyextra_flowmix_content_type_admin_title($subtype, $conf) {
  $settings = wallyextra_flowmix_content_type_get_conf(wallyextra_flowmix_content_type_content_type($subtype), $conf);

  $output = t('Flow mix');
  if (!empty($settings->content->admin_title)) {
    $title = $settings->content->admin_title;
  } elseif (!empty($settings->title)) {
    $title = $settings->title;
  } else {
    $title = $settings->caid;
  }

  if ($title) {
    if ($settings->flowmix_type != 'fixed') {
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
  $block->title = (!empty($settings->title)) ? filter_xss_admin($settings->title) : filter_xss_admin($settings->caid);
  
  // We don't want to render php output on preview here, because if something is
  // wrong the whole display will be borked. So we check to see if the php
  // evaluator filter is being used, and make a temporary change to the filter
  // so that we get the printed php, not the eval'ed php.
  $php_filter = FALSE;
  if (!empty($settings->format)) {
    foreach (filter_list_format($settings->format) as $filter) {
      if ($filter->module == 'php') {
        $php_filter = TRUE;
        break;
      }
    }
  }
  // If a php filter is active, just print the source, but only if the current
  // user has access to the actual filter.
  if ($php_filter) {
    if (!filter_access($settings->format)) {
      return NULL;
    }
    $block->content = '<pre>'. check_plain($settings->body) .'</pre>';
  }
  else {
    // We also need to filter through XSS admin because <script> tags can
    // cause javascript which will interfere with our ajax.
    if (!empty($settings->body)) {
      $block->content = filter_xss_admin(check_markup($settings->body, $settings->format));
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

function _wallyextra_flowmix_content_type_save_reusable(&$form_state) {
  cache_clear_all($form_state['values']['caid']);
  if ($form_state['settings']->flowmix_type == 'fixed') {
    $content = ctools_export_crud_load('wallyextra_flowmix_content', $form_state['subtype']['name']);
    _wallyextra_flowmix_content_type_edit_save($content, $form_state['conf']['flow_obj']);
  } elseif (!empty($form_state['values']['reusable'])) {
    // If the 'reusable' checkbox was checked, we will create a new
    // flowmix content and give it the proper values.
    $content = wallyextra_newctoolsexport('wallyextra_flowmix_content', 'flowmix');
    _wallyextra_flowmix_content_type_edit_save($content, $form_state['conf']['flow_obj']);
    $form_state['pane']->subtype = $content->name;
  }
}

/**
 * Export the configuration of the reusable block.
 */
function _wallyextra_flowmix_content_type_edit_save(&$content, $values) {
  // Apply updates to the content object.
  $content->name = $values->name;
  $content->category = $values->category;
  $content->admin_title = $values->admin_title;
  $content->admin_description = $values->admin_description;
  $defaults = (array)_wallyextra_flowmix_content_type_form_summary_defaults();
  $content->settings = new stdClass();
  foreach (array_keys($defaults) as $key) {
    if (isset($values->{$key})) {
      $content->settings->{$key} = $values->{$key};
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
    'description' => t('Fetch some flows and mix them together in a single feed'),
    'all contexts' => TRUE,
    'defaults' => _wallyextra_flowmix_content_type_form_summary_defaults(),
    'add form' => array(
      'wallyextra_flowmix_content_type_form_summary_form'        => t('Summary'),
      'wallyextra_flowmix_content_type_form_basicsetttings_form' => t('Create Flow 1/2: Basic settings'),
      'wallyextra_flowmix_content_type_form_pluginselector_form' => t('Create Flow 2/2: Plugin Selector'),
      'wallyextra_flowmix_content_type_form_pluginspecific_form' => t('Create Flow 2bis/2: Plugin specific settings'),
    ),
    'edit form' => array(
      'wallyextra_flowmix_content_type_form_summary_form'        =>t('Summary'),
      'wallyextra_flowmix_content_type_form_basicsetttings_form' =>t('Create Flow 1/2: Basic settings'),
      'wallyextra_flowmix_content_type_form_pluginselector_form' =>t('Create Flow 2/2: Plugin Selector'),
      'wallyextra_flowmix_content_type_form_pluginspecific_form' =>t('Create Flow 2bis/2: Plugin specific settings'),
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
    'all contexts' => TRUE,
    'icon' => 'icon_block_custom.png',
    // Store this here to make it easy to access.
    'content' => $content,
    'add form' => array(
      'wallyextra_flowmix_content_type_form_summary_form'        => t('Summary'),
      'wallyextra_flowmix_content_type_form_basicsetttings_form' => t('Create Flow 1/2: Basic settings'),
      'wallyextra_flowmix_content_type_form_pluginselector_form' => t('Create Flow 2/2: Plugin Selector'),
      'wallyextra_flowmix_content_type_form_pluginspecific_form' => t('Create Flow 2bis/2: Plugin specific settings'),
    ),
    'edit form' => array(
      'wallyextra_flowmix_content_type_form_summary_form'        =>t('Summary'),
      'wallyextra_flowmix_content_type_form_basicsetttings_form' =>t('Create Flow 1/2: Basic settings'),
      'wallyextra_flowmix_content_type_form_pluginselector_form' =>t('Create Flow 2/2: Plugin Selector'),
      'wallyextra_flowmix_content_type_form_pluginspecific_form' =>t('Create Flow 2bis/2: Plugin specific settings'),
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
  $feed = array();

  if (isset($settings['own_schema']) && !empty($settings['own_schema'])) {
    $mix_schema = explode(',', $settings['own_schema']);
  } else {
    if ($settings['mix_schema'] == 'alt') {
      $mix_schema = array();
      for ($i=0; $i<sizeof($settings['flows']); $i++) $mix_schema[] = $i+1;
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
    $item['__flow_id'] = $most_recent_index;
    if (isset($item)){
      array_push($feed, $item);
    }
  }

  $modif = FALSE;
  for ($i=0; $i<sizeof($feed); $i++) {
      $elem_package = $feed[$i];
      $feed[$i]['feed_md5'] = md5($elem_package['PublicationDate'].$elem_package['PackageTitle']['value'].$elem_package['ExternalURI']['value'].$elem_package['MainStory']['TextBody']['value']);
      if (!isset($old_feed[$i]['feed_md5']) || ($feed[$i]['feed_md5'] != $old_feed[$i]['feed_md5'])) {
        $modif = TRUE;
        break;
      }
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

function wallyextra_flowmix_content_type_form_summary_form(&$form, &$form_state) {
  if (isset($form_state['conf']['flow_obj']) && !empty($form_state['conf']['flow_obj']) && sizeof($form_state['input']) > 3 && (isset($form_state['input']['conf']) || isset($form_state['input']['save'])))
    $form_state['subtype']['content']->settings = $form_state['conf']['flow_obj'];
  $settings = wallyextra_flowmix_content_type_get_conf($form_state['subtype'], $form_state['conf']);
  $form_state['settings'] = $settings;

  $creation_infos = isset($form_state['conf']['flow_obj']) ? $form_state['conf']['flow_obj']->caid : $form_state['plugin']['defaults']->caid;
  $creation_date = str_replace('flowmix_', '', $creation_infos);
  if (($settings->flowmix_type == 'fixed' && ($creation_date + 1) >= time()) || (isset($form_state['post']['save']) && !isset($form_state['post']['title']))) {
    unset($form['buttons']['next']);
    $form['buttons']['save'] = array(
      '#type'          => 'submit',
      '#value'         => t('Save'),
      '#wizard type'   => 'finish',
      '#name'          => 'save',
    );
    
    return; // no form for this case.
  }

  foreach ($settings->flows as $key => $flow) {
    $form['flows'][$key]['edit_action']=array(
      '#type'          => 'submit',
      '#value'         => t('Edit'),
      '#wizard type'   => 'next',
      '#name'          => 'edit_'.$key,
      '#next'          => 'step2'
    );
    
    $form['flows'][$key]['delete_action']=array(
      '#type'          => 'submit',
      '#value'         => t('Remove'),
      '#wizard type'   => 'next',
      '#name'          => 'delete_'.$key,
      '#next'          => 'step2'
    );
    
    $form['flows'][$key]['title']=array(
      '#type'          => 'markup',
      '#value'         => $flow->name,
    );
  }
  
  $form['title'] = array(
    '#type'             => 'textfield',
    '#title'            => t('Title'),
    '#description'      => t('The title of the block.'),
    '#default_value'    => $settings->title,
    '#required'         => TRUE,
    '#weight'           => 0,
  );

  $form['maxitem_global'] = array (
    '#type'             => 'textfield',
    '#title'            => t('Max item displayed in the final mixed flow'),
    '#default_value'    => $settings->maxitem_global,
    '#required'         => TRUE,
  );

  $mix_schemas = array(
    'alt'               => 'Alternate',
    'any'               => 'Most Recent',
    'mix'               => 'Define custom schema'
  );

  ctools_include('dependent');
  ctools_add_js('dependent');

  $form['mix_schema'] = array(
    '#type'             => 'select',
    '#title'            => t('Schema used for mixing the feeds'),
    '#default_value'    => $settings->mix_schema,
    '#options'          => $mix_schemas,
  );

  $form['own_schema'] = array (
    '#type'             => 'textfield',
    '#title'            => t('Define your custom schema used for mixing the feeds'),
    '#description'      => t('Examples:
        1,2,3 / 1-2,3,2,1-3,2,2-3 / any,2,1-2,3,any,2 / ... see <a href="http://rossel.audaxis.com/xwiki/wiki/drupal/view/Main/User_RSS_mix" target="_new">RSS mix documentation</a>.
      '),
    '#default_value'    => $settings->own_schema,
    '#required'         => FALSE,
    '#dependency'       => array('edit-mix-schema'=>array('mix')),
    '#process'          => array('ctools_dependent_process'),
  );

  $form['override_theme'] = array (
    '#type'             => 'checkbox',
    '#default_value'    => $settings->override_theme,
    '#id'               => 'override-theme-checkbox',
    '#title'            => t('Override Theming Function'),
  );

  $form['override_theme_text'] = array (
    '#type'             => 'textfield',
    '#default_value'    => $settings->override_theme_text,
    '#id'               => 'override-theme-textfield',
    '#dependency'       => array('override-theme-checkbox' => array('1')),
    '#process'          => array('ctools_dependent_process'),
    '#title'            => t('Theming Function Name'),
  );
  
  $form['caid'] = array (
    '#type'             => 'hidden',
    '#value'            => $settings->caid,
  );
  
  if (user_access('administer flowmix content') && $settings->flowmix_type == 'local') {
    $form['reusable'] = array(
      '#type' => 'checkbox',
      '#title' => t('Make this content reusable'),
      '#default_value' => $settings->reusable,
    );
  
    $form['admin_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Administrative title'),
      '#description' => t('The title used as name for the reusable block.'),
      '#default_value' => $settings->admin_title,
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('edit-reusable' => array(1)),
    );
  
    $form['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Machine name'),
      '#description' => t('The machine readable name of this content. It must be unique, and it must contain only alphanumeric characters and underscores. Once created, you will not be able to change this value!'),
      '#default_value' => $settings->name,
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('edit-reusable' => array(1)),
    );
  
    $form['category'] = array(
      '#type' => 'textfield',
      '#title' => t('Category'),
      '#description' => t('What category this content should appear in. If left blank the category will be "Wally reusable blocks".'),
      '#default_value' => $settings->category,
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('edit-reusable' => array(1)),
    );
  
    $form['admin_description'] = array(
      '#type' => 'textarea',
      '#title' => t('Administrative description'),
      '#description' => t('A description of what this content is, does or is for, for administrative use.'),
      '#default_value' => $settings->admin_description,
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('edit-reusable' => array(1)),
    );
  }

  unset($form['buttons']['next']);
  if(count($settings->flows)){
    $form['buttons']['save'] = array(
      '#type'=>'submit',
      '#value'=>t('Save'),
      '#wizard type'=>'finish',
      '#name'=>'save',
    );
  }
  
  $form['buttons']['add_flow'] = array(
    '#type'=>'submit',
    '#value'=>t('Add a new flow'),
    '#wizard type'=>'next',
    '#name'=>'next_'.count($settings->flows),
    '#next'=>'step2'
  );
}

function _wallyextra_flowmix_content_type_form_summary_defaults() {  
  $defaults = new stdClass();
  $defaults->title = '';
  $defaults->maxitem_global = '';
  $defaults->mix_schema = 'alt';
  $defaults->own_schema = '';
  $defaults->override_theme = 0;
  $defaults->override_theme_text = '';
  $defaults->caid = 'flowmix_'.time();
  $defaults->flows = array();
  
  return $defaults;
}

function wallyextra_flowmix_content_type_form_summary_form_submit(&$form, &$form_state) {
  $flowmix_item = &$form_state['conf']['flow_obj'];
  $submitted = $form_state['values'];

  if ((isset($form_state['input']['save']) && !isset($form_state['input']['title']))) {
    // This means no values were posted so we must be in the case where we use a reusable
    $flowmix_item = $form_state['settings'];
  } else {
    $save_values = array(
      'title',
      'maxitem_global',
      'mix_schema',
      'own_schema',
      'override_theme',
      'override_theme_text',
      'caid',
      'reusable',
      'admin_title',
      'name',
      'category',
      'admin_description',
    );
    foreach($save_values as $value) {
      // set the values in the cache object -- it gets passed back to the next step
      // because of all that work we did in the form_info array
      if (isset($submitted[$value]))
        $flowmix_item->$value = $submitted[$value];
    }
  }
  $flowmix_item->flows = $form_state['settings']->flows;

  if (strpos($form_state['clicked_button']['#name'], 'delete') !== false) {
    unset($flowmix_item->flows[str_replace('delete_', '', $form_state['clicked_button']['#name'])]);
    $form_state['clicked_button']['#next'] = 'step1';
  }
  elseif (strpos($form_state['clicked_button']['#name'], 'edit') !== false) {
    if ($form_state['step'] == 'step1') {
      $flowmix_item->index = (int)str_replace('edit_', '', $form_state['clicked_button']['#name']);
      $form_state['clicked_button']['#next'] = 'step2';
    }
  }
  elseif (strpos($form_state['clicked_button']['#name'], 'next') !== false) {
    if ($form_state['step'] == 'step1') {
      $flowmix_item->index = (int)str_replace('next_', '', $form_state['clicked_button']['#name']);
    }
  }
  elseif (strpos($form_state['clicked_button']['#name'], 'save') !== false) {
    if ($form_state['step'] == 'step1') {
      _wallyextra_flowmix_content_type_save_reusable($form_state);
    }
  }
}

function wallyextra_flowmix_content_type_form_basicsetttings_form(&$form, &$form_state) {
  $flowmix_item = &$form_state['conf']['flow_obj'];
  $flow_conf = _wallyextra_flowmix_content_type_form_basicsetttings_defaults($flowmix_item->flows[(int)$flowmix_item->index]);
  
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Flow feed name'),
    '#description' => t('The name you want to give to this flow.'),
    '#default_value' => $flow_conf->name,
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
    '#weight' => 0,
  );

  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Type of flow'),
    '#default_value' => $flow_conf->type,
    '#required' => TRUE,
    '#options' => wallytoolbox_get_available_flow_type(),
    '#weight' => 0.1,
  );

  $body_html_options = array();
  $body_html_query = db_query("SELECT * FROM `filter_formats");
  $i = 0;
  while ($body_html_element = db_fetch_array($body_html_query)) {
    $body_html_options[] = $body_html_element['name'];
    if ($body_html_element['name'] == $default_form_values['body_html'])
    $default_body_html = $i;
    $i++;
  }

  $form['body_html'] = array(
    '#type' => 'select',
    '#title' => t('Use HTML in the text body'),
    '#description' => t('Use HTML in the text body.'),
    '#options' => $body_html_options,
    '#default_value' => $flow_conf->body_html,
    '#weight' => 0.2,
  );

  // probably important -- i'm continuing to investigate
  $form_state['no buttons'] = TRUE;
}

function _wallyextra_flowmix_content_type_form_basicsetttings_defaults($flow_conf = NULL) {
  if (isset($flow_conf) && !empty($flow_conf)) {
    return $flow_conf;
  }
  
  $defaults = new stdClass();
  $defaults->name = '';
  $defaults->type = '';
  $defaults->body_html = 0;
  
  return $defaults;
}

function wallyextra_flowmix_content_type_form_basicsetttings_form_submit(&$form, &$form_state) {
  $flowmix_item = &$form_state['conf']['flow_obj'];
  $edited_flow = &$flowmix_item->flows[(int)$flowmix_item->index];
  $submitted = $form_state['values'];
  
  $save_values = array_keys((array)_wallyextra_flowmix_content_type_form_basicsetttings_defaults());
  // maybe don't imitate this foreach
  foreach ($save_values as $value) {
    // Set the values in the cache object -- it gets passed back to the next step
    // because of all that work we did in the form_info array
    $edited_flow->$value = $submitted[$value];
  }
}

function wallyextra_flowmix_content_type_form_pluginselector_form(&$form, &$form_state) {
  $flowmix_item = &$form_state['conf']['flow_obj'];
  $edited_flow = &$flowmix_item->flows[(int)$flowmix_item->index];

  $possible_plugins = wallytoolbox_get_plugins_that_handle_type($edited_flow->type);
  foreach ($possible_plugins as $plugin_name => $plugin) {
    $options[$plugin_name] = $plugin['title'].' ('.$plugin['description'].')';
  }

  $form['plugin_name'] = array(
    '#type' => 'radios',
    '#tree' => true,
    '#options' => $options,
    '#title' => t('Select A plugin'),
    '#default_value' => $edited_flow->plugin_name,
    '#required' => TRUE,
  );

  unset($form['buttons']['cancel']);
  $form_state['no buttons'] = TRUE;
}

function wallyextra_flowmix_content_type_form_pluginselector_form_submit(&$form, &$form_state) {
  $flowmix_item = &$form_state['conf']['flow_obj'];
  $edited_flow = &$flowmix_item->flows[(int)$flowmix_item->index];
  $submitted = $form_state['values'];
  
  $save_values = array('plugin_name');
  foreach ($save_values as $value) {
    // Set the values in the cache object -- it gets passed back to the next step
    // because of all that work we did in the form_info array
    $edited_flow->$value = $submitted[$value];
  }
  
  // Remove pluginspecific step if no need
  if (isset($flowmix_item->plugin_name) && wallytoolbox_has_settings_form_of_plugin(wallytoolbox_get_plugin_by_name($flowmix_item->plugin_name))) {
    $form_state['clicked_button']['#next'] = 'step1';
  }
}

function wallyextra_flowmix_content_type_form_pluginspecific_form(&$form, &$form_state) {
  $flowmix_item = &$form_state['conf']['flow_obj'];
  $edited_flow = &$flowmix_item->flows[(int)$flowmix_item->index];
  $form['conf'] = array('#tree' => TRUE);
  wallytoolbox_get_settings_form_of_plugin(wallytoolbox_get_plugin_by_name($edited_flow->plugin_name), $form['conf'], $edited_flow->conf);

  $form['buttons']['Ok'] = array(
    '#type'            => 'submit',
    '#value'           => t('Ok'),
    '#wizard type'     => 'next',
    '#name'            => 'next_'.max(0,count($flowmix_item->flows)-1),
    '#next'            => 'step1',
    '#weight'          => -1000,
  );

  unset($form['buttons']['cancel']);
  // probably important -- i'm continuing to investigate
  $form_state['no buttons'] = TRUE;
}

function wallyextra_flowmix_content_type_form_pluginspecific_form_validate(&$form, &$form_state) {
  $flowmix_item = &$form_state['conf']['flow_obj'];
  $edited_flow = &$flowmix_item->flows[(int)$flowmix_item->index];
  $plugin = wallytoolbox_get_plugin_by_name($edited_flow->plugin_name);
  
  // Call the plugin's proper(s) validation(s) function(s)
  if (!empty($form['conf']['#validate'])) {
    $conf_values['values'] = $form_state['values']['conf'];
    foreach ($form['conf']['#validate'] as $validate_func)
      drupal_validate_form($plugin['settings form'], $form['conf'], $conf_values);
  }
  
  drupal_get_messages();
  foreach (form_get_errors() as $field => $error) {
    form_set_error('conf]['.$field, $error);
  }
}

function wallyextra_flowmix_content_type_form_pluginspecific_form_submit(&$form, &$form_state) {
  $flowmix_item = &$form_state['conf']['flow_obj'];
  $edited_flow = &$flowmix_item->flows[(int)$flowmix_item->index];
  $edited_flow->conf = $form_state['values']['conf'];
  $form_state['settings'] = $flowmix_item;

  if (strpos($form_state['clicked_button']['#wizard type'], 'finish') !== false) {
    _wallyextra_flowmix_content_type_save_reusable($form_state);
  }
}

function theme_wallyextra_flowmix_content_type_form_summary_form($form){
  foreach(element_children($form['flows']) as $key=>$flow_form){
    $rows[] = array(
      drupal_render($form['flows'][$key]['title']),
      drupal_render($form['flows'][$key]['edit_action']).drupal_render($form['flows'][$key]['delete_action']),
    );
  }

  if(count($form['flows'])){
    $table_output.=theme('table',array('titre','action'),$rows);
    $button_add_flow_output.=drupal_render($form['buttons']['add_flow']);
  }

  $output_after=drupal_render($form);

  $output.=$table_output;
  $output.=$button_add_flow_output;
  $output.=$output_after;

  return $output;
}
