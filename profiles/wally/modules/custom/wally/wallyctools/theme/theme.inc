<?php

/**
 * Theme preprocess function for views-view-json.tpl.php
 *
 * @param array $vars
 */
function template_preprocess_wallyctools_style_redacblock(&$vars) {
  $view     = &$vars['view'];
  $options  = $view->style_handler->options;
  $handler  = $view->style_handler;
  $vars['destination']=$view->display_handler->handlers['argument']['field_destinations_tid']->value[0]  ;
  $vars['target']=$view->display_handler->handlers['filter']['field_destinations_target']->value[0];
  $vars['template_files'][]= 'wallyctools_style_redacblock';
  $vars['template_files'][]= 'wallyctools_style_redacblock'.'_'.$vars['destination'];
  $vars['template_files'][]= 'wallyctools_style_redacblock'.'_'.$vars['destination'].'_'.$vars['target'];

  if($view->split_index_count){
    $vars['template_files'][]= 'wallyctools_style_redacblock_split';
    $vars['template_files'][]= 'wallyctools_style_redacblock'.'_'.$vars['destination'].'_split';
    $vars['template_files'][]= 'wallyctools_style_redacblock'.'_'.$vars['destination'].'_'.$vars['target'].'_split';
    if($view->split_index_count==$view->split_total_count){
      $vars['template_files'][]= 'wallyctools_style_redacblock_split-last';
      $vars['template_files'][]= 'wallyctools_style_redacblock'.'_'.$vars['destination'].'_split-last';
      $vars['template_files'][]= 'wallyctools_style_redacblock'.'_'.$vars['destination'].'_'.$vars['target'].'_last';
    }
    if($view->split_index_count=="1"){
      $vars['template_files'][]= 'wallyctools_style_redacblock_split-first';
      $vars['template_files'][]= 'wallyctools_style_redacblock'.'_'.$vars['destination'].'_split-first';
      $vars['template_files'][]= 'wallyctools_style_redacblock'.'_'.$vars['destination'].'_'.$vars['target'].'_split-first';
    }
  }
}

/**
 * Theme preprocess function for views-view-row-unformatted.tpl.php
 */
function template_preprocess_wallyctools_row_redacblock(&$vars) {
  $view = &$vars['view'];
  $options = $vars['options'];

  // Make sure the variables are defined.
  $vars['node'] = '';
  $vars['comments'] = '';

  $nid = $vars['row']->{$vars['field_alias']};

  if($view->wallyedit_preview) {
    if(empty($vars['row']->done)) {
      if (!is_numeric($nid)) {
        return;
      }
      $node = node_load($nid);
    } else {
      $node = $vars['row'];
    }
  } else {
    if (!is_numeric($nid)) {
      return;
    }
    $node = node_load($nid);
  }

  $vars['destination'] = $view->display_handler->handlers['argument']['field_destinations_tid']->value[0];
  $vars['target'] = $view->display_handler->handlers['filter']['field_destinations_target']->value;
  $vars['target'] = $vars['target'][0];
  $vars['layout'] = wallyctools_api_getlayoutfromdestinationandtarget($node, $vars['destination'], $vars['target']);

  if (empty($node)) {
    return;
  }

  $node->view = $vars['view'];
  $node->node_view_layout=str_replace('_','-',$vars['layout']);
  $node->node_view_target=$vars['target'];
  $node->node_view_destination=$vars['destination'];

  if (isset($options['build_mode'])) {
    if ($options['build_mode'] == 'teaser') {
      $vars['teaser'] = TRUE;
    } else {
      $vars['teaser'] = FALSE;
    }
  } else {
    $vars['teaser'] = FALSE;
  }

  if (!isset($options['links'])) {
    $vars['links'] = TRUE;
  } else {
    $vars['links'] = $options['links'];
  }

  $vars['node'] = $node;
  
  if (!empty($options['comments']) && function_exists('comment_render')) {
    $vars['comments'] = comment_render($node);
  }
}

function theme_wallyctools_form_reorder_destination_form($form) {
  if ((!isset($form['rows']) || empty($form['rows'])) && !isset($form['saved_rows'])) {
    if (isset($form['submit'])) {
      drupal_set_message(t('No nodes to reorder.'));
    }
    return;
  } elseif (isset($form['saved_rows'])) {
    drupal_render($form['submit']);
    return drupal_render($form);
  }

  $node_types = node_get_types('names');

  $i = 1;

  //loop through each "row" in the table array
  foreach($form['rows'] as $id => $row){
    //we are only interested in numeric keys
    if (intval($id)){
      $fake_weight = array(
        '#type' => 'markup',
        '#value' => $i++,
        '#prefix' => '<div class="weight-id">',
        '#suffix' => '</div>',
      );

      $this_row = array(
        drupal_render($fake_weight),
        $row['data']['#value'],
        $node_types[$row['type']['#value']],
        date('D, d/m/Y - H:i', $row['publication']['#value']),
        date('D, d/m/Y - H:i', $row['last_update']['#value']),
        l(t('Edit'), 'node/'.$id.'/edit').' · '.l(t('Delete node'), 'node/'.$id.'/delete'),
      );

      //Add the weight field to the row
      $this_row[] = drupal_render($form['rows'][$id]['weight']);

      //Add the row to the array of rows
      $table_rows[] = array('data'=>$this_row, 'class'=>'draggable');
    }
  }

  //Make sure the header count matches the column count
  $header = array(
    '', t('Title'), t('Type'), t('Publication Date'), t('Last update'), t('Operations'), t('Order'),
  );

  // Call add_tabledrag to add and setup the JS for us
  // The key thing here is the first param - the table ID
  // and the 4th param, the class of the form item which holds the weight
  drupal_add_tabledrag('example-table', 'order', 'self', 'weight');

  drupal_add_js(drupal_get_path('module', 'wallyctools').'/scripts/update_reorder_ids.js');

  $output = theme('table', $header, $table_rows, array('id' => 'example-table'));
  //$output .= theme('pager', NULL, $form['pager_limit']['#value'], 0);
  $output .= drupal_render($form);

  return $output;
}

function theme_wallyctools_form_default_target_form($form) {
  if (!isset($form['rows']) || empty($form['rows'])) {
    drupal_set_message(t('No variant defined'));
    return;
  }

  $rows = array();
  foreach(element_children($form['rows']) as $row_id){
    $rows[] = array(
      drupal_render($form['rows'][$row_id]['name']),
      drupal_render($form['rows'][$row_id]['default_redacblock']),
    );
  }

  //Make sure the header count matches the column count
  $header = array(
    t('Variant Name'), t('Default Redacblock'),
  );

  $output = theme('table', $header, $rows);
  $output .= drupal_render($form);

  return $output;
}
