<?php
// $Id: googledoc.inc $

/**
 * @file
 *   This include processes googledoc media files for use by emfield.module.
 */

/**
 * hook_emother_PROVIDER_info()
 * this returns information relevant to a specific 3rd party media provider
 * @return
 *   an array of strings requested by various admin and other forms
 *   'name' => the translated name of the provider
 *   'url' => the url to the main page for the provider
 *   'settings_description' => a description of the provider that will be posted in the admin settings form
 *   'supported_features' => an array of rows describing the state of certain supported features by the provider.
 *      These will be rendered in a table, with the columns being 'Feature', 'Supported', 'Notes'.
 * 
 */
function emother_googledoc_info() {
  $features = array(
    array(t('Autoplay'), t('No'), ''),
    array(t('RSS Attachment'), t('No'), ''),
    array(t('Thumbnails'), t('No'), t('')),
  );
  return array(
    'provider' => 'googledoc',
    'name' => t('Google Doc'),
    'url' => MEDIA_GOOGLEDOC_MAIN_URL,
    'settings_description' => t('These settings specifically affect other media displayed from <a href="@googledoc" target="_blank">Google Doc</a>.', array('@googledoc' => MEDIA_GOOGLEDOC_MAIN_URL)),
    'supported_features' => $features,
  );
}

/**
 *  hook_emother_PROVIDER_settings()
 *  This should return a subform to be added to the emother_settings() admin
 *  settings page.
 */
function emother_googledoc_settings() {
  // We'll add a field set of options here. You may add other options
  // to this element, or remove the field set entirely if there are no
  // user-configurable options allowed by the googledoc provider.
  module_load_include('inc', 'media_googledoc', 'includes/media_googledoc.admin'); 
  return media_googledoc_admin_form();
}


/**
 * hook_emother_PROVIDER_extract()
 * this is called to extract the media code from a pasted URL or embed code.
 * 
 * @param $url
 *   a string with the pasted URL or embed code
 * 
 * @return
 *   The googledoc_id of the googledoc media.
 */
function emother_googledoc_extract($parse) {
  $matches = NULL;

  //https://spreadsheets.google.com/spreadsheet/embeddedform?formkey=dGs3T0I1NnNmdmROYkhhZnJYTElGcmc6MQ
  if (preg_match('/^https:\/\/spreadsheets.google\.com\/spreadsheet\/embeddedform\?formkey=([a-zA-Z0-9]*)/', $parse, $matches)) { //Get the googledoc code, can be normal or from embed link
    return $matches[1];
  }
  else {
    return FALSE;
  }
}

 /**
 * hook_emother_PROVIDER_embedded_link($code)
 * returns a link to view the media at the provider's site.
 *  @param $code
 *    The string containing the code of the googledoc media to watch.
 *  @return
 *    A string containing the URL to view the googledoc media at the original googledoc site.
 */
/*
function emother_googledoc_embedded_link($code) {
  return 'http://www.googledoc.com/index.php?option=com_altcaster&task=siteviewaltcast&altcast_code='.$code.'&height=550&width=470';
}
*/

/**
 *  hook_emother_PROVIDER_other()
 *  This displays the full/normal-sized media we want, usually on the
 *  default page view.
 *  @param $code
 *    The googledoc code for the other media to embed.
 *  @param $width
 *    The width to display the media.
 *  @param $height
 *    The height to display the media.
 *  @param $field
 *    The field info from the requesting node.
 *  @param $item
 *    The actual content from the field.
 *  @return
 *    The html of the embedded media.
 */
function emother_googledoc_other($embed, $width, $height, $field, $item, $node, $autoplay, $options = array()) {
  $options['width'] = isset($options['width']) ? $options['width'] : media_googledoc_variable_get('full_width');
  $options['height'] = isset($options['height']) ? $options['height'] : media_googledoc_variable_get('full_height');
  $options['field'] = isset($options['field']) ? $options['field'] : $field;
  $options['item'] = isset($options['item']) ? $options['item'] : $item;
  $options['node'] = isset($options['node']) ? $options['node'] : $node;
  $output = theme('media_googledoc', $embed, $options);
  return $output;
}

/**
 *  hook_emother_PROVIDER_preview()
 *
 *  This displays the preview-sized media we want, commonly for the
 *  teaser.
 *  @param $embed
 *    The googledoc code for the other media to embed.
 *  @param $width
 *    The width to display the media.
 *  @param $height
 *    The height to display the media.
 *  @param $field
 *    The field info from the requesting node.
 *  @param $item
 *    The actual content from the field.
 *  @return
 *    The html of the embedded media.
 */
function emother_googledoc_preview($embed, $width, $height, $field, $item, &$node, $autoplay, $options = array()) {
  
  $options['width'] = isset($options['width']) ? $options['width'] : media_googledoc_variable_get('preview_width');
  $options['height'] = isset($options['height']) ? $options['height'] : media_googledoc_variable_get('preview_height');
  $options['field'] = isset($options['field']) ? $options['field'] : $field;
  $options['item'] = isset($options['item']) ? $options['item'] : $item;
  $options['node'] = isset($options['node']) ? $options['node'] : $node;
  $output = theme('media_googledoc', $embed, $options);
  return $output;
}

