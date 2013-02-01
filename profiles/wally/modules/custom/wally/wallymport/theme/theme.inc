<?php

function theme_wallymport_logs($log){

  $content = '<div class = "wallymport_result">';
  $content .= '<h5 style = "margin:0px;">'.date ('d/m/Y H:i:s',$log['date']).'</h5>';
  if ($log['error']){
    $processed = t('Process failed');
  }
  else{
  	$processed = t('Process succeed');
  }
  $content .= '<div>'.$processed.'</div>';
  if (isset($log['error_msg'])){
    $content .= '<h5 style = "margin:0px;">'.t('Error messages').'</h5>';
    $content .= '<ul>';
    $content .= '<li> '.$log['error_msg'].'</li>';
    $content .= '</ul>';
  }
  if (isset($log['log']['log'])){
    $content .= '<h5 class = "logs" style = "margin:0px;">'.t('Logs').'</h5>';
    $content .= '<ul>';
    foreach ($log['log']['log'] as $msg_log){
      $content .=  '<li> '.$msg_log.'</li>';
    }
    $content .= '<ul>';
  }
  $content .= '</div>';
  return $content;
}