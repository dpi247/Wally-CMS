<?php 
/**
 * 
 * wallysynch admin settings form submit
 * 
 * @param unknown_type $form_state
 * @return $form
 */
function wallysynch_admin_settings_form($form_state) {
	
	$options = wallysynch_retrieve_data();
	
	$form['wallysynch_add_environment'] = array(
			'#type'          => 'textfield',
			'#title'         => t('environment name'),
			'#description'   => t('to add an environment name'),
			'#size'          => 80,
			'#maxlength'     => 512,
	);
	
	$form['wallysynch_environment'] = array(
			'#type' 		=> 'checkboxes',
			'#title'		=> t('all environment'),
			'#options'		=> $options,
			
			);
	
	$form['add'] = array(
			'#type' => 'submit',
			'#value' => t('add'),
	);
	
	$form['del'] = array(
			'#type' => 'submit',
			'#value' => t('delete'),
	);
	return $form;
}

/**
 * 
 * wallysynch admin settings form validate
 * 
 * @param $form
 * @param $form_state
 */
function wallysynch_admin_settings_form_validate($form,$form_state){

	$environmentName = $form_state['values']['wallysynch_add_environment'];
	
	//if add submit button is pressed and the textfield environment name is empty
	if($form_state['clicked_button']['#value'] == 'add' && empty($environmentName)) 
	{
		form_set_error('wallysynch_content_refresh_time', t('The environment name is required.'));
	}

}

/**
 * 
 * wallysynch admin settings form submit
 * 
 * @param $form
 * @param $form_state
 */
function wallysynch_admin_settings_form_submit($form,$form_state){
	
	//if the add submit button is pressed
	if($form_state['clicked_button']['#value'] == 'add')
	{
		$str = $form_state['values']['wallysynch_add_environment'];
		wallysynch_add_data($str);
	}
	//else if the delete submit button is pressed
	else if($form_state['clicked_button']['#value'] == 'delete')
	{
		$environmentDel = array_filter($form_state['values']['wallysynch_environment']);
		wallysynch_delete_data($environmentDel);
	}
	
}
/**
 * 
 * this function retrieve all environment name
 * 
 * @return $environement
 * the tab of all environment name
 */
function wallysynch_retrieve_data(){
	
	//unserialize the environment array of the wally_variable_get and return it
	$environment = unserialize(wally_variable_get('wally_synch_environment', ''));
	return $environment;
}

/**
 * 
 * this function add a new environment name
 * 
 * @param $str
 * the environment name from the textfield
 */
function wallysynch_add_data($str){
	
	$environment = wallysynch_retrieve_data();
	$environment[$str] = $str;
	//serialize the environment array and put the result in a wally_variable
	wally_variable_set('wally_synch_environment', serialize($environment));
	drupal_set_message(t('The environment name has been saved'));
}

/**
 * 
 * this function remove one or many environment name
 * 
 * @param $environementDel
 * an array of all environment name to remove
 */
function wallysynch_delete_data($environmentDel){
	
	if(empty($environmentDel))
	{
		drupal_set_message(t('you must check minimum one'),'error');
	}
	else
	{
		$environment = wallysynch_retrieve_data();
	
		foreach ($environmentDel as $nameDel)
		{
			//search the environment name to remove in the environment name array
			 unset($environment[array_search($nameDel, $environment)]);
		}
		
		wally_variable_set('wally_synch_environment', serialize($environment));
		drupal_set_message(t('The environment name has been deleted'));
	}
}