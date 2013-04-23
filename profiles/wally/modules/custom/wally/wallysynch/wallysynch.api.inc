<?php

/**
 * Implementation of hook_wallysynch_exporter()
 */
function hook_wallysynch_exporter(&$tabXml)
{
	//the array format to export
	$tabXml [] = array(
			'myModule' => array(
					'variables' => array(
							//all variables
							'variable1' => variable_get('variable1', 'defaultValue'),
							'variable2' => variable_get('variable2', 'defaultValue'),
					),
					'type'=> 'wally', //if it's a wally_variable
			),
	);
	
}
