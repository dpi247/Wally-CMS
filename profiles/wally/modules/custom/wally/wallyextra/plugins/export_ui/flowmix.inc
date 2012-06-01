<?php
// $Id: flowmix.inc,v 1.1.2.1 2010/07/14 01:57:42 merlinofchaos Exp $

$plugin = array(
  'schema' => 'wallyextra_flowmix_content',
  'access' => 'administer page manager',

  'menu' => array(
    //important ! this menu item should be the same as the one define in the path of the module file no space allowed
    'menu item' => 'flowmix',
    'menu title' => t('WallyExtra FlowMix blocks'),
    'menu description' => t('Manage FlowMix blocks.'),
    'menu prefix' => 'admin/build/wallyextra',
    'items' => array(
      'add' => array(),
      'edit callback' => array(),
      'edit' => array(),
    ),
  ),

  'title singular' => t('flowmix'),
  'title singular proper' => t('FlowMix'),
  'title plural' => t('flowmixes'),
  'title plural proper' => t('FlowMixes'),

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
