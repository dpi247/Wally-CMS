<?php

if (!function_exists('soirmag_tree_output')) {
/**
 * Returns a rendered menu tree.
 *
 * @param $tree
 *   A data structure representing the tree as returned from menu_tree_data.
 *
 * @return string
 *   The complete, rendered administration menu.
 */
function soirmag_tree_output($tree, $firstpass=1) { 
    $output = '';
    
    foreach ($tree as $data) {
      // if extra class set, add to the menu item.
      $extra_class = isset($data['link']['localized_options']['extra class']) ? $data['link']['localized_options']['extra class'] : NULL;
      // Get correct link for item.
      $link = soirmag_item_link($data['link']);
      // is there any sub-menu? 
      if ($data['below']) {
        $sub_menu = "<div class='ss-menu'><div>".soirmag_tree_output($data['below'], $firstpass++)."</div></div>";
        $output .= theme_soirmag_item($link, $data['link']['has_children'], $sub_menu, $data['link']['in_active_trail'], $extra_class);
      }
      else {
        $output .= theme_soirmag_item($link, $data['link']['has_children'], '', $data['link']['in_active_trail'], $extra_class);
      }
    }
    
    // @ Fist pass we add a clearfix class to the list.
    if ($firstpass==1) {
      return $output ? "\n<ul class='clearfix'>". $output .'</ul>' : '';
    } else {
      return $output ? "\n<ul>". $output .'</ul>' : '';
    }
  }
}

if (!function_exists('soirmag_item_link')) {
/**
 * High-performance implementation of theme_menu_item_link().
 *
 * This saves us a theme() call and does only the absolute minimum to get
 * the admin menu links rendered.
 */
function soirmag_item_link($link) {
    // Omit alias lookups.
    $link['localized_options']['alias'] = TRUE;
    return '<a href="'. check_url(url($link['href'], $link['localized_options'])) .'">'. (!empty($link['localized_options']['html']) ? $link['title'] : check_plain($link['title'])) .'</a>';
  }
}


if (!function_exists('theme_soirmag_item')) {
/**
 * Generate the HTML output for a single menu item and submenu.
 *
 * @param string $link
 *   A rendered menu item link.
 * @param bool $has_children
 *   Whether this item has children.
 * @param string $menu
 *   A string containing any rendered children of this item.
 * @param bool $in_active_trail
 *   Whether this item is in the active menu trail.
 * @param string $extra_class
 *   An additional CSS class to set for this item.
 *
 * @see theme_menu_item()
 * @ingroup themeable
 */
function theme_soirmag_item($link, $has_children, $menu = '', $in_active_trail = FALSE, $extra_class = NULL) {
    $class = ($menu || $has_children ? 'expandable' : '');
    if (!empty($extra_class)) {
      $class .= ' '. $extra_class;
    }
    if ($in_active_trail) {
      $class .= ' active-trail';
    }
    return '<li'. (!empty($class) ? ' class="'. $class .'"' : '') .'>'. $link . $menu .'</li>';
  }
}


$main_menu = soirmag_tree_output(menu_tree_all_data('primary-links'));

?>
<?php print $main_menu; ?>
