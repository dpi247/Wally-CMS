<?php
// $Id: embedly.inc,v 1.0 2011/01/07 ODM - JTH $

/**
 * @file
 *   This include processes Embed.ly xml media files for use by emfield.module.
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
function emvideo_embedly_info() {
  return array(
    'module' => 'media_embedly',
    'provides' => array('emvideo'),
    'provider' => 'embedly',
    'name' => t('Embedly'),
    'url' => MEDIA_EMBEDLY_MAIN_URL,
    'settings_description' => t('These settings specifically affect videos displayed from <a href="@embedly" target="_blank">Embedly</a>. You can learn more about its <a href="@api" target="_blank">API</a> here.', array('@embedly' => MEDIA_EMBEDLY_MAIN_URL, '@api' => MEDIA_EMBEDLY_API_INFO)),
    'supported_features' => '',
  );
}

/**
 * hook emvideo_PROVIDER_settings
 * this should return a subform to be added to the emvideo_settings() admin settings page.
 * note that a form field will already be provided, at $form['PROVIDER'] (such as $form['embedly'])
 * so if you want specific provider settings within that field, you can add the elements to that form field.
 */
function emvideo_embedly_settings() {
  module_load_include('inc', 'media_embedly', 'includes/media_embedly.admin');
  return media_embedly_video_admin_form();
}

/**
 *  Implement hook emvideo_PROVIDER_data_version().
 */
function emvideo_embedly_data_version() {
  return MEDIA_EMBEDLY_DATA_VERSION;
}

/**
 * hook emfield_PROVIDER_data
 *
 * provides an array to be serialised and made available with $item elsewhere
 */
function emvideo_embedly_data($field, $item) {
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
function emvideo_embedly_extract($video_id = '') {
  $return = array();
  $allowed_urls = media_embedly_variable_get('allowed_video_url');
  $allowed_urls = explode("\n", str_replace(array("\n", "\r\n"), "\n", trim($allowed_urls)));
  foreach ($allowed_urls as $allowed_url) {
    $temp_return = '';
    $temp_return = $allowed_url;
    $temp_return = "+".$allowed_url."+";
    $temp_return = str_replace('*','(.*)',$temp_return);
    $return[] = $temp_return;
  }
  
  $width = media_embedly_variable_get('width');  
  return $return;
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
function  _emvideo_embedly_request($src_url){
	$embedly_result = array();
  $url = "http://api.embed.ly/1/oembed";
  $data = "?url=".$src_url."&maxwidth=600&format=xml";
  $data = str_replace(":","%3A",$data);
  $data = str_replace("/","%2F",$data);
  $xml = simplexml_load_file($url.$data);
  $embedly_result = array(
   'version' => (string)$xml->version,
   'type' => (string)$xml->type, 
   'provider_url' => (string)$xml->provider_url,
   'provider_name' => (string)$xml->provider_name,
   'description' => (string)$xml->description,
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

function emvideo_embedly_error_check($url) {
    global $user;
    $error = t('This url ('.$url.') seems to be not supported by the Embedly API. Please retry or choose an other one.');
    if (user_access('administer site configuration')) {
      drupal_set_message($error, 'error');
    }
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
function emvideo_embedly_video($video_id, $width, $height, $field, $item, $node, $autoplay, $options = array()) {
	$item_url = $item['embed'];
  $embedly_result = _media_embedly_video_request($item_url);
  IF ($embedly_result["html"] == ""){
    $output = emvideo_embedly_error_check($item_url);
  } else {
    $output = theme('media_embedly_default', $embedly_result);
  }
  return $output;
}
