<?php

/**
 * @file
 * Plugin to provide access control based upon field_freeaccess value.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Free Access"),
  'description' => t('Control access by the value of the field_freeaccess.'),
  'callback' => 'wallycontenttypes_freeaccess_ctools_access_check',
  'default' => array(),
  'summary' => 'wallycontenttypes_freeaccess_ctools_access_summary',
  'required context' => new ctools_context_required(t('Node'), 'node'),
);

/**
 * Check for access.
 */
function wallycontenttypes_freeaccess_ctools_access_check($conf, $context) {
  dsm($context, 'context');
  
  return FALSE;
}

/**
 * Provide a summary description based upon the chosen access type.
 */
function wallycontenttypes_freeaccess_ctools_access_summary($conf, $context) {
  return t('Node is checked as free.');
}
