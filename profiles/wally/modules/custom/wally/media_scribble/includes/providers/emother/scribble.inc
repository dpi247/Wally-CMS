<?php
// $Id: scribble.inc $

/**
 * @file
 *   This include processes scribble media files for use by emfield.module.
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
function emother_scribble_info() {
  $features = array(
    array(t('Autoplay'), t('No'), ''),
    array(t('RSS Attachment'), t('No'), ''),
    array(t('Thumbnails'), t('No'), t('')),
  );
  return array(
    'provider' => 'scribble',
    'name' => t('scribble'),
    'url' => MEDIA_SCRIBBLE_MAIN_URL,
    'settings_description' => t('These settings specifically affect other media displayed from <a href="@scribble"       target="_blank">scribble</a>.', array('@scribble' => MEDIA_SCRIBBLE_MAIN_URL)),
    'supported_features' => $features,
  );
}

/**
 *  hook_emother_PROVIDER_settings()
 *  This should return a subform to be added to the emother_settings() admin
 *  settings page.
 */
function emother_scribble_settings() {
  // We'll add a field set of options here. You may add other options
  // to this element, or remove the field set entirely if there are no
  // user-configurable options allowed by the scribble provider.
  module_load_include('inc', 'media_scribble', 'includes/media_scribble.admin'); 
  return media_scribble_admin_form();
}


/**
 * hook_emother_PROVIDER_extract()
 * this is called to extract the media code from a pasted URL or embed code.
 * 
 * @param $url
 *   a string with the pasted URL or embed code
 * 
 * @return
 *   The scribble_id of the scribble media.
 */
function emother_scribble_extract($parse) {
  $matches = NULL;
  if (preg_match('@scribblelive\.com/.*?Id=([^/"&]+)@i', $parse, $matches)) { //Get the scribble code, can be normal or from embed link
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
 *    The string containing the code of the scribble media to watch.
 *  @return
 *    A string containing the URL to view the scribble media at the original scribble site.
 */
/*
function emother_scribble_embedded_link($code) {
  return 'http://www.scribblelive.com/index.php?option=com_altcaster&task=siteviewaltcast&altcast_code='.$code.'&height=550&width=470';
}
*/

/**
 *  hook_emother_PROVIDER_other()
 *  This displays the full/normal-sized media we want, usually on the
 *  default page view.
 *  @param $code
 *    The scribble code for the other media to embed.
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
function emother_scribble_other($embed, $width, $height, $field, $item, $node, $autoplay, $options = array()) {
  $options['width'] = isset($options['width']) ? $options['width'] : media_scribble_variable_get('full_width');
  $options['height'] = isset($options['height']) ? $options['height'] : media_scribble_variable_get('full_height');
  $options['theme_number'] = isset($options['theme_number']) ? $options['theme_number'] : media_scribble_variable_get('theme_number');
  $options['field'] = isset($options['field']) ? $options['field'] : $field;
  $options['item'] = isset($options['item']) ? $options['item'] : $item;
  $options['node'] = isset($options['node']) ? $options['node'] : $node;
  $output = theme('media_scribble', $embed, $options);
  return $output;
}

/**
 *  hook_emother_PROVIDER_preview()
 *
 *  This displays the preview-sized media we want, commonly for the
 *  teaser.
 *  @param $embed
 *    The scribble code for the other media to embed.
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
function emother_scribble_preview($embed, $width, $height, $field, $item, &$node, $autoplay, $options = array()) {
  
  $options['width'] = isset($options['width']) ? $options['width'] : media_scribble_variable_get('preview_width');
  $options['height'] = isset($options['height']) ? $options['height'] : media_scribble_variable_get('preview_height');
  $options['field'] = isset($options['field']) ? $options['field'] : $field;
  $options['item'] = isset($options['item']) ? $options['item'] : $item;
  $options['node'] = isset($options['node']) ? $options['node'] : $node;
  $output = theme('media_scribble', $embed, $options);
  return $output;
}
