<?php

/**
 * @file
 * Argument handler to filter results by target.
 */

/**
 * Argument handler to filter results by target.
 */
class cckdestinations_views_handler_argument_tid extends views_handler_argument_term_node_tid {
  function options_form(&$form, &$form_state) {
      parent::options_form($form, $form_state);
  
    
    $form['not'] = array(
        '#type' => 'checkbox',
        '#title' => t('Exclude the argument'),
        '#description' => t('If selected, the numbers entered in the argument will be excluded rather than limiting the view.'),
        '#default_value' => !empty($this->options['not']),
      );
  }
}
