<?php
 
/**
 * Form allowing to change default values of the wallycrop action from
 * any preset in imagecache.
 * 
 * @param $data
 *   The actual values of the wallycrop.
 * 
 * @return
 *   The form.
 */  
function wallycontenttypes_wallycrop_form($data = array()) {
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
    '#description' => t('Enter an offset in pixels or use a keyword: <em>left</em>, <em>center</em>, <em>center_max</em>,or <em>right</em>.'),
  );
  $form['yoffset'] = array(
    '#type' => 'textfield',
    '#title' => t('Y offset'),
    '#default_value' => $data['yoffset'],
    '#description' => t('Enter an offset in pixels or use a keyword: <em>top</em>, <em>center</em>, <em>center_max</em>, or <em>bottom</em>.'),
  );
  return $form;
}

/**
 * Theme of the wallycrop form.
 * 
 * @param $element
 * 
 * @return
 *   The theme.
 */ 
function theme_wallycontenttypes_wallycrop($element) {
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
 *   TRUE in case of success, FALSE in case of failure.
 */ 
function wallycontenttypes_wallycrop_image(&$image, $data) {
	$fid = db_fetch_object(db_query("SELECT `fid` FROM {files} WHERE `filepath` = '%s'", $image->source));
  if (db_column_exists('content_type_wally_photoobject', 'field_photofile_fid')) {
    $cropings = db_fetch_object(db_query("SELECT `field_croping_value` FROM {content_type_wally_photoobject} WHERE `field_photofile_fid` = '%s'", $fid->fid));
  } elseif (db_column_exists('content_field_photofile', 'field_photofile_fid')) {
    $nid = db_fetch_object(db_query("SELECT `nid` FROM {content_field_photofile} WHERE `field_photofile_fid` = '%s'", $fid->fid));
    $cropings = db_fetch_object(db_query("SELECT `field_croping_value` FROM {content_type_wally_photoobject} WHERE `nid` = '%s'", $nid->nid));
  }

  $preset = imagecache_preset($data['presetid']);
  $presetname = $preset['presetname'];
  $imgcropings = json_decode($cropings->field_croping_value, TRUE);

  if (isset($imgcropings[$presetname])) {
    $crop = array();
    $crop = $imgcropings[$presetname];
    $data['xoffset'] = $crop[0];
    $data['yoffset'] = $crop[1];
    $data['width'] = $crop[2];
    $data['height'] = $crop[3];
  }

  // Si l'action a un center_max comme parametre, alors on doit maximiser le crop dans l image
  if ($data["xoffset"]=="center_max" || $data["yoffset"] == "center_max") {
	  if (!imageapi_image_scale_and_crop($image, $data['width'], $data['height'])) {
	    watchdog('WallyContentTypes', 'WallyCrop Action : action failed. image: %image, data: %data.', array('%image' => $image->source, '%data' => print_r($data, TRUE)), WATCHDOG_ERROR);
	    return FALSE;
	  }
	} else {
	  if (!imageapi_image_crop($image, $data['xoffset'], $data['yoffset'], $data['width'], $data['height'])) {
	    watchdog('WallyContentTypes', 'WallyCrop Action : action failed. image: %image, data: %data.', array('%image' => $image->source, '%data' => print_r($data, TRUE)), WATCHDOG_ERROR);
	    return FALSE;
	  }
 	} 

  return TRUE;
}

/**
 * Form allowing to change default values of the cropcopy action from
 * any preset in imagecache.
 * 
 * @param $data
 *   The actual values of the cropcopy.
 * 
 * @return
 *   The form.
 */  
function wallycontenttypes_cropcopy_form($data = array()) {
  $form = array();

  $data += array(
    'copied_preset' => 0,
  );

  $presets = imagecache_presets();
  $existing_actions = array();
  foreach ($presets as $preset) {
    if ($preset['actions']) {
      foreach ($preset['actions'] as $action_id => $action) {
        if ($action['action'] == 'wallycontenttypes_wallycrop') {
          $ratio = round($action['data']['width']/$action['data']['height'], 2);
          $existing_actions[$preset['presetid']] = $preset['presetname'].' (ratio: '.$ratio.')';
          $actions_ids[$preset['presetid']] = $action_id;
          break;
        }
      }
    }
  }
  
  $form['copied_preset'] = array(
    '#type' => 'select',
    '#title' => t('Choose the action to copy'),
    '#options' => $existing_actions,
    '#default_value' => $data['copied_preset']
  );
  
  $form['actions_ids'] = array(
    '#type' => 'value',
    '#value' => $actions_ids,
  );
  
  $form['#submit'] = array('wallycontenttypes_cropcopy_form_submit');
  
  return $form;
}

/**
 * Theme of the cropcopy form.
 * 
 * @param $element
 * 
 * @return
 *   The theme.
 */ 
function theme_wallycontenttypes_cropcopy($element) {
  $data = $element['#value'];
  return t('Copied preset: @copied_preset', array('@copied_preset' => $data['copied_preset']));
}

/**
 * The actual action performed on the image. If the datas for this preset
 * are different from the defaults, they are used for the corp, else the
 * defaults are used.
 * 
 * @param &$image
 *   The image being croped.
 * @param $data
 *   The datas (copied crop infos) of the crop.
 * 
 * @return
 *   TRUE in case of success, FALSE in case of failure.
 */ 
function wallycontenttypes_cropcopy_image(&$image, $data) {
	$fid = db_fetch_object(db_query("SELECT `fid` FROM {files} WHERE `filepath` = '%s'", $image->source));
  if (db_column_exists('content_type_wally_photoobject', 'field_photofile_fid')) {
    $cropings = db_fetch_object(db_query("SELECT `field_croping_value` FROM {content_type_wally_photoobject} WHERE `field_photofile_fid` = '%s'", $fid->fid));
  } elseif (db_column_exists('content_field_photofile', 'field_photofile_fid')) {
    $nid = db_fetch_object(db_query("SELECT `nid` FROM {content_field_photofile} WHERE `field_photofile_fid` = '%s'", $fid->fid));
    $cropings = db_fetch_object(db_query("SELECT `field_croping_value` FROM {content_type_wally_photoobject} WHERE `nid` = '%s'", $nid->nid));
  }

  $copied_preset = imagecache_preset($data['copied_preset']);
  $copied_presetname = $copied_preset['presetname'];
  $imgcropings = json_decode($cropings->field_croping_value, TRUE);
  $actions_ids = $data['actions_ids'];
  $data['action_id'] = $actions_ids[$data['copied_preset']];
  
  if (isset($imgcropings[$copied_presetname])) {
    $copied_defaults = $copied_preset['actions'][$data['action_id']]['data'];
    $crop = array();
    $crop = $imgcropings[$copied_presetname];
    $data['xoffset'] = $crop[0];
    $data['yoffset'] = $crop[1];
    $data['width'] = $crop[2];
    $data['height'] = $crop[3];
  } else {
    watchdog('WallyContentTypes', 'CropCopy Action : copied preset not set. image: %image, data: %data.', array('%image' => $image->source, '%data' => print_r($data, TRUE)), WATCHDOG_ERROR);
    return FALSE;
  }

  // Si l'action a un center_max comme parametre, alors on doit maximiser le crop dans l image
  if ($data["xoffset"]=="center_max" || $data["yoffset"] == "center_max") {
	  if (!imageapi_image_scale_and_crop($image, $data['width'], $data['height'])) {
	    watchdog('WallyContentTypes', 'CropCopy Action : action failed. image: %image, data: %data.', array('%image' => $image->source, '%data' => print_r($data, TRUE)), WATCHDOG_ERROR);
	    return FALSE;
	  }
	} else {
	  if (!imageapi_image_crop($image, $data['xoffset'], $data['yoffset'], $data['width'], $data['height'])) {
	    watchdog('WallyContentTypes', 'CropCopy Action : action failed. image: %image, data: %data.', array('%image' => $image->source, '%data' => print_r($data, TRUE)), WATCHDOG_ERROR);
	    return FALSE;
	  }
 	} 

  return TRUE;
}
