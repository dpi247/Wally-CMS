<?php 
function ccimapping_form_configuration_form($form_state){
  $form=array();
  
  $form['ccimapping_ws_wsdluri']=array(
    '#type'=>'textfield',
  	'#title'=>t("CCI Web WSDL's UIR"),
    '#default_value' => variable_get('ccimapping_ws_wsdluri',''), 
  );
  $form['ccimapping_newsroom']=array(
    '#type'=>'textfield',
  	'#title'=>t("Newsroom"),
    '#default_value' => variable_get('ccimapping_newsroom',''), 
  
  );
  $form['ccimapping_product_name']=array(
    '#type'=>'textfield',
  	'#title'=>t("Product name"),
    '#default_value' => variable_get('ccimapping_product_name',''), 
  
  );
  
  return system_settings_form($form);
}