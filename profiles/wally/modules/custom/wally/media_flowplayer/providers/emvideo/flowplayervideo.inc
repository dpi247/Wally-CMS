<?php
// $Id: flowplayer.inc,v 1.1.2.2 2009/06/26 18:31:00 aaron Exp $

/**
 * @file
 *   This is an include file used by emfield.module.
 */

/**
 * Retrieves an associative array of types that are supported by Custom URLs.
 */
function _emvideo_flowplayervideo_default_types() {
  return array(
    'mp4' => t('MPEG-4 Part 14 (mp4)'),
    'mov' => t('Quicktime Movie (mov)'),
  );
}

function emvideo_flowplayervideo_info() {
  $features = array(
    array(t('Thumbnails'), t('No'), ''),
    array(t('Autoplay'), t('Yes'), ''),
    array(t('RSS attachment'), t('No'), ''),
  );
  return array(
    'provider' => 'flowplayervideo',
    'name' => t('FlowPlayer Video'),
    'url' => '',
    'settings_description' => t('These settings specifically affect how custom video files are displayed. When a field uses a URL it determines to be a link directly to an video file, it will embed that file into the content. Installing the <a href="@flowplayer">Flowplayer module</a> will give the embedded player a nicer look.', array('@flowplayer' => 'http://drupal.org/project/flowplayer')),
    'supported_features' => $features,
    'weight' => 9,
  );
}

function emvideo_flowplayervideo_settings() {
  $form = array();
  $form['emvideo_flowplayervideo_supported_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Supported Types'),
    '#options' => _emvideo_flowplayervideo_default_types(),
    '#default_value' => _emvideo_flowplayervideo_supported_types(),
    '#description' => t('Select the video types you wish to support. When a custom url with that type is entered into an embedded video field, it will be parsed and displayed appropriately. If a type is not supported, then it will be ignored.'),
  );
  return $form;
}

/**
 * Retrieves the types that are supported by Custom URLs.
 */
function _emvideo_flowplayervideo_supported_types() {
  return variable_get('emvideo_flowplayervideo_supported_types', array_keys(_emvideo_flowplayervideo_default_types()));
}

function emvideo_flowplayervideo_extract($embed = '') {
  $types = implode('|', _emvideo_flowplayervideo_supported_types());
  $baseurl = preg_quote(url(null, array('absolute' => TRUE)), '@');

  return array(
    '@' . $baseurl . '(.*' . '\.(?:' . $types . ')' . '(\?.*)?)@i',
    '@(.*\.(?:'. $types .')(\?.*)?)@i'
  );
}

function emvideo_flowplayervideo_data($field, $item) {
  $data = array();
  // adding the version control
  $data['emvideo_flowplayervideo_data_version'] = 1;

  // attempt to get info from headers
  $response = emfield_request_header('flowplayer', $item['embed']);

  if ($response->code == 200) {
    $data['url'] = $item['embed'];
    $data['size'] = $response->headers['Content-Length'];
    $data['mime'] = $response->headers['Content-Type'];
  }
  // @todo replace ['type'] with converted mime info if available
  $types = implode('|', _emvideo_flowplayervideo_supported_types());
  $regex = '@\.('. $types .')@i';
  if (preg_match($regex, $item['embed'], $matches)) {
    $data['type'] = $matches[1];
  }
  return $data;
}

/**
 * hook emfield_PROVIDER_rss
 */
function emvideo_flowplayervideo_rss($item, $teaser = NULL) {
  if ($item['value']) {
    if ($item['data']['emvideo_flowplayervideo_data_version'] >= 1) {
      $data = $item['data'];
    }
    else {
      $data = emvideo_flowplayervideo_data(NULL, $item);
    }

    $file = array();
    if ($data['size'] > 0) {
      $file['filepath'] = $data['url'];
      $file['filesize'] = $data['size'];
      $file['filemime'] = $data['mime'];
    }

    return $file;
  }
}

function theme_emvideo_flowplayervideo_flash($url = NULL, $width = "100%", $height = 0, $field = NULL, $data = array(), $node = NULL, $autoplay = FALSE) {
  
  // Display the custom URL using the embed tag.
  switch($data['data']['type']) {
    case 'mp4':
      $autoplay = $autoplay ? 'true' : 'false';
      return '<script src="http://embed.flowplayer.org/5.2.0/embed.min.js">
   				<div class="flowplayer" style="width: 100%; height: 100%;">
      				<video>
         				<source type="video/mp4" src="/'.$url.'"/>   
      				</video>
   				</div>
			  </script>';
      break;
    case 'mov':
      $autoplay = $autoplay ? 'true' : 'false';
      return '<script src="http://embed.flowplayer.org/5.2.0/embed.min.js">
               <div class="flowplayer" style="width: 100%; height: 100%;">
                 <video id="sampleMovie" width="640" height="360" preload controls>
                   <source  src="/'.$url.'" type=\'video/mp4; codecs="avc1.42E01E, mp4a.40.2"\'/>   
                 </video>
               </div>
             </script>';
  }
}


/**
 * hook emvideo_PROVIDER_video
 * this actually displays the full/normal-sized video we want, usually on the default page view
 *  @param $embed
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
function emvideo_flowplayervideo_video($url = NULL, $width = "100%", $height = 0, $field = NULL, $data = array(), $node = NULL, $autoplay = FALSE) {
  
  return theme('emvideo_flowplayervideo_flash', $url, $width, '24', $field, $data, $node, $autoplay);
}

/**
 * Implementation of hook_emfield_subtheme.
 */
function emvideo_flowplayervideo_emfield_subtheme() {
  return array(
    'emvideo_flowplayervideo_flash' => array(
      'arguments' => array('url' => NULL, 'width' => NULL, 'height' => NULL, 'field' => NULL, 'data' => NULL, 'node' => NULL, 'autoplay' => NULL),
      'file' => 'providers/flowplayer.inc'
    )
  );
}
