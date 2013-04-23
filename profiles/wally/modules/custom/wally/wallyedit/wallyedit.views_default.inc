<?php

/**
 * Implementation of hook_views_default_views().
 */
function wallyedit_views_default_views() {
  $views = array();

  // Exported view: prenode_selectbox_vbo
  $view = new view;
$view->name = 'prenode_selectbox_vbo';
$view->description = '';
$view->tag = 'Allow to select node in the wallyedit interface';
$view->base_table = 'node';
$view->human_name = '';
$view->core = 0;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['use_ajax'] = TRUE;
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['pager']['options']['items_per_page'] = 20;
$handler->display->display_options['style_plugin'] = 'bulk';
$handler->display->display_options['style_options']['columns'] = array(
  'title' => 'title',
  'type' => 'type',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'title' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
  'type' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
  ),
);
$handler->display->display_options['style_options']['override'] = 1;
$handler->display->display_options['style_options']['sticky'] = 0;
$handler->display->display_options['style_options']['empty_table'] = 0;
$handler->display->display_options['style_options']['operations'] = array(
  'cckdestinations_batch_dest_add' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'wallycontenttypes_batch_addfreetags_nodes' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'wallyedit_node_operations_selector_action' => array(
    'selected' => 1,
    'skip_confirmation' => 1,
    'label' => '',
  ),
  'views_bulk_operations_comment_setting_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'cckdestinations_batch_dest_change' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'node_assign_owner_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'wallycontenttypes_batch_clear_packagepopulate_cache' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'views_bulk_operations_delete_node_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'node_mass_update-a27b9efabcd054685a549378b174ad11' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'wallycontenttypes_batch_disable_comments_nodes' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'system_message_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'views_bulk_operations_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'views_bulk_operations_script_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'wallycontenttypes_batch_varnish_flush_cache_nodes' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'imagecache_flush_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'imagecache_generate_all_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'imagecache_generate_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'node_make_sticky_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'node_make_unsticky_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'node_mass_update-c4d3b28efb86fd703619a50b74d43794' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'views_bulk_operations_content_taxonomy_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'views_bulk_operations_fields_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
    'settings' => array(
      'php_code' => 0,
      'show_tokens' => 0,
      'display_fields' => array(
        '_all_' => '_all_',
      ),
    ),
  ),
  'views_bulk_operations_taxonomy_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'views_bulk_operations_argument_selector_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'node_promote_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'node_mass_update-14de7d028b4bffdf2b4a266562ca18ac' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'node_mass_update-9c585624b9b3af0b4687d5f97f35e047' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'wallyflowtonode_publish_nodes' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'wallycontenttypes_publish_nodes' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'node_mass_update-190ced8b5c07ecff46a3396519b3fe64' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'node_publish_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'system_goto_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'wallyflowtonode_reject_nodes' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'emfield_operations_reload' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'cckdestinations_batch_dest_remove_no_context' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'cckdestinations_batch_dest_remove' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'node_unpromote_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'node_mass_update-8ce21b08bb8e773d10018b484fe4815e' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'node_save_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'system_send_email_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'wallycontenttypes_batch_readonly_comments_nodes' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'wallycontenttypes_batch_readwrite_comments_nodes' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'cckdestinations_batch_dest_setmain' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'wallycontenttypes_batch_set_freeaccess_nodes' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'node_mass_update-0ccad85c1ebe4c9ceada1aa64293b080' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'wallycontenttypes_unpublish_nodes' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'node_mass_update-74add31efedf27aa219a63b5f7d21e97' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'node_unpublish_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'node_unpublish_by_keyword_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'wallycontenttypes_batch_unset_freeaccess_nodes' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
  'pathauto_node_update_alias_multiple-620e193b20ba9caa374fea9ca0ad38f0' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'label' => '',
  ),
);
$handler->display->display_options['style_options']['execution_type'] = '2';
$handler->display->display_options['style_options']['display_type'] = '1';
$handler->display->display_options['style_options']['display_result'] = 0;
$handler->display->display_options['style_options']['merge_single_action'] = 0;
$handler->display->display_options['style_options']['hide_selector'] = 0;
$handler->display->display_options['style_options']['preserve_selection'] = 0;
/* Field: Node: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['hide_empty'] = 0;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['link_to_node'] = 0;
/* Field: Node: Type */
$handler->display->display_options['fields']['type']['id'] = 'type';
$handler->display->display_options['fields']['type']['table'] = 'node';
$handler->display->display_options['fields']['type']['field'] = 'type';
$handler->display->display_options['fields']['type']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['type']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['type']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['type']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['type']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['type']['alter']['trim'] = 0;
$handler->display->display_options['fields']['type']['alter']['html'] = 0;
$handler->display->display_options['fields']['type']['hide_empty'] = 0;
$handler->display->display_options['fields']['type']['empty_zero'] = 0;
$handler->display->display_options['fields']['type']['link_to_node'] = 0;
$handler->display->display_options['fields']['type']['machine_name'] = 0;
/* Sort criterion: Node: Updated date */
$handler->display->display_options['sorts']['changed']['id'] = 'changed';
$handler->display->display_options['sorts']['changed']['table'] = 'node';
$handler->display->display_options['sorts']['changed']['field'] = 'changed';
$handler->display->display_options['sorts']['changed']['order'] = 'DESC';
/* Filter: Node: Title */
$handler->display->display_options['filters']['title']['id'] = 'title';
$handler->display->display_options['filters']['title']['table'] = 'node';
$handler->display->display_options['filters']['title']['field'] = 'title';
$handler->display->display_options['filters']['title']['operator'] = 'contains';
$handler->display->display_options['filters']['title']['exposed'] = TRUE;
$handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
$handler->display->display_options['filters']['title']['expose']['label'] = 'Node: Title';
$handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
$handler->display->display_options['filters']['title']['expose']['optional'] = 1;
$handler->display->display_options['filters']['title']['case'] = 1;
$translatables['prenode_selectbox_vbo'] = array(
  t('Defaults'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Title'),
  t('Type'),
  t('Node: Title'),
);
  
  $views[$view->name] = $view;

  return $views;
}
