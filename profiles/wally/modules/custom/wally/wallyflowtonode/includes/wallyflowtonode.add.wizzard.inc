<?php

/**
 * Get the cached changes to a given task handler.
* (Earl wrote that, not me...)
 */
function wallyflowtonode_add_get_page_cache($name) {
  
  $cache = ctools_object_cache_get('flow_item', $name);
  return $cache;
}


/**
 * Callback generated when the 'cancel' button is clicked.
 *
 * All we do here is clear the cache. 
* redirect them to where they started 
* and call them a coward
 */
function wallyflowtonode_addnewflow_subtask_cancel(&$form_state) {
  ctools_object_cache_clear('flow_item', $form_state['cache name']);
  $form_state['redirect'] = 'admin/wally/wallyflowtonode/adminflow';
  drupal_set_message('Operation Cancelled.');
}
/**
 * Callback for the proceed step
 *
 */
function wallyflowtonode_addnewflow_subtask_next(&$form_state) {
  $flow_item = &$form_state['flow_obj'];
 
  $cache = ctools_object_cache_set('flow_item', $form_state['cache name'], $flow_item);
}

/**
 * Callback generated when the 'finish' button is clicked.
 *
 * Need to handle the flow creation 
 */
function wallyflowtonode_addnewflow_subtask_finish(&$form_state) {
  $flow_item = &$form_state['flow_obj'];
  
  
  //Some cleaning before save
  $flow_item->conf=serialize($flow_item->conf);
  if(isset($flow_item->flow_key)){
    
    drupal_set_message('The flow '.$flow_item->name.' has been edited');
    drupal_write_record('wallyflowtonode_flows',$flow_item,'flow_key');
  }
  else{
    drupal_set_message('The flow '.$flow_item->name.' has been created');
    drupal_write_record('wallyflowtonode_flows',$flow_item);
  }
  // Clear the cache
  ctools_object_cache_clear('flow_item', $form_state['cache name']);
  
  $form_state['redirect'] = 'admin/wally/wallyflowtonode/adminflow/edit/'.$form_state['cache name'];

}

function wallyflowtonode_page_admin_add_wizzard(){

  $flow_id= arg(5);
  $step = arg(6);
  // required includes for wizard
  ctools_include('wizard');
  ctools_include('object-cache');
  
  // *** SETUP ARRAY multistep setup **** 
 // these are defined in some docs at end of article
 
  $form_info = array(
    'id' => 'wallyflowtonode_add',
    'path' => "admin/wally/wallyflowtonode/adminflow/edit/$flow_id/%step",
    'show trail' => TRUE,
    'show back' => TRUE,
    'show cancel' => true,
    'show return' =>false,
   // 'free trail' => TRUE, see http://drupal.org/node/663386
    'next text' => 'Proceed to next step',
    'next callback' =>  'wallyflowtonode_addnewflow_subtask_next',
    'finish callback' => 'wallyflowtonode_addnewflow_subtask_finish',
    'return callback' => 'wallyflowtonode_addnewflow_subtask_finish',
    'cancel callback' => 'wallyflowtonode_addnewflow_subtask_cancel',
   // this controls order, as well as form labels
    'order' => array(
      'basicsetttings' => t('Create Flow 1/3: Basic settings'),
      'pluginselector' => t('Create Flow 2/3: Plugin Selector'),
      'pluginspecific' => t('Create Flow 2bis/3: Plugin specific settings'),
      'finalstep' => t('Create Flow 3/3:  Final Step'),
  ),
   // here we map a step to a form id.
    'forms' => array(
      // e.g. this for the step at wombat/create 
      'basicsetttings' => array(
        'form id' => 'wallyflowtonode_basicsetttings_form'
      ),
      'pluginselector' => array(
        'form id' => 'wallyflowtonode_pluginselector_form'
      ),
      'pluginspecific' => array(
        'form id' => 'wallyflowtonode_pluginspecific_form'
      ),
      'finalstep' => array(
        'form id' => 'wallyflowtonode_finalstep_form'
      ),
      ),
  );
  
  
  
  
  // *** SETTING THE FORM UP FOR MULTISTEP *** //
  $form_state = array(
    'cache name' =>wallyflowtonode_get_cache_name($flow_id),
  );
  
  
  
  
  //CASE A: we are at the begin of the wizard, if we have a row in DB, we should use it
  if(!$step){
    $result=db_query('SELECT * FROM {wallyflowtonode_flows} WHERE flow_key=%d',$form_state['cache name']);
    $fff=db_fetch_object($result);
    if($fff){
      $flow_item=$fff;
    }
  }
  
  //CASE B : either We are not in the begenning of the process either we are in the begining and there is no result in DB
  // => we should use the cached version ()
  if (!$flow_item) {
  $flow_item = wallyflowtonode_add_get_page_cache($form_state['cache name']);
  } 
  // REmove pluginspecific step if no need
  if( isset($flow_item->plugin_name) && !test_has_settings_form_of_plugin(test_get_plugin_by_name($flow_item->plugin_name))){
    unset( $form_info['order']['pluginspecific']);
  }
  
  //CASE C : Blank case
  if (!$flow_item) {
    // set form to first step -- we have no data
    $step = current(array_keys($form_info['order']));
    //try to fetch flow from dtabase 
      $flow_item = new stdClass();
    
    // ** set the storage object so its ready for whatever comes next
    ctools_object_cache_set('flow_item', $form_state['cache name'], $flow_item);
  }
  //THIS IS WHERE WILL STORE ALL FORM DATA
  $form_state['flow_obj'] = $flow_item;
  
  // and this is the witchcraft that makes it work
  $output = ctools_wizard_multistep_form($form_info, $step, $form_state);
  
  
  
  return $output;
}



function wallyflowtonode_basicsetttings_form(&$form, &$form_state) {
  $flow_item = &$form_state['flow_obj'];
   $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('RSS feed name'),
    '#description' => t('The name you want to give to this feed.'),
    '#default_value' => $flow_item->name,
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
    '#weight' => 0,
  );
  
  $form['uri'] = array(
    '#type' => 'textfield',
    '#title' => t('RSS feed URI'),
    '#description' => t('URI of the requested RSS feed.'),
    '#default_value' => $flow_item->uri,
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
    '#weight' => 0.1,
  );
  
  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Type of flow'),
    '#default_value' => $flow_item->types,
  '#required' => TRUE,
    '#options' => test_get_availlable_flow_type(),
    '#weight' => 0.11,
  );
  
  
  
  $body_html_options = array();
  $body_html_query = db_query("SELECT * FROM `filter_formats");
  $i = 0;
  while ($body_html_element = db_fetch_array($body_html_query)) {
    $body_html_options[] = $body_html_element['name'];
    if ($body_html_element['name'] == $default_form_values['body_html'])
      $default_body_html = $i;
    $i++;
  }

  $form['bodyhtml'] = array(
    '#type' => 'select',
    '#title' => t('Use HTML in the text body'),
    '#description' => t('Use HTML in the text body.'),
    '#options' => $body_html_options,
    '#default_value' => $flow_item->bodyhtml,
      '#weight' => 0.2,
  );
  
  $minutes_list = array();
  for ($i=1; $i<=60; $i++) $minutes_list[$i] = $i;
  
  $form['deltat'] = array(
    '#type' => 'select',
    '#title' => t('Time between updates'),
    '#description' => t('Time, in minutes, between updates for this feed.'),
    '#options' => $minutes_list,
    '#default_value' => $flow_item->deltat,
      '#required' => TRUE,
    '#weight' => 1,
  );
  // probably important -- i'm continuing to investigate
  $form_state['no buttons'] = TRUE; 
}

/*
 * 
 * 
 */
function wallyflowtonode_basicsetttings_form_validate(&$form, &$form_state) {
  $uri_response = drupal_http_request($form['wallyflowtonode_uri']['#value']);
  if ($uri_response->status_message != 'OK') {
    form_set_error('wallyflowtonode_uri', t('The URI doesn\'t respond.'));
  }
}
function wallyflowtonode_basicsetttings_form_submit(&$form, &$form_state) {
  $submitted = $form_state['values'];
  $save_values = array('name', 'uri', 'type','bodyhtml','deltat'); 
  // maybe don't imitate this foreach
  foreach($save_values as $value) {
    // set the values in the cache object -- it gets passed back to the next step
   // because of all that work we did in the form_info array
    $form_state['flow_obj']->$value = $submitted[$value];
    
  }
}










function wallyflowtonode_pluginselector_form(&$form, &$form_state) {
  
  $flow_item = &$form_state['flow_obj'];
  $possible_plugins=test_get_plugins_that_handle_type($flow_item->type);
  
  foreach($possible_plugins as $plugin_name=>$plugin){
    $options[$plugin_name]=$plugin['name'];
    $dl="<dt>".$plugin['name']."</dt><dd>".$plugin['description']."</dd>";
  }
  dsm($options);
  dsm($flow_item);
  $dl="<dl>".$dl."</dl>";
  $form['plugin_name'] = array(
    '#type' => 'radios',
    '#tree' => true,
    '#options' => $options,
    '#title' => 'Select A plugin',
    '#default_value' => $flow_item->plugin_name,
    '#description' => $dl,
  '#required' => 1,
  );
  
  
  $form_state['no buttons'] = TRUE; 
}

function wallyflowtonode_pluginselector_form_submit(&$form, &$form_state) {
  $submitted = $form_state['values'];
  $save_values = array('plugin_name'); 
  foreach($save_values as $value) {
    // set the values in the cache object -- it gets passed back to the next step
   // because of all that work we did in the form_info array
    $form_state['flow_obj']->$value = $submitted[$value];
    
  }
  $flow_item=$form_state['flow_obj'];
  // REmove pluginspecific step if no need
  if( isset($flow_item->plugin_name) && test_has_settings_form_of_plugin(test_get_plugin_by_name($flow_item->plugin_name))){
  $form_state['clicked_button']['#next']='pluginspecific';
  }
  
}

function wallyflowtonode_pluginspecific_form(&$form, &$form_state) {
  $flow_item = &$form_state['flow_obj'];
  $form['conf']=array('#tree'=>true);
  test_get_settings_form_of_plugin(test_get_plugin_by_name($flow_item->plugin_name),$form['conf'],$form_state['flow_obj']->conf);
  
  // probably important -- i'm continuing to investigate
  $form_state['no buttons'] = TRUE; 
}
function wallyflowtonode_pluginspecific_form_submit(&$form, &$form_state) {
  $conf = $form_state['values']['conf'];
  $form_state['flow_obj']->conf = $conf;
    
  
}

function wallyflowtonode_finalstep_form(&$form, &$form_state) {
  $flow_item = &$form_state['wombat_obj'];
  $form['direct_process'] = array(
    '#type' => 'checkbox',
    '#required' => 1,
    '#title' => 'Direct process',
    '#default_value' => $flow_item->direct_process,
  );
  
  // probably important -- i'm continuing to investigate
  $form_state['no buttons'] = TRUE; 
}

/**
 * Return the unique cart_id of the user.
 */
function wallyflowtonode_get_cache_name($flow_id){
  global $user;

  if($flow_id!='new'){
    return $flow_id;
  }
  if ($user->uid) {
    $cache_name= $user->uid;
  }
  elseif (!isset($_SESSION['liste_cart_id'])) {
    $cache_name= $_SESSION['liste_cart_id'] = md5(uniqid(rand(), TRUE));
  }

  return 'wftn_'.$cache_name;
}