<?php

/**
 * Implementation of hook_node_info().
 */
function test_content_type_feature_node_info() {
  $items = array(
    'wally_textcontenttype' => array(
      'name' => t('wally_TextContentType'),
      'module' => 'features',
      'description' => t('Wally test import test content type. With all kind of field types. '),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Body'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}
