<?php
// $Id: qualifio.inc $

/**
 * @file
 *   This include processes qualifio media files for use by emfield.module.
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
function emother_qualifio_info() {
  $features = array(
    array(t('Autoplay'), t('No'), ''),
    array(t('RSS Attachment'), t('No'), ''),
    array(t('Thumbnails'), t('No'), t('')),
  );
  return array(
    'provider' => 'qualifio',
    'name' => t('Qualifio'),
    'url' => MEDIA_QUALIFIO_MAIN_URL,
    'settings_description' => t('These settings specifically affect other media displayed from <a href="@qualifio" target="_blank">Qualifio</a>.', array('@qualifio' => MEDIA_QUALIFIO_MAIN_URL)),
    'supported_features' => $features,
  );
}

/**
 *  hook_emother_PROVIDER_settings()
 *  This should return a subform to be added to the emother_settings() admin
 *  settings page.
 */
function emother_qualifio_settings() {
  // We'll add a field set of options here. You may add other options
  // to this element, or remove the field set entirely if there are no
  // user-configurable options allowed by the qualifio provider.
  module_load_include('inc', 'media_qualifio', 'includes/media_qualifio.admin'); 
  return media_qualifio_admin_form();
}


/**
 * hook_emother_PROVIDER_extract()
 * this is called to extract the media code from a pasted URL or embed code.
 * 
 * @param $url
 *   a string with the pasted URL or embed code
 * 
 * @return
 *   The qualifio_id of the qualifio media.
 */
function emother_qualifio_extract($parse) {
  $matches = NULL;
  if (preg_match('@qualifio\.com/.*id=([^/"&]+)@i', $parse, $matches)) { //Get the qualifio code, can be normal or from embed link
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
 *    The string containing the code of the qualifio media to watch.
 *  @return
 *    A string containing the URL to view the qualifio media at the original qualifio site.
 */
/*
function emother_qualifio_embedded_link($code) {
  return 'http://www.qualifio.com/index.php?option=com_altcaster&task=siteviewaltcast&altcast_code='.$code.'&height=550&width=470';
}
*/

/**
 *  hook_emother_PROVIDER_other()
 *  This displays the full/normal-sized media we want, usually on the
 *  default page view.
 *  @param $code
 *    The qualifio code for the other media to embed.
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
function emother_qualifio_other($embed, $width, $height, $field, $item, $node, $autoplay, $options = array()) {
  $options['width'] = isset($options['width']) ? $options['width'] : media_qualifio_variable_get('full_width');
  $options['height'] = isset($options['height']) ? $options['height'] : media_qualifio_variable_get('full_height');
  $options['field'] = isset($options['field']) ? $options['field'] : $field;
  $options['item'] = isset($options['item']) ? $options['item'] : $item;
  $options['node'] = isset($options['node']) ? $options['node'] : $node;
  $output = theme('media_qualifio', $embed, $options);
  return $output;
}

/**
 *  hook_emother_PROVIDER_preview()
 *
 *  This displays the preview-sized media we want, commonly for the
 *  teaser.
 *  @param $embed
 *    The qualifio code for the other media to embed.
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
function emother_qualifio_preview($embed, $width, $height, $field, $item, &$node, $autoplay, $options = array()) {
  
  $options['width'] = isset($options['width']) ? $options['width'] : media_qualifio_variable_get('preview_width');
  $options['height'] = isset($options['height']) ? $options['height'] : media_qualifio_variable_get('preview_height');
  $options['field'] = isset($options['field']) ? $options['field'] : $field;
  $options['item'] = isset($options['item']) ? $options['item'] : $item;
  $options['node'] = isset($options['node']) ? $options['node'] : $node;
  $output = theme('media_qualifio', $embed, $options);
  return $output;
}

