<h1>FILTRES</h1>
<?php

// $items, $display_limit = 0) {
  // theme('item_list') expects a numerically indexed array.
  $items = array_values($items);
  // If there is a limit and the facet count is over the limit, hide the rest.
  if (($display_limit > 0) && (count($items) > $display_limit)) {
    // Show/hide extra facets.
    drupal_add_js(drupal_get_path('module', 'apachesolr') . '/apachesolr.js');
    // Split items array into displayed and hidden.
    $hidden_items = array_splice($items, $display_limit);
    foreach ($hidden_items as $hidden_item) {
      if (!is_array($hidden_item)) {
        $hidden_item = array('data' => $hidden_item);
      }
      $hidden_item['class'] = isset($hidden_item['class']) ? $hidden_item['class'] . ' apachesolr-hidden-facet' : 'apachesolr-hidden-facet';
      $items[] = $hidden_item;
    }
  }
  $admin_link = '';
  if (user_access('administer search')) {
    $admin_link = l(t('Configure enabled filters'), 'admin/settings/apachesolr/enabled-filters');
  }
  
  dsm($items); 
