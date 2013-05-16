<?php 

/**
 * Callback function to supply a graph of published node,article,gallery,poll.
 */
function wallystat_viewers_stats_ctools_content_types() {
  return array(
    'single' => TRUE,
    'title' => t('Viewers\' statistics'),
    'category' => t('Dashboard'),
    'description' => t('Display graphs of a node or a destination viewers\' statistics.'),
    'defaults' => _wallystat_viewers_stats_get_defaults(),
  );
}

/**
 * Implementation of content_type_form()
 */
function wallystat_viewers_stats_content_type_edit_form(&$form, $form_state) {
  $conf = $form_state['conf'];

  $form['display_authenticated'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display authenticated vs anonymous'),
    '#default_value' => $conf['display_authenticated'],
  );

  $form['display_roles'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display authenticated users with special roles vs no special roles'),
    '#default_value' => $conf['display_roles'],
  );
}

/**
 * Implementation of content_type_form_submit()
 */
function wallystat_viewers_stats_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (array_keys(_wallystat_viewers_stats_get_defaults()) as $key) {
    $form_state['conf'][key] = $form_state['values'][key];
  }
}

/**
 * Implementation of content_type_render()
 */
function wallystat_viewers_stats_content_type_render($subtype, $conf, $panel_args, $context) {
  $block = new stdClass();
  $block->title = 'Viewers\' statistics';
  $block->content = '';

  if ($conf['display_authenticated']) {
    $data = array();
    if (arg(0) == 'node') {
      $data = array(
        array(t('Authenticated'), (int)wallystat_node_get_count_by_params('weekcount', array('nid = '.$panel_args[0], 'param02 = 1'))),
        array(t('Anonymous'), (int)wallystat_node_get_count_by_params('weekcount', array('nid = '.$panel_args[0], 'param02 = 0'))),
      );
    } else {
      $data = array(
        array(t('Authenticated'), (int)wallystat_node_get_count_by_params('weekcount', array('param01 = '.$panel_args[0], 'param02 = 1'))),
        array(t('Anonymous'), (int)wallystat_node_get_count_by_params('weekcount', array('param01 = '.$panel_args[0], 'param02 = 0'))),
      );
    }
    if (!empty($data)) {
      $block->content .= theme('wallytoolbox_pie_chart', $data, t('Authenticated vs Anonymous'), 'authenticated vs anonymous', 'views count');
    }
  }

  if ($conf['display_roles']) {
    $data = array();
    if (arg(0) == 'node') {
      $data = array(
        array(t('User type'), t('Users count')),
        array(t('Subscribed user'), (int)wallystat_node_get_count_by_params('weekcount', array('nid = '.$panel_args[0], 'param02 = 1', 'param03 > 1'))),
        array(t('Not subscribed user'), (int)wallystat_node_get_count_by_params('weekcount', array('nid = '.$panel_args[0], 'param02 = 1', 'param03 = 1'))),
      );
    } else {
      $data = array(
        array(t('User type'), t('Users count')),
        array(t('Subscribed user'), (int)wallystat_node_get_count_by_params('weekcount', array('param01 = '.$panel_args[0], 'param02 = 1', 'param03 > 1'))),
        array(t('Not subscribed user'), (int)wallystat_node_get_count_by_params('weekcount', array('param01 = '.$panel_args[0], 'param02 = 1', 'param03 = 1'))),
      );
    }
    if (!empty($data)) {
      $block->content .= theme('wallytoolbox_bar_chart', $data, t('Users with special(s) role(s) vs Users without any special role'), '', '');
    }
  }

  return $block;
}

function _wallystat_viewers_stats_get_defaults() {
  return array(
    'display_authenticated' => 1,
    'display_roles' => 1,
  );
}
