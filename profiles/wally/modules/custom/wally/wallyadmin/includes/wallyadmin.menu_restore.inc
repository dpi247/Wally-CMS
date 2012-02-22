<?php

function wallyadmin_page_form_menu_restore_form ($form_state) {
  $form = array();

  $form['backup'] = array(
    '#type' => 'submit',
    '#value' => t('Backup'),
    '#submit' => array('wallyadmin_page_form_menu_restore_form_submit_backup'),
    '#suffix' => '<br><span>'.t('This will copy the menu tables to new tables.').'</span><br><br><br><br><br><br><br><br><br><br>',
  );

  $form['restore'] = array(
    '#type' => 'submit',
    '#value' => t('Restore'),
    '#submit' => array('wallyadmin_page_form_menu_restore_form_submit_restore'),
    '#suffix' => '<br><span>'.t('This will replace the menu tables with the contents of the backup tables.').'</span>',
  );

  return  $form;
}

function wallyadmin_page_form_menu_restore_form_submit_backup ($form, $form_state) {
  $from_to = array(
    'menu_custom' => 'menu_custom_copy',
    'menu_links' => 'menu_links_copy',
    'menu_router' => 'menu_router_copy',
  );
  $error = FALSE;
  foreach ($from_to as $from => $to) {
    if (!_wydmin_copy_table($from, $to)) {
      $error = TRUE;
      drupal_set_message(t('SQL error while copying "@from" to "@to"', array('@from' => $from, '@to' => $to)), 'error');
    }
  }
  if (!$error) {
    drupal_set_message(t('Menu tables copied : @tables', array('@tables' => implode(', ', array_keys($from_to)))));
  }
}

function wallyadmin_page_form_menu_restore_form_submit_restore ($form, $form_state) {
  $from_to = array(
    'menu_custom_copy' => 'menu_custom',
    'menu_links_copy' => 'menu_links',
    'menu_router_copy' => 'menu_router',
  );
  $error = FALSE;
  foreach ($from_to as $from => $to) {
    if (!_wydmin_copy_table($from, $to)) {
      $error = TRUE;
      drupal_set_message(t('SQL error while restoring "@from" to "@to"', array('@from' => $from, '@to' => $to)), 'error');
    }
  }
  if (!$error) {
    drupal_set_message(t('Menu tables restored : @tables', array('@tables' => implode(', ', $from_to))));
    menu_rebuild();
    drupal_set_message(t('The menu router has been rebuilt.'));
  }
}

function _wydmin_copy_table ($from, $to) {
  if (db_table_exists($to)) {
    return (db_query("TRUNCATE TABLE {%s}", $to) && db_query("INSERT {%s} SELECT * FROM {%s}", $to, $from));
  } else {
    return (db_query("CREATE TABLE {%s} LIKE {%s}", $to, $from) && db_query("INSERT {%s} SELECT * FROM {%s}", $to, $from));
  }
}
