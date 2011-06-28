<?php

/**
* Plugins are described by creating a $plugin array which will be used
* by the system that includes this file.
*/
$plugin = array(
  'title' => t('Destinations'),
  'description' => t('Get view results from a term ID and transform them.'),
  'types' => array('View results')
);


function wallytoolbox_destinations_flow_settings_form(&$form, &$form_state) {
  module_load_include("inc", 'content', 'includes/content.node_form');
  $wallyrsstonode_destinations = content_fields('field_destinations');
  $form['destination'] = (array) content_field_form($form, $form_state, $wallyrsstonode_destinations);
  unset($form['destination']['#weight']);
  unset($form['destination']['field_destinations']['#theme']);
  unset($form['destination']['field_destinations']['#weight']);
  unset($form['destination']['field_destinations']['field_destinations_add_more']);
  unset($form['destination']['field_destinations'][0]['_weight']);
  unset($form['destination']['field_destinations'][0]['#columns'][2]);
  unset($form['destination']['field_destinations'][0]['#columns'][3]);
  unset($form['destination']['field_destinations'][0]['#columns'][4]);
  unset($form['destination']['field_destinations'][0]['#default_value']['layout']);
  unset($form['destination']['field_destinations'][0]['#default_value']['rank']);
  unset($form['destination']['field_destinations'][0]['#default_value']['time_rank']);
  unset($form['destination']['field_destinations'][1]);
  $form['destination']['field_destinations']['#type'] = 'fieldset';
}

function wallytoolbox_destinations_flow_build($feed_infos) {
  $array_packages = wallytoolbox_flow_to_array_init_array();

  $view = wallytoolbox_destinations_flow_get_view_results($feed_infos);
  $view_results = $view->result;
  
  $tranformed_nodes = wallytoolbox_flow_to_array_init_array();
  foreach ($view_results as $node_result) {
    $node = node_load($node_result->nid);
    $node->view = $view;
    wallycontenttypes_packagepopulate($node, '');
    $tranformed_nodes['Packages']['Package'][] = wallytoolbox_destinations_flow_transform_node($node, $feed_infos);
  }
  return $transformed_nodes;
}

function wallytoolbox_destinations_flow_get_view_results($feed_infos) {
  $tid = $feed_infos['conf']['destination']['field_destinations'][0]['tid'];
  $target = $feed_infos['conf']['destination']['field_destinations'][0]['target'];
  
  $redacblock = db_fetch_array(db_query('SELECT * FROM {wallyctools} WHERE name = "%s"', $target));
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

  $ret_view = $view;
  $view->destroy();
  
  return $ret_view;
}

function wallytoolbox_destinations_flow_transform_node($node, $feed_infos) {
  wallytoolbox_destinations_flow_set_package($transformed_node, $node, $feed_infos);

  switch ($node->type) {
    case 'wally_articlepackage':
      $transformed_node['MainStory'] = wallytoolbox_destinations_flow_set_textobject($node->field_mainstory_nodes[0]);
      break;
    case 'wally_gallerypackage':
      $expl_type = explode($embeded->type, '_');
      $short_type = $expl_type[1];
      $set_function = 'wallytoolbox_destinations_flow_set_'.$short_type;
      $transformed_node['MainObject'] = $set_function($node->field_mainobject[0]);
  }
  
  if (!empty($node->field_embededobjects_nodes)) {
    $transformed_node['EmbeddedContent']['EmbeddedObjects']['Object'] = array();
    foreach ($node->field_embededobjects_nodes as $embeded) {
      $expl_type = explode('_', $embeded->type);
      $short_type = $expl_type[1];
      $set_function = 'wallytoolbox_destinations_flow_set_'.$short_type;
      $transformed_node['EmbeddedContent']['EmbeddedObjects']['Object'][] = $set_function($embeded);
    }
  }

  return $transformed_node;
}

function wallytoolbox_destinations_flow_set_package(&$transformed_node, $node, $feed_infos) {
  $transformed_node = wallytoolbox_flow_to_array_new_package();
  $transformed_node['PackageID'] = wallytoolbox_destinations_flow_node_get_field($node, 'field_packageid', '');
  $transformed_node['PublicationDate'] = wallytoolbox_destinations_flow_node_get_field($node, 'field_publicationdate', '');
  $transformed_node['PackageTitle']['value'] = $node->title;
  $transformed_node['ExternalReference']['value'] = wallytoolbox_destinations_flow_node_get_field($node, 'field_externalreference', '');
  $transformed_node['ProductID']['value'] = variable_get('wallymport_productID', $_SERVER['HTTP_HOST']);
  $transformed_node['Destinations']['Destination'] = array();
  foreach ($node->field_destinations as $dest) {
    $temp_dest = array();
    $term = taxonomy_get_term($dest['tid']);
    $temp_dest['DestinationRank'] = $dest['rank'];
    $temp_dest['Position'] = $dest['target'];
    $temp_dest['DocumentLayout'] = $dest['layout'];
    $temp_dest['DestinationPath']['value'] = is_object($term) ? wallytoolbox_taxonomy_get_path_by_tid_or_term($term->tid) : '';
    $transformed_node['Destinations']['Destination'][] = $temp_dest;
  }
  $transformed_node['ExternalURI']['value'] = wallytoolbox_destinations_flow_node_get_field($node, 'field_externaluri', '');
}

function wallytoolbox_destinations_flow_set_textobject($textobejct) {
  $transformed_text = wallytoolbox_flow_to_array_new_textobject();
  $transformed_text['ObjectID'] = wallytoolbox_destinations_flow_node_get_field($textobejct, 'field_objectid', '');
  $transformed_text['Title']['value'] = $textobejct->title;
  $transformed_text['ExternalReference']['value'] =  wallytoolbox_destinations_flow_node_get_field($textobejct, 'field_externalreference', '');
  $transformed_text['Copyright']['value'] = wallytoolbox_destinations_flow_node_get_field($textobejct, 'field_copyright', '');
  $transformed_text['TextChapo']['value'] = wallytoolbox_destinations_flow_node_get_field($textobejct, 'field_textchapo', '');
  $transformed_text['TextBody']['value'] = wallytoolbox_destinations_flow_node_get_field($textobejct, 'field_textbody', '');
  
  return $transformed_text;
}

function wallytoolbox_destinations_flow_set_photoobject($photoobject) {
  $transformed_photo = wallytoolbox_flow_to_array_new_photoobject();
  $transformed_photo['ObjectID'] = wallytoolbox_destinations_flow_node_get_field($photoobject, 'field_objectid', '');
  $transformed_photo['MimeType'] = $photoobject->field_photofile[0]['filemime'];
  $transformed_photo['Title']['value'] = $photoobject->title;
  $transformed_photo['ExternalReference']['value'] = wallytoolbox_destinations_flow_node_get_field($photoobject, 'field_externalreference', '');
  $transformed_photo['Copyright']['value'] = wallytoolbox_destinations_flow_node_get_field($photoobject, 'field_copyright', '');
  $transformed_photo['FileSize']['value'] = $photoobject->field_photofile[0]['filesize'];
  $transformed_photo['Rating']['value'] = wallytoolbox_destinations_flow_node_get_field($photoobject, 'field_rating', '');
  
  return $transformed_photo;
}

function wallytoolbox_destinations_flow_set_videoobject($videoobject) {
  $transformed_video = wallytoolbox_flow_to_array_new_videoobject();
  $transformed_video['ObjectID'] = wallytoolbox_destinations_flow_node_get_field($videoobject, 'field_objectid', '');
  $transformed_video['MimeType'] = 'video';
  $transformed_video['Title']['value'] = $videoobject->title;
  $transformed_video['ExternalReference']['value'] = wallytoolbox_destinations_flow_node_get_field($videoobject, 'fied_externalreference', '');
  $transformed_video['Copyright']['value'] = wallytoolbox_destinations_flow_node_get_field($videoobject, 'field_copyright', '');
  $transformed_photo['Rating']['value'] = wallytoolbox_destinations_flow_node_get_field($videoobject, 'field_rating', '');
  
  return $transformed_video;
}

function wallytoolbox_destinations_flow_node_get_field($node, $field_name, $default = '') {
  return isset($node->{$field_name}[0]['value']) ? $node->{$field_name}[0]['value'] : $default;
}
