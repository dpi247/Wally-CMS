<?php

$href_value = $_GET['hrefvalue'];
$node_path = drupal_get_normal_path(trim($href_value, '/'));
$expl_path = explode('/', $node_path);

if ($expl_path[0] == 'node' && is_numeric($expl_path[1])) {
  $nid = $expl_path[1];
  $temp_node_hours_stats = array();
  $param_name_id = ($param_i == 10) ? 'param'.$param_i : 'param0'.$param_i;
  $node_stats_db = db_query('SELECT %s, SUM(totalcount) FROM {wallystat_node_counter} WHERE nid = "%s" GROUP BY %s', $param_name_id, $nid, $param_name_id);

  while ($stat = db_fetch_array($node_stats_db)) {
    $node_stats[] = $stat['SUM(totalcount)'];
    $labels[] = ($stat[$param_name_id] == '') ? 'Not set' : $stat[$param_name_id];
  }
  
  $chart = array(
    '#chart_id' => 'node_views_'.$param_name_id,
    '#title' => t($params_callback[$params_callback['current_callback']]['name']),
    '#type' => CHART_TYPE_PIE,
    '#size' => chart_size(240, 150),
    '#data' => $node_stats,
    '#labels' => $labels,
  );
  
  $rendered_chart = chart_render($chart);
  
  print $rendered_chart ? $rendered_chart : t('Chart error.');
}

?>