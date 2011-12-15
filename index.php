<?php

/**
 * @file
 * The PHP page that serves all page requests on a Drupal installation.
 *
 * The routines here dispatch control to the appropriate handler, which then
 * prints the appropriate page.
 *
 * All Drupal code is released under the GNU General Public License.
 * See COPYRIGHT.txt and LICENSE.txt.
 */

require_once './includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

if(arg(0)=='taxonomy' and arg(1)=='term'){
  $menu_item=menu_get_item();
  if($menu_item['page_callback']!='page_manager_term_view'){
  variable_set('site_offline',TRUE);
  }
}
$return = menu_execute_active_handler();

// Menu status constants are integers; page content is a string.
if (is_int($return)) {
  switch ($return) {
    case MENU_NOT_FOUND:
      header("Location: /404.html"); 
      //drupal_not_found();
     break;
    case MENU_ACCESS_DENIED:
      drupal_access_denied();
      break;
    case MENU_SITE_OFFLINE:
      drupal_site_offline();
      break;
  }
}
elseif (isset($return)) {
  // Print any value (including an empty string) except NULL or undefined:
  print theme('page', $return);
}

drupal_page_footer();
