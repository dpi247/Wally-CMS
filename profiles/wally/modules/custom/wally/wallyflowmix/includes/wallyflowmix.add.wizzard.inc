<?php

/**
 * Get the cached changes to a given task handler.
* (Earl wrote that, not me...)
 */
function wallyflowmix_add_get_page_cache($name) {
  $cache = ctools_object_cache_get('flowmix_item', $name);
  return $cache;
}


/**
 * Callback generated when the 'cancel' button is clicked.
 *
 * All we do here is clear the cache. 
* redirect them to where they started 
* and call them a coward
 */
function wallyflowmix_subtask_addnewflow_cancel(&$form_state) {
  ctools_object_cache_clear('flowmix_item', $form_state['cache name']);
  $form_state['redirect'] = 'admin/wally/wallyflowmix/adminflow';
  drupal_set_message('Operation Cancelled.');
}
/**
 * Callback for the proceed step
 *
 */
function wallyflowmix_subtask_addnewflow_next(&$form_state) {
  $flowmix_item = &$form_state['flow_obj'];
  if($form_state['step']=='summary'){
    $flowmix_item->current_op=$form_state['clicked_button'];
    $flowmix_item->index=(int)str_replace('next_','',$form_state['clicked_button']['#name']);;
  }
  $cache = ctools_object_cache_set('flowmix_item', $form_state['cache name'], $flowmix_item);
}

/**
 * Callback generated when the 'finish' button is clicked.
 *
 * Need to handle the flow creation 
 */
function wallyflowmix_subtask_addnewflow_finish(&$form_state) {
  $flowmix_item = &$form_state['flow_obj'];
  /*
  
  //Some cleaning before save
  $flowmix_item->conf=serialize($flowmix_item->conf);
  if(isset($flowmix_item->flow_key)){
    
    drupal_set_message('The flow '.$flowmix_item->name.' has been edited');
    drupal_write_record('wallyflowmix_flows',$flowmix_item,'flow_key');
  }
  else{
    drupal_set_message('The flow '.$flowmix_item->name.' has been created');
    drupal_write_record('wallyflowmix_flows',$flowmix_item);
  }
  // Clear the cache
  ctools_object_cache_clear('flowmix_item', $form_state['cache name']);
  
  $form_state['redirect'] = 'admin/wally/wallyflowmix/adminflow/edit/'.$form_state['cache name'];
*/
}

function wallyflowmix_page_admin_add_wizzard(){

  $flow_id= arg(5);
  $step = arg(6);
  // required includes for wizard
  ctools_include('wizard');
  ctools_include('object-cache');
  
  // *** SETUP ARRAY multistep setup **** 
 // these are defined in some docs at end of article
 
  $form_info = array(
    'id' => 'wallyflowmix_add',
    'path' => "admin/wally/wallyflowmix/adminflow/edit/$flow_id/%step",
    
    'show trail' => TRUE,
    'show back' => TRUE,
    'show cancel' => true,
    'show return' =>false,
    'next text' => 'Next',
     // 'free trail' => TRUE, see http://drupal.org/node/663386
   
  
    'next callback' =>  'wallyflowmix_subtask_addnewflow_next',
    'finish callback' => 'wallyflowmix_subtask_addnewflow_finish',
    'return callback' => 'wallyflowmix_subtask_addnewflow_finish',
    'cancel callback' => 'wallyflowmix_subtask_addnewflow_cancel',
     
     // this controls order, as well as form labels
    'order' => array(
      'summary' => t('Create Flow 1/3: Basic settings'),
      'basicsetttings' => t('Create Flow 1/3: Basic settings'),
  	  'pluginselector' => t('Create Flow 2/3: Plugin Selector'),
      'pluginspecific' => t('Create Flow 2bis/3: Plugin specific settings'),
      'finalstep' => t('Create Flow 3/3:  Final Step'),
  ),
   // here we map a step to a form id.
    'forms' => array(
      // e.g. this for the step at wombat/create 
      'summary' => array(
        'form id' => 'wallyflowmix_formstep_summary_form'
      ),
      'basicsetttings' => array(
        'form id' => 'wallyflowmix_formstep_basicsetttings_form'
      ),
      'pluginselector' => array(
        'form id' => 'wallyflowmix_formstep_pluginselector_form'
      ),
      'pluginspecific' => array(
        'form id' => 'wallyflowmix_formstep_pluginspecific_form'
      ),
      'finalstep' => array(
        'form id' => 'wallyflowmix_formstep_finalstep_form'
      ),
      ),
  );
  
  
  // *** SETTING THE FORM UP FOR MULTISTEP *** //
  $form_state = array(
    'cache name' =>wallyflowmix_get_cache_name($flow_id),
  );
  
  $flowmix_item=wallyflowmix_create_or_load_flowmix_item($cache_name,$step);
  
  //THIS IS WHERE WILL STORE ALL FORM DATA
  $form_state['flow_obj'] = $flowmix_item;
  

  // REmove pluginspecific step if no need
  if(!$step){
    $form_info['next text'] = 'Add a flow';
  }
  if( isset($flowmix_item->plugin_name) && !test_has_settings_form_of_plugin(test_get_plugin_by_name($flowmix_item->plugin_name))){
    unset( $form_info['order']['pluginspecific']);
  }
  
  
  
  
  
  
  // and this is the witchcraft that makes it work
  $output = ctools_wizard_multistep_form($form_info, $step, $form_state);
  
  
  
  return $output;
}

function wallyflowmix_create_or_load_flowmix_item($cache_name,$step){

  //CASE A: we are at the begin of the wizard, if we have a row in DB, we should use it
  if(!$step){
    $result=db_query('SELECT * FROM {wallyflowmix_flows} WHERE flow_key=%d',$cache_name);
    $fff=db_fetch_object($result);
    if($fff){
      $flowmix_item=$fff;
    }
    ctools_object_cache_set('flowmix_item', $cache_name, $flowmix_item);
    
  }
  //CASE B : either We are not in the begenning of the process either we are in the begining and there is no result in DB
  // => we should use the cached version ()
  if (!$flowmix_item) {
  $flowmix_item = wallyflowmix_add_get_page_cache($cache_name);
  } 
  //CASE C : Blank case
  if (!$flowmix_item) {
    // set form to first step -- we have no data
      $step = current(array_keys($form_info['order']));
    //try to fetch flow from dtabase 
      $flowmix_item = new stdClass();
      $flowmix_item->index=0;
    
    // ** set the storage object so its ready for whatever comes next
    ctools_object_cache_set('flowmix_item', $cache_name, $flowmix_item);
  }
  
  
  return $flowmix_item;
  
  
}
function wallyflowmix_formstep_summary_form(&$form, &$form_state) {
  $flowmix_item = &$form_state['flow_obj'];
  dsm($flowmix_item);
   $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('RSS feed name'),
    '#description' => t('The name you want to give to this feed.'),
    '#default_value' => $flowmix_item->name,
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
    '#weight' => 0,
  );
  
  $form['maxitem_global'] = array (
    '#type'             => 'textfield',
    '#title'            => t('Max item displayed in the final mixed feed'),
    '#default_value'    => $flowmix_item->maxitem_global,
    '#required'         => TRUE,
  );
  
  $form['maxitem_byfeed'] = array (
    '#type'             => 'textfield',
    '#title'            => t('Max item to fetch by feed'),
    '#default_value'    => $flowmix_item->maxitem_byfeed,
    '#required'         => TRUE,
  );
  
  $mix_schemas = array(
    'alt'               => 'Alternate',
    'any'               => 'Most Recent'
  );
  
  $form['mix_schema'] = array(
    '#type'             => 'select',
    '#title'            => t('Schema used for mixing the feeds'),
    '#default_value'    => $flowmix_item->mix_schema,
    '#options'          => $mix_schemas,
  );
  
  $form['own_schema'] = array (
    '#type'             => 'textfield',
    '#title'            => t('Define your own schema used for mixing the feeds'),
    '#description'      => t('Examples:
        1,2,3 / 1-2,3,2,1-3,2,2-3 / any,2,1-2,3,any,2 / ... see <a href="http://rossel.audaxis.com/xwiki/wiki/drupal/view/Main/User_RSS_mix" target="_new">RSS mix documentation</a>.
      '),
    '#default_value'    => $flowmix_item->own_schema,
    '#required'         => FALSE,
  );
  
  $form['get_distant_img'] = array (
    '#type'             => 'checkbox',
    '#default_value'    => $flowmix_item->get_distant_img,
    '#title'            => 'Copy RSS feeds images to locale folder.',
  );

  $form['override_theme'] = array (
    '#type'             => 'checkbox',
    '#default_value'    => $flowmix_item->override_theme,
    '#id'               =>  'override-theme-checkbox',
    '#title'            => 'Override Theming Function',
  );

  $form['override_theme_text'] = array (
    '#type'             => 'textfield',
    '#default_value'    => $flowmix_item->override_theme_text,
    '#id'               => 'override-theme-textfield',
    '#dependency'       => array('override-theme-checkbox'=>array('1')),
    '#dependency_type'  => 'disable',
    '#process'          => array('ctools_dependent_process'),
    '#title'            => 'Theming Function Name',
  );
  
  
  
  
    unset($form['buttons']['cancel']);
    if(count($flowmix_item->flows)){
      $form['buttons']['save'] = array(
      '#type'=>'submit',
      '#value'=>'Save',
      '#wizard type'=>'finish',
      '#name'=>'save',
      );   
    }
    $form['buttons']['add_flow'] = array(
    '#type'=>'submit',
    '#value'=>'Add a new flow',
    '#wizard type'=>'next',
    '#name'=>'next_'.count($flowmix_item->flows),
    '#next'=>'basicsetttings'
    ); 
    
  
}

function wallyflowmix_formstep_summary_form_submit(&$form, &$form_state) {
  $flowmix_item = &$form_state['flow_obj'];

  $submitted = $form_state['values'];
  $save_values = array('name','maxitem_global','maxitem_byfeed','mix_schema','own_schema','get_distant_img','override_theme','override_theme_text'); 
  foreach($save_values as $value) {
    // set the values in the cache object -- it gets passed back to the next step
   // because of all that work we did in the form_info array
    $form_state['flow_obj']->$value = $submitted[$value];
    
  }
  
}

function wallyflowmix_formstep_basicsetttings_form(&$form, &$form_state) {
  
  $flowmix_item = &$form_state['flow_obj'];
   $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('RSS feed name'),
    '#description' => t('The name you want to give to this feed.'),
    '#default_value' => $flowmix_item->flows[(int)$flowmix_item->index]->name,
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
    '#weight' => 0,
  );
  
  $form['uri'] = array(
    '#type' => 'textfield',
    '#title' => t('RSS feed URI'),
    '#description' => t('URI of the requested RSS feed.'),
    '#default_value' => $flowmix_item->flows[(int)$flowmix_item->index]->uri,
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
    '#weight' => 0.1,
  );
  
  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Type of flow'),
    '#default_value' => $flowmix_item->flows[(int)$flowmix_item->index]->types,
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
    '#default_value' => $flowmix_item->flows[(int)$flowmix_item->index]->bodyhtml,
      '#weight' => 0.2,
  );
  
  $minutes_list = array();
  for ($i=1; $i<=60; $i++) $minutes_list[$i] = $i;
  
  $form['deltat'] = array(
    '#type' => 'select',
    '#title' => t('Time between updates'),
    '#description' => t('Time, in minutes, between updates for this feed.'),
    '#options' => $minutes_list,
    '#default_value' => $flowmix_item->flows[(int)$flowmix_item->index]->deltat,
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
function wallyflowmix_formstep_basicsetttings_form_validate(&$form, &$form_state) {
  $uri_response = drupal_http_request($form['wallyflowmix_uri']['#value']);
  if ($uri_response->status_message != 'OK') {
  //  form_set_error('wallyflowmix_uri', t('The URI doesn\'t respond.'));
  }
}
function wallyflowmix_formstep_basicsetttings_form_submit(&$form, &$form_state) {
  $flowmix_item = &$form_state['flow_obj'];
  $submitted = $form_state['values'];
  $save_values = array('name', 'uri', 'type','bodyhtml','deltat'); 
  // maybe don't imitate this foreach
  $obj=new stdClass();
  foreach($save_values as $value) {
    // set the values in the cache object -- it gets passed back to the next step
   // because of all that work we did in the form_info array
    $obj->$value = $submitted[$value];
    
  }
  $form_state['flow_obj']->flows[(int)$flowmix_item->index] =$obj;
  
}


function wallyflowmix_formstep_pluginselector_form(&$form, &$form_state) {
  
  $flowmix_item = &$form_state['flow_obj'];
  $possible_plugins=test_get_plugins_that_handle_type($flowmix_item->flows[$flowmix_item->index]->type);
  foreach($possible_plugins as $plugin_name=>$plugin){
    $options[$plugin_name]=$plugin['name'];
    $dl="<dt>".$plugin['name']."</dt><dd>".$plugin['description']."</dd>";
  }
  $dl="<dl>".$dl."</dl>";
  $form['plugin_name'] = array(
    '#type' => 'radios',
    '#tree' => true,
    '#options' => $options,
    '#title' => 'Select A plugin',
    '#default_value' => $flowmix_item->flows[(int)$flowmix_item->index]->plugin_name,
    '#description' => $dl,
  '#required' => 1,
  );
  
  
  $form_state['no buttons'] = TRUE; 
}

function wallyflowmix_formstep_pluginselector_form_submit(&$form, &$form_state) {
  $flowmix_item = &$form_state['flow_obj'];
  $submitted = $form_state['values'];
  $save_values = array('plugin_name'); 
  foreach($save_values as $value) {
    // set the values in the cache object -- it gets passed back to the next step
   // because of all that work we did in the form_info array
    $form_state['flow_obj']->flows[$flowmix_item->index]->$value = $submitted[$value];
    
  }
  
 // $flowmix_item= $form_state['flow_obj']->flows[$flowmix_item->index];
  // Remove pluginspecific step if no need
  if( isset($flowmix_item->plugin_name) && test_has_settings_form_of_plugin(test_get_plugin_by_name($flowmix_item->plugin_name))){
  $form_state['clicked_button']['#next']='pluginspecific';
  }
  
}

function wallyflowmix_formstep_pluginspecific_form(&$form, &$form_state) {
  $flowmix_item = &$form_state['flow_obj'];
  $form['conf']=array('#tree'=>true);
  test_get_settings_form_of_plugin(test_get_plugin_by_name($flowmix_item->flows[(int)$flowmix_item->index]->plugin_name),$form['conf'],$form_state['flow_obj']->conf);
    
  // probably important -- i'm continuing to investigate
  $form_state['no buttons'] = TRUE; 
}
function wallyflowmix_formstep_pluginspecific_form_submit(&$form, &$form_state) {
  $conf = $form_state['values']['conf'];
  $form_state['flow_obj']->flows[(int)$flowmix_item->index]->conf = $conf;
    
  
}

function wallyflowmix_formstep_finalstep_form(&$form, &$form_state) {
  $flowmix_item = &$form_state['flow_obj'];
  $form['direct_process'] = array(
    '#type' => 'checkbox',
    '#required' => 1,
    '#title' => 'Direct process',
    '#default_value' => $form_state['flow_obj']->flows[(int)$flowmix_item->index]->direct_process,
  );
  unset($form['buttons']['return']);
  
 $form['buttons']['Ok'] = array(
    '#type'=>'submit',
    '#value'=>'Ok',
    '#wizard type'=>'next',
    '#name'=>'next_'.max(0,count($flowmix_item->flows)-1),
    '#next'=>'basicsetttings'
    );   
  // probably important -- i'm continuing to investigate
  $form_state['no buttons'] = TRUE; 
}

function wallyflowmix_formstep_finalstep_form_submit(&$form, &$form_state) {
  $form_state['clicked_button']['#next']='summary';
   
}

/**
 * Return the unique cart_id of the user.
 */
function wallyflowmix_get_cache_name($flow_id){
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