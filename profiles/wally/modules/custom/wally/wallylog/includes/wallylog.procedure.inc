<?php

function wallylog_page_procedure_form($form_state) {
  $form = array();
  
  $form['clear_cache'] = array(
    '#type' => 'submit',
    '#value' => t('Clear Cache'),
    '#submit' => array('wallylog_page_procedure_form_submit_cache'),
    '#suffix' => '<br><span>'.t('Clear the CSS cache and all database cache tables which store page, node, theme and variable caches.').'</span><br><br><br><br><br>',
  );

  $form['rebuild_menus'] = array(
    '#type' => 'submit',
    '#value' => t('Rebuild Menus'),
    '#submit' => array('wallylog_page_procedure_form_submit_menu'),
    '#suffix' => '<br><span>'.t('This will rebuild all menu caches.').'</span><br><br><br><br><br>',
  );
  
  $form['rebuild_theme'] = array(
    '#type' => 'submit',
    '#value' => t('Rebuild Theme Registry'),
    '#submit' => array('wallylog_page_procedure_form_submit_theme'),
    '#suffix' => '<br><span>'.t('This will rebuild the theme registry.').'</span><br><br><br><br><br>',
  );
  
  return $form;
}

function wallylog_page_procedure_form_submit_cache($form, $form_sate) {
  drupal_goto('admin/content/clearcache', 'destination='.$_GET['q']);
}

function wallylog_page_procedure_form_submit_menu($form, $form_sate) {
  drupal_goto('admin/wally/menurebuild', 'destination='.$_GET['q']);
}

function wallylog_page_procedure_form_submit_theme($form, $form_sate) {
  drupal_goto('admin/content/rebuildtheme', 'destination='.$_GET['q']);
}
