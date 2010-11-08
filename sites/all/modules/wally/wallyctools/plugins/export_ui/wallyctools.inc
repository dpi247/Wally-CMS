<?php
// $Id: ctools_custom_content.inc,v 1.1.2.1 2010/07/14 01:57:42 merlinofchaos Exp $

$plugin = array(
  'schema' => 'wallyctools',
  'access' => 'administer redactional block',

  'menu' => array(
    //important ! this menu item should be the same as the one define in the path of the module file no space allowed
    'menu item' => 'wallyctools',
    'menu title' => 'WallyCtools redactional block content panes',
    'menu description' => 'Add, edit or delete custom content panes.',
  ),

  'title singular' => t('content pane'),
  'title singular proper' => t('Content pane'),
  'title plural' => t('content panes'),
  'title plural proper' => t('Content panes'),

  'handler' => array(
    'class' => 'wallyctools_ui',
    'parent' => 'ctools_export_ui',
  ),
);

