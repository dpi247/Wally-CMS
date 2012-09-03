<?php

function wallypressfilters_form_config_import_export_form($form_state) {
  drupal_add_js(drupal_get_path('module', 'wallypressfilters').'/scripts/export_config.js');
  
  $form['export'] = array(
    '#type' => 'fieldset',
    '#title' => t('Export'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $filters = wallypressfilters_getfilters();
  $filters_options = array();
  foreach ($filters as $filter) {
    $filters_options[$filter->fid] = $filter->name;
  }
  $form['export']['select_filters'] = array(
    '#type' => 'radios',
    '#title' => t('Select a filter to export'),
    '#options' => $filters_options,
  );
  
  $form['export']['submit'] = array(
    '#type' => 'markup',
    '#value' => '<input type="button" id="export-submit" value="'.t('Export').'"  class="form-submit" />',
  );
  
  $form['export']['export_area'] = array(
    '#type' => 'textarea',
    '#title' => t('Exported XML'),
    '#id' => 'export-area',
    '#prefix' => '<br><img id="export-loading" src="/'.drupal_get_path('module', 'wallypressfilters').'/images/export-loading.gif" />',
  );
  
  $form['import'] = array(
    '#type' => 'fieldset',
    '#title' => t('Import'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $form['import']['import_area'] = array(
    '#type' => 'textarea',
    '#title' => t('XML'),
    '#description' => t('Paste XML here.'),
    '#required' => TRUE,
  );

  $form['import']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
    '#submit' => array('wallypressfilters_form_config_import_export_form_import_submit'),
  );
  
  return $form;
}

function wallypressfilters_form_config_import_export_form_import_submit($form, $form_state) {
  $values = $form_state['values'];
  $import_text = $values['import_area'];
  
  $dom = new DomDocument('1.0', 'UTF-8');
  if ($dom->loadXML(trim($import_text))) {
    _wallypressfilters_import_export_import_from_dom($dom);
    drupal_set_message(t('Import done'));
  } else {
    drupal_set_message(t('No XML to import or XML unreadable.'), 'error');
  }
}

function wallypressfilters_config_export($fid) {
  $filter = wallypressfilters_getfilterbyid($fid);
  $tags = wallypressfilters_getalltagsfromfilter($filter->name);
  $dom = _wallypressfilters_import_export_generate_dom($filter, $tags);
  
  module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
  print $dom->saveXML();
}

function _wallypressfilters_import_export_generate_dom($filter, $tags) {
  $dom = new DomDocument('1.0', 'UTF-8');
  $dom_filter = $dom->appendChild($dom->createElement('Filter'));
  $dom_filter->setAttribute('fid', $filter->fid);
  $dom_name = $dom_filter->appendChild($dom->createElement('Name', $filter->name));
  $dom_description = $dom_filter->appendChild($dom->createElement('Description', $filter->description));
  $dom_tags = $dom_filter->appendChild($dom->createElement('Tags'));

  foreach ($tags as $tag) {
    $dom_tag = $dom_tags->appendChild($dom->createElement('Tag'));
    $dom_tag->setAttribute('tid', $tag->tid);
    $dom_tag->appendChild($dom->createElement('Format', is_null($tag->format) ? 0 : $tag->format));
    $dom_tag->appendChild($dom->createElement('Type', $tag->type));
    $dom_tag->appendChild($dom->createElement('TagName', $tag->tag));
    $dom_tag->appendChild($dom->createElement('TagDescription', $tag->description));
    /*$settings = $dom->createCDATASection(serialize($tag->settings));
    $dom_settings = $dom_tag->appendChild($dom->createElement('Settings'));
    $dom_settings->appendChild($settings);*/
    $dom_tag->appendChild($dom->createElement('Settings', serialize($tag->settings)));
  }
  
  return $dom;
}

function _wallypressfilters_import_export_import_from_dom($dom) {
  $filter = new stdClass();
  $dom_filter = $dom->getElementsByTagName('Filter')->item(0);
  $filter->name = $dom_filter->getElementsByTagName('Name')->item(0)->nodeValue;
  $filter->description = $dom_filter->getElementsByTagName('Description')->item(0)->nodeValue;

  if ($orig_filter = wallypressfilters_getfilterbyname($filter->name)) {
    $filter->fid = $orig_filter->fid;
    drupal_write_record('wallypressfilters', $filter, array('fid'));
    db_query("DELETE FROM {wallypressfilters_tags} WHERE filter = '%s'", $filter->name);
  } else {
    drupal_write_record('wallypressfilters', $filter);
  }

  $dom_tags = $dom->getElementsByTagName('Tags');
  if ($dom_tags->item(0)) {
    $dom_tag = $dom_tags->item(0)->getElementsByTagName('Tag');
    if ($dom_tag->item(0)) {
      foreach ($dom_tag as $tag) {
        $new_tag = new stdClass();
        $new_tag->filter = $filter->name;
        $format = $tag->getElementsByTagName('Format')->item(0)->nodeValue;
        $new_tag->format = ($format == 0) ? NULL : $format;
        $new_tag->type = $tag->getElementsByTagName('Type')->item(0)->nodeValue;
        $new_tag->tag = $tag->getElementsByTagName('TagName')->item(0)->nodeValue;
        $new_tag->description = $tag->getElementsByTagName('TagDescription')->item(0)->nodeValue;
        $new_tag->settings = unserialize($tag->getElementsByTagName('Settings')->item(0)->nodeValue);
        drupal_write_record('wallypressfilters_tags', $new_tag);
      }
    }
  }
}
