<?php

/**
 * Implementation of hook_views_default_views().
 */
function wallydashboard_views_default_views() {
  $views = array();

  // Exported view: admin_content_wally
  $view = new view;
$view->name = 'admin_content_wally';
$view->description = 'Specific view to administer wally content';
$view->tag = '';
$view->base_table = 'node';
$view->human_name = '';
$view->core = 6;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'administer dashboard';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['pager']['options']['items_per_page'] = 30;
$handler->display->display_options['style_plugin'] = 'bulk';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'timestamp' => 'title',
  'type' => 'type',
  'status' => 'status',
  'field_publicationdate_value' => 'field_publicationdate_value',
  'field_editorialupdatedate_value' => 'field_editorialupdatedate_value',
  'field_destinations_aggregation' => 'field_destinations_aggregation',
  'name' => 'name',
  'edit_node' => 'edit_node',
  'delete_node' => 'edit_node',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'sortable' => 1,
    'separator' => '&nbsp;',
  ),
  'timestamp' => array(
    'separator' => '',
  ),
  'type' => array(
    'sortable' => 1,
    'separator' => '',
  ),
  'status' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'field_publicationdate_value' => array(
    'sortable' => 1,
    'separator' => '',
  ),
  'field_editorialupdatedate_value' => array(
    'sortable' => 1,
    'separator' => '',
  ),
  'field_destinations_aggregation' => array(
    'separator' => '',
  ),
  'name' => array(
    'sortable' => 1,
    'separator' => '',
  ),
  'edit_node' => array(
    'separator' => '&nbsp;',
  ),
  'delete_node' => array(
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['style_options']['operations'] = array(
  'cckdestinations_batch_dest_add' => array(
    'selected' => TRUE,
    'skip_confirmation' => 0,
  ),
  'wallycontenttypes_batch_addfreetags_nodes' => array(
    'selected' => TRUE,
    'skip_confirmation' => 0,
  ),
  'views_bulk_operations_delete_node_action' => array(
    'selected' => TRUE,
    'skip_confirmation' => 0,
  ),
  'wallycontenttypes_publish_nodes' => array(
    'selected' => TRUE,
    'skip_confirmation' => 0,
  ),
  'cckdestinations_batch_dest_remove_no_context' => array(
    'selected' => TRUE,
    'skip_confirmation' => 0,
  ),
  'wallycontenttypes_unpublish_nodes' => array(
    'selected' => TRUE,
    'skip_confirmation' => 0,
  ),
  'pathauto_node_update_alias_multiple-620e193b20ba9caa374fea9ca0ad38f0' => array(
    'selected' => TRUE,
    'skip_confirmation' => 0,
  ),
);
$handler->display->display_options['style_options']['execution_type'] = '1';
$handler->display->display_options['style_options']['display_type'] = '0';
$handler->display->display_options['style_options']['display_result'] = 1;
$handler->display->display_options['style_options']['merge_single_action'] = 1;
/* Field: Node: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['hide_empty'] = 0;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['link_to_node'] = 1;
/* Field: Node: Has new content */
$handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['table'] = 'history_user';
$handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
$handler->display->display_options['fields']['timestamp']['label'] = 'New?';
$handler->display->display_options['fields']['timestamp']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['timestamp']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['timestamp']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['trim'] = 0;
$handler->display->display_options['fields']['timestamp']['alter']['html'] = 0;
$handler->display->display_options['fields']['timestamp']['hide_empty'] = 0;
$handler->display->display_options['fields']['timestamp']['empty_zero'] = 0;
$handler->display->display_options['fields']['timestamp']['link_to_node'] = 0;
$handler->display->display_options['fields']['timestamp']['comments'] = 0;
/* Field: Node: Type */
$handler->display->display_options['fields']['type']['id'] = 'type';
$handler->display->display_options['fields']['type']['table'] = 'node';
$handler->display->display_options['fields']['type']['field'] = 'type';
$handler->display->display_options['fields']['type']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['type']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['type']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['type']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['type']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['type']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['type']['alter']['trim'] = 0;
$handler->display->display_options['fields']['type']['alter']['html'] = 0;
$handler->display->display_options['fields']['type']['hide_empty'] = 0;
$handler->display->display_options['fields']['type']['empty_zero'] = 0;
$handler->display->display_options['fields']['type']['link_to_node'] = 0;
$handler->display->display_options['fields']['type']['machine_name'] = 0;
/* Field: Node: Published */
$handler->display->display_options['fields']['status']['id'] = 'status';
$handler->display->display_options['fields']['status']['table'] = 'node';
$handler->display->display_options['fields']['status']['field'] = 'status';
$handler->display->display_options['fields']['status']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['status']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['status']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['status']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['status']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['status']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['status']['alter']['trim'] = 0;
$handler->display->display_options['fields']['status']['alter']['html'] = 0;
$handler->display->display_options['fields']['status']['hide_empty'] = 0;
$handler->display->display_options['fields']['status']['empty_zero'] = 0;
$handler->display->display_options['fields']['status']['not'] = 0;
/* Field: Content: Publication Date (field_publicationdate) */
$handler->display->display_options['fields']['field_publicationdate_value']['id'] = 'field_publicationdate_value';
$handler->display->display_options['fields']['field_publicationdate_value']['table'] = 'node_data_field_publicationdate';
$handler->display->display_options['fields']['field_publicationdate_value']['field'] = 'field_publicationdate_value';
$handler->display->display_options['fields']['field_publicationdate_value']['label'] = 'Publication Date';
$handler->display->display_options['fields']['field_publicationdate_value']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['field_publicationdate_value']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['field_publicationdate_value']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['field_publicationdate_value']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['field_publicationdate_value']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['field_publicationdate_value']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['field_publicationdate_value']['alter']['trim'] = 0;
$handler->display->display_options['fields']['field_publicationdate_value']['alter']['html'] = 0;
$handler->display->display_options['fields']['field_publicationdate_value']['hide_empty'] = 0;
$handler->display->display_options['fields']['field_publicationdate_value']['empty_zero'] = 0;
$handler->display->display_options['fields']['field_publicationdate_value']['link_to_node'] = 0;
/* Field: Content: Editorial Update Date (field_editorialupdatedate) */
$handler->display->display_options['fields']['field_editorialupdatedate_value']['id'] = 'field_editorialupdatedate_value';
$handler->display->display_options['fields']['field_editorialupdatedate_value']['table'] = 'node_data_field_editorialupdatedate';
$handler->display->display_options['fields']['field_editorialupdatedate_value']['field'] = 'field_editorialupdatedate_value';
$handler->display->display_options['fields']['field_editorialupdatedate_value']['label'] = 'Editorial Update Date';
$handler->display->display_options['fields']['field_editorialupdatedate_value']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['field_editorialupdatedate_value']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['field_editorialupdatedate_value']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['field_editorialupdatedate_value']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['field_editorialupdatedate_value']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['field_editorialupdatedate_value']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['field_editorialupdatedate_value']['alter']['trim'] = 0;
$handler->display->display_options['fields']['field_editorialupdatedate_value']['alter']['html'] = 0;
$handler->display->display_options['fields']['field_editorialupdatedate_value']['hide_empty'] = 0;
$handler->display->display_options['fields']['field_editorialupdatedate_value']['empty_zero'] = 0;
$handler->display->display_options['fields']['field_editorialupdatedate_value']['link_to_node'] = 0;
/* Field: Destinations: aggregation: Destinations (field_destinations) */
$handler->display->display_options['fields']['field_destinations_aggregation']['id'] = 'field_destinations_aggregation';
$handler->display->display_options['fields']['field_destinations_aggregation']['table'] = 'node_data_field_destinations';
$handler->display->display_options['fields']['field_destinations_aggregation']['field'] = 'field_destinations_aggregation';
$handler->display->display_options['fields']['field_destinations_aggregation']['label'] = 'Destination';
$handler->display->display_options['fields']['field_destinations_aggregation']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['field_destinations_aggregation']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['field_destinations_aggregation']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['field_destinations_aggregation']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['field_destinations_aggregation']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['field_destinations_aggregation']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['field_destinations_aggregation']['alter']['trim'] = 0;
$handler->display->display_options['fields']['field_destinations_aggregation']['alter']['html'] = 0;
$handler->display->display_options['fields']['field_destinations_aggregation']['hide_empty'] = 0;
$handler->display->display_options['fields']['field_destinations_aggregation']['empty_zero'] = 0;
$handler->display->display_options['fields']['field_destinations_aggregation']['link_to_node'] = 0;
$handler->display->display_options['fields']['field_destinations_aggregation']['label_type'] = 'custom';
$handler->display->display_options['fields']['field_destinations_aggregation']['format'] = 'tid';
$handler->display->display_options['fields']['field_destinations_aggregation']['multiple']['group'] = 1;
$handler->display->display_options['fields']['field_destinations_aggregation']['multiple']['multiple_reversed'] = 0;
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['label'] = 'Author';
$handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['name']['alter']['trim'] = 0;
$handler->display->display_options['fields']['name']['alter']['html'] = 0;
$handler->display->display_options['fields']['name']['hide_empty'] = 0;
$handler->display->display_options['fields']['name']['empty_zero'] = 0;
$handler->display->display_options['fields']['name']['link_to_user'] = 1;
$handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
/* Field: Node: Edit link */
$handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
$handler->display->display_options['fields']['edit_node']['table'] = 'node';
$handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
$handler->display->display_options['fields']['edit_node']['label'] = 'Edit';
$handler->display->display_options['fields']['edit_node']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['edit_node']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['edit_node']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['trim'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['html'] = 0;
$handler->display->display_options['fields']['edit_node']['hide_empty'] = 0;
$handler->display->display_options['fields']['edit_node']['empty_zero'] = 0;
/* Field: Node: Delete link */
$handler->display->display_options['fields']['delete_node']['id'] = 'delete_node';
$handler->display->display_options['fields']['delete_node']['table'] = 'node';
$handler->display->display_options['fields']['delete_node']['field'] = 'delete_node';
$handler->display->display_options['fields']['delete_node']['label'] = 'Delete';
$handler->display->display_options['fields']['delete_node']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['delete_node']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['delete_node']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['delete_node']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['delete_node']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['delete_node']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['delete_node']['alter']['trim'] = 0;
$handler->display->display_options['fields']['delete_node']['alter']['html'] = 0;
$handler->display->display_options['fields']['delete_node']['hide_empty'] = 0;
$handler->display->display_options['fields']['delete_node']['empty_zero'] = 0;
/* Sort criterion: Node: Updated date */
$handler->display->display_options['sorts']['changed']['id'] = 'changed';
$handler->display->display_options['sorts']['changed']['table'] = 'node';
$handler->display->display_options['sorts']['changed']['field'] = 'changed';
$handler->display->display_options['sorts']['changed']['order'] = 'DESC';
/* Filter: Node: Title */
$handler->display->display_options['filters']['title']['id'] = 'title';
$handler->display->display_options['filters']['title']['table'] = 'node';
$handler->display->display_options['filters']['title']['field'] = 'title';
$handler->display->display_options['filters']['title']['operator'] = 'word';
$handler->display->display_options['filters']['title']['exposed'] = TRUE;
$handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
$handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
$handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
$handler->display->display_options['filters']['title']['case'] = 0;
/* Filter: Node: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'wally_articlepackage' => 'wally_articlepackage',
  'wally_gallerypackage' => 'wally_gallerypackage',
  'wally_pollpackage' => 'wally_pollpackage',
);
$handler->display->display_options['filters']['type']['exposed'] = TRUE;
$handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
$handler->display->display_options['filters']['type']['expose']['label'] = 'Type';
$handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
$handler->display->display_options['filters']['type']['expose']['reduce'] = 1;
/* Filter: Node: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = 'All';
$handler->display->display_options['filters']['status']['exposed'] = TRUE;
$handler->display->display_options['filters']['status']['expose']['label'] = 'Published';
$handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
/* Filter: Destinations: Destinations tid: Destinations */
$handler->display->display_options['filters']['field_destinations_tid']['id'] = 'field_destinations_tid';
$handler->display->display_options['filters']['field_destinations_tid']['table'] = 'node_data_field_destinations';
$handler->display->display_options['filters']['field_destinations_tid']['field'] = 'field_destinations_tid';
$handler->display->display_options['filters']['field_destinations_tid']['value'] = '';
$handler->display->display_options['filters']['field_destinations_tid']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_destinations_tid']['expose']['operator'] = 'field_destinations_tid_op';
$handler->display->display_options['filters']['field_destinations_tid']['expose']['label'] = 'Destinations: Destinations tid: Destinations';
$handler->display->display_options['filters']['field_destinations_tid']['expose']['identifier'] = 'field_destinations_tid';
$handler->display->display_options['filters']['field_destinations_tid']['vid'] = '2';
/* Filter: Destinations: Destinations target: Destinations */
$handler->display->display_options['filters']['field_destinations_target']['id'] = 'field_destinations_target';
$handler->display->display_options['filters']['field_destinations_target']['table'] = 'node_data_field_destinations';
$handler->display->display_options['filters']['field_destinations_target']['field'] = 'field_destinations_target';
$handler->display->display_options['filters']['field_destinations_target']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_destinations_target']['expose']['operator'] = 'field_destinations_target_op';
$handler->display->display_options['filters']['field_destinations_target']['expose']['label'] = 'Target';
$handler->display->display_options['filters']['field_destinations_target']['expose']['identifier'] = 'field_destinations_target';
$handler->display->display_options['filters']['field_destinations_target']['expose']['reduce'] = 0;
/* Filter: Content: Free Tags (field_free_tags) */
$handler->display->display_options['filters']['field_free_tags_value']['id'] = 'field_free_tags_value';
$handler->display->display_options['filters']['field_free_tags_value']['table'] = 'node_data_field_free_tags';
$handler->display->display_options['filters']['field_free_tags_value']['field'] = 'field_free_tags_value';
$handler->display->display_options['filters']['field_free_tags_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_free_tags_value']['expose']['operator'] = 'field_free_tags_value_op';
$handler->display->display_options['filters']['field_free_tags_value']['expose']['label'] = 'Free Tags';
$handler->display->display_options['filters']['field_free_tags_value']['expose']['identifier'] = 'field_free_tags_value';
$handler->display->display_options['filters']['field_free_tags_value']['expose']['reduce'] = 0;
$handler->display->display_options['filters']['field_free_tags_value']['reduce_duplicates'] = 0;
/* Filter: Node: Type */
$handler->display->display_options['filters']['type_1']['id'] = 'type_1';
$handler->display->display_options['filters']['type_1']['table'] = 'node';
$handler->display->display_options['filters']['type_1']['field'] = 'type';
$handler->display->display_options['filters']['type_1']['value'] = array(
  'wally_articlepackage' => 'wally_articlepackage',
  'wally_gallerypackage' => 'wally_gallerypackage',
  'wally_pollpackage' => 'wally_pollpackage',
);
$handler->display->display_options['filters']['type_1']['expose']['operator'] = FALSE;

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page_1');
$handler->display->display_options['path'] = 'admin/content/packages';
$handler->display->display_options['menu']['type'] = 'normal';
$handler->display->display_options['menu']['title'] = 'Packages';
$handler->display->display_options['menu']['description'] = 'View and manage packages';
$handler->display->display_options['menu']['weight'] = '0';
$handler->display->display_options['menu']['name'] = 'admin';
$translatables['admin_content_wally'] = array(
  t('Defaults'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Items per page'),
  t('- All -'),
  t('Offset'),
  t('Title'),
  t('New?'),
  t('Type'),
  t('Published'),
  t('Publication Date'),
  t('Editorial Update Date'),
  t('Destination'),
  t('Author'),
  t('Edit'),
  t('Delete'),
  t('Destinations: Destinations tid: Destinations'),
  t('Target'),
  t('Free Tags'),
  t('Page'),
);
  

  $views[$view->name] = $view;


  //dashboard_last_created_article
$view = new view;
$view->name = 'dashboard_last_created_article';
$view->description = '';
$view->tag = '';
$view->base_table = 'node';
$view->human_name = '';
$view->core = 6;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'administer dashboard';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'created' => 'created',
  'title' => 'title',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'created' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'title' => array(
    'sortable' => 0,
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
/* Field: Node: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['hide_empty'] = 0;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['link_to_node'] = 1;
/* Field: Node: Post date */
$handler->display->display_options['fields']['created']['id'] = 'created';
$handler->display->display_options['fields']['created']['table'] = 'node';
$handler->display->display_options['fields']['created']['field'] = 'created';
$handler->display->display_options['fields']['created']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['created']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['created']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['created']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['created']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['created']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['created']['alter']['trim'] = 0;
$handler->display->display_options['fields']['created']['alter']['html'] = 0;
$handler->display->display_options['fields']['created']['hide_empty'] = 0;
$handler->display->display_options['fields']['created']['empty_zero'] = 0;
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['name']['alter']['trim'] = 0;
$handler->display->display_options['fields']['name']['alter']['html'] = 0;
$handler->display->display_options['fields']['name']['hide_empty'] = 0;
$handler->display->display_options['fields']['name']['empty_zero'] = 0;
$handler->display->display_options['fields']['name']['link_to_user'] = 1;
$handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
/* Field: Node: Edit link */
$handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
$handler->display->display_options['fields']['edit_node']['table'] = 'node';
$handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
$handler->display->display_options['fields']['edit_node']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['edit_node']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['edit_node']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['trim'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['html'] = 0;
$handler->display->display_options['fields']['edit_node']['hide_empty'] = 0;
$handler->display->display_options['fields']['edit_node']['empty_zero'] = 0;
/* Sort criterion: Node: Post date */
$handler->display->display_options['sorts']['created']['id'] = 'created';
$handler->display->display_options['sorts']['created']['table'] = 'node';
$handler->display->display_options['sorts']['created']['field'] = 'created';
$handler->display->display_options['sorts']['created']['order'] = 'DESC';
/* Argument: Destinations: Destinations tid: Destinations (field_destinations) */
$handler->display->display_options['arguments']['field_destinations_tid']['id'] = 'field_destinations_tid';
$handler->display->display_options['arguments']['field_destinations_tid']['table'] = 'node_data_field_destinations';
$handler->display->display_options['arguments']['field_destinations_tid']['field'] = 'field_destinations_tid';
$handler->display->display_options['arguments']['field_destinations_tid']['style_plugin'] = 'default_summary';
$handler->display->display_options['arguments']['field_destinations_tid']['default_argument_type'] = 'fixed';
/* Filter: Node: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'wally_articlepackage' => 'wally_articlepackage',
  'wally_gallerypackage' => 'wally_gallerypackage',
  'wally_pollpackage' => 'wally_pollpackage',
);
$handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;

/* Display: Content pane */
$handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'Latest created packages';
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Argument: Destinations: Destinations tid: Destinations (field_destinations) */
$handler->display->display_options['arguments']['field_destinations_tid']['id'] = 'field_destinations_tid';
$handler->display->display_options['arguments']['field_destinations_tid']['table'] = 'node_data_field_destinations';
$handler->display->display_options['arguments']['field_destinations_tid']['field'] = 'field_destinations_tid';
$handler->display->display_options['arguments']['field_destinations_tid']['style_plugin'] = 'default_summary';
$handler->display->display_options['arguments']['field_destinations_tid']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['field_destinations_tid']['add_table'] = 0;
$handler->display->display_options['arguments']['field_destinations_tid']['require_value'] = 0;
$handler->display->display_options['arguments']['field_destinations_tid']['reduce_duplicates'] = 0;
$handler->display->display_options['arguments']['field_destinations_tid']['set_breadcrumb'] = 0;
$handler->display->display_options['pane_title'] = 'Latest created packages';
$handler->display->display_options['pane_category']['name'] = 'Dashboard';
$handler->display->display_options['pane_category']['weight'] = '0';
$handler->display->display_options['allow']['use_pager'] = 0;
$handler->display->display_options['allow']['items_per_page'] = 'items_per_page';
$handler->display->display_options['allow']['offset'] = 0;
$handler->display->display_options['allow']['link_to_view'] = 0;
$handler->display->display_options['allow']['more_link'] = 0;
$handler->display->display_options['allow']['path_override'] = 0;
$handler->display->display_options['allow']['title_override'] = 0;
$handler->display->display_options['allow']['exposed_form'] = 0;
$handler->display->display_options['allow']['fields_override'] = 0;
$handler->display->display_options['argument_input'] = array(
  'field_destinations_tid' => array(
    'type' => 'context',
    'context' => 'term.tid',
    'context_optional' => 1,
    'panel' => '0',
    'fixed' => '',
    'label' => 'Destinations: Destinations tid: Destinations (field_destinations)',
  ),
);
$translatables['dashboard_last_created_article'] = array(
  t('Defaults'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Items per page'),
  t('- All -'),
  t('Offset'),
  t('Title'),
  t('Post date'),
  t('Name'),
  t('Edit link'),
  t('All'),
  t('Content pane'),
  t('Latest created packages'),
  t('Dashboard'),
);
  
  
  $views[$view->name] = $view;
  
  
  //my_last_created_article
  
$view = new view;
$view->name = 'dashboard_mylast_created_article';
$view->description = '';
$view->tag = '';
$view->base_table = 'node';
$view->human_name = '';
$view->core = 6;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'administer dashboard';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'created' => 'created',
  'title' => 'title',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'created' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'title' => array(
    'sortable' => 0,
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
/* Field: Node: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['hide_empty'] = 0;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['link_to_node'] = 1;
/* Field: Node: Post date */
$handler->display->display_options['fields']['created']['id'] = 'created';
$handler->display->display_options['fields']['created']['table'] = 'node';
$handler->display->display_options['fields']['created']['field'] = 'created';
$handler->display->display_options['fields']['created']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['created']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['created']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['created']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['created']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['created']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['created']['alter']['trim'] = 0;
$handler->display->display_options['fields']['created']['alter']['html'] = 0;
$handler->display->display_options['fields']['created']['hide_empty'] = 0;
$handler->display->display_options['fields']['created']['empty_zero'] = 0;
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['name']['alter']['trim'] = 0;
$handler->display->display_options['fields']['name']['alter']['html'] = 0;
$handler->display->display_options['fields']['name']['hide_empty'] = 0;
$handler->display->display_options['fields']['name']['empty_zero'] = 0;
$handler->display->display_options['fields']['name']['link_to_user'] = 1;
$handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
/* Field: Node: Edit link */
$handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
$handler->display->display_options['fields']['edit_node']['table'] = 'node';
$handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
$handler->display->display_options['fields']['edit_node']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['edit_node']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['edit_node']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['trim'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['html'] = 0;
$handler->display->display_options['fields']['edit_node']['hide_empty'] = 0;
$handler->display->display_options['fields']['edit_node']['empty_zero'] = 0;
/* Sort criterion: Node: Post date */
$handler->display->display_options['sorts']['created']['id'] = 'created';
$handler->display->display_options['sorts']['created']['table'] = 'node';
$handler->display->display_options['sorts']['created']['field'] = 'created';
$handler->display->display_options['sorts']['created']['order'] = 'DESC';
/* Argument: User: Uid */
$handler->display->display_options['arguments']['uid']['id'] = 'uid';
$handler->display->display_options['arguments']['uid']['table'] = 'users';
$handler->display->display_options['arguments']['uid']['field'] = 'uid';
$handler->display->display_options['arguments']['uid']['default_action'] = 'empty';
$handler->display->display_options['arguments']['uid']['style_plugin'] = 'default_summary';
$handler->display->display_options['arguments']['uid']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['uid']['break_phrase'] = 0;
$handler->display->display_options['arguments']['uid']['not'] = 0;
/* Argument: Destinations: Destinations tid: Destinations (field_destinations) */
$handler->display->display_options['arguments']['field_destinations_tid']['id'] = 'field_destinations_tid';
$handler->display->display_options['arguments']['field_destinations_tid']['table'] = 'node_data_field_destinations';
$handler->display->display_options['arguments']['field_destinations_tid']['field'] = 'field_destinations_tid';
$handler->display->display_options['arguments']['field_destinations_tid']['style_plugin'] = 'default_summary';
$handler->display->display_options['arguments']['field_destinations_tid']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['field_destinations_tid']['add_table'] = 0;
$handler->display->display_options['arguments']['field_destinations_tid']['require_value'] = 0;
$handler->display->display_options['arguments']['field_destinations_tid']['reduce_duplicates'] = 0;
$handler->display->display_options['arguments']['field_destinations_tid']['set_breadcrumb'] = 0;
/* Filter: Node: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'wally_articlepackage' => 'wally_articlepackage',
  'wally_gallerypackage' => 'wally_gallerypackage',
  'wally_pollpackage' => 'wally_pollpackage',
);
$handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;

/* Display: Content pane */
$handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'My last created packages';
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Argument: User: Uid */
$handler->display->display_options['arguments']['uid']['id'] = 'uid';
$handler->display->display_options['arguments']['uid']['table'] = 'users';
$handler->display->display_options['arguments']['uid']['field'] = 'uid';
$handler->display->display_options['arguments']['uid']['default_action'] = 'empty';
$handler->display->display_options['arguments']['uid']['style_plugin'] = 'default_summary';
$handler->display->display_options['arguments']['uid']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['uid']['break_phrase'] = 0;
$handler->display->display_options['arguments']['uid']['not'] = 0;
/* Argument: Destinations: Destinations tid: Destinations (field_destinations) */
$handler->display->display_options['arguments']['field_destinations_tid']['id'] = 'field_destinations_tid';
$handler->display->display_options['arguments']['field_destinations_tid']['table'] = 'node_data_field_destinations';
$handler->display->display_options['arguments']['field_destinations_tid']['field'] = 'field_destinations_tid';
$handler->display->display_options['arguments']['field_destinations_tid']['style_plugin'] = 'default_summary';
$handler->display->display_options['arguments']['field_destinations_tid']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['field_destinations_tid']['add_table'] = 0;
$handler->display->display_options['arguments']['field_destinations_tid']['require_value'] = 0;
$handler->display->display_options['arguments']['field_destinations_tid']['reduce_duplicates'] = 0;
$handler->display->display_options['arguments']['field_destinations_tid']['set_breadcrumb'] = 0;
$handler->display->display_options['pane_title'] = 'My last created packages';
$handler->display->display_options['pane_category']['name'] = 'Dashboard';
$handler->display->display_options['pane_category']['weight'] = '0';
$handler->display->display_options['allow']['use_pager'] = 0;
$handler->display->display_options['allow']['items_per_page'] = 'items_per_page';
$handler->display->display_options['allow']['offset'] = 0;
$handler->display->display_options['allow']['link_to_view'] = 0;
$handler->display->display_options['allow']['more_link'] = 0;
$handler->display->display_options['allow']['path_override'] = 0;
$handler->display->display_options['allow']['title_override'] = 0;
$handler->display->display_options['allow']['exposed_form'] = 0;
$handler->display->display_options['allow']['fields_override'] = 0;
$handler->display->display_options['argument_input'] = array(
  'uid' => array(
    'type' => 'context',
    'context' => 'user.uid',
    'context_optional' => 0,
    'panel' => '0',
    'fixed' => '',
    'label' => 'User: Uid',
  ),
  'field_destinations_tid' => array(
    'type' => 'context',
    'context' => 'term.tid',
    'context_optional' => 1,
    'panel' => '0',
    'fixed' => '',
    'label' => 'Destinations: Destinations tid: Destinations (field_destinations)',
  ),
);
$translatables['dashboard_mylast_created_article'] = array(
  t('Defaults'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Items per page'),
  t('- All -'),
  t('Offset'),
  t('Title'),
  t('Post date'),
  t('Name'),
  t('Edit link'),
  t('All'),
  t('Content pane'),
  t('My last created packages'),
  t('Dashboard'),
);
  
  $views[$view->name] = $view;
  
  //last_register_user
  $view = new view;
  $view->name = 'dashboard_last_register_user';
  $view->description = '';
  $view->tag = '';
  $view->base_table = 'users';
  $view->human_name = '';
  $view->core = 6;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  
  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer dashboard';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'created' => 'created',
    'name' => 'name',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'created' => array(
      'sortable' => 0,
      'separator' => '',
  ),
    'name' => array(
      'sortable' => 0,
      'separator' => '',
  ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  /* Field: User: Created date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'users';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['created']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['created']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['created']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['created']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['created']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['created']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['created']['alter']['html'] = 0;
  $handler->display->display_options['fields']['created']['hide_empty'] = 0;
  $handler->display->display_options['fields']['created']['empty_zero'] = 0;
  /* Sort criterion: User: Created date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'users';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  
  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: User: Created date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'users';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  $handler->display->display_options['pane_category']['name'] = 'Dashboard';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $handler->display->display_options['allow']['use_pager'] = 0;
  $handler->display->display_options['allow']['items_per_page'] = 'items_per_page';
  $handler->display->display_options['allow']['offset'] = 0;
  $handler->display->display_options['allow']['link_to_view'] = 0;
  $handler->display->display_options['allow']['more_link'] = 0;
  $handler->display->display_options['allow']['path_override'] = 0;
  $handler->display->display_options['allow']['title_override'] = 0;
  $handler->display->display_options['allow']['exposed_form'] = 0;
  $handler->display->display_options['allow']['fields_override'] = 0;
  $translatables['dashboard_last_register_user'] = array(
  t('Defaults'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Items per page'),
  t('- All -'),
  t('Offset'),
  t('Name'),
  t('Created date'),
  t('Content pane'),
  t('Dashboard'),
  );
  
  
  $views[$view->name] = $view;
  
//last_edited_article
  $view = new view;
  $view->name = 'dashboard_last_edited_article';
  $view->description = '';
  $view->tag = '';
  $view->base_table = 'node';
  $view->human_name = '';
  $view->core = 6;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  
  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer dashboard';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'created' => 'created',
    'title' => 'title',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'created' => array(
      'sortable' => 0,
      'separator' => '',
  ),
    'title' => array(
      'sortable' => 0,
      'separator' => '',
  ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  /* Field: Node: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: Node: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['changed']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['changed']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['html'] = 0;
  $handler->display->display_options['fields']['changed']['hide_empty'] = 0;
  $handler->display->display_options['fields']['changed']['empty_zero'] = 0;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  /* Field: Node: Edit link */
  $handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['table'] = 'node';
  $handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
  $handler->display->display_options['fields']['edit_node']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['edit_node']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['edit_node']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['edit_node']['alter']['html'] = 0;
  $handler->display->display_options['fields']['edit_node']['hide_empty'] = 0;
  $handler->display->display_options['fields']['edit_node']['empty_zero'] = 0;
  /* Sort criterion: Node: Updated date */
  $handler->display->display_options['sorts']['changed']['id'] = 'changed';
  $handler->display->display_options['sorts']['changed']['table'] = 'node';
  $handler->display->display_options['sorts']['changed']['field'] = 'changed';
  $handler->display->display_options['sorts']['changed']['order'] = 'DESC';
  /* Argument: Destinations: Destinations tid: Destinations (field_destinations) */
  $handler->display->display_options['arguments']['field_destinations_tid']['id'] = 'field_destinations_tid';
  $handler->display->display_options['arguments']['field_destinations_tid']['table'] = 'node_data_field_destinations';
  $handler->display->display_options['arguments']['field_destinations_tid']['field'] = 'field_destinations_tid';
  $handler->display->display_options['arguments']['field_destinations_tid']['style_plugin'] = 'default_summary';
  $handler->display->display_options['arguments']['field_destinations_tid']['default_argument_type'] = 'fixed';
  /* Filter: Node: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'wally_articlepackage' => 'wally_articlepackage',
    'wally_gallerypackage' => 'wally_gallerypackage',
    'wally_pollpackage' => 'wally_pollpackage',
  );
  $handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
  
  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Latest edited packages';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Argument: Destinations: Destinations tid: Destinations (field_destinations) */
  $handler->display->display_options['arguments']['field_destinations_tid']['id'] = 'field_destinations_tid';
  $handler->display->display_options['arguments']['field_destinations_tid']['table'] = 'node_data_field_destinations';
  $handler->display->display_options['arguments']['field_destinations_tid']['field'] = 'field_destinations_tid';
  $handler->display->display_options['arguments']['field_destinations_tid']['style_plugin'] = 'default_summary';
  $handler->display->display_options['arguments']['field_destinations_tid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_destinations_tid']['add_table'] = 0;
  $handler->display->display_options['arguments']['field_destinations_tid']['require_value'] = 0;
  $handler->display->display_options['arguments']['field_destinations_tid']['reduce_duplicates'] = 0;
  $handler->display->display_options['arguments']['field_destinations_tid']['set_breadcrumb'] = 0;
  $handler->display->display_options['pane_title'] = 'Latest edited packages';
  $handler->display->display_options['pane_category']['name'] = 'Dashboard';
  $handler->display->display_options['pane_category']['weight'] = '0';
  $handler->display->display_options['allow']['use_pager'] = 0;
  $handler->display->display_options['allow']['items_per_page'] = 'items_per_page';
  $handler->display->display_options['allow']['offset'] = 0;
  $handler->display->display_options['allow']['link_to_view'] = 0;
  $handler->display->display_options['allow']['more_link'] = 0;
  $handler->display->display_options['allow']['path_override'] = 0;
  $handler->display->display_options['allow']['title_override'] = 0;
  $handler->display->display_options['allow']['exposed_form'] = 0;
  $handler->display->display_options['allow']['fields_override'] = 0;
  $handler->display->display_options['argument_input'] = array(
    'field_destinations_tid' => array(
      'type' => 'context',
      'context' => 'term.tid',
      'context_optional' => 1,
      'panel' => '0',
      'fixed' => '',
      'label' => 'Destinations: Destinations tid: Destinations (field_destinations)',
  ),
  );
  $translatables['dashboard_last_edited_article'] = array(
  t('Defaults'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Items per page'),
  t('- All -'),
  t('Offset'),
  t('Title'),
  t('Updated date'),
  t('Name'),
  t('Edit link'),
  t('All'),
  t('Content pane'),
  t('Latest edited packages'),
  t('Dashboard'),
  );
  
  
  
  $views[$view->name] = $view;
  //mylast edited article
  $view = new view;
$view->name = 'dashboard_mylast_edited_article';
$view->description = '';
$view->tag = '';
$view->base_table = 'node';
$view->human_name = '';
$view->core = 6;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['access']['perm'] = 'administer dashboard';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'created' => 'created',
  'title' => 'title',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'created' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'title' => array(
    'sortable' => 0,
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
/* Field: Node: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['hide_empty'] = 0;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['link_to_node'] = 1;
/* Field: Node: Updated date */
$handler->display->display_options['fields']['changed']['id'] = 'changed';
$handler->display->display_options['fields']['changed']['table'] = 'node';
$handler->display->display_options['fields']['changed']['field'] = 'changed';
$handler->display->display_options['fields']['changed']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['changed']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['changed']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['changed']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['changed']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['changed']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['changed']['alter']['trim'] = 0;
$handler->display->display_options['fields']['changed']['alter']['html'] = 0;
$handler->display->display_options['fields']['changed']['hide_empty'] = 0;
$handler->display->display_options['fields']['changed']['empty_zero'] = 0;
/* Field: User: Name */
$handler->display->display_options['fields']['name']['id'] = 'name';
$handler->display->display_options['fields']['name']['table'] = 'users';
$handler->display->display_options['fields']['name']['field'] = 'name';
$handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['name']['alter']['trim'] = 0;
$handler->display->display_options['fields']['name']['alter']['html'] = 0;
$handler->display->display_options['fields']['name']['hide_empty'] = 0;
$handler->display->display_options['fields']['name']['empty_zero'] = 0;
$handler->display->display_options['fields']['name']['link_to_user'] = 1;
$handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
/* Field: Node: Edit link */
$handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
$handler->display->display_options['fields']['edit_node']['table'] = 'node';
$handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
$handler->display->display_options['fields']['edit_node']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['edit_node']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['edit_node']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['trim'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['html'] = 0;
$handler->display->display_options['fields']['edit_node']['hide_empty'] = 0;
$handler->display->display_options['fields']['edit_node']['empty_zero'] = 0;
/* Sort criterion: Node: Updated date */
$handler->display->display_options['sorts']['changed']['id'] = 'changed';
$handler->display->display_options['sorts']['changed']['table'] = 'node';
$handler->display->display_options['sorts']['changed']['field'] = 'changed';
$handler->display->display_options['sorts']['changed']['order'] = 'DESC';
/* Argument: User: Uid */
$handler->display->display_options['arguments']['uid']['id'] = 'uid';
$handler->display->display_options['arguments']['uid']['table'] = 'users';
$handler->display->display_options['arguments']['uid']['field'] = 'uid';
$handler->display->display_options['arguments']['uid']['default_action'] = 'empty';
$handler->display->display_options['arguments']['uid']['style_plugin'] = 'default_summary';
$handler->display->display_options['arguments']['uid']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['uid']['break_phrase'] = 0;
$handler->display->display_options['arguments']['uid']['not'] = 0;
/* Argument: Destinations: Destinations tid: Destinations (field_destinations) */
$handler->display->display_options['arguments']['field_destinations_tid']['id'] = 'field_destinations_tid';
$handler->display->display_options['arguments']['field_destinations_tid']['table'] = 'node_data_field_destinations';
$handler->display->display_options['arguments']['field_destinations_tid']['field'] = 'field_destinations_tid';
$handler->display->display_options['arguments']['field_destinations_tid']['style_plugin'] = 'default_summary';
$handler->display->display_options['arguments']['field_destinations_tid']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['field_destinations_tid']['add_table'] = 0;
$handler->display->display_options['arguments']['field_destinations_tid']['require_value'] = 0;
$handler->display->display_options['arguments']['field_destinations_tid']['reduce_duplicates'] = 0;
$handler->display->display_options['arguments']['field_destinations_tid']['set_breadcrumb'] = 0;
/* Filter: Node: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'wally_articlepackage' => 'wally_articlepackage',
  'wally_gallerypackage' => 'wally_gallerypackage',
  'wally_pollpackage' => 'wally_pollpackage',
);
$handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;

/* Display: Content pane */
$handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'My last edited packages';
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Argument: User: Uid */
$handler->display->display_options['arguments']['uid']['id'] = 'uid';
$handler->display->display_options['arguments']['uid']['table'] = 'users';
$handler->display->display_options['arguments']['uid']['field'] = 'uid';
$handler->display->display_options['arguments']['uid']['default_action'] = 'empty';
$handler->display->display_options['arguments']['uid']['style_plugin'] = 'default_summary';
$handler->display->display_options['arguments']['uid']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['uid']['break_phrase'] = 0;
$handler->display->display_options['arguments']['uid']['not'] = 0;
/* Argument: Destinations: Destinations tid: Destinations (field_destinations) */
$handler->display->display_options['arguments']['field_destinations_tid']['id'] = 'field_destinations_tid';
$handler->display->display_options['arguments']['field_destinations_tid']['table'] = 'node_data_field_destinations';
$handler->display->display_options['arguments']['field_destinations_tid']['field'] = 'field_destinations_tid';
$handler->display->display_options['arguments']['field_destinations_tid']['style_plugin'] = 'default_summary';
$handler->display->display_options['arguments']['field_destinations_tid']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['field_destinations_tid']['add_table'] = 0;
$handler->display->display_options['arguments']['field_destinations_tid']['require_value'] = 0;
$handler->display->display_options['arguments']['field_destinations_tid']['reduce_duplicates'] = 0;
$handler->display->display_options['arguments']['field_destinations_tid']['set_breadcrumb'] = 0;
$handler->display->display_options['pane_title'] = 'My last edited packages';
$handler->display->display_options['pane_category']['name'] = 'Dashboard';
$handler->display->display_options['pane_category']['weight'] = '0';
$handler->display->display_options['allow']['use_pager'] = 0;
$handler->display->display_options['allow']['items_per_page'] = 'items_per_page';
$handler->display->display_options['allow']['offset'] = 0;
$handler->display->display_options['allow']['link_to_view'] = 0;
$handler->display->display_options['allow']['more_link'] = 0;
$handler->display->display_options['allow']['path_override'] = 0;
$handler->display->display_options['allow']['title_override'] = 0;
$handler->display->display_options['allow']['exposed_form'] = 0;
$handler->display->display_options['allow']['fields_override'] = 0;
$handler->display->display_options['argument_input'] = array(
  'uid' => array(
    'type' => 'context',
    'context' => 'user.uid',
    'context_optional' => 0,
    'panel' => '0',
    'fixed' => '',
    'label' => 'User: Uid',
  ),
  'field_destinations_tid' => array(
    'type' => 'context',
    'context' => 'term.tid',
    'context_optional' => 1,
    'panel' => '0',
    'fixed' => '',
    'label' => 'Destinations: Destinations tid: Destinations (field_destinations)',
  ),
);
$translatables['dashboard_mylast_edited_article'] = array(
  t('Defaults'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Items per page'),
  t('- All -'),
  t('Offset'),
  t('Title'),
  t('Updated date'),
  t('Name'),
  t('Edit link'),
  t('All'),
  t('Content pane'),
  t('My last edited packages'),
  t('Dashboard'),
);
  
  
$views[$view->name] = $view;

//Last imported package
$view = new view;
$view->name = 'dashbord_last_import_package';
$view->description = '';
$view->tag = '';
$view->base_table = 'node';
$view->human_name = '';
$view->core = 6;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'created' => 'created',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'sortable' => 0,
    'separator' => '',
  ),
  'created' => array(
    'sortable' => 0,
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
/* Field: Node: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['hide_empty'] = 0;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['link_to_node'] = 1;
/* Field: Content: Publication Date (field_publicationdate) */
$handler->display->display_options['fields']['field_publicationdate_value']['id'] = 'field_publicationdate_value';
$handler->display->display_options['fields']['field_publicationdate_value']['table'] = 'node_data_field_publicationdate';
$handler->display->display_options['fields']['field_publicationdate_value']['field'] = 'field_publicationdate_value';
$handler->display->display_options['fields']['field_publicationdate_value']['label'] = 'Publication Date';
$handler->display->display_options['fields']['field_publicationdate_value']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['field_publicationdate_value']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['field_publicationdate_value']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['field_publicationdate_value']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['field_publicationdate_value']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['field_publicationdate_value']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['field_publicationdate_value']['alter']['trim'] = 0;
$handler->display->display_options['fields']['field_publicationdate_value']['alter']['html'] = 0;
$handler->display->display_options['fields']['field_publicationdate_value']['hide_empty'] = 0;
$handler->display->display_options['fields']['field_publicationdate_value']['empty_zero'] = 0;
$handler->display->display_options['fields']['field_publicationdate_value']['link_to_node'] = 0;
/* Field: Node: Edit link */
$handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
$handler->display->display_options['fields']['edit_node']['table'] = 'node';
$handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
$handler->display->display_options['fields']['edit_node']['label'] = 'Edit';
$handler->display->display_options['fields']['edit_node']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['edit_node']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['edit_node']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['trim'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['html'] = 0;
$handler->display->display_options['fields']['edit_node']['hide_empty'] = 0;
$handler->display->display_options['fields']['edit_node']['empty_zero'] = 0;
/* Sort criterion: Node: Post date */
$handler->display->display_options['sorts']['created']['id'] = 'created';
$handler->display->display_options['sorts']['created']['table'] = 'node';
$handler->display->display_options['sorts']['created']['field'] = 'created';
$handler->display->display_options['sorts']['created']['order'] = 'DESC';
/* Filter: Content: ExternalReference (field_externalreference) */
$handler->display->display_options['filters']['field_externalreference_value']['id'] = 'field_externalreference_value';
$handler->display->display_options['filters']['field_externalreference_value']['table'] = 'node_data_field_externalreference';
$handler->display->display_options['filters']['field_externalreference_value']['field'] = 'field_externalreference_value';
$handler->display->display_options['filters']['field_externalreference_value']['operator'] = 'not empty';
$handler->display->display_options['filters']['field_externalreference_value']['expose']['operator'] = FALSE;
$handler->display->display_options['filters']['field_externalreference_value']['case'] = 1;
/* Filter: Node: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'wally_articlepackage' => 'wally_articlepackage',
  'wally_gallerypackage' => 'wally_gallerypackage',
  'wally_pollpackage' => 'wally_pollpackage',
);
$handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;

/* Display: Dashboard 1 */
$handler = $view->new_display('panel_pane', 'Dashboard 1', 'panel_pane_1');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'Last imported packages';
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Node: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['hide_empty'] = 0;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['link_to_node'] = 1;
/* Field: Content: Publication Date (field_publicationdate) */
$handler->display->display_options['fields']['field_publicationdate_value']['id'] = 'field_publicationdate_value';
$handler->display->display_options['fields']['field_publicationdate_value']['table'] = 'node_data_field_publicationdate';
$handler->display->display_options['fields']['field_publicationdate_value']['field'] = 'field_publicationdate_value';
$handler->display->display_options['fields']['field_publicationdate_value']['label'] = 'Publication Date';
$handler->display->display_options['fields']['field_publicationdate_value']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['field_publicationdate_value']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['field_publicationdate_value']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['field_publicationdate_value']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['field_publicationdate_value']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['field_publicationdate_value']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['field_publicationdate_value']['alter']['trim'] = 0;
$handler->display->display_options['fields']['field_publicationdate_value']['alter']['html'] = 0;
$handler->display->display_options['fields']['field_publicationdate_value']['hide_empty'] = 0;
$handler->display->display_options['fields']['field_publicationdate_value']['empty_zero'] = 0;
$handler->display->display_options['fields']['field_publicationdate_value']['link_to_node'] = 0;
/* Field: Node: Published */
$handler->display->display_options['fields']['status']['id'] = 'status';
$handler->display->display_options['fields']['status']['table'] = 'node';
$handler->display->display_options['fields']['status']['field'] = 'status';
$handler->display->display_options['fields']['status']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['status']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['status']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['status']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['status']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['status']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['status']['alter']['trim'] = 0;
$handler->display->display_options['fields']['status']['alter']['html'] = 0;
$handler->display->display_options['fields']['status']['hide_empty'] = 0;
$handler->display->display_options['fields']['status']['empty_zero'] = 0;
$handler->display->display_options['fields']['status']['not'] = 0;
/* Field: Node: Edit link */
$handler->display->display_options['fields']['edit_node']['id'] = 'edit_node';
$handler->display->display_options['fields']['edit_node']['table'] = 'node';
$handler->display->display_options['fields']['edit_node']['field'] = 'edit_node';
$handler->display->display_options['fields']['edit_node']['label'] = 'Edit';
$handler->display->display_options['fields']['edit_node']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['absolute'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['edit_node']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['edit_node']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['trim'] = 0;
$handler->display->display_options['fields']['edit_node']['alter']['html'] = 0;
$handler->display->display_options['fields']['edit_node']['hide_empty'] = 0;
$handler->display->display_options['fields']['edit_node']['empty_zero'] = 0;
$handler->display->display_options['pane_title'] = 'Last imported packages';
$handler->display->display_options['pane_category']['name'] = 'Dashboard';
$handler->display->display_options['pane_category']['weight'] = '0';
$handler->display->display_options['allow']['use_pager'] = 0;
$handler->display->display_options['allow']['items_per_page'] = 'items_per_page';
$handler->display->display_options['allow']['offset'] = 0;
$handler->display->display_options['allow']['link_to_view'] = 0;
$handler->display->display_options['allow']['more_link'] = 0;
$handler->display->display_options['allow']['path_override'] = 0;
$handler->display->display_options['allow']['title_override'] = 0;
$handler->display->display_options['allow']['exposed_form'] = 0;
$handler->display->display_options['allow']['fields_override'] = 0;

/* Display: Dashboard 2 */
$handler = $view->new_display('panel_pane', 'Dashboard 2', 'panel_pane_2');
$handler->display->display_options['defaults']['title'] = FALSE;
$handler->display->display_options['title'] = 'Last unpublish imported package ';
$handler->display->display_options['defaults']['filters'] = FALSE;
/* Filter: Content: ExternalReference (field_externalreference) */
$handler->display->display_options['filters']['field_externalreference_value']['id'] = 'field_externalreference_value';
$handler->display->display_options['filters']['field_externalreference_value']['table'] = 'node_data_field_externalreference';
$handler->display->display_options['filters']['field_externalreference_value']['field'] = 'field_externalreference_value';
$handler->display->display_options['filters']['field_externalreference_value']['operator'] = 'not empty';
$handler->display->display_options['filters']['field_externalreference_value']['expose']['operator'] = FALSE;
$handler->display->display_options['filters']['field_externalreference_value']['case'] = 1;
/* Filter: Node: Type */
$handler->display->display_options['filters']['type']['id'] = 'type';
$handler->display->display_options['filters']['type']['table'] = 'node';
$handler->display->display_options['filters']['type']['field'] = 'type';
$handler->display->display_options['filters']['type']['value'] = array(
  'wally_articlepackage' => 'wally_articlepackage',
  'wally_gallerypackage' => 'wally_gallerypackage',
  'wally_pollpackage' => 'wally_pollpackage',
);
$handler->display->display_options['filters']['type']['expose']['operator'] = FALSE;
/* Filter: Node: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = '0';
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
$handler->display->display_options['pane_title'] = 'Last unpublish imported package';
$handler->display->display_options['pane_category']['name'] = 'Dashboard';
$handler->display->display_options['pane_category']['weight'] = '0';
$handler->display->display_options['allow']['use_pager'] = 0;
$handler->display->display_options['allow']['items_per_page'] = 'items_per_page';
$handler->display->display_options['allow']['offset'] = 0;
$handler->display->display_options['allow']['link_to_view'] = 0;
$handler->display->display_options['allow']['more_link'] = 0;
$handler->display->display_options['allow']['path_override'] = 0;
$handler->display->display_options['allow']['title_override'] = 0;
$handler->display->display_options['allow']['exposed_form'] = 0;
$handler->display->display_options['allow']['fields_override'] = 0;
$translatables['dashbord_last_import_package'] = array(
  t('Defaults'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Items per page'),
  t('- All -'),
  t('Offset'),
  t('Title'),
  t('Publication Date'),
  t('Edit'),
  t('Dashboard 1'),
  t('Last imported packages'),
  t('Published'),
  t('Dashboard'),
  t('Dashboard 2'),
  t('Last unpublish imported package '),
  t('Last unpublish imported package'),
);


  $views[$view->name] = $view;
  
  
  return $views;
}
