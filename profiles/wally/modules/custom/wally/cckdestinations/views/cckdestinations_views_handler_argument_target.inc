<?php

/**
 * @file
 * Argument handler to filter results by target.
 */

/**
 * Argument handler to filter results by target.
 */
class cckdestinations_views_handler_argument_target extends views_handler_argument {

  
 /**
   * Provide defaults for the argument when a new one is created.
   */
  function options(&$options) {
    parent::options($options);
  }
  
  /**
   * Set up the query for this argument.
   *
   * The argument sent may be found at $this->argument.
   */
  function query() {
    $this->ensure_my_table();
    $this->query->add_where(0, $this->table_alias .'.'. $this->real_field ." = '%s'", $this->argument);
  }
}
