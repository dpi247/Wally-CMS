<?php
// $Id: terms_from_node.inc,v 1.1.2.1 2010/02/23 22:09:47 merlinofchaos Exp $

/**
 * @file
 * Plugin to provide an relationship handler for all terms from node.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Main Destination'),
  'keyword' => 'term',
  'description' => t('Adds a taxonomy terms from a node context; if multiple terms are selected, they wil be concatenated.'),
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'context' => 'cckdestinations_termfromdestination_context',
  'settings form' => 'cckdestinations_termfromdestination_settings_form',
  'settings form validate' => 'cckdestinations_termfromdestination_form_validate',
  'defaults' => array('vid' => array(), 'concatenator' => ','),
);

/**
 * Return a new context based on an existing context.
 */
function cckdestinations_termfromdestination_context($context, $conf) {
  // If unset it wants a generic, unfilled context, which is just NULL.
  if (empty($context->data)) {
    return ctools_context_create_empty('term', NULL);
  }

  if (isset($context->data->field_destinations)) {
    if (isset($context->data->field_destinations[0])) {
      $term=taxonomy_get_term($context->data->field_destinations[0]['tid']);
      return ctools_context_create('term', $term);
    }
  }
  else{
    return ctools_context_create_empty('term', NULL);
  }
}

/**
 * Settings form for the relationship.
 */
function cckdestinations_termfromdestination_settings_form($conf) {
  $form = array();

  return $form;
}
