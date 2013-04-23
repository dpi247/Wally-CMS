<?php

/**
 * @file
 * Argument handler to filter results by target.
 */

/**
 * Argument handler to filter results by target.
 */
class cckdestinations_views_handler_argument_layout extends views_handler_argument {

  

  /**
   * Set up the query for this argument.
   *
   * The argument sent may be found at $this->argument.
   */
  function query() {
    $this->ensure_my_table();
    $this->query->add_where(0, $this->table_alias .'.'. $this->real_field ." LIKE '%%%s%'", $this->argument);
  }
}
