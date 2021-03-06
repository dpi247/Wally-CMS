<?php

module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.flowtoarray');

/**
* Plugins are described by creating a $plugin array which will be used
* by the system that includes this file.
*/
$plugin = array(
  'title' => t('Destinations'),
  'description' => t('Get view results from a term ID and transform them.'),
  'types' => array('Destination'),
  'defaults' => wallytoolbox_destinations_flow_settings_default(),
);

function wallytoolbox_destinations_flow_settings_form(&$form, &$form_state) {
  $conf = wallytoolbox_destinations_flow_settings_default($form_state);
  
  $vocabulary_id = variable_get('wallymport_destinationpath', 0);
  $hierarchies = wallyctools_get_hierarchies();
  
  drupal_add_js(array('shortcckdestchoices' => $hierarchies), 'setting');
  drupal_add_js(drupal_get_path('module', 'wallytoolbox').'/scripts/shortdest.js');  

  $form['destination'] = array(
    '#type' => 'fieldset',
    '#title' => t('Destination to extract'),
    '#attributes' => array('id' => 'field-destinations-items'),
  );

  $tid_options = array('Not Set');
  foreach ($hierarchies as $id => $val) $tid_options[$id] = $val['hierarchic_name'];
  $form['destination']['tid'] = array(
    '#title'            => t('Destination Path'),
    '#type'             => 'select',
    '#default_value'    => $conf['destination']['tid'],
    '#options'          => $tid_options,
    '#required'         => FALSE,
    '#attributes'       => array('class' => 'tid'),
    '#vid'              => $vocabulary_id,
  );

  $target_options = array('none' => 'none');
  $array_of_redacblock_panes = wallyctools_get_redacblock_panes();
  foreach ($array_of_redacblock_panes as $pane) $target_options[$pane->name] = $pane->name;
  $form['destination']['target'] = array(
    '#title'            => t('Target'),
    '#type'             => 'select',
    '#default_value'    => $conf['destination']['target'],
    '#options'          => $target_options,
    '#required'         => FALSE,
    '#attributes'       => array('class' => 'target'),
  );
  
  /*$form['sorting'] = array(
    '#type'             => 'fieldset',
    '#title'            => t('Sort results'),
    '#collapsible'      => TRUE,
    '#collapsed'        => FALSE,
  );
  
  $sort_crit = array(
    'field_destinations_time_rank' => t('Rank'),
    //'creationdate' => t('Creation date'),
    'field_publicationdate_value' => t('Publication date'),
    'title' => t('Node title'),
  );
  $form['sorting']['sort_crit'] = array(
    '#type'             => 'select',
    '#title'            => t('Sort criteria'),
    '#options'          => $sort_crit,
    '#default_value'    => $conf['sorting']['sort_crit'],
  );
  
  $form['sorting']['sort_order'] = array(
    '#type'             => 'radios',
    '#title'            => t('Sort order'),
    '#options'          => array('DESC' => t('Desc'), 'ASC' => t('Asc')),
    '#default_value'    => $conf['sorting']['sort_order'],
  );*/
  
  $form['maxitem'] = array (
    '#type'             => 'textfield',
    '#title'            => t('Max item to fetch for this feed'),
    '#default_value'    => $conf['maxitem'],
  );
  
  $form['render_node'] = array (
    '#type'             => 'radios',
    '#default_value'    => $conf['render_node'],
    '#title'            => t('Output node structure.'),
    '#options'          => array('wally' => t('Wally XML'), 'node' => t('Drupal node')),
  );
}

function wallytoolbox_destinations_flow_settings_default($form_state = array()) {
  if (isset($form_state) && !empty($form_state)) {
    return $form_state;
  }
  
  $defaults = array(
    'destination' => array(
      'tid' => '',
      'target' => '',
    ),
    /*'sorting' => array(
      'sort_crit' => 'time_rank',
      'sort_order' => 'DESC',
    ),*/
    'maxitem' => '',
    'render_node' => 'wally',
  );
  
  return $defaults;
}

function wallytoolbox_destinations_flow_build($feed_infos) {
  $conf = $feed_infos['conf'];
  $array_packages = wallytoolbox_flow_to_array_init_array($feed_infos);

  $view = wallytoolbox_destinations_flow_get_view_results($feed_infos);
  $view_results = $view->result;

  $transformed_nodes = wallytoolbox_flow_to_array_init_array();
  if (is_array($view_results)) {
    foreach ($view_results as $node_result) {
      $node = node_load($node_result->nid);
      $node->view = $view;
      wallycontenttypes_packagepopulate($node, '');
      if ($conf['render_node'] == 'wally') {
        $transformed_nodes['Packages']['Package'][] = wallytoolbox_destinations_flow_transform_node($node, $feed_infos);
      } else {
        $package = wallytoolbox_destinations_flow_set_package($node, $feed_infos, TRUE);
        $package['node'] = $node;
        $transformed_nodes['Packages']['Package'][] = $package;
      }
    }
  }

  return $transformed_nodes;
}

function wallytoolbox_destinations_flow_get_view_results($feed_infos) {
  $conf = $feed_infos['conf'];
  $tid = $conf['destination']['tid'];
  $target = $conf['destination']['target'];

  $redacblock = db_fetch_array(db_query('SELECT * FROM {wallyctools} WHERE name = "%s"', $target));
  while (is_string($redacblock['settings']))
    $redacblock['settings'] = unserialize($redacblock['settings']);
  
  $view_name = $redacblock['view_name'];
  $view_id = $redacblock['view_id'];
  $redacblock_name = $redacblock['name'];
  $view = views_get_view($view_name);
  $view->set_display($view_id);

  /*$sort_criteria = _wallytoolbox_destinations_flow_get_sorting_criteria($conf['sorting']);
  $view->display[$view->current_display]->handler->set_option('sorts', array(
    $conf['sorting']['sort_criteria'] => $sort_criteria,
  ));*/

  $options = array(
    'operator' => '=',
    'value' => array($redacblock_name),
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
  if ($feed_infos['conf']['maxitem'] && $feed_infos['conf']['maxitem'] > 0)
    $view->set_items_per_page($feed_infos['conf']['maxitem']);
  
  $view->execute();
  if (empty($view->result) && !$view->display_handler->get_option('empty') && empty($view->style_plugin->definition['even empty'])) {
    return array();
  }
  
  //DEmander à Olivier pourquoi view destroy
  //$view->destroy();
  
  return $view;
}

function wallytoolbox_destinations_flow_transform_node($node, $feed_infos = array()) {
  $transformed_node = wallytoolbox_destinations_flow_set_package($node, $feed_infos);

  // Package MainStory/MainObject
  switch ($node->type) {
    case 'wally_articlepackage':
      $main_story = wallytoolbox_destinations_flow_set_textobject($node->field_mainstory_nodes[0], $transformed_node);
      $main_story['type'] = $node->field_mainstory_nodes[0]->type;
      $transformed_node['MainStory'] = $main_story;
      break;
    case 'wally_gallerypackage':
      $expl_type = explode($embeded->type, '_');
      $short_type = $expl_type[1];
      $set_function = 'wallytoolbox_destinations_flow_set_'.$short_type;
      if (function_exists($set_function)) {
        $embed_object = $set_function($node->field_mainobject[0], $transformed_node);
        $embed_object['type'] = $embeded->type;
        $transformed_node['MainObject'] = $embed_object;
      }
  }
  
  // Package EmbededObjects
  if (!empty($node->field_embededobjects_nodes)) {
    $transformed_node['EmbeddedContent']['EmbeddedObjects']['Object'] = array();
    foreach ($node->field_embededobjects_nodes as $embeded) {
      $expl_type = explode('_', $embeded->type);
      $short_type = $expl_type[1];
      $set_function = 'wallytoolbox_destinations_flow_set_'.$short_type;
      if (function_exists($set_function)) {
        $embed_object = $set_function($embeded, $transformed_node);
        $embed_object['type'] = $embeded->type;
        $transformed_node['EmbeddedContent']['EmbeddedObjects']['Object'][] = $embed_object;
      }
    }
  }
  
  // Package LinkedObjects
  if (!empty($node->field_linkedobjects_nodes)) {
    $transformed_node['EmbeddedContent']['LinksLists']['LinksList'] = array();
    foreach ($node->field_linkedobjects_nodes as $linked) {
      $linked_object = wallytoolbox_destinations_flow_set_linklist($linked);
      $linked_object['type'] = $linked->type;
      $transformed_node['EmbeddedContent']['LinksLists']['LinksList'][] = $linked_object;
    }
  }

  return $transformed_node;
}

function wallytoolbox_destinations_flow_set_package($node, $feed_infos = array(), $meta_pack = FALSE) {
  if (!empty($feed_infos)) {
    $conf = $feed_infos['conf'];
    $tid = $conf['destination']['tid'];
  }
  
  $transformed_node = array();
  
  $transformed_node['type'] = $node->type;
  $transformed_node['nid'] = $node->nid;

  // Package Attributes
  $transformed_node['PackageID'] = wallytoolbox_destinations_flow_node_get_field($node, 'field_packageid', '');
  $transformed_node['PublicationDate'] = wallytoolbox_destinations_flow_node_get_field($node, 'field_publicationdate', '');
  $transformed_node['CreationDate'] = date('Y-m-d', $node->created).'T'.date('H:i:s', $node->created);
  $transformed_node['LastUpdateDate'] = date('Y-m-d', $node->changed).'T'.date('H:i:s', $node->changed);
  $transformed_node['EmbargoDate'] = wallytoolbox_destinations_flow_node_get_field($node, 'field_embargodatetime', '');
  $transformed_node['UnPublishDate'] = wallytoolbox_destinations_flow_node_get_field($node, 'field_unpublishdate', '');
  $transformed_node['Language'] = $node->language;
  $transformed_node['Version'] = $node->revision_uid;
  
  // Package Elements
  $transformed_node['SourceXML']['value'] = wallytoolbox_destinations_flow_node_get_field($node, 'field_sourcexml', '');
  $transformed_node['PackageTitle']['value'] = $node->title;
  $transformed_node['DisplayedTitle']['value'] = $node->field_displayed_title[0]['value'];
  $transformed_node['PackageDescription']['value'] = wallytoolbox_destinations_flow_node_get_field($node, 'field_objectdescription', '');
  $transformed_node['InternalReference']['value'] = 'Drupal_NID:'.$node->nid;
  $transformed_node['ExternalReference']['value'] = wallytoolbox_destinations_flow_node_get_field($node, 'field_externalreference', 'wally_'.$node->nid);
  if (isset($node->field_productid) & $node->field_productid[0]['value'] != NULL & $node->field_productid[0]['value'] != '' ){
    $transformed_node['ProductID']['value'] = $node->field_productid[0]['value'];
  } else{
    $transformed_node['ProductID']['value'] = variable_get('wallyadmin_product', $_SERVER['HTTP_HOST']);
  }
  if (!$meta_pack) {
    $transformed_node['Destinations']['Destination'] = array();
    foreach ($node->field_destinations as $dest) {
      $temp_dest = array();
      $term = taxonomy_get_term($dest['tid']);
      $temp_dest['DestinationRank'] = $dest['rank'];
      $temp_dest['Position'] = $dest['target'];
      $temp_dest['DocumentLayout'] = $dest['layout'];
      $temp_dest['DestinationPath']['value'] = is_object($term) ? wallytoolbox_taxonomy_get_path_by_tid_or_term($term->tid) : '';
      if (isset($tid) && $dest['tid'] == $tid && isset($dest['time_rank']))
        $transformed_node['PublicationDate'] = date('Y-m-d', $dest['time_rank']).'T'.date('H:i:s', $dest['time_rank']);
      $transformed_node['Destinations']['Destination'][] = $temp_dest;
    }
    /*$transformed_node['Editions']['Edition'] = array();
    foreach ($node->field_editions as $edition) {
      $temp_edition = array();
      $temp_edition['EditionID'] = $edition['value'];
      $temp_edition['value'] = TRUE;
      $transformed_node['Editions']['Edition'][] = $temp_edition;
    }*/
    if (isset($node->field_channels[0]['value'])) {
      $transformed_node['Channels']['Channel'] = array();
      $channels = unserialize($node->field_channels[0]['value']);
      foreach ($channels as $channel) {
        $transformed_node['Channels']['Channel'][]['Name'] = $channel;
      }
    }
    $transformed_node['CommentsAllowed']['value'] = $node->comment;
    $freeaccess = wallytoolbox_destinations_flow_node_get_field($node, 'field_freeaccess', '');
    if (!is_bool($freeaccess)) {
      $freeaccess = ($freeaccess == 'No') ? FALSE : TRUE;
    }
    $transformed_node['FreeAccess']['value'] = $freeaccess;
    /*$transformed_node['Authors']['Person'] = array();
    foreach ($node->field_authors_nodes as $author) {
      $transformed_node['Authors']['Person'][] = wallytoolbox_destinations_flow_set_persontax($author);
    }*/
    $transformed_node['PackageLayout']['value'] = wallytoolbox_destinations_flow_node_get_field($node, 'field_packagelayout', '');
    $transformed_node['Summary']['value'] = wallytoolbox_destinations_flow_node_get_field($node, 'field_summary', '');
  }
  $transformed_node['ExternalURI']['value'] = wallytoolbox_destinations_flow_node_get_field($node, 'field_externaluri', '');
  $transformed_node['Source']['value'] = wallytoolbox_destinations_flow_node_get_field($node, 'field_source', '');
  
  return $transformed_node;
}

function wallytoolbox_destinations_flow_set_object($object, $transformed_node) {
  $temp_object = array();
  
  $temp_object['type'] = $object->type;
  $temp_object['nid'] = $object->nid;

  // Object Attributes
  $temp_object['ObjectID'] = wallytoolbox_destinations_flow_node_get_field($object, 'field_objectid', '');
  $temp_object['ObjectDescription'] = wallytoolbox_destinations_flow_node_get_field($object, 'field_objectdescription', '');
  $temp_object['PublicationDate'] = $transformed_node['PublicationDate'];
  $temp_object['CreationDate'] = $transformed_node['CreationDate'];
  $temp_object['LastUpdateDate'] = $transformed_node['LastUpdateDate'];
  
  // Object Elements
  $temp_object['Title']['value'] = $object->title;
  $temp_object['InternalReference']['value'] = 'Drupal_NID:'.$object->nid;
  $temp_object['ExternalReference']['value'] =  wallytoolbox_destinations_flow_node_get_field($object, 'field_externalreference', 'wally_'.$object->nid);
  $temp_object['Taxonomy'] = wallytoolbox_destinations_flow_set_taxonomy($object, $transformed_node);
  $temp_object['Copyright']['value'] = wallytoolbox_destinations_flow_node_get_field($object, 'field_copyright', '');
  $temp_object['ByLine']['value'] = wallytoolbox_destinations_flow_node_get_field($object, 'field_byline', '');
  $temp_object['EmbeddedObjectsOrder']['value'] = sizeof($transformed_node['EmbeddedContent']['EmbeddedObjects']['Object']);
  $temp_object['Marker'] = wallytoolbox_destinations_flow_set_markertax($object->field_markers[0]);

  return $temp_object;
}

function wallytoolbox_destinations_flow_set_multimediaobject($multimediaobject, $transformed_node) {
  $temp_multimedia = wallytoolbox_destinations_flow_set_object($multimediaobject, $transformed_node);
  
  $type = explode('_', $multimediaobject->type);
  $type = explode('object', $type[1]);
  $mediatype = $type[0];
  
  // MultiMediaObject Attributes
  $temp_multimedia['MimeType'] = wallytoolbox_destinations_flow_node_get_field($multimediaobject, 'field_'.$mediatype.'file', $mediatype, 'filemime');
  
  // MultiMediaObject Elements
  $temp_multimedia['Caption']['value'] = wallytoolbox_destinations_flow_node_get_field($multimediaobject, 'field_summary', '');
  $url_3rdparty = wallytoolbox_destinations_flow_node_get_field($multimediaobject, 'field_'.$mediatype.'3rdparty', '', 'embed');
  if (empty($url_3rdparty)) {
    $file_uri = 'embed://'.wallytoolbox_destinations_flow_node_get_field($multimediaobject, 'field_'.$mediatype.'file', '', 'filename');
  } else {
    $file_uri = $url_3rdparty;
  }
  $temp_multimedia['FileURI']['value'] = $file_uri;
  $temp_multimedia['FileSize']['value'] = wallytoolbox_destinations_flow_node_get_field($multimediaobject, 'field_'.$mediatype.'file', '', 'filesize');
  $temp_multimedia['Thumbnail']['value'] = $multimediaobject->field_thumbnail[0];
  $temp_multimedia['Rating']['value'] = wallytoolbox_destinations_flow_node_get_field($multimediaobject, 'field_rating', '');
  if (isset($multimediaobject->{'field_'.$mediatype.'file'}[0]) && !empty($multimediaobject->{'field_'.$mediatype.'file'}[0])) {
    $temp_multimedia[ucfirst($mediatype).'File'] = $multimediaobject->{'field_'.$mediatype.'file'}[0];
  } elseif (isset($multimediaobject->{'field_'.$mediatype.'3rdparty'}[0]) && !empty($multimediaobject->{'field_'.$mediatype.'3rdparty'}[0])) {
    $temp_multimedia[ucfirst($mediatype).'3rdParty'] = $multimediaobject->{'field_'.$mediatype.'3rdparty'}[0];
  }
  
  return $temp_multimedia;
}

function wallytoolbox_destinations_flow_set_textobject($textobject, $transformed_node) {
  $temp_text = wallytoolbox_destinations_flow_set_object($textobject, $transformed_node);

  // TextObject Elements
  $temp_text['TextBarette']['value'] = wallytoolbox_destinations_flow_node_get_field($textobject, 'field_textbarette', '');
  $temp_text['TextForeTitle']['value'] = wallytoolbox_destinations_flow_node_get_field($textobject, 'field_textforetitle', '');
  $temp_text['TextTitle']['value'] = $textobject->title;
  $temp_text['TextSubTitle']['value'] = wallytoolbox_destinations_flow_node_get_field($textobject, 'field_textsubtitle', '');
  $temp_text['TextChapo']['value'] = wallytoolbox_destinations_flow_node_get_field($textobject, 'field_textchapo', '');
  $temp_text['TextBody']['value'] = wallytoolbox_destinations_flow_node_get_field($textobject, 'field_textbody', '');
  $temp_text['ExtractMedium']['value'] = wallytoolbox_destinations_flow_node_get_field($textobject, 'field_extractmedium', '');
  $temp_text['ExtractShort']['value'] = wallytoolbox_destinations_flow_node_get_field($textobject, 'field_extractshort', '');
  
  return $temp_text;
}

function wallytoolbox_destinations_flow_set_photoobject($photoobject, $transformed_node) {
  $temp_photo = wallytoolbox_destinations_flow_set_multimediaobject($photoobject, $transformed_node);

  // PhotoObject Elements
  if (isset($photoobject->field_croping[0]['value']) && is_array($photoobject->field_croping[0]['value'])) {
    $temp_photo['Crops']['Crop'] = array();
    while (!empty($photoobject->field_croping[0]['value']) && is_string($photoobject->field_croping[0]['value']))
      $photoobject->field_croping[0]['value'] = json_decode($photoobject->field_croping[0]['value']);
    foreach ($photoobject->field_croping[0]['value'] as $croping_name => $croping) {
      $temp_crop = array();
      $temp_crop['Type'] = $croping_name;
      $temp_crop['X'] = $croping->{0};
      $temp_crop['Y'] = $croping->{1};
      $temp_crop['W'] = $croping->{2};
      $temp_crop['H'] = $croping->{3};
      $temp_photo['Crops']['Crop'][] = $temp_crop;
    }
  }
  $temp_photo['LinkTitle']['value'] = wallytoolbox_destinations_flow_node_get_field($photoobject, 'field_link', '', 'title');
  $temp_photo['LinkURI']['value'] = wallytoolbox_destinations_flow_node_get_field($photoobject, 'field_link', '', 'url');
  $temp_photo['Width']['value'] = wallytoolbox_destinations_flow_node_get_field($photoobject, 'field_photofile', '', 'filesize');
  $temp_photo['Height']['value'] = wallytoolbox_destinations_flow_node_get_field($photoobject, 'field_photofile', '', 'url');
  
  return $temp_photo;
}

function wallytoolbox_destinations_flow_set_videoobject($videoobject, $transformed_node) {
  $temp_video = wallytoolbox_destinations_flow_set_multimediaobject($videoobject, $transformed_node);
  
  // VideoObject Elements
  //$temp_video['VideoLenght']['value'] = ???
  $temp_video['LinkTitle']['value'] = wallytoolbox_destinations_flow_node_get_field($videoobject, 'field_link', '', 'title');
  $temp_video['LinkURI']['value'] = wallytoolbox_destinations_flow_node_get_field($videoobject, 'field_link', '', 'url');
  
  return $temp_video;
}

function wallytoolbox_destinations_flow_set_audioobject($audioobject, $transformed_node) {
  $temp_audio = wallytoolbox_destinations_flow_set_multimediaobject($audioobject, $transformed_node);
  
  // AudioObject Elements
  //$temp_audio['VideoLenght']['value'] = ???
  $temp_audio['LinkTitle']['value'] = wallytoolbox_destinations_flow_node_get_field($audioobject, 'field_link', '', 'title');
  $temp_audio['LinkURI']['value'] = wallytoolbox_destinations_flow_node_get_field($audioobject, 'field_link', '', 'url');
  
  return $temp_video;
}

function wallytoolbox_destinations_flow_set_taxonomy($object, $transformed_node) {
  $temp_object = array();
  
  $temp_object['Locations']['Location'] = array();
  if ($object->field_locations && is_array($object->field_locations) && !empty($object->field_locations)) {
    foreach ($object->field_locations as $location) {
      if ($location['value']) {
        $temp_object['Locations']['Location'][$location['value']] = $location['value'];
        unset($object->taxonomy[$location['value']]);
      }
    }
  }
  if ($object->field_locations_nodes && is_array($object->field_locations_nodes) && !empty($object->field_locations_nodes)) {
    foreach ($object->field_locations_nodes as $location) {
      $tid = $location->field_locationtaxonomy[0]['value'];
      if (in_array($tid, $temp_object['Locations']['Location']))
        $temp_object['Locations']['Location'][$tid] = wallytoolbox_destinations_flow_set_locationtax($location, $tid);
      else
        $temp_object['Locations']['Location'][] = wallytoolbox_destinations_flow_set_locationtax($location, $tid);
    }
  }
  $temp_object['Entities']['Entity'] = array();
  if ($object->field_entities && is_array($object->field_entities) && !empty($object->field_entities)) {
    foreach ($object->field_entities as $entity) {
      if ($entity['value']) {
        $temp_object['Entities']['Entity'][$entity['value']] = $entity['value'];
        unset($object->taxonomy[$entity['value']]);
      }
    }
  }
  if ($object->field_entities_nodes && is_array($object->field_entities_nodes) && !empty($object->field_entities_nodes)) {
    foreach ($object->field_entities_nodes as $entity) {
      $tid = $entity->field_entity[0]['value'];
      if (in_array($tid, $temp_object['Entities']['Entity']))
        $temp_object['Entities']['Entity'][$tid] = wallytoolbox_destinations_flow_set_entitytax($entity, $tid);
      else
        $temp_object['Entities']['Entity'][] = wallytoolbox_destinations_flow_set_entitytax($entity, $tid);
    }
  }
  $temp_object['Persons']['Person'] = array();
  if ($object->field_persons && is_array($object->field_persons) && !empty($object->field_persons)) {
    foreach ($object->field_persons as $person) {
      if ($person['value']) {
        $temp_object['Persons']['Person'][$person['value']] = $person['value'];
        unset($object->taxonomy[$person['value']]);
      }
    }
  }
  if ($object->field_persons_nodes && is_array($object->field_persons_nodes) && !empty($object->field_persons_nodes)) {
    foreach ($object->field_persons_nodes as $person) {
      $tid = $person->field_persontaxonomy[0]['value'];
      if (in_array($tid, $temp_object['Persons']['Person']))
        $temp_object['Persons']['Person'][$tid] = wallytoolbox_destinations_flow_set_persontax($person, $tid);
      else
        $temp_object['Persons']['Person'][] = wallytoolbox_destinations_flow_set_persontax($person, $tid);
    }
  }
  $temp_object['Authors']['Person'] = array();
  if ($object->field_authors && is_array($object->field_authors) && !empty($object->field_authors)) {
    foreach ($object->field_authors as $author) {
      if ($author['value']) {
        $temp_object['Authors']['Person'][$author['value']] = $author['value'];
        unset($object->taxonomy[$author['value']]);
      }
    }
  }
  if ($object->field_authors_nodes && is_array($object->field_authors_nodes) && !empty($object->field_authors_nodes)) {
    foreach ($object->field_authors_nodes as $author) {
      $tid = $author->field_authortaxonomy[0]['value'];
      if (in_array($tid, $temp_object['Authors']['Person']))
        $temp_object['Authors']['Person'][$tid] = wallytoolbox_destinations_flow_set_persontax($author, $tid);
      else
        $temp_object['Authors']['Person'][] = wallytoolbox_destinations_flow_set_persontax($author, $tid);
    }
  }
  $temp_object['Taxonomy']['Vocabulary'] = array();
  if ($object->taxonomy && is_array($object->taxonomy)) {
    foreach ($object->taxonomy as $taxonomy) {
      $temp_object['Taxonomy']['Vocabulary'][] = wallytoolbox_destinations_flow_set_taxonomytax($taxonomy);
    }
  }

  $temp_taxonomy = wallytoolbox_flow_to_array_struct_term($temp_object);

  return $temp_taxonomy;
}

function wallytoolbox_destinations_flow_set_persontax($person, $tid) {
  $temp_person = array();

  $temp_person['PersonID'] = wallytoolbox_destinations_flow_node_get_field($person, 'field_persontaxonomy', '');
  $temp_person['PersonName']['value'] = $person->taxonomy[$tid]->name;
  $temp_person['PersonLastName']['value'] = wallytoolbox_destinations_flow_node_get_field($person, 'field_personlastname', '');
  $temp_person['PersonFirstName']['value'] = wallytoolbox_destinations_flow_node_get_field($person, 'field_personfirstname', '');
  $temp_person['PersonNickName']['value'] = wallytoolbox_destinations_flow_node_get_field($person, 'field_personnickname', '');
  $temp_person['PersonBirthDate']['value'] = wallytoolbox_destinations_flow_node_get_field($person, 'field_personbirthdate', '');
  $temp_person['PersonSex']['value'] = wallytoolbox_destinations_flow_node_get_field($person, 'field_personsex', '');
  $temp_person['PersonEmail']['value'] = wallytoolbox_destinations_flow_node_get_field($person, 'field_personemail', '', 'email');
  $temp_person['PersonWebSite']['value'] = wallytoolbox_destinations_flow_node_get_field($person, 'field_personwebsite', '', 'url');
  $temp_person['PersonPhoneNumber']['value'] = wallytoolbox_destinations_flow_node_get_field($person, 'field_phonenumber', '');
  
  return $temp_person;
}

function wallytoolbox_destinations_flow_set_locationtax($location, $tid) {
  $temp_location = array();

  $temp_location['LocationID'] = wallytoolbox_destinations_flow_node_get_field($location, 'field_locationtaxonomy', '');
  $temp_location['LocationName']['value'] = $location->taxonomy[$tid]->name;
  $temp_location['Address'] = array();
  $temp_location['Address']['AddressLine']['value'] = wallytoolbox_destinations_flow_node_get_field($location, 'field_address', '', 'street').' '.wallytoolbox_destinations_flow_node_get_field($location, 'field_adress', '', 'additional');
  $temp_location['Address']['City']['value'] = wallytoolbox_destinations_flow_node_get_field($location, 'field_address', '', 'city');
  $temp_location['Address']['Zip']['value'] = wallytoolbox_destinations_flow_node_get_field($location, 'field_address', '', 'postal_code');
  $temp_location['Address']['Country']['value'] = wallytoolbox_destinations_flow_node_get_field($location, 'field_address', '', 'country');
  $temp_location['LocationGeo']['value'] = wallytoolbox_destinations_flow_node_get_field($location, 'field_address', '', 'latitude').','.wallytoolbox_destinations_flow_node_get_field($location, 'field_address', '', 'longitude');
  if ($temp_location['LocationGeo']['value'] == ',') {
    unset($temp_location['LocationGeo']['value']);
  }

  return $temp_location;
}

function wallytoolbox_destinations_flow_set_markertax($marker) {
  $temp_location = array();

  $temp_location['ID'] = $marker['lid'];
  $temp_location['Name'] = $marker['name'];
  $temp_location['Address'] = array();
  $temp_location['Address']['AddressLine']['value'] = $marker['street'].' '.$marker['additional'];
  $temp_location['Address']['City']['value'] = $marker['city'];
  $temp_location['Address']['Zip']['value'] = $marker['postal_code'];
  $temp_location['Address']['Country']['value'] = $marker['country'];
  if (isset($marker['latitude']) && !empty($marker['latitude']) && isset($marker['longitude']) && !empty($marker['longitude'])) {
    $temp_location['LocationGeo']['value'] = $marker['latitude'].','.$marker['longitude'];
  }

  return $temp_location;
}

function wallytoolbox_destinations_flow_set_entitytax($entity, $tid) {
  $temp_entity = array();

  $temp_entity['EntityID'] = wallytoolbox_destinations_flow_node_get_field($entity, 'field_entity', '');
  $temp_entity['EntityTitle']['value'] = $entity->taxonomy[$tid]->name;
  $temp_entity['EntityDescription']['value'] = wallytoolbox_destinations_flow_node_get_field($entity, 'field_objectdescription', '');
  $temp_entity['EntityServices']['value'] = wallytoolbox_destinations_flow_node_get_field($entity, 'field_entityservices', '');
  $temp_entity['Locations']['value'] = wallytoolbox_destinations_flow_node_get_field($entity, 'field_locations', '');
  
  return $temp_entity;
}

function wallytoolbox_destinations_flow_set_linktax($link) {
  $temp_link = array();

  $temp_link['InternalReference']['value'] = 'Drupal_NID:'.$link->nid;
  $temp_link['ExternalReference']['value'] = wallytoolbox_destinations_flow_node_get_field($link, 'field_externalreference', 'wally_'.$link->nid);
  $temp_link['Title']['value'] = wallytoolbox_destinations_flow_node_get_field($link, 'field_link_item', '', 'title');
  $temp_link['Description']['value'] = wallytoolbox_destinations_flow_node_get_field($link, 'field_objectdescription', '');
  $temp_link['URI']['value'] = wallytoolbox_destinations_flow_node_get_field($link, 'field_link_item', '', 'url');
  
  return $temp_link;
}

function wallytoolbox_destinations_flow_set_taxonomytax($taxonomy) {
  $temp_taxonomy_term = array();

  $temp_taxonomy_term['VocabularyName'] = $taxonomy->name;
  $temp_taxonomy_term['VocabularyDescription'] = $taxonomy->description;
  $temp_taxonomy_term['VocabularyType'] = $taxonomy->vid;
  $temp_taxonomy_term['Term']['value'] = $taxonomy->tid;

  return $temp_taxonomy_term;
}

function wallytoolbox_destinations_flow_set_linklist($linked) {
  $temp_linklist = array();
  
  $temp_object['type'] = $linked->type;
  $temp_object['nid'] = $linked->nid;
  
  // LinkList Elements
  $temp_linklist['InternalReference']['value'] = 'Drupal_NID:'.$linked->nid;
  $temp_linklist['ExternalReference']['value'] = wallytoolbox_destinations_flow_node_get_field($linked, 'field_externalreference', 'wally_'.$linked->nid);
  $temp_linklist['Title']['value'] = $linked->title;
  $temp_linklist['Summary']['value'] = wallytoolbox_destinations_flow_node_get_field($linked, 'field_summary', '');
  $temp_linklist['Links']['Link'] = array();
  foreach ($linked->field_links_list_nodes as $link) {
    $temp_linklist['Links']['Link'][] = wallytoolbox_destinations_flow_set_linktax($link);
  }
  
  return $temp_linklist;
}

function wallytoolbox_destinations_flow_node_get_field($node, $field_name, $default = '', $subfield = 'value') {
  return isset($node->{$field_name}[0][$subfield]) ? $node->{$field_name}[0][$subfield] : $default;
}

/*function _wallytoolbox_destinations_flow_get_sorting_criteria($sorting) {
  $sort_criterias = array(
    'field_destinations_time_rank' => array(
      'id' => 'field_destinations_time_rank',
      'table' => 'node_data_field_destinations',
      'field' => 'field_destinations_time_rank',
    ),
    'field_publicationdate_value' => array(
      'id' => 'field_publicationdate_value',
      'table' => 'node_data_field_publicationdate',
      'field' => 'field_publicationdate_value',
    ),
    'title' => array(
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
    ),
  );
  
  if (isset($sort_criterias[$sorting['sort_crit']])) {
    $return = $sort_criterias[$sorting['sort_crit']];
    $return['order'] = $sorting['sort_order'];
    $return['relationship'] = 'none';
  } else {
    $return = array();
  }
  
  return $return;
}*/
