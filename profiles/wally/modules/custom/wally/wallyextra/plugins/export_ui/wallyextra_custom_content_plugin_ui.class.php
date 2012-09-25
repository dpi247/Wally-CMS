<?php
// $Id: wallyextra_custom_content_plugin_ui.class.php,v 1.1.2.1 2010/07/14 01:57:42 merlinofchaos Exp $

module_load_include('class.php', 'wallyextra', 'plugins/export_ui/wallyextra_plugin_ui');

class wallyextra_custom_content_plugin_ui extends wallyextra_plugin_ui {

  /**
   * Provide the actual editing form.
   */
  function edit_form(&$form, &$form_state) {
    $item = $form_state['item'];

    parent::edit_form($form, $form_state);

    if (empty($form_state['post']) && isset($item->cid)) {
      $version = db_result(db_query("SELECT MAX(version) FROM {wallyextra_custom_content_version} WHERE cid = %d", $item->settings['id']));
      drupal_set_message(t('Saving this form will create a new version of the block (current version : !version)', array('!version' => $version)), 'warning');
    }

    $form['category'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Category'),
      '#description'   => t('What category this content should appear in. If left blank the category will be "Miscellaneous".'),
      '#default_value' => $item->category,
    );
    
    $form['title'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Title'),
      '#default_value' => $item->settings['title'],
    );

    $form['body_field']['body'] = array(
      '#type'          => 'textarea',
      '#title'         => t('Body'),
      '#default_value' => $item->settings['body'],
    );
    $parents[] = 'format';
    $form['body_field']['format'] = filter_form($item->settings['format'], 1, $parents);
    
    // Set extended description if both CCK and Token modules are enabled, notifying of unlisted keywords
    if (module_exists('content') && module_exists('token')) {
      $description = t('If checked, context keywords will be substituted in this content. Note that CCK fields may be used as keywords using patterns like <em>%node:field_name-formatted</em>.');
    } elseif (!module_exists('token')) {
      $description = t('If checked, context keywords will be substituted in this content. More keywords will be available if you install the Token module, see http://drupal.org/project/token.');
    } else {
      $description = t('If checked, context keywords will be substituted in this content.');
    }
    $description .= '<br>';
    $description .= t('You have to edit this block in the page manager to know which contexts are available.');
    $form['substitute'] = array(
      '#type'          => 'checkbox',
      '#title'         => t('Use context keywords'),
      '#description'   => $description,
      '#default_value' => !empty($item->settings['substitute']),
    );
  }
  
  /**
   * Validate callback for the edit form.
   */
  function edit_form_validate($form, $form_state) {
    // Check for name collision
    if (!isset($form_state['item']->cid) && ($form_state['values']['name'] == 'custom_version' || (ctools_export_crud_load('wallyextra_custom_content', $form_state['values']['name'])))) {
      form_set_error('name', t('Content with this name already exists. Please choose another name or delete the existing item before creating a new one.'));
    }
  }
  
  /**
   * Handle the submission of the edit form.
   *
   * At this point, submission is successful. Our only responsibility is
   * to copy anything out of values onto the item that we are able to edit.
   *
   * If the keys all match up to the schema, this method will not need to be
   * overridden.
   */
  function edit_form_submit($form, $form_state) {
    module_load_include('inc', 'wallyextra', 'plugins/content_types/'.$this->plugin['name']);

    $item = $form_state['item'];
    if ($item->cid) {
      $id = $item->settings['id'];
      $version = db_result(db_query("SELECT MAX(version) FROM {wallyextra_custom_content_version} WHERE cid = %d", $id)) + 1;
    } else {
      $id = db_result(db_query("SELECT MAX(cid) FROM {wallyextra_custom_content_version}")) + 1;
      $version = 1;
    }

    $item->name = $form_state['values']['name'];
    $form_state['conf']['id'] = $id;
    $form_state['plugin']['defaults'] = array('admin_title' => '', 'title' => '', 'body' => '', 'format' => variable_get('filter_default_format', 1), 'substitute' => TRUE);
    _wallyextra_custom_version_content_type_edit_save($item, $form_state);

    $row = new stdClass();
    $row->cid = $id;
    $row->version = $version;
    $row->date = time();
    $row->settings = $item->settings;
    $row->settings['version'] = $version;
    $row->settings['name'] = $form_state['values']['name'];
    drupal_write_record('wallyextra_custom_content_version', $row);
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
      db_query("DELETE FROM {wallyextra_custom_content_version} WHERE cid = %d", $item->settings['id']);
      
      ctools_export_crud_delete($this->plugin['schema'], $item);
      $export_key = $this->plugin['export']['key'];
      $message = str_replace('%title', check_plain($item->{$export_key}), $this->plugin['strings']['confirmation'][$form_state['op']]['success']);
      drupal_set_message($message);
      drupal_goto(ctools_export_ui_plugin_base_path($this->plugin));
    }

    return $output;
  }
}
