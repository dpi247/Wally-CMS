<?php
function ccimapping_form_packagetype_editor_form($form_state,$wally_type=NULL){
  $form=array();
  if($wally_type){
    

    $form['xml'] = array(
      '#type' => 'textarea',
      '#title' => t('XML Mapping'),
      '#default_value' => ccimap_get_specific_type(ccimap_get_packagetype_fromWS($wally_type),$wally_type),
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
      '#value'=>theme("ccimapping_typeselector","productinfo"),
    );
   
  } 
  return $form;
}
  
function ccimapping_form_packagetype_editor_form_submit($form,$form_state){
  if($form_state['clicked_button']["#name"]=="send"){
    
    $wsdl_uri=variable_get('ccimapping_ws_wsdluri','');
    $newsroom=variable_get('ccimapping_newsroom','');
    $product_name=variable_get('ccimapping_product_name','');
    
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


/**
 * Call the CCI WS and ask for the XML definition of a content type
 * 
 * @param: $type
 *    the content type's machine name -not used for the moment-
 * 
 * @return the result of the call from the  WS: an XML structure in string format wich describe the packagetype
 */
function ccimap_get_packagetype_fromWS($wally_type){
  $cci_type=ccimap_type_converter($type);
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

function ccimap_get_specific_type($xml,$wally_type){
  
  $cci_type=ccimap_type_converter($wally_type);
  $dom = new DOMDocument('1.0', 'UTF-8');
  $dom->loadXML($xml);
  $packagedefs=$dom->getElementsByTagName('packagedefs');
  foreach($packagedefs as $packagedef){
    //we want to find the packagedef wit packagetype->name==$title .... but DomDocument API ins'nt easy as Jquery selector
    
    $packagetype=$packagedef->getElementsByTagName('packagetype');
    $packagetype= $packagetype->item(0);
    $name=$packagetype->getElementsByTagName('name');
    $name=$name->item(0);
    if($name->nodeValue==$cci_type){
      $newdoc = new DOMDocument('1.0', 'UTF-8');
      $newdoc->formatOutput = true;
      $node = $newdoc->importNode($packagedef, true);
      $newdoc->appendChild($node);
      return $newdoc->saveXML();
    }
  }
  
}


