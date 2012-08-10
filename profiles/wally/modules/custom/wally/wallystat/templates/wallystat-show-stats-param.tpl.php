<?php

if ($nid) {
  $temp_node_hours_stats = array();
  $param_name_id = ($param_i == 10) ? 'param'.$param_i : 'param0'.$param_i;
  $node_stats_db = db_query('SELECT %s, SUM(totalcount) FROM {wallystat_node_counter} WHERE nid = "%s" GROUP BY %s', $param_name_id, $nid, $param_name_id);

  while ($stat = db_fetch_array($node_stats_db)) {
    $node_stats[] = $stat['SUM(totalcount)'];
    $labels[] = ($stat[$param_name_id] == '') ? t('Not set') : $stat[$param_name_id];
  }
print_r($node_stats);
  $chart = array(
    '#chart_id' => 'node_views_'.$param_name_id,
    '#title' => t($params_callback[$params_callback['current_callback']]['name']),
    '#type' => CHART_TYPE_PIE,
    '#size' => chart_size($width, $height),
    '#data' => $node_stats,
    '#labels' => $labels,
  );

  $rendered_chart = chart_render($chart);

  print $rendered_chart ? $rendered_chart : t('Chart error.');
}

?>
