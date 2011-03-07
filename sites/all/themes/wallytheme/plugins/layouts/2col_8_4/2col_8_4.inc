<?php

// Plugin definition
$plugin = array(
  'title' => t('two columns 8 and 4 + top / bottom'),
  'category' => t('Soirmag'),
  'icon' => '2col_8_4.png',
  'theme' => '2col_8_4',
  'css' => '2col_8_4.css',
  'panels' => array(
    'top_content' => t('Top Content'),
    'colone_above_8' => t('Column 1 large above content'),
    'colone_above_4_1' => t('Column 1 second above content left'),
    'colone_above_4_2' => t('Column 1 second above content right'),
    'colone_middle_8' => t('Column 1 large middle content'),
    'colone_middle_4_1' => t('Column 1 second middle content left'),
    'colone_middle_4_2' => t('Column 1 second middle content right'),
    'colone_bottom_8' => t('Column 1 large bottom content'),
    'colone_bottom_4_1' => t('Column 1 second bottom content left'),
    'colone_bottom_4_2' => t('Column 1 second bottom content right'),
    'coltwo' => t('Column 2'),
    'bottom_content' => t('Bottom Content')
   ),
);
