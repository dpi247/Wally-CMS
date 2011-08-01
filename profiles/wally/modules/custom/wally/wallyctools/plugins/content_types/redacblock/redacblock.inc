<?php
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
  'no title override' => FALSE,
  'defaults' => array(
    'admin_title' => '',
    'title' => '',
    'override_pager_settings' => FALSE,
    'use_pager' => FALSE,
    'nodes_per_page' => 10,
    'pager_id' => 0,
    'offset' => 0,
    'more_link' => FALSE,
    'substitute' => TRUE,
    'link_to_view' => FALSE,
    'panel_args' => FALSE,
    'args' => '',
    'url' => '',
    'required context' => new ctools_context_required(t('Term'), 'term'),
  ),
  'js' => array('misc/autocomplete.js', 'misc/textarea.js', 'misc/collapse.js'),
  'edit text' => t('Edit'),
  'all contexts' => TRUE,
);

/**
 * Return the custom content types with the specified $subtype_id.
 */
function wallyctools_redacblock_content_type_content_type($subtype_id) {
  if ($subtype_id == 'redacblock') {
    return _wallyctools_default_content_type();
  }
  else {
    ctools_include('export');
    $content = ctools_export_crud_load('wallyctools', $subtype_id);
    if ($content) {
      return _wallyctools_redacblock_content_type($content);
    }
  }
}

/**
 * Return all redacblock content types available.
 */
function wallyctools_redacblock_content_type_content_types($plugin) {
  ctools_include('export');
  $types = array();
  $types['redacblock'] = _wallyctools_default_content_type();

  foreach (ctools_export_crud_load_all('wallyctools') as $name => $content) {
    $types[$name] = _wallyctools_redacblock_content_type($content);
  }
  return $types;
}

/**
 * Settings for the default redacblock content type.
 *
 * The default is the one that allows the user to actually create a new reusable type.
 */
function _wallyctools_default_content_type() {
  $plugin = array(
    'name' => 'redacblock',
    'title' => t('New redacblock content'),
    'top level' => TRUE,
    'category' => t('redacblock'),
    'description' => t('Create a completely new redacblock'),
    'edit form' => 'ctools_redacblock_content_type_edit_form',
    'all contexts' => TRUE,
    'add form' => array(
      'redacblock_new_redacblock_form' => t('Create RedacBlock'),
      'wallyctools_redacblock_content_type_edit_form' => array(
        'default' => TRUE, // put wrapper here, not on the previous form.
        'title' => t('Override settings of the redacblock'),
      ),
    ),
  );
  
  return $plugin;
}

/**
 * Returns the administrative title for a type.
 */
function wallyctools_redacblock_content_type_admin_title($subtype, $conf) {
  return $subtype;
}

/**
 * Return an info array for a specific redacblock content type.
 */
function _wallyctools_redacblock_content_type($content) {
  $info = array(
    'name' => $content->name,
    'title' => check_plain($content->admin_title),
    'description' => check_plain($content->admin_description),
    'category' => $content->category ? check_plain($content->category) : t('Redac Block'),
    'all contexts' => TRUE,
  //'icon' => 'icon_block_redacblock.png',
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
 * is local or shared redacblock content.
 */
function wallyctools_redacblock_content_type_get_conf($subtype, $conf) {
  if ($subtype['name'] != 'redacblock') {
    $settings = $subtype['content']->settings;
    $settings['redacblock_type'] = 'fixed';
    $settings['content'] = $subtype['content'];
    $settings=array_merge($settings,$conf);
  }
  else {
    // This means they created it as redacblock content and then set it as
    // reusable. Since we're not allowed to change the subtype, we're
    // still stored as though we are local, but are pointing off to
    // non-local.
    if (!empty($conf['name'])) {
      ctools_include('export');
      $content = ctools_export_crud_load('wallyctools', $settings['name']);
      if ($content) {
        $settings = $content->settings;
        $settings['redacblock_type'] = 'fixed';
        $settings['content'] = $content;
      }
      else {
        $content = ctools_export_crud_new('wallyctools');
        $content->name = $conf['name'];
        $settings = array(
          'admin_title' => t('Missing/deleted content'),
          'title' => '',
          'body' => '',
          'format' => FILTER_FORMAT_DEFAULT,
          'substitute' => TRUE,
          'redacblock_type' => 'fixed',
          'content' => $content,
        );
      }
    }
    // This means that it is created as redacblock and has not been set to
    // reusable.
    else {
      $settings = $conf;
      $settings['redacblock_type'] = 'local';
    }
  }

  /*
   //SET GLOBAL SETTINGS
   $settings['item'] = $conf['item'];
   $settings['offset'] = $conf['offset'];
   $settings['title_override'] = $conf['title_override'];
   */

  return $settings;
}

function _wallyctools_views_conf(&$conf, $subtype) {
  $plugin = ctools_get_content_type('views');

  // Special: Existing content types get a different default than new ones:
  if (!empty($conf) && !isset($conf['override_pager_settings'])) {
    $conf['override_pager_settings'] = TRUE;
  }

  // Make sure that our defaults are always set if there is no
  // previous setting. This helps updates go more smoothly.
  foreach ($plugin['defaults'] as $key => $value) {
    if (!isset($conf[$key])) {
      $conf[$key] = $value;
    }
  }

  if (strpos($subtype, '-')) {
    list($name, $display) = explode('-', $subtype);
    $view = views_get_view($name);
    if (!isset($conf['display'])) {
      $conf['display'] = $display;
    }
  }
  else {
    $name = $subtype;
    $view = views_get_view($subtype);
    $display = isset($conf['display']) ? $conf['display'] : 'default';
  }

  if (empty($view)) {
    return $name;
  }

  $view->set_display($display);
  // $view->current_display will now reflect this value.

  // If set NOT to override, go ahead and refresh from the view.
  if (empty($conf['override_pager_settings'])) {
    if (method_exists($view, 'init_pager')) {
      $pager = $view->display_handler->get_option('pager');
      $conf['use_pager'] = $pager['type'] != 'none' && $pager['type'] != 'some';
      $conf['pager_id'] = isset($pager['options']['id']) ? $pager['options']['id'] : 0;
      $conf['offset'] = isset($pager['options']['offset']) ? $pager['options']['offset'] : 0;
      $conf['nodes_per_page'] = isset($pager['options']['items_per_page']) ? $pager['options']['items_per_page'] : 0;
    }
    else {
      $conf['use_pager'] = $view->display_handler->get_option('use_pager');
      $conf['pager_id'] = $view->display_handler->get_option('element_id');
      $conf['nodes_per_page'] = $view->display_handler->get_option('items_per_page');
      $conf['offset'] = $view->display_handler->get_option('offset');
    }
  }
  return $view;
}

/**
 * Output function for the 'redacblock' content type. Outputs a redacblock
 * based on the module and delta supplied in the configuration.
 */
function wallyctools_redacblock_content_type_render($subtype, $conf, $panel_args, $contexts) {
  $conf =wallyctools_redacblock_content_type_get_conf(wallyctools_redacblock_content_type_content_type($subtype), $conf);
  $redacblock_name = $conf['content']->name;
  if (!is_array($contexts)) {
    $contexts = array($contexts);
  }

  $view_name = $conf['content']->view_name;
  $view_id = $conf['content']->view_id;

  $view = views_get_view($view_name);

  dsm($conf, 'conf');
  dsm($contexts, 'contexts');
  dsm($view, 'view');

  $view->set_display($view_id);
  $options = array(
    'field_destinations_target' => array(
      'operator' => '=',
      'value' => $redacblock_name,
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'case' => 1,
      'id' => 'field_destinations_target',
      'table' => 'node_data_field_destinations',
      'field' => 'field_destinations_target',
      'relationship' => 'none',
    ),
  );

  $options = array(
    'operator' => '=',
    'value' => $redacblock_name,
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'case' => 1,
    'id' => 'field_destinations_target',
    'table' => 'node_data_field_destinations',
    'field' => 'field_destinations_target',
    'relationship' => 'none',
  );

  $options = array(
    'operator' => '=',
    'value' => $redacblock_name,
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'relationship' => 'none',
  );

  //$view->display_handler->get_handlers('argument')

  $view->add_item($view_id, 'filter', 'node_data_field_destinations', 'field_destinations_target', $options);
  $view->is_cacheable = FALSE;
  //init handler should be placed after add_item !!!
  $view->init_handlers();

  //$view->display[$view_id]->handler->handlers['filter']['t']=$options;
  //$view->set_item($view_name, 'filter', $name, $item);

  $block->delta  = $view_name .'-'.  $view_id;

  /*$handler->override_option('filters', array(
    'des_target' => array(
      'operator' => '=',
      'value' => 'primary_list',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'relationship' => 'none',
      ),
    )
  );*/

  if (empty($view) || !is_object($view) || empty($view->display_handler)) {
    return;
  }

  if (!$view->display_handler->access($GLOBALS['user'])) {
    return;
  }

  $arguments = explode('/', $_GET['q']);
  $args = $conf['args'];
  foreach ($arguments as $id => $arg) {
    $args = str_replace("%$id", $arg, $args);
  }

  // if (isset($panel_args)) {
    foreach ($panel_args as $id => $arg) {
      if (is_string($arg)) {
        $args = str_replace("@$id", $arg, $args);
      }
    }
  //}
  $args = preg_replace(',/?(%\d|@\d),', '', $args);
  $args = $args ? explode('/', $args) : array();

  if ($conf['panel_args'] && is_array($panel_args)) {
    $args = array_merge($panel_args, $args);
  }
  if (isset($contexts) && is_array($contexts)) {
    array_splice($args, $count, 0, array($contexts['argument_terms_1']->data->tid));
  }
  $view->set_arguments($args);

  if ($conf['url']) {
    $view->override_path = $conf['url'];
  }

  $block = new stdClass();
  $block->module = 'views';
  $block->delta  = $view->name .'-'.  $view->current_display;

  if (!empty($conf['link_to_view'])) {
    $block->title_link = $view->get_url();
  }

  if (!empty($conf['more_link'])) {
    $block->more = array('href' => $view->get_url());
    $view->display_handler->set_option('use_more', FALSE);
  }

  // Only set use_pager if they differ, this way we can avoid overwriting the
  // pager type that Views uses.
  if ($conf['override_pager_settings']) {
    if (method_exists($view, 'init_pager')) {
      // Views 3 version
      $view->set_items_per_page($conf['nodes_per_page']);
      $view->set_offset($conf['offset']);

      $pager = $view->display_handler->get_option('pager');
      if ($conf['use_pager'] && ($pager['type'] == 'none' || $pager['type'] == 'some')) {
        $pager['type'] = 'full';
      }
      elseif (!$conf['use_pager'] && $pager['type'] != 'none' && $pager['type'] != 'some') {
        $pager['type'] = $view->get_items_per_page() ? 'some' : 'none';
      }

      if ($conf['use_pager']) {
        if (!isset($pager['options']['id']) || $pager['options']['id'] != $conf['pager_id']) {
          $pager['options']['id'] = $conf['pager_id'];
        }
      }

      $view->display_handler->set_option('pager', $pager);
    }
    else {
      if (!$view->display_handler->get_option('use_pager') || empty($conf['use_pager'])) {
        $view->display_handler->set_option('use_pager', $conf['use_pager']);
      }
      $view->display_handler->set_option('pager_element', $conf['pager_id']);
      $view->display_handler->set_option('items_per_page', $conf['nodes_per_page']);
      $view->display_handler->set_option('offset', $conf['offset']);
    }
  }

  $stored_feeds = drupal_add_feed();
  $block->content = $view->preview();
  $block->title = $view->get_title();

  if (empty($view->result) && !$view->display_handler->get_option('empty') && empty($view->style_plugin->definition['even empty'])) {
    return;
  }

  if (!empty($conf['feed_icons'])) {
    $new_feeds = drupal_add_feed();
    if ($diff = array_diff(array_keys($new_feeds), array_keys($stored_feeds))) {
      foreach ($diff as $url) {
        $block->feeds[$url] = $new_feeds[$url];
      }
    }
  }

  $view->destroy();
  return $block;
}

/**
 * Returns an edit form for the redacblock type.
 */
function wallyctools_redacblock_content_type_edit_form(&$form, &$form_state) {
  dsm(func_get_args(), 'args');
  $conf = wallyctools_redacblock_content_type_get_conf($form_state['subtype'], $form_state['conf']);
  $view_name = $conf['content']->view_name;
  $view_id = $conf['content']->view_id;

  $view = views_get_view($view_name);
  $view->set_display($view_id);
  $form_state['settings'] = $conf;

  if (!user_access('administer redacblock content')) {
    return;
  }

  if (empty($view) || !is_object($view)) {
    $form['markup'] = array('#value' => t('Broken/missing/deleted view.'));
    return;
  }

  $form['contexts'] = array(
    '#title' => t('Substitutions'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
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

  // @todo
  // If using the older format, just a context is listed. We should go through
  // and check for that and forcibly set them to the right converter so that
  // it doesn't get changed to some whacky default. Oooor just let it get changed
  // to 'no context', I suppose.

  $required = array();
  if (isset($view->display_handler) && $arguments = $view->display_handler->get_handlers('argument')) {
    foreach ($arguments as $arg) {
      $required[] = new ctools_context_optional($arg->ui_name(), 'any');
    }
  }

  /*if ($required) {
    $form['context'] = ctools_context_converter_selector($form_state['contexts'], $required, isset($conf['context']) ? $conf['context'] : array());
  }*/

  $form['pager_settings'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#title' => t('Custom pager settings'),
  );

  $form['pager_settings']['override_pager_settings'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use different pager settings from view settings'),
    '#default_value' => $conf['override_pager_settings'],
    '#id' => 'override-pager-checkbox',
  );

  if ($view->display_handler->get_option('use_ajax')) {
    $form['pager_settings']['warning'] = array(
      '#value' => '<div>' . t('<strong>Warning: </strong> This view has AJAX enabled. Overriding the pager settings will work initially, but when the view is updated via AJAX, the original settings will be used. You should not override pager settings on Views with the AJAX setting enabled.') . '</div>',
    );
  }
  
  $form['pager_settings']['use_pager'] = array(
    '#prefix' => '<div class="container-inline">',
    '#type' => 'checkbox',
    '#title' => t('Use pager'),
    '#default_value' => $conf['use_pager'],
    '#id' => 'use-pager-checkbox',
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('override-pager-checkbox' => array(1)),
  );
  
  $form['pager_settings']['pager_id'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['pager_id'],
    '#title' => t('Pager ID'),
    '#size' => 4,
    '#id' => 'use-pager-textfield',
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('override-pager-checkbox' => array(1), 'use-pager-checkbox' => array(1)),
    '#dependency_count' => 2,
    '#suffix' => '</div>',
  );

  $form['pager_settings']['nodes_per_page'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['nodes_per_page'],
    '#size' => 4,
    '#title' => t('Num posts'),
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('override-pager-checkbox' => array(1)),
  );

  $form['pager_settings']['offset'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['offset'],
    '#title' => t('Offset'),
    '#size' => 4,
    '#description' => t('The number of items to skip and not display.'),
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('override-pager-checkbox' => array(1)),
  );

  $form['link_to_view'] = array(
    '#type' => 'checkbox',
    '#default_value' => $conf['link_to_view'],
    '#title' => t('Link title to view'),
  );

  $form['more_link'] = array(
    '#type' => 'checkbox',
    '#default_value' => $conf['more_link'],
    '#title' => t('Provide a "more" link that links to the view'),
    '#description' => t('This is independent of any more link that may be provided by the view itself; if you see two more links, turn this one off. Views will only provide a more link if using the "block" type, however, so if using embed, use this one.'),
  );

  $form['panel_args'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send arguments'),
    '#default_value' => $conf['panel_args'],
    '#description' => t('Select this to send all arguments from the panel directly to the view. If checked, the panel arguments will come after any context arguments above and precede any additional arguments passed in through the Arguments field below. Note that arguments do not include the base URL; only values after the URL or set as placeholders are considered arguments.'),
  );

  $form['args'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['args'],
    '#title' => t('Arguments'),
    '#size' => 30,
    '#description' => t('Additional arguments to send to the view as if they were part of the URL in the form of arg1/arg2/arg3. You may use %0, %1, ..., %N to grab arguments from the URL. Or use @0, @1, @2, ..., @N to use arguments passed into the panel. Note: use these values only as a last resort. In future versions of Panels these may go away.'),
  );

  $form['url'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['url'],
    '#title' => t('Override URL'),
    '#size' => 30,
    '#description' => t('If this is set, override the View URL; this can sometimes be useful to set to the panel URL'),
  );

  $form_state['title'] = t('Configure view @view (@display)', array('@view' => $view->name, '@display' => $view->display[$conf['display']]->display_title));

  // Make the other form items dependent upon it.
  ctools_include('dependent');
  ctools_add_js('dependent');
  if ($conf['redacblock_type'] != 'fixed') {
    $form['admin_title'] = array(
    '#type' => 'textfield',
    '#default_value' => isset($conf['admin_title']) ? $conf['admin_title'] : '',
    '#title' => t('Administrative title'),
    '#description' => t('This title will be used administratively to identify this pane. If blank, the regular title will be used.'),
    );

    $form['title'] = array(
      '#type' => 'textfield',
      '#default_value' => $conf['title'],
      '#title' => t('Title'),
    );
    
    $form['block']['view'] = array(
      '#type' => 'select',
      '#options'=> wallyctools_get_redac_view_options(),
      '#title' => t('view associated with the block'),
      '#required'=>true,
    );

    $form['reusable'] = array(
      '#type' => 'checkbox',
      '#title' => t('Make this content reusable'),
      '#default_value' => TRUE,
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
      '#description' => t('What category this content should appear in. If left blank the category will be "Miscellaneous".'),
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
  $view->destroy();
  return $form;
}

function _wallyctools_redacblock_content_type_edit_save(&$content, $form_state) {
  // Apply updates to the content object.

  $content->name = $form_state['values']['name'];
  $content->category = $form_state['values']['category'];
  $content->view_name = $form_state['values']['view_name'];
  $content->view_id = $form_state['values']['view_id'];
  $content->admin_title = $form_state['values']['admin_title'];
  $content->admin_description = $form_state['values']['admin_description'];

  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    if (isset($form_state['values'][$key])) {
      $content->settings[$key] = $form_state['values'][$key];
    }
  }
  ctools_export_crud_save('wallyctools', $content);
}

/**
 * The validate form to ensure the redacblock content data is okay.
 */
function wallyctools_redacblock_content_type_edit_form_validate(&$form, &$form_state) {
  if ($form_state['settings']['redacblock_type'] != 'fixed' && !empty($form_state['values']['reusable'])) {
    if (empty($form_state['values']['name'])) {
      form_error($form['name'], t('Name is required.'));
    }

    // Check for string identifier sanity
    if (!preg_match('!^[a-z0-9_]+$!', $form_state['values']['name'])) {
      form_error($form['name'], t('The name can only consist of lowercase letters, underscores, and numbers.'));
      return;
    }

    // Check for name collision
    if ($form_state['values']['name'] == 'redacblock' || (ctools_export_crud_load('wallyctools', $form_state['values']['name']))) {
      form_error($form['name'], t('Content with this name already exists. Please choose another name or delete the existing item before creating a new one.'));
    }

    $view=split('\+',$form_state['values']['view']);
    unset($form_state['values']['view']);
    $form_state['values']['view_name']=$view[0];
    $form_state['values']['view_id']=$view[1];
  }
}

/**
 * The submit form stores the data in $conf.
 */
function wallyctools_redacblock_content_type_edit_form_submit(&$form, &$form_state) {
  //DEFAULT SAVE OVERRIDABLE VARIABLES:
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
  $form_state['conf']['context'] = $form_state['values']['context'];

  //CASE 1: Adding an existing redac block.
  if ($form_state['settings']['redacblock_type'] == 'fixed') {
    _wallyctools_redacblock_content_type_edit_save($form_state['settings']['content'], $form_state);
  }
  // If the 'reusable' checkbox was checked, we will create a new
  // redacblock content and give it the proper values.

  //CASE 1: Create and add a new redac block (re-usable).
  else if (!empty($form_state['values']['reusable'])) {
    $content = ctools_export_crud_new('wallyctools');
    $content->name = $form_state['values']['name'];

    _wallyctools_redacblock_content_type_edit_save($content, $form_state);

    //FIX TO HAVE THE CORRECT PANE SUBTYPE IN THE PANELS-PANE DATABASE
    $form_state['pane']->subtype=$content->name;
  }
  // This case shouldn't be handeled
  else {
    // Otherwise, just save values into $conf normally.
    foreach (array_keys($form_state['plugin']['defaults']) as $key) {
      if (isset($form_state['values'][$key])) {
        $form_state['conf'][$key] = $form_state['values'][$key];
      }
    }
  }

  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    if (isset($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}

function redacblock_new_redacblock_form($form,$form_state){

  ctools_include('ctools_export_ui','ctools','plugins/export_ui');
  //ctools_include('wallyctools.ui.class', 'wallyctools','plugins/export_ui');

  $redacblock_export=new wallyctools_ui();
  $redacblock_export->edit_form(&$form, &$form_state);

  $form['category'] = array(
    '#type' => 'textfield',
    '#title' => t('Category'),
    '#description' => t('What category this content should appear in. If left blank the category will be "Miscellaneous".'),
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
  );
  
  /*$form['block']['view'] = array(
    '#type' => 'select',
    '#options'=> wallyctools_get_redac_view_options(),
    '#title' => t('view associated with the block'),
    '#required'=>true,
  );

  $form['substitute'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use context keywords'),
    '#description' => t('If checked, context keywords will be substituted in this content.'),
  );*/

   return $form;
}

/*function redacblock_new_redacblock_form_submit($form,$form_state){
   $redacblock_export=new wallyctools_ui();
   $redacblock_export->edit_form_submit(&$form, &$form_state);
}*/
