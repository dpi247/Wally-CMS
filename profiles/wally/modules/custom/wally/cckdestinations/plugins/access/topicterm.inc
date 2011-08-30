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
  'title' => t("Topic Term"),
  'description' => t('Control access by the existence of a related topic Term.'),
  'callback' => 'cckdestinations_topicterm_ctools_access_check',
  'default' => array('type' => array()),
  'settings form' => 'cckdestinations_topicterm_ctools_access_settings',
  'settings form submit' => 'cckdestinations_topicterm_ctools_access_settings_submit',
  'summary' => 'cckdestinations_topicterm_ctools_access_summary',
  'required context' => new ctools_context_required(t('Term'), 'term'),
  //TODO: what restriction is for ?
  //'restrictions' => 'cckdestinations_topicterm_ctools_access_restrictions',
);

/**
 * Settings form for the 'by node_type' access plugin
 */
function cckdestinations_topicterm_ctools_access_settings(&$form, &$form_state, $conf) {

}

/**
 * Compress the topicterms allowed to the minimum.
 */
function cckdestinations_topicterm_ctools_access_settings_submit(&$form, &$form_state) {

}

/**
 * Check for access.
 */
function cckdestinations_topicterm_ctools_access_check($conf, $context) {
  // As far as I know there should always be a context at this point, but this
  // is safe.
  return $context->data->topic_destination>0;
}

//TODO: what restriction is for ?
/*function cckdestinations_topicterm_ctools_access_restrictions($conf, &$context) {
  if (isset($context->restrictions['type'])) {
    $context->restrictions['type'] = array_unique(array_merge($context->restrictions['type'], array_keys(array_filter($conf['type']))));
  }
  else {
    $context->restrictions['type'] = array_keys(array_filter($conf['type']));
  }
}
*/

/**
 * Provide a summary description based upon the checked topicterms.
 */
function cckdestinations_topicterm_ctools_access_summary($conf, $context) {
  return t('"@term" has an associated topic term', array('@term' => $context->identifier,));
  
  if (empty($names)) {
    return t('@identifier is any node type', array('@identifier' => $context->identifier));
  }

  return format_plural(count($names), '@identifier is type "@types"', '@identifier type is one of "@types"', array('@types' => implode(', ', $names), '@identifier' => $context->identifier));
}

