<?php 

function theme_wallydashboard_shortcut($shortcuts_list){
  $content = '';
  dsm($shortcuts_list);
  if (is_array($shortcuts_list)){
    $content .= '<ul>';
    foreach ($shortcuts_list as $short){
      $content .= '<li><a href="'.$short['link'].'">'.$short['title'].'</a></li>';
    }
    $content .= '</ul>';
  }
  return $content;
}