<?php

// Plugin definition
$plugin = array(
  'title' => t('two columns + header / footer'),
  'category' => t('WallyNews'),
  'icon' => 'detailpage.png',
  'theme' => 'detailpage',
  'css' => 'detailpage.css',
  'panels' => array(
    'header' => t('Header'),
    'colmain' =>t('Main column'),
    'colrelated' => t('Related column'),
    'footer' => t('Footer')
   ),
);
