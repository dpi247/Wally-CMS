<?php 


$plugin = array(
  'title' => t('3 column'),
  'category' => t('Soirmag'),
  'icon' => '3col_article.png',
  'theme' => '3col_article',
  'css' => '3col_article.css',
	'panels' => array(
    'top_content' => t('Top Content'),
		'colarticle'=>t('Main Content'),
		'colmiddle'=>t('Middle Column'),
    'coltier' => t('Column 2'),
    'bottom_content' => t('Bottom Content')
   )
   );

?>