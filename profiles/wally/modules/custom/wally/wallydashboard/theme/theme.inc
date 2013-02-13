<?php 

function theme_wallydashboard_shortcut($shortcuts_list){
  drupal_add_css(drupal_get_path('module', 'wallydashboard').'/css/dashboard.css');
  $content = '';
  if (is_array($shortcuts_list)){
    $content .= '<ul class="dashboard_shortcut">';
    foreach ($shortcuts_list as $short){
      $content .= '<li><a href="'.$short['link'].'"><img src="/'.$short['img'].'"></a></li>';
    }
    $content .= '</ul>';
  }
  return $content;
}