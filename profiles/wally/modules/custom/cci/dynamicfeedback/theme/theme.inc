<?php

function theme_dynamicfeedback_page_render($datas) {
$content = '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
</head>
<body>
';

  if (!$datas['nid']) {
	  $content .= '<div><span>Package not found on the Wall-iMedia side</span></div>';
  } else {
    $content .= '<div><span><a href="http://'.$datas['base_url'].'/node/'.$datas['nid'].'" onClick="return popup(this, \'Wally_web_page\');">,'.$datas['title'].'</a>';
    foreach ($datas['options'] as $option => $op_datas) {
      $content .= $op_datas['label'].' : '.$op_datas['value'].'. ';
    }
    $content .= '</span></div>';
  }

  $content .= '</body></html>';
  return $content;
}
