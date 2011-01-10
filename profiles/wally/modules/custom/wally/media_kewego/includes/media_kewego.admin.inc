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
    '#default_value' => media_kewego_variable_get('allowed_url'),
    '#description' => t('Enter here the URL from which the videos may come. One URL by line.'),
    '#required' => TRUE,
  );
  $form['settings'][media_kewego_variable_name('autostart')] = array(
    '#type' => 'checkbox',
    '#title' => t('Autoplay'),
    '#default_value' => media_kewego_variable_get('autostart'),
    '#description' => t('Automatically start the video when loaded.'),
  );
  $form['settings'][media_kewego_variable_name('fullscreen')] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow full screen'),
    '#default_value' => media_kewego_variable_get('fullscreen'),
    '#description' => t('Allow the user to watch the video full screen.'),
  );
  $videoformat_options = array(
    'old' => t('Old (flv)'),
    'normal' => t('Normal (mp4)'),
    'high' => t('High (HD)'),
  );
  $form['settings'][media_kewego_variable_name('videoformat')] = array(
    '#type' => 'select',
    '#title' => t('Prefered Format'),
    '#default_value' => media_kewego_variable_get('videoformat'),
    '#description' => t('Choose the default format. If a video isn\'t available in the chosen format the lower format will be used.'),
    '#options' => $videoformat_options,
  );
  $language_options = array(
    'en' => t('English'),
    'fr' => t('French'),
  );
  $form['settings'][media_kewego_variable_name('language')] = array(
    '#type' => 'select',
    '#title' => t('Prefered language'),
    '#default_value' => media_kewego_variable_get('language'),
    '#description' => t('The language used in the player.'),
    '#options' => $language_options,
  );
  $form['settings'][media_kewego_variable_name('width')] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => media_kewego_variable_get('width'),
    '#description' => t('The Player width.'),
  );
  $form['settings'][media_kewego_variable_name('height')] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#default_value' => media_kewego_variable_get('height'),
    '#description' => t('The player height.'),
  );
  
  $form['api'] = array(
    '#type' => 'fieldset',
    '#title' => t('Kewego API'),
    '#description' => t('If you wish to allow video uploads via the Media Mover module, you will need to apply for an API Developer Key from the <a href="@kewego" target="_blank">Kewego Developer Profile page</a>. Note that you do not need this key to display Kewego videos or their thumbnails.', array('@kewego' => MEDIA_KEWEGO_API_APPLICATION_URL)),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['api'][media_kewego_variable_name('api_key')] = array(
    '#type' => 'textfield',
    '#title' => t('App key'),
    '#default_value' => media_kewego_variable_get('api_key'),
    '#description' => t('Please enter your YouTube Developer Key here.'),
  );
  $form['api'][media_kewego_variable_name('kewego_username')] = array(
    '#type' => 'textfield',
    '#title' => t('Kewego username'),
    '#default_value' => media_kewego_variable_get('kewego_username'),
    '#description' => t('If you plan to upload videos, you will need to enter the Kewego account username here.'),
  );
  $form['api'][media_kewego_variable_name('kewego_password')] = array(
    '#type' => 'password',
    '#title' => t('Kewego password'),
    '#default_value' => media_kewego_variable_get('kewego_password'),
    '#description' => t('If you plan to upload videos, you will need to enter the Kewego account password here.'),
  );
  
  $form['status'] = array(
    '#type' => 'fieldset',
    '#title' => t('Video status'),
    '#description' => t('Settings concerning the status of a video.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['status'][media_kewego_variable_name('status_display_'. EMFIELD_STATUS_UNAVAILABLE)] = array(
    '#type' => 'textarea',
    '#title' => t('Status unavailable message'),
    '#default_value' => media_kewego_variable_get('status_display_'. EMFIELD_STATUS_UNAVAILABLE),
    '#description' => t('The message to display if a video is unavailable. Note that if this is blank, then no attempt will be made to hide an unavailable video, regardless of the other settings. Note also that this message may contain HTML markup, and may use token replacements.'),
  );
  $form['status']['duplicates'] = array(
    '#type' => 'fieldset',
    '#title' => t('Duplicate status'),
    '#description' => t('Settings concerning duplicate videos.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['status']['duplicates'][media_kewego_variable_name('check_for_duplicates')] = array(
    '#type' => 'checkbox',
    '#title' => t('Check for duplicates'),
    '#description' => t('If checked, then an attempt will be made to discover if an unavailable video was rejected for being duplicate, and if so, display the following message.'),
    '#default_value' => media_kewego_variable_get('check_for_duplicates'),
  );
  $form['status']['duplicates'][media_kewego_variable_name('status_display_duplicates')] = array(
    '#type' => 'textarea',
    '#title' => t('Status duplicate message'),
    '#default_value' => media_kewego_variable_get('status_display_duplicates'),
    '#description' => t('The message to display if a video is unavailable because it is duplicate. Note that if this is blank, then the message will fall back to the unavailable message above. Note also that this message may contain HTML markup, and may use token replacements.'),
  );
  $form['status']['rejected'] = array(
    '#type' => 'fieldset',
    '#title' => t('Rejected status'),
    '#description' => t('Settings concerning duplicate videos.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['status']['rejected'][media_kewego_variable_name('check_for_rejected')] = array(
    '#type' => 'checkbox',
    '#title' => t('Check for rejected uploads'),
    '#description' => t('If checked, then an attempt will be made to discover if an unavailable video was rejected for any other reason, and if so, display the following message.'),
    '#default_value' => media_kewego_variable_get('check_for_rejected'),
  );
  $form['status']['rejected'][media_kewego_variable_name('status_display_rejected')] = array(
    '#type' => 'textarea',
    '#title' => t('Status rejected message'),
    '#default_value' => media_kewego_variable_get('status_display_rejected'),
    '#description' => t('The message to display if a video is unavailable because it is rejected. Note that if this is blank, then the message will fall back to the unavailable message above. Note also that this message may contain HTML markup, and may use token replacements.'),
  );
  $form['status']['failed'] = array(
    '#type' => 'fieldset',
    '#title' => t('Failed status'),
    '#description' => t('Settings concerning duplicate videos.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['status']['failed'][media_kewego_variable_name('check_for_failed')] = array(
    '#type' => 'checkbox',
    '#title' => t('Check for failed uploads'),
    '#description' => t('If checked, then an attempt will be made to discover if the upload of an unavailable video failed for any other reason, and if so, display the following message.'),
    '#default_value' => media_kewego_variable_get('check_for_failed'),
  );
  $form['status']['failed'][media_kewego_variable_name('status_display_upload_failed')] = array(
    '#type' => 'textarea',
    '#title' => t('Status upload failed message'),
    '#default_value' => media_kewego_variable_get('status_display_upload_failed'),
    '#description' => t('The message to display if a video is unavailable because its upload failed. Note that if this is blank, then the message will fall back to the unavailable message above. Note also that this message may contain HTML markup, and may use token replacements.'),
  );

  $form['status']['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced status settings'),
    '#description' => t('Cron status update functionality.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $options = array(
    MEDIA_KEWEGO_STATUS_UPDATE_NONE => t('Never update'),
    MEDIA_KEWEGO_STATUS_UPDATE_FROM_UNAVAILABLE => t('Update only from unavailable'),
    MEDIA_KEWEGO_STATUS_UPDATE_ALWAYS => t('Update always'),
  );
  $form['status']['advanced'][media_kewego_variable_name('status_update_frequency')] = array(
    '#type' => 'select',
    '#title' => t('Status update frequency'),
    '#options' => $options,
    '#default_value' => media_kewego_variable_get('status_update_frequency'),
    '#description' => t('Please select the frequency you wish to update the status of individual videos. If you select %from_unavailable, the status will only be rechecked if a video is unavailable, until it has finally been marked by Kewego as being available. If you select %always, the status will be checked always. Otherwise, the status will never be checked, once set initially. Note that the results are always cached, so the Kewego API is kept happy.', array('%from_unavailable' => t('Update only from unavailable'), '%always' => t('Update always'))),
  );
  $options = array(
    0 => 'Always',
    5 => '5 minutes',
    10 => '10 minutes',
    15 => '15 minutes',
    30 => '30 minutes',
    60 => '1 hour',
    120 => '2 hours',
    240 => '4 hours',
    480 => '8 hours',
    60 * 24 => '1 day',
    60 * 24 * 2 => '2 days',
    60 * 24 * 4 => '4 days',
    60 * 24 * 7 => '1 week',
  );
  $form['status']['advanced'][media_kewego_variable_name('cron_time')] = array(
    '#type' => 'select',
    '#title' => t('Cron time limit'),
    '#options' => $options,
    '#default_value' => media_kewego_variable_get('cron_time'),
    '#description' => t('Kewego videos will only be checked for a new status if at least this much time has passed since they were last checked. If the site has a lot of videos, you might consider raising this value so that the Kewego API gods stay happy.'),
  );
  $options = array(
    0 => 'All',
    1 => 1,
    2 => 2,
    5 => 5,
    10 => 10,
    25 => 25,
    50 => 50,
    100 => 100,
    200 => 200,
    250 => 250,
    500 => 500,
  );
  $form['status']['advanced'][media_kewego_variable_name('cron_limit')] = array(
    '#type' => 'select',
    '#title' => t('Cron video count limit'),
    '#options' => $options,
    '#default_value' => media_kewego_variable_get('cron_limit'),
    '#description' => t('Only up to this many videos will be checked for a new status during each cron run. If you are experiencing cron time outs, you might consider reducing this limit.'),
  );

  return $form;
}
