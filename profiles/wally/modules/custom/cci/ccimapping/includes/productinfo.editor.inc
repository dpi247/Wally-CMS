<?php
function ccimapping_form_productinfo_editor_form($form_state,$type=NULL){
  $form=array();
    

    $form['xml'] = array(
      '#type' => 'textarea',
      '#title' => t('XML Mapping'),
      '#default_value' => ccimap_build_productinfo($type),
    );
    
    $form['type'] = array(
      '#type' => 'value',
      '#weight'=>9,
      '#value' => $type,
    );
        $form['fieldset']=array(
      '#type'=>'fieldset',
      '#title'=>t("XML actuel"),
     '#collapsible' => TRUE,
     '#collapsed' => TRUE,
    );
    
     $form['fieldset']['actual_xml'] = array(
      '#type' => 'markup',
      '#weight'=>999,
      '#value' =>'<pre>'.htmlentities(ccimap_get_productinfo_fromWS($type)).'</pre>',
    );
    $form['send'] = array(
      '#type' => 'submit',
      '#name'=>'send',
      '#value' => t('Save & Send to CCI'),
    );
    
  

  return $form;
}
  
function ccimapping_form_productinfo_editor_form_submit($form,$form_state){
  if($form_state['clicked_button']["#name"]=="send"){
    
    $wsdl_uri=variable_get('ccimapping_ws_wsdluri','');
    $newsroom=variable_get('ccimapping_newsroom','');
    $product_name=variable_get('ccimapping_product_name','');
    
    $client = new SoapClient($wsdl_uri,array('features' => SOAP_SINGLE_ELEMENT_ARRAYS));
    
    $parameters_array=array("newsroom"=>$newsroom,'productname'=>$product_name,'xml'=>$form_state['values']['xml']);
    $parameters_object=(object) $parameters_array;
    try {
    $result = $client->setProductTemplate( $parameters_array);
    drupal_set_message('the productinfo has been sent to CCI');
    
    
  } catch (SoapFault $exception) {
    drupal_set_message("WebService Errror: \n FAULTSTRING:".$exception->faultstring."\n FAULTCODE:".$exception->faultcode,'error');
    dsm($exception);
  }
  }


  
}

/**
 * Call the CCI WS and ask for the XML definition of  the site structure (product info : destinations, target, layout)
 * 
 * 
 * @return the result of the call from the  WS: an XML structure in string format wich describe the packagetype
 */
function ccimap_get_productinfo_fromWS(){
 
  $wsdl_uri=variable_get('ccimapping_ws_wsdluri','');
  $newsroom=variable_get('ccimapping_newsroom','');
  $product_name=variable_get('ccimapping_product_name','');
    
  $client = new SoapClient($wsdl_uri,array('features' => SOAP_SINGLE_ELEMENT_ARRAYS));
        
  $parameters_array=array("newsroom"=>$newsroom,'productname'=>$product_name,'xml'=>$form_state['values']['xml']);
  try {
    
    $result = $client->getProductTemplate( array("newsroom"=>'CciNewsRoom','productname'=>'Wally News'));
    
  }catch (SoapFault $exception) {
    drupal_set_message("WebService Errror: \n FAULTSTRING:".$exception->faultstring."\n FAULTCODE:".$exception->faultcode,'error');
    dsm($exception);
  }
  return $result->return;
}


/**
 * Build an XML file corresponding the actual structure of the site. this XML should Be sent to the CCI WS
 * 
 * 
 * @return an XML in string format
 */
function ccimap_build_productinfo(){
  $hierarchies_array=wallyctools_get_hierarchies();
  return theme('ccimapping_hierarchies',$hierarchies_array);

}
