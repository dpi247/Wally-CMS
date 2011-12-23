<?php

function theme_wallymport_logs($log){

  $content = '';
  $content .= '<h2>'.date ('d/m/Y H:i:s',$log['date']).'</h2>';
  $processed = t('Process succeed');
  if ($log['error']){
    $processed = t('Process failed');
  }
  $content .= '<div>'.$processed.'</div>';
  if (isset($log['error_msg'])){
    $content .= '<h3>'.t('Error messages').'</h3>';
    $content .= '<ul>';
    $content .= '<li> '.$log['error_msg'].'</li>';
    $content .= '</ul>';
  }
  if (isset($log[0]['log']['log'])){
    $content .= '<h3>'.t('Logs').'</h3>';
    $content .= '<ul>';
    foreach ($log[0]['log']['log'] as $msg_log){
      $content .=  '<li> '.$msg_log.'</li>';
    }
    $content .= '<ul>';
  }
  return $content;
}