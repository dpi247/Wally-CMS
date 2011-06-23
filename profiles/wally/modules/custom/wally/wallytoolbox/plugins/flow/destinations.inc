<?php

function wallytoolbox_destinations_flow_build($feed_infos) {
  $array_packages = wallytoolbox_flow_to_array_init_array();

  $view_results = wallytoolbox_destinations_flow_get_view_results($feed_infos);
  
  $tranformed_nodes = array();
  foreach ($view_results as $node_result) {
    $node = node_load($node_result->nid);
    $tranformed_nodes[] = wallytoolbox_destinations_flow_transform_node($node, $feed_infos);
  }
  
  return $transformed_nodes;
}

function wallytoolbox_destinations_flow_get_view_results($feed_infos) {
  $tid = $feed_infos['tid'];
  $target = $feed_infos['target'];
  
  $redacblock = db_fetch_array(db_query('SELECT * FROM wallyctools WHERE name = "%s"', $target));
  while (is_string($redacblock['settings']))
    $redacblock['settings'] = unserialize($redacblock['settings']);
  
  $view_name = $redacblock['view_name'];
  $view_id = $redacblock['view_id'];
  $redacblock_name = $redacblock['name'];

  $view = views_get_view($view_name);
  $view->set_display($view_id);
  
  $options = array(
    'operator' => '=',
    'value' => $redacblock_name,
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'relationship' => 'none',
  );
  
  $view->add_item($view_id, 'filter', 'node_data_field_destinations', 'field_destinations_target', $options);
  $view->is_cacheable = FALSE;
  //init handler should be placed after add_item !!!
  $view->init_handlers();
  
  if (empty($view) || !is_object($view) || empty($view->display_handler)) {
    return array();
  }

  if (!$view->display_handler->access($GLOBALS['user'])) {
    return array();
  }

  $view->set_arguments(array($tid));
  $view->execute();

  if (empty($view->result) && !$view->display_handler->get_option('empty') && empty($view->style_plugin->definition['even empty'])) {
    return array();;
  }

  $view_results = $view->result;
  $view->destroy();
  
  return $view_results;
}

function wallytoolbox_destinations_flow_transform_node($node, $feed_infos) {
  return array();
}
