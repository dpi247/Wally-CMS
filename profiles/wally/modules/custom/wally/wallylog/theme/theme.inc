<?php

function theme_wallylog_page_main_board($status) {
  $output = '';
  
  drupal_add_css(drupal_get_path('module', 'wallylog').'/css/main_board.css');
  
  foreach ($status as $stat) {
    $fieldset = array(
      '#title' => $stat['title'],
      '#description' => $stat['description'] ? $stat['description'] : '',
      '#collapsible' => TRUE,
      '#collapsed' => isset($stat['error']) ? !$stat['error'] : TRUE,
      '#children' => $stat['content'],
      '#attributes' => array(
        'class' => $stat['error'] ? 'error' : '',
      ),
    );
    $output .= theme('fieldset', $fieldset);
  }

  return $output;
}

/**
 * Theme wallylog administration filter selector.
 *
 * @ingroup themeable
 */
function theme_wallylog_filters($form) {
  $output = '';
  foreach (element_children($form['status']) as $key) {
    $output .= drupal_render($form['status'][$key]);
  }
  $output .= drupal_render($form['display_dblog']);
  $output .= '<div id="wallylog-admin-buttons">'. drupal_render($form['buttons']) .'</div>';
  return $output;
}
