<?php

/**
 * Settings form.
 */
function wallystat_page_admin_form($form_state) {
  $form = array();
  $form['#tree'] = TRUE;
  
  $params_functions = array('none' => t('Not used')) + wallystat_available_params_function();
  
  $form['wallystat_params'] = array(
    '#type' => 'fieldset',
    '#title' => 'Statistics parameters',
  );
  
  ctools_include('dependent');
  ctools_add_js('dependent');
  
  for ($i=1; $i<=10; $i++) {
    $params_callback = variable_get('wallystat_param_'.$i, array());
    $current_callback = isset($params_callback['current_callback']) ? $params_callback['current_callback'] : 'none';
    
    $form['wallystat_params']['wallystat_param_'.$i] = array(
      '#type' => 'fieldset',
      '#title' => t('Parameter').' '.$i,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    
    $form['wallystat_params']['wallystat_param_'.$i]['wallystat_param_name_'.$i] = array(
      '#type' => 'textfield',
      '#title' => t('Parameter name'),
      '#default_value' => isset($params_callback[$current_callback]['name']) ? $params_callback[$current_callback]['name'] : '',
      '#size' => 80,
      '#maxlength' => 512,
    );
    
    $form['wallystat_params']['wallystat_param_'.$i]['wallystat_callback_'.$i] = array(
      '#type' => 'select',
      '#title' => t('Value'),
      '#default_value' => $current_callback,
      '#options' => $params_functions,
    );
    
    foreach (wallystat_available_params_function() as $callback => $name) {
      $form_callback = $callback.'_form';
      $form_elems = $form_callback();
      
      for ($j=0; $j<count($form_elems); $j++) {
        $form_elems[$j]['#default_value'] = isset($params_callback[$callback]['param_'.$j]) ? $params_callback[$callback]['param_'.$j] : NULL;
        $form_elems[$j]['#process'] = array('ctools_dependent_process');
        $form_elems[$j]['#dependency'] = array('edit-wallystat-params-wallystat-param-'.$i.'-wallystat-callback-'.$i => array($callback));
        $form['wallystat_params']['wallystat_param_'.$i][$callback.'_param_'.$j] = $form_elems[$j];
      }
    }
  }
  
  $form['buttons']['submit'] = array(
  	'#type' => 'submit',
  	'#value' => t('Save configuration')
  );
  
  $form['buttons']['reset'] = array(
  	'#type' => 'submit',
  	'#value' => t('Reset to defaults')
  );
  
  return $form;
}

/**
 * Validate function for the settings form.
 */
function wallystat_page_admin_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  
  if ($values['op'] == $values['buttons']['submit']) {
    for ($i=1; $i<=10; $i++) {
      $param = $values['wallystat_params']['wallystat_param_'.$i];
      if ($param['wallystat_callback_'.$i] == 'wallystat_get_node_field' && $param['wallystat_get_node_field_param_0'] == '')
        form_set_error('wallystat_params][wallystat_param_'.$i.'][wallystat_get_node_field_param_0', t('Please enter a field name.'));
    }
  }
}

/**
 * Submit function for the settings form.
 */
function wallystat_page_admin_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  
  if ($values['op'] == $values['buttons']['submit']) {
    for ($i=1; $i<=10; $i++) {
      $temp_param = array();
      $param = $values['wallystat_params']['wallystat_param_'.$i];
      if ($param['wallystat_callback_'.$i] != 'none') {
        $temp_param['current_callback'] = $param['wallystat_callback_'.$i];
        $temp_param[$temp_param['current_callback']] = array();
        $temp_param[$temp_param['current_callback']]['name'] = $param['wallystat_param_name_'.$i];
        $j=0;
        while (isset($param[$temp_param['current_callback'].'_param_'.$j])) {
          $temp_param[$temp_param['current_callback']]['param_'.$j] = $param[$temp_param['current_callback'].'_param_'.$j];
          $j++;
        }
        variable_set('wallystat_param_'.$i, $temp_param);
      }
    }
    drupal_set_message(t('The configuration options have been saved.'));
  } elseif ($values['op'] == $values['buttons']['reset']) {
    wallytoolbox_variable_del_like('wallystat_');
    drupal_set_message(t('The configuration options have been reset to their default values.'));
  }
}
