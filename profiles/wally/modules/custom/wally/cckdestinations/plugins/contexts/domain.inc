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
  'title' => t('Domain'),
  'description' => t('The page domain.'),
  'context' => 'cckdestinations_domain_context_create',
  'settings form' => 'cckdestinations_domain_context_settings_form',
  'settings form validate' => 'cckdestinations_domain_context_settings_form_validate',
  'settings form submit' => 'cckdestinations_domain_context_settings_form_submit',
  'js' => array('misc/autocomplete.js'),
);

/**
 * Settings form for the 'by node_type' access plugin
 */
function cckdestinations_domain_context_settings($conf) {
  $form = array();
  return $form;
}

/**
 * It's important to remember that $conf is optional here, because contexts
 * are not always created from the UI.
 */
function cckdestinations_domain_context_create($empty, $data = NULL, $conf = FALSE) {
  $context = new ctools_context('domain');
  $context->plugin = 'domain';
  $context = $_SERVER['HTTP_HOST'];
  return $context;
}
