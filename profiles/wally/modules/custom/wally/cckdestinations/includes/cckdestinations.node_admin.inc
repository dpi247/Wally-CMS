<?php

/**
 * Menu callback: content administration.
 */
function cckdestinations_node_admin_content($form_state) {
  module_load_include('inc', 'node', 'node.admin');
  if (isset($form_state['values']['operation']) && $form_state['values']['operation'] == 'delete') {
    $form = node_multiple_delete_confirm($form_state, array_filter($form_state['values']['nodes']));
    $form['#submit'][] = 'cckdestination_node_multiple_delete_confirm_submit';
    return $form;
  }
  $form = cckdestinations_node_filter_form();

  $form['admin']  = cckdestinations_node_admin_nodes();

  return $form;
}

/**
 * Form builder: Builds the node administration overview.
 */
function cckdestinations_node_admin_nodes() {
  module_load_include('inc', 'node', 'node.admin');
  $session = &$_SESSION['node_overview_filter'];
  $session = is_array($session) ? $session : array();

  $field_dest_tid = array();
  foreach ($session as $filter) {
    list($type, $value) = $filter;
    if ($type == 'category') {
      $field_dest_tid[] = '`field_destinations_tid` = "'.$value.'"';
    }
  }
  
  if (sizeof($field_dest_tid)) {
    $where_query = ' WHERE '.implode(wally_variable_get('cckdestinations_and_or_or', ' AND '), $field_dest_tid);
  }

  $result = pager_query(db_rewrite_sql('SELECT `nid` FROM {content_field_destinations}'.$where_query.' ORDER BY `field_destinations_time_rank` DESC'), 50, 0, NULL, $filter['args']);
	
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
  $and_or_or = wally_variable_get('cckdestinations_and_or_or', ' AND ');

  $form = array();
  
  $vocabulary_id = variable_get('wallymport_destinationpath', 0);
  $hierarchies = wallyctools_get_hierarchies();
  
  drupal_add_js(array('shortcckdestchoices' => $hierarchies), 'setting');
  drupal_add_js(drupal_get_path('module', 'wallytoolbox').'/scripts/shortdest.js');

  $form['reorder'] = array(
    '#type' => 'fieldset',
    '#title' => t('Reorder by'),
    '#tree' => TRUE,
    '#attributes' => array('id' => 'field-destinations-items'),
  );
  
  $tid_options = array('Not Set');
  foreach ($hierarchies as $id => $val) $tid_options[$id] = $val['hierarchic_name'];
  $form['reorder']['tid'] = array(
    '#title' => t('Destination Path'),
    '#type' => 'select',
    '#options' => $tid_options,
    '#vid' => $vocabulary_id,
    '#attributes' => array('class' => 'tid'),
  );

  $target_options = array('none' => 'none');
  $array_of_redacblock_panes = wallyctools_get_redacblock_panes();
  foreach ($array_of_redacblock_panes as $pane) $target_options[$pane->name] = $pane->name;
  $form['reorder']['target'] = array(
    '#title' => t('Target'),
    '#type' => 'select',
    '#options' => $target_options,
  );
  
  $form['reorder']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Reorder'),
  );

  $i = 0;
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only items where'),
    '#theme' => 'cckdestinations_node_filters',
  );
  
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

  $form['filters']['status']['category'] = array('#type' => 'select', '#options' => $filters['category']['options']);
  $form['filters']['filter'] = array('#type' => 'radios', '#options' => array('category' => $filters['category']['title']), '#default_value' => 'category');
  $form['filters']['buttons']['submit'] = array('#type' => 'submit', '#value' => $filter_button_value);
  if (count($session)) {
    $form['filters']['buttons']['undo'] = array('#type' => 'submit', '#value' => t('Undo'));
    $form['filters']['buttons']['reset'] = array('#type' => 'submit', '#value' => t('Reset'));
  } else {
    $form['and_or_or'] = array('#type' => 'radios', '#title' => t('Selection rule'), '#options' => array(' AND ' => t('All destination must pass'), ' OR ' => t('Only one destination must pass')), '#default_value' => ' AND ');
  }

  drupal_add_js('misc/form.js', 'core');

  $form['#submit'][] = 'cckdestinations_node_filter_form_submit';
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
  $values = $form_state['values'];
  $filters = node_filters();

  switch ($values['op']) {
  	case t('Reorder'):
    	$form_state['redirect'] = 'admin/wally/wallyctools-operations/reorder/25/'.$values['reorder']['tid'].'/'.$values['reorder']['target'];
    	break;

    case t('Filter'):
      wally_variable_set('cckdestinations_and_or_or', $values['and_or_or']);
    case t('Add'):
    case t('Refine'):
      if (isset($values['filter'])) {
        $filter = $values['filter'];
        // Flatten the options array to accommodate hierarchical/nested options.
        $flat_options = form_options_flatten($filters[$filter]['options']);
        if (isset($flat_options[$form_state['values'][$filter]])) {
          $_SESSION['node_overview_filter'][] = array($filter, $values[$filter]);
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
 * 
 * Form submit in case of delete action
 * @param array $form
 * @param array $form_state
 */
function cckdestination_node_multiple_delete_confirm_submit($form, &$form_state) {
  module_load_include('inc', 'node', 'node.admin');
  node_multiple_delete_confirm_submit($form, $form_state);
  $form_state['redirect'] = 'admin/wally/cckdestinations/node';
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
  
  $and_or_or = wally_variable_get('cckdestinations_and_or_or', ' AND ');

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


