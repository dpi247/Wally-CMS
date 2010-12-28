<?php
/**
 * @file wally.profile
 * 
 * Wally installation profile
 */

/**
 * Implementation of hook_profile_details()
 */
function wally_profile_details() {  
  return array(
    'name' => 'Wally',
    'description' => t('The ultimate power of Drupal for online publisher from Audaxis.'),
  );
} 

/**
 * Return an array of developpement modules.
 */
function wally_devel_modules() {
  $dev = array(
    // Developpement & testing modules 
    'api', 'devel', 'grammar_parser', 
  ); 

  return $dev;
}

/**
 * Implementation of hook_profile_modules().
 */
function wally_profile_modules() {

  $mod = array(); 
  $custom = array(); 

  // Core Required & Optional modules.
  $core_modules = array(
    // Required core modules
    'block', 'filter', 'node', 'system', 'user', 'profile',

    // Optional core modules.
    'comment', 'help', 'locale', 'menu', 'statistics', 
    'search', 'taxonomy', 'translation', 'upload',
    'update',  
  );
  
  // Modified modules
  $mod = array(
    // CCK (content)
    'content', 'fieldgroup', 'nodereference',
    'text', 'optionwidgets',   
 
    // Location 
    'location', 'location_cck', 
  )

  // Third party requiered modules
  $third = array(
    // Administration
    'Admin',

    // CCK (content)
    'content_taxonomy', 'content_taxonomy_autocomplete', 
    'content_taxonomy_options', 'content_taxonomy_tree',
    'email', 'filefield', 'imagefield', 'link', 
    'pollfield', 
  
    // Chaos tool suite
    'ctools', 'page_manager', 'views_content',
  
    // Date/Time
    'date_api', 'date', 'date_timezone',
  
  ); 


  return array_merge($core_modules,$third,$mod,$custom);
} 

/**
 * Features module and Wally specific features
 */
function wally_feature_modules() {
  $features = array(
    'op_imce_config',
	  'op_author', 
	  'op_author_layout',	  
	  'op_author_panels', 
    'op_advuser_config', 
    'op_article', 
    'op_audio',	  
	  'op_blog', 
	  'op_event',
	  'op_image',
	  'op_imagecrop_config', 
	  'op_misc',
	  'op_package',
	  'op_resource',
	  'op_scheduler_config', 
	  'op_slideshow', 
	  'op_videos',	  
	  'op_defaults',
	  'op_contexts',
	  //'op_facebook',
    'op_editors_choice',
    'op_default_workflow',
    'op_default_workflow_nodetypes',
    
    // Rules needs to be enabled after op_default_workflow which contains default rules for the default workflow
    'rules', 
    'rules_admin',
	  
	  // Custom modules developed for OpenPublish. Openpublish_core needs op_advuser_config to run first.
	  'openpublish_core', 'openpublish_administration', 'openpublish_menu', 'openpublish_popular_terms',

  );
  return $features;
}

/**
 * Return a list of tasks that this profile supports.
 *
 * @return
 *   A keyed array of tasks the profile will perform during
 *   the final stage. The keys of the array will be used internally,
 *   while the values will be displayed to the user in the installer
 *   task list.
 */
function openpublish_profile_task_list() {

  global $conf;
  $conf['site_name'] = 'OpenPublish';
  $conf['site_footer'] = 'OpenPublish by <a href="http://phase2technology.com">Phase2 Technology</a>';
  $conf['theme_settings'] = array(
    'default_logo' => 0,
    'logo_path' => 'sites/all/themes/openpublish_theme/images/openpublish-logo.png',
  );
  
  $tasks['op-configure-batch'] = st('Configure OpenPublish');
  
  if (_openpublish_language_selected()) {
    $tasks['op-translation-import-batch'] = st('Importing translations');
  }
  
  return $tasks;
}

/**
 * Implementation of hook_profile_tasks().
 */
function openpublish_profile_tasks(&$task, $url) {
  global $install_locale;
  $output = "";
  
  install_include(openpublish_profile_modules());

  if($task == 'profile') {
    drupal_set_title(t('OpenPublish Installation'));
    _openpublish_log(t('Starting Installation'));
    _openpublish_base_settings();
    $task = "op-configure";
  }
    
  if($task == 'op-configure') {
    $batch['title'] = st('Configuring @drupal', array('@drupal' => drupal_install_profile_name()));
    $files = module_rebuild_cache();
    $cck_files = file_scan_directory ( dirname(__FILE__) . '/cck' , '.*\.inc$' );
    foreach ( $cck_files as $file ) {   
      $batch['operations'][] = array('_openpublish_import_cck', array($file));      
    }    
    foreach ( openpublish_feature_modules() as $feature ) {   
      $batch['operations'][] = array('_install_module_batch', array($feature, $files[$feature]->info['name']));      
      //-- Initialize each feature individually rather then all together in the end, to avoid execution timeout.
      $batch['operations'][] = array('features_flush_caches', array()); 
    }    
    $batch['operations'][] = array('_openpublish_set_permissions', array());      
    $batch['operations'][] = array('_openpublish_initialize_settings', array());      
    $batch['operations'][] = array('_openpublish_placeholder_content', array());      
    $batch['operations'][] = array('_openpublish_set_views', array());      
    $batch['operations'][] = array('_openpublish_install_menus', array());      
    $batch['operations'][] = array('_openpublish_setup_blocks', array()); 
    $batch['operations'][] = array('_openpublish_cleanup', array());      
    $batch['error_message'] = st('There was an error configuring @drupal.', array('@drupal' => drupal_install_profile_name()));
    $batch['finished'] = '_openpublish_configure_finished';
    variable_set('install_task', 'op-configure-batch');
    batch_set($batch);
    batch_process($url, $url);
  }

  if ($task == 'op-translation-import') {
    if (_openpublish_language_selected() && module_exists('l10n_update')) {
      module_load_install('l10n_update');
      module_load_include('batch.inc', 'l10n_update');

      $history = l10n_update_get_history();
      $available = l10n_update_available_releases();
      $updates = l10n_update_build_updates($history, $available);

      // Filter out updates in other languages. If no languages, all of them will be updated
      $updates = _l10n_update_prepare_updates($updates, NULL, array($install_locale));

      // Edited strings are kept, only default ones (previously imported)
      // are overwritten and new strings are added
      $mode = 1;

      if ($batch = l10n_update_batch_multiple($updates, $mode)) {
        $batch['finished'] = '_openpublish_import_translations_finished';
        variable_set('install_task', 'op-translation-import-batch');
        batch_set($batch);
        batch_process($url, $url);
      }
    }
  }
  
  // Land here until the batches are done
  if (in_array($task, array('op-translation-import-batch', 'op-configure-batch'))) {
    include_once 'includes/batch.inc';
    $output = _batch_page();
  }
    
  return $output;
} 

/**
 * Translation import process is finished, move on to the next step
 */
function _openpublish_import_translations_finished($success, $results) {
  _openpublish_log(t('Translations have been imported.'));
  /**
   * Necessary as the openpublish_theme's status gets reset to 0
   * by a part of the automated batch translation in l10n_update
   */
  install_default_theme('openpublish_theme');
  variable_set('install_task', 'profile-finished');
}

/**
 * Import process is finished, move on to the next step
 */
function _openpublish_configure_finished($success, $results) {
  _openpublish_log(t('OpenPublish has been installed.'));
  if (_openpublish_language_selected()) {
    // Other language, different part of the process
    variable_set('install_task', 'op-translation-import');
  }
  else {
    // English installation
    variable_set('install_task', 'profile-finished');
  }
}

/**
 * Do some basic setup
 */
function _openpublish_base_settings() {  
  global $base_url;  

  // create pictures dir
  $pictures_path = file_create_path('pictures');
  file_check_directory($pictures_path,TRUE);

  // Set distro tracker server URL for this distribution
  distro_set_tracker_server('http://tracker.openpublishapp.com/distro/components');
  variable_set('openpublish_version', '2.3'); 
 
  $types = array(
    array(
      'type' => 'page',
      'name' => st('Page'),
      'module' => 'node',
      'description' => st("A <em>page</em>, similar in form to a <em>story</em>, is a simple method for creating and displaying information that rarely changes, such as an \"About us\" section of a website. By default, a <em>page</em> entry does not allow visitor comments and is not featured on the site's initial home page."),
      'custom' => TRUE,
      'modified' => TRUE,
      'locked' => FALSE,
      'help' => '',
      'min_word_count' => '',
    ),   
  );

  foreach ($types as $type) {
    $type = (object) _node_type_set_defaults($type);
    node_type_save($type);
  }

  // Default page to not be promoted and have comments disabled.
  variable_set('node_options_page', array('status'));
  variable_set('comment_page', COMMENT_NODE_DISABLED);

  // Theme related.  
  install_default_theme('openpublish_theme');
  install_admin_theme('rubik');	
  variable_set('node_admin_theme', TRUE);    
  
  $theme_settings = variable_get('theme_settings', array());
  $theme_settings['toggle_node_info_page'] = FALSE;
  $theme_settings['default_logo'] = FALSE;
  $theme_settings['logo_path'] = 'sites/all/themes/openpublish_theme/images/logo.gif';
  variable_set('theme_settings', $theme_settings);    
  
  // Basic Drupal settings.
  variable_set('site_frontpage', 'node');
  variable_set('user_register', 1); 
  variable_set('user_pictures', '1');
  variable_set('statistics_count_content_views', 1);
  variable_set('filter_default_format', '1');
  
  // Set the default timezone name from the offset
  $offset = variable_get('date_default_timezone', '');
  $tzname = timezone_name_from_abbr("", $offset, 0);
  variable_set('date_default_timezone_name', $tzname);
  
  _openpublish_log(st('Configured basic settings'));
}

/**
 * Import cck definitions from included files
 */
function _openpublish_import_cck($file, &$context) {   
  // blog type is from drupal, so modify it
  if ($file->name == 'blog') {
    install_add_existing_field('blog', 'field_teaser', 'text_textarea');
    install_add_existing_field('blog', 'field_show_author_info', 'optionwidgets_onoff');      
  }
  else if ($file->name == 'feed') {
    $content = array();
    ob_start();
    include $file->filename; 
    ob_end_clean();
    $feed = (object)$content['type'];
    variable_set('feedapi_settings_feed', $feed->feedapi);
  }
  else {
    install_content_copy_import_from_file($file->filename);
  }
  
  $msg = st('Content Type @type setup', array('@type' => $file->name));
  _openpublish_log($msg);
  $context['message'] = $msg;
}  

/**
 * Configure user/role/permission data
 */
function _openpublish_set_permissions(&$context){
  
  // Profile Fields
  $profile_full_name = array(
    'title' => 'Full Name', 
	  'name' => 'profile_full_name',
    'category' => 'Author Info',
    'type' => 'textfield',
  	'required'=> 0,
  	'register'=> 0,
  	'visibility' => 2,		
  	'weight' => -10,	
  );
  $profile_job_title = array(
    'title' => 'Job Title', 
	  'name' => 'profile_job_title',
    'category' => 'Author Info',
    'type' => 'textfield',
  	'required'=> 0,
  	'register'=> 0,
  	'visibility' => 2,		
  	'weight' => -9,	
  );
  $profile_bio = array(
    'title' => 'Bio', 
	  'name' => 'profile_bio',
    'category' => 'Author Info',
    'type' => 'textarea',
  	'required'=> 0,
  	'register'=> 0,
  	'visibility' => 2,		
  	'weight' => -8,	
  );
  install_profile_field_add($profile_full_name);
  install_profile_field_add($profile_job_title);
  install_profile_field_add($profile_bio);
  
  $context['message'] = st('Configured Permissions');
}

/**
 * Set misc settings
 */
function _openpublish_initialize_settings(&$context){

  // Disable default Flag
  $flag = flag_get_flag('bookmarks');
  $flag->types = array();
  $flag->save();
 
  // Login Destination
  variable_set('ld_condition_type', 'pages');
  variable_set('ld_condition_snippet', 'user
user/login');
  variable_set('ld_url_type', 'snippet');
  variable_set('ld_destination', 0);
  variable_set('ld_url_destination', '
global $user;
if ($user->uid == 1 || in_array("administrator", $user->roles)) {
  return "admin";
}
else if (in_array("editor", $user->roles) || in_array("web editor", $user->roles))  {
  return "admin/content/node/filter";
}
else if (in_array("author", $user->roles)) {
  return "node/add";
}
return "user";');
  
  // Calais
  $calais_all = calais_api_get_all_entities();
  
  $calais_ignored = array('CalaisDocumentCategory', 'Anniversary', 'Currency', 'EmailAddress', 'FaxNumber', 'PhoneNumber', 'URL');
  $calais_used = array_diff($calais_all, $calais_ignored);
  
  $calais_entities = calais_get_entity_vocabularies();
  
  variable_set('calais_applied_entities_global', drupal_map_assoc($calais_used));
  
  variable_set('calais_node_blog_request', 2);
  variable_set('calais_node_blog_process', 'AUTO');
  variable_set('calais_node_article_request', 2);
  variable_set('calais_node_article_process', 'AUTO');
  variable_set('calais_node_audio_request', 2);
  variable_set('calais_node_audio_process', 'AUTO');
  variable_set('calais_node_video_request', 2);
  variable_set('calais_node_video_process', 'AUTO');
  variable_set('calais_node_op_image_request', 2);
  variable_set('calais_node_op_image_process', 'AUTO');
  variable_set('calais_node_resource_request', 2);
  variable_set('calais_node_resource_process', 'AUTO');
  variable_set('calais_node_event_request', 2);
  variable_set('calais_node_event_process', 'AUTO');
  variable_set('calais_node_feeditem_request', 2);
  variable_set('calais_node_feeditem_process', 'AUTO');
  
  // Config feed items to use SemanticProxy by default
  variable_set('calais_semanticproxy_field_feeditem', 'calais_feedapi_node');
  
  $node_types = array('article', 'blog', 'audio', 'video', 'op_image', 'resource', 'event', 'feeditem');
  foreach($node_types as $key) {
    if(!empty($calais_entities)) {
      foreach ($calais_entities as $entity => $vid) {
        if (!in_array($entity, $calais_ignored)) {
          db_query("INSERT INTO {vocabulary_node_types} (vid, type) values('%d','%s') ", $vid, $key);
        }
      }
    }
  }
 
  variable_set('calais_threshold_article', '.25');
  variable_set('calais_threshold_audio', '.25');
  variable_set('calais_threshold_blog', '.25');
  variable_set('calais_threshold_event', '.25');
  variable_set('calais_threshold_feeditem', '.25');
  variable_set('calais_threshold_op_image', '.25');
  variable_set('calais_threshold_resource', '.25');
  variable_set('calais_threshold_video', '.25');
  
  // Calais Geo
  $geo_vocabs = array();
  foreach($calais_entities as $key => $value) {
    if ($key == 'ProvinceOrState' || $key == 'City' || $key == 'Country') {
      $geo_vocabs[$value] = $value;
    }
  }
  variable_set('gmap_default', array('width' => '100%', 'height' => '300px', 'zoom' => '5', 'maxzoom' => '14'));
  variable_set('calais_geo_vocabularies', $geo_vocabs);
  variable_set('calais_geo_nodes_enabled', array('blog'=>'blog', 'article'=>'article', 'audio'=>'audio',
  				'event'=>'event','op_image'=>'op_image', 'resource'=>'resource', 'video'=>'video'));

  // Calais Tagmods
  variable_set('calais_tag_blacklist', 'Other, Person Professional, Quotation, Person Political, Person Travel, Person Professional Past, Person Political Past');
  
  // MoreLikeThis
  $target_types = array('blog' => 'blog', 'article' => 'article', 'event' => 'event', 'resource' => 'resource');
  variable_set('morelikethis_taxonomy_threshold_article', '.25');
  variable_set('morelikethis_taxonomy_threshold_blog', '.25');
  variable_set('morelikethis_taxonomy_threshold_event', '.25');
  variable_set('morelikethis_taxonomy_threshold_resource', '.25');
  variable_set('morelikethis_taxonomy_target_types_resource', $target_types);
  variable_set('morelikethis_taxonomy_target_types_event', $target_types);
  variable_set('morelikethis_taxonomy_target_types_article', $target_types);
  variable_set('morelikethis_taxonomy_target_types_blog', $target_types);
  variable_set('morelikethis_taxonomy_enabled_resource', 1);
  variable_set('morelikethis_taxonomy_enabled_article', 1);
  variable_set('morelikethis_taxonomy_enabled_blog', 1);
  variable_set('morelikethis_taxonomy_enabled_event', 1);
  variable_set('morelikethis_taxonomy_count_article', '5');
  variable_set('morelikethis_taxonomy_count_event', '5');
  variable_set('morelikethis_taxonomy_count_blog', '5');
  variable_set('morelikethis_gv_content_type_article', 1);
  variable_set('morelikethis_gv_content_type_blog', 1);
  variable_set('morelikethis_gv_content_type_event', 1);
  variable_set('morelikethis_gv_content_type_resource', 1);
  variable_set('morelikethis_flickr_content_type_resource ', 1);
  variable_set('morelikethis_flickr_content_type_event', 1);
  variable_set('morelikethis_flickr_content_type_blog', 1);
  variable_set('morelikethis_flickr_content_type_article', 1);
  variable_set('morelikethis_calais_default', 1); 
  variable_set('morelikethis_calais_relevance', '.45');  
  
  // Topic Hubs
  variable_set('topic_hub_plugin_type_default', array('blog' => 'blog', 'article' => 'article' ,'audio' => 'audio',
    'event' => 'event', 'op_image' => 'op_image', 'video' => 'video'));
  variable_set('topichubs_contrib_ignore', array(1=>1));
  
  $msg = st('Setup general configuration');
  _openpublish_log($msg);
  $context['message'] = $msg;
}

/**
 * Create some content of type "page" as placeholders for content
 * and so menu items can be created
 */
function _openpublish_placeholder_content(&$context) {
  global $base_url;  

  $user = user_load(array('uid' => 1));
 
  $page = array (
    'type' => 'page',
    'language' => 'en',
    'uid' => 1,
    'status' => 1,
    'comment' => 0,
    'promote' => 0,
    'moderate' => 0,
    'sticky' => 0,
    'tnid' => 0,
    'translate' => 0,    
    'revision_uid' => 1,
    'title' => st('Default'),
    'body' => 'Placeholder',    
    'format' => 2,
    'name' => $user->name,
  );
  
  $about_us = (object) $page;
  $about_us->title = st('About Us');
  node_save($about_us);	
  
  $adverstise = (object) $page;
  $adverstise->title = st('Advertise');
  node_save($adverstise);	
  
  $subscribe = (object) $page;
  $subscribe->title = st('Subscribe');
  node_save($subscribe);	
  
  $rss = (object) $page;
  $rss->title = st('RSS Feeds List');
  $rss->body = '<p><strong>Articles</strong><ul><li><a href="'. $base_url . '/rss/articles/all">All Categories</a></li><li><a href="'. $base_url . '/rss/articles/Business">Business</a></li><li><a href="'. $base_url . '/rss/articles/Health">Health</a></li><li><a href="'. $base_url . '/rss/articles/Politics">Politics</a></li><li><a href="'. $base_url . '/rss/articles/Technology">Technology</a></li><li><a href="'. $base_url . '/rss/blogs">Blogs</a></li><li><a href="/rss/events">Events</a></li><li><a href="'. $base_url . '/rss/resources">Resources</a></li><li><a href="'. $base_url . '/rss/multimedia">Multimedia</a></li></p>';
  node_save($rss);
  
  $jobs = (object) $page;
  $jobs->title = st('Jobs');
  node_save($jobs);
  
  $store = (object) $page;
  $store->title = st('Store');
  node_save($store);
  
  $sitemap = (object) $page;
  $sitemap->title = st('Site Map');
  node_save($sitemap);
  
  $termsofuse = (object) $page;
  $termsofuse->title = st('Terms of Use');
  node_save($termsofuse);
  
  $privacypolicy = (object) $page;
  $privacypolicy->title = st('Privacy Policy');
  node_save($privacypolicy); 
  
  menu_rebuild();
  
  $context['message'] = st('Installed Content');
}

/**
 * Load views
 */
function _openpublish_set_views() {
  views_include_default_views();
  
  //popular view is disabled by default, enable it
  $view = views_get_view('popular');
  $view->disabled = FALSE;
  $view->save();
} 

/**
 * Setup custom menus and primary links.
 */
function _openpublish_install_menus(&$context) {
  cache_clear_all();
  menu_rebuild();
  
  // TODO: Rework into new Dashboard
  $op_plid = install_menu_create_menu_item('admin/settings/openpublish/api-keys', 'OpenPublish Control Panel', 'Short cuts to important functionality.', 'navigation', 1, -49);
  install_menu_create_menu_item('admin/settings/openpublish/api-keys', 'API Keys', 'Calais, Apture and Flickr API keys.', 'navigation', $op_plid, 1);
  install_menu_create_menu_item('admin/settings/openpublish/calais-suite', 'Calais Suite', 'Administrative links to Calais, More Like This and Topic Hubs functionality.', 'navigation', $op_plid, 2);
  install_menu_create_menu_item('admin<?php
/**
 * @file openpublish.profile
 *
 * TODO:
 *  - More general variable handling
 *  - Integrate Content for better block handling
 */

/**
 * Implementation of hook_profile_details()
 */
function openpublish_profile_details() {  
  return array(
    'name' => 'OpenPublish',
    'description' => st('The power of Drupal for today\'s online publishing from Phase2 Technology.'),
  );
} 

/**
 * Implementation of hook_profile_modules().
 */
function openpublish_profile_modules() {
  $core_modules = array(
    // Required core modules
    'block', 'filter', 'node', 'system', 'user', 'profile',

    // Optional core modules.
    'blog', 'comment', 'help', 'locale', 'menu', 'openid', 'path',
	  'statistics', 'search', 'taxonomy', 'translation', 'upload', 
  );

  $contributed_modules = array(
    //misc stand-alone, required by others
    'admin', 'rdf', 'token', 'gmap', 'devel', 'bulk_export', 'flickrapi', 'autoload', 
    'ckeditor', 'flag', 'imce', 'imce_mkdir', 'mollom', 'nodewords', 'nodewords_basic', 'paging',
    'pathauto', 'tabs', 'login_destination', 'cmf', 'install_profile_api','scheduler','advuser',
    'jquery_ui', 'jquery_update', 'modalframe', 'nodequeue', 'twitter_pull', 'advanced_help', 'ie_css_optimizer',
    'node_embed', 'nodeblock', 'libraries', 
    'mimemail', 'op_workflow_bonus', 'workflow', 'translation_helpers',

    //date
    'date_api', 'date', 'date_timezone', 'date_popup',
  
    //imagecache
    'imageapi', 'imageapi_gd', 'imagecache', 'imagecache_ui', 'imagecrop', 
  
    //cck
    'content', 'content_copy', 'emfield', 'emaudio', 'emimage', 
    'emvideo', 'fieldgroup', 'filefield', 'imagefield', 'filefield_sources', 'link', 'number',
    'optionwidgets', 'text', 'nodereference', 'noderelationships', 'userreference',
	
    // Calais
    'calais_api', 'calais', 'calais_geo', 'calais_tagmods',
    
    // Feed API
    //'feedapi', 'feedapi_node', 'feedapi_inherit', 'feedapi_mapper', 'parser_simplepie', 
    
    // Feeds
    'job_scheduler', 'feeds', 'feeds_ui',

    // More Like this
    'morelikethis', 'morelikethis_flickr', 'morelikethis_googlevideo', 'morelikethis_taxonomy',
    'morelikethis_yboss',
	
    //swftools
    'swftools', 'swfobject2', 'flowplayer3', 'onepixelout',
	
    //views
    'views', 'views_export', 'views_ui', 
    
    //gallery stuff
    'jcarousel', 'viewscarousel',
    
    'premium', 'premium_views_field', 'premium_default_off',

    // ctools, panels
	  'ctools', 'page_manager', 'panels', 'panels_node', 
      //'ctools_custom_content', 'views_content',
	  
	  //context
	  'context','context_ui',
  
    // requries ctools
    'strongarm', 

    //topic hubs
    'topichubs', 'topichubs_calais_geo', 'topichubs_contributors', 'topichubs_most_comments',
    'topichubs_most_recent', 'topichubs_most_viewed', 'topichubs_panels', 'topichubs_recent_comments',
    'topichubs_related_topics',
    
    // distribution management
    'distro_client', 'features', 'diff', 
    
    // misc modules easing development/maintenance
    'custompage', 'custompage_ui', 'openidadmin',
    
    // l10n
    'l10n_update',
  
    // Document Cloud
    'document_cloud',
  );

  return array_merge($core_modules, $contributed_modules);
} 

/**
 * Features module and OpenPublish features
 */
function openpublish_feature_modules() {
  $features = array(
    'op_imce_config',
	  'op_author', 
	  'op_author_layout',	  
	  'op_author_panels', 
    'op_advuser_config', 
    'op_article', 
    'op_audio',	  
	  'op_blog', 
	  'op_event',
	  'op_image',
	  'op_imagecrop_config', 
	  'op_misc',
	  'op_package',
	  'op_resource',
	  'op_scheduler_config', 
	  'op_slideshow', 
	  'op_videos',	  
	  'op_defaults',
	  'op_contexts',
	  //'op_facebook',
    'op_editors_choice',
    'op_default_workflow',
    'op_default_workflow_nodetypes',
    
    // Rules needs to be enabled after op_default_workflow which contains default rules for the default workflow
    'rules', 
    'rules_admin',
	  
	  // Custom modules developed for OpenPublish. Openpublish_core needs op_advuser_config to run first.
	  'openpublish_core', 'openpublish_administration', 'openpublish_menu', 'openpublish_popular_terms',
  );
  return $features;
}

/**
 * Return a list of tasks that this profile supports.
 *
 * @return
 *   A keyed array of tasks the profile will perform during
 *   the final stage. The keys of the array will be used internally,
 *   while the values will be displayed to the user in the installer
 *   task list.
 */
function openpublish_profile_task_list() {
  global $conf;
  $conf['site_name'] = 'OpenPublish';
  $conf['site_footer'] = 'OpenPublish by <a href="http://phase2technology.com">Phase2 Technology</a>';
  $conf['theme_settings'] = array(
    'default_logo' => 0,
    'logo_path' => 'sites/all/themes/openpublish_theme/images/openpublish-logo.png',
  );
  
  $tasks['op-configure-batch'] = st('Configure OpenPublish');
  
  if (_openpublish_language_selected()) {
    $tasks['op-translation-import-batch'] = st('Importing translations');
  }
  
  return $tasks;
}

/**
 * Implementation of hook_profile_tasks().
 */
function openpublish_profile_tasks(&$task, $url) {
  global $install_locale;
  $output = "";
  
  install_include(openpublish_profile_modules());

  if($task == 'profile') {
    drupal_set_title(t('OpenPublish Installation'));
    _openpublish_log(t('Starting Installation'));
    _openpublish_base_settings();
    $task = "op-configure";
  }
    
  if($task == 'op-configure') {
    $batch['title'] = st('Configuring @drupal', array('@drupal' => drupal_install_profile_name()));
    $files = module_rebuild_cache();
    $cck_files = file_scan_directory ( dirname(__FILE__) . '/cck' , '.*\.inc$' );
    foreach ( $cck_files as $file ) {   
      $batch['operations'][] = array('_openpublish_import_cck', array($file));      
    }    
    foreach ( openpublish_feature_modules() as $feature ) {   
      $batch['operations'][] = array('_install_module_batch', array($feature, $files[$feature]->info['name']));      
      //-- Initialize each feature individually rather then all together in the end, to avoid execution timeout.
      $batch['operations'][] = array('features_flush_caches', array()); 
    }    
    $batch['operations'][] = array('_openpublish_set_permissions', array());      
    $batch['operations'][] = array('_openpublish_initialize_settings', array());      
    $batch['operations'][] = array('_openpublish_placeholder_content', array());      
    $batch['operations'][] = array('_openpublish_set_views', array());      
    $batch['operations'][] = array('_openpublish_install_menus', array());      
    $batch['operations'][] = array('_openpublish_setup_blocks', array()); 
    $batch['operations'][] = array('_openpublish_cleanup', array());      
    $batch['error_message'] = st('There was an error configuring @drupal.', array('@drupal' => drupal_install_profile_name()));
    $batch['finished'] = '_openpublish_configure_finished';
    variable_set('install_task', 'op-configure-batch');
    batch_set($batch);
    batch_process($url, $url);
  }

  if ($task == 'op-translation-import') {
    if (_openpublish_language_selected() && module_exists('l10n_update')) {
      module_load_install('l10n_update');
      module_load_include('batch.inc', 'l10n_update');

      $history = l10n_update_get_history();
      $available = l10n_update_available_releases();
      $updates = l10n_update_build_updates($history, $available);

      // Filter out updates in other languages. If no languages, all of them will be updated
      $updates = _l10n_update_prepare_updates($updates, NULL, array($install_locale));

      // Edited strings are kept, only default ones (previously imported)
      // are overwritten and new strings are added
      $mode = 1;

      if ($batch = l10n_update_batch_multiple($updates, $mode)) {
        $batch['finished'] = '_openpublish_import_translations_finished';
        variable_set('install_task', 'op-translation-import-batch');
        batch_set($batch);
        batch_process($url, $url);
      }
    }
  }
  
  // Land here until the batches are done
  if (in_array($task, array('op-translation-import-batch', 'op-configure-batch'))) {
    include_once 'includes/batch.inc';
    $output = _batch_page();
  }
    
  return $output;
} 

/**
 * Translation import process is finished, move on to the next step
 */
function _openpublish_import_translations_finished($success, $results) {
  _openpublish_log(t('Translations have been imported.'));
  /**
   * Necessary as the openpublish_theme's status gets reset to 0
   * by a part of the automated batch translation in l10n_update
   */
  install_default_theme('openpublish_theme');
  variable_set('install_task', 'profile-finished');
}

/**
 * Import process is finished, move on to the next step
 */
function _openpublish_configure_finished($success, $results) {
  _openpublish_log(t('OpenPublish has been installed.'));
  if (_openpublish_language_selected()) {
    // Other language, different part of the process
    variable_set('install_task', 'op-translation-import');
  }
  else {
    // English installation
    variable_set('install_task', 'profile-finished');
  }
}

/**
 * Do some basic setup
 */
function _openpublish_base_settings() {  
  global $base_url;  

  // create pictures dir
  $pictures_path = file_create_path('pictures');
  file_check_directory($pictures_path,TRUE);

  // Set distro tracker server URL for this distribution
  distro_set_tracker_server('http://tracker.openpublishapp.com/distro/components');
  variable_set('openpublish_version', '2.3'); 
 
  $types = array(
    array(
      'type' => 'page',
      'name' => st('Page'),
      'module' => 'node',
      'description' => st("A <em>page</em>, similar in form to a <em>story</em>, is a simple method for creating and displaying information that rarely changes, such as an \"About us\" section of a website. By default, a <em>page</em> entry does not allow visitor comments and is not featured on the site's initial home page."),
      'custom' => TRUE,
      'modified' => TRUE,
      'locked' => FALSE,
      'help' => '',
      'min_word_count' => '',
    ),   
  );

  foreach ($types as $type) {
    $type = (object) _node_type_set_defaults($type);
    node_type_save($type);
  }

  // Default page to not be promoted and have comments disabled.
  variable_set('node_options_page', array('status'));
  variable_set('comment_page', COMMENT_NODE_DISABLED);

  // Theme related.  
  install_default_theme('openpublish_theme');
  install_admin_theme('rubik');	
  variable_set('node_admin_theme', TRUE);    
  
  $theme_settings = variable_get('theme_settings', array());
  $theme_settings['toggle_node_info_page'] = FALSE;
  $theme_settings['default_logo'] = FALSE;
  $theme_settings['logo_path'] = 'sites/all/themes/openpublish_theme/images/logo.gif';
  variable_set('theme_settings', $theme_settings);    
  
  // Basic Drupal settings.
  variable_set('site_frontpage', 'node');
  variable_set('user_register', 1); 
  variable_set('user_pictures', '1');
  variable_set('statistics_count_content_views', 1);
  variable_set('filter_default_format', '1');
  
  // Set the default timezone name from the offset
  $offset = variable_get('date_default_timezone', '');
  $tzname = timezone_name_from_abbr("", $offset, 0);
  variable_set('date_default_timezone_name', $tzname);
  
  _openpublish_log(st('Configured basic settings'));
}

/**
 * Import cck definitions from included files
 */
function _openpublish_import_cck($file, &$context) {   
  // blog type is from drupal, so modify it
  if ($file->name == 'blog') {
    install_add_existing_field('blog', 'field_teaser', 'text_textarea');
    install_add_existing_field('blog', 'field_show_author_info', 'optionwidgets_onoff');      
  }
  else if ($file->name == 'feed') {
    $content = array();
    ob_start();
    include $file->filename; 
    ob_end_clean();
    $feed = (object)$content['type'];
    variable_set('feedapi_settings_feed', $feed->feedapi);
  }
  else {
    install_content_copy_import_from_file($file->filename);
  }
  
  $msg = st('Content Type @type setup', array('@type' => $file->name));
  _openpublish_log($msg);
  $context['message'] = $msg;
}  

/**
 * Configure user/role/permission data
 */
function _openpublish_set_permissions(&$context){
  
  // Profile Fields
  $profile_full_name = array(
    'title' => 'Full Name', 
	  'name' => 'profile_full_name',
    'category' => 'Author Info',
    'type' => 'textfield',
  	'required'=> 0,
  	'register'=> 0,
  	'visibility' => 2,		
  	'weight' => -10,	
  );
  $profile_job_title = array(
    'title' => 'Job Title', 
	  'name' => 'profile_job_title',
    'category' => 'Author Info',
    'type' => 'textfield',
  	'required'=> 0,
  	'register'=> 0,
  	'visibility' => 2,		
  	'weight' => -9,	
  );
  $profile_bio = array(
    'title' => 'Bio', 
	  'name' => 'profile_bio',
    'category' => 'Author Info',
    'type' => 'textarea',
  	'required'=> 0,
  	'register'=> 0,
  	'visibility' => 2,		
  	'weight' => -8,	
  );
  install_profile_field_add($profile_full_name);
  install_profile_field_add($profile_job_title);
  install_profile_field_add($profile_bio);
  
  $context['message'] = st('Configured Permissions');
}

/**
 * Set misc settings
 */
function _openpublish_initialize_settings(&$context){

  // Disable default Flag
  $flag = flag_get_flag('bookmarks');
  $flag->types = array();
  $flag->save();
 
  // Login Destination
  variable_set('ld_condition_type', 'pages');
  variable_set('ld_condition_snippet', 'user
user/login');
  variable_set('ld_url_type', 'snippet');
  variable_set('ld_destination', 0);
  variable_set('ld_url_destination', '
global $user;
if ($user->uid == 1 || in_array("administrator", $user->roles)) {
  return "admin";
}
else if (in_array("editor", $user->roles) || in_array("web editor", $user->roles))  {
  return "admin/content/node/filter";
}
else if (in_array("author", $user->roles)) {
  return "node/add";
}
return "user";');
  
  // Calais
  $calais_all = calais_api_get_all_entities();
  
  $calais_ignored = array('CalaisDocumentCategory', 'Anniversary', 'Currency', 'EmailAddress', 'FaxNumber', 'PhoneNumber', 'URL');
  $calais_used = array_diff($calais_all, $calais_ignored);
  
  $calais_entities = calais_get_entity_vocabularies();
  
  variable_set('calais_applied_entities_global', drupal_map_assoc($calais_used));
  
  variable_set('calais_node_blog_request', 2);
  variable_set('calais_node_blog_process', 'AUTO');
  variable_set('calais_node_article_request', 2);
  variable_set('calais_node_article_process', 'AUTO');
  variable_set('calais_node_audio_request', 2);
  variable_set('calais_node_audio_process', 'AUTO');
  variable_set('calais_node_video_request', 2);
  variable_set('calais_node_video_process', 'AUTO');
  variable_set('calais_node_op_image_request', 2);
  variable_set('calais_node_op_image_process', 'AUTO');
  variable_set('calais_node_resource_request', 2);
  variable_set('calais_node_resource_process', 'AUTO');
  variable_set('calais_node_event_request', 2);
  variable_set('calais_node_event_process', 'AUTO');
  variable_set('calais_node_feeditem_request', 2);
  variable_set('calais_node_feeditem_process', 'AUTO');
  
  // Config feed items to use SemanticProxy by default
  variable_set('calais_semanticproxy_field_feeditem', 'calais_feedapi_node');
  
  $node_types = array('article', 'blog', 'audio', 'video', 'op_image', 'resource', 'event', 'feeditem');
  foreach($node_types as $key) {
    if(!empty($calais_entities)) {
      foreach ($calais_entities as $entity => $vid) {
        if (!in_array($entity, $calais_ignored)) {
          db_query("INSERT INTO {vocabulary_node_types} (vid, type) values('%d','%s') ", $vid, $key);
        }
      }
    }
  }
 
  variable_set('calais_threshold_article', '.25');
  variable_set('calais_threshold_audio', '.25');
  variable_set('calais_threshold_blog', '.25');
  variable_set('calais_threshold_event', '.25');
  variable_set('calais_threshold_feeditem', '.25');
  variable_set('calais_threshold_op_image', '.25');
  variable_set('calais_threshold_resource', '.25');
  variable_set('calais_threshold_video', '.25');
  
  // Calais Geo
  $geo_vocabs = array();
  foreach($calais_entities as $key => $value) {
    if ($key == 'ProvinceOrState' || $key == 'City' || $key == 'Country') {
      $geo_vocabs[$value] = $value;
    }
  }
  variable_set('gmap_default', array('width' => '100%', 'height' => '300px', 'zoom' => '5', 'maxzoom' => '14'));
  variable_set('calais_geo_vocabularies', $geo_vocabs);
  variable_set('calais_geo_nodes_enabled', array('blog'=>'blog', 'article'=>'article', 'audio'=>'audio',
  				'event'=>'event','op_image'=>'op_image', 'resource'=>'resource', 'video'=>'video'));

  // Calais Tagmods
  variable_set('calais_tag_blacklist', 'Other, Person Professional, Quotation, Person Political, Person Travel, Person Professional Past, Person Political Past');
  
  // MoreLikeThis
  $target_types = array('blog' => 'blog', 'article' => 'article', 'event' => 'event', 'resource' => 'resource');
  variable_set('morelikethis_taxonomy_threshold_article', '.25');
  variable_set('morelikethis_taxonomy_threshold_blog', '.25');
  variable_set('morelikethis_taxonomy_threshold_event', '.25');
  variable_set('morelikethis_taxonomy_threshold_resource', '.25');
  variable_set('morelikethis_taxonomy_target_types_resource', $target_types);
  variable_set('morelikethis_taxonomy_target_types_event', $target_types);
  variable_set('morelikethis_taxonomy_target_types_article', $target_types);
  variable_set('morelikethis_taxonomy_target_types_blog', $target_types);
  variable_set('morelikethis_taxonomy_enabled_resource', 1);
  variable_set('morelikethis_taxonomy_enabled_article', 1);
  variable_set('morelikethis_taxonomy_enabled_blog', 1);
  variable_set('morelikethis_taxonomy_enabled_event', 1);
  variable_set('morelikethis_taxonomy_count_article', '5');
  variable_set('morelikethis_taxonomy_count_event', '5');
  variable_set('morelikethis_taxonomy_count_blog', '5');
  variable_set('morelikethis_gv_content_type_article', 1);
  variable_set('morelikethis_gv_content_type_blog', 1);
  variable_set('morelikethis_gv_content_type_event', 1);
  variable_set('morelikethis_gv_content_type_resource', 1);
  variable_set('morelikethis_flickr_content_type_resource ', 1);
  variable_set('morelikethis_flickr_content_type_event', 1);
  variable_set('morelikethis_flickr_content_type_blog', 1);
  variable_set('morelikethis_flickr_content_type_article', 1);
  variable_set('morelikethis_calais_default', 1); 
  variable_set('morelikethis_calais_relevance', '.45');  
  
  // Topic Hubs
  variable_set('topic_hub_plugin_type_default', array('blog' => 'blog', 'article' => 'article' ,'audio' => 'audio',
    'event' => 'event', 'op_image' => 'op_image', 'video' => 'video'));
  variable_set('topichubs_contrib_ignore', array(1=>1));
  
  $msg = st('Setup general configuration');
  _openpublish_log($msg);
  $context['message'] = $msg;
}

/**
 * Create some content of type "page" as placeholders for content
 * and so menu items can be created
 */
function _openpublish_placeholder_content(&$context) {
  global $base_url;  

  $user = user_load(array('uid' => 1));
 
  $page = array (
    'type' => 'page',
    'language' => 'en',
    'uid' => 1,
    'status' => 1,
    'comment' => 0,
    'promote' => 0,
    'moderate' => 0,
    'sticky' => 0,
    'tnid' => 0,
    'translate' => 0,    
    'revision_uid' => 1,
    'title' => st('Default'),
    'body' => 'Placeholder',    
    'format' => 2,
    'name' => $user->name,
  );
  
  $about_us = (object) $page;
  $about_us->title = st('About Us');
  node_save($about_us);	
  
  $adverstise = (object) $page;
  $adverstise->title = st('Advertise');
  node_save($adverstise);	
  
  $subscribe = (object) $page;
  $subscribe->title = st('Subscribe');
  node_save($subscribe);	
  
  $rss = (object) $page;
  $rss->title = st('RSS Feeds List');
  $rss->body = '<p><strong>Articles</strong><ul><li><a href="'. $base_url . '/rss/articles/all">All Categories</a></li><li><a href="'. $base_url . '/rss/articles/Business">Business</a></li><li><a href="'. $base_url . '/rss/articles/Health">Health</a></li><li><a href="'. $base_url . '/rss/articles/Politics">Politics</a></li><li><a href="'. $base_url . '/rss/articles/Technology">Technology</a></li><li><a href="'. $base_url . '/rss/blogs">Blogs</a></li><li><a href="/rss/events">Events</a></li><li><a href="'. $base_url . '/rss/resources">Resources</a></li><li><a href="'. $base_url . '/rss/multimedia">Multimedia</a></li></p>';
  node_save($rss);
  
  $jobs = (object) $page;
  $jobs->title = st('Jobs');
  node_save($jobs);
  
  $store = (object) $page;
  $store->title = st('Store');
  node_save($store);
  
  $sitemap = (object) $page;
  $sitemap->title = st('Site Map');
  node_save($sitemap);
  
  $termsofuse = (object) $page;
  $termsofuse->title = st('Terms of Use');
  node_save($termsofuse);
  
  $privacypolicy = (object) $page;
  $privacypolicy->title = st('Privacy Policy');
  node_save($privacypolicy); 
  
  menu_rebuild();
  
  $context['message'] = st('Installed Content');
}

/**
 * Load views
 */
function _openpublish_set_views() {
  views_include_default_views();
  
  //popular view is disabled by default, enable it
  $view = views_get_view('popular');
  $view->disabled = FALSE;
  $view->save();
} 

/**
 * Setup custom menus and primary links.
 */
function _openpublish_install_menus(&$context) {
  cache_clear_all();
  menu_rebuild();
  
  // TODO: Rework into new Dashboard
  $op_plid = install_menu_create_menu_item('admin/settings/openpublish/api-keys', 'OpenPublish Control Panel', 'Short cuts to important functionality.', 'navigation', 1, -49);
  install_menu_create_menu_item('admin/settings/openpublish/api-keys', 'API Keys', 'Calais, Apture and Flickr API keys.', 'navigation', $op_plid, 1);
  install_menu_create_menu_item('admin/settings/openpublish/calais-suite', 'Calais Suite', 'Administrative links to Calais, More Like This and Topic Hubs functionality.', 'navigation', $op_plid, 2);
  install_menu_create_menu_item('admin/settings/openpublish/content', 'Content Links', 'Administrative links to content, comment, feed and taxonomy management.', 'navigation', $op_plid, 3);

  // Primary Navigation
  install_menu_create_menu_item('<front>',             'Home',       '', 'primary-links', 0, 1);
  
  // Business -- Finance, Markets, Personal Finance, Management 
  $business_plid = install_menu_create_menu_item('articles/Business',   'Business',       '', 'primary-links', 0, 2);
  install_menu_create_menu_item('articles/Finance',             'Finance',                '', 'primary-links', $business_plid, 1);
  install_menu_create_menu_item('articles/Markets',             'Markets',                '', 'primary-links', $business_plid, 2);
  install_menu_create_menu_item('articles/Personal Finance',    'Personal Finance',       '', 'primary-links', $business_plid, 3);
  install_menu_create_menu_item('articles/Management',          'Management',             '', 'primary-links', $business_plid, 4);
  
  // Healthcare -- Medicine, Fitness, Nutrition
  $health_plid = install_menu_create_menu_item('articles/Health',     'Health',     '', 'primary-links', 0, 3);
  install_menu_create_menu_item('articles/Medicine',     'Medicine',   '', 'primary-links', $health_plid, 1);
  install_menu_create_menu_item('articles/Fitness',     'Fitness',     '', 'primary-links', $health_plid, 2);
  install_menu_create_menu_item('articles/Nutrition',     'Nutrition', '', 'primary-links', $health_plid, 3);
  
  // Politics -- Local, National, International
  $politics_plid = install_menu_create_menu_item('articles/Politics',   'Politics',   '', 'primary-links', 0, 4);
  install_menu_create_menu_item('articles/Local',      'Local',   '', 'primary-links', $politics_plid, 1);
  install_menu_create_menu_item('articles/National',   'National',   '', 'primary-links', $politics_plid, 2);
  install_menu_create_menu_item('articles/International',   'International',   '', 'primary-links', $politics_plid, 3);
  
  // Technology --  Internet, Gadgets, Biotech, Social Media, Gaming
  $tech_plid = install_menu_create_menu_item('articles/Technology', 'Technology', '', 'primary-links', 0, 5);
  install_menu_create_menu_item('articles/Internet',   'Internet',   '', 'primary-links', $tech_plid, 1);
  install_menu_create_menu_item('articles/Gadgets',   'Gadgets',   '', 'primary-links', $tech_plid, 2);
  install_menu_create_menu_item('articles/Biotech',   'Biotech',   '', 'primary-links', $tech_plid, 3);
  install_menu_create_menu_item('articles/Social Media',   'Social Media',   '', 'primary-links', $tech_plid, 4);
  install_menu_create_menu_item('articles/Gaming',   'Gaming',   '', 'primary-links', $tech_plid, 5);
  
  install_menu_create_menu_item('blogs',               'Blogs',      '', 'primary-links', 0, 6);
  install_menu_create_menu_item('resources',           'Resources',  '', 'primary-links', 0, 7);
  install_menu_create_menu_item('events',              'Events',     '', 'primary-links', 0, 8);
  install_menu_create_menu_item('topic-hubs',          'Topic Hubs', '', 'primary-links', 0, 9);

  install_menu_create_menu('Footer Primary', 'footer-primary');
  install_menu_create_menu_item('articles/all', 'Latest News', '', 'menu-footer-primary', 0, 1);
  // install_menu_create_menu_item('popular/all',  'Hot Topics',  '', 'menu-footer-primary', 0, 2);
  install_menu_create_menu_item('blogs',        'Blogs',       '', 'menu-footer-primary', 0, 3);
  install_menu_create_menu_item('resources',    'Resources',   '', 'menu-footer-primary', 0, 4);
  install_menu_create_menu_item('events',       'Events',      '', 'menu-footer-primary', 0, 5);

  install_menu_create_menu('Footer Secondary', 'footer-secondary');
  install_menu_create_menu_item('node/3', 'Subscribe',      '', 'menu-footer-secondary', 0, 1);
  install_menu_create_menu_item('node/2', 'Advertise',      '', 'menu-footer-secondary', 0, 2);
  install_menu_create_menu_item('node/5', 'Jobs',           '', 'menu-footer-secondary', 0, 3);
  install_menu_create_menu_item('node/6', 'Store',          '', 'menu-footer-secondary', 0, 4);
  install_menu_create_menu_item('node/1', 'About Us',       '', 'menu-footer-secondary', 0, 5);
  install_menu_create_menu_item('node/7', 'Site Map',       '', 'menu-footer-secondary', 0, 6);
  install_menu_create_menu_item('node/8', 'Terms of Use',   '', 'menu-footer-secondary', 0, 7);
  install_menu_create_menu_item('node/9', 'Privacy Policy', '', 'menu-footer-secondary', 0, 8);

  install_menu_create_menu('Top Menu', 'top-menu'); 
  install_menu_create_menu_item('node/1', 'About Us',  '', 'menu-top-menu', 0, 1);
  install_menu_create_menu_item('node/2', 'Advertise', '', 'menu-top-menu', 0, 2);
  install_menu_create_menu_item('node/3', 'Subscribe', '', 'menu-top-menu', 0, 3);
  install_menu_create_menu_item('node/4', 'RSS',       '', 'menu-top-menu', 0, 4);
  
  $msg = st('Installed Menus');
  _openpublish_log($msg);
  $context['message'] = $msg;
} 

/**
 * Create custom blocks and set region and pages.
 */
function _openpublish_setup_blocks(&$context) {  
  global $theme_key, $base_url; 
  cache_clear_all();

  // Ensures that $theme_key gets set for new block creation
  $theme_key = 'openpublish_theme';

  // install the demo ad blocks  
  $ad_base = $base_url . '/sites/all/themes/openpublish_theme/images/banner';
  $b1 = install_create_custom_block('<p id="credits"><a href="http://www.phase2technology.com/" target="_blank">Powered by Phase2 Technology</a></p>', 'Credits', FILTER_HTML_ESCAPE);
  $b2 = install_create_custom_block('<a href="http://openpublishapp.com"><img src="' . $ad_base . '/banner_openpublish.jpg"/></a><div class="clear"></div>', 'Top Banner Ad', 2);
  $b3 = install_create_custom_block('<p><a href="http://phase2technology.com"><img src="' . $ad_base . '/banner_phase2.jpg"/></a></p>', 'Right Block Square Ad', 2);
  $b4 = install_create_custom_block('<p><a href="http://tattlerapp.com"><img src="' . $ad_base . '/banner_tattler.jpg"/></a></p>', 'Homepage Ad Block 1', 2);
  $b5 = install_create_custom_block('<p><a href="http://phase2technology.com"><img src="' . $ad_base . '/banner_phase2.jpg"/></a></p>', 'Homepage Ad Block 2', FILTER_HTML_ESCAPE);

  // Get these new boxes in blocks table
  install_init_blocks();

  //-- Disable titles for all views-driven blocks, by default, to avoid double-titling:
  db_query("UPDATE {blocks} SET title = '%s' WHERE module = '%s' AND theme= '%s'", 
            '<none>', 'views', 'openpublish_theme');

  _openpublish_set_block_title('Google Videos Like This', 'morelikethis', 'googlevideo', 'openpublish_theme');
  _openpublish_set_block_title('Flickr Images Like This', 'morelikethis', 'flickr', 'openpublish_theme');
  _openpublish_set_block_title('Recommended Reading', 'morelikethis', 'taxonomy', 'openpublish_theme');

  install_disable_block('user', '0', 'openpublish_theme');
  install_disable_block('user', '1', 'openpublish_theme');
  install_disable_block('system', '0', 'openpublish_theme');
  
  $msg = st('Configured Blocks');
  _openpublish_log($msg);
  $context['message'] = $msg;
}

/**
 * Helper for setting a block's title only.
 */
function _openpublish_set_block_title($title, $module, $delta, $theme) {
  db_query("UPDATE {blocks} SET title = '%s' WHERE module = '%s' AND delta = '%s' AND theme= '%s'", $title, $module, $delta, $theme);
}

/**
 * Cleanup after the install
 */
function _openpublish_cleanup() {
  // DO NOT call drupal_flush_all_caches(), it disables all themes
  $functions = array(
    'drupal_rebuild_theme_registry',
    'menu_rebuild',
    'install_init_blocks',
    'views_invalidate_cache',    
    'node_types_rebuild',    
  );
  
  foreach ($functions as $func) {
    //$start = time();
    $func();
    //$elapsed = time() - $start;
    //error_log("####  $func took $elapsed seconds ###");
  }
  
  ctools_flush_caches(); 
  cache_clear_all('*', 'cache', TRUE);  
  cache_clear_all('*', 'cache_content', TRUE);
}

/**
 * Set OpenPublish as the default install profile
 */
function system_form_install_select_profile_form_alter(&$form, $form_state) {
  foreach($form['profile'] as $key => $element) {
    $form['profile'][$key]['#value'] = 'openpublish';
  }
}


/**
 * Consolidate logging.
 */
function _openpublish_log($msg) {
  error_log($msg);
  drupal_set_message($msg);
}

/**
 * Checks if installation is being done in a language other than English
 */
function _openpublish_language_selected() {
  global $install_locale;
  return !empty($install_locale) && ($install_locale != 'en');
}
/settings/openpublish/content', 'Content Links', 'Administrative links to content, comment, feed and taxonomy management.', 'navigation', $op_plid, 3);

  // Primary Navigation
  install_menu_create_menu_item('<front>',             'Home',       '', 'primary-links', 0, 1);
  
  // Business -- Finance, Markets, Personal Finance, Management 
  $business_plid = install_menu_create_menu_item('articles/Business',   'Business',       '', 'primary-links', 0, 2);
  install_menu_create_menu_item('articles/Finance',             'Finance',                '', 'primary-links', $business_plid, 1);
  install_menu_create_menu_item('articles/Markets',             'Markets',                '', 'primary-links', $business_plid, 2);
  install_menu_create_menu_item('articles/Personal Finance',    'Personal Finance',       '', 'primary-links', $business_plid, 3);
  install_menu_create_menu_item('articles/Management',          'Management',             '', 'primary-links', $business_plid, 4);
  
  // Healthcare -- Medicine, Fitness, Nutrition
  $health_plid = install_menu_create_menu_item('articles/Health',     'Health',     '', 'primary-links', 0, 3);
  install_menu_create_menu_item('articles/Medicine',     'Medicine',   '', 'primary-links', $health_plid, 1);
  install_menu_create_menu_item('articles/Fitness',     'Fitness',     '', 'primary-links', $health_plid, 2);
  install_menu_create_menu_item('articles/Nutrition',     'Nutrition', '', 'primary-links', $health_plid, 3);
  
  // Politics -- Local, National, International
  $politics_plid = install_menu_create_menu_item('articles/Politics',   'Politics',   '', 'primary-links', 0, 4);
  install_menu_create_menu_item('articles/Local',      'Local',   '', 'primary-links', $politics_plid, 1);
  install_menu_create_menu_item('articles/National',   'National',   '', 'primary-links', $politics_plid, 2);
  install_menu_create_menu_item('articles/International',   'International',   '', 'primary-links', $politics_plid, 3);
  
  // Technology --  Internet, Gadgets, Biotech, Social Media, Gaming
  $tech_plid = install_menu_create_menu_item('articles/Technology', 'Technology', '', 'primary-links', 0, 5);
  install_menu_create_menu_item('articles/Internet',   'Internet',   '', 'primary-links', $tech_plid, 1);
  install_menu_create_menu_item('articles/Gadgets',   'Gadgets',   '', 'primary-links', $tech_plid, 2);
  install_menu_create_menu_item('articles/Biotech',   'Biotech',   '', 'primary-links', $tech_plid, 3);
  install_menu_create_menu_item('articles/Social Media',   'Social Media',   '', 'primary-links', $tech_plid, 4);
  install_menu_create_menu_item('articles/Gaming',   'Gaming',   '', 'primary-links', $tech_plid, 5);
  
  install_menu_create_menu_item('blogs',               'Blogs',      '', 'primary-links', 0, 6);
  install_menu_create_menu_item('resources',           'Resources',  '', 'primary-links', 0, 7);
  install_menu_create_menu_item('events',              'Events',     '', 'primary-links', 0, 8);
  install_menu_create_menu_item('topic-hubs',          'Topic Hubs', '', 'primary-links', 0, 9);

  install_menu_create_menu('Footer Primary', 'footer-primary');
  install_menu_create_menu_item('articles/all', 'Latest News', '', 'menu-footer-primary', 0, 1);
  // install_menu_create_menu_item('popular/all',  'Hot Topics',  '', 'menu-footer-primary', 0, 2);
  install_menu_create_menu_item('blogs',        'Blogs',       '', 'menu-footer-primary', 0, 3);
  install_menu_create_menu_item('resources',    'Resources',   '', 'menu-footer-primary', 0, 4);
  install_menu_create_menu_item('events',       'Events',      '', 'menu-footer-primary', 0, 5);

  install_menu_create_menu('Footer Secondary', 'footer-secondary');
  install_menu_create_menu_item('node/3', 'Subscribe',      '', 'menu-footer-secondary', 0, 1);
  install_menu_create_menu_item('node/2', 'Advertise',      '', 'menu-footer-secondary', 0, 2);
  install_menu_create_menu_item('node/5', 'Jobs',           '', 'menu-footer-secondary', 0, 3);
  install_menu_create_menu_item('node/6', 'Store',          '', 'menu-footer-secondary', 0, 4);
  install_menu_create_menu_item('node/1', 'About Us',       '', 'menu-footer-secondary', 0, 5);
  install_menu_create_menu_item('node/7', 'Site Map',       '', 'menu-footer-secondary', 0, 6);
  install_menu_create_menu_item('node/8', 'Terms of Use',   '', 'menu-footer-secondary', 0, 7);
  install_menu_create_menu_item('node/9', 'Privacy Policy', '', 'menu-footer-secondary', 0, 8);

  install_menu_create_menu('Top Menu', 'top-menu'); 
  install_menu_create_menu_item('node/1', 'About Us',  '', 'menu-top-menu', 0, 1);
  install_menu_create_menu_item('node/2', 'Advertise', '', 'menu-top-menu', 0, 2);
  install_menu_create_menu_item('node/3', 'Subscribe', '', 'menu-top-menu', 0, 3);
  install_menu_create_menu_item('node/4', 'RSS',       '', 'menu-top-menu', 0, 4);
  
  $msg = st('Installed Menus');
  _openpublish_log($msg);
  $context['message'] = $msg;
} 

/**
 * Create custom blocks and set region and pages.
 */
function _openpublish_setup_blocks(&$context) {  
  global $theme_key, $base_url; 
  cache_clear_all();

  // Ensures that $theme_key gets set for new block creation
  $theme_key = 'openpublish_theme';

  // install the demo ad blocks  
  $ad_base = $base_url . '/sites/all/themes/openpublish_theme/images/banner';
  $b1 = install_create_custom_block('<p id="credits"><a href="http://www.phase2technology.com/" target="_blank">Powered by Phase2 Technology</a></p>', 'Credits', FILTER_HTML_ESCAPE);
  $b2 = install_create_custom_block('<a href="http://openpublishapp.com"><img src="' . $ad_base . '/banner_openpublish.jpg"/></a><div class="clear"></div>', 'Top Banner Ad', 2);
  $b3 = install_create_custom_block('<p><a href="http://phase2technology.com"><img src="' . $ad_base . '/banner_phase2.jpg"/></a></p>', 'Right Block Square Ad', 2);
  $b4 = install_create_custom_block('<p><a href="http://tattlerapp.com"><img src="' . $ad_base . '/banner_tattler.jpg"/></a></p>', 'Homepage Ad Block 1', 2);
  $b5 = install_create_custom_block('<p><a href="http://phase2technology.com"><img src="' . $ad_base . '/banner_phase2.jpg"/></a></p>', 'Homepage Ad Block 2', FILTER_HTML_ESCAPE);

  // Get these new boxes in blocks table
  install_init_blocks();

  //-- Disable titles for all views-driven blocks, by default, to avoid double-titling:
  db_query("UPDATE {blocks} SET title = '%s' WHERE module = '%s' AND theme= '%s'", 
            '<none>', 'views', 'openpublish_theme');

  _openpublish_set_block_title('Google Videos Like This', 'morelikethis', 'googlevideo', 'openpublish_theme');
  _openpublish_set_block_title('Flickr Images Like This', 'morelikethis', 'flickr', 'openpublish_theme');
  _openpublish_set_block_title('Recommended Reading', 'morelikethis', 'taxonomy', 'openpublish_theme');

  install_disable_block('user', '0', 'openpublish_theme');
  install_disable_block('user', '1', 'openpublish_theme');
  install_disable_block('system', '0', 'openpublish_theme');
  
  $msg = st('Configured Blocks');
  _openpublish_log($msg);
  $context['message'] = $msg;
}

/**
 * Helper for setting a block's title only.
 */
function _openpublish_set_block_title($title, $module, $delta, $theme) {
  db_query("UPDATE {blocks} SET title = '%s' WHERE module = '%s' AND delta = '%s' AND theme= '%s'", $title, $module, $delta, $theme);
}

/**
 * Cleanup after the install
 */
function _openpublish_cleanup() {
  // DO NOT call drupal_flush_all_caches(), it disables all themes
  $functions = array(
    'drupal_rebuild_theme_registry',
    'menu_rebuild',
    'install_init_blocks',
    'views_invalidate_cache',    
    'node_types_rebuild',    
  );
  
  foreach ($functions as $func) {
    //$start = time();
    $func();
    //$elapsed = time() - $start;
    //error_log("####  $func took $elapsed seconds ###");
  }
  
  ctools_flush_caches(); 
  cache_clear_all('*', 'cache', TRUE);  
  cache_clear_all('*', 'cache_content', TRUE);
}

/**
 * Set OpenPublish as the default install profile
 */
function system_form_install_select_profile_form_alter(&$form, $form_state) {
  foreach($form['profile'] as $key => $element) {
    $form['profile'][$key]['#value'] = 'openpublish';
  }
}


/**
 * Consolidate logging.
 */
function _openpublish_log($msg) {
  error_log($msg);
  drupal_set_message($msg);
}

/**
 * Checks if installation is being done in a language other than English
 */
function _openpublish_language_selected() {
  global $install_locale;
  return !empty($install_locale) && ($install_locale != 'en');
}
