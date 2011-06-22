<?php

/**
 * Implementation of hook_views_default_views().
 */
function cckdestinations_views_default_views() {
  $views = array();

  // Exported view: batch_destinations
  $view = new view;
  $view->name = 'batch_destinations';
  $view->description = 'List nodes with exposed filters to change destinations in a batch process';
  $view->tag = 'Batch destination change';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => 'Title',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
    'field_publicationdate_value' => array(
      'label' => 'Publication Date',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_to' => '',
        'group' => '',
      ),
      'repeat' => array(
        'show_repeat_rule' => '',
      ),
      'fromto' => array(
        'fromto' => 'both',
      ),
      'exclude' => 0,
      'id' => 'field_publicationdate_value',
      'table' => 'node_data_field_publicationdate',
      'field' => 'field_publicationdate_value',
      'relationship' => 'none',
    ),
    'field_destinations_aggregation' => array(
      'label' => 'Destinations',
      'alter' => array(
        'alter_text' => 0,
        'text' => 'Path : [field_destinations_tid] | Target : [field_destinations_target]',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'batch',
      'multiple' => array(
        'group' => 1,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => 0,
      ),
      'exclude' => 0,
      'id' => 'field_destinations_aggregation',
      'table' => 'node_data_field_destinations',
      'field' => 'field_destinations_aggregation',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'field_publicationdate_value' => array(
      'order' => 'DESC',
      'delta' => -1,
      'id' => 'field_publicationdate_value',
      'table' => 'node_data_field_publicationdate',
      'field' => 'field_publicationdate_value',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'wally_articlepackage' => 'wally_articlepackage',
        'wally_gallerypackage' => 'wally_gallerypackage',
        'wally_pollpackage' => 'wally_pollpackage',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
    'title' => array(
      'operator' => 'contains',
      'value' => '',
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 1,
        'operator' => 'title_op',
        'identifier' => 'title',
        'label' => 'Node: Title',
        'optional' => 1,
        'remember' => 1,
      ),
      'case' => 1,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
    'name' => array(
      'operator' => '=',
      'value' => '',
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 1,
        'operator' => 'name_op',
        'identifier' => 'name',
        'label' => 'Taxonomy: Term',
        'optional' => 1,
        'remember' => 1,
      ),
      'case' => 0,
      'id' => 'name',
      'table' => 'term_data',
      'field' => 'name',
      'relationship' => 'none',
    ),
    'field_destinations_tid' => array(
      'operator' => 'or',
      'value' => array(),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 1,
        'operator' => 'field_destinations_tid_op',
        'identifier' => 'field_destinations_tid',
        'label' => 'Destinations: Destinations tid: Destinations',
        'optional' => 1,
        'single' => 1,
        'remember' => 1,
        'reduce' => 0,
      ),
      'type' => 'select',
      'limit' => TRUE,
      'vid' => '2',
      'id' => 'field_destinations_tid',
      'table' => 'node_data_field_destinations',
      'field' => 'field_destinations_tid',
      'hierarchy' => 1,
      'relationship' => 'none',
      'reduce_duplicates' => 0,
    ),
    'field_destinations_target' => array(
      'operator' => '=',
      'value' => '',
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 1,
        'operator' => 'field_destinations_target_op',
        'identifier' => 'field_destinations_target',
        'label' => 'Destinations: Destinations target: Destinations',
        'optional' => 1,
        'remember' => 1,
      ),
      'case' => 0,
      'id' => 'field_destinations_target',
      'table' => 'node_data_field_destinations',
      'field' => 'field_destinations_target',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('items_per_page', 20);
  $handler->override_option('use_pager', '0');
  $handler->override_option('style_plugin', 'bulk');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'title' => 'title',
      'delta' => 'delta',
      'field_publicationdate_value' => 'field_publicationdate_value',
    ),
    'info' => array(
      'title' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'delta' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'field_publicationdate_value' => array(
        'sortable' => 0,
        'separator' => '',
      ),
    ),
    'default' => '-1',
    'execution_type' => '1',
    'display_type' => '1',
    'hide_select_all' => 0,
    'skip_confirmation' => 0,
    'display_result' => 0,
    'merge_single_action' => 1,
    'selected_operations' => array(
      'cckdestinations_batch_dest_add' => 'cckdestinations_batch_dest_add',
      'cckdestinations_batch_dest_change' => 'cckdestinations_batch_dest_change',
      'wallyedit_selector_action' => 0,
      'wallyedit_node_operations_selector_action' => 0,
      'node_assign_owner_action' => 0,
      'views_bulk_operations_delete_node_action' => 0,
      'node_mass_update:a27b9efabcd054685a549378b174ad11' => 0,
      'system_message_action' => 0,
      'views_bulk_operations_action' => 0,
      'views_bulk_operations_script_action' => 0,
      'node_make_sticky_action' => 0,
      'node_make_unsticky_action' => 0,
      'node_mass_update:c4d3b28efb86fd703619a50b74d43794' => 0,
      'views_bulk_operations_fields_action' => 0,
      'views_bulk_operations_taxonomy_action' => 0,
      'views_bulk_operations_argument_selector_action' => 0,
      'node_promote_action' => 0,
      'node_mass_update:14de7d028b4bffdf2b4a266562ca18ac' => 0,
      'node_mass_update:9c585624b9b3af0b4687d5f97f35e047' => 0,
      'node_publish_action' => 0,
      'system_goto_action' => 0,
      'wallyrsstonode_reject_nodes' => 0,
      'emfield_operations_reload' => 0,
      'node_unpromote_action' => 0,
      'node_mass_update:8ce21b08bb8e773d10018b484fe4815e' => 0,
      'node_save_action' => 0,
      'system_send_email_action' => 0,
      'node_mass_update:0ccad85c1ebe4c9ceada1aa64293b080' => 0,
      'node_unpublish_action' => 0,
      'node_unpublish_by_keyword_action' => 0,
    ),
    'views_bulk_operations_fields_action' => array(
      'php_code' => 0,
      'display_fields' => array(
        '_all_' => '_all_',
      ),
    ),
  ));

  $views[$view->name] = $view;

  return $views;
}
