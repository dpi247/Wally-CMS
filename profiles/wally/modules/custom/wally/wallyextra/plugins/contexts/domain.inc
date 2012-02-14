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
  'keyword' => 'domain',
  'description' => t('The page domain.'),
  'context' => 'wallyextra_domain_context_create',
  'settings form' => 'wallyextra_domain_context_settings_form',
  'js' => array('misc/autocomplete.js'),
);

/**
 * Settings form for the 'by node_type' access plugin
 */
function wallyextra_domain_context_settings($conf) {
  $form = array();
  return $form;
}

/**
 * It's important to remember that $conf is optional here, because contexts
 * are not always created from the UI.
 */
function wallyextra_domain_context_create($empty, $data = NULL, $conf = FALSE) {
  $context = new ctools_context('domain');
  $context->plugin = 'domain';
  $context->domain = $_SERVER['HTTP_HOST'];
  return $context;
}
