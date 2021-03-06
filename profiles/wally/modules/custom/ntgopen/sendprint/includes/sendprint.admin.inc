<?php 
/**
 * 
 * Form to send a print
 * @param array $form_state
 * @return array form
 */
function sendprint_page_sendtoprint_form($form_state) {
  $form = array();

  
  $form['sendprint_name'] = array(
    '#type' => 'textfield',
    '#title' => t('The name of the newspaper'),
    '#description' => t('The name of the newspaper will figure on the first page'),
    '#default_value' => wally_variable_get('sendprint_name', 'The Unfold'),
    '#required' => TRUE,
  );
  $form['sendprint_date'] = array(
    '#type' => 'textfield',
    '#title' => t('The edition date'),
    '#description' => t('The date of the edition'),
    '#default_value' => wally_variable_get('sendprint_date', t(date('j F Y'))),
    '#required' => TRUE,
  );
  $form['sendprint_front'] = array(
    '#type' => 'textfield',
    '#title' => t('The name of the first page'),
    '#description' => t('The name of the first page'),
    '#default_value' => wally_variable_get('sendprint_front', 'UNE'),
    '#required' => TRUE,
  );
  
  $form['sendtoprint'] = array(
    '#type' => 'submit',
    '#value' => t('Send to print'),
  );
  
  if (wally_variable_get('sendprint_customer_name', '') != "" and wally_variable_get('sendprint_customer_id', '') != ''){
  	$link = 'http://vm2030.cloud.seeweb.it:8080/index.jsp?customer='.wally_variable_get('sendprint_customer_id', '').'&customerName='.wally_variable_get('sendprint_customer_name', '');
  	$markup = l($link, $link).'<iframe src="'.$link.'" width="100%" height="500px"></iframe>';
  } else {
  	$markup = t('You need to define the CPS customer id and CPS customer Name to get access to the preview engine.');
  }
  
  $form['sendprint_link_to_render_engine'] = array(
  	'#type' => 'markup',
  	'#value' => $markup,
  	"#prefix"=>'<div class="form-item">',
  	"#suffix"=>'</div>',
  );
  return $form;
}
/**
 * 
 * Submit of the sendtoprint form
 * @param array $form
 * @param array $form_state
 */
function sendprint_page_sendtoprint_form_submit($form, $form_state) {
  wally_variable_set('sendprint_name', $form_state['values']['sendprint_name']);
  wally_variable_set('sendprint_date', $form_state['values']['sendprint_date']);
  wally_variable_set('sendprint_front', $form_state['values']['sendprint_front']);
  sendprint_send();
}
/**
 * Page settings form
 * */
function sendprint_page_settings_form(){
  $form['sendprint_export_filepath'] = array(
    '#type' => 'textfield',
    '#title' => t('Folder to put the zip before sending'),
    '#description' => t('The path of the folder where the zip will be stored before sending'),
    '#default_value' => wally_variable_get('sendprint_export_filepath', 'sites/default/files/sendprint/export'),
    '#required' => TRUE,
  );
  $form['sendprint_ssh_server'] = array(
    '#type' => 'textfield',
    '#title' => t('FTP Server'),
    '#description' => t('The server ssh'),
    '#default_value' => wally_variable_get('sendprint_ssh_server', ''),
    '#required' => TRUE,
  );
  $form['sendprint_ssh_name'] = array(
    '#type' => 'textfield',
    '#title' => t('FTP User'),
    '#description' => t('The name of the user ssh'),
    '#default_value' => wally_variable_get('sendprint_ssh_name', ''),
    '#required' => TRUE,
  );
  $form['sendprint_ssh_password'] = array(
    '#type' => 'password',
    '#title' => t('FTP Password'),
    '#description' => t('The password for ssh'),
    '#default_value' => wally_variable_get('sendprint_ssh_password', ''),
    '#required' => TRUE,
  );
  $form['sendprint_customer_id'] = array(
  	'#type' => 'textfield',
  	'#title' => t('Customer ID'),
  	'#description' => t('CPS Customer Name'),
  	'#default_value' => wally_variable_get('sendprint_customer_id', ''),
  	'#required' => TRUE,
  );

  $form['sendprint_customer_name'] = array(
  	'#type' => 'textfield',
  	'#title' => t('Customer Name'),
  	'#description' => t('CPS Customer Name'),
  	'#default_value' => wally_variable_get('sendprint_customer_name', ''),
  	'#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );  
  return $form;
}
/**
 * Validation of the page_settings_form
 * */
function sendprint_page_settings_form_validate($form, &$form_state){

  $path = $form_state['values']['sendprint_export_filepath'];
  $dir_exist = file_check_directory($path,0);	// IN D7 this function becomes file_prepare_directory()
  if (!$dir_exist) {
    form_set_error('sendprint_export_filepath', t('Source directory path : The path provided is not an existing writable directory.'));
  }
}
function sendprint_page_settings_form_submit($form, &$form_state){
  wally_variable_set('sendprint_ssh_server', $form_state['values']['sendprint_ssh_server']);
  wally_variable_set('sendprint_ssh_name', $form_state['values']['sendprint_ssh_name']);
  wally_variable_set('sendprint_ssh_password', $form_state['values']['sendprint_ssh_password']);
  wally_variable_set('sendprint_customer_id', $form_state['values']['sendprint_customer_id']);
  wally_variable_set('sendprint_customer_name', $form_state['values']['sendprint_customer_name']);
}
function sendprint_page_log(){
  $content  = '';
  $content .= drupal_get_form('sendprint_page_log_form');
  if (arg(3) !== NULL && is_numeric(arg(3))){
    $result = db_query("SELECT * FROM {sendprint_logs} WHERE sid = %d", arg(3));
    $log = db_fetch_object($result);
    $log_info = unserialize($log->log);
    $logs_array = array();
    foreach ($log_info as $tid => $redac){
      $term = taxonomy_get_term($tid);
      $logs_array[$term->name] = array();
      foreach ($redac as $redac_name => $nid){
        $logs_array[$term->name][$redac_name] = node_load(array('nid' => $nid));
      }
    }
    $content .= theme('sendprint_logs', $logs_array);
  }
  
  return $content;
}
function sendprint_page_log_form(){
  $options = array();
  $result = db_query("SELECT sid,date FROM {sendprint_logs} ORDER BY date DESC LIMIT 10");
  while ($obj = db_fetch_object($result)){
    $options[$obj->sid] = date('d/m/Y H:i', $obj->date);
  }
  $form['log'] = array(
    '#type' => 'select',
    '#title' => t('Pick a date to see the history'),
    '#options' => $options,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('See history'),
  );
  return $form;
}
function sendprint_page_log_form_submit($form, &$form_state){
  drupal_goto('admin/sendprint/log/'.$form_state['values']['log']);
}