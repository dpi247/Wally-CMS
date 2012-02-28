<?php
$external_js = 'http://voo.sudinfo.be/staticNew/js/seed.js';
drupal_add_js('document.write(unescape("%3Cscript src=\''. $external_js . '\' type=\'text/javascript\'%3E%3C/script%3E"));', 'inline');

if (!function_exists('spmainmenus_set_activetrail')) {
/**
 * Returns a rendered menu tree.
 *
 * @param $tree
 *   A data structure representing the tree as returned from menu_tree_data.
 *
 * @return string
 *   The complete, rendered administration menu.
 */
function spmainmenus_set_activetrail(&$tree, $firstpass=1, &$special_div,$tid=NULL) {

	
	//    dsm(debug_backtrace(),"BACKTRAVCRE");
	
	if($tid) $current_url = "taxonomy/term/".$tid;
	else $current_url = $_GET['q'];
	$found = FALSE; 
	foreach ($tree as $key=>$data) {
		// Est ce le current path?
		if ($data["link"]["link_path"]==$current_url) {
          // We got the URL
          $found = TRUE; 
          $data["link"]["in_active_trail"] = TRUE;
          				 
          if ($data["below"] && $firstpass==2) {
          	$data["link"]["menu_edition2"] = "menu_edition2";
          	
          	// $special_div est une div qui doit comprendre un niveau Deux virtuel
          	// avec l'element de niv2 courrant + lal iste des elements de niveau 3
          	$special_div[0][$key]["link"] = $data["link"];
          	$special_div[0][$key]["below"] = FALSE;
          	$special_div[0][$key]["link"]["menu_edition2"] = "level2-fst-special"; 
          	$special_div[0] = array_merge($special_div[0], $data["below"]);
          	
          } else {
          	$data["link"]["menu_edition2"] = NULL;
          }
          $tree[$key] = $data;
	      return(TRUE);
		} else {
			
			if ($data["below"]) {
				$r = spmainmenus_set_activetrail($data["below"], $firstpass+1, $special_div,$tid);
				if ($r) {
					$data["link"]["in_active_trail"] = TRUE;
					if ($data["below"] && $firstpass==2) {
				  	$data["link"]["menu_edition2"] = "menu_edition2";
				  	
				  	// $special_div est une div qui doit comprendre un niveau Deux virtuel
				  	// avec l'element de niv2 courrant + lal iste des elements de niveau 3
						$special_div[0][$key]["link"] = $data["link"];
						$special_div[0][$key]["below"] = FALSE;
						$special_div[0][$key]["link"]["menu_edition2"] = "level2-fst-special"; 
						$special_div[0] = array_merge($special_div[0], $data["below"]);
						
					} else {
				  	$data["link"]["menu_edition2"] = NULL;
					}
					$tree[$key] = $data;
			    return(TRUE);
				} else {
					$data["link"]["in_active_trail"] = FALSE;
			    $tree[$key] = $data;
				}					
			}
		}
	}
	return FALSE;
  }
}


if (!function_exists('spmainmenus_mainmenu_tree_output')) {
/**
 * Returns a rendered menu tree.
 *
 * @param $tree
 *   A data structure representing the tree as returned from menu_tree_data.
 *
 * @return string
 *   The complete, rendered administration menu.
 */
function spmainmenus_mainmenu_tree_output($tree, $menuid="menu-primary-links", $firstpass=1, &$special_div, $tid=NULL) {
  $cur = 0;
/*
 * Comme nous avons le menu en cache 1x pour toutes les pages, l'active trail est erroné. 
 * nous devons le reconstituer dans le template. 
 */
	if ($firstpass==1) {
		$special_div = array(); 
		if(!spmainmenus_set_activetrail($tree, 1, $special_div, $tid)) {
		  
		  //ON DEFINIT LE DEFAULT CURRENT
		  $index = array_keys($tree);
          $tree[$index[1]]["link"]["in_active_trail"] = TRUE;
          ///////////////////////////////////////////////////
         
		}
	}   
	
	
/*
 * Note pour PSp.
 * 
 * Cette fonction partours le menu de maniere récursive. Cad elle parcours un niveau et regarde si un niveau 
 * inférieur existe sous l'élément courant. Si c'est le cas, la fonction est ré-appellée par elle meme pour
 * constituer le niveau inférieur.
 * 
 */

	/*
	 * En cas de premier passage, on ajoute les JS nécessaire au menu SuperFich. Si le menu ne devait pas utiliser superfish
	 * il faudrait alros remplacer par le bon pluggin et remmplacer la fonction par : 
	 * 
	 * drupal_add_js(drupal_get_path('theme', 'wallydemov3') . '/scripts/xxxxxxx.js', 'theme');
	 * 
	 * si la CSS du menu était dans un fichier séparé de la home/autres pages il suffit de préciser ici le chemin vers
	 * cette css. 
	 * 
	 * drupal_add_css(drupal_get_path('theme', 'wallydemo') . '/css/xxxxx.css', 'theme');
	 * 
	 */
	$first_level_class = "";
    if ($firstpass==1) {
	  $first_level_class = " first_level";
      drupal_add_js(drupal_get_path('theme', 'wallydemo') . '/scripts/superfish.js','theme');

      /*
       * C'est le déclencheur Jquery. 
       */
      drupal_add_js("
          $(document).ready(function() {
            jQuery('ul#".$menuid."').superfish({
				pathClass : 'current',
				pathLevels : 1,
				delay : 3000,
				disableHI : true,
				dropShadows:   false, 
				autoArrows	: true,
				animation : {opacity:'show'},
				speed : 0,
				firstOnClick: true,
                onInit:  function(){
                	$(\"#special-nav\").fadeIn(1);
                }
            });
          });
      ", 'inline');
    }
         
    $output = '';

    /*
     * Boucle principale sur tous les items ($data) du menu.
     */
    foreach ($tree as $data) {
      //if($data['link']['href'] == '<front>') { dsm('zzz');dsm('zzz');dsm('zzz'); dsm($data); }
      // On vérifie que le lien n'a pas été caché dans l'admin.
      if (!$data['link']['hidden']) {
      	
				// on ajoute une classe (extra_class) à active si on est dans le lien courant.
        $extra_class = isset($data['link']['localized_options']['attributes']['class']) ? $data['link']['localized_options']['attributes']['class'] : NULL;
        if (isset($link['href']) && ($link['href'] == $_GET['q'] || ($link['href'] == '<front>' && drupal_is_front_page()))
         && (empty($link['language']) || $link['language']->language == $language->language)) {
          $extra_class .= ' active';
        }
        
        if(isset($data['link']['menu_edition2'])) {
          $extra_class .= $data['link']['menu_edition2'];         
        }
        
        // Je recherche le lien correct pour cet item (la fonction spmainmenus_mainmenu_menu_item_link constitue le
        // html de ce lien (<a>.......</a>))..
        $link = spmainmenus_mainmenu_menu_item_link($data['link']);

        // Ici je regarde si il y a un sous-menu de niveau-1 disponible.
        // Si oui on ré-appelle la fonction spmainmenus_mainmenu_tree_output().  
        if ($data['below']) {
        	$sub_menu = spmainmenus_mainmenu_tree_output($data['below'], "", $firstpass+1, $special_div,$tid);
          $output .= theme_spmainmenus_mainmenu_item($link, $data['link']['has_children'], $sub_menu, $data['link']['in_active_trail'], $extra_class.$first_level_class);
        }
        else 
        // Si non, on place juste le iien. 
        {
          $output .= theme_spmainmenus_mainmenu_item($link, $data['link']['has_children'], '', $data['link']['in_active_trail'], $extra_class.$first_level_class);
        }
        if($data['link']['in_active_trail']) $cur = 1;
      }
    }
 		/*
 		 * Si on est en premiere "passe" dans la fonction, ajouter l'id du menu pour
 		 * que le jquery puisse agie sur le bon element.
 		 */   
    if ($firstpass==1) {
      
      // Ajout du bouton VOO
      ////////////////////////////////////////////////////////////////////////////////////
      $voo = '';      
      if($menuid=="menuprincipal" && strstr($_GET['q'],"taxonomy/term")) {
        $tid = explode('/',$_GET['q']);
        $tid = $tid[2];
        $term = taxonomy_get_term($tid);
        if($term->name=="Accueil sudinfo.be"||$term->name=="Accueil lacapitale.be"||$term->name=="Accueil lameuse.be"||$term->name=="Accueil lanouvellegazette.be"||$term->name=="Accueil laprovince.be"||$term->name=="Accueil nordeclair.be") {
          $p = explode('?', $_SERVER['REQUEST_URI']);
          $voo =       	
          '<li class="first_level partner_enable">
          	   <a class="close-voo" href="http://'.$_SERVER['SERVER_NAME'].$p[0].'">Désactiver VOO</a>
          </li>
          <li class="first_level partner_disable">
          <a href="http://'.$_SERVER['SERVER_NAME'].$p[0].'?=voo">Activer VOO</a>
          </li>';
        }        
      }  
      ////////////////////////////////////////////////////////////////////////////////////
      
      return $output ? "\n<ul class='sf-menu sf-navbar sf-js-enabled sf-shadow' id='".$menuid."'>". $output.$voo."</ul>\n" : "";
    }
	elseif ($firstpass==2) {
      return $output ? "\n<ul class='sf-submenu' id='".$menuid."'>". $output ."</ul>\n" : "" ;
    }
	else {
      return $output ? "\n<ul class='specmenu'>". $output ."</ul>\n" : "";
    }
  }
}

if (!function_exists('spmainmenus_mainmenu_menu_item_link')) {
/**
 * High-performance implementation of theme_menu_item_link().
 *
 * This saves us a theme() call and does only the absolute minimum to get
 * the menu "links" rendered.
 */
function spmainmenus_mainmenu_menu_item_link($link) {

    //dsm($link,'LINK');
		// note pur PSP : dsm($link); pour avoir toutes les infos.
		if(isset($link['localized_options']['attributes']['target'])){ $target=" target=\"".$link['localized_options']['attributes']['target']."\""; } else { $target = ""; }
	  $lien = '<a href="'. check_url(url($link['href'])) .'"'.$target.'>';
	  $lien .= $link['title'];
	  $lien .= '</a>'; 
    return $lien;
  }
}

if (!function_exists('theme_spmainmenus_mainmenu_item')) {
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
function theme_spmainmenus_mainmenu_item($link, $has_children, $menu = '', $in_active_trail = FALSE, $extra_class = NULL) {
 
 	/*
	 * note pour PSp: on constitue l'item de menu a partir du lien, des extra-classe eventuelle.  
	 * return '<li class="first_level '. $extra_class  .'">'. $link . $menu .'</li>';
	*/
	if ($in_active_trail) {
		$extra_class .= ' current';
	} 
	
	return '<li'. (!empty($extra_class) ? ' class="'. $extra_class .'"' : '') .'>'. $link . $menu .'</li>';
	
 }
}

/*
 * Appel principal des fonction. CHargement le la variable $main_menu avec le HTML généré.
*/

$tid = $context['relationship_termfromdestination_1']->data->tid;
$special_div = array(); 
$main_menu = spmainmenus_mainmenu_tree_output($menu, "menuprincipal", 1, $special_div,$tid);
$dum = array();
$special_menu = "";
if(isset($special_div[0])){
	if (sizeof($special_div[0])>0) {
	  $tid = $context['argument_terms_1']->data->tid;
		$special_menu = spmainmenus_mainmenu_tree_output($special_div[0], "special_menu", 1, $dum,tid);
	}
}

?>
<div id="navcontainer" class="clearfix">
	<div class="clearfix"><?php print $main_menu; ?></div>
</div>
<div id="special-nav"><?php print $special_menu; ?></div>