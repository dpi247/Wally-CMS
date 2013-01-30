<?php 

function theme_sendprint_logs($logs_array){
  
  $content = '';
  $header = array(t('Page'), t('Block'), t('Article'));
  $rows = array();
  foreach ($logs_array as $destination_name => $redacblock){
    $page_title = $destination_name;
    foreach ($redacblock as $redacblock_name => $node){
      $row = array();
      $row[] = $page_title;
      $row[] = $redacblock_name;
      $row[] = '<a href="/'.$node->path.'">'.$node->title.'</a>';
      $rows[] = $row;
      $page_title = '';
    }
  }
  $content .= theme('table', $header, $rows);
  return $content;
}