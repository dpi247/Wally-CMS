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
  'no title override' => FALSE,
  'defaults' => _wallyextra_taxonomylist_defaultformvalues(),
  'js' => array('misc/autocomplete.js', 'misc/textarea.js', 'misc/collapse.js'),
  // Make sure the edit form is only used for some subtypes.
  'edit form' => '',
  'add form' => '',
  'edit text' => t('Edit'),
  'icon' => 'icon_node.png',
  'required context' => new ctools_context_required(t('Node'), 'node'),
);

/**
 * Building the configuration form of a taxonomy list.
 */
function wallyextra_taxonomylist_content_type_edit_form(&$form, &$form_state) {
  $settings = wallyextra_taxonomylist_content_type_get_conf($form_state['subtype'], $form_state['conf']);
  $form_state['settings'] = $settings;

  if ($settings['taxonomylist_type'] == 'fixed' && ($form_state['conf']['creation_date']+1)>=time()) {
    return; // no form for this case.
  }

  $conf = $form_state['conf'];
      
  $form['taxonomyfield'] = array (
    '#type'             => 'select',
    '#title'            => t('Taxonomy field to extract'),
    '#options'          => array(
                            "field_authors"=>t('Authors'),
                            "field_persons"=>t('Persons'),
                            "field_locations"=>t('Locations'),
                            "field_entities"=>t('Entities'),
                            "field_free_tags"=>t('Free Tags'),
                            "field_tags"=>t('Classified Tags'),
                          ),
    '#required'         => TRUE,
    '#default_value'    => $settings['taxonomyfield'],
  );

  $form['taxonomystyle'] = array (
    '#type'             => 'select',
    '#title'            => t('Taxonomy Style'),
    '#options'          => array( 'simple'=>t('Simple List'), 'tree'=>t('Tree List') ),
    '#required'         => TRUE,
    '#default_value'    => $settings['taxonomystyle'],
  );
  
  $form['creation_date'] = array (
    '#type'             => 'hidden',
    '#value'            => $settings['creation_date'],
  );
  
  if (!user_access('administer taxonomylist content') || $settings['taxonomylist_type'] == 'fixed') {
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
 * Submitting the configuration form of a taxonomy list.
 */
function wallyextra_taxonomylist_content_type_edit_form_submit(&$form, &$form_state) {
    if ($form_state['settings']['taxonomylist_type'] == 'fixed') {
    _wallyextra_taxonomylist_content_type_edit_save($form_state['settings']['content'], $form_state);
  } else if (!empty($form_state['values']['reusable'])) {
    // If the 'reusable' checkbox was checked, we will create a new
    // taxonomylist content and give it the proper values.
    $content = wallyextra_newctoolsexport('wallyextra_taxonomylist_content', 'taxonomylist');
    $content->name = $form_state['values']['name'];
    _wallyextra_taxonomylist_content_type_edit_save($content, $form_state);
    $form_state['conf']['name'] = $content->name;
  } else {
    // Otherwise, just save values into $conf normally.
    $form_state['conf']['taxonomyfield']=$form['taxonomyfield']['#value'];
    $form_state['conf']['taxonomystyle']=$form['taxonomystyle']['#value'];
    $form_state['conf']['creation_date'] = $form['creation_date']['#value'];
  }
}

/**
 * Export the configuration of the reusable block.
 */
function _wallyextra_taxonomylist_content_type_edit_save(&$content, $form_state) {
  // Apply updates to the content object.
  $content->category = $form_state['values']['category'];
  $content->admin_title = $form_state['values']['name'];
  $content->admin_description = $form_state['values']['admin_description'];
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    if (isset($form_state['values'][$key])) {
      $content->settings[$key] = $form_state['values'][$key];
    }
  }

  ctools_export_crud_save('wallyextra_taxonomylist_content', $content);
}

/**
 * Return all taxonomylist content types available.
 */
function wallyextra_taxonomylist_content_type_content_types() {
  ctools_include('export');
  $types = array();
  $types['taxonomylist'] = _wallyextra_taxonomylist_default_ctools_content_type();
  
  foreach (ctools_export_crud_load_all('wallyextra_taxonomylist_content') as $name => $content) {
    $types[$name] = _wallyextra_taxonomylist_ctools_content_type($content);
  }
  return $types;
}

/**
 * Return the taxonomylist content types with the specified $subtype_id.
 */
function wallyextra_taxonomylist_content_type_content_type($subtype_id) {
  if ($subtype_id == 'taxonomylist') {
    return _wallyextra_taxonomylist_default_ctools_content_type();
  }
  else {
    ctools_include('export');
    $content = ctools_export_crud_load('wallyextra_taxonomylist_content', $subtype_id);
    if ($content) {
      return _wallyextra_taxonomylist_ctools_content_type($content);
    }
  }
}

/**
 * Settings for the default taxonomylist content type.
 *
 * The default is the one that allows the user to actually create a type.
 */
function _wallyextra_taxonomylist_default_ctools_content_type() {
  $info = array(
    'name' => 'taxonomylist',
    'title' => t('Package related Taxonomy List (simple/tree)'),
    'category' => t('Packages'),
    'edit form' => 'wallyextra_taxonomylist_content_type_edit_form',
    'description' => t('Fetch all taxonomy terms related to package node (List/Tree Mode).'),
    'defaults' => _wallyextra_taxonomylist_defaultformvalues(),
  );

  return $info;
}

/**
 * Return an info array for a specific taxonomylist content type.
 */
function _wallyextra_taxonomylist_ctools_content_type($content) {
  $info = array(
    'name' => $content->name,
    'title' => check_plain($content->admin_title),
    'description' => check_plain($content->admin_description),
    'category' => $content->category ? check_plain($content->category) : t('Wally reusable blocks'),
    'edit form' => 'wallyextra_taxonomylist_content_type_edit_form',
    // Store this here to make it easy to access.
    'content' => $content,
  );

  return $info;
}

/**
 * Rendering of the taxonomy list ctools.
 */
function wallyextra_taxonomylist_content_type_render($subtype, $conf, $panel_args, $context) {
  $settings = wallyextra_taxonomylist_content_type_get_conf(wallyextra_taxonomylist_content_type_content_type($subtype), $conf);
  
  $node = isset($context->data) ? drupal_clone($context->data) : NULL;
  $block = new stdClass();
  
  if (empty($node)) {
    $block->title = "Error.";
    $block->content = t('wallycontenttype: A context is required.<br/> Please specify a node context in the settings of this pane');
  } else {
    $taxonomyfield = $settings['taxonomyfield'];
         
    $block->title = "Taxonomy Tag List.";
    if ($conf['taxonomystyle']=="tree") {
      $block->content = theme("wallyct_taxotermlist_tree",$node->$taxonomyfield, $node, $settings); 
    } else {
      $block->content = theme("wallyct_taxotermlist",$node->$taxonomyfield, $node, $settings); 
    } 

  }
  return $block;
}

/**
 * Given a $subtype and a $conf, return the actual settings to use.
 *
 * The actual settings may be stored directly in the pane or this may
 * be a pointer to re-usable content that may be in the database or in
 * an export. We have to determine from the subtype whether or not it
 * is local or shared taxonomylist content.
 * 
 * @param $subtype
 *   The given subtype.
 * @param $conf
 *   The configuration informations.
 * 
 * @return
 *   The usable settings.
 */
function wallyextra_taxonomylist_content_type_get_conf($subtype, $conf) {
  if ($subtype['name'] != 'taxonomylist') {
    $settings = $subtype['content']->settings;
    $settings['taxonomylist_type'] = 'fixed';
    $settings['content'] = $subtype['content'];
  } else {
    // This means they created it as taxonomylist content and then set it as
    // reusable. Since we're not allowed to change the subtype, we're
    // still stored as though we are local, but are pointing off to
    // non-local.
    if (!empty($conf['name'])) {
      ctools_include('export');
      $content = ctools_export_crud_load('wallyextra_taxonomylist_content', $conf['name']);
      if ($content) {
        $settings = $content->settings;
      } else {
        $content = wallyextra_newctoolsexport('wallyextra_taxonomylist_content', 'taxonomylist');
        $content->name = $conf['name'];
        $settings = _wallyextra_taxonomylist_defaultformvalues();
      }
      $settings['taxonomylist_type'] = 'fixed';
      $settings['content'] = $content;
    } else {
      // This means that it is created as taxonomylist and has not been set to
      // reusable.
      $settings = $conf;
      $settings['taxonomylist_type'] = 'local';
    }
  }

  return $settings;
}

/**
 * Callback to provide the administrative title of the taxonomylist content.
 */
function wallyextra_taxonomylist_content_type_admin_title($subtype, $conf) {
  $settings = wallyextra_taxonomylist_content_type_get_conf(wallyextra_taxonomylist_content_type_content_type($subtype), $conf);

  $output = t('Package related Taxonomy List (simple/tree)');
  $title = !empty($settings['admin_title']) ? $settings['admin_title'] : $settings['title'];
  if ($title) {
    if ($settings['taxonomylist_type'] != 'fixed') {
      $output = t('Package related Taxonomy List (simple/tree): @title', array('@title' => $title));
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
function wallyextra_taxonomylist_content_type_admin_info($subtype, $conf) {
  $settings = wallyextra_taxonomylist_content_type_get_conf(wallyextra_taxonomylist_content_type_content_type($subtype), $conf);

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
function _wallyextra_taxonomylist_defaultformvalues() {
  $default_form_values = array();
  $default_form_values['taxonomyfield'] = 'field_authors';
  $default_form_values['taxonomystyle'] = 'simple';
  $default_form_values['creation_date'] = time();

  return $default_form_values;
}
