<?php
// $Id: media_coveritlive.admin.inc $

/**
 * @file media_coveritlive/includes/media_coveritlive.admin.inc
 * Administrative functions for Media: CoverItLive
 */

/**
 * hook_PROVIDER_settings()
 * The administrative settings form for Media: CoverItLive.
 * This form will be displayed both at /admin/settings/media_coveritlive 
 */

function media_scribble_settings() {
  $form = media_scribble_admin_form();
  $form['settings_info'] = array(
    '#type' => 'item',
    '#value' => t('These settings specifically affect medias displayed from !scribble.', array('!scribble' => l(t('scribble.com'), MEDIA_SCRIBBLE_MAIN_URL, array('attributes' => array('target' => '_blank'))))),
    '#weight' => -10,
  );  
  return system_settings_form($form);
}

/**
 * this function return a subform to be added to the emother_settings() admin settings page 
 * as well as the media_scribble_settings().
 * This form will be displayed both at /admin/settings/media_scribble and
 * admin/content/emfield/emother.
 * note that a form field will already be provided, at $form['PROVIDER'] (such as $form['scribble'])
 * so if you want specific provider settings within that field, you can add the elements to that form field.
 */
function media_scribble_admin_form() {
  $form = array();
  $form['full'] = array(
    '#type' => 'fieldset',
    '#title' => t('Full display settings'),
    '#description' => t('Parameters used to display a scribble media in full size.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['full'][media_scribble_variable_name('theme_number')] = array(
    '#type' => 'textfield',
    '#title' => t('Theme number'),
    '#default_value' => media_scribble_variable_get('theme_number'),
    '#description' => t('The theme number'),
  );
  $form['full'][media_scribble_variable_name('full_width')] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => media_scribble_variable_get('full_width'),
    '#description' => t('The display width.'),
  );
  $form['full'][media_scribble_variable_name('full_height')] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#default_value' => media_scribble_variable_get('full_height'),
    '#description' => t('The display height.'),
  );
  $form['preview'] = array(
    '#type' => 'fieldset',
    '#title' => t('Preview display settings'),
    '#description' => t('Parameters used to display a preview for a scribble media.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['preview'][media_scribble_variable_name('preview_width')] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => media_scribble_variable_get('preview_width'),
    '#description' => t('The preview width.'),
  );
  $form['preview'][media_scribble_variable_name('preview_height')] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#default_value' => media_scribble_variable_get('preview_height'),
    '#description' => t('The preview height.'),
  );
  $form['#validate'][] = 'media_scribble_admin_form_validate';
  return $form;
}

/*
 * This is the validate function to validate the content that are entered in the
 * admin form. Note that it only works in  /admin/settings/media_scribble.
 */

function media_scribble_admin_form_validate($form, &$form_state) {
  if(!is_numeric($form_state['values'][media_scribble_variable_name('full_width')]) || intval($form_state['values'][media_scribble_variable_name('full_width')]) != $form_state['values'][media_scribble_variable_name('full_width')] || $form_state['values'][media_scribble_variable_name('full_width')] < 300) {
    form_set_error(media_scribble_variable_name('full_width'), t('"Full width" must be an integer superior to 300.'));
  }
  
  if(!is_numeric($form_state['values'][media_scribble_variable_name('full_height')]) || intval($form_state['values'][media_scribble_variable_name('full_height')]) != $form_state['values'][media_scribble_variable_name('full_height')] ||$form_state['values'][media_scribble_variable_name('full_height')] < 385) {
    form_set_error(media_scribble_variable_name('full_height'), t('"Full height" must be an integer superior to 385.'));
  }
  if(!is_numeric($form_state['values'][media_scribble_variable_name('theme_number')]) ) {
    form_set_error(media_scribble_variable_name('full_width'), t('"Theme number" must be an integer'));
  }
  if(!is_numeric($form_state['values'][media_scribble_variable_name('preview_width')]) || intval($form_state['values'][media_scribble_variable_name('preview_width')]) != $form_state['values'][media_scribble_variable_name('preview_width')] ||$form_state['values'][media_scribble_variable_name('preview_width')] < 300) {
    form_set_error(media_scribble_variable_name('preview_width'), t('"Preview width" must be an integer superior to 300.'));
  }
  if(!is_numeric($form_state['values'][media_scribble_variable_name('preview_height')]) || intval($form_state['values'][media_scribble_variable_name('preview_height')]) != $form_state['values'][media_scribble_variable_name('preview_height')] ||$form_state['values'][media_scribble_variable_name('preview_height')] < 385) {
    form_set_error(media_scribble_variable_name('preview_height'), t('"Preview height" must be an integer superior to 385.'));
  }
}
