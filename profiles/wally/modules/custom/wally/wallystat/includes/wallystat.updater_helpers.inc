<?php 

/**
 * Return the list of all available functions which return a statistic's parameter.
 */
function wallystat_available_params_function() {
  return array(
    'wallystat_get_url_argument' => t('One of the URL arguments'),
    'wallystat_get_node_field' => t('The value of one of the node fields'),
    'wallystat_get_eval' => t('PHP code'),
  );
}

/**
 * Return an URL argument
 */
function wallystat_get_url_argument($node, $i) {
  return arg($i);
}

/**
 * Return the form element(s) for this callback
 */
function wallystat_get_url_argument_form() {
  $form = array();
  
  $form[] = array(
    '#type' => 'select',
    '#title' => t('Argument ID in URL'),
    '#options' => array(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10),
  );
  
  return $form;
}

/**
 * Return the value of a node field
 */
function wallystat_get_node_field($node, $field) {
  $node_field = isset($node->$field) ? $node->$field : NULL;
  return isset($node_field) ? $node_field[0]['value'] : '';
}

/**
 * Return the form element(s) for this callback
 */
function wallystat_get_node_field_form() {
  $form = array();
  
  $form[] = array(
    '#type'      => 'textfield',
    '#title'     => t('Field name'),
    '#size'      => 80,
    '#maxlength' => 512,
  );
  
  return $form;
}

/**
 * Return the evaluated code
 */
function wallystat_get_eval($node, $code) {
  return eval($code);
}

/**
 * Return the form element(s) for this callback
 */
function wallystat_get_eval_form() {
  $form = array();
  
  $form[] = array(
    '#type'        => 'textarea',
    '#description' => t('The $node variable is available.'),
    '#title'       => t('PHP Code'),
    '#rows'        => 5,
  );
  
  return $form;
}

/**
 * Add the stat AJAX updater callback to node pages
 */
function _wallystat_add_updater_callback(&$vars) {
  $current_id = $vars['node']->nid;
  $title = $vars['node']->title;
  $current_terms = '';
  if (isset($vars['node']->field_destinations) && is_array($vars['node']->field_destinations)){
    foreach ($vars['node']->field_destinations as $dest)
      $current_terms .= $dest['tid'].'+';
  }
  $query = "SELECT tid FROM {term_node} WHERE nid = ".$current_id;
  $result = db_query($query);
  while ($row = db_fetch_array($result))
    $current_terms .= $row['tid'].'+';

  $params_array = array();
  for ($i = 1; $i <= 10; $i++) {
    $params_callback = variable_get('wallystat_param_'.$i, array());
    $current_callback = isset($params_callback['current_callback']) ? $params_callback['current_callback'] : 'none';

    unset($params_callback[$current_callback]['name']);
    if ($current_callback != 'none') {
      $param_value = call_user_func_array($current_callback, array($vars['node']) + $params_callback[$current_callback]);
      $param_value = rawurldecode($param_value);
      $params_array[] = ($i == 10) ? 'param'.$i.'='.$param_value : 'param0'.$i.'='.$param_value;
    } else {
      $params_array[] = ($i == 10) ? 'param'.$i.'=' : 'param0'.$i.'=';
    }
  }

  $current_terms = trim($current_terms, '+');
  drupal_add_js("$(document).ready(function(){
    									updateStats();
									});
									
									function updateStats(){
										$.ajax({
                						      url: \""."/".drupal_get_path('module', 'wallystat')."/updater.php?nid=".$current_id."&title=".$title."&terms=".$current_terms."&".implode('&', $params_array)."\",
                						      cache: false,
                						      async: false
                						    })
									}
									", "inline");
}
