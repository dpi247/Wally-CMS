<?php
// $Id: embedly.inc,v 1.1.2.2 2009/06/26 18:31:00 aaron Exp $

/**
 * @file
 *   This is an include file used by emfield.module.
 */

function emaudio_embedly_info() {
  return array(
    'module' => 'media_embedly',
    'provides' => array('emaudio'),
    'provider' => 'embedly',
    'name' => t('Embedly'),
    'url' => MEDIA_EMBEDLY_MAIN_URL,
    'settings_description' => t('These settings specifically affect audios displayed from <a href="@embedly" target="_blank">Embedly</a>. You can learn more about its <a href="@api" target="_blank">API</a> here.', array('@embedly' => MEDIA_EMBEDLY_MAIN_URL, '@api' => MEDIA_EMBEDLY_API_INFO)),
    'supported_features' => '',
    'weight' => 9,
  );
}

function emaudio_embedly_settings() {
	module_load_include('inc', 'media_embedly', 'includes/media_embedly.admin');
  return media_embedly_audio_admin_form();
}


function emaudio_embedly_extract($embed = '') {
  $return = array();
  $allowed_urls = media_embedly_variable_get('allowed_audio_url');
  $allowed_urls = explode("\n", str_replace(array("\n", "\r\n"), "\n", trim($allowed_urls)));
  foreach ($allowed_urls as $allowed_url) {
    $temp_return = '';
    $temp_return = $allowed_url;
    $temp_return = "+".$allowed_url."+";
    $temp_return = str_replace('*','(.*)',$temp_return);
    $return[] = $temp_return;
  }
  return $return;
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
function emaudio_embedly_audio($url = NULL, $width = 0, $height = 0, $field = NULL, $data = array(), $node = NULL, $autoplay = FALSE) {
	$data_url = $data['embed'];
  $embedly_result = _media_embedly_request($data_url);
  $output = theme('media_embedly_default', $embedly_result);
  return $output;
}
