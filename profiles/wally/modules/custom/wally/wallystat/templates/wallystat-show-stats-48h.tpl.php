<?php

$href_value = $_GET['hrefvalue'];
$node_path = drupal_get_normal_path(trim($href_value, '/'));
$expl_path = explode('/', $node_path);

if ($expl_path[0] == 'node' && is_numeric($expl_path[1])) {
  $nid = $expl_path[1];
  $temp_node_hours_stats = array();
  $node_hours_stats_db = db_query('SELECT timestamp, SUM(count) FROM {wallystat_node_hourly_counter} WHERE nid = "%s" GROUP BY timestamp ORDER BY timestamp ASC', $nid);
  while ($stat = db_fetch_array($node_hours_stats_db))
    $temp_node_hours_stats[$stat['timestamp']] = $stat['SUM(count)'];
  
  $current_tstamp = time() - date('i')*60 - date('s');
  $last_tstamp = $current_tstamp - 48*60*60;
  
  $node_hours_stats = array();
  for ($tstamp = $last_tstamp; $tstamp < $current_tstamp; $tstamp += 60*60)
    $node_hours_stats[$tstamp] = isset($temp_node_hours_stats[$tstamp]) ? $temp_node_hours_stats[$tstamp] : '0';
  
  $chart = array(
    '#chart_id' => 'node_views_48h',
    '#title' => t('Node views in the last 48h'),
    '#type' => CHART_TYPE_LINE,
    '#size' => chart_size(240, 150),
    '#data' => array('node_views' => $node_hours_stats),
    '#adjust_resolution' => TRUE,
    '#mixed_axis_labels' => array(
      CHART_AXIS_Y_LEFT => array(
        array(
          chart_mixed_axis_range_label(0, max($node_hours_stats)),
        ),
        array(
          chart_mixed_axis_label(t('Views'), 90),
        ),
      ),
    ),
    '#grid_lines' => chart_grid_lines(10, 10, 1, 5),
  );
  
  $rendered_chart = chart_render($chart);
  
  print $rendered_chart ? $rendered_chart : t('Chart error.');
}

?>
