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
  'title' => t('Topic Term of a Term'),
  'keyword' => 'topicterm',
  'description' => t('Adds the related topic terms from a term context.'),
  'required context' => new ctools_context_required(t('Term'), 'term'),
  'context' => 'cckdestinations_topictermfromterm_context',
  'settings form' => 'cckdestinations_topictermfromterm_settings_form',
  'settings form validate' => 'cckdestinations_	_form_validate',
);

/**
 * Return a new context based on an existing context.
 */
function cckdestinations_topictermfromterm_context($context, $conf) {
  // If unset it wants a generic, unfilled context, which is just NULL.
  if (empty($context->data)) {
    return ctools_context_create_empty('term', NULL);
  }

  if (isset($context->data->topic_term)) {
    $term=taxonomy_get_term($context->data->topic_term);
      return ctools_context_create('term', $term);
    
  }
  else{
        return ctools_context_create_empty('term', NULL);
    
  }
}

/**
 * Settings form for the relationship.
 */
function cckdestinations_topictermfromterm_settings_form($conf) {
  $options = array();
  $form=array();
  /*
  foreach (taxonomy_get_vocabularies() as $vid => $vocabulary) {
    $options[$vid] = $vocabulary->name;
  }
  $form['vid'] = array(	
    '#title' => t('Vocabulary'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => $conf['vid'],
    '#prefix' => '<div class="clear-block">',
    '#suffix' => '</div>',
  );
  */
  return $form;
}
