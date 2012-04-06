<?php
// $Id: flowplayer.inc,v 1.1.2.2 2009/06/26 18:31:00 aaron Exp $

/**
 * @file
 *   This is an include file used by emfield.module.
 */

/**
 * Retrieves an associative array of types that are supported by Custom URLs.
 */
function _emaudio_flowplayer_default_types() {
  return array(
    'mp3' => t('MPEG-1 Audio Layer 3 (mp3)'),
    'wav' => t('Waveform Audio Format (wav)'),
    'ra' => t('Real Audio (ra)'),
    'mid' => t('Musical Instrument Digital Interface (mid)'),
    'ogg' => t('Ogg Vorbis (ogg)'),
  );
}

function emaudio_flowplayer_info() {
  $features = array(
    array(t('Thumbnails'), t('No'), ''),
    array(t('Autoplay'), t('Yes'), ''),
    array(t('RSS attachment'), t('No'), ''),
  );
  return array(
    'provider' => 'flowplayer',
    'name' => t('FlowPlayer'),
    'url' => '',
    'settings_description' => t('These settings specifically affect how custom audio files are displayed. When a field uses a URL it determines to be a link directly to an audio file, it will embed that file into the content. Installing the <a href="@flowplayer">Flowplayer module</a> will give the embedded player a nicer look.', array('@flowplayer' => 'http://drupal.org/project/flowplayer')),
    'supported_features' => $features,
    'weight' => 9,
  );
}

function emaudio_flowplayer_settings() {
  $form = array();
  $form['emaudio_flowplayer_supported_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Supported Types'),
    '#options' => _emaudio_flowplayer_default_types(),
    '#default_value' => _emaudio_flowplayer_supported_types(),
    '#description' => t('Select the audio types you wish to support. When a custom url with that type is entered into an embedded audio field, it will be parsed and displayed appropriately. If a type is not supported, then it will be ignored.'),
  );
  return $form;
}

/**
 * Retrieves the types that are supported by Custom URLs.
 */
function _emaudio_flowplayer_supported_types() {
  return variable_get('emaudio_flowplayer_supported_types', array_keys(_emaudio_flowplayer_default_types()));
}

function emaudio_flowplayer_extract($embed = '') {
  $types = implode('|', _emaudio_flowplayer_supported_types());
  $baseurl = preg_quote(url(null, array('absolute' => TRUE)), '@');

  return array(
    '@' . $baseurl . '(.*' . '\.(?:' . $types . ')' . '(\?.*)?)@i',
    '@(.*\.(?:'. $types .')(\?.*)?)@i'
  );
}

function emaudio_flowplayer_data($field, $item) {
  $data = array();
  // adding the version control
  $data['emaudio_flowplayer_data_version'] = 1;

  // attempt to get info from headers
  $response = emfield_request_header('flowplayer', $item['embed']);

  if ($response->code == 200) {
    $data['url'] = $item['embed'];
    $data['size'] = $response->headers['Content-Length'];
    $data['mime'] = $response->headers['Content-Type'];
  }
  // @todo replace ['type'] with converted mime info if available
  $types = implode('|', _emaudio_flowplayer_supported_types());
  $regex = '@\.('. $types .')@i';
  if (preg_match($regex, $item['embed'], $matches)) {
    $data['type'] = $matches[1];
  }
  return $data;
}

/**
 * hook emfield_PROVIDER_rss
 */
function emaudio_flowplayer_rss($item, $teaser = NULL) {
  if ($item['value']) {
    if ($item['data']['emaudio_flowplayer_data_version'] >= 1) {
      $data = $item['data'];
    }
    else {
      $data = emaudio_flowplayer_data(NULL, $item);
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

function theme_emaudio_flowplayer_flash($url = NULL, $width = "100%", $height = 0, $field = NULL, $data = array(), $node = NULL, $autoplay = FALSE) {
  // Display the audio using Flowplayer if it's available.
  if (module_exists('flowplayer')) {
    $config = array(
      'clip' => array(
        'autoPlay' => $autoplay,
        'url' => url($url, array('absolute' => TRUE)),
      ),
    );
    $attributes = array(
      'style' => "width:{$width};height:24px;",
    );
    return theme('flowplayer', $config, 'emaudio_flowplayer_flash', $attributes);
  }

  // Display the custom URL using the embed tag.
  switch($data['data']['type']) {
    case 'wav':
    case 'ra':
    case 'mp3':
    case 'mid':
      $autoplay = $autoplay ? 'true' : 'false';
      return "<embed src='$url' autostart='$autoplay' width='$width' height='24px'></embed>";
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
function emaudio_flowplayer_audio($url = NULL, $width = "100%", $height = 0, $field = NULL, $data = array(), $node = NULL, $autoplay = FALSE) {
  return theme('emaudio_flowplayer_flash', $url, $width, '24', $field, $data, $node, $autoplay);
}

/**
 * Implementation of hook_emfield_subtheme.
 */
function emaudio_flowplayer_emfield_subtheme() {
  return array(
    'emaudio_flowplayer_flash' => array(
      'arguments' => array('url' => NULL, 'width' => NULL, 'height' => NULL, 'field' => NULL, 'data' => NULL, 'node' => NULL, 'autoplay' => NULL),
      'file' => 'providers/flowplayer.inc'
    )
  );
}
