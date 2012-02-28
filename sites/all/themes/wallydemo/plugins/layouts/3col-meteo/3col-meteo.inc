<?php 
$plugin = array(
  
  'title' => t('3 colonnes météo'),
  'category' => t('Sudpresse'),
  'icon' => '3col-meteo.png',
  'theme' => '3col-meteo',
  'css' => '3col-meteo.css',

	'panels' => array(
		'top-content' => t('bloc largeur 100% du haut'),
		'col-429-first' => t('colonne 1 du haut'),
		'col-181-first' => t('colonne 2 du haut'),
		'col-300-first' => t('colonne 3 du haut'),
		'middle-content' => t('bloc largeur 100% du milieu'),
		'col-429-second' => t('colonne 1 du bas'),
		'col-181-second' => t('colonne 2 du bas'),
		'col-300-second' => t('colonne 3 du bas'),
		'bottom-content' => t('bloc largeur 100% du bas'),
   )
   );
?>
