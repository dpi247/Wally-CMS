<?php

function wallynewssitemap_admin_settings_page_form($form_state) {
  $form = array();

  $default_settings = array_merge(wallynewssitemap_default_settings(), wally_variable_get('wallynewssitemap_sitemaps_settings', array()));

  $form['wallynewssitemap_path'] = array(
    '#type' => 'textfield',
    '#title' => t('News sitemap\'s XMLs folder path'),
    '#description' => t('News sitemap\'s XMLs will be saved there.<br>
        This must be a writable directory.'),
    '#default_value' => $default_settings['wallynewssitemap_path'],
    '#required' => TRUE,
  );
  
  $form['wallynewssitemap_delta_hours'] = array(
    '#type' => 'textfield',
    '#title' => t('Nodes\' maximum age in hours'),
    '#default_value' => $default_settings['wallynewssitemap_delta_hours'],
    '#required' => TRUE,
  );
  
  $blacklist_defaults = implode(PHP_EOL, $default_settings['wallynewssitemap_blacklist']);
  $form['wallynewssitemap_blacklist'] = array(
    '#type' => 'textarea',
    '#title' => t('URLs black list'),
    '#description' => t('These URLs will not be included in the news sitemap.<br>
        Insert one URL per line, without the site\'s domain.<br>
        Example : <i>contact</i> or <i>article/this-article-must-not-be-sitemapped</i>'),
    '#default_value' => $blacklist_defaults,
  );
  
  $add_medias_options = array(
    'wallynewssitemap_add_images' => t('Add images informations'),
    'wallynewssitemap_add_videos' => t('Add videos informations'),
  );
  $add_medias_defaults = array();
  if ($default_settings['wallynewssitemap_add_images']) $add_medias_defaults[] = 'wallynewssitemap_add_images';
  if ($default_settings['wallynewssitemap_add_videos']) $add_medias_defaults[] = 'wallynewssitemap_add_videos';
  $form['wallysnewsitemap_add_medias'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Medias handling'),
    '#options' => $add_medias_options,
    '#default_value' => $add_medias_defaults,
  );
  
  $form['wallynewssitemap_publication_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Publication name'),
    '#default_value' => $default_settings['wallynewssitemap_publication_name'],
  );
  
  $content_types_options = node_get_types('names');
  $form['wallynewssitemap_content_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Content types to add to news sitemap'),
    '#options' => $content_types_options,
    '#default_value' => $default_settings['wallynewssitemap_content_types'],
    '#required' => TRUE,
  );
  
  $tags_fields_options = _wallynewssitemap_content_field_get_taxonomy_fields();
  $form['wallynewssitemap_tags_fields'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Fields used as tags'),
    '#description' => t('Some fields may not be used if they are not used in the selected content types.'),
    '#options' => $tags_fields_options,
    '#default_value' => $default_settings['wallynewssitemap_tags_fields'],
  );
  
  $freq_options = array(
    'always' => t('Always'),
    'hourly' => t('Hourly'),
    'daily' => t('Daily'),
    'weekly' => t('Weekly'),
    'monthly' => t('Monthly'),
    'yearly' => t('Yearly'),
    'never' => t('Never'),
  );
  $form['wallynewssitemap_nodes_freqs'] = array(
    '#type' => 'wallysitemap_taxonomy_tree',
    '#title' => t('Nodes refresh frequency per destination'),
    '#description' => t('Set refresh frequency parameter for nodes per destination'),
    '#freq_options' => $freq_options,
    '#default_value' => $default_settings['wallynewssitemap_nodes_freqs'],
    '#vid' => variable_get('wallymport_destinationpath', 2),
    '#parent' => 0,
    '#expand_all' => TRUE,
    '#multiple' => TRUE,
    '#add_checkbox' => FALSE,
  );
  
  $presets = imagecache_presets();
  $presets_options = array();
  foreach($presets as $preset) {
    $presets_options[$preset['presetname']] = $preset['presetname'];
  }
  
  $form['wallynewssitemap_image_preset'] = array(
    '#type' => 'select',
    '#title' => t('Preset used for images'),
    '#options' => $presets_options,
    '#default_value' => $default_settings['wallynewssitemap_image_preset'],
  );
  
  $form['wallynewssitemap_image_license'] = array(
    '#type' => 'textfield',
    '#title' => t('Images license'),
    '#description' => t('This must be a valid URL'),
    '#default_value' => $default_settings['wallynewssitemap_image_license'],
  );
  
  $form['wallynewssitemap_video_preset'] = array(
    '#type' => 'select',
    '#title' => t('Preset used for videos thumbnails'),
    '#options' => $presets_options,
    '#default_value' => $default_settings['wallynewssitemap_video_preset'],
  );
  
  $form['wallynewssitemap_default_img'] = array(
    '#type' => 'textfield',
    '#title' => t('Default image path'),
    '#description' => t('This must be a valid local path'),
    '#default_value' => $default_settings['wallynewssitemap_default_img'],
  );
  
  $form['wallynewssitemap_allow_embed'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow embeds'),
    '#description' => t('Is Google allowed to embed videos or not'),
    '#default_value' => $default_settings['wallynewssitemap_allow_embed'] == 'yes' ? 1 : 0,
  );
  
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  
  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#validate' => array(),
    '#submit' => array('wallynewssitemap_admin_settings_page_form_submit_reset'),
  );
  
  return $form;
}

function wallynewssitemap_admin_settings_page_form_validate($form, &$form_state) {
  $values = &$form_state['values'];

  if (!file_check_directory($values['wallynewssitemap_path'], 0)) {
    form_set_error('wallynewssitemap_path', t('The provided path is not an existing writable directory.'));
  }
  
  if (!is_numeric($values['wallynewssitemap_delta_hours']) || $values['wallynewssitemap_delta_hours'] <= 0) {
    form_set_error('wallynewssitemap_delta_hours', t('The nodes\' maximum age must a numeric value greater than 0.'));
  }
  
  if (!file_exists($values['wallynewssitemap_default_img'])) {
    form_set_error('wallynewssitemap_default_img', t('The provided image does not exists.'));
  }
}

function wallynewssitemap_admin_settings_page_form_submit($form, $form_state) {
  $values = $form_state['values'];

  $orig_settings = array_merge(wallynewssitemap_default_settings(), wally_variable_get('wallynewssitemap_sitemaps_settings', array()));

  $settings = $orig_settings;
  $settings['wallynewssitemap_path'] = $values['wallynewssitemap_path'];
  $settings['wallynewssitemap_delta_hours'] = $values['wallynewssitemap_delta_hours'];
  $tmp_blacklist = explode(PHP_EOL, $values['wallynewssitemap_blacklist']);
  $settings['wallynewssitemap_blacklist'] = array();
  foreach ($tmp_blacklist as $tmp_blackelem) {
    $blackelem = trim(trim($tmp_blackelem), '/');
    if (!empty($blackelem)) $settings['wallynewssitemap_blacklist'][] = $blackelem;
  }
  $settings['wallynewssitemap_add_images'] = $values['wallysnewsitemap_add_medias']['wallynewssitemap_add_images'] ? 1 : 0;
  $settings['wallynewssitemap_add_videos'] = $values['wallysnewsitemap_add_medias']['wallynewssitemap_add_videos'] ? 1 : 0;
  $settings['wallynewssitemap_publication_name'] = $values['wallynewssitemap_publication_name'];
  $settings['wallynewssitemap_content_types'] = array();
  foreach ($values['wallynewssitemap_content_types'] as $content_type) {
    if ($content_type) {
      $settings['wallynewssitemap_content_types'][] = $content_type;
    }
  }
  $settings['wallynewssitemap_tags_fields'] = array();
  foreach ($values['wallynewssitemap_tags_fields'] as $tag_field) {
    if ($tag_field) {
      $settings['wallynewssitemap_tags_fields'][] = $tag_field;
    }
  }
  $settings['wallynewssitemap_nodes_freqs'] = $values['wallynewssitemap_nodes_freqs'];
  $settings['wallynewssitemap_image_preset'] = $values['wallynewssitemap_image_preset'];
  $settings['wallynewssitemap_image_license'] = $values['wallynewssitemap_image_license'];
  $settings['wallynewssitemap_video_preset'] = $values['wallynewssitemap_video_preset'];
  $settings['wallynewssitemap_default_img'] = $values['wallynewssitemap_default_img'];
  $settings['wallynewssitemap_allow_embed'] = $values['wallynewssitemap_allow_embed'] ? 'yes' : 'no';

  wally_variable_set('wallynewssitemap_sitemaps_settings', $settings);
  
  drupal_set_message(t('Settings saved.'));
}

function wallynewssitemap_admin_settings_page_form_submit_reset($form, $form_state) {
  $orig_settings = array_merge(wallynewssitemap_default_settings(), wally_variable_get('wallynewssitemap_sitemaps_settings', array()));
  $settings = array();
  $settings['wallynewssitemap_consolidate_sitemap'] = $orig_settings['wallynewssitemap_consolidate_sitemap'];
  $settings['wallynewssitemap_consolidate_sitemap_type'] = $orig_settings['wallynewssitemap_consolidate_sitemap_type'];
  $settings['wallynewssitemap_consolidate_sitemap_amount'] = $orig_settings['wallynewssitemap_consolidate_sitemap_amount'];
  wally_variable_set('wallynewssitemap_sitemaps_settings', $settings);
  drupal_set_message(t('Settings restored'));
}

/**
 * Get all taxonomy fields.
 *
 * @return
 *   The field arrays.
 */
function _wallynewssitemap_content_field_get_taxonomy_fields() {
  $where = ' WHERE nfi.widget_module LIKE "content_taxonomy%%"';

  $db_result = db_query("SELECT DISTINCT(nfi.field_name), nfi.label FROM {". content_instance_tablename() ."} nfi ".
      "$where ORDER BY nfi.label ASC");

  $fields = array();
  while ($instance = db_fetch_array($db_result)) {
    $fields[$instance['field_name']] = $instance['label'];
  }
  return $fields;
}
