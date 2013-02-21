<?php

function theme_wallysitemap_checkboxes($element) {
  return theme('checkboxes', $element);
}

/**
 * theme function for root element
 *
 * @param $element
 * @return html output
 */
function theme_wallysitemap_taxonomy_tree($element) {
  $tree = theme('wallysitemap_taxonomy_tree_elements', $element['#elements']);
  if ((!$element['#parent'] && !$element['#siblings_page']) || $element['#render_whole_tree']) {
    $output = '<div id="'. $element['#id'] .'">';
    $output .= $tree;
    $output .= '</div>';
    return theme('form_element', $element, $output);
  }

  return $tree;
}

/**
 * recursive theme function for term elements
 *
 * @param $element
 * @return html lists
 */
function theme_wallysitemap_taxonomy_tree_elements($element) {
  $output .= '<ul class="treeview">';

  if (is_array($element)) {
    foreach (element_children($element) as $tid) {
      if (is_numeric($tid)) {
        $output .= '<li'. drupal_attributes($element[$tid]['#attributes']) .'>';
        if (strpos($element[$tid]['#attributes']['class'], 'has-children') !== FALSE || is_array($element[$tid]['children'])) {
          $output .= '<div class="hitArea"></div>';
        }
        $output .='<div class="term-line">';
        $output .= drupal_render($element[$tid]['check']);
        $output .= drupal_render($element[$tid]['weight_select']);
        $output .= drupal_render($element[$tid]['freq']);

        if (is_array($element[$tid]['weight']) && is_array($element[$tid]['tid'])) {
          $output .= drupal_render($element[$tid]['weight']);
          $output .= drupal_render($element[$tid]['tid']);
        }

        $output .= '</div>';

        if (is_array($element[$tid]['children'])) {
          $output .= theme('wallysitemap_taxonomy_tree_elements', $element[$tid]['children']);
        }

        $output .='</li>';
      }
    }
  }

  $output .= "</ul>";
  if (isset($element['language'])) {
    $output .= drupal_render($element['language']);
  }
  return $output;
}

/**
 * Themes a select list
 */
function theme_wallysitemap_taxonomy_tree_select($element) {
  _form_set_class($element, array('form-select'));
  $select = '<select ';
  $select .= 'name="'. $element['#name'] .'" ';
  $select .= 'id="'. $element['#id'] .'" ' ;
  $select .= drupal_attributes($element['#attributes']) .' />';
  $select .= form_select_options($element);
  $select .= '</select>';

  $title = check_plain($element['#title']);

  if (!is_null($title)) {
    $select = '<label class="option">'. $title .' '. $select .'</label>';
  }

  unset($element['#title']);
  return theme('form_element', $element, $select);
}

/**
 * Themes a checkbox
 */
function theme_wallysitemap_taxonomy_tree_checkbox($element) {
  _form_set_class($element, array('form-checkbox'));
  $checkbox = '<input ';
  $checkbox .= 'type="checkbox" ';
  $checkbox .= 'name="'. $element['#name'] .'" ';
  $checkbox .= 'id="'. $element['#id'] .'" ' ;
  $checkbox .= 'value="'. $element['#return_value'] .'" ';
  $checkbox .= $element['#value'] ? ' checked="checked" ' : ' ';
  $checkbox .= drupal_attributes($element['#attributes']) .' />';

  unset($element['#title']);
  return theme('form_element', $element, $checkbox);
}
