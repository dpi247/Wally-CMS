<?php


/**
 * Theme function for 'default' text field formatter.
 */
function theme_office_hours_formatter_default($element) {   
  
  $items = array();
  $first = variable_get('date_first_day',0); 
  $field = content_fields($element['#field_name'], $element['#type_name']);
  $output = '';
  $days = array(0 => t('Sunday'), 1 => t('Monday'), 2 => t('Tuesday'),3 => t('Wednesday'), 4 => t('Thursday'), 5 => t('Friday'), 6 => t('Saturday') );
  foreach (element_children($element) as $key => $arraykey) {
    $item = $element[$arraykey]['#item'];
    $day = $days[check_plain($element[$arraykey]['#item']['day'])];
    if (isset($day)) { 
      $endhrs = _office_hours_mil_to_tf(check_plain($item['endhours']));
      $strhrs = _office_hours_mil_to_tf(check_plain($item['starthours']));  
      if ($field['hoursformat']) {
        $endhrs = _office_hours_convert_to_ampm($endhrs);
        $strhrs = _office_hours_convert_to_ampm($strhrs);
      }
      $items[$day][] = array('strhrs' => $strhrs,'endhrs' => $endhrs) ;//we're aggregating hours for days together. 
    }   
  }
  
  $keys = array_keys($items);
  if ($keys[0] != $days[$first] && in_array(t($days[$first]), $items) ) {
   while ($keys[0] != $days[$first] && count($keys)>1):   
    $shifted = array_shift($items);
    $items[$keys[0]] = $shifted;
    $keys = array_keys($items);
  endwhile;
  }
  
  foreach ($items as $day => $val) {
    $strhrs = $val[0]['strhrs'];
    $endhrs = $val[0]['endhrs'];
    $strhrs2 = $val[1]['strhrs'];
    $endhrs2 = $val[1]['endhrs'];
    $additional = (isset($val[1])) ? ' <span class="oh-display-hours">, '.$strhrs2.' - '.$endhrs2.'</span>' : '';
    $output .= '<div class="oh-display">'.$day.': <span class="oh-display-hours">'.$strhrs.' - '.$endhrs.'</span>'.$additional.'</div>';  
  }
  return $output;     
}

function theme_office_hours($element) { 
//dpm($element);
  return $element['#children'];
}

/**
 * Theme function for the office hours selector element.
 */
function theme_office_hours_select($element) {
  return theme('form_element', $element, $element['#children']);
}

/**
 * copied from content module's theme_content_multiple_values- we're taking out the draggable feature.
 *
 * Theme the hour table form
 *
 */
function theme_office_hours_multiple_values($element) {
  $field_name = $element['#field_name'];
  $field = content_fields($field_name);
  $output = '';

  if ($field['multiple'] >= 1) {
    $table_id = $element['#field_name'] .'_values';
    $order_class = $element['#field_name'] .'-delta-order';
    $required = !empty($element['#required']) ? '<span class="form-required" title="'. t('This field is required.') .'">*</span>' : '';

    $header = array(
      array(
        'data' => t('!title: !required', array('!title' => $element['#title'], '!required' => $required)),
        'colspan' => 1
      ),
    );
    $rows = array();

    // Sort items according to '_weight' (needed when the form comes back after
    // preview or failed validation)
    $items = array();
    foreach (element_children($element) as $key) {
      if ($key !== $element['#field_name'] .'_add_more') {
        $items[] = &$element[$key];
      }
    }
    
  // dpm($items);
    $items = _office_hours_arrange_day($items); //this calls the function that arranges the first day of the week.

    // Add the items as table rows.
    foreach ($items as $key => $item) {
      $delta_element = drupal_render($item['_weight']);
      if (!($key % 2)) { //this is an even row, start a new row array_keys
        $cells = array (
          drupal_render($item),
        );
      }
      else { //this is an odd row
        $cells[] = drupal_render($item); //we've add the second cell;
        $rows[] = array(
        'data' => $cells,
        );
      }
    }

    $output .= theme('table', $header, $rows, array('id' => $table_id, 'class' => 'office_hours_table content-multiple-table'));
    $output .= $element['#description'] ? '<div class="description">'. $element['#description'] .'</div>' : '';
  //  $output .= drupal_render($element[$element['#field_name'] .'_add_more']);

    //drupal_add_tabledrag($table_id, 'order', 'sibling', $order_class);
  }
  else {
    foreach (element_children($element) as $key) {
      $output .= drupal_render($element[$key]);
    }
  }

  return $output;
}
