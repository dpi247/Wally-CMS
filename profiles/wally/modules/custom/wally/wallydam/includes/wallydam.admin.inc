<?php
// $Id: yaxim.admin.inc,v 1.0 2010/08/23 14:39:00 rso Exp $

/**
 * @file
 *   Import Press Content to Drupal structure Administration
 */
function wallydam_page_admin_packages_sync(){
  $content = '';
  $content .= drupal_get_form('wallydam_page_admin_packages_sync_form');
  $content .= wallydam_list_source();
  return $content;
}
/**
 * Packages synchronization settings form.
 */
function wallydam_page_admin_packages_sync_form($form_state) {
  $form = array();

  $form['wallydam_updateurl'] = array(
    '#type' => 'textfield',
    '#title' => t('URL to send the updated alias'),
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
    if (preg_match('/^packages/', $imp->importer_type) || preg_match('/^dampackages/', $imp->importer_type)){
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

  $form['gen']['thumbnails'] = array(
    '#type' => 'textfield',
    '#title' => t('Thumbnails URL'),
    '#description' => t('URL to get thumbnails from packages'),
    '#default_value' => wally_variable_get('wallydam_search_thumbnails_URL', ''),
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

      if (!valid_url($values['thumbnails'], TRUE)) {
        form_set_error('thumbnails', t('The given thumbnails URL is not a valid URL'));
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
      wally_variable_set('wallydam_search_thumbnails_URL', $values['thumbnails']);
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

  list($options, $js_options) = _wallydam_page_admin_search_populate_thesaurus_options($thesaurus);

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

  $js_product_options = (isset($js_options['product']) && is_array($js_options['product'])) ? $js_options['product'] : array();
  $js_zone_options = (isset($js_options['zone']) && is_array($js_options['zone'])) ? $js_options['zone'] : array();
  drupal_add_js(array('damSearchSelectOptions' => array('products' => $js_product_options, 'zones' => $js_zone_options)), 'setting');
  drupal_add_js(drupal_get_path('module', 'wallydam').'/scripts/dam_search_dependent.js');

  $form['thesaurus_options']['newsroom'] = array(
    '#type' => 'select',
    '#title' => t('NewsRoom'),
    '#options' => $newsroom_options,
    '#default_value' => $default_values['newsroom'],
    '#multiple' => TRUE,
  );

  $form['thesaurus_options']['product'] = array(
    '#type' => 'select',
    '#title' => t('Product'),
    '#options' => $product_options,
    '#default_value' => $default_values['product'],
    '#multiple' => TRUE,
  );

  $form['thesaurus_options']['zone'] = array(
    '#type' => 'select',
    '#title' => t('Zone'),
    '#options' => $zone_options,
    '#default_value' => $default_values['zone'],
    '#multiple' => TRUE,
  );

  $form['thesaurus_options']['edition'] = array(
    '#type' => 'select',
    '#title' => t('Edition'),
    '#options' => $edition_options,
    '#default_value' => $default_values['edition'],
    '#multiple' => TRUE,
  );

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
  $settings['newsroom'] = isset($thesaurus_options['newsroom']) ? $thesaurus_options['newsroom'] : 'all';
  $settings['product'] = isset($thesaurus_options['product']) ? $thesaurus_options['product'] : 'all';
  $settings['zone'] = isset($thesaurus_options['zone']) ? $thesaurus_options['zone'] : 'all';
  $settings['edition'] = isset($thesaurus_options['edition']) ? $thesaurus_options['edition'] : 'all';
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
    'newsroom' => 'all',
    'product' => 'all',
    'zone' => 'all',
    'edition' => 'all',
    'cn' => ''
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
  $all_options = array(
    'filetype' => array(),
    'newsroom' => array('all' => t('All')),
    'product' => array(),
    'zone' => array(),
    'edition' => array('all' => t('All')),
  );

  $js_options = array(
    'product' => array(),
    'zone' => array(),
  );

  if (isset($thesaurus->thInfo->Type->values) && is_array($thesaurus->thInfo->Type->values)) {
    foreach ($thesaurus->thInfo->Type->values as $filetype_value) {
      $all_options['filetype'][$filetype_value->code] = $filetype_value->displayName;
    }
  }

  if (isset($thesaurus->thInfo->Newsroom->values) && is_array($thesaurus->thInfo->Newsroom->values)) {
    foreach ($thesaurus->thInfo->Newsroom->values as $newsroom_value) {
      $all_options['newsroom'][$newsroom_value->code] = strtolower($newsroom_value->displayName);
      $js_options['product'][$newsroom_value->code] = array();
    }
  }

  if (isset($thesaurus->thInfo->Product->values) && is_array($thesaurus->thInfo->Product->values)) {
    foreach ($thesaurus->thInfo->Product->values as $product_value) {
      if (isset($product_value->links->Newsroom) && is_array($product_value->links->Newsroom)) {
        foreach ($product_value->links->Newsroom as $linked_newsroom) {
          $js_options['product'][$linked_newsroom->code][] = array(
            'code' => $product_value->code,
            'dname' => $product_value->displayName,
          );
        }
      }
      $all_options['product'][$product_value->code] = $product_value->displayName;
      $js_options['zone'][$product_value->code] = array();
    }
  }

  if (isset($thesaurus->thInfo->Zone->values) && is_array($thesaurus->thInfo->Zone->values)) {
    foreach ($thesaurus->thInfo->Zone->values as $zone_value) {
      if (isset($zone_value->links->Product) && is_array($zone_value->links->Product)) {
        foreach ($zone_value->links->Product as $linked_product) {
          $js_options['zone'][$linked_product->code][] = array(
            'code' => $zone_value->code,
            'dname' => $zone_value->displayName,
          );
        }
      }
      $all_options['zone'][$zone_value->code] = $zone_value->displayName;
    }
  }

  if (isset($thesaurus->thInfo->Edition->values) && is_array($thesaurus->thInfo->Edition->values)) {
    foreach ($thesaurus->thInfo->Edition->values as $edition_value) {
      $all_options['edition'][$edition_value->code] = $edition_value->displayName;
    }
  }

  return array($all_options, $js_options);
}
/**
 * Form to add a source on the package
 * */
function wallydam_page_admin_source_form($form_state, $edit_id = NULL){

  ctools_include('dependent');
  ctools_add_js('dependent');
  
  $form = array();
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('The identifier of the source (this name must be unique)'),
    '#description' => t('The identifier of the source used in the import files.'),
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  
  $form['archive'] = array(
    '#type' => 'select',
    '#title' => t('Archive'),
    '#description' => t('Archive the content into the DAM'),
    '#options' => array('' => '', 0 => t('No'), 1 => t('Yes')),
    '#required' => TRUE,
  );
  
  $form['archive_from_wallymport'] = array(
    '#type' => 'select',
    '#title' => t('Archive from wallymport'),
    '#description' => t('Archive the content even when it\'s comming from wallymport. (Example : Packages from newsgate must not be archived when comming from wallymport to avoid loop. But package from flow to node must be arhcived from wallymport)'),
  	'#options' => array(0 => t('No'), 1 => t('Yes')),
    '#dependency'       => array('edit-archive' => array(1)),
    '#process'          => array('ctools_dependent_process'),
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  
  //Edition
  if ($edit_id != NULL){
    $source = wallydam_getsourcebyid($edit_id);
    if ($source){
      $form['sid'] = array('#type' => 'value', '#value' => $edit_id);
      $form['name']['#default_value'] = $source->name;
      $form['archive']['#default_value'] = $source->archive;
      $form['archive_from_wallymport']['#default_value'] = $source->archive_from_wallymport;
    
      $form['delete'] = array(
        '#type' => 'markup',
        '#value' => '<a href = "/admin/wally/wallydam/deletesource/'.$source->sid.'">'.t('Delete').'</a>',
      );
    }
  }
  
  return $form;
}
/**
 * Validation of the wallydam_page_admin_addsource_form
 *  */
function wallydam_page_admin_source_form_validate($form, $form_state){
  if ($form_state['values']['sid'] === NULL && wallydam_getsourcebyname($form_state['values']['name']) !== NULL){
    form_set_error('name', t('A source with the name %name already exists', array('%name' => $form_state['values']['name'])));
  }
}
/**
* Submit of the wallydam_page_admin_addsource_form
*  */
function wallydam_page_admin_source_form_submit($form, $form_state){
  $source = new stdClass();
  $source->name = $form_state['values']['name'];
  $source->archive = $form_state['values']['archive'];
  $source->archive_from_wallymport = $form_state['values']['archive_from_wallymport'];
  $update = array();
  
  //Update
  if ($form_state['values']['sid']){
    $update = array('sid');
    $source->sid = $form_state['values']['sid'];
  }
  
  if (drupal_write_record('wallydam_source', $source, $update)){
    drupal_set_message(t('The source "@name" has been saved.', array('@name' => $form_state['values']['name'])));
  } else {
    drupal_set_message(t('The source could not be saved.'));
  }
}
/**
 * 
 * Form to delete a source
 * @param array $form_state
 * @param int $edit_id
 */
function wallydam_page_admin_source_delete_form($form_state, $delete_id = NULL){
  
  if ($delete_id != NULL){
    $form['sid'] = array(
      '#type' => 'value',
      '#value' => $delete_id,
    );
    $form['text'] = array(
      '#type' => 'markup',
      '#value' => t('Are you sure you want to delete this source? '),
    );
    $form['buttons']['submit'] = array('#type' => 'submit', '#value' => t('Delete') );
  }
  return $form;
}
  
/**
 * Submit of the source delete form
 */
function wallydam_page_admin_source_delete_form_submit($form, $form_state){
  
  $result = db_query("DELETE FROM {wallydam_source} WHERE sid = %d", $form_state['values']['sid']);
  if ($result != FALSE){
    $message = t('The source as been deleted');
  } else {
    $message = t('The importer as not been deleted');
  }
  drupal_set_message(t($message));
  drupal_goto('admin/wally/wallydam');
}
/**
 * Liste des sources
 */
function wallydam_list_source() {
  $content = '';
  $sources = wallydam_getsources();
  $header = array(t('Source'), t('Archive'), t('Archive from wallymport'), t('Operations'));
  $rows = array();
  $noyes = array(0 => t('No'), 1 => t('Yes'));
  foreach ($sources as $source){
    $rows[] = array(
      $source->name, 
      $noyes[$source->archive], 
      $noyes[$source->archive_from_wallymport], 
      '<a href="/admin/wally/wallydam/editsource/'.$source->sid.'">'.t('Edit').'</a>, <a href="/admin/wally/wallydam/deletesource/'.$source->sid.'">'.t('Delete').'</a>');
  }
  $content .= theme('table', $header, $rows);
  return $content;
}
