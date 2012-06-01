<?php
// $Id: remotehtml.inc,v 1.1.2.1 2010/07/14 01:57:42 merlinofchaos Exp $

$plugin = array(
  'schema' => 'wallyextra_remotehtml_content',
  'access' => 'administer page manager',

  'menu' => array(
    //important ! this menu item should be the same as the one define in the path of the module file no space allowed
    'menu item' => 'remotehtml',
    'menu title' => t('WallyExtra RemoteHTML blocks'),
    'menu description' => t('Manage RemoteHTML blocks.'),
    'menu prefix' => 'admin/build/wallyextra',
    'items' => array(
      'add' => array(),
      'edit callback' => array(),
      'edit' => array(),
    ),
  ),

  'title singular' => t('remotehtml'),
  'title singular proper' => t('RemoteHTML'),
  'title plural' => t('remotehtmls'),
  'title plural proper' => t('RemoteHTMLs'),

  'handler' => array(
    'class' => 'wallyextra_plugin_ui',
    'parent' => 'ctools_export_ui',
  ),
  
  'allowed operations' => array(
    'edit'    => NULL,
    'enable'  => NULL,
    'disable' => NULL,
    'revert'  => NULL,
    'delete' => array(
      'title' => t('Delete'),
    ),
    'clone'   => NULL,
    'import'  => NULL,
    'export'  => NULL,
  ),
);
