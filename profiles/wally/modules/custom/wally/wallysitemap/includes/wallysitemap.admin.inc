<?php

function wallysitemap_admin_settings_page_form($form_state) {
  $form = array();
  
  $default_settings = array_merge(wallysitemap_default_settings(), wally_variable_get('wallysitemap_sitemaps_settings', array()));
  
  $form['wallysitemap_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Sitemap\'s XMLs folder path'),
    '#description' => t('Sitemap\'s and sitemap index\'s XMLs will be saved there.<br>
        This must be a writable directory.'),
    '#default_value' => $default_settings['wallysitemap_path'],
    '#required' => TRUE,
  );
  
  $form['wallysitemap_base_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Base URL'),
    '#description' => t('URL used as base to build the URLs listed in sitemaps'),
    '#default_value' => $default_settings['wallysitemap_base_url'],
    '#required' => TRUE,
  );

  $blacklist_defaults = implode(PHP_EOL, $default_settings['wallysitemap_blacklist']);
  $form['wallysitemap_blacklist'] = array(
    '#type' => 'textarea',
    '#title' => t('URLs black list'),
    '#description' => t('These URLs will not be included in the main sitemap.<br>
        Insert one URL per line, without the site\'s domain.<br>
        Example : <i>contact</i> or <i>article/this-article-must-not-be-sitemapped</i>'),
    '#default_value' => $blacklist_defaults,
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
  $weights_options = array(
    '0' => '0',
    '0.1' => '0.1',
    '0.2' => '0.2',
    '0.3' => '0.3',
    '0.4' => '0.4',
    '0.5' => '0.5',
    '0.6' => '0.6',
    '0.7' => '0.7',
    '0.8' => '0.8',
    '0.9' => '0.9',
    '1' => '1',
  );
  
  $menus = menu_get_menus();
  $menus_default_values = $default_settings['wallysitemap_handled_menus'];
  $menu_default_values = array(
    'check' => 0,
    'weight_select' => 0,
    'freq' => 'daily',
  );
  foreach ($menus as $menu_name => $menu_value) {
    if (!isset($menus_default_values[$menu_name])) {
      $menus_default_values[$menu_name] = array();
    }
    $menus_default_values[$menu_name] = array_merge($menu_default_values, $menus_default_values[$menu_name]);
  }
  $form['wallysitemap_handled_menus'] = array(
    '#type' => 'wallysitemap_checkboxes',
    '#title' => t('Menus to use for sitemap construction'),
    '#description' => t('Only menus entries included in the selected menus will be added to the sitemap.'),
    '#check_options' => $menus,
    '#select_options' => $weights_options,
    '#freq_options' => $freq_options,
    '#default_value' => $menus_default_values,
  );

  $form['wallysitemap_dests_params'] = array(
    '#type' => 'wallysitemap_taxonomy_tree',
    //'#type' => 'taxonomy_manager_tree',
    '#title' => t('Destinations parameters (active, weight and refresh frequency)'),
    '#description' => t('Set URLs parameters per destination :<br>
        The checkbox specifies wheter the destination must be sitemapped or not,<br>
        The bigger the weight the more important the destination is,<br>
        The refresh frequency.'),
    '#select_options' => $weights_options,
    '#freq_options' => $freq_options,
    '#default_value' => $default_settings['wallysitemap_dests_params'],
    '#vid' => variable_get('wallymport_destinationpath', 2),
    '#parent' => 0,
    '#expand_all' => TRUE,
    '#multiple' => TRUE,
  );
  
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  
  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#validate' => array(),
    '#submit' => array('wallysitemap_admin_settings_page_form_submit_reset'),
  );
  
  return $form;
}

function wallysitemap_admin_settings_page_form_validate($form, $form_state) {
  $values = $form_state['values'];

  if (!file_check_directory($values['wallysitemap_path'], 0)) {
    form_set_error('wallysitemap_path', t('The provided path is not an existing writable directory.'));
  }
}

function wallysitemap_admin_settings_page_form_submit($form, $form_state) {
  $values = $form_state['values'];
  $settings = array();
  $settings['wallysitemap_path'] = $values['wallysitemap_path'];
  $settings['wallysitemap_base_url'] = $values['wallysitemap_base_url'];
  $tmp_blacklist = explode(PHP_EOL, $values['wallysitemap_blacklist']);
  $settings['wallysitemap_blacklist'] = array();
  foreach ($tmp_blacklist as $tmp_blackelem) {
    $blackelem = trim(trim($tmp_blackelem), '/');
    if (!empty($blackelem)) $settings['wallysitemap_blacklist'][] = $blackelem;
  }
  $settings['wallysitemap_default_freq'] = $values['wallysitemap_default_freq'];
  $settings['wallysitemap_handled_menus'] = array();
  foreach ($values['wallysitemap_handled_menus'] as $menu) {
    if ($menu) {
      $settings['wallysitemap_handled_menus'][] = $menu;
    }
  }
  $settings['wallysitemap_dests_params'] = $values['wallysitemap_dests_params'];

  wally_variable_set('wallysitemap_sitemaps_settings', $settings);
}

function wallysitemap_admin_settings_page_form_submit_reset($form, $form_state) {
  wally_variable_del('wallysitemap_sitemaps_settings');
}
