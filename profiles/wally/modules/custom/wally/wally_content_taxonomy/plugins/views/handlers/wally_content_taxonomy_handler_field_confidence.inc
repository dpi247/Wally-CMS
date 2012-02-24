<?php 

class wally_content_taxonomy_handler_field_confidence extends views_handler_field_prerender_list {


function init(&$view, $options) {
  parent::init($view, $options);
    if ($view->base_table == 'node_revisions') {
      $this->additional_fields['vid'] = array('table' => 'node_revisions', 'field' => 'vid');
    }
    else {
      $this->additional_fields['vid'] = array('table' => 'node', 'field' => 'vid');
    }
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['link_to_taxonomy'] = array('default' => TRUE);
    $options['limit'] = array('default' => FALSE);
    $options['vids'] = array('default' => array());

    return $options;
  }

  /**
   * Provide "link to term" option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_taxonomy'] = array(
      '#title' => t('Link this field to its term page'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_taxonomy']),
    );

    $form['term_to_show'] = array(
      '#title' => t('Choose the term to show'),
      '#type' => 'textfield',
      '#default_value' => $this->options['term_to_show'],
      '#autocomplete_path' => 'wally_content_taxonomy/term_list/autocomplete',
      '#required' => TRUE,
    );
  }

  /**
   * Add this term to the query
   */
  function query() {
    $this->add_additional_fields();
  }

  function pre_render($values) {
    $term_to_show = explode('[tid]', $this->options['term_to_show']);
    $tid = $term_to_show[1];
    
    $this->field_alias = $this->aliases['vid'];
    $vids = array();
    foreach ($values as $result) {
      if (!empty($result->{$this->aliases['vid']})) {
        $vids[] = $result->{$this->aliases['vid']};
      }
    }

    if ($vids) {
      $voc = '';
      $voc_ids = array_filter($this->options['vids']);
      if (!empty($this->options['limit']) && !empty($voc_ids)) {
        $voc = " AND td.vid IN (" . implode(', ', array_keys($voc_ids)) . ")";
      }

      $result = db_query("SELECT tn.vid AS node_vid, tn.wally_confidence_score, td.*, v.name as vocabulary 
      					  FROM {term_data} td 
      					  INNER JOIN {term_node} tn ON td.tid = tn.tid 
      					  INNER JOIN {vocabulary} v ON v.vid = td.vid 
      					  WHERE td.tid = %d 
      					  ORDER BY td.weight, td.name", $tid);

      while ($term = db_fetch_object($result)) {
        $this->items[$term->node_vid][$term->tid]['name'] = check_plain($term->name);
        $this->items[$term->node_vid][$term->tid]['tid'] = $term->tid;
        $this->items[$term->node_vid][$term->tid]['vid'] = $term->vid;
        $this->items[$term->node_vid][$term->tid]['vocabulary'] = check_plain($term->vocabulary);
        $this->items[$term->node_vid][$term->tid]['wally_confidence_score'] = $term->wally_confidence_score;
      
        if (!empty($this->options['link_to_taxonomy'])) {
          $this->items[$term->node_vid][$term->tid]['make_link'] = TRUE;
          $this->items[$term->node_vid][$term->tid]['path'] = taxonomy_term_path($term);
        }
      }
    }
  }

  function render_item($count, $item) {
    
    $content = $item['wally_confidence_score'];
    return $content;
  }

  function document_self_tokens(&$tokens) {
    $tokens['[' . $this->options['id'] . '-tid' . ']'] = t('The taxonomy term ID for the term.');
    $tokens['[' . $this->options['id'] . '-name' . ']'] = t('The taxonomy term name for the term.');
    $tokens['[' . $this->options['id'] . '-vid' . ']'] = t('The vocabulary ID for the vocabulary the term belongs to.');
    $tokens['[' . $this->options['id'] . '-vocabulary' . ']'] = t('The name for the vocabulary the term belongs to.');
  }

  function add_self_tokens(&$tokens, $item) {
    $tokens['[' . $this->options['id'] . '-tid' . ']'] = $item['tid'];
    $tokens['[' . $this->options['id'] . '-name' . ']'] = $item['name'];
    $tokens['[' . $this->options['id'] . '-vid' . ']'] = $item['vid'];
    $tokens['[' . $this->options['id'] . '-vocabulary' . ']'] = $item['vocabulary'];
  }
}