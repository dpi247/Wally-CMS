<?php 
/**
 *
 * Save form.
 */
function wallysynch_admin_save_form($form_state) {

  ctools_include('dependent');

  $options = unserialize(wally_variable_get('wally_synch_environment', ''));

  if(wally_variable_get('wallysynch_settings', '')!='') {
  		$path = wally_variable_get('wallysynch_settings', '');
  } else {
    $path = $_SERVER[DOCUMENT_ROOT];
  }

  $form['wallysynch_admin'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Export file config directory path'),
    '#description'   => t('the directory path for the xml config file.'),
    '#size'          => 150,
    '#maxlength'     => 150,
    '#required'      => TRUE,
    '#default_value' => $path,
  );

  $form['var'] = array(
    '#type' => 'checkbox',
    '#title' => t('add variables ?'),
  );

  $form['additional_var'] = array(
    '#type' => 'textarea',
    '#title' => t('additional variables'),
    '#description'   => t('add variables'),
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-var' => array(1)),
  );

  $form['additional_var_wally'] = array(
    '#type' => 'textarea',
    '#title' => t('additional wally variables'),
    '#description'   => t('add wally variables'),
    '#process' => array('ctools_dependent_process'),
    '#dependency' => array('edit-var' => array(1)),
  );

  $form['conf_type'] = array(
    '#type' 	=> 'select',
    '#title'	=> t('the type of configuration'),
    '#options'	=> $options,
  );

  if (function_exists('mcrypt_module_open')) {
    $form['crypted'] = array(
      '#type' => 'checkbox',
      '#title' => t('encrypt data ?'),
    );

    $form['wallysynch_encryption_key'] = array(
      '#type'          => 'textfield',
      '#title'         => t('encryption key'),
      '#description'   => t('to encrypt the config file'),
      '#size'          => 80,
      '#maxlength'     => 512,
      '#process' => array('ctools_dependent_process'),
      '#dependency' => array('edit-crypted' => array(1)),
    );
  }

  $form['submit'] = array(
  		'#type' => 'submit',
  		'#value' => t('Save'),
  );

  return $form;
}

/**
 *
 * Save form validate
 */
function wallysynch_admin_save_form_validate($form,$form_state){
  
	$path = $form_state['values']['wallysynch_admin'];
	//if the path is good
	if(!file_exists($path))
	{
		form_set_error('wallysynch_file_exists', t('Configuration has not saved the path is not good: %s',array('%s'=>$path)));
	}
	//else if the path is writable
	else if(!is_writable($path))
	{
		form_set_error('wallysynch_is_writable', t('The directory path is not writable !'));
	}
	
	if (isset($form_state['values']['crypted'])) {
    	$crypted = $form_state['values']['crypted'];
    	$valueKey = $form_state['values']['wallysynch_encryption_key'];

    	//if the data are crypted and the encryption key is empty
    	if ($crypted && empty($valueKey)) {
      		form_set_error('wallysynch_content_refresh_time', t('The key for encryption is required.'));
    	}
  	}
}

/**
 *
 * Save form submit
 */
function wallysynch_admin_save_form_submit($form,$form_state) {

  $valuePath = $form_state['values']['wallysynch_admin'];
  $valueKey = $form_state['values']['wallysynch_encryption_key'];

  $isEncrypted = isset($form_state['values']['crypted']) ? $form_state['values']['crypted'] : FALSE;

  $confType = $form_state['values']['conf_type'];
  //additional variables
  $addVar = $form_state['values']['additional_var'];
  $addWallyVar = $form_state['values']['additional_var_wally'];
  $addVar = trim($addVar);
  $addWallyVar = trim($addWallyVar);

  wally_variable_set('wallysynch_settings', $valuePath);

  if($addVar == '' && $addWallyVar == '')
  {
    wallysynch_save_config($valueKey, $isEncrypted, $confType);
  }
  else
  {
    $externeTab = wallysynch_cut_variables($addVar, $addWallyVar);
    wallysynch_save_config($valueKey, $isEncrypted, $confType, $externeTab);
  }
}

/**
 *
 * this function save the config in a xml file
 *
 *	@param $key
 *	the encryption key.
 */
function wallysynch_save_config($key, $isEncrypted, $confType, $externTab = NULL){
  $path = wally_variable_get('wallysynch_settings', '');

  $data = wallysynch_create_xml($confType, $externTab);

  if($isEncrypted)
  {
    //choose the encryption algo
    $td = mcrypt_module_open('tripledes', '', 'ecb', '');
     
    $iv = mcrypt_create_iv(mcrypt_enc_get_iv_size($td), MCRYPT_RAND);
     
    $data = wallysynch_encryption($data, $key, $td, $iv);
  }

  wallysynch_save_file($data, $path);
}

/**
 *
 * this function encrypt data
 *
 * @param $data
 * the data to encrypt.
 * @param $key
 * the encrypt key.
 * @param unknown_type $td
 * the encrypt algo.
 * @param $iv
 * the temp.
 * @return string
 */
function wallysynch_encryption($data,$key,$td,$iv){

  mcrypt_generic_init($td, $key, $iv);

  //encrypt data with the mcrypt library
  $encrypted_data = mcrypt_generic($td, $data);

  mcrypt_generic_deinit($td);
  mcrypt_module_close($td);

  return $encrypted_data;
}

/**
 *
 * this function create the array
 *
 * @return $tab
 * the array with for each module the variables and the query
 */
function wallysynch_create_export_tab($ExternTab = NULL){

  //apachesolr module
  $tabXml [] = array(
    'apachesolr' => array(
      'variables' => array(
        'apachesolr_host' => variable_get('apachesolr_host', 'localhost')
      ),
      'type'=> '',
    ),
  );
  //warnish module
  $tabXml [] = array(
    'warnish' => array(
      'variables' => array(
        'warnish_control_terminal' => wally_variable_get('warnish_control_terminal', '127.0.0.1:6082'),
        'warnish_control_key' => wally_variable_get('warnish_control_key', ''),
      ),
      'type'=> 'wally',
    ),
  );
  //cci module
  $tabXml [] = array(
    'cci' => array(
      'variables' => array(
        'ccimapping_ws_wsdluri' => wally_variable_get('ccimapping_ws_wsdluri','')
      ),
      'type'=> 'wally',
    ),
  );
  //wallymport module
  $tabXml [] = array(
    'wallymport' => array(
      'variables' => array(
        'wallymport_productID' => variable_get('wallymport_productID',$server_name)
      ),
      'type'=> '',
    ),
  );
  //pub module
  $tabXml [] = array(
    'pub' => array(
      'variables' => array(
        'pub_webserv' => variable_get('pub_webserv', 'http://172.21.132.180:8080/Wally-Tags/rest/wally/script/detail'),
        'pub_purl_header' => variable_get('pub_purl_header', 'http://172.21.132.180:8080/Wally-Tags/rest/wally/script/header'),
        'pub_openx_webserv' => variable_get('pub_openx_webserv', 'http://172.21.132.180:8080/Wally-Tags/rest/wally/script/detail')
      ),
      'type'=> '',
    ),
  );


  //invoke all module who implement the hook_wallysynch_exporter()
  module_invoke_all('wallysynch_exporter',$tabXml);

  if($ExternTab != NULL)
  {
    $result = array_merge($tabXml,$ExternTab);
    return $result;
  }
  else
  {
    return $tabXml;
  }
}

/**
 *
 * this function create the xml
 *
 * @return $data
 * return xml data
 */
function wallysynch_create_xml($confType, $externTab = NULL){

  $tab = wallysynch_create_export_tab($externTab);

  //the xml declaration
  $xml = new SimpleXMLElement("<?xml version=\"1.0\" encoding=\"UTF-8\"?><wallysynch configuration='".$confType."'></wallysynch>");

  //call this function to create xml with an array
  wallysynch_array_to_xml($tab,$xml);

  //to give a good format to the xml with carrige return
  $dom = dom_import_simplexml($xml)->ownerDocument;

  //for a better visual output format
  $dom->formatOutput = true;

  return $data = $dom->saveXML();
}

/**
 *
 * this is a recurcive function that transform a php array in xml data
 *
 * @param $tab
 * a php array
 * @param &$xml
 * the xml result
 */
function wallysynch_array_to_xml($tab, &$xml) {

  foreach($tab as $key => $value) {

    if(is_array($value))
    {
      if(!is_numeric($key))
      {
        $subnode = $xml->addChild("$key");
        wallysynch_array_to_xml($value, $subnode);
      }
      else
      {
        wallysynch_array_to_xml($value, $xml);
      }
    }
    else
    {
      $xml->addChild("$key","$value");
    }
  }
}

/**
 *
 * this function Save the xml configuration file
 *
 * @param $data
 * the data encrypted or not
 * @param $path
 * the path where it save the file
 */
function wallysynch_save_file($data,$path){

  if(file_exists($path))
  {
    //write the config.xml file
    $file = 'config.xml';
    $FileName = $path.'/'.$file;
    $FileHandle = fopen($FileName, 'w');

    fwrite($FileHandle, $data);
    fclose($FileHandle);
	//dsm(file_create_filename($file, $path));
    drupal_set_message(t('Configuration has been saved in %s',array('%s'=>$path)));
    drupal_set_message(t('Your configuration file is accessible with ftp in %s',array('%s'=>$path)));
    
  }
  else
  {
    drupal_set_message(t('ERROR : Configuration has not saved the path is not good: %s',array('%s'=>$path)),'error');
  }
}

/**
 *
 * this function cut the string from the textarea and return an array
 *
 * @param $addVarsars
 * the string from variables textarea.
 * @param $addWallyVars
 * the string from wally variables textarea.
 * @return $tabXml
 */
function wallysynch_cut_variables($addVars,$addWallyVars){

  if(!$addVars == '')
  {
    //cut string by line
    $tabLineVars = preg_split('/\R/',$addVars);
    $resultTabVar = wallysynch_add_variables($tabLineVars);
    $tabXml = $resultTabVar;
  }

  if(!$addWallyVars == '')
  {
    //cut the string by line
    $tabLineWallyVars = preg_split('/\R/',$addWallyVars);
    $resultTabWallyVar = wallysynch_add_wally_variables($tabLineWallyVars);
    $tabXml = $resultTabWallyVar;
  }

  if(!$addVars == '' && !$addWallyVars == '')
  {
    $tabXml = array_merge($resultTabVar,$resultTabWallyVar);
  }
  return $tabXml;

}

/**
 *
 * this function create an array with additional variables
 *
 * @param $tabLineVars
 * the array of variables cut up by the wallysynch_cut_variables function
 * @return $tabXmlvars
 */
function wallysynch_add_variables($tabLineVars){

  for($i=0;$i<count($tabLineVars);$i++)
  {
    $varName = trim($tabLineVars[$i]);
    $var['variables'][$varName] = variable_get($varName,'');
  }

  $tabXmlvars [] = array(
    'custom' => array(
      $var,
      'type' => '',

    ),
  );

  return $tabXmlvars;
}

/**
 *
 * this function create an array with additional variables
 *
 * @param $tabLineWallyVars
 * the array of wally variables cut up by the wallysynch_cut_variables function
 * @return $tabXmlWally
 */
function wallysynch_add_wally_variables($tabLineWallyVars)
{

  for($i=0;$i<count($tabLineWallyVars);$i++)
  {
    $varName = trim($tabLineWallyVars[$i]);
    $var['variables'][$varName] = wally_variable_get($varName,'');
  }

  $tabXmlWally [] = array(
    'customWally' => array(
      $var,
      'type' => 'wally',

    ),
  );
  return $tabXmlWally;

}