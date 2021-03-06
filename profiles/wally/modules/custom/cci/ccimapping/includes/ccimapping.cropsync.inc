<?php

function ccimapping_cropsync_form_cropings_editor_form($form_state) {
  $form = array();
  
  module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
  
  $form['xml'] = array(
    '#type' => 'textarea',
    '#title' => t('XML Cropings'),
    '#default_value' => formatXmlString(ccimapping_cropsync_get_cropings_xml()),
    '#disabled' => !user_access('edit synchronization datas'),
  );

  $form['send'] = array(
    '#type' => 'submit',
    '#name' => 'send',
    '#value' => t('Save & Send to CCI'),
  );
  
  $form['fyi'] = array(
    '#type' => 'fieldset',
    '#title' => t('FYI, these are the current croping values'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => 999,
  );
  
  $form['fyi']['markup'] = array(
    '#type' => 'markup',
    '#value' => '<pre>'.htmlentities(ccimapping_cropsync_get_cropings_fromWS()).'</pre>',
  );

  return $form;
}

function ccimapping_cropsync_form_cropings_editor_form_submit($form, $form_state) {
  $wsdl_uri = wally_variable_get('ccimapping_ws_wsdluri', '');
  $newsroom = wally_variable_get('ccimapping_newsroom', '');
  $product_name = wally_variable_get('wallyadmin_product', wally_variable_get('ccimapping_product_name',''));
  $parameters_array = array(
    'newsroom' => $newsroom,
    'productname' => $product_name,
    'xml' => $form_state['values']['xml'],
  );

  if ($client = cciadmin_load_soapclient($wsdl_uri)) {
    try {
      $result = $client->setImageSettings($parameters_array);
      drupal_set_message(t('The cropings have been sent to CCI'));
    } catch (SoapFault $exception) {
      drupal_set_message(t('WebService error while sending the XML: \n FAULTSTRING: @faultstring\n FAULTCODE: @faultcode', array('@faultstring' => $exception->faultstring, '@faultcode' => $exception->faultcode)),'error');
      dsm($exception);
    }
    
    /*dsm($client->__getLastRequest(), 'submit req');
    dsm($client->__getLastRequestHeaders(), 'submit reqhead');
    dsm($client->__getLastResponse(), 'submit rep');
    dsm($client->__getLastResponseHeaders(), 'submit rephead');*/
  }
}

/**
 * Call the CCI WS and ask for the XML definition of the cropings
 * 
 * @return the result of the call from the  WS: an XML structure in string format wich describe the cropings
 */
function ccimapping_cropsync_get_cropings_fromWS() {
  $wsdl_uri = wally_variable_get('ccimapping_ws_wsdluri', '');
  $newsroom = wally_variable_get('ccimapping_newsroom', '');
  $product_name = wally_variable_get('wallyadmin_product', wally_variable_get('ccimapping_product_name',''));
  $parameters_array = array(
    'newsroom' => $newsroom,
    'productname' => $product_name,
  );
  
  if ($client = ccimapping_load_soapclient($wsdl_uri)) {
    //dsm($client->__getFunctions(), 'fct');
    //dsm($client->__getTypes(), 'types');
    try {
      $result = $client->getImageSettings($parameters_array);
      return $result->return;
    } catch (SoapFault $exception) {
      drupal_set_message(t('WebService error while getting the XML: \n FAULTSTRING: @faultstring\n FAULTCODE: @faultcode', array('@faultstring' => $exception->faultstring, '@faultcode' => $exception->faultcode)),'error');
      dsm($exception);
    }
    
    /*dsm($client->__getLastRequest(), 'req');
    dsm($client->__getLastRequestHeaders(), 'reqhead');
    dsm($client->__getLastResponse(), 'rep');
    dsm($client->__getLastResponseHeaders(), 'rephead');*/
  }

  return '';
}

function ccimapping_cropsync_get_cropings_xml() {
  $dom = new DOMDocument('1.0', 'UTF-8');
  $imageSettings = $dom->appendChild($dom->createElement('imageSettings'));
  $imageSettings->setAttribute('xmlns:cci', 'http://cci/extension/integration');
  
  $crops = ccimapping_cropsync_get_crops();

  foreach ($crops as $crop) {
    $cropSetting = $imageSettings->appendChild($dom->createElement('cropSetting'));
    $cropSetting->setAttribute('id', $crop['id']);
    $cropSetting->setAttribute('validate', $crop['validate']);
    $cropSetting->appendChild($dom->createElement('label', $crop['label']));
    $dimension = $cropSetting->appendChild($dom->createElement('dimension'));
    $dimension->setAttribute('width', $crop['width']);
    $dimension->setAttribute('height', $crop['height']);
    $dimension->setAttribute('type', $crop['type']);
    
    $gallerySetting = $imageSettings->appendChild($dom->createElement('gallerySetting'));
    $gallerySetting->setAttribute('id', $crop['id']);
    $gallerySetting->setAttribute('validate', $crop['validate']);
    $gallerySetting->appendChild($dom->createElement('label', $crop['label']));
    $gDimension = $gallerySetting->appendChild($dom->createElement('dimension'));
    $gDimension->setAttribute('width', $crop['width']);
    $gDimension->setAttribute('height', $crop['height']);
    $gDimension->setAttribute('type', $crop['type']);
  }
  
  return $dom->saveXML();
}

function ccimapping_cropsync_get_crops() {
  $crops = array();
  $presets = imagecache_presets();
  foreach ($presets as $preset) {
    if ($preset['actions']) {
      $found_wallycrop = FALSE;
      $datas = array();
      foreach ($preset['actions'] as $action) {
        if ($action['action'] == 'wallycontenttypes_wallycrop') {
          $found_wallycrop = TRUE;
        } elseif (($action['action'] == 'imagecache_scale' || $action['action'] == 'imagecache_scale_and_crop') && $found_wallycrop) {
          $crops[$preset['presetid']] = array(
            'id' => $preset['presetname'],
            'validate' => 'none',
            'label' => $preset['presetname'],
            'width' => $action['data']['width'],
            'height' => $action['data']['height'],
            'type' => 'pixel',
          );
        }
      }
    }
  }

  return $crops;
}
