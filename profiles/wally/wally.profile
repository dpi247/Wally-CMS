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
    'update',  
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
    'admin', 'login_destination', 'install_profile_api',

    // CCK (content)
    'content_taxonomy', 'content_taxonomy_autocomplete', 
    'content_taxonomy_options', 'content_taxonomy_tree',
    'email', 'filefield', 'imagefield', 'link', 
    'pollfield', 
  
    // Chaos tool suite
    'ctools', 'page_manager', 'views_content',
  
    // Date/Time
    'date_api', 'date', 'date_timezone',
  
    // Features
    'features',
    
    // Image & ImageCache
    'imageapi', 'imageapi_gd', 
    'imagecache', 'imagecache_ui',  
    
    // Media
    'emfield', 'emimage', 'emwave', 'emaudio', 'emvideo',

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
    'wallyctools', 'wallytoolbox', 'wallymport',  
    
  ); 

  return array_merge($core_modules,$third,$mod,$custom);
} 

/**
 * Features module and Wally specific features
 */
function wally_feature_modules() {
  $features = array(
    'integrationstarter',
    'wallycontenttypes', 
    'wallyfundamentals', 
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
  $conf['site_footer'] = 'wally by <a href="http://www.audaxis.com">Audaxis</a> - Sponsorized by <a href="http://www.rossel.be">Rossel</a>';
  
  $tasks['wally-configure-batch'] = st('Configure Wally');
    
  return $tasks;
}

/**
 * Implementation of hook_profile_tasks().
 */
function wally_profile_tasks(&$task, $url) {

// @todo: Use "locale" for installation translation.
// global $install_locale;
  
  $output = "";
  // Install Modules.
  install_include(wally_profile_modules());

  if($task == 'profile') {
    // Starting process of Wally Installation Profile
    drupal_set_title(t('OpenPublish Installation'));
    _wally_log(t('Starting Wally Installation'));
    _wally_base_settings();
    $task = "wally-configure";
  }
    
  if($task == 'wally-configure') {

    $batch['title'] = st('Configuring @drupal .', array('@drupal' => drupal_install_profile_name()));

    $files = module_rebuild_cache();

    // Building "Batch" operations list.
    
      // We initialize each feature individually rather then all together
      // in the end, to avoid php execution timeout.
      foreach ( wally_feature_modules() as $feature ) {   
        $batch['operations'][] = array('_install_module_batch', array($feature, $files[$feature]->info['name']));      
        $batch['operations'][] = array('features_flush_caches', array()); 
      }    

      $batch['operations'][] = array('_wally_set_permissions', array());      
      $batch['operations'][] = array('_wally_initialize_settings', array());      
      $batch['operations'][] = array('_wally_placeholder_content', array());      
      $batch['operations'][] = array('_wally_set_views', array());      
      $batch['operations'][] = array('_wally_install_menus', array());      
      $batch['operations'][] = array('_wally_setup_blocks', array()); 
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
 * Translation import process is finished, move on to the next step
 */
function _wally_import_translations_finished($success, $results) {
  _openpublish_log(t('Translations have been imported.'));
  /**
   * Necessary as the wally_theme's status gets reset to 0
   * by a part of the automated batch translation in l10n_update
   */
  install_default_theme('wally_theme');
  variable_set('install_task', 'profile-finished');
}

/**
 * Import process is finished, move on to the next step
 */
function _wally_configure_finished($success, $results) {
  _wally_log(t('Yeah! Wally has been installed.'));
  if (_wally_language_selected()) {
    // Other language, different part of the process

    // @todo: 
    //variable_set('install_task', 'wally-translation-import');
    variable_set('install_task', 'profile-finished'); // Remove this if wally "translation" supported
  }
  else {
    // English (default) installation
    variable_set('install_task', 'profile-finished');
  }
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
  _openpublish_log($msg);
  $context['message'] = $msg;
}

/**
 * Set misc settings
 */
function _wally_initialize_settings(&$context){
 
  // Login Destination ( Administrator / Editor / Author )
  // @todo: move it to a module/feature 
  variable_set('ld_condition_type', 'pages');
  variable_set('ld_condition_snippet', 'user
user/login');
  variable_set('ld_url_type', 'snippet');
  variable_set('ld_destination', 0);
  // PHP code snipset for destination (yeah ... pretty nice, no?)  
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
      
  $msg = st('Setup general configuration');
  _wally_log($msg);
  $context['message'] = $msg;
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
 * Load & Updates views
 */
function _wally_set_views() {
  views_include_default_views();
  
  //most popular (statistics) view is disabled by default, enable it
  $view = views_get_view('popular');
  $view->disabled = FALSE;
  $view->save();

  $msg = st('Installed Views');
  _wally_log($msg);
  $context['message'] = $msg;
} 

/**
 * Setup custom menus and primary links.
 */
function _wally_install_menus(&$context) {

  cache_clear_all();
  menu_rebuild();

  // Primary Navigation
  install_menu_create_menu_item('<front>',             'Home',       '', 'primary-links', 0, 1);

  // Secondary Navigation
  install_menu_create_menu_item('node/1', 'About Us',  '', 'secondary-links', 0, 1);
  install_menu_create_menu_item('node/2', 'Terms of use', '', 'secondary-links', 0, 2);
  install_menu_create_menu_item('node/3', 'Privacy', '', 'secondary-links', 0, 3);

  // @todo: Settings about taxonomy menu
  
  $msg = st('Installed Menus');
  _openpublish_log($msg);
  $context['message'] = $msg;
} 

/**
 * Create custom blocks and set region and pages.
 */
function _wally_setup_blocks(&$context) {  

  cache_clear_all();
  // Ensures that $theme_key gets set for new block creation
  $theme_key = 'wallynews';

  install_disable_block('user', '0', 'wallynews');
  install_disable_block('user', '1', 'wallynews');
  install_disable_block('system', '0', 'wallynews');
  
  $msg = st('Configured Blocks');
  _wally_log($msg);
  $context['message'] = $msg;
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
}

/**
 * Set Wally as the default install profile
 */
function system_form_install_select_profile_form_alter(&$form, $form_state) {
  foreach($form['profile'] as $key => $element) {
    $form['profile'][$key]['#value'] = 'wally';
  }
}

/**
 * Consolidate logging.
 */
function _wally_log($msg) {
  error_log($msg);
  drupal_set_message($msg);
}

/**
 * Checks if installation is being done in a language other than English
 */
function _wally_language_selected() {
  global $install_locale;
  return !empty($install_locale) && ($install_locale != 'en');
}
