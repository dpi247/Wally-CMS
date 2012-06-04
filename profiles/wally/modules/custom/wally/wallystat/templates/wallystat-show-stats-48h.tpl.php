<?php

if ($nid) {
  $temp_node_hours_stats = array();
  $node_hours_stats_db = db_query('SELECT timestamp, SUM(count) FROM {wallystat_node_hourly_counter} WHERE nid = "%s" GROUP BY timestamp ORDER BY timestamp ASC', $nid);
  while ($stat = db_fetch_array($node_hours_stats_db)) {
    $temp_node_hours_stats[$stat['timestamp']] = $stat['SUM(count)'];
  }
  
  $current_tstamp = time() - date('i')*60 - date('s');
  $last_tstamp = $current_tstamp - 48*60*60;
  
  $node_hours_stats = array();
  for ($tstamp = $last_tstamp; $tstamp < $current_tstamp; $tstamp += 60*60) {
    $node_hours_stats[$tstamp] = isset($temp_node_hours_stats[$tstamp]) ? $temp_node_hours_stats[$tstamp] : '0';
  }
  
  $chart = array(
    '#chart_id' => 'node_views_48h',
    '#title' => t('Views for 48h'),
    '#type' => CHART_TYPE_LINE,
    '#size' => chart_size($width, $height),
    '#data' => array('node_views' => $node_hours_stats),
    '#adjust_resolution' => TRUE,
    '#mixed_axis_labels' => array(
      CHART_AXIS_Y_LEFT => array(
        array(chart_mixed_axis_range_label(0, max($node_hours_stats))),
        array(chart_mixed_axis_label(t('Views'), 90)),
      ),
      CHART_AXIS_X_BOTTOM => array(
        array(chart_mixed_axis_range_label(-48, 0)),
      ),
    ),
    '#grid_lines' => chart_grid_lines(10, 10, 1, 5),
  );
  
  if ($display_title == FALSE) {
    unset($chart['#title']);
    $chart['#mixed_axis_labels'][CHART_AXIS_Y_LEFT][0] = array();
    $chart['#mixed_axis_labels'][CHART_AXIS_X_BOTTOM][0] = array();
  }

  $rendered_chart = chart_render($chart);
  
  print $rendered_chart ? $rendered_chart : t('Chart error.');
}

?>
