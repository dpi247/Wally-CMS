<?php
// $Id: press_imagecache_actions.inc,v 1.0 2010/09/21 11:50:00 rso Exp $

/**
 * @file
 * Imagecache actions implementation.
 *
 * @param $data values passed on by imagecache
 */

function imagecrop_wally_form($data) {
  $form['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Crop Width'),
    '#required' => TRUE,
    '#default_value' => $data['width'],
    '#description' => t('Enter a width in pixels or as a percentage. i.e. 500 or 80%.'),
  );
  $form['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Crop Height'),
    '#required' => TRUE,
    '#default_value' => $data['height'],
    '#description' => t('Enter a height in pixels or as a percentage. i.e. 500 or 80%.'),
  );
  $form['xoffset'] = array(
    '#type' => 'textfield',
    '#title' => t('Crop X offset'),
    '#default_value' => $data['xoffset'],
    '#description' => t('Enter an offset in pixels or use a keyword: <em>left</em>, <em>center</em>, or <em>right</em>.'),
  );
  $form['yoffset'] = array(
    '#type' => 'textfield',
    '#title' => t('Crop Y offset'),
    '#default_value' => $data['yoffset'],
    '#description' => t('Enter an offset in pixels or use a keyword: <em>top</em>, <em>center</em>, or <em>bottom</em>.'),
  );
  $form['swidth'] = array(
    '#type' => 'textfield',
    '#title' => t('Scale Width'),
    '#required' => TRUE,
    '#default_value' => $data['swidth'],
    '#description' => t('Enter a final width in pixels or as a percentage. i.e. 500 or 80%.'),
  );
  $form['sheight'] = array(
    '#type' => 'textfield',
    '#title' => t('Scale Height'),
    '#required' => TRUE,
    '#default_value' => $data['sheight'],
    '#description' => t('Enter a final height in pixels or as a percentage. i.e. 500 or 80%.'),
  );
  return $form;
}

/**
 * Display properties of a single action
 *
 * @param $element passed on by imagecache
 * @return string
 */
function theme_imagecrop_wally($element) {
  $data = $element['#value'];
  return 'width: '. $data['width'] .', height: '. $data['height'] .', xoffset: '. $data['xoffset'] .', yoffset: '. $data['yoffset'] .', resizable: '. $data['resizable'] .', aspect ratio: '. $data['aspect'] .', don\'t crop if region is not set: '. $data['disable_if_no_data'];
}

/**
 * Callback to perform the crop on an image
 *
 * @param $image current image resource e.g.:
 *	  Object(
 *	    [source] => sites/default/files/article Russia.jpg
 *	    [info] => Array
 *	        (
 *	            [width] => 474
 *	            [height] => 300
 *	            [extension] => jpg
 *	            [file_size] => 37012
 *	            [mime_type] => image/jpeg
 *	        )
 *	    [toolkit] => imageapi_gd
 *	    [resource] => Resource id #358
 *		)
 * @param $data values associated with this action, e.g.:
 *	  Array(
 *		    [width] => 200
 *		    [height] => 100
 *		    [xoffset] => 0
 *		    [yoffset] => 100
 *		    [swidth] => 120
 *		    [sheight] => 60
 *		)
 * @return false or true
 */
function imagecrop_wally_image(&$image, $data) {
  // TODO: Load the node of the Photo Object which displays this image, in order to get the Crops infos from $node->field_wally_crops
  // These x,y,w,h will be used to crop the image instead of the values stored in the action
/*
echo '<pre>'.__FUNCTION__." :\n";
echo '$image='; print_r($image); echo "\n";
echo '$data='; print_r($data); echo "\n";
die();
*/
  if (!imageapi_image_crop($image, $data['xoffset'], $data['yoffset'], $data['width'], $data['height'])) {
    watchdog('imagecrop_wally', t('image_crop failed. image: %image, data: %data.', array('%path' => $image->source, '%data' => print_r($data, TRUE))), WATCHDOG_ERROR);
    return FALSE;
  }

  if (!imageapi_image_scale($image, $data['swidth'], $data['sheight'], TRUE)) {
    watchdog('imagecrop_wally', t('image_scale failed. image: %image, data: %data.', array('%path' => $image->source, '%data' => print_r($data, TRUE))), WATCHDOG_ERROR);
	return FALSE;
  }

  return TRUE;
}
