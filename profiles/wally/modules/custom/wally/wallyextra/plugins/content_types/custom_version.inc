<?php
// $Id: custom.inc,v 1.9.2.15 2010/10/19 21:27:41 merlinofchaos Exp $

/**
 * @file
 * Custom content type.
 *
 * This content type is nothing more than a title and a body that is entered
 * by the user and run through standard filters. The information is stored
 * right in the config, so each custom content is unique.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Custom content Versionable'),
  'no title override' => TRUE,
  'defaults' => array('admin_title' => '', 'title' => '', 'body' => '', 'format' => variable_get('filter_default_format', 1), 'substitute' => TRUE),
  'js' => array('misc/autocomplete.js', 'misc/textarea.js', 'misc/collapse.js'),
  // Make sure the edit form is only used for some subtypes.
  'edit form' => '',
  'add form' => '',
  'edit text' => t('Edit'),
  'all contexts' => TRUE,
);

/**
 * Return the custom content types with the specified $subtype_id.
 */
function wallyextra_custom_version_content_type_content_type($subtype_id) {
  if ($subtype_id == 'custom_version') {
    return _wallyextra_default_custom_version_content_type_content_type();
  }
  else {
    ctools_include('export');
    $content = ctools_export_crud_load('wallyextra_custom_content', $subtype_id);
    if ($content) {
      return _wallyextra_custom_version_content_type_content_type($content);
    }
  }
}

/**
 * Return all custom content types available.
 */
function wallyextra_custom_version_content_type_content_types() {
  ctools_include('export');
  $types = array();
  $types['custom_version'] = _wallyextra_default_custom_version_content_type_content_type();

  foreach (ctools_export_crud_load_all('wallyextra_custom_content') as $name => $content) {
    $types[$name] = _wallyextra_custom_version_content_type_content_type($content);
  }
  return $types;
}

/**
 * Settings for the default custom content type.
 *
 * The default is the one that allows the user to actually create a type.
 */
function _wallyextra_default_custom_version_content_type_content_type() {
  $info = array(
    'name'           => 'custom_version',
    'title'          => t('New custom content Versionable'),
    'top level'      => TRUE,
    'category'       => t('Custom'),
    'description'    => t('Create a completely custom piece of HTML content.'),
    'edit form'      => 'wallyextra_custom_version_content_type_edit_form',
    'all contexts'   => TRUE,
    'check editable' => 'wallyextra_custom_version_content_type_editable',
  );

  return $info;
}


/**
 * Return an info array for a specific custom content type.
 */
function _wallyextra_custom_version_content_type_content_type($content) { 
  
  $info = array(
    'name' => $content->name,
    'title' => check_plain($content->admin_title),
    'description' => check_plain($content->admin_description),
    'category' => $content->category ? check_plain($content->category) : t('Miscellaneous'),
    'all contexts' => TRUE,
    'icon' => 'icon_block_custom.png',
    'edit form' => 'wallyextra_custom_version_content_type_edit_form',
    // Store this here to make it easy to access.
    'content' => $content,
  );

  return $info;
}

/**
 * Given a subtype and a $conf, return the actual settings to use.
 *
 * The actual settings may be stored directly in the pane or this may
 * be a pointer to re-usable content that may be in the database or in
 * an export. We have to determine from the subtype whether or not it
 * is local or shared custom content.
 */
function wallyextra_custom_version_content_type_get_conf($subtype, $conf) { 
  if ($subtype['name'] != 'custom_version') {
    $settings = $subtype['content']->settings;
    $settings['custom_type'] = 'fixed';
    $settings['content'] = $subtype['content'];
    $settings['id'] = $subtype['content']->settings['id'];
  }
  else {
    // This means they created it as custom content and then set it as
    // reusable. Since we're not allowed to change the subtype, we're
    // still stored as though we are local, but are pointing off to
    // non-local.
    if (!empty($conf['name'])) {
      ctools_include('export');
      $content = ctools_export_crud_load('wallyextra_custom_content', $conf['name']);
      if ($content) {
        $settings = $content->settings;
        $settings['custom_type'] = 'fixed';
        $settings['content'] = $content;
        $settings['admin_title'] = $content->admin_title;
      }
      else {
        $content = ctools_export_crud_new('wallyextra_custom_content');
        $content->name = $conf['name'];
        $settings = array(
          'admin_title' => t('Missing/deleted content'),
          'title'       => '',
          'body'        => '',
          'format'      => FILTER_FORMAT_DEFAULT,
          'substitute'  => TRUE,
          'custom_type' => 'fixed',
          'content'     => $content,
        );
      }
    }
    // This means that it is created as custom and has not been set to
    // reusable.
    else {
      $settings = $conf;
      $settings['custom_type'] = 'local';
    }
  }

  return $settings;
}

function wallyextra_custom_version_content_type_editable($content_type, $subtype, $conf) {
  return TRUE;
}

/**
 * Output function for the 'custom' content type. Outputs a custom
 * based on the module and delta supplied in the configuration.
 */
function wallyextra_custom_version_content_type_render($subtype, $conf, $args, $contexts) { 
  $settings = wallyextra_custom_version_content_type_get_conf(wallyextra_custom_version_content_type_content_type($subtype), $conf);

  static $delta = 0;

  $block          = new stdClass();
  $block->subtype = ++$delta;
  $block->title   = filter_xss_admin($settings['title']);

  // Add keyword substitutions if we were configured to do so.
  $content = $settings['body'];
  if (!empty($contexts) && !empty($settings['substitute'])) {
    $content = ctools_context_keyword_substitute($content, array(), $contexts);
  }

  $block->content = check_markup($content, $settings['format'], FALSE);
  return $block;
}

/**
 * Callback to provide the administrative title of the custom content.
 */
function wallyextra_custom_version_content_type_admin_title($subtype, $conf) { 
  $settings = wallyextra_custom_version_content_type_get_conf(wallyextra_custom_version_content_type_content_type($subtype), $conf);

  $output = t('Custom');
  $title = !empty($settings['admin_title']) ? $settings['admin_title'] : $settings['title'];
  if ($title) {
    if ($settings['custom_type'] != 'fixed') {
      $output = t('Custom: @title', array('@title' => $title));
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
function wallyextra_custom_version_content_type_admin_info($subtype, $conf) { 
  $settings = wallyextra_custom_version_content_type_get_conf(wallyextra_custom_version_content_type_content_type($subtype), $conf);

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
    $block->content = filter_xss_admin(check_markup($settings['body'], $settings['format'], FALSE));
  }
  return $block;
}

/**
 * Returns an edit form for the custom type.
 */
function wallyextra_custom_version_content_type_edit_form(&$form, &$form_state) {

  if (!db_table_exists('wallyextra_custom_content_version')){
    $form['warning'] = array(
      '#type'  => 'markup',
      '#value' => t('You must update the module "WallyExtra" to enable this functionality'),
    );
  } else {
    $settings = wallyextra_custom_version_content_type_get_conf($form_state['subtype'], $form_state['conf']);
    $form_state['settings'] = $settings;

    foreach ($form_state['conf'] as $key => $value){
      $form['conf']['#'.$key] = $value; 
    }
    if ($form_state['subtype_name'] != 'custom_version' && !isset($form_state['conf']['id'])){
      $form['reusable'] = array(
        '#type' => 'value',
        '#value' => 'reusable',
      ); 
      return;
    }
  
    $form['admin_title'] = array(
      '#type'          => 'textfield',
      '#default_value' => isset($settings['admin_title']) ? $settings['admin_title'] : '',
      '#title'         => t('Administrative title'),
      '#description'   => t('This title will be used administratively to identify this pane. If blank, the regular title will be used.'),
    );
  
    $form['title'] = array(
      '#type'          => 'textfield',
      '#default_value' => $settings['title'],
      '#title'         => t('Title'),
    );
  
    $form['body_field']['body'] = array(
      '#title'         => t('Body'),
      '#type'          => 'textarea',
      '#default_value' => $settings['body'],
    );
    $parents[] = 'format';
    $form['body_field']['format'] = filter_form($settings['format'], 1, $parents);
  
    if (!empty($form_state['contexts'])) {
      // Set extended description if both CCK and Token modules are enabled, notifying of unlisted keywords
      if (module_exists('content') && module_exists('token')) {
        $description = t('If checked, context keywords will be substituted in this content. Note that CCK fields may be used as keywords using patterns like <em>%node:field_name-formatted</em>.');
      } elseif (!module_exists('token')) {
        $description = t('If checked, context keywords will be substituted in this content. More keywords will be available if you install the Token module, see http://drupal.org/project/token.');
      } else {
        $description = t('If checked, context keywords will be substituted in this content.');
      }
  
      $form['substitute'] = array(
        '#type'          => 'checkbox',
        '#title'         => t('Use context keywords'),
        '#description'   => $description,
        '#default_value' => !empty($settings['substitute']),
      );
      $form['contexts'] = array(
        '#title'       => t('Substitutions'),
        '#type'        => 'fieldset',
        '#collapsible' => TRUE,
        '#collapsed'   => TRUE,
      );
  
      $rows = array();
      foreach ($form_state['contexts'] as $context) {
        foreach (ctools_context_get_converters('%' . check_plain($context->keyword) . ':', $context) as $keyword => $title) {
          $rows[] = array(
            check_plain($keyword),
            t('@identifier: @title', array('@title' => $title, '@identifier' => $context->identifier)),
          );
        }
      }
      $header = array(t('Keyword'), t('Value'));
      $form['contexts']['context'] = array('#value' => theme('table', $header, $rows));
    }
  
    if (!user_access('make a widget reusable') || !module_exists('wallyextra')) {
      return;
    }
  
    // Make the other form items dependent upon it.
    ctools_include('dependent');
    ctools_add_js('dependent');
  
    if ($form_state['subtype_name'] == 'custom_version' && $form_state['settings']['custom_type'] == 'local'){
      $form['reusable'] = array(
        '#type'          => 'checkbox',
        '#title'         => t('Make this content reusable'),
        '#default_value' => FALSE,
      );
      $form['name'] = array(
        '#type'        => 'textfield',
        '#title'       => t('Machine name'),
        '#description' => t('The machine readable name of this content. It must be unique, and it must contain only alphanumeric characters and underscores. Once created, you will not be able to change this value!'),
        '#process'     => array('ctools_dependent_process'),
        '#dependency'  => array('edit-reusable' => array(1)),
      );
    
      $form['category'] = array(
        '#type'        => 'textfield',
        '#title'       => t('Category'),
        '#description' => t('What category this content should appear in. If left blank the category will be "Miscellaneous".'),
        '#process'     => array('ctools_dependent_process'),
        '#dependency'  => array('edit-reusable' => array(1)),
      );
    
      $form['admin_description'] = array(
        '#type'        => 'textarea',
        '#title'       => t('Administrative description'),
        '#description' => t('A description of what this content is, does or is for, for administrative use.'),
        '#process'     => array('ctools_dependent_process'),
        '#dependency'  => array('edit-reusable' => array(1)),
      );
    }
  }
}

function _wallyextra_custom_version_content_type_edit_save(&$content, $form_state) {
  $content->category = $form_state['values']['category'];
  $content->admin_title = $form_state['values']['admin_title'];
  $content->admin_description = $form_state['values']['admin_description'];
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    if (isset($form_state['values'][$key])) {
      $content->settings[$key] = $form_state['values'][$key];
    }
  }
  $content->settings['id'] = $form_state['conf']['id'];
  ctools_export_crud_save('wallyextra_custom_content', $content);
}

/**
 * The validate form to ensure the custom content data is okay.
 */
function wallyextra_custom_version_content_type_edit_form_validate(&$form, &$form_state) {
  if ($form_state['settings']['custom_type'] != 'fixed' && !empty($form_state['values']['reusable'])) {
    if (empty($form_state['values']['name'])) {
      form_error($form['name'], t('Name is required.'));
    }

    // Check for string identifier sanity
    if (!preg_match('!^[a-z0-9_]+$!', $form_state['values']['name'])) {
      form_error($form['name'], t('The name can only consist of lowercase letters, underscores, and numbers.'));
      return;
    }

    // Check for name collision
    if ($form_state['values']['name'] == 'custom_version' || (ctools_export_crud_load('wallyextra_custom_content', $form_state['values']['name']))) {
      form_error($form['name'], t('Content with this name already exists. Please choose another name or delete the existing item before creating a new one.'));
    }
  }
}

/**
 * The submit form stores the data in $conf.
 */
function wallyextra_custom_version_content_type_edit_form_submit(&$form, &$form_state) {

  //If it's not a reused content
  if ($form_state['subtype_name'] == 'custom_version'){
    //if it's a new content, a cid is assign and the version is set to one
    if ($form_state['conf']['id'] === NULL){
      $result = db_query("SELECT MAX(cid) as max FROM {wallyextra_custom_content_version}");
      $obj = db_fetch_object($result);
      $form_state['conf']['id'] = ($obj->max + 1);
      $version = 1;
    } else {
      //if it's in edition it looks for the last version number
      $result = db_query("SELECT MAX(version) as version
          					FROM {wallyextra_custom_content_version}
          					WHERE cid = %d", $form_state['conf']['id']);
      $obj = db_fetch_object($result);
      $version = ($obj->version + 1);
    }
    $cid = $form_state['conf']['id'];
    $form_state['conf']['body'] = $form_state['values']['body'];
  } else {
    //If its a reused content the last version is found
    $result = db_query("SELECT MAX(version) as version
              			FROM {wallyextra_custom_content_version}
              			WHERE cid = %d", $form_state['settings']['id']);
    $obj = db_fetch_object($result);
    $version = ($obj->version + 1);
    $cid = $form_state['settings']['id'];
    $form_state['conf']['id'] = $cid;
    $form_state['conf']['body'] = $form_state['values']['body'];
  }
  $form_state['conf']['version'] = $version;
  
  
  if ($form_state['settings']['custom_type'] == 'fixed') {
    _wallyextra_custom_version_content_type_edit_save($form_state['settings']['content'], $form_state);
  }
  // If the 'reusable' checkbox was checked, we will create a new
  // custom content and give it the proper values.
  else if (!empty($form_state['values']['reusable'])) {
    $content = ctools_export_crud_new('wallyextra_custom_content');
    $content->name = $form_state['values']['name'];
    _wallyextra_custom_version_content_type_edit_save($content, $form_state);
    $form_state['conf']['name'] = $content->name;
  }
  else {
    // Otherwise, just save values into $conf normally.
    foreach (array_keys($form_state['plugin']['defaults']) as $key) {
      if (isset($form_state['values'][$key])) {
        $form_state['conf'][$key] = $form_state['values'][$key];
      }
    }
  }
  //Don't save version if it's a new reused content
  if (isset($form_state['values']['body'])){ 
    $row = new stdClass();
    $row->cid = $cid;
    $row->version = $version;
    $row->date = time();
    $row->settings = $form_state['conf'];

    drupal_write_record('wallyextra_custom_content_version', $row);
  }
}
