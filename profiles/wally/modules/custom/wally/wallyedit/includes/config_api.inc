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
    $modules = array('comment', 'menu', 'path', 'upload', 'pathauto');
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
    if (module_exists('menu')) {
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
