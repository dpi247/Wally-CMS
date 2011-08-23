<?php

/**
 * @file
 * Contains filter handlers for protocol filters with views.
 */

/**
 * Filter handler for limiting a view to URLs of a certain protocol.
 */
class cckdestinations_views_handler_filter_tid extends views_handler_filter_term_node_tid {
  function construct() {
    parent::construct();
    $this->content_field = content_fields($this->definition['content_field_name']);
    $field = $this->content_field;
    $this->value_title = $field['widget']['label'];
  }
}
