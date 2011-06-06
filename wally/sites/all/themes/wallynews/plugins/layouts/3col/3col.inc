<?php

// Plugin definition
$plugin = array(
  'title' => t('three columns + header / footer'),
  'category' => t('WallyNews'),
  'icon' => '3col.png',
  'theme' => '3col',
  'css' => '3col.css',
  'panels' => array(
    'header' => t('Header'),
    'colone' =>t('Column 1'),
    'coltwo' =>t('Column 2'),
    'colthree' =>t('Column 3'),
    'footer' => t('Footer')
   ),
);
