<?php

/**
 * @file
 * Contains filter handlers for protocol filters with views.
 */

/**
 * Filter handler for limiting a view to URLs of a certain protocol.
 */
class cckdestinations_views_handler_filter_tid extends views_handler_filter_term_node_tid {
  function construct() {
    parent::construct();
    $this->content_field = content_fields($this->definition['content_field_name']);
    $field = $this->content_field;
    $this->value_title = $field['widget']['label'];
  }
  
  function value_form(&$form, &$form_state) {
    if ($this->options['type'] == 'textfield') {
      $vocabulary = taxonomy_vocabulary_load($this->options['vid']);
      if (empty($vocabulary) && $this->options['limit']) {
        $form['markup'] = array(
          '#prefix' => '<div class="form-item">',
          '#suffix' => '</div>',
          '#value' => t('An invalid vocabulary is selected. Please change it in the options.'),
        );
        return;
      }
      
      $default = '';
      if ($this->value) {
        $terms = array();
        foreach ($this->value as $val_tid) {
          $terms[] = taxonomy_get_term($val_tid);
        }
        $default = cckdestinations_autocomplete_merge_tags($terms, $vocabulary->vid);
      }
      
      $form['value'] = array(
        '#title' => $this->options['limit'] ? t('Select terms from vocabulary @voc', array('@voc' => $vocabulary->name)) : t('Select terms'),
        '#type' => 'textfield',
        '#default_value' => $default,
      );
      
      if ($this->options['limit']) {
        $form['value']['#autocomplete_path'] = 'cckdestinations/autocomplete/' . $vocabulary->vid . '/multiple';
      }
    } else {
      parent::value_form($form, $form_state);
    }
  }
  
  function value_validate(&$form, &$form_state) {
    // We only validate if they've chosen the text field style.
    if ($this->options['type'] != 'textfield') {
      return;
    }

    $vocabulary = taxonomy_vocabulary_load($this->options['vid']);
    $extracted_ids = cckdestinations_autocomplete_tags_get_tids($form_state['values']['options']['value'], $vocabulary->vid, 'multiple');
    
    if ($extracted_ids) {
      $form_state['values']['options']['value'] = $extracted_ids['existing_tids'];
    }
  }
  
  function exposed_validate(&$form, &$form_state) {
    if (empty($this->options['exposed'])) {
      return;
    }
  
    $identifier = $this->options['expose']['identifier'];
  
    // We only validate if they've chosen the text field style.
    if ($this->options['type'] != 'textfield') {
      if ($form_state['values'][$identifier] != 'All')  {
        $this->validated_exposed_input = (array) $form_state['values'][$identifier];
      }
      return;
    }
  
    if (empty($this->options['expose']['identifier'])) {
      return;
    }
    $tids = cckdestinations_autocomplete_tags_get_tids($form_state['values'][$identifier], 2, 'multiple');
  
    if ($tids) {
      $this->validated_exposed_input = $tids['existing_tids'];
    }
  }
}
