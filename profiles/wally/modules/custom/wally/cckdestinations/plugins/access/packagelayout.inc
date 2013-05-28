<?php
// $Id: topicterm.inc,v 1.7.2.2 2009/11/13 00:44:16 merlinofchaos Exp $

/**
 * @file
 * Plugin to provide access control based upon node type.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Package Layout'),
  'description' => t('Control access by the match of a package layout.'),
  'callback' => 'cckdestinations_packagelayout_ctools_access_check',
  'default' => array('packagelayout' => array()),
  'settings form' => 'cckdestinations_packagelayout_ctools_access_settings',
  'settings form submit' => 'cckdestinations_packagelayout_ctools_access_settings_submit',
  'summary' => 'cckdestinations_packagelayout_ctools_access_summary',
  'required context' => new ctools_context_required(t('Node'), 'node'),
  //TODO: what restriction is for ?
  //'restrictions' => 'cckdestinations_topicterm_ctools_access_restrictions',
);

/**
 * Settings form for the 'by node_type' access plugin
 */
function cckdestinations_packagelayout_ctools_access_settings(&$form, &$form_state, $conf) {
  $packagelayouts_vid = variable_get('wallymport_documenttype', 0);
  $defined_layouts = taxonomy_get_tree($packagelayouts_vid);

  $options = array();
  foreach ($defined_layouts as $layout) {
    $options[$layout->tid] = $layout->name;
  }

  $form['settings']['packagelayout'] = array(
    '#type' => 'select',
    '#title' => t('Package Layout'),
    '#options' => $options,
    '#default_value' => $conf['packagelayout'],
    '#required' => TRUE,
  );
}

/**
 * Compress the topicterms allowed to the minimum.
 */
function cckdestinations_packagelayout_ctools_access_settings_submit(&$form, &$form_state) {
  
}

/**
 * Check for access.
 */
function cckdestinations_packagelayout_ctools_access_check($conf, $context) {
  $node = $context->data;
  if ($node->field_packagelayout[0]['value'] == $conf['packagelayout']) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Provide a summary description based upon the checked topicterms.
 */
function cckdestinations_packagelayout_ctools_access_summary($conf, $context) {
  $packagelayouts_vid = variable_get('wallymport_documenttype', 0);
  $defined_layouts = taxonomy_get_tree($packagelayouts_vid);

  $options = array();
  foreach ($defined_layouts as $layout) {
    $options[$layout->tid] = $layout->name;
  }

  return t('Package layout of "@node" is "@packagelayout"', array('@node' => $context->identifier, '@packagelayout' => $options[$conf['packagelayout']]));
}

