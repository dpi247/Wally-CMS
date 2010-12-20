<?php

GLOBAL $base_url; 

/**
 * Override or insert PHPTemplate variables into the templates.
 */
function wallynews_preprocess_page(&$vars) {
  $vars['sf_primarymenu'] = theme("wallyct_mainmenu", 'primary-links', 'menu-primary-links');
  $vars['sf_secondarymenu'] = theme("wallyct_mainmenu", 'secondary-links', 'menu-secondary-links');
  $vars['scripts'] = drupal_get_js();
  $vars['styles'] = drupal_get_css();
}

/**
 * Generates IE CSS links for LTR and RTL languages.
 */
function wallynews_get_ie_styles() {
}

