<?php
zef
  drupal_add_css(drupal_get_path('module', 'wallycontenttypes') . '/css/superfish/superfish.css', 'theme', 'header', FALSE, TRUE, FALSE);
    drupal_add_js(drupal_get_path('module', 'wallycontenttypes') . '/scripts/superfish/hoverIntent.js', 'theme', 'header', FALSE, TRUE, FALSE);
if (!function_exists('wallyct_mainmenu_tree_output')) {
/**
 * Returns a rendered menu tree.
 *
 * @param $tree
 *   A data structure representing the tree as returned from menu_tree_data.
 *
 * @return string
 *   The complete, rendered administration menu.
 */
function wallyct_mainmenu_tree_output($tree, $menuid="menu-primary-links", $firstpass=1) {

    drupal_add_css(drupal_get_path('module', 'wallycontenttypes') . '/css/superfish/superfish.css', 'theme');
    drupal_add_js(drupal_get_path('module', 'wallycontenttypes') . '/scripts/superfish/hoverIntent.js', 'theme');
    drupal_add_js(drupal_get_path('module', 'wallycontenttypes') . '/scripts/superfish/superfish.js','theme');

    drupal_add_js("
        $(document).ready(function() {
          jQuery('ul.sf-menu').superfish();
        });
    ", 'inline');

         
    $output = '';

    foreach ($tree as $data) {
      // if extra class set, add to the menu item.
      $extra_class = isset($data['link']['localized_options']['extra class']) ? $data['link']['localized_options']['extra class'] : NULL;
      // Get correct link for item.
      $link = wallyct_mainmenu_menu_item_link($data['link']);
      // is there any sub-menu? 
      if ($data['below']) {
        $sub_menu = wallyct_mainmenu_tree_output($data['below'], "", $firstpass+1);
        $output .= theme_wallyct_mainmenu_item($link, $data['link']['has_children'], $sub_menu, $data['link']['in_active_trail'], $extra_class);
      }
      else {
        $output .= theme_wallyct_mainmenu_item($link, $data['link']['has_children'], '', $data['link']['in_active_trail'], $extra_class);
      }
    }
    
    if ($firstpass==1) {
      return $output ? "\n<ul class='sf-menu' id='".$menuid."'>". $output ."</ul>\n" : "";
    } else {
      return $output ? "\n<ul>". $output ."</ul>\n" : "";
    }
  }
}


if (!function_exists('wallyct_mainmenu_menu_item_link')) {
/**
 * High-performance implementation of theme_menu_item_link().
 *
 * This saves us a theme() call and does only the absolute minimum to get
 * the menu "links" rendered.
 */
function wallyct_mainmenu_menu_item_link($link) {
    // Omit alias lookups.
    $link['localized_options']['alias'] = TRUE;
    return '<a href="'. check_url(url($link['href'], $link['localized_options'])) .'">'. (!empty($link['localized_options']['html']) ? $link['title'] : check_plain($link['title'])) .'</a>';
  }
}


if (!function_exists('theme_wallyct_mainmenu_item')) {
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
function theme_wallyct_mainmenu_item($link, $has_children, $menu = '', $in_active_trail = FALSE, $extra_class = NULL) {
 //   $class = ($menu || $has_children ? 'expandable' : '');
 //   if (!empty($extra_class)) {
 //     $class .= ' '. $extra_class;
 //   }
 //   if ($in_active_trail) {
 //     $class .= ' active-trail';
 //   }
    return '<li'. (!empty($class) ? ' class="'. $class .'"' : '') .'>'. $link . $menu .'</li>';
  }
}

$main_menu = wallyct_mainmenu_tree_output(menu_tree_all_data($menu), $menuid, 1);
?>
<?php print $main_menu; ?>
