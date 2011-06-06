<?php
function ccimapping_form_packagetype_editor_form($form_state,$type=NULL){
  $form=array();
  if($type){
    

    $form['xml'] = array(
      '#type' => 'textarea',
      '#title' => t('XML Mapping'),
      '#default_value' => ccimap_get_packagetype_fromWS($type),
    );
    $form['type'] = array(
      '#type' => 'value',
      '#value' => $type,
    );
    
   /* $form['save'] = array(
      '#type' => 'submit',
      '#name'=>'save',
      '#value' => t('Save'),
    );
    */
    $form['send'] = array(
      '#type' => 'submit',
      '#name'=>'send',
      '#value' => t('Save & Send to CCI'),
    );
    
  
  }
  else{
    $form['type_selector']=array(
      '#type'=>'markup',
      '#value'=>theme('ccimapping_typeselector'),
    );
   
  } 
  return $form;
}
  
function ccimapping_form_packagetype_editor_form_submit($form,$form_state){
  if($form_state['clicked_button']["#name"]=="send"){
    
    $wsdl_uri=variable_get('ccimapping_ws_wsdluri');
    $newsroom=variable_get('ccimapping_newsroom');
    $product_name=variable_get('ccimapping_product_name');
    
    $client = new SoapClient($wsdl_uri,array('features' => SOAP_SINGLE_ELEMENT_ARRAYS));
    
    $parameters_array=array("newsroom"=>$newsroom,'productname'=>$product_name,'xml'=>$form_state['values']['xml']);
    $parameters_object=(object) $parameters_array;
    try {
    $result = $client->setPackageType( $parameters_array);
    drupal_set_message('the packageType has been sent to CCI');
    
    
  } catch (SoapFault $exception) {
    drupal_set_message("WebService Errror: \n FAULTSTRING:".$exception->faultstring."\n FAULTCODE:".$exception->faultcode,'error');
    dsm($exception);
  }
  }


  
}

function ccimap_get_packagetype_fromWS($type){
 
 $wsdl_uri=variable_get('ccimapping_ws_wsdluri','');
    $newsroom=variable_get('ccimapping_newsroom','');
    $product_name=variable_get('ccimapping_product_name','');
    
    $client = new SoapClient($wsdl_uri,array('features' => SOAP_SINGLE_ELEMENT_ARRAYS));
        
    try {
    
    $result = $client->getPackageType( array("newsroom"=>$newsroom,'productname'=>$product_name));
    
  } catch (SoapFault $exception) {
    drupal_set_message("WebService Errror: \n FAULTSTRING:".$exception->faultstring."\n FAULTCODE:".$exception->faultcode,'error');
    dsm($exception);
  }
  
  return $result->return;
}





/*
function ccimap_get_object($type){
  $query=db_query("Select * FROM {cci_packagetype} WHERE type='%s'",$type);
  return db_fetch_array($query);
}

*/