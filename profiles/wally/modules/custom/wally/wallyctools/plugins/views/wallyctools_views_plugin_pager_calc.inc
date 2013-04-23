<?php
/**
 * @file
 * Extend Full Pager plugin to create Calc pager
 */

class wallyctools_views_plugin_pager_calc extends views_plugin_pager_full {
  function summary_title() {
    if (!empty($this->options['offset'])) {
      return format_plural($this->options['items_per_page'], 'Calc pager, @count item, skip @skip', 'Calc pager, @count items, skip @skip', array('@count' => $this->options['items_per_page'], '@skip' => $this->options['offset']));
    }
    return format_plural($this->options['items_per_page'], 'Calc pager, @count item', 'Calc pager, @count items', array('@count' => $this->options['items_per_page']));
  }

  function use_count_query() {
    return FALSE;
  }

  function post_execute(&$result) {
    $SQL = 'SELECT found_rows() AS total_items';
    $total_results = $this->view->query->db_query($SQL);
    $this->total_items = $this->view->query->db_result($total_results);
    $this->update_page_info();
  }

  function pre_execute(&$query, &$args) {
    $needle = 'SELECT ';
    $replacement = 'SELECT SQL_CALC_FOUND_ROWS ';
    $pos = strpos($query, $needle);
    if ($pos !== false) {
      $query = substr_replace($query, $replacement, $pos, strlen($needle));
    }
  }
}
