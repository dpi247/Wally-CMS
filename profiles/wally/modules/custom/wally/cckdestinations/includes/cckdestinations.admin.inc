<?php
// $Id: yaxim.admin.inc,v 1.0 2010/08/23 14:39:00 rso Exp $

/**
 * Settings form.
 */
function cckdestinations_page_admin_form($form_state) {
  $form['cckdestinations_rank_delta_t'] = array(
    '#type' => 'textfield',
    '#title' => t('Time "between" two ranks'),
    '#description' => t('The step of time, in hours, that will be set between two rank.'),
    '#default_value' => variable_get('cckdestinations_rank_delta_t', 1),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  
  $form['cckdestinations_rank_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum rank value'),
    '#description' => t('The available ranks range will be from 1 to this limit, with step of 1.'),
    '#default_value' => variable_get('cckdestinations_rank_limit', 10),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  
  return system_settings_form($form);
}

/**
 * Validate function for the settings form.
 */
function wallyexport_page_admin_form_validate($form, &$form_state) {
  $delta_t = $form_state['values']['cckdestinations_rank_delta_t'];
  if (!is_numeric($delta_t) || $delta_t <= 0) {
    form_set_error('cckdestinations_rank_delta_t', t('The given time value must be a numeric value greater than 0.'));
  }
  
  $rank_limit = $form_state['values']['cckdestinations_rank_limit'];
  if (!is_numeric($rank_limit) || $rank_limit < 1) {
    form_set_error('cckdestinations_rank_limit', t('The given maximum rank value must be a numeric value greater or equal than 1.'));
  }
}

/**
 * Menu callback: content administration.
 */
function cckdestinations_node_admin_content($form_state) {
  require_once './'.drupal_get_path('module', 'node').'/node.admin.inc';
  if (isset($form_state['values']['operation']) && $form_state['values']['operation'] == 'delete') {
    return node_multiple_delete_confirm($form_state, array_filter($form_state['values']['nodes']));
  }
  $form = cckdestinations_node_filter_form();

  $form['#theme'] = 'node_filter_form';
  $form['admin']  = cckdestinations_node_admin_nodes();

  return $form;
}

/**
 * Form builder: Builds the node administration overview.
 */
function cckdestinations_node_admin_nodes() {
  require_once './'.drupal_get_path('module', 'node').'/node.admin.inc';
  $session = &$_SESSION['node_overview_filter'];
  $session = is_array($session) ? $session : array();

  $field_dest_tid = array();
  foreach ($session as $filter) {
    list($type, $value) = $filter;
    if ($type == 'category') {
      $field_dest_tid[] = '`field_destinations_tid` = "'.$value.'"';
    }
  }
  
  if (sizeof($field_dest_tid))
    $where_query = ' WHERE '.implode(variable_get('cckdestinations_and_or_or', ' AND '), $field_dest_tid);

  $result = pager_query(db_rewrite_sql('SELECT `nid` FROM {content_field_destinations}'.$where_query), 50, 0, NULL, $filter['args']);

  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );
  $options = array();
  foreach (module_invoke_all('node_operations') as $operation => $array) {
    $options[$operation] = $array['label'];
  }
  $form['options']['operation'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => 'approve',
  );
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#submit' => array('node_admin_nodes_submit'),
  );

  $languages = language_list();
  $destination = drupal_get_destination();
  $nodes = array();
  while ($node = db_fetch_object($result)) {
    $node = node_load($node->nid);
    $nodes[$node->nid] = '';
    $options = empty($node->language) ? array() : array('language' => $languages[$node->language]);
    $form['title'][$node->nid] = array('#value' => l($node->title, 'node/'. $node->nid, $options) .' '. theme('mark', node_mark($node->nid, $node->changed)));
    $form['name'][$node->nid] =  array('#value' => check_plain(node_get_types('name', $node)));
    $form['username'][$node->nid] = array('#value' => theme('username', $node));
    $form['status'][$node->nid] =  array('#value' => ($node->status ? t('published') : t('not published')));
    $form['operations'][$node->nid] = array('#value' => l(t('edit'), 'node/'. $node->nid .'/edit', array('query' => $destination)));
  }
  $form['nodes'] = array('#type' => 'checkboxes', '#options' => $nodes);
  $form['pager'] = array('#value' => theme('pager', NULL, 50, 0));
  $form['#theme'] = 'node_admin_nodes';
  return $form;
}

/**
 * Return form for node administration filters.
 */
function cckdestinations_node_filter_form() {
  require_once './'.drupal_get_path('module', 'node').'/node.admin.inc';
  $session = &$_SESSION['node_overview_filter'];
  $session = is_array($session) ? $session : array();
  $filters = cckdestinations_node_filters();
  
  $and_or_or = variable_get('cckdestinations_and_or_or', ' AND ');

  $i = 0;
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only items where'),
    '#theme' => 'cckdestinations_node_filters',
  );
  $form['#submit'][] = 'cckdestinations_node_filter_form_submit';
  
  foreach ($session as $filter) {
    list($type, $value) = $filter;
    if ($type == 'category') {
      // Load term name from DB rather than search and parse options array.
      $value = module_invoke('taxonomy', 'get_term', $value);
      $value = $value->name;
      
      if ($i++) {
        $form['filters']['current'][] = array('#value' => t('<em>%c</em> where <strong>%a</strong> is <strong>%b</strong>', array('%a' => $filters[$type]['title'], '%b' => $value, '%c' => strtolower(trim($and_or_or)))));
      }
      else {
        $form['filters']['current'][] = array('#value' => t('<strong>%a</strong> is <strong>%b</strong>', array('%a' => $filters[$type]['title'], '%b' => $value)));
      }
    }
    if (in_array($type, array('type', 'language'))) {
      // Remove the option if it is already being filtered on.
      unset($filters[$type]);
    }
  }
  
  if (count($session)) {
    if ($and_or_or == ' AND ') {
      $filter_button_value = t('Refine');
    } else {
      $filter_button_value = t('Add');
    }
  } else {
    $filter_button_value = t('Filter');
  }
  
  $name = $filters['category']['title'];
  $form['filters']['status']['category'] = array('#type' => 'select', '#options' => $filters['category']['options']);
  
  $form['filters']['filter'] = array('#type' => 'radios', '#options' => array('category' => $name), '#default_value' => 'category');
  $form['filters']['buttons']['submit'] = array('#type' => 'submit', '#value' => $filter_button_value);
  if (count($session)) {
    $form['filters']['buttons']['undo'] = array('#type' => 'submit', '#value' => t('Undo'));
    $form['filters']['buttons']['reset'] = array('#type' => 'submit', '#value' => t('Reset'));
  } else {
    $form['and_or_or'] = array('#type' => 'radios', '#title' => t('Selection rule'), '#options' => array(' AND ' => t('All destination must pass'), ' OR ' => t('Only one destination must pass')), '#default_value' => ' AND ');
  }

  drupal_add_js('misc/form.js', 'core');

  return $form;
}

/**
 * List node administration filters that can be applied.
 */
function cckdestinations_node_filters() {
  // The taxonomy filter
  if ($taxonomy = module_invoke('taxonomy', 'form_all', 1)) {
    $filters['category'] = array('title' => t('destination'), 'options' => array('Destination Path' => $taxonomy['Destination Path']));
  }

  return $filters;
}

/**
 * Process result from node administration filter form.
 */
function cckdestinations_node_filter_form_submit($form, &$form_state) {
  $filters = node_filters();
  switch ($form_state['values']['op']) {
    case t('Filter'):
      variable_set('cckdestinations_and_or_or', $form_state['values']['and_or_or']);
    case t('Add'):
    case t('Refine'):
      if (isset($form_state['values']['filter'])) {
        $filter = $form_state['values']['filter'];

        // Flatten the options array to accommodate hierarchical/nested options.
        $flat_options = form_options_flatten($filters[$filter]['options']);

        if (isset($flat_options[$form_state['values'][$filter]])) {
          $_SESSION['node_overview_filter'][] = array($filter, $form_state['values'][$filter]);
        }
      }
      break;
    case t('Undo'):
      array_pop($_SESSION['node_overview_filter']);
      break;
    case t('Reset'):
      $_SESSION['node_overview_filter'] = array();
      break;
  }
}

/**
 * Theme node administration filter selector.
 *
 * @ingroup themeable
 */
function theme_cckdestinations_node_filters($form) {
  $output = '';
  $output .= '<ul class="clear-block">';
  if (!empty($form['current'])) {
    foreach (element_children($form['current']) as $key) {
      $output .= '<li>'. drupal_render($form['current'][$key]) .'</li>';
    }
  }
  
  $and_or_or = variable_get('cckdestinations_and_or_or', ' AND ');

  $output .= '<li><dl class="multiselect">'. (!empty($form['current']) ? '<dt><em>'. t(strtolower(trim($and_or_or))) .'</em> '. t('where') .'</dt>' : '') .'<dd class="a">';
  foreach (element_children($form['filter']) as $key) {
    $output .= drupal_render($form['filter'][$key]);
  }
  $output .= '</dd>';

  $output .= '<dt>'. t('is') .'</dt><dd class="b">';

  foreach (element_children($form['status']) as $key) {
    $output .= drupal_render($form['status'][$key]);
  }
  $output .= '</dd>';

  $output .= '</dl>';
  $output .= '<div class="container-inline" id="node-admin-buttons">'. drupal_render($form['buttons']) .'</div>';
  $output .= '</li></ul>';

  return $output;
}
