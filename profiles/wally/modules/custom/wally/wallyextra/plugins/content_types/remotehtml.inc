<?php

$plugin = array(
	'no_title_override' => FALSE,
	'defaults' => _wallyextra_remotehtml_defaultformvalues(),
	'js' => array('misc/autocomplete.js', 'misc/textarea.js', 'misc/collapse.js'),
		// Make sure the edit form is only used for some subtypes.
	'edit form' => '',
	'add form' => '',
	'edit text' => t('Edit'),
	'all contexts' => TRUE,
);

/**
 * Building the configuration form of a Remote HTML Block.
 */
function wallyextra_remotehtml_content_type_edit_form(&$form, &$form_state) {
  $settings = wallyextra_remotehtml_content_type_get_conf($form_state['subtype'], $form_state['conf']);
  $form_state['settings'] = $settings;
  $creation_infos = explode('_', $form_state['conf']['caid']);
  $creation_date = $creation_infos[1];

  if ($settings['remotehtml_type'] == 'fixed' && ($creation_date+1)>=time()) {
    return; // no form for this case.
  }
              
  $form['uri'] = array (
    '#type'             => 'textfield',
    '#title'            => t('URL of the HTML Block to include'),
    '#default_value'    => $settings['uri'],
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
  
  
  // Make the other form items dependent upon it.
  ctools_include('dependent');
  ctools_add_js('dependent');

  $form['reusable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Make this content reusable'),
    '#default_value' => FALSE,
  );
  
  $form['admin_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Administrative title'),
    '#description' => t('The title used as name for the reusable block.'),
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-reusable' => array(1)),
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

function wallyextra_remotehtml_content_type_edit_form_validate(&$form, &$form_state) {
  if (isset($form['uri']['#value'])) {
    $uri_response = drupal_http_request($form['uri']['#value']);
    if ($uri_response->status_message != 'OK') {
      form_set_error('uri', t('The URI '.check_plain($uri_string).' doesn\'t respond.'));
    }
  }
}

/**
 * Submitting the configuration form of a Remote Html Block
 */
function wallyextra_remotehtml_content_type_edit_form_submit(&$form, &$form_state) {
  if ($form_state['settings']['remotehtml_type'] == 'fixed') {
    _wallyextra_remotehtml_content_type_edit_save($form_state['settings']['content'], $form_state);
  } else if (!empty($form_state['values']['reusable'])) {
    // If the 'reusable' checkbox was checked, we will create a new
    // remotehtml content and give it the proper values.
    $content = wallyextra_newctoolsexport('wallyextra_remotehtml_content', 'remotehtml');
    $content->name = $form_state['values']['name'];
    _wallyextra_remotehtml_content_type_edit_save($content, $form_state);
    $form_state['conf']['name'] = $content->name;
  } else {
    // Otherwise, just save values into $conf normally.
    cache_clear_all($cid = $form['caid']['#value'], $table = 'cache');
    $form_state['conf']['uri']=$form['uri']['#value'];
    $form_state['conf']['override_theme']=$form['override_theme']['#value'];
    $form_state['conf']['override_theme_text']=$form['override_theme_text']['#value'];
    $form_state['conf']['caid'] = $form['caid']['#value'];
  }
}

function _wallyextra_remotehtml_content_type_edit_save(&$content, $form_state){
  // Apply updates to the content object.
  $content->name = $form_state['values']['name'];
  $content->category = $form_state['values']['category'];
  $content->admin_title = $form_state['values']['admin_title'];
  $content->admin_description = $form_state['values']['admin_description'];
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    if (isset($form_state['values'][$key])) {
      $content->settings[$key] = $form_state['values'][$key];
    }
  }

  ctools_export_crud_save('wallyextra_remotehtml_content', $content);
} 

/**
 * Return all remotehtml content types available.
 */
function wallyextra_remotehtml_content_type_content_types() {
  ctools_include('export');
  $types = array();
  $types['remotehtml'] = _wallyextra_remotehtml_default_ctools_content_type();
  
  foreach (ctools_export_crud_load_all('wallyextra_remotehtml_content') as $name => $content) {
    $types[$name] = _wallyextra_remotehtml_ctools_content_type($content);
  }
  return $types;
}

/**
 * Return the remotehtml content types with the specified $subtype_id.
 */
function wallyextra_remotehtml_content_type_content_type($subtype_id) {
  if ($subtype_id == 'remotehtml') {
    return _wallyextra_remotehtml_default_ctools_content_type();
  }
  else {
    ctools_include('export');
    $content = ctools_export_crud_load('wallyextra_remotehtml_content', $subtype_id);
    if ($content) {
      return _wallyextra_remotehtml_ctools_content_type($content);
    }
  }
}

/**
 * Settings for the default remotehtml content type.
 *
 * The default is the one that allows the user to actually create a type.
 */
function _wallyextra_remotehtml_default_ctools_content_type() {
  $info = array(
    'name' => 'remotehtml',
    'title' => t('Remote HTML'),
    'category' => t('Wally Extra'),
    'edit form' => 'wallyextra_remotehtml_content_type_edit_form',
    'description' => t('Remote HTML Block from a trusted partner website. '),
    'all contexts' => TRUE,
    'defaults' => _wallyextra_remotehtml_defaultformvalues(),
  );

  return $info;
}


function _wallyextra_remotehtml_ctools_content_type($content) {
  $info = array(
    'name' => $content->name,
    'title' => check_plain($content->admin_title),
    'description' => check_plain($content->admin_description),
    'category' => $content->category ? check_plain($content->category) : t('Wally reusable blocks'),
    'edit form' => 'wallyextra_remotehtml_content_type_edit_form',
    'all contexts' => TRUE,
    'icon' => 'icon_block_custom.png',
    // Store this here to make it easy to access.
    'content' => $content,
  );

  return $info;
}

/**
 * Given a $subtype and a $conf, return the actual settings to use.
 *
 * The actual settings may be stored directly in the pane or this may
 * be a pointer to re-usable content that may be in the database or in
 * an export. We have to determine from the subtype whether or not it
 * is local or shared remotehtml content.
 * 
 * @param $subtype
 *   The given subtype.
 * @param $conf
 *   The configuration informations.
 * 
 * @return
 *   The usable settings.
 */
function wallyextra_remotehtml_content_type_get_conf($subtype, $conf) {
  if ($subtype['name'] != 'remotehtml') {
    $settings = $subtype['content']->settings;
    $settings['remotehtml_type'] = 'fixed';
    $settings['content'] = $subtype['content'];
  } else {
    // This means they created it as remotehtml content and then set it as
    // reusable. Since we're not allowed to change the subtype, we're
    // still stored as though we are local, but are pointing off to
    // non-local.
    if (!empty($conf['name'])) {
      ctools_include('export');
      $content = ctools_export_crud_load('wallyextra_remotehtml_content', $conf['name']);
      if ($content) {
        $settings = $content->settings;
      } else {
        $content = wallyextra_newctoolsexport('wallyextra_remotehtml_content', 'remotehtml');
        $content->name = $conf['name'];
        $settings = _wallyextra_remotehtml_defaultformvalues();
      }
      $settings['remotehtml_type'] = 'fixed';
      $settings['content'] = $content;
    } else {
      // This means that it is created as remotehtml and has not been set to
      // reusable.
      $settings = $conf;
      $settings['remotehtml_type'] = 'local';
    }
  }

  return $settings;
}

/**
 * Rendering of the Remote HTML ctools.
 */
function wallyextra_remotehtml_content_type_render($subtype, $conf, $panel_args, $context) {
  $settings = wallyextra_remotehtml_content_type_get_conf(wallyextra_remotehtml_content_type_content_type($subtype), $conf);
  
  $block = new stdClass();
  $block->module = 'wallyextra';
  $block->title = 'HTML Block';
  $cid = $conf['caid'];
  
  $htmlblock = wallyextra_get_cache_or_create($cid, '_wallyextra_remotehtml_setcache', $settings, $subtype, $context, 'wallyct_remotehtml', TRUE);
    
  if (is_string($htmlblock))
    $block->content = $htmlblock;
  elseif ($settings['override_theme_text'] != "")// && function_exists($settings['override_theme_text'])){
    $block->content = theme($settings['override_theme_text'], $subtype, $context, $htmlblock, $settings ) ;
  else
    $block->content = theme("wallyct_remotehtml", $subtype, $context, $htmlblock, $settings);
  
  return $block;
}

/**
 * Callback to provide the administrative title of the HTML Block content.
 */
function wallyextra_remotehtml_content_type_admin_title($subtype, $conf) {
  $settings = wallyextra_remotehtml_content_type_get_conf(wallyextra_remotehtml_content_type_content_type($subtype), $conf);

  $output = t('Remote HTML Block');
  if (!empty($settings['content']->admin_title)) {
    $title = $settings['content']->admin_title;
  } elseif (!empty($settings['title'])) {
    $title = $settings['title'];
  } else {
    $title = $settings['caid'];
  }
  
  if ($title) {
    if ($settings['remotehtml_type'] != 'fixed') {
      $output = t('Remote HTML Block: @title', array('@title' => $title));
    }
    else {
      $output = t('Reusable Remote HTML Block: @title', array('@title' => $title));
    }
  }

  return $output;
}

/**
 * Callback to provide administrative info. In this case we'll render the
 * content as long as it's not PHP, which is too risky to render here.
 */
function wallyextra_remotehtml_content_type_admin_info($subtype, $conf) {
  $settings = wallyextra_remotehtml_content_type_get_conf(wallyextra_remotehtml_content_type_content_type($subtype), $conf);

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
 * Custom refresh action for Remote HTML blocks
 */
function wallyextra_remotehtml_refresh_block($settings) {
  _wallyextra_remotehtml_setcache($settings);
}

/**
 * Definition of the default values of the edit form.am
 * 
 * @return
 *   The default values.
 */
function _wallyextra_remotehtml_defaultformvalues() {
  $default_form_values = array();
  $default_form_values['uri'] = '';
  $default_form_values['override_theme'] = 0;
  $default_form_values['override_theme_text'] = '';
  $default_form_values['caid'] = 'remotehtml_'.time();

  return $default_form_values;
}

/**
 * Refresh the cache for the selected remote HTML entry in the cache table
 * 
 * @param $settings
 *   The array containing the settings for this object.
 * 
 * @return
 *   The remote HTML content.
 */
function _wallyextra_remotehtml_setcache($settings) {
  $internal_url = drupal_get_normal_path(trim($_SERVER[REQUEST_URI], '/'));
  wallytoolbox_flush_all_pages($internal_url, TRUE);

  $block_uri = $settings['uri'];
  $htmlblock = file_get_contents($block_uri);

  cache_set($cid, $htmlblock, 'cache');

  return $htmlblock;
}
