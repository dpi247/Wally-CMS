<?php

function ccimapping_form_productinfo_editor_form($form_state) {
  $form = array();

  module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
  
  $generated_dom = ccimap_build_productinfo();
  
  $form['xml'] = array(
    '#type' => 'textarea',
    '#title' => t('XML Mapping'),
    '#default_value' => formatXmlString($generated_dom->saveXML()),
    '#disabled' => !user_access('edit synchronization datas'),
  );

  $form['type'] = array(
    '#type' => 'value',
    '#weight' => 9,
    '#value' => $type,
  );
  
  $form['actual_xml_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Current version of the XML from CCI-Newsgate'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['actual_xml_fieldset']['actual_xml'] = array(
    '#type' => 'markup',
    '#weight' => 999,
    '#value' => '<pre>'.htmlspecialchars(formatXmlString(ccimap_get_productinfo_fromWS())).'</pre>',
  );
  
  $form['xml_to_be_sent_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('XML that will be sent to CCI'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  drupal_alter('ccimapping_productinfo', $generated_dom, 'before_send');
  $form['xml_to_be_sent_fieldset']['xml_to_be_sent'] = array(
    '#type' => 'markup',
    '#weight' => 1000,
    '#value' => '<pre>'.htmlspecialchars(formatXmlString($generated_dom->saveXML())).'</pre>',
  );
  
  $form['send'] = array(
    '#type' => 'submit',
    '#name' => 'send',
    '#value' => t('Save & Send to CCI'),
  );

  return $form;
}

function ccimapping_form_productinfo_editor_form_submit($form, $form_state) {
  if($form_state['clicked_button']['#name'] == 'send') {
    $wsdl_uri = wally_variable_get('ccimapping_ws_wsdluri', '');
    $newsroom = wally_variable_get('ccimapping_newsroom', '');
    $product_name = wally_variable_get('ccimapping_product_name', '');

    $dom = new DomDocument('1.0', 'UTF-8');
    $dom->loadXML($form_state['values']['xml']);
    drupal_alter('ccimapping_productinfo', $dom, 'before_send');

    //if ($dom->schemaValidate(drupal_get_path('module', 'ccimapping').'/definitions/CCIPackageTypeXML.xsd')){
      $xml = $dom->saveXML();
/*module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
dsm(formatXmlString($xml));
return;*/
      if ($client = ccimapping_load_soapclient($wsdl_uri)) {
        $parameters_array = array('newsroom' => $newsroom, 'productname' => $product_name, 'xml' => $xml);
  
        try {
          $result = $client->setProductTemplate($parameters_array);
          drupal_set_message(t('The productinfo has been sent to CCI'));
        } catch (SoapFault $exception) {
          drupal_set_message(t('WebService Error when sending the XML: \n FAULTSTRING: @faultstring\n FAULTCODE: @faultcode', array('@faultstring' => $exception->faultstring, '@faultcode' => $exception->faultcode)),'error');
          dsm($exception);
        }
      }
    /*} else {
      drupal_set_message(t('XML not validated'));
    }*/
  }
}

/**
 * Call the CCI WS and ask for the XML definition of  the site structure (product info : destinations, target, layout)
 *
 * @return the result of the call from the  WS: an XML structure in string format wich describe the packagetype
 */
function ccimap_get_productinfo_fromWS() {
  $wsdl_uri = wally_variable_get('ccimapping_ws_wsdluri', '');
  $newsroom = wally_variable_get('ccimapping_newsroom', '');
  $product_name = wally_variable_get('ccimapping_product_name', '');
//return '';
  if ($client = ccimapping_load_soapclient($wsdl_uri)) {
    try {
      $result = $client->getProductTemplate(array('newsroom' => $newsroom, 'productname' => $product_name));
      return $result->return;
    } catch (SoapFault $exception) {
      drupal_set_message(t('WebService Error when getting the XML: \n FAULTSTRING: @faultstring\n FAULTCODE: @faultcode', array('@faultstring' => $exception->faultstring, '@faultcode' => $exception->faultcode)),'error');
      dsm($exception);
    }
  }

  return '';
}

/**
 * Build an XML file corresponding the actual structure of the site. this XML should Be sent to the CCI WS
 *
 * @return an XML in string format
 */
function ccimap_build_productinfo(){
  $hierarchies_array = wallyctools_get_hierarchies();
  $taxonomy_tree = taxonomy_get_tree(variable_get('wallymport_destinationpath', 0));
  $nested_taxonomy_tree = wallytoolbox_convert_taxonomy_flat_to_tree($taxonomy_tree);
  $dom = new DomDocument('1.0', 'UTF-8');
  ccimap_build_productinfo_xml($dom, $hierarchies_array, $nested_taxonomy_tree);
  drupal_alter('ccimapping_productinfo', $dom, 'generate');
  return $dom;
}

function ccimap_build_productinfo_xml(&$dom, $hierarchies_array, $nested_taxonomy_tree) {
  $dom->standalone = FALSE;
  $exportproductinformation = $dom->appendChild($dom->createElement('exportproductinformation'));
  $exportproductinformation->setAttribute('newsroom', wally_variable_get('ccimapping_newsroom', ''));
  $exportproductinformation->setAttribute('publicationchannel', wally_variable_get('ccimapping_product_name', ''));
  $exportproductinformation->setAttribute('subtype', 'Online');
  $pubdatestring = $exportproductinformation->appendChild($dom->createElement('pubdatestring'));
  $pubdatestring->setAttribute('name', '%d-%m-%Y');
  $product = $exportproductinformation->appendChild($dom->createElement('product', wally_variable_get('ccimapping_product_name', '')));

  $baseinfo = $exportproductinformation->appendChild($dom->createElement('baseinfo'));
  $leafname = $baseinfo->appendChild($dom->createElement('leafname'));
  $leafname->setAttribute('name', 'Target');
  $mainpriorities = $baseinfo->appendChild($dom->createElement('mainpriorities'));
  $rank_limit = wally_variable_get('cckdestinations_rank_limit', 10);
  $mainpriority = $mainpriorities->appendChild($dom->createElement('mainpriority'));
  $mainpriority->setAttribute('name', 'Rank 0');
  $mainpriority->setAttribute('value', '0');
  $mainpriority->setAttribute('default', 'yes');
  for ($i = 1; $i <= $rank_limit; $i++) {
    $mainpriority = $mainpriorities->appendChild($dom->createElement('mainpriority'));
    $mainpriority->setAttribute('name', 'Rank '.$i);
    $mainpriority->setAttribute('value', $i);
  }
  
  $budgettemplate = $exportproductinformation->appendChild($dom->createElement('budgettemplate'));
  ccimap_build_productinfo_xml_hierarchy_items($dom, $budgettemplate, $hierarchies_array, $nested_taxonomy_tree);
}

function ccimap_build_productinfo_xml_hierarchy_items($dom, &$dom_element, $hierarchies, $nested_taxonomy_tree) {
  foreach ($nested_taxonomy_tree as $key => $values) {
    $term = $values['term'];
    $level = $term->depth + 1;
    $dom_level = $dom_element->appendChild($dom->createElement('level'.$level));
    $dom_level->setAttribute('name', $hierarchies[$term->tid]['term_name']);

    unset($hierarchies[$term->tid]['redacblocks']['none']);
    foreach ($hierarchies[$term->tid]['redacblocks'] as $redack_machine_name => $redacblock) {
      $leaf = $dom_level->appendChild($dom->createElement('leaf'));
      $leaf->setAttribute('name', $redack_machine_name);
    }
    ccimap_build_productinfo_xml_hierarchy_items($dom, $dom_level, $hierarchies, $values['childrens']);
  }
}
