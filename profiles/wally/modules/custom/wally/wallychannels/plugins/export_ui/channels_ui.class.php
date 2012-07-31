<?php

class channels_ui extends ctools_export_ui {
  /**
   * Build a row based on the item.
   *
   * By default all of the rows are placed into a table by the render
   * method, so this is building up a row suitable for theme('table').
   * This doesn't have to be true if you override both.
   */
  function list_build_row($item, &$form_state, $operations) {
    // Set up sorting
    $key = $item->{$this->plugin['export']['key']};

    // Note: $item->type should have already been set up by export.inc so
    // we can use it safely.
    switch ($form_state['values']['order']) {
      case 'disabled':
        $this->sorts[$key] = empty($item->disabled) . $item->name;
        break;
      case 'title':
        $this->sorts[$key] = $item->{$this->plugin['export']['admin_title']};
        break;
      case 'name':
        $this->sorts[$key] = $item->name;
        break;
      case 'storage':
        $this->sorts[$key] = $item->type . $item->name;
        break;
      case 'channel':
        $this->sorts[$key] = $item->channel;
        break;
    }

    $this->rows[$key]['data'] = array();
    $this->rows[$key]['class'] = !empty($item->disabled) ? 'ctools-export-ui-disabled' : 'ctools-export-ui-enabled';

    // If we have an admin title, make it the first row.
    if (!empty($this->plugin['export']['admin_title'])) {
      $this->rows[$key]['data'][] = array('data' => check_plain($item->{$this->plugin['export']['admin_title']}), 'class' => 'ctools-export-ui-title');
    }
    $channel = wallychannels_get_plugin_by_name($item->channel);
    $this->rows[$key]['data'][] = array('data' => check_plain($channel['title']), 'class' => 'ctools-export-ui-channel');
    $this->rows[$key]['data'][] = array('data' => check_plain($item->name), 'class' => 'ctools-export-ui-name');
    $details = wallychannels_get_admin_preview_of_plugin(wallychannels_get_plugin_by_name($channel['name']), $item->settings);
    $this->rows[$key]['data'][] = array('data' => $details, 'class' => 'ctools-export-ui-details');
    $this->rows[$key]['data'][] = array('data' => check_plain($item->type), 'class' => 'ctools-export-ui-storage');
    $this->rows[$key]['data'][] = array('data' => theme('links', $operations), 'class' => 'ctools-export-ui-operations');

    // @todo : ajouter des infos propre aux plugin (par exemple le nom d'utilisateur), à hooker
    
    // Add an automatic mouseover of the description if one exists.
    if (!empty($this->plugin['export']['admin_description'])) {
      $this->rows[$key]['title'] = $item->{$this->plugin['export']['admin_description']};
    }
  }
  
  /**
   * Provide the table header.
   *
   * If you've added columns via list_build_row() but are still using a
   * table, override this method to set up the table header.
   */
  function list_table_header() {
    $header = array();
    if (!empty($this->plugin['export']['admin_title'])) {
      $header[] = array('data' => t('Title'), 'class' => 'ctools-export-ui-title');
    }

    $header[] = array('data' => t('Channel'), 'class' => 'ctools-export-ui-channel');
    $header[] = array('data' => t('Machine name'), 'class' => 'ctools-export-ui-name');
    $header[] = array('data' => t('Details'), 'class' => 'ctools-export-ui-details');
    $header[] = array('data' => t('Storage'), 'class' => 'ctools-export-ui-storage');
    $header[] = array('data' => t('Operations'), 'class' => 'ctools-export-ui-operations');

    return $header;
  }
}
