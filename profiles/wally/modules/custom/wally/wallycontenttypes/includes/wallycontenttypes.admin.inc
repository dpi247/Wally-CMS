<?php
// $Id: yaxim.admin.inc,v 1.0 2010/08/23 14:39:00 rso Exp $

/**
 * Settings form.
 */
function wallycontenttypes_page_admin_form($form_state) {
  $form['wallycontenttypes_packagepopulate_cache'] = array(
    '#type' => 'checkbox',
    '#title' => t('Cache the results of the packagepopulate function'),
    '#default_value' => variable_get('wallycontenttypes_packagepopulate_cache', 1),
  );
  
  $form['wallycontenttypes_packagepopulate_recursive'] = array(
    '#type' => 'checkbox',
    '#title' => t('Load the node recursively in the packagepopulate function'),
    '#default_value' => variable_get('wallycontenttypes_packagepopulate_recursive', 0),
  );
  
  return system_settings_form($form);
}
