<?php

function theme_wallyextra_custom_version_content_type_edit_form($form) {
  ctools_include('ajax');

  $content = '';
  $content .= '<div>';
  $content .= '<ul class = "tabs primary">';
  $content .= '<li id = "custom" class = "active">'.ctools_ajax_text_button(t('Custom Content'), 'wallyextra/customcontent/form/custom/ajax', t('Custom content')).'</li>';
  $content .= '<li id = "version">'.ctools_ajax_text_button(t('Versions'), 'wallyextra/customcontent/form/version/ajax', t('Version')).'</li>';
  $content .= '</ul>';
  $content .= '<div id = "wallyextra_tabs">';
  $content .= '<div id = "custom_form">';
  $content .= drupal_render($form);
  $content .= '</div>';
  $content .= '<div id = "version_form" style = "display:none;">';
  $versions = wallyextra_custom_content_type_getversions($form['conf']['#id']);
  $content .= theme('wallyextra_custom_content_type_listversion', $versions);
  $content .= '</div>';
  $content .= '</div>';
  $content .= '</div>';

  return $content;
}

function theme_wallyextra_custom_content_type_listversion($versions) {
  if ($versions != NULL){
    $content = '<ul>';
    foreach ($versions  as $version){
      $content .= '<li class = "wallyextra_version_list" id = "wallyextra_version_'.$version->vid.'">'.ctools_ajax_text_button('Version : '.$version->version.' Date : '.format_date($version->date), 'wallyextra/customcontent/getversion/'.$version->vid.'/ajax', t('Choose a version')).'</li>';
    }
    $content .= '</ul>';
    $content .= '<div id = "wallyextra_version"></div>';
  } else {
    $content = t('There is no old version of this content type');
  }
  return $content;
}
