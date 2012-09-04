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
  
  /**
   * Execute the wizard for editing.
   *
   * For complex objects, sometimes a wizard is needed. This is normally
   * activated by setting 'use wizard' => TRUE in the plugin definition
   * and then creating a 'form info' array to feed the wizard the data
   * it needs.
   *
   * When creating this wizard, the plugin is responsible for defining all forms
   * that will be utilized with the wizard.
   *
   * Using 'add order' or 'edit order' can be used to ensure that add/edit order
   * is different.
   */
  function edit_execute_form_wizard(&$form_state) {
    $form_info = $this->get_wizard_info($form_state);

    // If there aren't any forms set, fail.
    if (empty($form_info['order'])) {
      return MENU_NOT_FOUND;
    }

    // Figure out if this is a new instance of the wizard
    if (empty($form_state['step'])) {
      $form_state['step'] = reset(array_keys($form_info['order']));
    }

    if (empty($form_info['order'][$form_state['step']]) && empty($form_info['forms'][$form_state['step']])) {
      return MENU_NOT_FOUND;
    }

    ctools_include('wizard');
    $output = ctools_wizard_multistep_form($form_info, $form_state['step'], $form_state);
    if (!empty($form_state['complete'])) {
      $this->edit_save_form($form_state);
      if (isset($form_state['redirect_after_save'])) {
        $this->plugin['redirect']['add'] = $form_state['redirect_after_save'];
        $this->plugin['redirect']['edit'] = $form_state['redirect_after_save'];
      }
    }
    else if ($output && !empty($form_state['item']->export_ui_item_is_cached)) {
      drupal_set_message(t('You have unsaved changes. These changes will not be made permanent until you click <em>Save</em>.'), 'warning');
    }

    // Unset the executed flag if any non-wizard button was pressed. Those
    // buttons require special handling by whatever client is operating them.
    if (!empty($form_state['executed']) && empty($form_state['clicked_button']['#wizard type'])) {
      unset($form_state['executed']);
    }
    return $output;
  }
  
  /**
   * Page callback to reset authentication for an exportable item.
   */
  function reset_page($js, $input, $item) {
    $form_state = array(
      'plugin' => $this->plugin,
      'object' => &$this,
      'ajax' => $js,
      'item' => $item,
      'op' => 'reset',
      'rerender' => TRUE,
      'no_redirect' => TRUE,
    );
  
    ctools_include('form');

    $output = ctools_build_form('ctools_export_ui_delete_confirm_form', $form_state);
    if (!empty($form_state['executed'])) {
      $channel = wallychannels_get_plugin_by_name($item->channel);
      $channel_plugin = wallychannels_get_plugin_by_name($channel['name']);
      if(wallychannels_has_reset_function_of_plugin($channel_plugin)) {
        $reset_function = wallychannels_get_reset_function_of_plugin($channel_plugin);
        $reset_function($item->settings);
        if ($result = ctools_export_crud_save($this->plugin['schema'], $item)) {
          $message_op = 'success';
          $message_type = 'status';
        } else {
          $message_op = 'fail';
          $message_type = 'error';
        }
      } else {
        $message_op = 'fail';
        $message_type = 'error';
      }

      $export_key = $this->plugin['export']['key'];
      $message = str_replace('%title', check_plain($item->{$export_key}), $this->plugin['strings']['confirmation'][$form_state['op']][$message_op]);
      drupal_set_message($message, $message_type);
      drupal_goto(ctools_export_ui_plugin_base_path($this->plugin));
    }

    return $output;
  }
}
