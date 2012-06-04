<?php 

/**
 * Return the HTML of the stats for a node
 */
function wallystat_show_stats($nid) {
  $cached_content = cache_get('wallystat_stats_content_'.$nid);
  $refresh_time = wally_variable_get('wallystat_stats_content_refresh_time', 15);

  if (!is_object($cached_content) || !isset($cached_content) || empty($cached_content) || ($cached_content->created + $refresh_time*60) < time()) {
    if (module_exists('chart')) {
      module_load_include('module', 'chart');

      // This cache doesn't exists or is too old, create it
      $last_48h = theme('wallystat_show_stats_48h', $nid);
      $params = array();
      for ($i = 1; $i <= 10; $i++) {
        if ($params_callback = variable_get('wallystat_param_'.$i, FALSE)) {
          $params[] = theme('wallystat_show_stats_param', $nid, $i, $params_callback);
        }
      }

      $content = $last_48h.implode('', $params);
    } else {
      t('Please install "chart" module.');
    }
    cache_set('wallystat_stats_content_'.$nid, $content, 'cache');
  } else {
    // The object exists in cache and is not too old
    $content = $cached_content->data;
  }
  
  print $content;
}

function wallystat_set_display_follower($val) {
  cache_clear_all('wallystat_stats_content_', 'cache', TRUE);
  module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
  wally_variable_set('wallystat_display_follower', wallytoolbox_toboolean($val));
  $redirect = isset($_GET['destination']) ? $_GET['destination'] : '';
  drupal_goto($redirect);
}
