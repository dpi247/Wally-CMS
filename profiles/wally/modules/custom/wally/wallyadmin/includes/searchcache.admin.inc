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
    '#description' => t('If no text is given, all caches will be displayed.'),
  );
  
  $form['search_subtable'] = array(
    '#title' => t('Subtable'),
    '#type' => 'textfield',
    '#description' => t('If no subtable is given, all subtables will be displayed.'),
  );
  
  $form['search_submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit'
  ); 
  
  return $form;
}

function search_cache_admin_form_submit(&$form, &$form_state) {
  $key = ($form_state['values']['search_cache'] == '') ? 'all' : $form_state['values']['search_cache'];
  $form['#redirect'] = 'admin/wally/searchcache/key/'.$key;
}

/**
 * HTML code for the delete selected cache elements admin page.
 */
function del_cache_admin($key, $subtable = '') {
  $return = '';
  $return .= drupal_get_form('del_cache_admin_form', $key, $subtable);
  
  return $return;
}

/**
 * Form for the delete selected cache elements admin page.
 */
function del_cache_admin_form(&$form_state, $key, $subtable = '') {
  if (module_exists('wallymemcache')) {
    if ($subtable != '') {
      $cache_elems[$subtable] = dwallymemcache_get_subtable_keys_from_keys_index($subtable);
    } else {
      $cache_elems = dwallymemcache_get_keys_index();
    }
    
    if ($cache_elems && !empty($cache_elems)) {
      foreach ($cache_elems as $subtable_name => $subtable_elems) {
        if ($subtable_elems && !empty($subtable_elems)) {
          foreach ($subtable_elems as $elem_key => $elem_fullkey) {
            if (strstr($elem_key, $key)) {
              $expl_id = explode('-', $elem_key);
              $form['blocks_cache'][$subtable_name][$expl_id[0]] = array(
                '#type' => 'checkbox',
                '#title' => $expl_id[0],
                '#default_value' => 0,
              );
            }
          }
          
          if (isset($form['blocks_cache'][$subtable_name])) {
            $form['blocks_cache'][$subtable_name] = array(
              '#title' => $subtable_name,
              '#collapsible' => TRUE,
              '#collapsed' => FALSE,
            );
          }
        }
      }
    }
  } else {
    $form['#tree'] = TRUE;
    $res = db_query("SHOW TABLES LIKE 'cache%'");
    while($table = db_fetch_array($res)){
      $table = array_values($table);
      $form['blocks_cache'][$table[0]] = array(
        '#type' => 'fieldset',
        '#title' => $table[0],
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );
      $where = ($key == 'all') ? '' : 'WHERE `cid` LIKE "%%%s%%"';
      $list = db_query('SELECT * FROM {'.$table[0].'} '.$where.' ORDER BY `cid`', $key);
      
      while ($elem = db_fetch_array($list)) {
        $expl_id = explode('-', $elem['cid']);
        $form['blocks_cache'][$table[0]][$expl_id[0]] = array(
          '#type' => 'checkbox',
          '#title' => $expl_id[0],
          '#default_value' => 0,
        );
      }
    }
  }

  if(!isset($form['blocks_cache']) || empty($form['blocks_cache'])){
    drupal_set_message('No result for '.$key);
    drupal_goto('admin/wally/searchcache/');
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
  foreach (element_children($form['blocks_cache']) as $table) {
    foreach (element_children($form['blocks_cache'][$table]) as $value) {
      if ($form['blocks_cache'][$table][$value]['#value'] == 1)
        return;
    }
  }
  form_set_error('blocks_cache', t('Select at least one block.'));
}

/**
 * Submit actions for form for the delete selected cache admin page.
 */
function del_cache_admin_form_submit(&$form, &$form_state) {
  foreach (element_children($form['blocks_cache']) as $table) {
    foreach (element_children($form['blocks_cache'][$table]) as $value) {
      if ($form['blocks_cache'][$table][$value]['#value'] == 1) {
        cache_clear_all($value, $table, TRUE);
        drupal_set_message($value.' has been deleted from '.$table); 
      }
    }
  }
  $form['#redirect'] = 'admin/wally/searchcache/';
}
