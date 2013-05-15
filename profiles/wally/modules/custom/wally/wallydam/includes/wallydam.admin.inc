<?php
// $Id: yaxim.admin.inc,v 1.0 2010/08/23 14:39:00 rso Exp $

/**
 * @file
 *   Import Press Content to Drupal structure Administration
 */

/**
 * Packages synchronization settings form.
 */
function wallydam_page_admin_packages_sync_form($form_state) {
  $form = array();

  $form['wallydam_updateurl'] = array(
    '#type' => 'textfield',
    '#title' => t('URL to send the informations of the node'),
    '#description' => t('URL to send the information of the node.'),
    '#default_value' => wally_variable_get('wallydam_updateurl', 'http://esb2.rossel.be:8081/rest/pandamServices/updateUrl'),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );

  $form['wallydam_updatepackageurl'] = array(
    '#type' => 'textfield',
    '#title' => t('URL to send the updated packages'),
    '#description' => t('URL to send the updated package.'),
    '#default_value' => wally_variable_get('wallydam_updatepackageurl', ''),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );

  $form['wallydam_cronmaxnumber'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum number of packages sent in a post'),
    '#description' => t('Maximum number of packages sent in a post'),
    '#default_value' => wally_variable_get('wallydam_cronmaxnumber', 50),
    '#size' => 80,
    '#maxlenght' => 512,
    '#required' => TRUE,
  );

  return wally_system_settings_form($form, TRUE);
}

/**
 * Validate function for the settings form.
 */
function wallydam_page_admin_packages_sync_form_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['wallydam_cronmaxnumber'])){
    form_set_error('wallydam_cronmaxnumber', t('The maximum number of packages must be a numeric value'));
  }
}

function wallydam_page_admin_archive_form(){
  $form = array();
  $options = array(FALSE => 'OFF', TRUE => 'ON');
  $form['wallydam_archive_on'] = array(
    '#type' => 'select',
    '#title' => t('Active archive retrieving'),
    '#default_value' => wally_variable_get('wallydam_archive_on', FALSE),
    '#options' => $options,
    '#required' => TRUE,
  );

  $importer = wallymport_getimporters();
  $options = array('' => '');
  foreach ($importer as $imp){
    if (preg_match('/^packages/', $imp->importer_type)){
      $options[$imp->name] = $imp->name;
    }
  }

  $form['wallydam_archive_importer'] = array(
    '#type' => 'select',
    '#title' => t('Importer to use to extract package'),
    '#description' => t('You must have an importer of type packages to use the archive (V16,V18,...)'),
    '#default_value' => wally_variable_get('wallydam_archive_importer', ''),
    '#options' => $options,
    '#required' => TRUE,
  );

  $form['wallydam_archive_geturl'] = array(
    '#type' => 'textfield',
    '#title' => t('URL of the DAM to retrieve archive'),
    '#default_value' => wally_variable_get('wallydam_archive_geturl', ''),
    '#require' => TRUE,
  );

  return wally_system_settings_form($form, TRUE);
}

/**
 * DAM Search settings page, manage search profiles
 */
function wallydam_page_admin_search() {
  $profiles_header = array(t('Machine name'), t('Name'), t('Default profile'), array('data' => t('Operations'), 'colspan' => '2'));
  $profiles_rows = array();
  $profiles_caption = t('Profiles List');

  $db_profiles = db_query('SELECT * FROM {wallydam_search_profiles}');
  while ($profile = db_fetch_array($db_profiles)) {
    $operations = _wallydam_page_admin_search_get_operations($profile);
    $row = array(
      check_plain($profile['name']),
      check_plain($profile['label']),
      (isset($profile['default_profile']) && $profile['default_profile'] == 1) ? t('Yes') : t('No'),
      array('data' => $operations),
    );

    $profiles_rows[] = $row;
  }

  $output = !empty($profiles_rows) ? theme('table', $profiles_header, $profiles_rows, array(), $profiles_caption) : '';
  $output .= drupal_get_form('wallydam_page_admin_search_add_new_profile_form');
  $output .= drupal_get_form('wallydam_page_admin_search_general_settings_form');

  return $output;
}

/**
 * Add new profile
 */
function wallydam_page_admin_search_add_new_profile_form($form_state) {
  $form = array();

  $form['add'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add a new profile'),
    '#collapsible' => FALSE,
  );

  $form['add']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('New profile machine name'),
    '#description' => t('Must be a machine readable name (no spaces, no capital letters, no punctuation, "-" allowed).'),
    '#default_value' => '',
    '#required' => TRUE,
  );

  $form['add']['label'] = array(
    '#type' => 'textfield',
    '#title' => t('New profile label'),
    '#default_value' => '',
    '#required' => TRUE,
  );

  $form['add']['add-submit'] = array(
    '#type' => 'submit',
    '#title' => t('Add'),
    '#value' => t('Add'),
  );

  $form['add']['default_profile'] = array(
    '#type' => 'hidden',
    '#value' => '0',
  );

  return $form;
}

function wallydam_page_admin_search_add_new_profile_form_validate($form, $form_state) {
  $values = $form_state['values'];

  if (!preg_match('/^[a-z0-9\-]*$/', $values['name'])) {
    form_set_error('name', t('Please enter a machine readale name (no spaces, no capital letters, no punctuation, "-" allowed).'));
  }
}

function wallydam_page_admin_search_add_new_profile_form_submit($form, $form_state) {
  $values = $form_state['values'];
  $values['settings'] = array();

  $existing_profiles = wallydam_get_search_profiles();
  if (empty($existing_profiles)) {
    $values['default_profile'] = 1;
  }

  drupal_write_record('wallydam_search_profiles', $values);
  drupal_set_message(t('Profile "@profile" saved! Do not forget to configure it before using it!', array('@profile' => $values['label'])));
}

/**
 * General settings for DAM search
 */
function wallydam_page_admin_search_general_settings_form($form_state) {
  $form = array();

  $form['gen'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => FALSE,
  );

  $form['gen']['query'] = array(
    '#type' => 'textfield',
    '#title' => t('Query URL'),
    '#description' => t('URL that will actually be called to perform a search query'),
    '#default_value' => wally_variable_get('wallydam_search_query_URL', ''),
    '#required' => TRUE,
  );

  $form['gen']['thesaurus'] = array(
    '#type' => 'textfield',
    '#title' => t('URL of the search parameters\' thesaurus'),
    '#description' => t('Return list of defined NewsRooms, Products, Zones, Editions, ...'),
    '#default_value' => wally_variable_get('wallydam_search_thesaurus_URL', ''),
    '#required' => TRUE,
  );

  $form['gen']['gen-submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['gen']['clear-thesaurus'] = array(
    '#type' => 'submit',
    '#value' => t('Clear parameters\' thesaurus cache')
  );

  return $form;
}

function wallydam_page_admin_search_general_settings_form_validate($form, $form_state) {
  $values = $form_state['values'];
  $clicked_button = $form_state['clicked_button'];

  switch ($clicked_button['#value']) {
    case $form['gen']['gen-submit']['#value']:
      if (!valid_url($values['query'], TRUE)) {
        form_set_error('query', t('The given query URL is not a valid URL'));
      }

      if (!valid_url($values['thesaurus'], TRUE)) {
        form_set_error('thesaurus', t('The given search parameters\' thesaurus URL is not a valid URL'));
      }
      break;

    case $form['gen']['clear-thesaurus']['#value']:

      break;
  }
}

function wallydam_page_admin_search_general_settings_form_submit($form, $form_state) {
  $values = $form_state['values'];
  $clicked_button = $form_state['clicked_button'];

  switch ($clicked_button['#value']) {
    case $form['gen']['gen-submit']['#value']:
      wally_variable_set('wallydam_search_query_URL', $values['query']);
      wally_variable_set('wallydam_search_thesaurus_URL', $values['thesaurus']);

      drupal_set_message(t('Configuration saved'));
      break;

    case $form['gen']['clear-thesaurus']['#value']:
      cache_clear_all('wallydam_thesaurus', 'cache');

      drupal_set_message(t('Cache cleared'));
      break;
  }
}

/**
 * Edit a DAM search profile form
 */
function wallydam_page_admin_search_edit_profile_form($form_state, $pid) {
  $redirect_url = 'admin/wally/wallydam/search';
  $error = FALSE;

  $form = array(
    '#redirect' => $redirect_url,
  );

  $default_values = _wallydam_page_admin_search_get_default_values();
  if ($profile = wallydam_get_search_profile($pid)) {
    $default_values = array_merge($default_values, $profile['settings']);
    $default_values['label'] = $profile['label'];
  } else {
    $error = t('No matching search profile');
  }

  $thesaurus = _wallydam_page_admin_search_get_thesaurus($error);

  if ($error) {
    drupal_set_message($error, 'warning');
    drupal_goto($redirect_url);
  }

  ctools_include('dependent');
  ctools_add_js('dependent');

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value' => $default_values['label'],
    '#required' => TRUE,
  );

  $options = _wallydam_page_admin_search_populate_thesaurus_options($thesaurus);

  $filetype_options = (isset($options['filetype']) && is_array($options['filetype'])) ? $options['filetype'] : array();

  // @todo Handle subtypes
  $form['filetype'] = array(
    '#type' => 'select',
    '#title' => t('Type of object on which the research is performed'),
    '#description' => t('The research will only be performed on selected type'),
    '#options' => $filetype_options,
    '#default_value' => $default_values['filetype'],
    '#multiple' => TRUE,
  );

  $form['advanced'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use advanced search'),
    '#description' => t('The advanced search increases the search possibilities (search on several words, exclude words, ...)'),
    '#default_value' => $default_values['advanced'],
  );

  $form['withthumbs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include thumbnails'),
    '#description' => t('Include photos thumbnails in search results'),
    '#default_value' => $default_values['withthumbs'],
  );

  $form['thesaurus_options'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#tree' => TRUE,
  );

  $newsroom_options = (isset($options['newsroom']) && is_array($options['newsroom'])) ? $options['newsroom'] : array();
  $product_options = (isset($options['product']) && is_array($options['product'])) ? $options['product'] : array();
  $zone_options = (isset($options['zone']) && is_array($options['zone'])) ? $options['zone'] : array();
  $edition_options = (isset($options['edition']) && is_array($options['edition'])) ? $options['edition'] : array();

  // @todo Handle multiple choice

  $form['thesaurus_options']['newsroom'] = array(
    '#type' => 'select',
    '#title' => t('NewsRoom'),
    '#options' => $newsroom_options,
    '#default_value' => $default_values['newsroom'],
  );

  foreach ($product_options as $prod_key => $prod_options) {
    $form['thesaurus_options']['product'][$prod_key] = array(
      '#type' => 'select',
      '#title' => t('Product'),
      '#options' => $prod_options,
      '#default_value' => str_replace('.', '--', $default_values['product']),
      '#dependency' => array('edit-thesaurus-options-newsroom' => array($prod_key)),
      '#process' => array('ctools_dependent_process'),
    );
  }

  foreach ($zone_options as $zo_key => $zo_options) {
    list($newsroom_key, $product_key) = explode('---', $zo_key, 2);
    $name_newsroom_key = str_replace('_', '-', $newsroom_key);

    $form['thesaurus_options']['zone'][$zo_key] = array(
      '#type' => 'select',
      '#title' => t('Zone'),
      '#options' => $zo_options,
      '#default_value' => str_replace('.', '--', $default_values['zone']),
      '#dependency' => array(
        'edit-thesaurus-options-newsroom' => array($newsroom_key),
        'edit-thesaurus-options-product-'.$name_newsroom_key => array($product_key)),
      '#dependency_count' => 2,
      '#process' => array('ctools_dependent_process'),
    );
  }

  foreach ($edition_options as $edi_key => $edi_options) {
    list($newsroom_key, $product_key) = explode('---', $edi_key, 2);
    $name_newsroom_key = str_replace('_', '-', $newsroom_key);

    $form['thesaurus_options']['edition'][$edi_key] = array(
      '#type' => 'select',
      '#title' => t('Edition'),
      '#options' => $edi_options,
      '#default_value' => str_replace('.', '--', $default_values['edition']),
      '#dependency' => array(
        'edit-thesaurus-options-newsroom' => array($newsroom_key),
        'edit-thesaurus-options-product-'.$name_newsroom_key => array($product_key)),
      '#dependency_count' => 2,
      '#process' => array('ctools_dependent_process'),
    );
  }

  $form['cn'] = array(
    '#type' => 'select',
    '#title' => t('Connection NewsRoom'),
    '#description' => t('The NewsRoom of the user performing the search'),
    '#options' => array('none' => t('None')) + $newsroom_options,
    '#default_value' => $default_values['cn'],
  );

  $form['pid'] = array(
    '#type' => 'hidden',
    '#value' => $pid,
  );

  $form['name'] = array(
    '#type' => 'hidden',
    '#value' => $profile['name'],
  );

  $form['default_profile'] = array(
    '#type' => 'hidden',
    '#value' => $profile['default_profile'],
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['cancel'] = array(
    '#type' => 'markup',
    '#value' => l(t('Cancel'), $redirect_url),
  );

  return $form;
}

function wallydam_page_admin_search_edit_profile_form_validate($form, $form_state) {
  // @todo Validations
  // Simple search => 1! filetype
  // Simple search => only newsrooms
}

function wallydam_page_admin_search_edit_profile_form_submit($form, $form_state) {
  $values = $form_state['values'];

  $settings = array();
  $settings['filetype'] = $values['filetype'];
  $settings['advanced'] = $values['advanced'];
  $settings['withthumbs'] = $values['withthumbs'];
  $thesaurus_options = $values['thesaurus_options'];
  $settings['newsroom'] = $newsroom = isset($thesaurus_options['newsroom']) ? $thesaurus_options['newsroom'] : 'all';
  $product = isset($thesaurus_options['product'][$newsroom]) ? $thesaurus_options['product'][$newsroom] : 'all';
  $settings['product'] = str_replace('--', '.', $product);
  $full_key = $newsroom.'---'.$product;
  $zone = isset($thesaurus_options['zone'][$full_key]) ? $thesaurus_options['zone'][$full_key] : 'all';
  $settings['zone'] = str_replace('--', '.', $zone);
  $edition = isset($thesaurus_options['edition'][$full_key]) ? $thesaurus_options['edition'][$full_key] : 'all';
  $settings['edition'] = str_replace('--', '.', $edition);
  $settings['cn'] = $values['cn'];

  $profile = array(
    'pid' => $values['pid'],
    'name' => $values['name'],
    'label' => $values['label'],
    'default_profile' => $values['default_profile'],
    'settings' => $settings,
  );

  drupal_write_record('wallydam_search_profiles', $profile, array('pid'));
}

/**
 * Delete a DAM search profile confirmation form
 */
function wallydam_page_admin_search_delete_profile_form($form_state, $pid) {
  if ($profile = wallydam_get_search_profile($pid)) {
    $question = t('Are you sure you want to delete this search profile?');

    $add_form = array();
    $add_form['pid'] = array(
      '#type' => 'hidden',
      '#value' => $pid,
    );

    $form = confirm_form($add_form, $question, 'admin/wally/wallydam/search');
    $form['#redirect'] = 'admin/wally/wallydam/search';

    return $form;
  } else {
    drupal_set_message(t('No matching search profile'), 'warning');
    drupal_goto('admin/wally/wallydam/search');
  }
}

function wallydam_page_admin_search_delete_profile_form_submit($form, $form_state) {
  $values = $form_state['values'];
  $pid = $values['pid'];
  $profile = wallydam_get_search_profile($pid);

  db_query("DELETE FROM {wallydam_search_profiles} WHERE pid = %d", $pid);
  if ($profile['default_profile'] == 1) {
    if ($min_pid = db_result(db_query("SELECT MIN(pid) FROM {wallydam_search_profiles}"))) {
      db_query("UPDATE {wallydam_search_profiles} SET default_profile = 1 WHERE pid = %d", $min_pid);
    }
  }
}

/**
 * Set a DAM search profile as default profile
 */
function wallydam_page_admin_search_set_default_profile($pid) {
  db_query("UPDATE {wallydam_search_profiles} SET default_profile = 0");
  db_query("UPDATE {wallydam_search_profiles} SET default_profile = 1 WHERE pid = %d", $pid);
  drupal_goto('admin/wally/wallydam/search');
}

/**
 * Get links to available operations
 */
function _wallydam_page_admin_search_get_operations($profile) {
  $return = l(t('Edit'), 'admin/wally/wallydam/search/edit/'.$profile['pid']).' - ';
  $return .= l(t('Delete'), 'admin/wally/wallydam/search/delete/'.$profile['pid']).' - ';
  $return .= l(t('Set as default'), 'admin/wally/wallydam/search/setdefault/'.$profile['pid']);
  return $return;
}

/**
 * Get default values for the profile edit form
 */
function _wallydam_page_admin_search_get_default_values() {
  return array(
    'label' => '',
    'filetype' => 'Text',
    'advanced' => 0,
    'newsroom' => '',
  );
}

/**
 * Get thesaurus from URL or cache
 */
function _wallydam_page_admin_search_get_thesaurus(&$error) {
  $thesaurus = array();

  $thesaurus_URL = wally_variable_get('wallydam_search_thesaurus_URL', '');
  if (empty($thesaurus_URL) || !valid_url($thesaurus_URL)) {
    $error = t('The thesaurus URL has not been set or is invalid');
  }

  $cached_thesaurus = cache_get('wallydam_thesaurus', 'cache');
  if (!is_object($cached_thesaurus) || empty($cached_thesaurus) || !isset($cached_thesaurus->data) || empty($cached_thesaurus->data)) {
    $http_thesaurus = drupal_http_request($thesaurus_URL);
    module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
    if (wallytoolbox_is_http_request_successfull($http_thesaurus)) {
      $thesaurus = json_decode($http_thesaurus->data);
      if (!$thesaurus) {
        $error = t('Unable to read search parameters\' thesaurus datas');
      } else {
        cache_set('wallydam_thesaurus', $thesaurus);
      }
    } else {
      $error = t('Couldn\'t reach search parameters\' thesaurus URL');
    }
  } else {
    $thesaurus = $cached_thesaurus->data;
  }

  return $thesaurus;
}

/**
 * Populate selectboxes options according to the thesaurus
 */
function _wallydam_page_admin_search_populate_thesaurus_options($thesaurus) {
  $options = array(
    'filetype' => array(),
    'newsroom' => array(),
    'product' => array(),
    'zone' => array(),
    'edition' => array(),
  );

  if (isset($thesaurus->thInfo->Type->values) && is_array($thesaurus->thInfo->Type->values)) {
    foreach ($thesaurus->thInfo->Type->values as $filetype_value) {
      $options['filetype'][$filetype_value->code] = $filetype_value->displayName;
    }
  }

  if (isset($thesaurus->thInfo->Newsroom->values) && is_array($thesaurus->thInfo->Newsroom->values)) {
    foreach ($thesaurus->thInfo->Newsroom->values as $newsroom_value) {
      $options['newsroom'][$newsroom_value->code] = $newsroom_value->displayName;
    }
  }

  if (isset($thesaurus->thInfo->Zone->values) && is_array($thesaurus->thInfo->Zone->values)) {
    foreach ($thesaurus->thInfo->Zone->values as $zone_value) {
      if (isset($zone_value->links->Newsroom) && is_array($zone_value->links->Newsroom) && isset($zone_value->links->Product) && is_array($zone_value->links->Product)) {
        foreach ($zone_value->links->Newsroom as $linked_newsroom) {
          foreach ($zone_value->links->Product as $linked_product) {
            $linked_product_code = str_replace('.', '--', $linked_product->code);
            $options['zone'][$linked_newsroom->code.'---'.$linked_product_code][$zone_value->code] = $zone_value->displayName;
          }
        }
      }
    }
  }

  if (isset($thesaurus->thInfo->Edition->values) && is_array($thesaurus->thInfo->Edition->values)) {
    foreach ($thesaurus->thInfo->Edition->values as $edition_value) {
      if (isset($edition_value->links->Newsroom) && is_array($edition_value->links->Newsroom) && isset($edition_value->links->Product) && is_array($edition_value->links->Product)) {
        foreach ($edition_value->links->Newsroom as $linked_newsroom) {
          foreach ($edition_value->links->Product as $linked_product) {
            $linked_product_code = str_replace('.', '--', $linked_product->code);
            $options['edition'][$linked_newsroom->code.'---'.$linked_product_code][$edition_value->code] = $edition_value->displayName;
          }
        }
      }
    }
  }

  if (isset($thesaurus->thInfo->Product->values) && is_array($thesaurus->thInfo->Product->values)) {
    foreach ($thesaurus->thInfo->Product->values as $product_value) {
      if (isset($product_value->links->Newsroom) && is_array($product_value->links->Newsroom)) {
        foreach ($product_value->links->Newsroom as $linked_newsroom) {
          $product_code = str_replace('.', '--', $product_value->code);
          $options['product'][$linked_newsroom->code][$product_code] = $product_value->displayName;

          $full_key = $linked_newsroom->code.'---'.$product_code;
          if (!isset($options['zone'][$full_key])) {
            $options['zone'][$full_key] = array();
          }
          $insert_array = array('all' => t('All'));
          $options['zone'][$full_key] = $insert_array + $options['zone'][$full_key];
          if (!isset($options['edition'][$full_key])) {
            $options['edition'][$full_key] = array();
          }
          $options['edition'][$full_key] = $insert_array + $options['edition'][$full_key];
        }
      }
    }
  }

  return $options;
}
