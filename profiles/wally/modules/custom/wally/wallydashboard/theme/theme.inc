<?php 

function theme_wallydashboard_shortcut($shortcuts_list){
  drupal_add_css(drupal_get_path('module', 'wallydashboard').'/css/dashboard.css');
  $content = '';
  if (is_array($shortcuts_list)){
    $content .= '<ul class="dashboard_shortcut">';
    foreach ($shortcuts_list as $short){
      if ($short['img'] != NULL){
        $content .= '<li><a href="/'.$short['link'].'"><img alt="'.$short['title'].'" src="/'.$short['img'].'"></a></li>';
      } else {
        $content .= '<li><a href="/'.$short['link'].'">'.$short['title'].'</a></li>';
      }
    }
    $content .= '</ul>';
    $content .= '<div class="clear"></div>';
  }
  return $content;
}