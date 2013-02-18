<?php 

function theme_wallydashboard_shortcut($shortcuts_list){
  drupal_add_css(drupal_get_path('module', 'wallydashboard').'/css/dashboard.css');
  $content = '';
  if (is_array($shortcuts_list)){
    $content .= '<ul class="dashboard_shortcut">';
    foreach ($shortcuts_list as $short){
      if ($short['img'] != NULL){
        $content .= '<li title="'.$short['title'].'"><a href="/'.$short['link'].'"><img alt="'.$short['title'].'" src="/'.$short['img'].'" ></a></li>';
      } else {
        $content .= '<li><a href="/'.$short['link'].'">'.$short['title'].'</a></li>';
      }
    }
    $content .= '</ul>';
    $content .= '<div class="clear"></div>';
  }
  return $content;
}
function theme_wallydashboard_status($requirements){
  $content = '';
 
  $header = array(t('Status'), t('Title'), t('Value'), t('Description'));
  $rows = array();
  foreach ($requirements as $require){
    if ($require['severity'] === NULL){
      $require['severity'] = 0;
    }
    $img = '<img alt="'.$require['severity'].'" src="/'.drupal_get_path('module', 'wallydashboard').'/images/status/status_'.$require['severity'].'.png">';
    $rows[] = array($img, $require['title'], $require['value'],$require['description']);
  }
  $content = theme('table', $header, $rows);
  return $content;
}