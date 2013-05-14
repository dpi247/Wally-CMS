<?php

function ccimapping_form_packagetype_editor_form($form_state,$wally_type=NULL){
  $form = array();
  
  if ($wally_type) {
    module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');

    $form['xml'] = array(
      '#type' => 'textarea',
      '#title' => t('XML Mapping'),
      '#default_value' => formatXmlString(ccimap_get_specific_type(ccimap_get_packagetype_fromWS($wally_type), $wally_type)),
      '#disabled' => !user_access('edit synchronization datas'),
    );

    $form['type'] = array(
      '#type' => 'value',
      '#value' => $type,
    );

    $form['send'] = array(
      '#type' => 'submit',
      '#name'=>'send',
      '#value' => t('Save & Send to CCI'),
    );
  } else{
    $form['type_selector'] = array(
      '#type' => 'markup',
      '#value' => theme('ccimapping_typeselector', 'packagetype'),
    );
  }

  return $form;
}

function ccimapping_form_packagetype_editor_form_submit($form,$form_state){
  if($form_state['clicked_button']['#name'] == 'send') {
    $wsdl_uri=wally_variable_get('ccimapping_ws_wsdluri', '');
    $newsroom=wally_variable_get('ccimapping_newsroom', '');
    $product_name = wally_variable_get('wallyadmin_product', wally_variable_get('ccimapping_product_name',''));

    $dom = new DomDocument('1.0', 'UTF-8');
    $dom->loadXML($form_state['values']['xml']);
    
    module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
    if (wallytoolbox_validate_xml($dom, drupal_get_path('module', 'ccimapping').'/definitions/CCIPackageTypeXML.xsd')) {
      $xml = $dom->saveXML();

      if ($client = ccimapping_load_soapclient($wsdl_uri)) {
        $parameters_array = array('newsroom' => $newsroom, 'productname' => $product_name, 'xml' => $xml);
  
        try {
          $result = $client->setPackageType($parameters_array);
          drupal_set_message(t('The packageType has been sent to CCI'));
        } catch (SoapFault $exception) {
          drupal_set_message(t('WebService error while sending the XML: \n FAULTSTRING: @faultstring\n FAULTCODE: @faultcode', array('@faultstring' => $exception->faultstring, '@faultcode' => $exception->faultcode)),'error');
          dsm($exception);
        }
      }
    } else {
      drupal_set_message(t('XML not validated'));
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
  $cci_type = ccimap_type_converter($type);
  $wsdl_uri = wally_variable_get('ccimapping_ws_wsdluri', '');
  $newsroom = wally_variable_get('ccimapping_newsroom', '');
  $product_name = wally_variable_get('wallyadmin_product', wally_variable_get('ccimapping_product_name',''));

  if ($client = ccimapping_load_soapclient($wsdl_uri)) {
    try {
      $result = $client->getPackageType(array('newsroom' => $newsroom, 'productname' => $product_name));
      return $result->return;
    } catch (SoapFault $exception) {
      drupal_set_message(t('WebService error while getting the XML: \n FAULTSTRING: @faultstring\n FAULTCODE: @faultcode', array('@faultstring' => $exception->faultstring, '@faultcode' => $exception->faultcode)),'error');
      dsm($exception);
    }
  }

  return '';
}

function ccimap_get_specific_type($xml, $wally_type) {
  if (!empty($xml)) {
    $cci_type = ccimap_type_converter($wally_type);
    $dom = new DOMDocument('1.0', 'UTF-8');
    $dom->loadXML($xml);
    $packagedefs = $dom->getElementsByTagName('packagedefs');
    foreach($packagedefs as $packagedef) {
      //we want to find the packagedef wit packagetype->name==$title .... but DomDocument API ins'nt easy as Jquery selector
  
      $packagetype = $packagedef->getElementsByTagName('packagetype');
      $packagetype = $packagetype->item(0);
      $name = $packagetype->getElementsByTagName('name');
      $name = $name->item(0);
      if ($name->nodeValue == $cci_type) {
        $newdoc = new DOMDocument('1.0', 'UTF-8');
        $newdoc->formatOutput = true;
        $node = $newdoc->importNode($packagedef, true);
        $newdoc->appendChild($node);
        return $newdoc->saveXML();
      }
    }
  } else {
    return '';
  }
}
