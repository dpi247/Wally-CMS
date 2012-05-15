<?php

if (!function_exists('wallydemo_menu_tree_output')) {
/**
 * Returns a rendered menu tree.
 *
 * @param $tree
 *   A data structure representing the tree as returned from menu_tree_data.
 *
 * @return string
 *   The complete, rendered administration menu.
 */
function wallydemo_menu_tree_output($tree, $menuid="menu-selectboxeditions", $firstpass=1) {
/*
 * Note pour PSp.
 * 
 * Cette fonction partours le menu de maniere récursive. Caf elle parcours un niveau et regarde si un niveau 
 * inférieur existe sous l'élément courant existe. Si c'est le cas, la fonction est ré-appellée par elle meme pour
 * constituer le niveau inférieur.
 * 
 */
         
    $output = '';

    /*
     * Boucle principale sur tous les items ($data) du menu.
     */
    foreach ($tree as $data) {
      
        // Je recherche le lien correct pour cet item (la fonction spmainmenus_mainmenu_menu_item_link constitue le
        // html de ce lien (<a>.......</a>))..
        $link = wallydemo_menu_item_link($data['link']);
      
      
        // Ici je regarde si il y a un sous-menu de niveau-1 disponible.
        // Si oui on ré-appelle la fonction spmainmenus_mainmenu_tree_output().  
        if ($data['below']) {
          $sub_menu = wallydemo_menu_tree_output($data['below'], "", $firstpass+1);
          $output .= wallydemo_menu_item_render($link, $data['link']['has_children'], $sub_menu, $data['link']['in_active_trail'], $extra_class=NULL);
        }
        else 
        // Si non, on place juste le iien. 
        {
          $output .= wallydemo_menu_item_render($link, $data['link']['has_children'], '', $data['link']['in_active_trail'], $extra_class=NULL);
        }
      //}
 
    }
    /*
     * Si on est en premiere "passe" dans la fonction, ajouter l'id du menu pour
     * que le jquery puisse agie sur le bon element.
     */   
    if ($firstpass==1) {
      return $output ? "\n<select onchange='location.href=document.getElementById(\"s_titre\").value' name=\"s_titre\" id=\"s_titre\">\n<option value=\"http://www.sudpresse.be\">Choisir une &eacute;dition</option>\n". $output ."</select>" : "";
    } else {
      return $output ? "\n". $output ."\n" : "";
    }
  }
}


if (!function_exists('wallydemo_menu_item_link')) {
/**
 * High-performance implementation of theme_menu_item_link().
 *
 * This saves us a theme() call and does only the absolute minimum to get
 * the menu "links" rendered.
 */
function wallydemo_menu_item_link($link) {
    // note pur PSP : dsm($link); pour avoir toutes les infos.
    if($link['has_children'] == 0){
      $class="edition";
    } else {
      $class="s_titre";
    }
    $lien = '<option class="'.$class.'" value="'.check_url($link['link_path']).'">'.wallydemo_check_plain($link['link_title']).'</option>'; 
    return $lien;
  }
}


if (!function_exists('wallydemo_menu_item_render')) {
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
function wallydemo_menu_item_render($link, $has_children, $menu = '', $in_active_trail = FALSE, $extra_class = NULL) {

  /*
   * note pour PSp: on constitue l item de menu a partir du lien, des extra-classe eventuelle.  
   */

    return $link.$menu; 

}

}
/*
 * Appel principal des fonction. CHargement le la variable $main_menu avec le HTML généré.
*/
$main_menu = wallydemo_menu_tree_output($menu, "selectboxeditions", 1);
?>
<li>    
  <div class="date">
    <?php  
    $unix = time();
    $date = _wallydemo_date_edition_diplay($unix, 'date_jour');
    print $date;
    print "&nbsp;";
    if (module_exists('spmeteo')) {
      print theme("spmeteo_get_saint");
    } ?>
  </div>     
<?php print $main_menu; ?>
</li>

<?php 
//ici unset variables
unset($menu);
unset($main_menu);
?>
