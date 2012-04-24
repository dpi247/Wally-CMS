<?php

function wallyctools_page_default_target_form($form_state) {
  $form = array();

  drupal_set_message(t('You can choose, for every variant of the taxonomy term page, which redacblock will be used as default'));

  $default_redacblocks = variable_get('wallyctools_default_redacblocks', array());

  $form['rows'] = array();

  $panels = wallytoolbox_get_panels('term_view');
  foreach ($panels as $panel) {
    $panel_id = $panel->conf['did'];

    $redac_options = array();
    foreach ($panel->display->content as $content) {
      if ($content->type == 'redacblock') {
        $redac_options[$content->subtype] = $content->subtype;
      }
    }
    if (!empty($redac_options)) {
      $form['rows'][$panel_id] = array();

      $form['rows'][$panel_id]['name'] = array(
        '#type' => 'markup',
        '#value' => $panel->conf['title'],
      );

      $form['rows'][$panel_id]['default_redacblock'] = array(
        '#type' => 'select',
        '#options' => $redac_options,
        '#default_value' => isset($default_redacblocks[$panel_id]) ? $default_redacblocks[$panel_id]['default_redacblock'] : 0,
      );
    }
  }

  if (!empty($form['rows'])) {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
  }

  $form['#theme'] = 'wallyctools_form_default_target_form';
  $form['#tree'] = TRUE;

  return $form;
}

function wallyctools_page_default_target_form_submit($form, $form_state) {
  variable_set('wallyctools_default_redacblocks', $form_state['values']['rows']);
}

function wallyctools_page_reorder_settings_form($form_state) {
  $form = array();
  
  $form['wallyctools_reorder_check_conflict'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check conflicts before applying reorder'),
    '#default_value' => variable_get('wallyctools_reorder_check_conflict', 1),
  );
  
  return system_settings_form($form);
}

function wallyctools_page_destinations_tree_caching_form($form_state) {
  $form = array();
  
  $form['refresh_cache'] = array(
    '#type' => 'submit',
    '#value' => t('Force refresh of the cached destination tree'),
  );
  
  return $form;
}

function wallyctools_page_destinations_tree_caching_form_submit($form, $form_state) {
  $dest_vid = variable_get('wallymport_destinationpath', 0);
  $depth = 0;
  if (wallyctools_build_term_handler_mapping($dest_vid, $depth, TRUE)) {
    drupal_set_message(t('Cached destination tree rebuild'));
  } else {
    drupal_set_message(t('An error occurred'), 'error');
  }
}
