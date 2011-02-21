<?php

/**
 * @file
 * Contains filter handlers for protocol filters with views.
 */

/**
 * Filter handler for limiting a view to URLs of a certain protocol.
 */
class cckdestinations_views_handler_filter_target extends views_handler_filter_string {
  /*
  function query() {
   if (empty($this->value)) {
      return;
    }

    $this->ensure_my_table();
    $field = "$this->table_alias.$this->real_field";

    // Normalize values.
    $value = $this->value;
    if (is_array($value)) {
      $value = array_keys($value);
      if (count($value) == 1) {
        // If multiple is allowed but only one was chosen, use a string instead.
        $value = reset($value);
      }
    }

    if (is_array($value)) {
      // Multiple values
      $placeholders = db_placeholders($value, 'varchar');
      $operator = ($this->operator == 'is') ? 'IN' : 'NOT IN';
      $this->query->add_where($this->options['group'], "$field $operator($placeholders)", $value);
    }
    else {
      // Single value
      $operator = ($this->operator == 'is') ? '=' : '!=';
      $this->query->add_where($this->options['group'], "$field $operator '%s'", $this->value);
    }
  
  
  
  }
  */
}
