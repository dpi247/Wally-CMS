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
function del_cache_admin($key) {
  $return = '';
  $return .= drupal_get_form('del_cache_admin_form', $key);
  
  return $return;
}

/**
 * Form for the delete selected cache elements admin page.
 */
function del_cache_admin_form(&$form_state, $key) {
  if (class_exists('Memcached') || class_exists('Memcache')) {
    $cache_elems = array();
    $res = db_query("SHOW TABLES LIKE 'cache%'");
    
    while ($one_res = db_fetch_array($res)) {
      $val_res = array_values($one_res);
      $cache_elems[$val_res[0]] = dwallymemcache_get_keys_index($val_res[0]);
    }
    
    if (!empty($cache_elems)) {
      foreach ($cache_elems as $cache_table => $subtables) {
        if (!empty($subtables)) {
          foreach ($subtables as $subtable => $cached_keys) {
            if (!empty($cached_keys)) {
              foreach ($cached_keys as $cached_key => $cached_fullkey) {
                if (strstr($cached_key, $key)) {
                  $form['blocks_cache'][$cache_table][$subtable][$cached_key] = array(
                    '#type' => 'checkbox',
                    '#title' => '<a href="/admin/wally/searchcache/viewkey/'.rawurlencode($cached_key).'">'.$cached_fullkey.'</a>',
                    '#default_value' => 0,
                  );
                }
              }
              
              if (isset($form['blocks_cache'][$cache_table][$subtable])) {
                $form['blocks_cache'][$cache_table][$subtable]['#type'] = 'fieldset';
                $form['blocks_cache'][$cache_table][$subtable]['#title'] = $subtable;
                $form['blocks_cache'][$cache_table][$subtable]['#collapsible'] = TRUE;
                $form['blocks_cache'][$cache_table][$subtable]['#collapsed'] = TRUE;
              }
            }
          }
          
          if (isset($form['blocks_cache'][$cache_table])) {
            $form['blocks_cache'][$cache_table]['#type'] = 'fieldset';
            $form['blocks_cache'][$cache_table]['#title'] = $cache_table;
            $form['blocks_cache'][$cache_table]['#collapsible'] = TRUE;
            $form['blocks_cache'][$cache_table]['#collapsed'] = TRUE;
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
  
  $form['back'] = array(
    '#type' => 'submit',
    '#title' => t('Cancel'),
    '#value' => t('Cancel'),
  );
   
  return $form;
}

/**
 * Validation for form for the delete selected cache admin page.
 */
function del_cache_admin_form_validate($form, &$form_state) {
  if ($form_state['clicked_button']['#parents'][0] == 'back')
    return;
  
  $values = $form_state['values'];
  if (is_array($values)) {
    foreach ($values as $value) {
      if ($value == 1)
        return;
    }
  }
  
  form_set_error('blocks_cache', t('Select at least one block.'));
}

/**
 * Submit actions for form for the delete selected cache admin page.
 */
function del_cache_admin_form_submit(&$form, &$form_state) {
  if ($form_state['clicked_button']['#parents'][0] == 'submit') {
    if (class_exists('Memcached') || class_exists('Memcache')) {
      foreach (element_children($form['blocks_cache']) as $cache_table) {
        foreach (element_children($form['blocks_cache'][$cache_table]) as $subtable) {
          foreach (element_children($form['blocks_cache'][$cache_table][$subtable]) as $name) {
            if ($form['blocks_cache'][$cache_table][$subtable][$name]['#value'] == 1) {
              if (dwallymemcache_delete($name, $cache_table, FALSE, $subtable)) {
                drupal_set_message($name.' has been deleted from '.$cache_table);
              } else {
                drupal_set_message($name.' has not been deleted.', 'warning');
              }
            }
          }
        }
      }
    } else {
      foreach (element_children($form['blocks_cache']) as $table) {
        foreach (element_children($form['blocks_cache'][$table]) as $value) {
          if ($form['blocks_cache'][$table][$value]['#value'] == 1) {
            cache_clear_all($value, $table, TRUE);
            drupal_set_message($value.' has been deleted from '.$table); 
          }
        }
      }
    }
  }
  
  $form['#redirect'] = 'admin/wally/searchcache/';
}

function view_cache_admin($cache_id) {
  $cached = cache_get($cache_id);
  dsm($cached->data);
  return '';
}
