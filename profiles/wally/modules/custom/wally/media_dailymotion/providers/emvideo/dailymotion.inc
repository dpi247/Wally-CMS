<?php
// $Id: dailymotion.inc,v 1.0 2011/01/07 ODM $

/**
 * @file
 *   This include processes dailymotion.com media files for use by emfield.module.
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
function emvideo_dailymotion_info() {
  $features = array(
    array(t('Autoplay'), t('Yes'), ''),
    array(t('Thumbnails'), t('Yes'), t('')),
    array(t('Duration'), t('Yes'), ''),
    array(t('Full screen mode'), t('Yes'), t('You may customize the player to enable or disable full screen playback. Full screen mode is enabled by default.')),
  );
  return array(
    'module' => 'media_dailymotion',
    'provides' => array('emvideo'),
    'provider' => 'dailymotion',
    'name' => t('dailymotion'),
    'url' => MEDIA_DAILYMOTION_MAIN_URL,
    'settings_description' => t('These settings specifically affect videos displayed from <a href="@dailymotion" target="_blank">dailymotion</a>. You can learn more about its <a href="@api" target="_blank">API</a> here.', array('@dailymotion' => MEDIA_DAILYMOTION_MAIN_URL, '@api' => MEDIA_DAILYMOTION_API_INFO)),
    'supported_features' => $features,
  );
}

/**
 * hook emvideo_PROVIDER_settings
 * this should return a subform to be added to the emvideo_settings() admin settings page.
 * note that a form field will already be provided, at $form['PROVIDER'] (such as $form['dailymotion'])
 * so if you want specific provider settings within that field, you can add the elements to that form field.
 */
function emvideo_dailymotion_settings() {
  module_load_include('inc', 'media_dailymotion', 'includes/media_dailymotion.admin');
  return media_dailymotion_admin_form();
}

/**
 *  Implement hook emvideo_PROVIDER_data_version().
 */
function emvideo_dailymotion_data_version() {
  return MEDIA_DAILYMOTION_DATA_VERSION;
}

/**
 * hook emfield_PROVIDER_data
 *
 * provides an array to be serialised and made available with $item elsewhere
 */
function emvideo_dailymotion_data($field, $item) {
  $data = array();
  $emdata = _emvideo_dailymotion_request($item['embed']);
  $data['thumbnail']['url'] = $emdata['thumbnail_url'];
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
function emvideo_dailymotion_extract($video_id = '') {

  $return = array();
  $allowed_urls = media_dailymotion_variable_get('allowed_url');
  $allowed_urls = explode("\n", str_replace(array("\n", "\r\n"), "\n", trim($allowed_urls)));
  $delimiters = array('@', '+', '#');
  foreach ($allowed_urls as $allowed_url) {
    if ($allowed_url && $allowed_url != '') {
      foreach ($delimiters as $delim) {
        if (!strstr($allowed_url, $delim)) {
          if (!strstr($allowed_url, 'http')) $allowed_url = 'http://'.$allowed_url;
          $return[] = $delim.$allowed_url.'(.*)'.$delim.'i';
          break;
        }
      }
    }
    //print_r($return); exit();
    $temp_return = '';
    $parsed_url = parse_url($allowed_url);
    if (isset($parsed_url['host']) && !empty($parsed_url['host'])) {
      $temp_return = '@'.$parsed_url['host'];
      if (isset($parsed_url['path']) && !empty($parsed_url['path'])) {
        $temp_return .= $parsed_url['path'];
      }
      $temp_return .= '([^"\& ]+)@i';
      $return[] = $temp_return;
    }
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
function emvideo_dailymotion_video($video_id, $width, $height, $field, $item, $node, $autoplay, $options = array()) {
  /*$options['video_id'] = isset($options['video_id']) ? $options['video_id'] : $video_id;
  $options['video_id'] = explode('.', $options['video_id']);
  $options['video_id'] = $options['video_id'][0];
  $options['width'] = isset($options['width']) ? $options['width'] : media_dailymotion_variable_get('width') ? media_dailymotion_variable_get('width') : $width;
  $options['height'] = isset($options['height']) ? $options['height'] : media_dailymotion_variable_get('height') ? media_dailymotion_variable_get('height') : $height;
  $options['field'] = isset($options['field']) ? $options['field'] : $field;
  $options['item'] = isset($options['item']) ? $options['item'] : $item;
  $options['node'] = isset($options['node']) ? $options['node'] : $node;
  $options['autoplay'] = isset($options['autoplay']) ? $options['autoplay'] : media_dailymotion_variable_get('autostart') == 1 ? 'true' : $autoplay;
  $options['fullscreen'] = media_dailymotion_variable_get('fullscreen') == 1 ? 'true' : 'false';
  $options['videoformat'] = media_dailymotion_variable_get('videoformat') ? media_dailymotion_variable_get('videoformat') : 'old';
  $options['language'] = media_dailymotion_variable_get('language') ? media_dailymotion_variable_get('language') : 'en';
  $output = theme('media_dailymotion_default', $options);*/
  
  $item_url = $item['embed'];
  $dailymotion_result = _emvideo_dailymotion_request($item_url);

  IF ($dailymotion_result["html"] == ""){
    $output = emvideo_dailymotion_error_check($item_url);
  } else {
    $output = theme('media_dailymotion_default', $dailymotion_result);
  }

  return $output;
}

/**
 *  Make the GET request to the embed.ly API, get back the xml result and parse it
 *  
 *  @param $src_url
 *    the url from the user's input
 *  
 *  @param $return
 *    array with the casted as string simplexml objects
 *  
 */
function _emvideo_dailymotion_request($src_url){
  preg_match('/^http:\/\/www\.dailymotion\.tv\/video\/(.*)_', $src_url,$urlT);
  $id = $urlT[1]; 

  
  $embedly_result = array();
  $url = "http://www.dailymotion.com/services/oembed?url=$src_url&format=xml";
  $xml = simplexml_load_file($url);  

  $embedly_result = array(
   'version' => (string)$xml->version,
   'type' => (string)$xml->type, 
   'video_id' =>$id,
   'provider_url' => (string)$xml->provider_url,
   'provider_name' => (string)$xml->provider_name,
   'title' => (string)$xml->title,
   'author' => (string)$xml->author_name,
   'author_url' => (string)$xml->author_url,
   'height' => (string)$xml->height,
   'width' => (string)$xml->width,
   'html' => (string)$xml->html,
   'thumbnail_url' => (string)$xml->thumbnail_url,
   'thumbnail_width' => (string)$xml->thumbnail_width,   
   'thumbnail_height' => (string)$xml->thumbnail_height
  );
  
  return $embedly_result;
}

function emvideo_dailymotion_error_check($url) {
  global $user;
  $error = t('This url ('.$url.') seems to be not supported by the Embedly API. Please retry or choose an other one.');
  if (user_access('administer site configuration')) {
    drupal_set_message($error, 'error');
  }
}