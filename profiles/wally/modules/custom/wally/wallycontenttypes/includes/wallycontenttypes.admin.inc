<?php
// $Id: yaxim.admin.inc,v 1.0 2010/08/23 14:39:00 rso Exp $

/**
 * Settings form.
 */
function wallycontenttypes_page_admin_form($form_state) {
  ctools_include('dependent');
  ctools_add_js('dependent');
  
  $form['wallycontenttypes_packagepopulate_cache'] = array(
    '#type' => 'checkbox',
    '#title' => t('Cache the results of the packagepopulate function'),
    '#default_value' => variable_get('wallycontenttypes_packagepopulate_cache', 1),
  );

  $form['wallycontenttypes_packagepopulate_flush_frequency'] = array (
    '#type'             => 'textfield',
    '#title'            => t('Cache lifetime (days)'),
    '#description'      => t('After how many days do you want to flush old content (0 or empty for never)'),
    '#required'         => FALSE,
    '#default_value'		=> variable_get('wallycontenttypes_packagepopulate_flush_frequency', 7),
    '#dependency'       => array('edit-wallycontenttypes-packagepopulate-cache' => array(1)),
    '#process'          => array('ctools_dependent_process'),
  );

  $form['wallycontenttypes_packagepopulate_recursive'] = array(
    '#type' => 'checkbox',
    '#title' => t('Load the node recursively in the packagepopulate function'),
    '#default_value' => variable_get('wallycontenttypes_packagepopulate_recursive', 0),
  );

  return system_settings_form($form);
}
/**
 * Render the view admin_content_wally
 * */
function wallycontenttypes_page_view_admin_content(){
  
  $content = t('Please update the wallyconttypes feature');
  
  $view_name = 'admin_content_wally';
  $display_name = 'default';
  
  $view = views_get_view($view_name);
  if ( is_object($view) ) {
    $view->set_display($display_name);
    $view->pre_execute();
    $content = $view->render($display_name);
  }
  
  return $content;
}