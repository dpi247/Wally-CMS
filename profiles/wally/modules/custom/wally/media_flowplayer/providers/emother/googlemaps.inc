<?php
// $Id: googlemaps.inc,v 1.1.2.2 2009/06/26 18:31:00 aaron Exp $

/**
 * @file
 *   This is an include file used by emfield.module.
 */

function emother_googlemaps_info() {
  $features = array(
  array(t('Google Maps'), t('Yes'), ''),
  );
  return array(
    'provider' => 'googlemaps',
    'name' => t('googlemaps'),
    'url' => '',
    'settings_description' => t('These settings specifically affect how custom audio files are displayed. When a field uses a URL it determines to be a link directly to an audio file, it will embed that file into the content. Installing the <a href="@googlemaps">googlemaps module</a> will give the embedded player a nicer look.', array('@googlemaps' => 'http://drupal.org/project/googlemaps')),
    'supported_features' => $features,
    'weight' => 9,
  );
}

function emother_googlemaps_settings() {
  $form = array();
  return $form;
}

function emother_googlemaps_extract($embed = '') {
  $embed_url=parse_url($embed);
  if($embed_url['host']=="maps.google.com"){
    return $embed;
  }
  else{
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
function emother_googlemaps_other($url = NULL, $width = 0, $height = 0, $field = NULL, $data = array(), $node = NULL) {
  $url.='&output=embed';
  return '<iframe width="425" height="350" frameborder="0" scrolling="no" marginheight="0" marginwidth="0" src="'.$url.'"></iframe><br /><small><a href="'.$url.'" style="color:#0000FF;text-align:left">View Larger Map</a></small>';
}

/**
 * Retrieves an associative array of types that are supported by Custom URLs.
 */
function _emother_googlemaps_default_types() {
  return array(
    'google_maps' => t('googlemaps word (dovc)'),
  );
}

/**
 * Retrieves the types that are supported by Custom URLs.
 */
function _emother_googlemaps_supported_types() {
  return variable_get('emother_googlemaps_supported_types', array_keys(_emother_googlemaps_default_types()));
}
