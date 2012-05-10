<?php

module_load_include('inc', 'wallyedit', 'includes/config_api');

function wallyedit_form_config_import_export_form($form_state) {
  $form = array(
    '#method' => 'POST',
    '#attributes' => array(
      'enctype' => 'multipart/form-data',
    ),
  );
  
  ctools_include('dependent');
  ctools_add_js('dependent');
  
  $form['export'] = array(
    '#type' => 'fieldset',
    '#title' => t('Export'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $type_options = array(
    t('All'),
    t('Selected profiles'),
    t('Selected content types'),
  );
  $form['export']['export_type'] = array(
    '#type' => 'radios',
    '#title' => t('Datas to export'),
    '#options' => $type_options,
    '#default_value' => 0,
  );
  
  $profiles = wyditadmin_get_profiles();
  $profiles_options = array();
  foreach ($profiles as $profile) {
    $profiles_options[$profile['pid']] = $profile['label'];
  }
  $form['export']['select_profiles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Select profiles to export'),
    '#options' => $profiles_options,
    '#process' => array('ctools_dependent_process', 'expand_checkboxes'),
    '#dependency' => array('radio:export_type' => array(1)),
    '#prefix' => '<div id="edit-select-profiles-wrapper">',
    '#suffix' => '</div>',
  );
  
  $names = node_get_types('names');
  $names_options = array();
  foreach ($names as $key => $name) {
    $names_options[$key] = $name;
  }
  $form['export']['select_content_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Select content types to export'),
    '#options' => $names_options,
    '#process' => array('ctools_dependent_process', 'expand_checkboxes'),
    '#dependency' => array('radio:export_type' => array(2)),
    '#prefix' => '<div id="edit-select-content-types-wrapper">',
    '#suffix' => '</div>',
  );
  
  $form['export']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Export'),
    '#validate' => array('wallyedit_form_config_import_export_form_export_validate'),
    '#submit' => array('wallyedit_form_config_import_export_form_export_submit'),
  );
  
  $form['import'] = array(
    '#type' => 'fieldset',
    '#title' => t('Import'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $form['import']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
    '#validate' => array('wallyedit_form_config_import_export_form_import_validate'),
    '#submit' => array('wallyedit_form_config_import_export_form_import_submit'),
    '#prefix' => t('Choose a tar file to import:').' <input type="file" name="uploadedfile" /><br/>',
  );
  
  return $form;
}

function wallyedit_form_config_import_export_form_export_validate($form, $form_state) {
  $values = $form_state['values'];
  $empty = TRUE;
  switch ($values['export_type']) {
    case 0:
      break;
    case 1:
      foreach ($values['select_profiles'] as $val) {
        if ($val) $empty = FALSE;
      }
      if ($empty) {
        form_set_error('select_profiles', t('Select at least one profile to export'));
      }
      break;
    case 2:
      foreach ($values['select_content_types'] as $val) {
        if ($val) $empty = FALSE;
      }
      if ($empty) {
        form_set_error('select_content_types', t('Select at least one content type to export'));
      }
      break;
  }
}

function wallyedit_form_config_import_export_form_export_submit($form, $form_state) {
  $values = $form_state['values'];

  $profiles = wyditadmin_get_profiles();
  $export_profiles = array();
  switch ($values['export_type']) {
    case 0:
    case 2:
      $export_profiles = $profiles;
      break;
    case 1:
      foreach ($values['select_profiles'] as $key => $select_profile) {
        if ($key === $select_profile) {
          $export_profiles[$select_profile] = $profiles[$select_profile];
        }
      }
      break;
  }
  
  $export_content_types = array();
  switch ($values['export_type']) {
    case 0:
    case 1:
      $export_content_types = array_keys($values['select_content_types']);
      break;
    case 2:
      foreach ($values['select_content_types'] as $key => $select_content_type) {
        if ($key === $select_content_type) {
          $export_content_types[] = $select_content_type;
        }
      }
      break;
  }

  $dom = _wallyedit_import_export_generate_dom($export_profiles, $export_content_types);

  $site_name = str_replace(' ', '-', strtolower(variable_get('site_name', '')));
  $date = date('Y-m-d');
  $filename = 'wallyedit-config-export-'.$site_name.'-'.$date;
  $files = array(
    array(
      'name' => $filename,
      'extension' => '.xml',
      'content' => $dom->saveXML(),
    ),
  );
  wallytoolbox_export_files_to_tar($files, $filename.'.tar');
}

function wallyedit_form_config_import_export_form_import_validate($form, $form_state) {
  if (!isset($_FILES['uploadedfile']) || empty($_FILES['uploadedfile']) || $_FILES['uploadedfile']['error'] > 0) {
    form_set_error('import', t('No files to import'));
  }
}

function wallyedit_form_config_import_export_form_import_submit($form, $form_state) {
  $post_file = $_FILES['uploadedfile'];

  if (file_exists($post_file['tmp_name'])) {
    $filename = $post_file['name'];
    $tmp_path = $post_file['tmp_name'];
    $return = _wallyedit_import_export_extract_dom_from_tar($filename, $tmp_path);
    if ($return['error']) {
      drupal_set_message($return['error_msg'], 'error');
    } else {
      _wallyedit_import_export_import_from_dom($return['content']);
      drupal_set_message(t('Import done'));
    }
  }
}

function _wallyedit_import_export_generate_dom($export_profiles, $export_content_types) {
  $dom = new DomDocument('1.0', 'UTF-8');
  $config_export = $dom->appendChild($dom->createElement('ConfigExport'));
  $dom_profiles = $config_export->appendChild($dom->createElement('Profiles'));
  $dom_content_types = $config_export->appendChild($dom->createElement('ContentTypes'));
  $dom_interfaces = $config_export->appendChild($dom->createElement('Interfaces'));
  
  foreach ($export_profiles as $profile) {
    $dom_profile = $dom_profiles->appendChild($dom->createElement('Profile'));
    $dom_profile->setAttribute('pid', $profile['pid']);
    $dom_profile->setAttribute('default', $profile['default_profile']);
    $dom_profile->appendChild($dom->createElement('label', $profile['label']));
    
    foreach ($export_content_types as $content_type) {
      $dom_content_type = $dom_content_types->appendChild($dom->createElement('ContentType'));
      $dom_content_type->setAttribute('name', $content_type);
      $dom_tabs = $dom_content_type->appendChild($dom->createElement('Tabs'));
      $dom_groups = $dom_content_type->appendChild($dom->createElement('Groups'));
      $dom_fields = $dom_content_type->appendChild($dom->createElement('Fields'));

      $tabs = wyditadmin_get_tabs($profile['pid'], $content_type);
      unset($tabs[0]);
      foreach ($tabs as $tab) {
        $dom_tab = $dom_tabs->appendChild($dom->createElement('Tab'));
        $dom_tab->setAttribute('tid', $tab['tid']);
        $dom_tab->appendChild($dom->createElement('pid', $tab['pid']));
        $dom_tab->appendChild($dom->createElement('name', $tab['name']));
        $dom_tab->appendChild($dom->createElement('label', $tab['label']));
        $dom_tab->appendChild($dom->createElement('weight', $tab['weight']));
        $dom_tab->appendChild($dom->createElement('erasable', $tab['erasable']));
      }
      
      $groups = wyditadmin_get_groups($content_type);
      unset($groups[0]);
      foreach ($groups as $group) {
        $dom_group = $dom_groups->appendChild($dom->createElement('Group'));
        $dom_group->setAttribute('gid', $group['gid']);
        $dom_group->appendChild($dom->createElement('name', $group['name']));
        $dom_group->appendChild($dom->createElement('label', $group['label']));
        $dom_group->appendChild($dom->createElement('weight', $group['weight']));
      }
      
      $fields = wyditadmin_get_fields_flat($profile['pid'], $content_type);
      foreach ($fields as $field) {
        $dom_field = $dom_fields->appendChild($dom->createElement('Field'));
        $dom_field->setAttribute('fid', $field['fid']);
        $dom_field->appendChild($dom->createElement('pid', $field['pid']));
        $dom_field->appendChild($dom->createElement('tid', $field['tid']));
        $dom_field->appendChild($dom->createElement('gid', $field['gid']));
        $dom_field->appendChild($dom->createElement('type', $field['type']));
        $dom_field->appendChild($dom->createElement('widget', $field['widget']));
        $dom_field->appendChild($dom->createElement('name', $field['name']));
        $dom_field->appendChild($dom->createElement('label', $field['label']));
        $dom_field->appendChild($dom->createElement('weight', $field['weight']));
        $dom_field->appendChild($dom->createElement('display', $field['display']));
      }
    }
    
    $interfaces = variable_get('wallyedit_content_types_interface_type_'.$profile['pid'], array());
    $dom_interface = $dom_interfaces->appendChild($dom->createElement('Interface'));
    $dom_interface->setAttribute('pid', $profile['pid']);
    foreach ($interfaces as $content_type => $interface) {
      if (in_array($content_type, $export_content_types)) {
        $dom_content_type = $dom_interface->appendChild($dom->createElement('ContentType'));
        $dom_content_type->setAttribute('name', $content_type);
        $dom_content_type->appendChild($dom->createElement('type', $interface));
      }
    }
  }
  
  return $dom;
}

function _wallyedit_import_export_extract_dom_from_tar($filename, $tmp_path) {
  set_error_handler('_wallyedit_import_export_tar_error');
  include_once 'Archive/Tar.php';
  restore_error_handler();
  
  $return = array(
    'error' => FALSE,
    'error_msg' => '',
    'content' => NULL,
  );

  if (class_exists('Archive_Tar')) {
    $ext = pathinfo($filename, PATHINFO_EXTENSION);
    $dir = '/tmp/';

    //Check file extension
    if($ext == 'tar') {
      //Open file to read
      $tar = new Archive_Tar($tmp_path, TRUE);

      //Extract the file using Pear library
      if($tar->extract($dir)) {
        $list_content = $tar->listContent();
        $local_path = $dir.$list_content[0]['filename'];
        $handle = fopen($local_path, 'r');

        if (!$handle) {
          $return['error'] = TRUE;
          $return['error_msg'] = t('The XML file (!xml_file) isn\'t readable', array('!xml_file' => $local_path));
        } else {
          $contents = fread($handle, filesize($local_path));
          $dom = new DomDocument();
          $dom->LoadXML(trim($contents));
          $return['content'] = $dom;
        }
      } else {
        $return['error'] = TRUE;
        $return['error_msg'] = t('Unable to extract tar file contents');
      }
    } else {
      $return['error'] = TRUE;
      $return['error_msg'] = t('Given file is not a tar file');
    }
  } else {
    $return['error'] = TRUE;
    $return['error_msg'] = t('Class "Archive_Tar" not found');
  }

  return $return;
}

function _wallyedit_import_export_import_from_dom($dom) {
  $pid_mapping = $tid_mapping = $gid_mapping = $fid_mapping = array();

  $dom_profiles = $dom->getElementsByTagName('Profiles');
  if ($dom_profiles->item(0)) {
    $dom_profile = $dom_profiles->item(0)->getElementsByTagName('Profile');
    if ($dom_profile->item(0)) {
      $current_profiles = wyditadmin_get_profiles();
      
      foreach ($dom_profile as $dom_prof) {
        $profile = array();
        $old_pid = $profile['pid'] = $dom_prof->getAttributeNode('pid')->value;
        $profile['default_profile'] = $dom_prof->getAttributeNode('default')->value;
        $profile['label'] = $dom_prof->getElementsByTagName('label')->item(0)->nodeValue;
        
        if ($profile['default_profile']) {
          db_query("UPDATE {wallyedit_profiles} SET default_profile = 0");
        }

        if (array_key_exists($profile['pid'], $current_profiles) && ($profile['label'] == $current_profiles[$profile['pid']]['label'])) {
          drupal_write_record('wallyedit_profiles', $profile, array('pid'));
        } else {
          unset($profile['pid']);
          drupal_write_record('wallyedit_profiles', $profile);
        }
        $current_profiles[$profile['pid']] = $profile;
        $pid_mapping[$old_pid] = $profile['pid'];
      }
    }
  }

  $dom_content_types = $dom->getElementsByTagName('ContentTypes');
  if ($dom_content_types->item(0)) {
    $dom_content_type = $dom_content_types->item(0)->getElementsByTagName('ContentType');
    if ($dom_content_type->item(0)) {
      $current_tabs = wyditadmin_get_all_tabs();
      $current_groups = wyditadmin_get_all_groups();
      $current_fields = wyditadmin_get_all_fields();
      
      foreach ($dom_content_type as $dom_ct) {
        $content_type = $dom_ct->getAttributeNode('name')->value;

        $dom_tabs = $dom_ct->getElementsByTagName('Tabs')->item(0);
        foreach ($dom_tabs->getElementsByTagName('Tab') as $dom_tab) {
          $tab = array();
          $old_tid = $tab['tid'] = $dom_tab->getAttributeNode('tid')->value;
          $tab['pid'] = $pid_mapping[$dom_tab->getElementsByTagName('pid')->item(0)->nodeValue];
          $tab['content_type'] = $content_type;
          $tab['name'] = $dom_tab->getElementsByTagName('name')->item(0)->nodeValue;
          $tab['label'] = $dom_tab->getElementsByTagName('label')->item(0)->nodeValue;
          $tab['weight'] = $dom_tab->getElementsByTagName('weight')->item(0)->nodeValue;
          $tab['erasable'] = $dom_tab->getElementsByTagName('erasable')->item(0)->nodeValue;

          if (array_key_exists($tab['tid'], $current_tabs) && ($tab['pid'] == $current_tabs[$tab['tid']]['pid']) && ($tab['content_type'] == $current_tabs[$tab['tid']]['content_type'])) {
            drupal_write_record('wallyedit_tabs', $tab, array('tid'));
          } else {
            if (!array_key_exists($tab['tid'], $current_tabs)) {
              $force = $tab['tid'];
            } else {
              $force = FALSE;
            }
            drupal_write_record('wallyedit_tabs', $tab);
            if ($force !== FALSE) {
              db_query("UPDATE {wallyedit_tabs} SET tid = %d WHERE tid = %d", $force, $tab['tid']);
              $tab['tid'] = $force;
            }
          }
          $current_tabs[$tab['tid']] = $tab;
          $tid_mapping[$old_tid] = $tab['tid'];
        }

        $dom_groups = $dom_ct->getElementsByTagName('Groups')->item(0);
        foreach ($dom_groups->getElementsByTagName('Group') as $dom_group) {
          $group = array();
          $old_gid = $group['gid'] = $dom_group->getAttributeNode('gid')->value;
          $group['content_type'] = $content_type;
          $group['name'] = $dom_group->getElementsByTagName('name')->item(0)->nodeValue;
          $group['label'] = $dom_group->getElementsByTagName('label')->item(0)->nodeValue;
          $group['weight'] = $dom_group->getElementsByTagName('weight')->item(0)->nodeValue;

          if (array_key_exists($group['gid'], $current_groups) && ($group['content_type'] == $current_groups[$group['gid']]['content_type'])) {
            drupal_write_record('wallyedit_groups', $group, array('gid'));
          } else {
            if (!array_key_exists($group['gid'], $current_groups)) {
              $force = $group['gid'];
            } else {
              $force = FALSE;
            }
            drupal_write_record('wallyedit_groups', $group);
            if ($force !== FALSE) {
              db_query("UPDATE {wallyedit_groups} SET gid = %d WHERE gid = %d", $force, $group['gid']);
              $group['gid'] = $force;
            }
          }
          $current_groups[$group['gid']] = $group;
          $gid_mapping[$old_gid] = $group['gid'];
        }

        $dom_fields = $dom_ct->getElementsByTagName('Fields')->item(0);
        foreach ($dom_fields->getElementsByTagName('Field') as $dom_field) {
          $field = array();
          $field['fid'] = $dom_field->getAttributeNode('fid')->value;
          $field['pid'] = $pid_mapping[$dom_field->getElementsByTagName('pid')->item(0)->nodeValue];
          $field['content_type'] = $content_type;
          $field['tid'] = $tid_mapping[$dom_field->getElementsByTagName('tid')->item(0)->nodeValue];
          $field['gid'] = $gid_mapping[$dom_field->getElementsByTagName('gid')->item(0)->nodeValue];
          $field['type'] = $dom_field->getElementsByTagName('type')->item(0)->nodeValue;
          $field['widget'] = $dom_field->getElementsByTagName('widget')->item(0)->nodeValue;
          $field['name'] = $dom_field->getElementsByTagName('name')->item(0)->nodeValue;
          $field['label'] = $dom_field->getElementsByTagName('label')->item(0)->nodeValue;
          $field['weight'] = $dom_field->getElementsByTagName('weight')->item(0)->nodeValue;
          $field['display'] = $dom_field->getElementsByTagName('display')->item(0)->nodeValue;

          if (array_key_exists($field['fid'], $current_fields) && ($field['pid'] == $current_fields[$field['fid']]['pid']) && ($field['content_type'] == $current_fields[$field['fid']]['content_type'])) {
            drupal_write_record('wallyedit_fields', $field, array('fid'));
          } else {
            if (!array_key_exists($field['fid'], $current_fields)) {
              $force = $field['fid'];
            } else {
              $force = FALSE;
            }
            drupal_write_record('wallyedit_fields', $field);
            if ($force !== FALSE) {
              db_query("UPDATE {wallyedit_fields} SET fid = %d WHERE fid = %d", $force, $field['fid']);
              $field['fid'] = $force;
            }
          }
          $current_fields[$field['fid']] = $field;
        }
      }
    }
  }

  $dom_interfaces = $dom->getElementsByTagName('Interfaces');
  if ($dom_interfaces->item(0)) {
    $dom_interface = $dom_interfaces->item(0)->getElementsByTagName('Interface');
    if ($dom_interface->item(0)) {
      foreach ($dom_interface as $dom_int) {
        $pid = $pid_mapping[$dom_int->getAttributeNode('pid')->value];
        $current_interfaces = variable_get('wallyedit_content_types_interface_type_'.$pid, array());
        foreach ($dom_int->getElementsByTagName('ContentType') as $dom_content_type) {
          $content_type = $dom_content_type->getAttributeNode('name')->value;
          $current_interfaces[$content_type] = $dom_content_type->getElementsByTagName('type')->item(0)->nodeValue;
        }
        variable_set('wallyedit_content_types_interface_type_'.$pid, $current_interfaces);
      }
    }
  }
}

function _wallyedit_import_export_tar_error($errno, $errstr, $errfile, $errline) {
  if ($errno === 2) {
    drupal_set_message(t('The "Archive_Tar" package need to be installed on your system to use this functionality (see !link for further informations)', array('!link' => l('here', 'http://pear.php.net/package/Archive_Tar/redirected'))), 'error');
  }
}
