<?php
// $Id: media_dailymotion.admin.inc,v 1.0 2011/01/07 ODM $

/**
 * @file
 * Administrative functions for Media: dailymotion.
 */

/**
 * The administrative settings form for Media: dailymotion.
 */
function media_dailymotion_settings() {
  $form = media_dailymotion_admin_form();
  $form['settings_info'] = array(
    '#type' => 'item',
    '#value' => t('These settings specifically affect videos displayed from !dailymotion. You can also read more about its !api.', array('!dailymotion' => l(t('dailymotion.tv'), MEDIA_DAILYMOTION_MAIN_URL, array('attributes' => array('target' => '_blank'))), '!api' => l(t("developer's API"), MEDIA_DAILYMOTION_API_INFO, array('attributes' => array('target' => '_blank'))))),
    '#weight' => -10,
  );
  
  return system_settings_form($form);
}

/**
 * This form will be displayed both at /admin/settings/media_dailymotion and
 * admin/content/emfield.
 */
function media_dailymotion_admin_form() {
  $form = array();
  
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Player settings'),
    '#description' => t('General parameters used to display a dailymotion video.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['settings'][media_dailymotion_variable_name('allowed_url')] = array(
    '#type' => 'textarea',
    '#title' => t('Allowed URLs'),
    '#default_value' => media_dailymotion_variable_get('allowed_url'),
    '#description' => t('Enter here the URL from which the videos may come. One URL by line.'),
    '#required' => TRUE,
  );
  $form['settings'][media_dailymotion_variable_name('autostart')] = array(
    '#type' => 'checkbox',
    '#title' => t('Autoplay'),
    '#default_value' => media_dailymotion_variable_get('autostart'),
    '#description' => t('Automatically start the video when loaded.'),
  );
  $form['settings'][media_dailymotion_variable_name('fullscreen')] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow full screen'),
    '#default_value' => media_dailymotion_variable_get('fullscreen'),
    '#description' => t('Allow the user to watch the video full screen.'),
  );
  $videoformat_options = array(
    'old' => t('Old (flv)'),
    'normal' => t('Normal (mp4)'),
    'high' => t('High (HD)'),
  );
  $form['settings'][media_dailymotion_variable_name('videoformat')] = array(
    '#type' => 'select',
    '#title' => t('Prefered Format'),
    '#default_value' => media_dailymotion_variable_get('videoformat'),
    '#description' => t('Choose the default format. If a video isn\'t available in the chosen format the lower format will be used.'),
    '#options' => $videoformat_options,
  );
  $language_options = array(
    'en' => t('English'),
    'fr' => t('French'),
  );
  $form['settings'][media_dailymotion_variable_name('language')] = array(
    '#type' => 'select',
    '#title' => t('Prefered language'),
    '#default_value' => media_dailymotion_variable_get('language'),
    '#description' => t('The language used in the player.'),
    '#options' => $language_options,
  );
  $form['settings'][media_dailymotion_variable_name('width')] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => media_dailymotion_variable_get('width'),
    '#description' => t('The Player width.'),
  );
  $form['settings'][media_dailymotion_variable_name('height')] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#default_value' => media_dailymotion_variable_get('height'),
    '#description' => t('The player height.'),
  );

  return $form;
}
