<?php

function wallyflowtonode_page_admin_add_wizard() {
  $flow_key = arg(4);
  $flow_key = flowid_to_arg($flow_key, 'new');
  $step = arg(5);
  $step = flowid_to_arg($step, 'basicsettings');

  // required includes for wizard
  ctools_include('wizard');
  ctools_include('object-cache');
  
  // *** SETUP ARRAY multistep setup **** 
 // these are defined in some docs at end of article
  $form_info = array(
    'id' => 'wallyflowtonode_add',
    'path' => 'admin/wally/wallyflowtonode/add/'.$flow_key.'/%step',
    'show trail' => TRUE,
    'show back' => TRUE,
    'show cancel' => TRUE,
    'show return' => FALSE,
    'next text' => t('Proceed to next step'),
    'next callback' => 'wallyflowtonode_addnewflow_subtask_next',
    'finish callback' => 'wallyflowtonode_addnewflow_subtask_finish',
    'return callback' => 'wallyflowtonode_addnewflow_subtask_finish',
    'cancel callback' => 'wallyflowtonode_addnewflow_subtask_cancel',
    // this controls order, as well as form labels
    'order' => array(
      'basicsettings' => t('Create Flow 1/3: Basic settings'),
      'pluginselector' => t('Create Flow 2/3: Plugin Selector'),
      'pluginspecific' => t('Create Flow 2bis/3: Plugin specific settings'),
      'finalstep' => t('Create Flow 3/3:  Final Step'),
    ),
    // here we map a step to a form id.
    'forms' => array(
      // e.g. this for the step at wombat/create 
      'basicsettings' => array(
        'form id' => 'wallyflowtonode_basicsettings_form'
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
    'cache name' => wallyflowtonode_get_cache_name($flow_key),
  );
  
  // CASE A: we are at the begin of the wizard, if we have a row in DB, we should use it
  if (!$step || $step == 'basicsettings') {
    $result = db_query('SELECT * FROM {wallyflowtonode_flows} WHERE flow_key = %d', $form_state['cache name']);
    $flow_object = db_fetch_object($result);
    if ($flow_object) {
      $flow_item = $flow_object;
    }
  }

  // CASE B : either We are not in the beginning of the process either we are in the begining and there is no result in DB
  // => we should use the cached version ()
  if (!$flow_item) {
    $flow_item = wallyflowtonode_add_get_page_cache($form_state['cache name']);
  }
  // Remove pluginspecific step if no need
  if( isset($flow_item->plugin_name) && !wallytoolbox_has_settings_form_of_plugin(wallytoolbox_get_plugin_by_name($flow_item->plugin_name))) {
    unset($form_info['order']['pluginspecific']);
  }
  
  // CASE C : Blank case
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
  $form_state['redirect'] = 'admin/wally/wallyflowtonode';
  drupal_set_message('Operation Cancelled.');
}

/**
 * Callback for the proceed step
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
  dsm($form_state);
  //Some cleaning before save
  if (isset($flow_item->flow_key)) {
    drupal_set_message('The flow '.$flow_item->name.' has been edited');
    drupal_write_record('wallyflowtonode_flows', $flow_item, 'flow_key');
  }
  else {
    drupal_set_message('The flow '.$flow_item->name.' has been created');
    drupal_write_record('wallyflowtonode_flows', $flow_item);
  }
  // Clear the cache
  ctools_object_cache_clear('flow_item', $form_state['cache name']);
  
  $form_state['redirect'] = 'admin/wally/wallyflowtonode';
}

function wallyflowtonode_basicsettings_form(&$form, &$form_state) {
  $flow_item = &$form_state['flow_obj'];
  $flow_item = (array)$flow_item;
  if (!isset($flow_item) || empty($flow_item)) {
    $flow_item = (object)_wallyflowtonode_get_default_form_values();
  } else {
    $flow_item = (object)$flow_item;
  }
  while(is_string($flow_item->conf))
    $flow_item->conf = unserialize($flow_item->conf);
  while(is_string($flow_item->feeds_md5))
    $flow_item->feeds_md5 = unserialize($flow_item->feeds_md5);

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Flow name'),
    '#description' => t('The name you want to give to this feed.'),
    '#default_value' => $flow_item->name,
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
    '#weight' => 0,
  );
  
  $form['prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('External reference prefix.'),
    '#description' => t('Flow name by default.'),
    '#default_value' => $flow_item->prefix,
    '#size' => 80,
    '#maxlength' => 512,
    '#weight' => 0.05,
  );
  
  $form['uri'] = array(
    '#type' => 'textfield',
    '#title' => t('Flow "URI"'),
    '#description' => t('URI of the requested RSS feed.'),
    '#default_value' => $flow_item->uri,
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
    '#weight' => 0.1,
  );
  
  $form['publish'] = array(
    '#type' => 'radios',
    '#title' => t('Publishing comportement'),
    '#default_value' => $flow_item->publish,
    '#options' => array(t('Place nodes in a queue and choose which ones to publish'), t('Publish all nodes')),
    '#required' => TRUE,
    '#weight' => 0.15,
  );
  
  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Type of flow'),
    '#default_value' => $flow_item->types,
    '#required' => TRUE,
    '#options' => wallytoolbox_get_available_flow_type(),
    '#weight' => 0.11,
  );
  
  $body_html_options = array();
  $body_html_query = db_query("SELECT * FROM {filter_formats}");
  $i = 0;
  while ($body_html_element = db_fetch_array($body_html_query)) {
    $body_html_options[] = $body_html_element['name'];
    if ($body_html_element['name'] == $default_form_values['body_html'])
      $default_body_html = $i;
    $i++;
  }

  $form['body_html'] = array(
    '#type' => 'select',
    '#title' => t('Use HTML in the text body'),
    '#description' => t('Use HTML in the text body.'),
    '#options' => $body_html_options,
    '#default_value' => $flow_item->body_html,
    '#weight' => 0.2,
  );
  
  $minutes_list = array();
  for ($i=1; $i<=60; $i++) $minutes_list[$i] = $i;
  
  $form['delta_t'] = array(
    '#type' => 'select',
    '#title' => t('Time between updates'),
    '#description' => t('Time, in minutes, between updates for this feed.'),
    '#options' => $minutes_list,
    '#default_value' => $flow_item->delta_t,
    '#required' => TRUE,
    '#weight' => 1,
  );
  
  $all_vocabularies = taxonomy_get_vocabularies();
  foreach ($all_vocabularies as $vocabulary) {
    if ($vocabulary->name=='Document Type') {
      $form['package_layout'] = taxonomy_form($vocabulary->vid, $default_form_values['package_layout']);
      $form['package_layout']['#required'] = TRUE;
      $form['package_layout']['#weight'] = 4;
      $form['package_layout']['#default_value'] = $flow_item->package_layout;
      break;
    }
  }
  
  $default_destinations = explode('/', $flow_item->destination);
  module_load_include("inc",'content','includes/content.node_form');
  $destinations = content_fields('field_destinations');
  $destinations['widget']['default_value'][0]['tid'] = $default_destinations[0];
  $destinations['widget']['default_value'][0]['target'] = $default_destinations[1];
  $destinations['widget']['default_value'][0]['layout'] = $default_destinations[2];
  $destinations['widget']['default_value'][0]['rank'] = $default_destinations[3];
  $form['destinations'] = (array) content_field_form($form, $form_state, $destinations);
  $form['destinations']['#weight'] = 3;
  unset($form['destinations']['field_destinations'][1]);
  unset($form['destinations']['field_destinations']['field_destinations_add_more']);
  
  // probably important -- i'm continuing to investigate
  $form_state['no buttons'] = TRUE; 
}

/**
 * Basic settings validation
 */
function wallyflowtonode_basicsettings_form_validate(&$form, &$form_state) {
  $values = $form_state['values'];
  $uri_response = drupal_http_request($values['uri']);
  if ($uri_response->status_message != 'OK') {
    form_set_error('wallyflowtonode_uri', t('The URI doesn\'t respond.'));
  }
}

function wallyflowtonode_basicsettings_form_submit(&$form, &$form_state) {
  $submitted = $form_state['values'];
  $save_values = array('name', 'prefix', 'uri', 'publish', 'type', 'body_html', 'delta_t', 'package_layout'); 
  // maybe don't imitate this foreach
  foreach ($save_values as $value) {
    // set the values in the cache object -- it gets passed back to the next step
    // because of all that work we did in the form_info array
    $form_state['flow_obj']->$value = $submitted[$value];
  }
  
  if (empty($form_state['flow_obj']->prefix))
    $form_state['flow_obj']->prefix = $form_state['flow_obj']->name;
  
  $rank = strstr($submitted['field_destinations'][0]['rank'], 'DNC') ? substr($submitted['field_destinations'][0]['rank'], 3) : $submitted['field_destinations'][0]['rank'];
  $form_state['flow_obj']->destination = $submitted['field_destinations'][0]['tid'].'/'.$submitted['field_destinations'][0]['target'].'/'.$submitted['field_destinations'][0]['layout'].'/'.$rank;
}

function wallyflowtonode_pluginselector_form(&$form, &$form_state) {
  $flow_item = &$form_state['flow_obj'];
  $possible_plugins = wallytoolbox_get_plugins_that_handle_type($flow_item->type);
  
  foreach($possible_plugins as $plugin_name => $plugin){
    $options[$plugin_name] = isset($plugin['title']) ? $plugin['title'] : $plugin['name'];
    $dl = "<dt>".$options[$plugin_name]."</dt><dd>".$plugin['description']."</dd>";
  }

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
  // Remove pluginspecific step if no need
  if( isset($flow_item->plugin_name) && wallytoolbox_has_settings_form_of_plugin(wallytoolbox_get_plugin_by_name($flow_item->plugin_name))){
    $form_state['clicked_button']['#next']='pluginspecific';
  }
}

function wallyflowtonode_pluginspecific_form(&$form, &$form_state) {
  $flow_item = &$form_state['flow_obj'];
  $form['conf']=array('#tree'=>true);
  wallytoolbox_get_settings_form_of_plugin(wallytoolbox_get_plugin_by_name($flow_item->plugin_name),$form['conf'],$form_state['flow_obj']->conf);
  dsm($form, 'form');
  // probably important -- i'm continuing to investigate
  $form_state['no buttons'] = TRUE; 
}

function wallyflowtonode_pluginspecific_form_submit(&$form, &$form_state) {
  dsm($form_state, 'form state');
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
function wallyflowtonode_get_cache_name($flow_key){
  global $user;

  if($flow_key != 'new'){
    return $flow_key;
  }
  if ($user->uid) {
    $cache_name = $user->uid;
  }
  elseif (!isset($_SESSION['liste_cart_id'])) {
    $cache_name= $_SESSION['liste_cart_id'] = md5(uniqid(rand(), TRUE));
  }

  return 'wftn_'.$cache_name;
}

function _wallyflowtonode_get_default_form_values() {
  $default_form_values = new stdClass();
  
  $default_form_values->name = '';
  $default_form_values->prefix = '';
  $default_form_values->uri = '';
  $default_form_values->publish = 0;
  $default_form_values->delta_t = 5;
  $default_form_values->destination = '0///0';
  $default_form_values->package_layout = '';
  $default_form_values->body_html = t('No HTML');
  
  return $default_form_values;
}
