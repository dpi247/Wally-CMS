<?php
// $Id: googlemaps.inc,v 1.1.2.2 2009/06/26 18:31:00 aaron Exp $

/**
 * @file
 *   This is an include file used by emfield.module.
 */

function emimage_flickr_info() {
  $features = array(
  array(t('Flickr'), t('Yes'), ''),
  );
  return array(
    'provider' => 'flickr',
    'name' => t('flickr'),
    'url' => '',
    'settings_description' => t('These settings specifically affect how flicker files are displayed. When a field uses a URL it determines to be a link directly to an audio file, it will embed that file into the content. Installing the <a href="@googlemaps">googlemaps module</a> will give the embedded player a nicer look.', array('@googlemaps' => 'http://drupal.org/project/googlemaps')),
    'supported_features' => $features,
    'weight' => 9,
  );
}

function emimage_flickr_settings() {
  $form = array();
  return $form;
}

function emimage_flickr_extract($embed = '') {
  
  $return = drupal_get_path('module', 'media_flowplayer').'/images/copyright.jpg';

  $embed_url = parse_url($embed);
  $domain_array = explode('.',$embed_url['host']);
  $subdomain = $domain_array[0];
  $domain = $domain_array[1];
  
  if ($domain == "flickr"){
    $url = "http://flickr.com/services/oembed?url=http%3A//";
    $data = 'flickr.com'.$embed_url['path'];
    if ($xml = simplexml_load_file($url.$data)){
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
         'thumbnail_height' => (string)$xml->thumbnail_height,
         'url' => (string)$xml->url,
      );
      if ($embedly_result['url'] != NULL){
        $return = $embedly_result['url'];
      }
    }
  } 
  return $return;
}

/**
* This hook of the emimage_PROVIDER_embedded_link($code) returns a link to
* view the content at the provider's site.
*
*  @param  $code The string containing the content to watch
*  @return       a string containing the URL to view the image at the original
*          provider's site.
*/
function emimage_flickr_embedded_link($code, $data) {
  return $code;
}

/**
 *  This function is an implementation of the emimage_PROVIDER_image_url()
 *
 *  @param  $code The code of the image
 *  @param  $data Any stored data for the image, which may already have the title
 *  @return       The url directly to the image to display
 */
function emimage_flickr_image_url($code, $data) {
  return $code;
}
/**
 * Retrieves an associative array of types that are supported by Custom URLs.
 */
function _emimage_flickr_default_types() {
  return array(
    'flickr' => t('flickr word (dovc)'),
  );
}

/**
 * Retrieves the types that are supported by Custom URLs.
 */
function _emimage_flickr_supported_types() {
  return variable_get('emimage_flickr_supported_types', array_keys(_emimage_flickr_default_types()));
}
