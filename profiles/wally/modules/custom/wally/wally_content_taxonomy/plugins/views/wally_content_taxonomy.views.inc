<?php
// $Id: signup.views.inc,v 1.4.2.7 2009/07/22 21:47:47 dww Exp $


/**
 * @file
 * Provides support for Views integration.
 *
 * This exposes signup-related data and declares handlers (fields, filters)
 * and plugins (argument validators).
 */

/**
 * Implementation of hook_views_data().
 */
function wally_content_taxonomy_views_data() {
  
  $data['term_node']['field_wally_confidence_score'] = array(
    'real_field' => 'tid',
    'title'      => t('Confidence Score'),
    'help'       => t('Confidence Score.'),
    'field'      => array(
      'handler' => 'wally_content_taxonomy_handler_field_confidence',
      'help'    => 'Display all taxonomy terms associated with a node from specified vocabularies. with confidence score',
    ),
  );
  $data['term_node']['wally_confidence_score'] = array(
    'title' => t('Confidence Score'),
    'help'  => t('Confidence Score'),
  	'sort'  => array(
      'handler' => 'views_handler_sort',
      'help'    => t("Sort by confidence score"),
    ),
  );
  $data['term_node']['field_wally_relevancy_score'] = array(
    'real_field' => 'tid',
    'title'      => t('Relevancy Score'),
    'help'       => t('Relevancy Score.'),
    'field'      => array(
      'handler' => 'wally_content_taxonomy_handler_field_relevancy',
      'help'    => 'Display all taxonomy terms associated with a node from specified vocabularies. with relevancy score',
    ),
  );
  $data['term_node']['wally_relevancy_score'] = array(
    'title' => t('Relevancy Score'),
    'help'  => t('Relevancy Score'),
    'sort'  => array(
      'handler' => 'views_handler_sort',
      'help'    => t("Sort by relevancy score"),
    ),
  );
  return $data;
}

/**
 * Implementation of hook_views_handlers().
 */
function wally_content_taxonomy_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'wally_content_taxonomy') .'/plugins/views/handlers',
    ),
    'handlers' => array(
      'wally_content_taxonomy_handler_field_confidence' => array(
        'parent' => 'views_handler_field_prerender_list',
      ),
  	  'wally_content_taxonomy_handler_field_relevancy' => array(
        'parent' => 'views_handler_field_prerender_list',
      ),
    ),
  );
}
