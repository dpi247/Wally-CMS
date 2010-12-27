<?php
 
/**
 * Form allowing to change default values of the customcrop action from
 * any preset in imagecache.
 * 
 * @param $data
 *   The actual values of the customcrop.
 * 
 * @return
 *   The form.
 */  
function wallycontenttypes_customcrop_form($data = array()) {
  $data += array(
    'width' => '',
    'height' => '',
    'xoffset' => '',
    'yoffset' => '',
  );
  $form['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => $data['width'],
    '#description' => t('Enter a width in pixels or as a percentage. i.e. 500 or 80%.'),
  );
  $form['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#default_value' => $data['height'],
    '#description' => t('Enter a height in pixels or as a percentage. i.e. 500 or 80%.'),
  );
  $form['xoffset'] = array(
    '#type' => 'textfield',
    '#title' => t('X offset'),
    '#default_value' => $data['xoffset'],
    '#description' => t('Enter an offset in pixels or use a keyword: <em>left</em>, <em>center</em>, or <em>right</em>.'),
  );
  $form['yoffset'] = array(
    '#type' => 'textfield',
    '#title' => t('Y offset'),
    '#default_value' => $data['yoffset'],
    '#description' => t('Enter an offset in pixels or use a keyword: <em>top</em>, <em>center</em>, or <em>bottom</em>.'),
  );
  return $form;
}

/**
 * Theme of the customcrop form.
 * 
 * @param $element
 * 
 * @return
 *   The theme.
 */ 
function theme_wallycontenttypes_customcrop($element) {
  $data = $element['#value'];
  return t('width: @width, height: @height, xoffset: @xoffset, yoffset: @yoffset', array(
    '@width' => $data['width'],
    '@height' => $data['height'],
    '@xoffset' => $data['xoffset'],
    '@yoffset' => $data['yoffset'],
  ));
}

/**
 * The actual action performed on the image. If the datas for this preset
 * are different from the defaults, they are used for the corp, else the
 * defaults are used.
 * 
 * @param &$image
 *   The image being croped.
 * @param $data
 *   The datas (width, height, xoffset and yoffset) of the crop.
 * 
 * @return
 *   The croped image.
 */ 
function wallycontenttypes_customcrop_image(&$image, $data) {
  //$node=imagecache_actions_node_from_filepath($image->source);
  
  $fid = db_fetch_object(db_query("SELECT `fid` FROM `files` WHERE `filepath` = '%s'", $image->source));
  $cropings = db_fetch_object(db_query("SELECT `field_croping_value` FROM `content_type_wally_photoobject` WHERE `field_photofile_fid` = '%s'", $fid->fid));
  $preset = imagecache_preset($data['presetid']);
  $presetname = $preset['presetname'];
  $imgcropings = json_decode($cropings->field_croping_value);
watchdog('ODM', print_r($image, TRUE).' +++++++++++++ '.print_r($fid, TRUE).' +++++++++++++ '.print_r($cropings, TRUE).' ++++++++++++++ '.print_r($preset, TRUE), array('%image' => $image->source, '%data' => print_r($data, TRUE)), WATCHDOG_ERROR);
  if (isset($imgcropings->$presetname)) {
    $crop = array();
    $crop = $imgcropings->$presetname;
    $data['xoffset'] = $crop[0];
    $data['yoffset'] = $crop[1];
    $data['width'] = $crop[2];
    $data['height'] = $crop[3];
  }
  if (!imageapi_image_crop($image, $data['xoffset'], $data['yoffset'], $data['width'], $data['height'])) {
      watchdog('wallycontenttypes_actions', 'wallycontenttypes_customcrop failed. image: %image, data: %data.', array('%image' => $image->source, '%data' => print_r($data, TRUE)), WATCHDOG_ERROR);
    return FALSE;
  }
  return TRUE;
}
