<?php
// $Id: document.inc,v 1.1.2.2 2009/06/26 18:31:00 aaron Exp $

/**
 * @file
 *   This is an include file used by emfield.module.
 */

function emother_document_info() {
  $features = array(
  array(t('PDF'), t('Yes'), ''),
  array(t('DOC'), t('Yes'), ''),
  array(t('PPT'), t('Yes'), ''),
  array(t('XLS'), t('Yes'), ''),
  );
  return array(
    'provider' => 'document',
    'name' => t('Document'),
    'url' => '',
    'settings_description' => t('These settings specifically affect how custom audio files are displayed. When a field uses a URL it determines to be a link directly to an audio file, it will embed that file into the content. Installing the <a href="@document">document module</a> will give the embedded player a nicer look.', array('@document' => 'http://drupal.org/project/document')),
    'supported_features' => $features,
    'weight' => 9,
  );
}

function emother_document_settings() {
  $form = array();
  $form['emother_document_supported_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Supported Types'),
    '#options' => _emother_document_default_types(),
    '#default_value' => _emother_document_supported_types(),
    '#description' => t('Select the document types you wish to support. When a custom url with that type is entered into an embedded audio field, it will be parsed and displayed appropriately. If a type is not supported, then it will be ignored.'),
  );
  return $form;
}

function emother_document_extract($embed = '') {
  //https://spreadsheets.google.com/spreadsheet/embeddedform?formkey=dGs3T0I1NnNmdmROYkhhZnJYTElGcmc6MQ
  if (preg_match('/\.(pdf|doc|ppt|xls)$/', $embed, $matches)) { //Get the googledoc code, can be normal or from embed link
    return $embed;
  }
  else {
    return FALSE;
  }
}

/**
 * hook emaudio_PROVIDER_audio
 * this actually displays the full/normal-sized video we want, usually on the default page view
 *  @param $embed
 *    the video code for the audio to embed
 *  @param $width
 *    the width to display the audio
 *  @param $height
 *    the height to display the audio
 *  @param $field
 *    the field info from the requesting node
 *  @param $item
 *    the actual content from the field
 *  @return
 *    the html of the embedded audio
 */
function emother_document_other($url = NULL, $width = 0, $height = 0, $field = NULL, $data = array(), $node = NULL) {
  return '<iframe src="http://docs.google.com/viewer?url='.$url.'&embedded=true" width="'.$width.'" height="'.$height.'" style="border: none;"></iframe>';
  return theme_emother_pdf_embeddeccode($url, $width, $height, $field, $data, $node);
}

function theme_emother_pdf_embeddeccode($url, $width, $height, $field, $data, $node){
  return '<iframe src="http://docs.google.com/viewer?url='.$url.'&embedded=true" width="'.$width.'" height="'.$height.'" style="border: none;"></iframe>';
}

/**
 * Retrieves an associative array of types that are supported by Custom URLs.
 */
function _emother_document_default_types() {
  return array(
    'pdf' => t('PDF'),
    'doc' => t('Document word (dovc)'),
    'ppt' => t('PowerPoint (ppt)'),
    'xls' => t('Stylesheet (xls)'),
  );
}

/**
 * Retrieves the types that are supported by Custom URLs.
 */
function _emother_document_supported_types() {
  return variable_get('emother_document_supported_types', array_keys(_emother_document_default_types()));
}
