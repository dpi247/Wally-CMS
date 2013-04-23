<?php 
$plugin = array(
  'title' => t('Article Default'),
  'category' => t('The Unfolded'),
  'icon' => 'base-layout-article.png',
  'theme' => 'base-layout-article',
  'css' => 'base-layout-article.css',  
  'admin css' => 'admin-base-layout-article.css',
  'panels' => array(
    'contenttop' => t('Content Top'),
		'mainContent' => t('Main Content'),
		'complementary' => t('Complementary'),
    'subcontentleft' => t('Sub Content Left'),
    'subcontentright' => t('Sub Content Right'),
    'contentbottom' => t('Content Bottom'),
  )
);
?>