<?php

$plugin = array(
  'schema' => 'wallychannels_channels',
  'access' => 'manage channels',

  'menu' => array(
    //important ! this menu item should be the same as the one define in the path of the module file no space allowed
    'menu item' => 'wallychannels',
    'menu title' => t('Channels'),
    'menu description' => t('Manage Channels.'),
    'menu prefix' => 'admin/content',
    'items' => array(
      'infos' => array(
        'path' => 'infos',
        'title' => t('General Informations'),
        'page callback' => 'wallychannels_plugins_overview',
        'access arguments' => array('view channels plugins'),
        'type' => MENU_LOCAL_TASK,
        'weight' => 10,
      ),
    ),
  ),

  'title singular' => t('channel'),
  'title singular proper' => t('Channel'),
  'title plural' => t('channels'),
  'title plural proper' => t('Channels'),

  'handler' => array(
    'class' => 'channels_ui',
    'parent' => 'ctools_export_ui',
  ),
  
  'export' => array(
    'admin_title' => 'title',
    'admin_description' => 'description',
  ),
  
  'use wizard' => TRUE,
  'form info' => array(
    'free trail' => FALSE,
    'show back' => FALSE,
    'show return' => FALSE,
    'finish callback' => 'ctools_export_ui_wizard_finish',
    'next callback' => 'ctools_export_ui_wizard_next',
    'next text' => t('Continue'),
    'cancel callback' => 'ctools_export_ui_wizard_cancel',
    'order' => array(
      'wallychannels_channels_form_basicsettings_form' => t('Create Channel 1/2 : Basic Settings'),
      'wallychannels_channels_form_pluginspecific_form' => t('Create Channel 2/2 : Plugin Settings'),
    ),
    'forms' => array(
      'wallychannels_channels_form_basicsettings_form' => array(
        'form id' => 'wallychannels_channels_form_basicsettings_form',
      ),
      'wallychannels_channels_form_pluginspecific_form' => array(
        'form id' => 'wallychannels_channels_form_pluginspecific_form',
      ),
    ),
  ),
);

function wallychannels_channels_form_basicsettings_form(&$form, &$form_state) {
  $defaults = _wallychannels_channels_form_basicsetttings_defaults((array)$form_state['item']);

  $form['cid'] = array(
    '#type' => 'hidden',
    '#value' => $defaults['cid'],
  );
  
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Machine Name'),
    '#description' => '',
    '#required' => TRUE,
    '#default_value' => $defaults['name'],
  );
  
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Human Readable Name'),
    '#description' => t('This is the name that will be displayed to users.'),
    '#required' => TRUE,
    '#default_value' => $defaults['title'],
  );
  
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => '',
    '#default_value' => $defaults['description'],
  );
  
  $channels = ctools_get_plugins('wallychannels', 'channels');
  $options_channels = array();
  foreach ($channels as $channel) {
    $options_channels[$channel['name']] = $channel['title'];
  }
  $form['channel'] = array(
    '#type' => 'select',
    '#title' => t('Channel'),
    '#options' => $options_channels,
    '#required' => TRUE,
    '#default_value' => $defaults['channel'],
  );
  
  $form_state['next'] = TRUE;
}

function wallychannels_channels_form_basicsettings_form_validate($form, $form_state) {
  $submitted = $form_state['values'];
  $instances = wallychannels_get_instances_of_channel(TRUE, FALSE);
  if (array_key_exists($submitted['name'], $instances)) {
    form_set_error('name', t('This machine name has already been used.'));
  }
}

function wallychannels_channels_form_basicsettings_form_submit($form, &$form_state) {
  $submitted = $form_state['values'];
  $save_values = array_keys((array)_wallychannels_channels_form_basicsetttings_defaults());
  // maybe don't imitate this foreach
  foreach ($save_values as $value) {
    // Set the values in the cache object -- it gets passed back to the next step
    // because of all that work we did in the form_info array
    $form_state['item']->$value = $submitted[$value];
  }
}

function wallychannels_channels_form_pluginspecific_form(&$form, &$form_state) {
  $form['settings'] = array('#tree' => TRUE);
  wallychannels_get_settings_form_of_plugin(wallychannels_get_plugin_by_name($form_state['item']->channel), $form['settings'], $form_state['item']->settings);
  foreach (element_properties($form['settings']) as $prop) {
    if (is_array($form[$prop])) {
      $form[$prop] = array_merge($form[$prop], $form['settings'][$prop]);
    } elseif ($prop != '#tree') {
      $form[$prop] = $form['settings'][$prop];
    }
  }
}

function wallychannels_channels_form_pluginspecific_form_submit($form, &$form_state) {
  if (strpos($form_state['clicked_button']['#wizard type'], 'finish') !== FALSE) {
    $form_state['item']->settings = $form_state['values']['settings'];
  }
}

function _wallychannels_channels_form_basicsetttings_defaults($form_state = array()) {
  if (isset($form_state) && !empty($form_state)) {
    return $form_state;
  }

  $defaults = array(
    'cid' => NULL,
    'name' => '',
    'title' => '',
    'description' => '',
    'channel' => 0,
  );

  return $defaults;
}
