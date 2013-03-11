<?php

/**
 * 
 * Import form
 */
function wallysynch_admin_import_form($form_state) {
	
	ctools_include('dependent');
	
	$form['#attributes'] = array('enctype' => "multipart/form-data");
	$form['config'] = array(
		'#type' => 'file',
		'#title' => t('Your configuration file'),
		'#size' => 40,
	);
	
	$form['crypted'] = array(
			'#type' => 'checkbox',
			'#title' => t('encrypted data ?'),
	);
	
	$form['wallysynch_decryption_key'] = array(
			'#type'          => 'textfield',
			'#title'         => t('decryption key'),
			'#description'   => t('to decrypt the config file'),
			'#size'          => 30,
			'#maxlength'     => 30,
			'#process' => array('ctools_dependent_process'),
  			'#dependency' => array('edit-crypted' => array(1)),
	);
	$form['submit'] = array(
			'#type' => 'submit',
			'#value' => t('import'),
	);
	return $form;
}

/**
 * 
 * Import form validate
 */
function wallysynch_admin_save_form_validate($form,$form_state){
	
	$crypted = $form_state['values']['crypted'];
	$valueKey = $form_state['values']['wallysynch_encryption_key'];
	
	//if crypted checkbox is checked and the textfield encryption key is empty
	if ($crypted && empty($valueKey)) 
	{
		form_set_error('wallysynch_content_refresh_time', t('The key for decryption is required.'));
	}
}

/**
 * 
 * Import form submit
 */
function wallysynch_admin_import_form_submit($form,$form_state) {
	
	//the path of the file
	$dest = '';
	$validators = array(
			'file_validate_extensions' => array("xml"),
			);
	
	$file = file_save_upload('config',$validators,$dest,FILE_EXISTS_REPLACE);
	$valueKey = $form_state['values']['wallysynch_decryption_key'];
	$isEncrypted = $form_state['values']['crypted'];
	
	//if no file has been uploaded
	if($file != NULL)
	{
		wallysynch_import_config($file,$valueKey,$isEncrypted);
	}
	else
	{
		drupal_set_message('WARNING	: you must uploaded an xml file','warning');
	}
}

/**
 * Import xml file that contain configuration
 * 
 *  @param $file
 *  the uploaded file.
 *  @param $key
 *  the encryption key.		
 *  @param $isEncrypted
 *  if the file is encrypted.
 */
function wallysynch_import_config($file,$key,$isEncrypted){
	
	wallysynch_set_configuration($file,$key,$isEncrypted);
	
}

/**
 * 
 * Set the configuration with the new configuration from xml file
 * 
 *  @param $file
 *  the uploaded file.
 *  @param $key
 *  the encryption key.		
 *  @param $isEncrypted
 *  if the file is encrypted.
 */
function wallysynch_set_configuration($file,$key,$isEncrypted){
	
	$xml = wallysynch_read_file($file,$key,$isEncrypted);

	//retrieve all data from the xml file and set variable or wally_variable
	if($xml!=NULL)
	{
		$typeSet = '';
		foreach($xml as $module)
		{
			foreach ($module as $type)
			{
				$typeSet = '';
				$typeSet = (string)$type;
			}
			foreach($module as $variables)
			{
				foreach($variables as $var => $values)
				{
					//if type of set is wally_variables_set
					if($typeSet == 'wally')
					{
						//if value is different of empty then set variable
						if((string)$values != '')
						{
							wally_variable_set($var,(string)$values);
						}
					}
					else if($typeSet == '')
					{
						if((string)$values != '')
						{
							variable_set($var,(string)$values);
						}
					}

				}
			}
		}
	
		drupal_set_message('Configuration has been imported with success');
	}
	else
	{
		drupal_set_message('ERROR : Configuration has not imported the key is not good','error');
	}
}


/**
 * decrypt data
 *
 * @param $data
 * the encrypted data.
 * @param $key
 * the encrypt key.
 * @param unknown_type $td
 * the encrypt algo.
 * @param $iv
 * the temp.
 * @return string
 */
function wallysynch_decryption($data,$key,$td,$iv){
	
	mcrypt_generic_init($td, $key, $iv);
	
	//decrypt the data with the mcrypt library
	$decrypted = mdecrypt_generic($td, $data);
	
	mcrypt_generic_deinit($td);
	mcrypt_module_close($td);

	return $decrypted;
}

/**
 * 
 * Read the xml file and extract data
 * 
 *  @param $file
 *  the uploaded file.
 *  @param $key
 *  the encryption key.		
 *  @param $isEncrypted
 *  if the file is encrypted.
 * @return SimpleXMLElement
 * the xml.
 */
function wallysynch_read_file($file,$key,$isEncrypted){
	
	//choose the decryption algo
	$td = mcrypt_module_open('tripledes', '', 'ecb', '');
	
	$iv = mcrypt_create_iv(mcrypt_enc_get_iv_size($td), MCRYPT_RAND);
	
	if(file_exists($file->filepath))
	{
		//read the config.xml file
		$FileName = $file->filepath;
		$FileHandle = fopen($FileName, 'r');
		
		$data = fread($FileHandle, filesize($FileName));
		fclose($FileHandle);
			
		//if is encrypted decrypt data from the file
		if($isEncrypted)
		{
			$data = wallysynch_decryption($data,$key,$td,$iv);
		}
		//create xml from string
		$xml = simplexml_load_string($data);
	}
	else
	{
		drupal_set_message('ERROR : Configuration has not imported','error');
	}
	unlink($FileName);
	return $xml;
}