<?php

/**
 * @file
 *
 * Plugin to provide a subdomain context
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Subdomain'),
  'description' => t('The page subdomain.'),
  'context' => 'cckdestinations_subdomain_context_create',
  'settings form' => 'cckdestinations_subdomain_context_settings_form',
  'settings form validate' => 'cckdestinations_subdomain_context_settings_form_validate',
  'settings form submit' => 'cckdestinations_subdomain_context_settings_form_submit',
  'js' => array('misc/autocomplete.js'),
);

/**
 * Settings form for the 'by node_type' access plugin
 */
function cckdestinations_subdomain_context_settings($conf) {
  $form = array();
  return $form;
}

/**
 * It's important to remember that $conf is optional here, because contexts
 * are not always created from the UI.
 */
function cckdestinations_subdomain_context_create($empty, $data = NULL, $conf = FALSE) {
  $context = new ctools_context('subdomain');
  $context->plugin = 'subdomain';
  $domain = $_SERVER['HTTP_HOST'];
  $expl_domain = explode('.', $domain);
  $context->subdomain = $expl_domain[0];
  return $context;
}
