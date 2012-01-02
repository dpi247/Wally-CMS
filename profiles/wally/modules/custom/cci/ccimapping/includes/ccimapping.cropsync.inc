<?php

function ccimapping_cropsync_form_cropings_editor_form($form_state) {
  $form = array();
  
  $form['xml'] = array(
    '#type' => 'textarea',
    '#title' => t('XML Cropings'),
    '#default_value' => ccimapping_cropsync_get_cropings_fromWS(),
  );

  $form['send'] = array(
    '#type' => 'submit',
    '#name'=>'send',
    '#value' => t('Save & Send to CCI'),
  );

  return $form;
}

function ccimapping_cropsync_form_cropings_editor_form_submit($form_state) {
  $wsdl_uri = variable_get('ccimapping_ws_wsdluri', '');
  $newsroom = variable_get('ccimapping_newsroom', '');
  $product_name = variable_get('ccimapping_product_name', '');
  $parameters_array = array(
    'newsroom' => $newsroom,
    'productname' => $product_name,
    //'xml' => '<![CDATA['.$form_state['values']['xml'].']]>',
    'xml' => '<![CDATA['.$form_state['values']['xml'].']]>',
  );

  $client = new SoapClient($wsdl_uri, array('features' => SOAP_SINGLE_ELEMENT_ARRAYS));

  try {
    $result = $client->setImageSettings($parameters_array);
    drupal_set_message(t('The cropings have been sent to CCI'));
  } catch (SoapFault $exception) {
    drupal_set_message('WebService Error: FaultString: '.$exception->faultstring.'. FaultCode: '.$exception->faultcode, 'error');
    dsm($exception);
  }
}

/**
 * Call the CCI WS and ask for the XML definition of the cropings
 * 
 * @return the result of the call from the  WS: an XML structure in string format wich describe the cropings
 */
function ccimapping_cropsync_get_cropings_fromWS() {
  $wsdl_uri = variable_get('ccimapping_ws_wsdluri', '');
  $newsroom = variable_get('ccimapping_newsroom', '');
  $product_name = variable_get('ccimapping_product_name', '');
  $parameters_array = array(
    'newsroom' => $newsroom,
    'productname' => $product_name,
  );
  
  $client = new SoapClient($wsdl_uri, array('features' => SOAP_SINGLE_ELEMENT_ARRAYS));

  try {
    $result = $client->getImageSettings($parameters_array);
  } catch (SoapFault $exception) {
    drupal_set_message('WebService Error: FaultString: '.$exception->faultstring.'. FaultCode: '.$exception->faultcode, 'error');
    dsm($exception);
  }

  return $result->return;
}

/*function ccimapping_cropsync_get_cropings_xml() {
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
        } elseif ($action['action'] == 'imagecache_scale' && $found_wallycrop) {
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

function ccimapping_cropsync_set_cropings_xml($req_xml) {
  $dom = new DOMDocument();
  $dom->loadXML($req_xml);
  $crops_settings = $dom->getElementsByTagName('cropSetting');
  
  $result = TRUE;
  
  if ($crops_settings->item(0)) {
    foreach($crops_settings as $crop_settings) {
      $presetname = $crop_settings->getAttribute('id');
      $preset = imagecache_preset_by_name($presetname);
      $dimension = $crop_settings->getElementsByTagName('dimension')->item(0);
      $datas['width'] = $dimension->getAttribute('width');
      $datas['height'] = $dimension->getAttribute('height');

      if ($preset['actions']) {
        $found_wallycrop = FALSE;
        foreach ($preset['actions'] as $action) {
          if ($action['action'] == 'wallycontenttypes_wallycrop') {
            $action['data']['width'] = $datas['width'];
            $action['data']['height'] = $datas['height'];
            imagecache_action_save($action);
            $found_wallycrop = TRUE;
          } elseif ($action['action'] == 'imagecache_scale' && $found_wallycrop) {
            $action['data']['width'] = $datas['width'];
            $action['data']['height'] = $datas['height'];
            imagecache_action_save($action);
          }
        }
      }
    }
  }
  
  return $result;
}*/
