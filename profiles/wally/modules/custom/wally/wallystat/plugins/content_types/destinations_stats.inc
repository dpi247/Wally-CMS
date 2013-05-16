<?php 

/**
 * Callback function to supply a graph of published node,article,gallery,poll.
 */
function wallystat_destinations_stats_ctools_content_types() {
  return array(
    'single' => TRUE,
    'title' => t('Destination\'s statistics'),
    'category' => t('Dashboard'),
    'description' => t('Display graphs of a destination\'s statistics.'),
    'defaults' => _wallystat_destinations_stats_get_defaults(),
  );
}

/**
 * Implementation of content_type_form()
 */
function wallystat_destinations_stats_content_type_edit_form(&$form, $form_state) {
  $conf = $form_state['conf'];

  $form['display_ratio'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display views in the destination vs all other destinations'),
    '#default_value' => $conf['display_ratio'],
  );

  $form['display_views'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display views in the destination vs all destinations over time'),
    '#default_value' => $conf['display_views'],
  );
}

/**
 * Implementation of content_type_form_submit()
 */
function wallystat_destinations_stats_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (array_keys(_wallystat_destinations_stats_get_defaults()) as $key) {
    $form_state['conf'][key] = $form_state['values'][key];
  }
}

/**
 * Implementation of content_type_render()
 */
function wallystat_destinations_stats_content_type_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $block->title = 'Destination\'s statistics';
  $block->content = '';

  if ($conf['display_ratio']) {
    $dest_count = (int)wallystat_node_get_count_by_params('weekcount', array('param01 = '.$panel_args[0]));
    $all_dest_count = (int)wallystat_node_get_count_by_params('weekcount', array(1));
    $dest_ratio = (int)(100 * $dest_count/$all_dest_count);
    $data = array(
      array(t('Views'), t('This destination'), t('All other destinations')),
      array(t('Percentage'), $dest_ratio, 100 - $dest_ratio),
    );
    $block->content .= theme('wallytoolbox_gauge_chart', $data, t('Views in this destination vs all other destinations'), '', '');
  }

  if ($conf['display_views']) {
    $data = array();

    $time = time();
    $hours = date('G', $time);
    $limit_tstamp_inf = $time - ($hours * 60 * 60);
    $limit_tstamp_sup = $time + (60 * 60);
    $nbr_range = 48;
    $axeXTitle = t('Hours of the day');

    $data[0][0] = 'Hours';
    for($i = 1; $i <= $nbr_range; $i++) {
      $data[$i][] = $i - 48;
      $data[$i][] = 0;
      $data[$i][] = 0;
    }

    $data[0][] = t('This destination');
    $data[0][] = t('All destinations');

    $dest_stats = wallystat_destination_get_hourly_counts(array('param01 = '.$panel_args[0]));
    foreach ($dest_stats as $timestamp => $count) {
      $hour = date('G', $time - $timestamp) - 1;
      $data[48 - $hour][1] = (int)$count;
    }
    $all_stats = wallystat_destination_get_hourly_counts(array(1));
    foreach ($all_stats as $timestamp => $count) {
      $hour = date('G', $time - $timestamp) - 1;
      $data[48 - $hour][2] = (int)$count;
    }

    $block->content .= theme('wallytoolbox_line_chart', $data, t('This destination\'s views vs all destinations\' views'), 3, $axeXTitle);
  }

  return $block;
}

function _wallystat_destinations_stats_get_defaults() {
  return array(
    'display_ratio' => 1,
    'display_views' => 1,
  );
}
