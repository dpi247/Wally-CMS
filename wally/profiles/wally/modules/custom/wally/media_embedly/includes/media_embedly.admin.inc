<?php
// $Id: media_embedly.admin.inc,v 1.0 2011/01/07 ODM $

/**
 * @file
 * Administrative functions for Media: Kewego.
 */

/**
 * The administrative settings form for Media: Embedly.
 */
function media_embedly_settings() {
  $form = media_embedly_admin_form();
  $form['settings_info'] = array(
    '#type' => 'item',
    '#value' => t('These settings specifically affect embed displayed from !Embedly. You can also read more about its !api.', array('!Embedly' => l(t('embed.ly'), MEDIA_EMBEDLY_MAIN_URL, array('attributes' => array('target' => '_blank'))), '!api' => l(t("developer's API"), MEDIA_EMBEDLY_API_INFO, array('attributes' => array('target' => '_blank'))))),
    '#weight' => -10,
  );  
  return system_settings_form($form);
}

/**
 * This form will be displayed both at /admin/settings/media_embedly and
 * admin/content/emfield.
 */
function media_embedly_admin_form() {
  $form = array();
  
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Player settings'),
    '#description' => t('Any general parameters aren\'t used to display the Embedly embeds. Please go to the Embedded Media Field configuration to configure each media field types.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['settings'][media_embedly_variable_name('cache_time')] = array(
    '#type' => 'textfield',
    '#title' => t('Cache time'),
    '#default_value' => media_embedly_variable_get('cache_time'),
    '#description' => t('The time for which the cached info is considered valid (in minutes).'),
  );
  return $form;
};


/**
 * This forms will be displayed at admin/content/emfield/mediaType.
 */

function media_embedly_video_admin_form() {
  $form = array();
  
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Player settings'),
    '#description' => t('General parameters used to display a Embedly video embed.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['settings'][media_embedly_variable_name('allowed_video_url')] = array(
    '#type' => 'textarea',
    '#title' => t('Allowed URLs'),
    '#default_value' => media_embedly_variable_get('allowed_video_url'),
    '#description' => t('Enter here the URL from which the videos may come. One URL by line.'),
    '#required' => TRUE,
  );
  $form['settings'][media_embedly_variable_name('video_width')] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => media_embedly_variable_get('video_width'),
    '#description' => t('The Player width.'),
  );
  //$form['settings'][media_embedly_variable_name('height')] = array(
    //'#type' => 'textfield',
    //'#title' => t('Height'),
    //'#default_value' => media_embedly_variable_get('height'),
    //'#description' => t('The player height.'),
  //);

  return $form;
}


function media_embedly_audio_admin_form() {
  $form = array();
  
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Player settings'),
    '#description' => t('General parameters used to display a Embedly audio embed.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['settings'][media_embedly_variable_name('allowed_audio_url')] = array(
    '#type' => 'textarea',
    '#title' => t('Allowed URLs'),
    '#default_value' => media_embedly_variable_get('allowed_audio_url'),
    '#description' => t('Enter here the URL from which the audios may come. One URL by line.'),
    '#required' => TRUE,
  );
  $form['settings'][media_embedly_variable_name('audio_width')] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => media_embedly_variable_get('audio_width'),
    '#description' => t('The Player width.'),
  );
  //$form['settings'][media_embedly_variable_name('height')] = array(
    //'#type' => 'textfield',
    //'#title' => t('Height'),
    //'#default_value' => media_embedly_variable_get('height'),
    //'#description' => t('The player height.'),
  //);

  return $form;
}