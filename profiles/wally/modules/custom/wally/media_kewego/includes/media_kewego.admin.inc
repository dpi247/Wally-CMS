<?php
// $Id: media_kewego.admin.inc,v 1.0 2011/01/07 ODM $

/**
 * @file
 * Administrative functions for Media: Kewego.
 */

/**
 * The administrative settings form for Media: Kewego.
 */
function media_kewego_settings() {
  $form = media_kewego_admin_form();
  $form['settings_info'] = array(
    '#type' => 'item',
    '#value' => t('These settings specifically affect videos displayed from !kewego. You can also read more about its !api.', array('!kewego' => l(t('Kewego.com'), MEDIA_KEWEGO_MAIN_URL, array('attributes' => array('target' => '_blank'))), '!api' => l(t("developer's API"), MEDIA_KEWEGO_API_INFO, array('attributes' => array('target' => '_blank'))))),
    '#weight' => -10,
  );
  
  return system_settings_form($form);
}

/**
 * This form will be displayed both at /admin/settings/media_kewego and
 * admin/content/emfield.
 */
function media_kewego_admin_form() {
  $form = array();
  
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Player settings'),
    '#description' => t('General parameters used to display a Kewego video.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $form['settings'][media_kewego_variable_name('allowed_url')] = array(
    '#type' => 'textarea',
    '#title' => t('Allowed URLs'),
    '#default_value' => media_kewego_variable_get('allowed_url', ''),
    '#description' => t('Enter here the URL from which the videos may come. One URL by line.'),
    '#required' => TRUE,
  );
  
  $form['settings'][media_kewego_variable_name('player_key')] = array(
    '#type' => 'textfield',
    '#title' => t('Player Key'),
    '#default_value' => media_kewego_variable_get('player_key', ''),
  );
  
  $form['settings'][media_kewego_variable_name('skin_key')] = array(
    '#type' => 'textfield',
    '#title' => t('Skin Key'),
    '#default_value' => media_kewego_variable_get('skin_key', ''),
  );
  
  $videoformat_options = array(
    'old' => t('Old (flv)'),
    'normal' => t('Normal (mp4)'),
    'high' => t('High (HD)'),
  );
  $form['settings'][media_kewego_variable_name('videoformat')] = array(
    '#type' => 'select',
    '#title' => t('Prefered Format'),
    '#default_value' => media_kewego_variable_get('videoformat', 'old'),
    '#description' => t('Choose the default format. If a video isn\'t available in the chosen format the lower format will be used.'),
    '#options' => $videoformat_options,
  );
  
  $form['settings'][media_kewego_variable_name('width')] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => media_kewego_variable_get('width', 400),
    '#description' => t('The Player width.'),
  );
  
  $form['settings'][media_kewego_variable_name('height')] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#default_value' => media_kewego_variable_get('height', 300),
    '#description' => t('The player height.'),
  );
  
  $form['settings'][media_kewego_variable_name('autostart')] = array(
    '#type' => 'checkbox',
    '#title' => t('Autoplay'),
    '#default_value' => media_kewego_variable_get('autostart', 0),
    '#description' => t('Automatically start the video when loaded.'),
  );
  
  $form['settings'][media_kewego_variable_name('fullscreen')] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow full screen'),
    '#default_value' => media_kewego_variable_get('fullscreen', 0),
    '#description' => t('Allow the user to watch the video full screen.'),
  );
  
  $wmode_options = array(
    'opaque' => 'Opaque',
    'window' => 'Window',
    'transparent' => 'Transparent',
  );
  $form['settings'][media_kewego_variable_name('wmode')] = array(
    '#type' => 'select',
    '#title' => t('WMode'),
    '#options' => $wmode_options,
    '#default_value' => media_kewego_variable_get('wmode', 'opaque'),
  );
  
  $form['settings'][media_kewego_variable_name('advertise')] = array(
    '#type' => 'checkbox',
    '#title' => t('Advertise'),
    '#default_value' => media_kewego_variable_get('advertise', 0),
  );
  
  $form['settings'][media_kewego_variable_name('preload')] = array(
    '#type' => 'textfield',
    '#title' => t('Preload'),
    '#default_value' => media_kewego_variable_get('preload', 'none'),
  );
  
  $form['settings'][media_kewego_variable_name('allow_script_access')] = array(
    '#type' => 'textfield',
    '#title' => t('Allow Script Access'),
    '#default_value' => media_kewego_variable_get('allow_script_access', 'always'),
  );

  return $form;
}
