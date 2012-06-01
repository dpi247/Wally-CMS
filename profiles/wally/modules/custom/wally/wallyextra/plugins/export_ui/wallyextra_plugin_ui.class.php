<?php
// $Id: wallyextra_plugin_ui.class.php,v 1.1.2.1 2010/07/14 01:57:42 merlinofchaos Exp $

class wallyextra_plugin_ui extends ctools_export_ui {
  function list_form(&$form, &$form_state) {
    $this->list_form_submit($form, $form_state);
  }

  function list_filter($form_state, $item) {
    return FALSE;
  }

  function list_build_row($item, &$form_state, $operations) {
    $this->sorts[$item->name] = $item->admin_title;

    //$operations = array('delete' => $operations['delete']);
    $this->rows[$item->name] = array(
      'data' => array(
        array('data' => check_plain($item->name), 'class' => 'ctools-export-ui-name'),
        array('data' => check_plain($item->admin_title), 'class' => 'ctools-export-ui-title'),
        array('data' => check_plain($item->category), 'class' => 'ctools-export-ui-category'),
        array('data' => theme('links', $operations), 'class' => 'ctools-export-ui-operations'),
      ),
      'title' => check_plain($item->admin_description),
      'class' => !empty($item->disabled) ? 'ctools-export-ui-disabled' : 'ctools-export-ui-enabled',
    );
  }

  function list_table_header() {
    return array(
      array('data' => t('Name'), 'class' => 'ctools-export-ui-name'),
      array('data' => t('Admin title'), 'class' => 'ctools-export-ui-title'),
      array('data' => t('Category'), 'class' => 'ctools-export-ui-category'),
      array('data' => t('Operations'), 'class' => 'ctools-export-ui-operations'),
    );
  }
  
  /**
   * Page callback to delete an exportable item.
   */
  function delete_page($js, $input, $item) {
    $form_state = array(
      'plugin' => $this->plugin,
      'object' => &$this,
      'ajax' => $js,
      'item' => $item,
      'op' => $item->export_type & EXPORT_IN_CODE ? 'revert' : 'delete',
      'rerender' => TRUE,
      'no_redirect' => TRUE,
    );

    ctools_include('form');

    $output = ctools_build_form('ctools_export_ui_delete_confirm_form', $form_state);
    if (!empty($form_state['executed'])) {
      db_query("DELETE FROM {panels_pane} WHERE subtype = '%s'", $item->name);
      
      ctools_export_crud_delete($this->plugin['schema'], $item);
      $export_key = $this->plugin['export']['key'];
      $message = str_replace('%title', check_plain($item->{$export_key}), $this->plugin['strings']['confirmation'][$form_state['op']]['success']);
      drupal_set_message($message);
      drupal_goto(ctools_export_ui_plugin_base_path($this->plugin));
    }

    return $output;
  }
}
