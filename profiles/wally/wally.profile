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
 * 
 * @TODO: doing something interesting with this
 */
function wally_devel_modules() {
  $dev = array(
    // Developpement & testing modules 
    'api', 'devel', 'grammar_parser', 'devel_themer', 
  ); 
  return $dev;
}

/**
 * Implementation of hook_profile_modules().
 */
function wally_profile_modules() {

  // Core Required & Optional modules.
  $core_modules = array(
    // Required core modules
    'block', 'filter', 'node', 'system', 'user', 'profile',

    // Optional core modules.
    'comment', 'help', 'locale', 'menu', 'statistics', 
    'search', 'taxonomy', 'translation', 'upload',
    'update', 'dblog',  
  );
  
  
  // Modified modules
  $mod = array(
    // CCK (content)
    'content', 'fieldgroup', 'nodereference',
    'text', 'optionwidgets',
 
    // Location 
    'location', 'location_cck',
  );

  // Third party requiered modules
  $third = array(
    // Administration & Installation module
    'devel', 'admin', 'install_profile_api',

    // CCK (content)
    'content_taxonomy', 'content_taxonomy_autocomplete',
    'content_taxonomy_options', 'content_taxonomy_tree',
    'email', 'filefield', 'imagefield', 'link', 
    'pollfield', 
  
    // Chaos tool suite
    'ctools', 'page_manager', 'views_content', 'ctools_custom_content',
  
    // Date/Time
    'date_api', 'date', 'date_timezone',
  
    // Features
    'features',
    
    // Image & ImageCache
    'imageapi', 'imageapi_gd', 
    'imagecache', 'imagecache_ui',  
    
    // Media
    'emfield', 'emimage', 'emwave', 'emaudio', 'emvideo', 'emother',
    'media_vimeo', 'media_youtube', 'media_kewego','media_embedly','media_flickr', 'media_coveritlive',
    // Others
    'job_queue', 
    
    // Panels
    'panels', 
    
    // Taxonomy related
    'taxonomy_manager', 'taxonomy_menu', 
    
    // User interface
    'jquery_ui',
    
    // Views
    'views', 'views_ui',
  ); 
  
  // Wally custom & specific modules
  $custom = array(

    'cckdestinations', 'wallyadmin', 'wally_content_taxonomy',
    'wallyctools', 'wallytoolbox', 'wallymport', 'wallyrsstonode',
    'wallyedit', 'wallyfeaturessynchronizer',
    
  );

  return array_merge($core_modules,$mod,$third,$custom);
} 

/**
 * Features module and Wally specific features
 * 
 * Creating content types, taxonomies, 
 * default views, default pages, etc.
 */
function wally_feature_modules() {
  $features = array(
    'integrationstarter',
    'wallycontenttypes',
    'wallyfundamentals',
    'wallyextra',
  );
  return $features;
}

/**
 * Features module and Wally specific features
 */
function wally_demo_feature_modules() {
  $features = array(
    'wallydefaultpages',
    'wallyfinfo',
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
function wally_profile_task_list() {
  global $conf;
  $conf['site_name'] = 'Wally';
  $conf['site_footer'] = 'Wally by <a href="http://www.audaxis.com">Audaxis</a> - Sponsorized by <a href="http://www.rossel.be">Rossel</a>';
  $tasks['wally-configure-batch'] = st('Configure Wally');
  return $tasks;
}

/**
 * Implementation of hook_profile_tasks().
 */
function wally_profile_tasks(&$task, $url) {
// @TODO: Use "locale" for installation translation.
// global $install_locale;

  $output = "";
  // Install Modules.
  install_include(wally_profile_modules());

  if($task == 'profile') {
    // Starting process of Wally Installation Profile
    drupal_set_title(t('Wally Installation'));
    _wally_log(t('Starting Wally Installation'));
    _wally_base_settings();
    $task = "wally-configure";
  }
    
  if($task == 'wally-configure') {
    $wally_install_config = variable_get('wally_install_config', array());

    $batch['title'] = st('Configuring @drupal .', array('@drupal' => drupal_install_profile_name()));

    $files = module_rebuild_cache();

    // Building "Batch" operations list.
  
    // We initialize each feature individually rather then all together
    // in the end, to avoid php execution timeout.
    foreach ( wally_feature_modules() as $feature ) {   
      $batch['operations'][] = array('_install_module_batch', array($feature, $files[$feature]->info['name']));      
      $batch['operations'][] = array('features_flush_caches', array()); 
    }
    
    if (isset($wally_install_config['demo_content']) && $wally_install_config['demo_content']) {
      foreach ( wally_demo_feature_modules() as $feature ) {   
        $batch['operations'][] = array('_install_module_batch', array($feature, $files[$feature]->info['name']));      
        $batch['operations'][] = array('features_flush_caches', array()); 
      }
      $batch['operations'][] = array('_wally_install_menus', array());
    }
    
    $batch['operations'][] = array('_wally_install_taxonomysettingsmenus', array());
    
    if (isset($wally_install_config['demo_content']) && $wally_install_config['demo_content']) {
      $batch['operations'][] = array('_wally_initialize_taxonomy_terms', array());
      $batch['operations'][] = array('_wally_placeholder_content', array());
    }
    
    $batch['operations'][] = array('_wally_set_permissions', array());
    $batch['operations'][] = array('_wally_initialize_settings', array());
    $batch['operations'][] = array('_wally_set_views', array());
    
    // Enable view popular after loading default views.
    if (isset($wally_install_config['demo_content']) && $wally_install_config['demo_content']) {
      $batch['operations'][] = array('_wally_enable_view_popular', array());
      $batch['operations'][] = array('_wally_enable_page_system_node_view', array());
      $batch['operations'][] = array('_wally_enable_page_system_term_view', array());
      $batch['operations'][] = array('_wally_build_wallyctools_views_layout_entry', array());
    }
    $batch['operations'][] = array('_wally_wallyctools_initial_setup', array());
    
    
    $batch['operations'][] = array('_wally_cleanup', array());

    $batch['error_message'] = st('There was an error configuring @drupal.', array('@drupal' => drupal_install_profile_name()));

    // Callback function when finished
    $batch['finished'] = '_wally_configure_finished';
    
    variable_set('install_task', 'wally-configure-batch');
    batch_set($batch);
    batch_process($url, $url);
  }
  
  // Land here until the batches are done
  if (in_array($task, array('wally-configure-batch'))) {
    include_once 'includes/batch.inc';
    $output = _batch_page();
  }
    
  return $output;
} 

/**
 * Import process is finished, move on to the next step
 */
function _wally_configure_finished($success, $results) {
  _wally_log(t('Yeah! Wally has been installed.'));
  variable_set('install_task', 'profile-finished');
}

/**
 * Do some basic setup
 */
function _wally_base_settings() {  
  global $base_url;  

  // create pictures dir
  $pictures_path = file_create_path('pictures');
  file_check_directory($pictures_path,TRUE);

  variable_set('wally_version', '1.0'); 

  // Creating core content types.
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

  // Theme installation & settings.  
  //_wally_system_theme_data(); // @TODO: move wallynews theme to profile folder
  install_default_theme('wallynews');
  install_admin_theme('rubik');	
  variable_set('node_admin_theme', TRUE);    
  $theme_settings = variable_get('theme_settings', array());
  $theme_settings['toggle_node_info_page'] = FALSE;
  $theme_settings['default_logo'] = FALSE;
  variable_set('theme_settings', $theme_settings);    
  
  // Basic Drupal settings.
  variable_set('site_frontpage', 'node');
  variable_set('user_register', FALSE); 
  variable_set('user_pictures', '1');
  variable_set('filter_default_format', '1');
 
  // Statistics settings
  variable_set('statistics_count_content_views', 1);
  
  // Set the default timezone name from the offset
  $offset = variable_get('date_default_timezone', '');
  $tzname = timezone_name_from_abbr("", $offset, 0);
  variable_set('date_default_timezone_name', $tzname);
 
  _wally_log(st('Configured basic settings'));
}

/**
 * Configure user/role/permission data
 */
function _wally_set_permissions(&$context){
  
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
  install_profile_field_add($profile_full_name);
  install_profile_field_add($profile_job_title);

  $msg = st('Configured Users & Permissions');
  _wally_log($msg);
  $context['message'] = $msg;
}

/**
 * Set default taxonomy terms
 */
function _wally_initialize_taxonomy_terms(&$context){
       
  // Destination taxonomy (vocabulary created by wallycontenttype feature).
  $vid = install_taxonomy_get_vid("Destination Path");
  if ($vid) {
    foreach (_wally_destinationtaxonomy_terms($vid) as $term) {
      install_taxonomy_add_term($vid, $term['name'], $term['description'], $term);
    }
  }  
  
  // Document Type taxonomy (vocabulary created by wallycontenttype feature).
  $vid = install_taxonomy_get_vid("Document Type");     
  if ($vid) {
    foreach (_wally_documenttypetaxonomy_terms($vid) as $term) {
      install_taxonomy_add_term($vid, $term['name'], $term['description'], $term);
    }
  }

  // Rating taxonomy (vocabulary created by wallycontenttype feature).
  $vid = install_taxonomy_get_vid("Rating");     
  if ($vid) {
    foreach (_wally_ratingtaxonomy_terms($vid) as $term) {
      install_taxonomy_add_term($vid, $term['name'], $term['description'], $term);
    }
  }
  
  menu_rebuild();
  
  $msg = st('Setup default taxonomy terms');
  _wally_log($msg);
  $context['message'] = $msg;
}

/**
 * Set misc settings
 */
function _wally_initialize_settings(&$context){
  
   // Wally Import Settings
   // Doing here because we need features to be
   // activated before
  $vid = install_taxonomy_get_vid("Destination Path");
  variable_set('wallymport_destinationpath',strval($vid));
  $vid = install_taxonomy_get_vid("Editions");
  variable_set('wallymport_edition',strval($vid));
  $vid = install_taxonomy_get_vid("Locations");
  variable_set('wallymport_location',strval($vid));
  $vid = install_taxonomy_get_vid("Persons");
  variable_set('wallymport_person',strval($vid));
  $vid = install_taxonomy_get_vid("Entities");
  variable_set('wallymport_entity',strval($vid));
  $vid = install_taxonomy_get_vid("Free tags");
  variable_set('wallymport_freetagtaxonomy',strval($vid));
  $vid = install_taxonomy_get_vid("Keywords categorized");
  variable_set('wallymport_classifiedtagtaxonomy',strval($vid));

  variable_set('wallymport_taxonomy_recusive',"true");
  variable_set('wallymport_defaultuser',"1");
  variable_set('wallymport_source',"sites/default/files/import");
  variable_set('wallymport_import_done',"sites/default/files/import/done");
  variable_set('wallymport_import_error',"sites/default/files/import/error");
  variable_set('wallymport_definition',"profiles/wally/modules/custom/wally/wallymport/definitions/packages.xsd");
  variable_set('wallymport_temp',"/tmp");
  variable_set('wallymport_debug',"0");
  
  // Embedly Module config settings
  variable_set('emfield_emvideo_allow_embedly',"i:1;");
  variable_set('media_embedly__video_width','s:0:"";');
  variable_set('emfield_emaudio_allow_embedly',"i:1;");
  variable_set('media_embedly__audio_width','s:0:"";');  
  
  // Flickr Module config settings
  variable_set('emfield_emimage_allow_flickr',"i:1;");
  
  $msg = st('Setup general configuration');
  _wally_log($msg);
  $context['message'] = $msg;
}

/**
 * Return an array of terms for default destinations taxonomy.
 */
function _wally_destinationtaxonomy_terms($vid) {

  $terms = array();

  //tid-1
  $terms[] = array(
    'name' => 'News',
    'description' => 'All about news',
    'parent' => array(),
    'relations' => array(),
    'weight' => 1,
    'vid' => $vid,
  );

  //tid-2
  $terms[] = array(
    'name' => 'Sports',
    'description' => 'All about sports',
    'parent' => array(),
    'relations' => array(),
    'weight' => 2,
    'vid' => $vid,
  );

  //tid-3
  $terms[] = array(
    'name' => 'Economy',
    'description' => 'All about economy',
    'parent' => array(),
    'relations' => array(),
    'weight' => 3,
    'vid' => $vid,
  );

  //tid-4
  $terms[] = array(
    'name' => 'Politics',
    'description' => 'All about politic',
    'parent' => array(),
    'relations' => array(),
    'weight' => 4,
    'vid' => $vid,
  );

  return $terms; 
}


function _wally_documenttypetaxonomy_terms($vid) {

  $terms = array();

  //tid-5
  $terms[] = array(
    'name' => 'Article',
    'description' => 'Article',
    'parent' => array(),
    'relations' => array(),
    'weight' => 1,
    'vid' => $vid,
  );

  //tid-6
  $terms[] = array(
    'name' => 'Blog Post',
    'description' => 'Blog Post',
    'parent' => array(),
    'relations' => array(),
    'weight' => 2,
    'vid' => $vid,
  );

  return $terms; 
}

function _wally_ratingtaxonomy_terms($vid) {

  $terms = array();

  //tid-7
  $terms[] = array(
    'name' => 'G',
    'description' => 'General Audiences - All ages admitted',
    'parent' => array(),
    'relations' => array(),
    'weight' => 1,
    'vid' => $vid,
  );
   
  //tid-8
  $terms[] = array(
    'name' => 'PG',
    'description' => 'Parental Guidance Suggested - Some material may not be suitable for children',
    'parent' => array(),
    'relations' => array(),
    'weight' => 1,
    'vid' => $vid,
  );

  //tid-9
  $terms[] = array(
    'name' => 'PG-13',
    'description' => 'Parents Strongly Cautioned - Some material may be not be appropriate for children under 13',
    'parent' => array(),
    'relations' => array(),
    'weight' => 1,
    'vid' => $vid,
  );
  
  //tid-10
  $terms[] = array(
    'name' => 'R',
    'description' => 'Restricted - Under 17 requires accompanying parent or adult guardian',
    'parent' => array(),
    'relations' => array(),
    'weight' => 1,
    'vid' => $vid,
  );

  //tid-11
  $terms[] = array(
    'name' => 'NC-17',
    'description' => 'No One 17 and under admitted',
    'parent' => array(),
    'relations' => array(),
    'weight' => 1,
    'vid' => $vid,
  );
  
  return $terms; 
}


/**
 * Create some content of type "page" as placeholders for content
 * and so menu items can be created
 */
function _wally_placeholder_content(&$context) {
  global $base_url;

  $user = user_load(array('uid' => 1));

  // Generic Array for a page.
  $page = array (
    'type' => 'page',
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
    'body' => 'place some text here....',
    'format' => 2,
    'name' => $user->name,
  );

  // /node/1
  $node = (object) $page;
  $node->title = st('About Us');
  $node->body = 'Tell something about you and this site ...';
  node_save($node);	

  // /node/2
  $node = (object) $page;
  $node->title = st('Terms of Use');
  $node->body = 'Write here all your terms of use.';
  node_save($node);	

  // /node/3
  $node = (object) $page;
  $node->title = st('Privacy Policy');
  $node->body = 'Tell herre everything about your privacy policies ...';
  node_save($node);	

  menu_rebuild();
  
  $msg = st('Installed Content');
  _wally_log($msg);
  $context['message'] = $msg;
}

/**
 * Enable view popular
 */
function _wally_enable_view_popular(&$context) {
  //most popular (statistics) view is disabled by default, enable it
  $view = views_get_view('popular');
  $view->disabled = FALSE;
  $view->save();

  $msg = st('Enabled view popular');
  _wally_log($msg);
  $context['message'] = $msg;
} 


/**
 * Enable page_system_node_view
 */
function _wally_enable_page_system_node_view(&$context) {
 variable_set('page_manager_node_view_disabled', FALSE);
  
} 

/**
 * Enable page_system_term_view
 */
function _wally_enable_page_system_term_view(&$context) {
 variable_set('page_manager_term_view_disabled', FALSE);
}

/**
 * Build mapping table between views and pages for redacblock. in wallyctools_views_layout table
 */
function _wally_build_wallyctools_views_layout_entry(&$context) {
  //wallyctools_build_all_wallyctools_views_layout_entry();
} 

function _wally_wallyctools_initial_setup(&$context) {
  wallyctools_initial_setup();
}

/**
 * Load & Updates views
 */
function _wally_set_views(&$context) {
  views_include_default_views();

  $msg = st('Installed Views');
  _wally_log($msg);
  $context['message'] = $msg;
} 

/**
 * Setup custom menus and primary links.
 */
function _wally_install_menus(&$context) {

  $menu_name = "primary-links";

  cache_clear_all();
  menu_rebuild();

  // Primary Navigation
  install_menu_create_menu_item('<front>',             'Home',       '', 'primary-links', 0, 1);

  // Secondary Navigation
  install_menu_create_menu_item('node/1', 'About Us',  '', 'secondary-links', 0, 1);
  install_menu_create_menu_item('node/2', 'Terms of use', '', 'secondary-links', 0, 2);
  install_menu_create_menu_item('node/3', 'Privacy', '', 'secondary-links', 0, 3);
  
  $msg = st('Installed Menus');
  _wally_log($msg);
  $context['message'] = $msg;
} 

/**
 * Setup taxonomy settings for the menus and primary links.
 */
function _wally_install_taxonomysettingsmenus(&$context) {

  $menu_name = "primary-links";
  
  // Settings about taxonomy menu
  // Be aware that "Menu Setup" need called after 
  // populate taxonomies ( @see: _wally_initialize_settings ); 
  $vocabularies = taxonomy_get_vocabularies();
  foreach ($vocabularies as $vocabulary) {
    if ($vocabulary->name == "Destination Path") {
      $vid = $vocabulary->vid;
      break; 
    }
  }
  variable_set('taxonomy_menu_expanded_'.$vid, 0);
  variable_set('taxonomy_menu_voc_name_'.$vid, "");
  variable_set('taxonomy_menu_display_descendants_'.$vid, O);
  variable_set('taxonomy_menu_blank_title_'.$vid, 0);
  variable_set('taxonomy_menu_end_all_'.$vid, 0);
  variable_set('taxonomy_menu_rebuild_'.$vid, 0);
  variable_set('taxonomy_menu_vocab_menu_'.$vid, $menu_name);
  variable_set('taxonomy_menu_vocab_parent_'.$vid, "0");
  variable_set('taxonomy_menu_path_'.$vid, "taxonomy_menu_path_default");
  variable_set('taxonomy_menu_sync_'.$vid, 1);
  variable_set('taxonomy_menu_display_num_'.$vid, 0);
  variable_set('taxonomy_menu_hide_empty_terms_'.$vid, 0);
  variable_set('taxonomy_menu_voc_item_'.$vid, 0);
  _wally_setup_taxonomymenu($vid, $menu_name);
  
  $msg = st('Installed Taxonomy Settings for the Menus');
  _wally_log($msg);
  $context['message'] = $msg;
} 

/*
 * Reset "Taxonomy Menu" items (normaly empty)
 * and ADD terms from "Destination Path" (Voc 2) to it.
 */ 
function _wally_setup_taxonomymenu($vid, $menu_name) {

  _taxonomy_menu_delete_all($vid);

/*
  $args = array(
    'vid' => $vid,
    'menu_name' => $menu_name,
  );
  $mlid = taxonomy_menu_handler('insert', $args);
  */
  
  $terms = taxonomy_get_tree($vid);
  foreach ($terms as $term) {
    $args = array(
      'term' => $term,
      'menu_name' => $menu_name,
    );
    $mlid = taxonomy_menu_handler('insert', $args);
  }
}

/**
 * Cleanup after the install
 */
function _wally_cleanup() {

  // DO NOT call drupal_flush_all_caches(), it disables all themes
  $functions = array(
    'drupal_rebuild_theme_registry',
    'menu_rebuild',
    'install_init_blocks',
    'views_invalidate_cache',    
    'node_types_rebuild',    
  );

  // Trash & Durty coding :-) 
  foreach ($functions as $func) {
    $func();
  }
  
  ctools_flush_caches(); 
  cache_clear_all('*', 'cache', TRUE);  
  cache_clear_all('*', 'cache_content', TRUE);

  $msg = st('Cleanup');
  _wally_log($msg);
  $context['message'] = $msg;

}

/**
 * Set Wally as the default install profile
 * 
 * @TODO: This might be impolite/too aggressive. We should at least check that
 * other install profiles are not present to ensure we don't collide with a
 * similar form alter in their profile.
 */
function system_form_install_select_profile_form_alter(&$form, $form_state) {
  foreach($form['profile'] as $key => $element) {
    $form['profile'][$key]['#value'] = 'wally';
  }
  // Make wally appear as the first choice.
  $form['profile']['wally']['#weight'] = -40;
}

/**
 * Consolidate logging.
 */
function _wally_log($msg) {
  error_log('Wally install : '.$msg);
  drupal_set_message($msg,"wally profile");
}

/**
 * Checks if installation is being done in a language other than English
 */
function _wally_language_selected() {
  global $install_locale;
  return !empty($install_locale) && ($install_locale != 'en');
}


/**
 * Reimplementation of system_theme_data(). The core function's static cache
 * is populated during install prior to active install profile awareness.
 * This workaround makes enabling themes in profiles/[profile]/themes possible.
 * 
 * Gently copy/pasted from OpenAtrium installation profile.
 * @see: http://openatrium.com/ 
 */
function _wally_system_theme_data() {
  global $profile;
  $profile = 'wally';

  $themes = drupal_system_listing('\.info$', 'themes');
  $engines = drupal_system_listing('\.engine$', 'themes/engines');

  $defaults = system_theme_default();

  $sub_themes = array();
  foreach ($themes as $key => $theme) {
    $themes[$key]->info = drupal_parse_info_file($theme->filename) + $defaults;

    if (!empty($themes[$key]->info['base theme'])) {
      $sub_themes[] = $key;
    }

    $engine = $themes[$key]->info['engine'];
    if (isset($engines[$engine])) {
      $themes[$key]->owner = $engines[$engine]->filename;
      $themes[$key]->prefix = $engines[$engine]->name;
      $themes[$key]->template = TRUE;
    }

    // Give the stylesheets proper path information.
    $pathed_stylesheets = array();
    foreach ($themes[$key]->info['stylesheets'] as $media => $stylesheets) {
      foreach ($stylesheets as $stylesheet) {
        $pathed_stylesheets[$media][$stylesheet] = dirname($themes[$key]->filename) .'/'. $stylesheet;
      }
    }
    $themes[$key]->info['stylesheets'] = $pathed_stylesheets;

    // Give the scripts proper path information.
    $scripts = array();
    foreach ($themes[$key]->info['scripts'] as $script) {
      $scripts[$script] = dirname($themes[$key]->filename) .'/'. $script;
    }
    $themes[$key]->info['scripts'] = $scripts;

    // Give the screenshot proper path information.
    if (!empty($themes[$key]->info['screenshot'])) {
      $themes[$key]->info['screenshot'] = dirname($themes[$key]->filename) .'/'. $themes[$key]->info['screenshot'];
    }
  }

  foreach ($sub_themes as $key) {
    $themes[$key]->base_themes = system_find_base_themes($themes, $key);
    // Don't proceed if there was a problem with the root base theme.
    if (!current($themes[$key]->base_themes)) {
      continue;
    }
    $base_key = key($themes[$key]->base_themes);
    foreach (array_keys($themes[$key]->base_themes) as $base_theme) {
      $themes[$base_theme]->sub_themes[$key] = $themes[$key]->info['name'];
    }
    // Copy the 'owner' and 'engine' over if the top level theme uses a
    // theme engine.
    if (isset($themes[$base_key]->owner)) {
      if (isset($themes[$base_key]->info['engine'])) {
        $themes[$key]->info['engine'] = $themes[$base_key]->info['engine'];
        $themes[$key]->owner = $themes[$base_key]->owner;
        $themes[$key]->prefix = $themes[$base_key]->prefix;
      }
      else {
        $themes[$key]->prefix = $key;
      }
    }
  }

  // Extract current files from database.
  system_get_files_database($themes, 'theme');
  db_query("DELETE FROM {system} WHERE type = 'theme'");
  foreach ($themes as $theme) {
    $theme->owner = !isset($theme->owner) ? '' : $theme->owner;
    db_query("INSERT INTO {system} (name, owner, info, type, filename, status, throttle, bootstrap) VALUES ('%s', '%s', '%s', '%s', '%s', %d, %d, %d)", $theme->name, $theme->owner, serialize($theme->info), 'theme', $theme->filename, isset($theme->status) ? $theme->status : 0, 0, 0);
  }
}

/**
 * Alter the install profile configuration form and provide option to install demo content
 */
function system_form_install_configure_form_alter(&$form, $form_state) {

  // Populate some fields.
  $form['site_information']['site_name']['#default_value'] = 'Wally News';
  $form['site_information']['site_mail']['#default_value'] = 'admin@'. $_SERVER['HTTP_HOST'];
  $form['admin_account']['account']['name']['#default_value'] = 'admin';
  $form['admin_account']['account']['mail']['#default_value'] = 'admin@'. $_SERVER['HTTP_HOST'];
  
  $form['wally'] = array(
    '#type' => 'fieldset',
    '#title' => t('Wally settings'),
    '#weight' => 10, // be sure we're at the end
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  
  $form['wally']['demo_content'] = array(
    '#type' => 'checkbox',
    '#title' => t('Install Demo content?'),
    '#description' => t('Do you want some demo content items?'),
  );
  
  $form['#submit'][] = '_wally_install_form_submit';
}

function _wally_install_form_submit(&$form, $form_state) {
  $wally_install_config = array();
  foreach ($form['wally'] as $name => $value) {
    $wally_install_config[$name] = $value['#value'];
  }
  
  variable_set('wally_install_config', $wally_install_config);
}
