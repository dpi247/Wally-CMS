<?php

$plugin = array(
  'schema' => 'wallyextra_custom_content',
  'access' => 'administer page manager',

  'menu' => array(
    //important ! this menu item should be the same as the one define in the path of the module file no space allowed
    'menu item' => 'custom-content',
    'menu title' => t('WallyExtra Versionable Custom Content blocks'),
    'menu description' => t('WallyExtra Versionable Custom Content blocks.'),
    'menu prefix' => 'admin/build/wallyextra',
    'items' => array(
      'select-version' => array(
        'path' => 'list/%ctools_export_ui/select-version',
        'title' => t('Select version'),
        'page callback' => 'drupal_get_form',
        'page arguments' => array('wallyextra_custom_content_version_select_version_form', 5),
        'load arguments' => array('custom_version'),
        'access arguments' => array('administer custom content'),
        'type' => MENU_CALLBACK,
      ),
    ),
  ),

  'title singular' => t('versionable content pane'),
  'title singular proper' => t('Versionable content pane'),
  'title plural' => t('versionable content panes'),
  'title plural proper' => t('Versionable content panes'),

  'handler' => array(
    'class' => 'wallyextra_custom_content_plugin_ui',
    'parent' => 'wallyextra_plugin_ui',
  ),

  'allowed operations' => array(
    'select-version' => array('title' => t('Select version')),
    'clone' => NULL,
    'export' => NULL,
    'import' => NULL,
  ),
);
