<?php
// $Id: kewego.inc,v 1.0 2011/01/07 ODM $

/**
 * @file
 *   This include processes kewego.com media files for use by emfield.module.
 */

/**
 * hook emvideo_PROVIDER_info
 * this returns information relevant to a specific 3rd party video provider
 * @return
 *   an array of strings requested by various admin and other forms
 *   'name' => the translated name of the provider
 *   'url' => the url to the main page for the provider
 *   'settings_description' => a description of the provider that will be posted in the admin settings form
 *   'supported_features' => an array of rows describing the state of certain supported features by the provider.
 *      These will be rendered in a table, with the columns being 'Feature', 'Supported', 'Notes'.
 * 
 * @todo: Check if these infos are correct.
 */
function emvideo_kewego_info() {
  $features = array(
    array(t('Autoplay'), t('Yes'), ''),
    array(t('Thumbnails'), t('Yes'), t('')),
    array(t('Duration'), t('Yes'), ''),
    array(t('Full screen mode'), t('Yes'), t('You may customize the player to enable or disable full screen playback. Full screen mode is enabled by default.')),
  );
  return array(
    'module' => 'media_kewego',
    'provides' => array('emvideo'),
    'provider' => 'kewego',
    'name' => t('Kewego'),
    'url' => MEDIA_KEWEGO_MAIN_URL,
    'settings_description' => t('These settings specifically affect videos displayed from <a href="@kewego" target="_blank">Kewego</a>. You can learn more about its <a href="@api" target="_blank">API</a> here.', array('@kewego' => MEDIA_KEWEGO_MAIN_URL, '@api' => MEDIA_KEWEGO_API_INFO)),
    'supported_features' => $features,
  );
}

/**
 * hook emvideo_PROVIDER_settings
 * this should return a subform to be added to the emvideo_settings() admin settings page.
 * note that a form field will already be provided, at $form['PROVIDER'] (such as $form['kewego'])
 * so if you want specific provider settings within that field, you can add the elements to that form field.
 */
function emvideo_kewego_settings() {
  module_load_include('inc', 'media_kewego', 'includes/media_kewego.admin');
  return media_kewego_admin_form();
}

/**
 *  Implement hook emvideo_PROVIDER_data_version().
 */
function emvideo_kewego_data_version() {
  return MEDIA_KEWEGO_DATA_VERSION;
}

/**
 * hook emfield_PROVIDER_data
 *
 * provides an array to be serialised and made available with $item elsewhere
 */
function emvideo_kewego_data($field, $item) {
  $data = array();

  return $data;
}

/**
 * hook emvideo_PROVIDER_extract
 * this is called to extract the video code from a pasted URL or embed code.
 * 
 * @param $video_id
 *   an optional string with the pasted URL or embed code
 * 
 * @return
 *   either an array of regex expressions to be tested, or a string with the video code to be used
 *   if the hook tests the code itself, it should return either the string of the video code (if matched), or an empty array.
 *   otherwise, the calling function will handle testing the embed code against each regex string in the returned array.
 */
function emvideo_kewego_extract($video_id = '') {
  $return = array();
  $allowed_urls = media_kewego_variable_get('allowed_url');
  $allowed_urls = explode("\n", str_replace(array("\n", "\r\n"), "\n", trim($allowed_urls)));
  foreach ($allowed_urls as $allowed_url) {
    $return[] = '+(.*)'.$allowed_url.'(.*)+i';
    /*$temp_return = '';
    $parsed_url = parse_url($allowed_url);
    if (isset($parsed_url['host']) && !empty($parsed_url['host'])) {
      $temp_return = '@'.$parsed_url['host'];
      if (isset($parsed_url['path']) && !empty($parsed_url['path'])) {
        $temp_return .= $parsed_url['path'];
      }
      $temp_return .= '([^"\& ]+)@i';
      $return[] = $temp_return;
    }*/
  }

  return $return;
}

/**
 * hook emvideo_PROVIDER_video
 * this actually displays the full/normal-sized video we want, usually on the default page view
 *  @param $video_id
 *    the video code for the video to embed
 *  @param $width
 *    the width to display the video
 *  @param $height
 *    the height to display the video
 *  @param $field
 *    the field info from the requesting node
 *  @param $item
 *    the actual content from the field
 *  @return
 *    the html of the embedded video
 */
function emvideo_kewego_video($video_id, $width, $height, $field, $item, $node, $autoplay, $options = array()) {
  $options['video_id'] = isset($options['video_id']) ? $options['video_id'] : $video_id;
  $options['video_id'] = explode('.', $options['video_id']);
  $options['video_id'] = $options['video_id'][0];
  $options['width'] = isset($options['width']) ? $options['width'] : media_kewego_variable_get('width') ? media_kewego_variable_get('width') : $width;
  $options['height'] = isset($options['height']) ? $options['height'] : media_kewego_variable_get('height') ? media_kewego_variable_get('height') : $height;
  $options['field'] = isset($options['field']) ? $options['field'] : $field;
  $options['item'] = isset($options['item']) ? $options['item'] : $item;
  $options['node'] = isset($options['node']) ? $options['node'] : $node;
  $options['autoplay'] = isset($options['autoplay']) ? $options['autoplay'] : media_kewego_variable_get('autostart') == 1 ? 'true' : $autoplay;
  $options['fullscreen'] = media_kewego_variable_get('fullscreen') == 1 ? 'true' : 'false';
  $options['videoformat'] = media_kewego_variable_get('videoformat') ? media_kewego_variable_get('videoformat') : 'old';
  $options['language'] = media_kewego_variable_get('language') ? media_kewego_variable_get('language') : 'en';
  $output = theme('media_kewego_default', $options);
  return $output;
}
