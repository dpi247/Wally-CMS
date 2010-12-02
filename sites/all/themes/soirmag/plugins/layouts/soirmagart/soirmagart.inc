<?php

// Plugin definition
$plugin = array(
  'title' => t('Three column + one header SOIRMAG'),
  'category' => t('Soirmag'),
  'icon' => 'soirmagart.png',
  'theme' => 'soirmagart',
  'css' => 'soirmagart.css',
  'panels' => array(
    'colarticle' =>t('Colonne Article'),
    'colrelated' => t('Colonne Related'),
    'colmilieu' => t('Colonne du Milieu'),
    'coldroite' => t('Colonne Droite')
   ),
);
