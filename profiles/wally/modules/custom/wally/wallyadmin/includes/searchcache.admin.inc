<?php

/**
 * HTML code for the search cache admin page.
 */
function search_cache_admin() {
  $return = '';
  $return .= drupal_get_form('search_cache_admin_form');
  
  return $return;
}

/**
 * Form for the search cache admin page.
 */
function search_cache_admin_form(&$form_state) {
    $form['search_cache'] = array(
      '#title' => t('Type the text to search'),
      '#type' => 'textfield',

    );
    $form['search_submit'] = array(
      '#type' => 'submit',
      '#value' => 'Submit'
    ); 
    
return $form;
}

function search_cache_admin_form_submit(&$form, &$form_state) {

  $form['#redirect'] = 'admin/wally/searchcache/key/'.$form_state['values']['search_cache'];
}

/**
 * HTML code for the delete selected cache elements admin page.
 */
function del_cache_admin($key) {
  $return = '';
  $return .= drupal_get_form('del_cache_admin_form', $key);
  
  return $return;
}

/**
 * Form for the delete selected cache elements admin page.
 */
function del_cache_admin_form(&$form_state, $key) {
    $list = db_query('SELECT * FROM {cache} WHERE `cid` LIKE "%%%s%%" ORDER BY `cid`', $key);
    if(!db_result($list)){
      drupal_set_message('No result for '.$key);
      drupal_goto('admin/wally/searchcache/');
    }
    while ($elem = db_fetch_array($list)) {
      $expl_id = explode('-', $elem['cid']);
      $form['blocks_cache'][$expl_id[0]] = array(
        '#type' => 'checkbox',
        '#title' => $expl_id[0],
        '#default_value' => 0,
      );
    }
   $form['submit'] = array(
    '#type' => 'submit',
    '#title' => t('Delete selected cache'),
    '#value' => t('Delete selected cache'),
  );
   
return $form;
}

/**
 * Validation for form for the delete selected cache admin page.
 */
function del_cache_admin_form_validate($form, &$form_state) {
  foreach ($form['blocks_cache'] as $name => $value) {
    if (substr($name, 0, 1) != '#' && $value['#value'] == 1)
      return;
  }
  form_set_error('blocks_cache', t('Select at least one block.'));
}

/**
 * Submit actions for form for the delete selected cache admin page.
 */
function del_cache_admin_form_submit(&$form, &$form_state) {
  $message = '';
  foreach ($form['blocks_cache'] as $name => $value) {
    if (substr($name, 0, 1) != '#' && $value['#value'] == 1) {
      cache_clear_all($name, 'cache', TRUE);
        drupal_set_message($name.' has been deleted.'); 
    }
  }
  $form['#redirect'] = 'admin/wally/searchcache/';
}
