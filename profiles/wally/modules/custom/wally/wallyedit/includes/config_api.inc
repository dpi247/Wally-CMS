<?php 

/**
 * Return list of profiles
 */
function wyditadmin_get_profiles() {
  static $profiles = array();
  if (empty($profiles)) {
    $db_profiles = db_query('SELECT * FROM {wallyedit_profiles}');
    while ($profile = db_fetch_array($db_profiles)) {
      $profiles[$profile['pid']] = $profile;
    }
  }
  return $profiles;
}

/**
 * Get the default profile for current user
 */
function wallyedit_get_default_profile() {
  global $user;
  static $default_profile = array();

  if (!isset($default_profile[$user->uid])) {
    $default_profile[$user->uid] = 0;
    if (isset($user->wydit_default_profile)) {
      $default_profile[$user->uid] = $user->wydit_default_profile;
    } else {
      $available_profiles = wallyedit_get_available_profiles();
      if ($available_profiles != NULL){
        foreach ($available_profiles as $available_profile) {
          if ($available_profile['default_profile'] == 1) {
            $default_profile[$user->uid] = $available_profile['pid'];
            break;
          }
        }
      }
    }
  }
  
  return $default_profile[$user->uid];
}

/**
 * Get available profiles for current user
 */
function wallyedit_get_available_profiles() {
  global $user;
  static $available_profiles = array();
  
  if (!isset($available_profiles[$user->uid])) {
    module_load_include('inc', 'wallyedit', 'includes/config_api');
    $profiles = wyditadmin_get_profiles();
    foreach ($profiles as $profile) {
      if (wallyedit_access($profile['pid'])) {
        $available_profiles[$user->uid][$profile['pid']] = $profile;
      }
    }
  }
  
  return $available_profiles[$user->uid];
}

/**
 * Return list of tabs for a profile and a content type
 */
function wyditadmin_get_tabs($profile_id, $type_name) {
  static $tabs = array();
  if (!isset($tabs[$profile_id][$type_name])) {
    $tabs[$profile_id][$type_name] = array();
    $db_tabs = db_query('SELECT * FROM {wallyedit_tabs} WHERE pid = %d AND content_type = "%s" ORDER BY weight ASC', $profile_id, $type_name);
    while ($tab = db_fetch_array($db_tabs)) {
      $tabs[$profile_id][$type_name][$tab['tid']] = $tab;
    }
    $tabs[$profile_id][$type_name][0] = array(
      'tid' => 0,
      'content_type' => $type_name,
      'name' => 'no_tab',
      'label' => t('Hidden'),
    );
  }
  return $tabs[$profile_id][$type_name];
}

/**
 * Return list of all tabs
 */
function wyditadmin_get_all_tabs() {
  static $tabs = array();
  if (empty($tabs)) {
    $db_tabs = db_query('SELECT * FROM {wallyedit_tabs}');
    while ($tab = db_fetch_array($db_tabs)) {
      $tabs[$tab['tid']] = $tab;
    }
  }
  return $tabs;
}

/**
 * Return list of groups for a content type
 */
function wyditadmin_get_groups($type_name) {
  static $groups = array();
  if (!isset($groups[$type_name])) {
    $groups[$type_name] = array();
    $db_groups = db_query('SELECT * FROM {wallyedit_groups} WHERE content_type = "%s" ORDER BY weight ASC', $type_name);
    while ($group = db_fetch_array($db_groups)) {
      $groups[$type_name][$group['gid']] = $group;
    }
    $groups[$type_name][0] = array(
      'gid' => 0,
      'content_type' => $type_name,
      'name' => 'no_group',
      'label' => t('No group'),
    );
  }
  return $groups[$type_name];
}

/**
 * Return list of all groups
 */
function wyditadmin_get_all_groups() {
  static $groups = array();
  if (empty($groups)) {
    $db_groups = db_query('SELECT * FROM {wallyedit_groups}');
    while ($group = db_fetch_array($db_groups)) {
      $groups[$group['gid']] = $group;
    }
  }
  return $groups;
}

/**
 * Return list of fields in a flat stucture
 */
function wyditadmin_get_fields_flat($profile_id, $type_name) {
  static $fields = array();
  if (!isset($fields[$profile_id][$type_name])) {
    $fields[$profile_id][$type_name] = array();
    $db_fields = db_query('SELECT * FROM {wallyedit_fields} WHERE pid = %d AND content_type = "%s" ORDER BY weight ASC', $profile_id, $type_name);
    while ($field = db_fetch_array($db_fields)) {
      $fields[$profile_id][$type_name][$field['name']] = $field;
    }
  }
  return $fields[$profile_id][$type_name];
}

/**
 * Return list of all fields
 */
function wyditadmin_get_all_fields() {
  static $fields = array();
  if (empty($fields)) {
    $db_fields = db_query('SELECT * FROM {wallyedit_fields}');
    while ($field = db_fetch_array($db_fields)) {
      $fields[$field['fid']] = $field;
    }
  }
  return $fields;
}


/**
 * Return list of fields in a tree stucture
 */
function wyditadmin_get_fields_tree($profile_id, $type_name) {
  static $tree_fields = array();
  if (!isset($tree_fields[$profile_id][$type_name])) {
    $tree_fields[$profile_id][$type_name] = array();
    $tabs = wyditadmin_get_tabs($profile_id, $type_name);
    $groups = wyditadmin_get_groups($type_name);
    $flat_fields = wyditadmin_get_fields_flat($profile_id, $type_name);
    foreach ($tabs as $tab) {
      $tree_fields[$profile_id][$type_name][$tab['name']] = $tab;
      $tree_fields[$profile_id][$type_name][$tab['name']]['elements'] = array();
      foreach ($groups as $group) {
        $tree_fields[$profile_id][$type_name][$tab['name']]['elements'][$group['name']] = $group;
        $tree_fields[$profile_id][$type_name][$tab['name']]['elements'][$group['name']]['fields'] = array();
      }
    }
    foreach ($flat_fields as $fid => $field) {
      $tab_name = $tabs[$field['tid']]['name'];
      $group_name = $groups[$field['gid']]['name'];
      if ($group_name != 'none') {
        $tree_fields[$profile_id][$type_name][$tab_name]['elements'][$group_name]['fields'][$field['name']] = $field;
      } else {
        $tree_fields[$profile_id][$type_name][$tab_name]['elements'][$field['name']] = $field;
      }
    }
    foreach ($tabs as $tab) {
      foreach ($groups as $group) {
        if (empty($tree_fields[$profile_id][$type_name][$tab['name']]['elements'][$group['name']]['fields'])) {
          unset ($tree_fields[$profile_id][$type_name][$tab['name']]['elements'][$group['name']]);
        }
      }
      if (empty($tree_fields[$profile_id][$type_name][$tab['name']]['elements'])) {
        unset ($tree_fields[$profile_id][$type_name][$tab['name']]);
      }
    }
  }
  return $tree_fields[$profile_id][$type_name];
}

/**
 * Return list of existing fields for a content type
 */
function wyditadmin_get_existing_fields($type_name, $field = NULL) {
  static $existing_fields = array();
  if (!isset($existing_fields[$type_name])) {
    module_load_include('inc', 'content', 'includes/content.crud');
    $content_fields = content_field_instance_read(array('type_name' => $type_name));
    foreach ($content_fields as $content_field) {
      $existing_fields[$type_name][$content_field['field_name']] = array(
        'type' => $content_field['type'],
        'widget' => $content_field['widget']['type'],
        'name' => $content_field['field_name'],
        'label' => $content_field['widget']['label'],
        'multiple' => $content_field['multiple'],
        'module' => $content_field['module'],
        'required' => $content_field['required'],
      );
      if ($content_field['type'] ==  'nodereference') {
        $existing_fields[$type_name][$content_field['field_name']]['referenceable_types'] = $content_field['referenceable_types'];
      }
    }

    $form_state = array();
    $node = new Stdclass;
    $node->type = $type_name;
    $node->name = '';
    module_load_include('inc', 'node', 'node.pages');
    $form = node_form($form_state, $node);
    $modules = array('comment', 'path', 'upload', 'pathauto');
    if (!variable_get('menu_override_parent_selector', FALSE)) {
      $modules[] = 'menu';
    }
    foreach ($modules as $module) {
      if (module_exists($module)) {
        $function = $module.'_form_alter';
        $function($form, $form_state, $type_name.'_node_form');
      }
    }
    $existing_fields[$type_name]['title'] = array(
      'type' => $form['title']['#type'],
      'widget' => $form['title']['#type'],
      'name' => 'title',
      'label' => $form['title']['#title'],
    );
    $existing_fields[$type_name]['revision'] = array(
      'type' => $form['revision_information']['revision']['#type'],
      'widget' => $form['revision_information']['revision']['#type'],
      'name' => 'revision',
      'label' => $form['revision_information']['revision']['#title'],
    );
    $existing_fields[$type_name]['log'] = array(
      'type' => $form['revision_information']['log']['#type'],
      'widget' => $form['revision_information']['log']['#type'],
      'name' => 'log',
      'label' => $form['revision_information']['log']['#title'],
    );
    $existing_fields[$type_name]['name'] = array(
      'type' => $form['author']['name']['#type'],
      'widget' => $form['author']['name']['#type'],
      'name' => 'name',
      'label' => $form['author']['name']['#title'],
    );
    $existing_fields[$type_name]['date'] = array(
      'type' => $form['author']['date']['#type'],
      'widget' => $form['author']['date']['#type'],
      'name' => 'date',
      'label' => $form['author']['date']['#title'],
    );
    $existing_fields[$type_name]['status'] = array(
      'type' => $form['options']['status']['#type'],
      'widget' => $form['options']['status']['#type'],
      'name' => 'status',
      'label' => $form['options']['status']['#title'],
    );
    $existing_fields[$type_name]['promote'] = array(
      'type' => $form['options']['promote']['#type'],
      'widget' => $form['options']['promote']['#type'],
      'name' => 'promote',
      'label' => $form['options']['promote']['#title'],
    );
    $existing_fields[$type_name]['sticky'] = array(
      'type' => $form['options']['sticky']['#type'],
      'widget' => $form['options']['sticky']['#type'],
      'name' => 'sticky',
      'label' => $form['options']['sticky']['#title'],
    );
    if (module_exists('comment')) {
      $existing_fields[$type_name]['comment'] = array(
        'type' => $form['comment_settings']['comment']['#type'],
        'widget' => $form['comment_settings']['comment']['#type'],
        'name' => 'comment',
        'label' => $form['comment_settings']['#title'],
      );
    }
    if (module_exists('menu') && isset($form['menu'])) {
      $existing_fields[$type_name]['link_title'] = array(
        'type' => $form['menu']['link_title']['#type'],
        'widget' => $form['menu']['link_title']['#type'],
        'name' => 'link_title',
        'label' => $form['menu']['link_title']['#title'],
      );
      $existing_fields[$type_name]['parent'] = array(
        'type' => $form['menu']['parent']['#type'],
        'widget' => $form['menu']['parent']['#type'],
        'name' => 'parent',
        'label' => $form['menu']['parent']['#title'],
      );
      $existing_fields[$type_name]['weight'] = array(
        'type' => $form['menu']['weight']['#type'],
        'widget' => $form['menu']['weight']['#type'],
        'name' => 'weight',
        'label' => $form['menu']['weight']['#title'],
      );
    }
    if (module_exists('path')) {
      $existing_fields[$type_name]['path'] = array(
        'type' => $form['path']['path']['#type'],
        'widget' => $form['path']['path']['#type'],
        'name' => 'path',
        'label' => $form['path']['#title'],
      );
    }
    if (module_exists('upload')) {
      $existing_fields[$type_name]['upload'] = array(
        'type' => $form['attachments']['wrapper']['new']['upload']['#type'],
        'widget' => $form['attachments']['wrapper']['new']['upload']['#type'],
        'name' => 'upload',
        'label' => $form['attachments']['wrapper']['new']['upload']['#title'],
      );
    }
    if (module_exists('pathauto')) {
      $existing_fields[$type_name]['pathauto_perform_alias'] = array(
        'type' => $form['path']['pathauto_perform_alias']['#type'],
        'widget' => $form['path']['pathauto_perform_alias']['#type'],
        'name' => 'pathauto_perform_alias',
        'label' => $form['path']['pathauto_perform_alias']['#title'],
      );
    }
  }
  return $field ? $existing_fields[$type_name][$field] : $existing_fields[$type_name];
}

/**
 * Return tabs structure for fields management
 */
function wallyedit_get_onglets($profile_id, $type_name, $form_state=array()) {
  static $onglets = array();
  
  if (!isset($onglets[$profile_id][$type_name])) {
    module_load_include('inc', 'content', 'includes/content.node_form');
    module_load_include('inc', 'content', 'includes/content.admin');
    $form = drupal_retrieve_form('content_field_overview_form', $form_state, $type_name);
  
    $node = new Stdclass;
    $node->type = $type_name;
    $node->name = '';
    module_load_include('inc', 'node', 'node.pages');
    $node_form = drupal_retrieve_form($type_name.'_node_form', $form_state, $node);
    drupal_prepare_form('node_form', $node_form, $form_state);
  
    $fields_and_groups = wydit_get_fields_infos_for_typename($type_name, $profile_id, 'fields');
    foreach (element_children($node_form) as $element_name) {
      if ($node_form[$element_name]['#type'] == 'fieldset') {
        wallyedit_add_new_group_to_fields($fields_and_groups, $node_form[$element_name], $element_name);
      }
    }
  
    //remove fields already included in a groups
    foreach ($fields_and_groups as $element_name => &$elem) {
      if (isset($elem['group'])) {
        $elem = $elem['group'];
      } elseif (isset($elem['field'])) {
        $elem = $elem['field'];
      } else {
        $elem = array_merge($form[$element_name], $elem);
        foreach ($elem as $key => &$value) {
          if (is_array($value) && isset($value['#value'])) {
            $value = $value['#value'];
          }
        }
      }
      
      if (isset($elem['group_name'])) {
        foreach($elem["fields"] as $key => $field) {
          unset($fields_and_groups[$key]);
        }
      }
    }
  
    $db_tabs = db_query('SELECT tid, name, label, weight, erasable FROM {wallyedit_tabs_config} WHERE pid = %d AND content_type = "%s"', $profile_id, $type_name);
    $tab_settings = db_fetch_array(db_query('SELECT tabs FROM {wallyedit_tabs_edition} WHERE pid = %d AND content_type = "%s"', $profile_id, $type_name));
    while (is_string($tab_settings['tabs'])) {
      $tab_settings['tabs'] = unserialize($tab_settings['tabs']);
    }
    $tab_settings = $tab_settings['tabs'];
  
    $temp_onglets = array();
    while ($db_tab = db_fetch_array($db_tabs)) {
      $temp_onglets[$db_tab['name']] = $db_tab;
      $temp_onglets[$db_tab['name']]['elements'] = array();
    }
    $temp_onglets['no_tab'] = array(
      'name' => 'no_tab',
      'label' => t('Uncategorized'),
      'weight' => 1000,
      'erasable' => 0,
      'elements' => array(),
    );
  
    foreach ($fields_and_groups as $element_name => $element) {
      $element_settings = isset($tab_settings['level1'][$element_name]) ? $tab_settings['level1'][$element_name] : array();
      if (isset($temp_onglet[$element_settings['onglet']])) {
        $tab_name = $element_settings['onglet'];
      } else {
        // Default tab for uncategorized elements
        $tab_name = 'no_tab';
      }
      if (isset($element['group_name'])) {
        // This is a group
        // Merge to edit weight
        $temp_onglets[$tab_name]['elements'][$element_name] = array_merge($fields_and_groups[$element_name], $element_settings);
        foreach ($fields_and_groups[$element_name]['fields'] as $field_name => $field) {
          $element_settings['level2'][$field_name] = isset($element_settings['level2'][$field_name]) ? $element_settings['level2'][$field_name] : array();
          $temp_onglets[$tab_name]['elements'][$element_name]['fields'][$field_name] = array_merge($fields_and_groups[$element_name]['fields'][$field_name], $element_settings['level2'][$field_name]);
        }
      } else {
        // This is a field
        $temp_onglets[$tab_name]['elements'][$element_name] = array_merge($fields_and_groups[$element_name], $element_settings);
      }
    }
    
    $onglets[$profile_id][$type_name] = $temp_onglets;
  }

  return $onglets[$profile_id][$type_name];
}

function wallyedit_get_extra_fields(&$extra_fields, $form, $cck_fields, $parent = '') {
  foreach (element_children($form) as $element_key) {
    if (empty($parent) && $form[$element_key]['#type'] == 'fieldset') {
      $element = $form[$element_key];
      wallyedit_get_extra_fields($extra_fields, $element, $cck_fields, $element_key);
    } elseif (!empty($parent)) {
      if (!in_array($element_key, $cck_fields)) {
        $extra_fields[$element_key]['display_settings']['parent'] = $parent;
      }
    }
  }
}

function wallyedit_add_new_group_to_fields(&$fields, $node_fields, $field_name) {
  $fields[$field_name]['group']['group_name'] = $field_name;
  $fields[$field_name]['group']['group_type'] = 'Standard group';
  $fields[$field_name]['group']['label'] = $node_fields['#title'];
  $fields[$field_name]['group']['weight'] = $node_fields['#weight'];
  
  $elem_child = element_children($node_fields);
  
  if (!empty($elem_child)) {
    $fields[$field_name]['group']['fields'] = array();
    foreach ($elem_child as $node_field_name) {
      $node_field = $node_fields[$node_field_name];
      $temp_field = array();
      $temp_field['field_name'] = $node_field_name;
      $temp_field['weight'] = 0;
      $temp_field['label'] = $node_field['#title'];
      $fields[$field_name]['group']['fields'][$node_field_name] = $temp_field;
    }
  }
}
