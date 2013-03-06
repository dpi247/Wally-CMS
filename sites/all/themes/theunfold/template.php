<?php
/**
 * Implementation of hook_theme
 * */
function theunfold_theme(&$var) {

  $path = drupal_get_path('theme', 'theunfold');
  $base = array(
    'file' => 'theme.inc',
  );
  return array(
    'theme_package_linkslists' => array(
  	  'arguments' => array('vars' => NULL),
    ),
    'theme_package_bear_items' => array(
  	  'arguments' => array('vars' => NULL),
    ),
    'theme_package_bottom_items' => array(
  	  'arguments' => array('vars' => NULL),
    ),
    'theme_package_top_items' => array(
  	  'arguments' => array('vars' => NULL),
    ),
    'theme_package_related_items' => array(
  	  'arguments' => array('vars' => NULL),
    ),
    'theme_package_by' => array(
  	  'arguments' => array('vars' => NULL),
    ),
    'theme_package_freetags' => array(
  	  'arguments' => array('vars' => NULL),
    ),
  	'theme_package_persons' => array(
  	  'arguments' => array('vars' => NULL),
  	),
    'emaudio_flowplayer_flash' => array(
  	  'arguments' => array('url' => NULL, 'width' => '100%', 'height' => 0, 'field' => NULL, 'data' => array(), 'node' => NULL, 'autoplay' => FALSE),
    ),

	// Widget Templates.
    'widget_article_summary' => $base + array(
	  'arguments' => array('widget' => "",),
	  'template' => 'widget_article_summary',
	  'path' => $path.'/templates/widgets',
	),
    'widget_article_summary_inline' => $base + array(
	  'arguments' => array('widget' => "",),
	  'template' => 'widget_article_summary_inline',
	  'path' => $path.'/templates/widgets',
	),
    'widget_article_summary_titleonly' => $base + array(
	  'arguments' => array('widget' => "",),
	  'template' => 'widget_article_summary_titleonly',
	  'path' => $path.'/templates/widgets',
	),
    'widget_article_quote' => $base + array(
	  'arguments' => array('widget' => "",),
	  'template' => 'widget_article_quote',
	  'path' => $path.'/templates/widgets',
	),
    'widget_media' => $base + array(
  	  'arguments' => array('widget' => "",),
  	  'template' => 'widget_media',
  	  'path' => $path.'/templates/widgets',
    ),
    'widget_linkslist' => $base + array(
      'arguments' => array('widget' => '',),
      'template' => 'widget_linkslist',
      'path' => $path.'/templates/widgets',
    ),
    'widget_address' => $base + array(
      'arguments' => array('widget' => ''),
      'template' => 'widget_address',
      'path' => $path.'/templates/widgets',
    ),  
  // Node Widgets
	
	'theme_redacblock_node_linklist' => array(
	  'arguments' => array('items' => NULL),
    ),
	'theme_redacblock_node_figure' => array(
	  'arguments' => array('item' => NULL),
    ),
	'theme_redacblock_node_mediaicon' => array(
	  'arguments' => array('items' => NULL),
    ),    
    'wallyct_flowmix' => array(
      'arguments' => array("subtype" => NULL, "context" => NULL, "feed" => NULL, "options" => NULL),
    ),
    
    'theme_flowmix_image' => array(
      'arguments' => array("subtype" => NULL, "context" => NULL, "feed" => NULL, "options" => NULL),
    ),
    'theunfold_flowmix_image' => array(
      'arguments' => array('title' => NULL, 'link' => NULL, 'flow' => NULL),
      'template' => 'theunfold_flowmix_image',
      'path' => $path.'/templates/flowmix',
    ),
  );
}

function legacy_theunfold_preprocess_node_build_redacblock_node (&$vars) {

  $type = $vars["type"];
  $node = &$vars['node'];
  
  $vars['redacblock_node'] = array(); 
  $merged_medias = array();
  if (isset($node->field_embededobjects_nodes) && !empty($node->field_embededobjects_nodes)) {
  	foreach ($node->field_embededobjects_nodes as $embed){
	    $merged_medias += theunfold_preprocess_node_build_embedded_photos($embed);
	    $merged_medias += theunfold_preprocess_node_build_embedded_videos($embed);
	    $merged_medias += theunfold_preprocess_node_build_embedded_audios($embed);
	    $merged_medias += theunfold_preprocess_node_build_embedded_links($embed);
	    $merged_medias += theunfold_preprocess_node_build_embedded_documents($embed);
	    $merged_medias += theunfold_preprocess_node_build_embedded_text($embed);
    }
  }
  
  $topItems = array();
  $bottomItems = array();
  theunfold_preprocess_node_article_dispatch_top_bottom($vars, $merged_medias, $topItems, $bottomItems, $bearItems);
  $linkslist = theunfold_get_sorted_links($vars['node']);

  $vars['redacblock_node']['url'] = $vars["url"]; 
  $vars['redacblock_node']['title'] = $vars["title"]; 
  $vars['redacblock_node']['barette'] =  $vars['barette']; 
  $vars['redacblock_node']['by'] = $vars['by'];
  $vars['redacblock_node']['publicationdate'] = $vars['publicationdate'];
  $vars['redacblock_node']['textchapo'] = $vars['textchapo'];
  $vars['redacblock_node']['body'] = $vars['body'];
  $vars['redacblock_node']['publicationdate']= $vars['publicationdate'];
  $vars['redacblock_node']['comment_count'] = $vars['comment_count'];
  $vars['redacblock_node']['figure'] = theme('theme_redacblock_node_figure', $topItems[key($topItems)]);
  $vars['redacblock_node']['mediaicons'] = theme('theme_redacblock_node_mediaicon', $merged_medias);
  $vars['redacblock_node']['linklist'] = theme('theme_redacblock_node_linklist',$linkslist[0]); 

}

/*
 * Prepare data for widget "article_summary" from node.
 * 
 */
function theunfold_widget_prepare_article_summary_node(&$node) {

  $widget= array(); 
  $widget += theunfold_preprocess_node_build($node);

  $merged_medias = array();
  if (isset($node->field_embededobjects_nodes) && !empty($node->field_embededobjects_nodes)) {
  	foreach ($node->field_embededobjects_nodes as $embed){
	    $merged_medias += theunfold_preprocess_node_build_embedded_photos($embed);
	    $merged_medias += theunfold_preprocess_node_build_embedded_videos($embed);
	    $merged_medias += theunfold_preprocess_node_build_embedded_audios($embed);
	    $merged_medias += theunfold_preprocess_node_build_embedded_links($embed);
	    $merged_medias += theunfold_preprocess_node_build_embedded_documents($embed);
	    $merged_medias += theunfold_preprocess_node_build_embedded_text($embed);
    }
  }
  
  $topItems = array();
  $bottomItems = array();
  theunfold_preprocess_node_article_dispatch_top_bottom($node, $merged_medias, $topItems, $bottomItems, $bearItems);
  $linkslist = theunfold_get_sorted_links($node);

  $widget['figure'] = theme('theme_redacblock_node_figure', $topItems[key($topItems)]);
  $widget['mediaicons'] = theme('theme_redacblock_node_mediaicon', $merged_medias);
  $widget['linklist'] = theme('theme_redacblock_node_linklist',$linkslist[0]); 
  
  return $widget; 
  
}
/**
 * Prepare dara fot widget "media" from node
 * */
function theunfold_widget_prepare_media_node(&$node){
  $widget = array();

  $widget['title'] = $node->title;
  
  $by = NULL;
  if ($node->field_authors[0]['value'] != NULL){
    foreach ($node->field_authors as $author){
      $by[] = $author['view'];
    }
  }
  if ($mainstory->field_copyright[0]['safe'] != NULL){
    $by[] = $mainstory->field_copyright[0]['safe'];
  }
  $widget['by'] = theme('theme_package_by', array('by' => $by));
  $widget['publicationdate'] = $node->created;
  $widget['comment_count'] = $node->comment_count;
  $widget['summary'] = $node->field_summary[0]['safe'];
  
  //Render media
  $medias = array();
  $medias += theunfold_preprocess_node_build_embedded_photos($node);
  $medias += theunfold_preprocess_node_build_embedded_videos($node);
  $medias += theunfold_preprocess_node_build_embedded_audios($node);
  $medias += theunfold_preprocess_node_build_embedded_links($node);
  $medias += theunfold_preprocess_node_build_embedded_documents($node);
  $medias += theunfold_preprocess_node_build_embedded_text($node);
  $widget['media'] = '';
  
  if (count($medias) == 1){
    $media = array_pop($medias);
    $widget['media'] = $media['content'];
  }
  return $widget;
}
function theunfold_widget_prepare_linkslist_node(&$node){
  $widget = array();
  $widget['title'] = $node->title;
  $widget['links'] = array();
  foreach ($node->field_links_list as $link){
    $widget['links'][] = array('title' => $link['safe']['title'], 'link' => '/'.drupal_get_path_alias('node/'.$link['safe']['nid']));
  }
  return $widget;
}
/*
 * theming function for figures
 */
function theunfold_theme_redacblock_node_figure($item) {
	$content = "<figure>".$item['content']."</figure>";
	return ($content); 
}

/**
 * Hook preprocess node
 */
function theunfold_preprocess_node(&$vars){
  $type = $vars["type"];
  $node = &$vars['node'];

  if (isset($vars['view'])) {
    $view = &$vars['view'];
  }

  switch ($node->type){
    case 'wally_articlepackage':
      // We build data for node template teaser mode
      if ($node->teaser) {
        $vars["widget"] = theunfold_widget_prepare_article_summary_node($node);
      }
      // We build data for redacblock views
      if (isset($view)) {
        $current_display = &$view->display[$view->current_display];
        if ($current_display->display_options['row_plugin'] == 'redacblock_row') {
          $vars["widget"] = theunfold_widget_prepare_article_summary_node($node);
        }
      }

      // build data for node page
      if ($node->nid == arg(1) || $node->preview || true ) {

        // We unset the body, theunfold_preprocess_node_build will create a new one.
        unset($vars["body"]);
        $vars += theunfold_preprocess_node_build($node);
         
        if ($node->preview && isset($node->field_embededobjects_nodes) && !empty($node->field_embededobjects_nodes)) {
          foreach ($node->field_embededobjects_nodes as $delta => $embed) {
            // Fake nid in case of preview
            $node->field_embededobjects_nodes[$delta]->nid = $delta;
          }
        }
        $merged_medias = array();
        if (isset($node->field_embededobjects_nodes) && !empty($node->field_embededobjects_nodes)) {
          if (module_exists('cciinlineobjects')) {
            cciinlineobjects_flag_inline_embed_objects($node);
          }
          foreach ($node->field_embededobjects_nodes as $embed) {
            $merged_medias += theunfold_preprocess_node_build_embedded_photos($embed);
            $merged_medias += theunfold_preprocess_node_build_embedded_videos($embed);
            $merged_medias += theunfold_preprocess_node_build_embedded_audios($embed);
            $merged_medias += theunfold_preprocess_node_build_embedded_links($embed);
            $merged_medias += theunfold_preprocess_node_build_embedded_documents($embed);
            $merged_medias += theunfold_preprocess_node_build_embedded_text($embed);
          }
          
          $topItems = array();
          $bottomItems = array();
          $bearItems = array();
          theunfold_preprocess_node_article_dispatch_top_bottom($node, $merged_medias, $topItems, $bottomItems, $bearItems);
          $linkslist = theunfold_get_sorted_links($node);

          $vars['top_html'] = theme('theme_package_top_items', array('topItems' => $topItems));
          $vars['bottom_html'] = theme('theme_package_bottom_items', array('bottomItems' => $bottomItems));
          $vars['related_html'] = theme('theme_package_related_items',
            array('linkslist' => $linkslist,
              'earItems' => $bearItems,
              'freeTags'  => $vars['field_free_tags'],
              'persons'  => $vars['field_persons'],
            )
          );
        }
      }
      break;

    case 'wally_photoobject':
    case 'wally_videoobject':
    case 'wally_audioobject':
    case 'wally_digitalobject':
    case 'wally_linktype':
      $vars['widget'] = theunfold_widget_prepare_media_node($node);
      break;
    case 'wally_textobject':
      unset($vars["body"]);
      $vars += theunfold_preprocess_node_build_textobject($node);
      break;
    case 'wally_linkslistobject':
      $vars['widget'] = theunfold_widget_prepare_linkslist_node($node);
      break;
  }
}

/**
 * Render the classique elements of the package
 * */
function theunfold_preprocess_node_build(&$node){
  
  $vars = array();
  
  $mainstory = node_build_content($node->field_mainstory_nodes[0]);
  
  $vars['title'] = $mainstory->title;
  $vars['barette'] = $mainstory->field_textbarette[0]['safe'];
  
  //by
  $by = NULL;
  if ($mainstory->field_authors[0]['value'] != NULL){
    foreach ($mainstory->field_authors as $author){
      $author_term = taxonomy_get_term($author['value']);
      $by[] = $author_term->name;
    }
  }
  if ($mainstory->field_byline[0]['safe'] != NULL){
    $by[] = $mainstory->field_byline[0]['safe'];
  }
  if ($mainstory->field_copyright[0]['safe'] != NULL){
    $by[] = $mainstory->field_copyright[0]['safe'];
  }
  $vars['by'] = theme('theme_package_by', array('by' => $by));
  
  //Convert date to timestamp with the correct timezone
  date_default_timezone_set($node->field_publicationdate[0]['timezone_db']);
  $vars['publicationdate'] = strtotime($node->field_publicationdate[0]['value']);
  date_default_timezone_set($node->field_publicationdate[0]['timezone']);

  $vars['textchapo'] = $mainstory->field_textchapo[0]['safe'];
  $vars['body'] = $mainstory->field_textbody[0]['safe'];
  $vars['url'] = drupal_get_path_alias("node/".$node->nid); 
  $vars['comment_count'] = $node->comment_count; 
  
  return $vars; 
}
/**
 * Preprocess textobject
 * 
 * */
function theunfold_preprocess_node_build_textobject($node){
  $vars = array();
  $vars['title'] = $node->title;
  $vars['barette'] = $node->field_textbarette[0]['safe'];
  
  //by
  $by = NULL;
  if ($node->field_authors[0]['value'] != NULL){
    foreach ($node->field_authors as $author){
      $by[] = $author['view'];
    }
  }
  if ($node->field_byline[0]['safe'] != NULL){
    $by[] = $node->field_byline[0]['safe'];
  }
  if ($node->field_copyright[0]['safe'] != NULL){
    $by[] = $node->field_copyright[0]['safe'];
  }
  $vars['by'] = theme('theme_package_by', array('by' => $by));
  
  $vars['publicationdate'] = $node->created;
  
  $vars['textchapo'] = $node->field_textchapo[0]['safe'];
  $vars['body'] = $node->field_textbody[0]['safe'];
  $vars['url'] = drupal_get_path_alias("node/".$node->nid);
  $vars['comment_count'] = $node->comment_count;
  
  return $vars;
}
/**
* Theme function to render the html of the media box
* */
function theunfold_theme_package_top_items($vars){

  drupal_add_js(drupal_get_path('theme', 'theunfold').'/scripts/mylibs/pagination.js');
  $content = '';
  if (count($vars['topItems']) != 0){
    $content .= '<div class="block-slidepic media">';
    $content .= '<ul class="page-inner">';
    foreach ($vars['topItems'] as $item){
      $content .= '<li>';
      $content .= '<figure>';
      $content .= $item['content'];
      
      $content .= '<figcaption>'.$item['title'].' '.$item['summary'].'</figcaption>';
      
      $content .= '</figure>';
      $content .= '</li>';
    }
    $content .= '</ul>';
    $content .= '<div class="pagination"></div>';
    $content .= '</div>';
  }
  return $content;
}
/**
* Theme function to render the html of the bottom media
* */
function theunfold_theme_package_bottom_items($vars){
  if (count($vars['bottomItems'])){
    $bottom_html .= '<div class="bottom-items">';
    $bottom_html .= '<ul class="article-group">';
    foreach ($vars['bottomItems'] as $id => $item){
      $bottom_html .= '<li class="article-inline small">';
      $bottom_html .= '<a href="'.$item['link'].'">';
      if ($item['thumb'] != NULL){
        $bottom_html .= '<figure>'.theme('imagecache', '120x120', $item['thumb'],'', '').'</figure>';
      }
      $bottom_html .= '<h2>'.$item['title'];
      if ($item['copyright'] != NULL){
        $bottom_html .= '<span class="meta"><span class="author">';
        $bottom_html .= theme('theme_package_by', array('by' => array($item['copyright'])));
        $bottom_html .= '</span></span>';
      }
      $bottom_html .= '</h2>';
      $bottom_html .= '</a>';
      if ($item['summary'] != NULL){
        $bottom_html .= '<div class="description">'.$item['summary'].'</div>';
      }
      $bottom_html .= $item["content"];
      $bottom_html .= '</li>';
    }
    $bottom_html .= '</ul>';
    $bottom_html .= '</div>';
  }
  return $bottom_html;
}
function theunfold_theme_package_bear_items($vars){
  $bear_html = '';
  if (is_array($vars['bearItems'])){
    foreach ($vars['bearItems'] as $bear){
      $bear_html .= '<h2 class="section-title"><a href="/'.drupal_get_path_alias('node/'.$bear['package']->nid).'">'.$bear['title'].'</a></h2>';
      $bear_html .= '<ul class="article-group">';
      $bear_html .= '<li>';
      if ($bear['photo_object'] != NULL){
        $bear_html .= theme('imagecache', '743x430', $bear['photo_object'][0]->field_photofile[0]['filepath'], '', '');
      }
      $bear_html .= '<div class="description">'.$bear["text"].'</div>';
      $bear_html .= '</li>';
      $bear_html .= '</ul>';
    }
  }
  return $bear_html;
}


function theunfold_theme_package_related_items($vars){
  $content  = '';
  $content .= theme('theme_package_linkslists', array('linkslist' => $vars['linkslist']));
  $content .= theme('theme_package_freetags', array('freeTags' => $vars['freeTags']));
  $content .= theme('theme_package_persons', array('persons' => $vars['persons']));
  $content .= theme('theme_package_bear_items', array('bearItems' => $vars['bearItems']));
 
  return $content;
}
function theunfold_theme_package_persons($vars){
  $content = '';
  if ($vars['persons'][0]['value'] != NULL){
    $content .= '<h2 class="section-title">'.t('Persons').'</h2>';
    $vir = '';
    foreach ($vars['persons'] as $freetags){
      $content .= $vir.'<a href="/'.drupal_get_path_alias('taxonomy/term/'.$freetags['value']).'">'.$freetags['view'].'</a>';
      $vir = ', ';
    }
  }
  return $content;
}
function theunfold_theme_package_freetags($vars){
	$content = '';
	if ($vars['freeTags'][0]['value'] != NULL){
		$content .= '<h2 class="section-title">'.t('Free Tags').'</h2>';
		$vir = '';
		foreach ($vars['freeTags'] as $freetags){
			$content .= $vir.'<a href="/'.drupal_get_path_alias('taxonomy/term/'.$freetags['value']).'">'.$freetags['view'].'</a>';
			$vir = ', ';
		}
	}
	return $content;
}
function theunfold_theme_package_by($vars){
  $content = NULL;
  $vir = '';
  if (count($vars['by']) != 0){
    $content .= t('By').' ';
    foreach ($vars['by'] as $by){
      $content .= $vir.mb_strtoupper($by);
      $vir = ', ';
    }
  }
  return $content;
}
/**
 * Dispatch media in two part, top and bottom
 */
function theunfold_preprocess_node_article_dispatch_top_bottom($node, $allItems, &$top, &$bottom, &$bear){
  if ($node->field_embededobjects_nodes != NULL) {
    foreach ($node->field_embededobjects_nodes as $nid => $embed) {
      if (!isset($embed->inline_object) || !$embed->inline_object) {
        if ($item = $allItems[$embed->nid]) {
          switch ($item['group_type']){
            case 'photo':
              $top[$embed->nid] = $item;
              break;
            case 'video':
              $top[$embed->nid] = $item;
              break;
            case 'package':
              $bear[$embed->nid] = $item;
              break;
            default:
              $bottom[$embed->nid] = $item;
              break;
          }
        }
      }
    }
  }
}

function theunfold_theme_package_linkslists($vars){
  $content = '';
  if ($vars['linkslist'] != NULL){
    foreach ($vars['linkslist'] as $linkslist){
      $content .= '<h2 class="section-title">'.$linkslist['title'].'</h2>';
      if ($linkslist['links'] != NULL){
        $content .= '<ul class="article-list">';
        foreach ($linkslist['links'] as $link){
          $target = '';
          if ($link['target'] == '_blank'){
            $target = ' target="_blank" ';
          }
          $content .= '<li><a href="'.$link['url'].'"'.$target.'>'.$link['title'].'</a></li>';
        }
        $content .= '</ul>';
      }
    }
  }
  return $content;
}

/*
 * theming function for figures media icons
 */
function theunfold_theme_redacblock_node_linklist($linkslist) {
	  
  $content = '';
  if ($linkslist['links'] != NULL){
    $content .= '<ul class="hidden-phone article-list">';
    foreach ($linkslist['links'] as $link){
      $target = '';
      if ($link['target'] == '_blank'){
        $target = ' target="_blank" ';
      } 
      $content .= '<li><a href="'.$link['url'].'"'.$target.'>'.$link['title'].'</a></li>';
    }
    $content .= '</ul>';
  }
  return $content;
}

/*
 * theming function for figures media icons
 */
function theunfold_theme_redacblock_node_mediaicon($items) {
	$img = 0; 
	$content = "";
	
	foreach($items as $key => $item) {
		if ($item['icon'] == "image") $img++;
		$icons[$item['icon']]=$item['icon'];
	}
	if ($img<2) unset($icons["image"]); 
	if (count($icons)){
	  $content = "<span class=\"hidden-phone lsf\">". implode(",", $icons)."</span>";
	} 
	return ($content); 
}

/**
 * Hook_closure
 * 
 * @param : $main (optional): Whether the current page is the front page of the site.
 * 
 * @return : A string containing the results of the hook_footer() calls.
 * 
 **/
function theunfold_closure($main = 0) {
  /* Add inline javascripts & external javascript files into footer. */
  $vars["theme_path"] = drupal_get_path('theme', 'theunfold');
  $footer = module_invoke_all('footer', $main);
  $footer[] = "<script src=\"http://ajax.googleapis.com/ajax/libs/jquery/1.3.2/jquery.min.js\"></script>";
  $footer[] = "<script>window.jQuery || document.write('<script src=\"".$vars["theme_path"]."js/vendor/jquery-1.3.2.min.js\"><\/script>')</script>";
  $footer[] = "<script src='http://cdn.jquerytools.org/1.2.7/full/jquery.tools.min.js'></script>";
  return implode("\n", $footer) . drupal_get_js('footer');
}


/**
 * _theunfold_set_js : Set all necessary javascripts.
 * 
 * @param : $vars
 * @return : $vars
 * 
 **/
function _theunfold_set_js(&$vars) {

  // Put Scripts at the Bottom 
  // @info: Look at hook_closure
  drupal_add_js($vars["theme_path"].'/scripts/mylibs/script.js', 'theme', 'footer');
  drupal_add_js($vars["theme_path"].'/scripts/mylibs/swipe.js', 'theme',  'footer');
  drupal_add_js($vars["theme_path"].'/scripts/mylibs/ticker.js', 'theme',  'footer');
  drupal_add_js($vars["theme_path"].'/scripts/mylibs/jquery.easing.1.3.js', 'theme',  'footer');
  drupal_add_js($vars["theme_path"].'/scripts/mylibs/jquery.elastislide.js', 'theme',  'footer');

  // Add js to the header
 drupal_add_js($vars["theme_path"].'/scripts/vendor/modernizr-2.6.1.min.js');
 drupal_add_js("
<script type=\"text/javascript\">
    (function(doc) {
    var addEvent = 'addEventListener',
        type = 'gesturestart',
        qsa = 'querySelectorAll',
        scales = [1, 1],
        meta = qsa in doc ? doc[qsa]('meta[name=viewport]') : [];

      function fix() {
        meta.content = 'width=device-width,minimum-scale=' + scales[0] + ',maximum-scale=' + scales[1];
        doc.removeEventListener(type, fix, true);
      }

      if ((meta = meta[meta.length - 1]) && addEvent in doc) {
        fix();
        scales = [.25, 1.6];
        doc[addEvent](type, fix, true);
      }

    }(document));
  </script>
", 'inline'); 
	
  $vars['scripts'] = drupal_get_js(); // necessary in D7?
  $vars['closure'] = theme('closure'); // necessary in D7?
  
  return $vars;
}

/**
 * _theunfold_genral_theme_settings: Setting up some TheUnfold Specific theme variables.
 * 
 * @param : $vars
 * @return : $vars
 * 
 **/
function _theunfold_general_theme_settings(&$vars) {

	$vars['site_name'] = (theme_get_setting('toggle_name') ? filter_xss_admin(variable_get('site_name', 'TheUnfold')) : '');
  $vars['mission'] = (theme_get_setting('toggle_mission') ? filter_xss_admin(variable_get('site_mission', 'TheUnfold : Happily published by <a href="http://www.dpi247.com">DPI247</a>, a <a href="http://">Drupal</a> media dedicated distribution.')) : ''); 

  $vars['body_class'] = isset($vars['node']) ? $vars['node']->type.' ' : '';

	//Allows specific theming for taxonomy listings
  if (arg(0) == 'taxonomy') {
  	$vars['body_class'] = 'taxonomy_list';
  	// @todo: Add taxonomy term to class list.
  }

  //Add additional class if this is the front page (for home page specific theming)
  if (drupal_is_front_page()) {
  	$vars['body_class'] .= ' home';
  }
  $vars['body_class']=trim($vars['body_class']); 

  
/**
 * @info : comes from preprocess_page original hook. For Themer information only.
 *
 * 
  $vars['head_title']        = implode(' | ', $head_title);
  $vars['base_path']         = base_path();
  $vars['front_page']        = url();
  $vars['breadcrumb']        = theme('breadcrumb', drupal_get_breadcrumb());
  $vars['feed_icons']        = drupal_get_feeds();
  $vars['footer_message']    = filter_xss_admin(variable_get('site_footer', FALSE));
  $vars['head']              = drupal_get_html_head();
  $vars['help']              = theme('help');
  $vars['language']          = $GLOBALS['language'];
  $vars['language']->dir     = $GLOBALS['language']->direction ? 'rtl' : 'ltr';
  $vars['messages']          = $variables['show_messages'] ? theme('status_messages') : '';
  $vars['primary_links']     = theme_get_setting('toggle_primary_links') ? menu_primary_links() : array();
  $vars['secondary_links']   = theme_get_setting('toggle_secondary_links') ? menu_secondary_links() : array();
  $vars['search_box']        = (theme_get_setting('toggle_search') ? drupal_get_form('search_theme_form') : '');
  $vars['tabs']              = theme('menu_local_tasks');
  $vars['title']             = drupal_get_title();
 *
 *  
 **/

 return $vars;
  
}



/*
 * theunfold_preprocess_page : Hook_Preprocess_Page
 */
function theunfold_preprocess_page(&$vars){

  $vars["theme_path"] = drupal_get_path('theme', 'theunfold');
 
  _theunfold_set_js($vars);
  _theunfold_general_theme_settings($vars); 
  
}
/**
 * hook_preprocess_search_result
 * */
function theunfold_preprocess_search_result(&$vars){

  if ($vars['type'] == 'node'){
    //In node case, add a type name
    switch ($vars['result']['node']->type){
      case 'wally_articlepackage': $vars['info_split']['type_name'] = t('Article');break;
      case 'wally_audioobject': $vars['info_split']['type_name'] = t('Audio');break;
      case 'wally_digitalobject': $vars['info_split']['type_name'] = t('Digital document');break;
      case 'wally_linkslistobject': $vars['info_split']['type_name'] = t('Links list');break;
      case 'wally_photoobject': $vars['info_split']['type_name'] = t('Photo');break;
      case 'wally_pollobject': $vars['info_split']['type_name'] = t('Poll');break;
      case 'wally_textobject': $vars['info_split']['type_name'] = t('Text');break;
      case 'wally_videoobject': $vars['info_split']['type_name'] = t('Video');break;
      case 'wally_gallerypackage': $vars['info_split']['type_name'] = t('Gallery');break;
      case 'wally_pollpackage': $vars['info_split']['type_name'] = t('Poll');break;
      case 'wally_entitytype': $vars['info_split']['type_name'] = t('Entity');break;
      case 'wally_linktype': $vars['info_split']['type_name'] = t('Link');break;
      case 'wally_locationtype': $vars['info_split']['type_name'] = t('Location');break;
      case 'wally_persontype': $vars['info_split']['type_name'] = t('Person');break;
      default:$vars['info_split']['type_name'] = $vars['info_split']['type'];
    }
  } 
}

/**
 * Filter front text
 * This function has to be completed with our own text filter
 */
function theunfold_check_plain($text){
  return strip_tags($text);
}

/**
 * Try to find the package's first photoObject.
 * If not, return false
 *
 */
function theunfold_get_first_photoEmbededObject_from_package($embededObjects_array){
  if (is_array($embededObjects_array)){
	foreach ($embededObjects_array as $embededObject){
	  if ($embededObject->type == "wally_photoobject"){
	    $photoObject = $embededObject;
	    break;
	  }
	}
  }
  if (isset($photoObject)){
    return $photoObject;
  } else {
    return FALSE;
  }
}

/**
* Try to find the package's first photoObject.
* If not, return false
*
*/
function theunfold_get_first_textEmbededObject_from_package($embededObjects_array){
  $textObject = FALSE;
  if (is_array($embededObjects_array)){
    foreach ($embededObjects_array as $embededObject){
      if ($embededObject->type == "wally_textobject"){
        $textObject = $embededObject;
        break;
      }
    }
  }
  return $textObject;
}
/**
 *
 * Brackets the embeddedObjects to be displayed in the package template
 * @param $node
 *
 *
 * @return $data
 *  new array of node's embdeddedObjects
 *
 */

function theunfold_bracket_embeddedObjects_from_package($node){
  $data = array();
  $photos = array();
  $videos = array();
  $photosvideos = array();
  $audios = array();
  $digital = array();
  $links = array();
  $texts = array();
  $textobject = array();
  $all = array();
  $embeddedObjects = $node->field_embededobjects_nodes;
  if ($node->type == "wally_articlepackage"){
    $data["mainObject"] = $embeddedObjects[0];
  } else {
	$data["mainObject"] = $node->field_mainobject_nodes[0];
  }
  foreach ($embeddedObjects as $embeddedObject){
  	array_push($all, $embeddedObject);
    switch ($embeddedObject->type){
      case "wally_photoobject":
    	array_push($photosvideos, $embeddedObject);
    	array_push($photos, $embeddedObject);
    	break;
      case "wally_videoobject":
    	array_push($photosvideos, $embeddedObject);
    	array_push($videos, $embeddedObject);
    	break;
      case "wally_audioobject":
        array_push($audios, $embeddedObject);
    	break;
      case "wally_digitalobject":
        array_push($digital, $embeddedObject);
    	break;
      case "wally_linktype":
        array_push($links, $embeddedObject);
        break;
      case "wally_articlepackage":
        array_push($texts, $embeddedObject);
        break;
      case "wally_textobject":
        array_push($textobject, $embeddedObject);
        break;
    }
  }
  $data["photos"] = $photos;
  $data["videos"] = $videos;
  $data["audios"] = $audios;
  $data["digital"] = $digital;
  $data["links"] = $links;
  $data["photosvideos"] = $photosvideos;
  $data["textobjects"] = $textobject;
  $data["all"] = $all;
  $data["texts"] = $texts;
  return $data;
}

/**
 * Render an array with a texts's (package) infos for theming operations
 *
 */
function theunfold_get_texts_infos_and_display($node,$template="default"){

	$mainstory = $node->field_mainstory_nodes[0];
  $text = array();
  $text["nid"] = $node->nid;
  $text["title"] = $mainstory->title;
	$text["chapeau"] = $mainstory->field_textchapo[0]['value'];
  $text["text"] = check_markup($mainstory->field_textbody[0]['value'],$mainstory->field_textbody[0]['format']);

  $photo = theunfold_get_first_photoEmbededObject_from_package($node->field_embededobjects_nodes);
	if ($photo) $text["photo"][] = theunfold_get_photo_infos_and_display($photo);
	return $text;

}

/**
 * Render an array with a texts's (package) infos for theming operations
 *
 */
function theunfold_get_textobject_infos_and_display($textobject,$template="default"){

  $text = array();
  $text["nid"] = $textobject->nid;
  $text["title"] = $textobject->title;
  $text["chapeau"] = $textobject->field_textchapo[0]['value'];
  $text["text"] = check_markup($textobject->field_textbody[0]['value'],$textobject->field_textbody[0]['format']);
  return $text;

}

/**
 * Render an array with a photoObject's infos for theming operations
 *
 */
function theunfold_get_photo_infos_and_display($photoObject,$template="default"){
  $photo = array();
  $photo["nid"] = $photoObject->nid;
  $photo["title"] = $photoObject->title;
  $photo["type"] = $photoObject->type;
  $photo['credit'] = $photoObject->field_copyright[0]['value'];
  $photo['summary'] = $photoObject->field_summary[0]['value'];
  $photo['fullpath'] = $photoObject->field_photofile[0]['filepath'];
  $photo['size'] = $photoObject->field_photofile[0]['filesize'];
  $photo['filename'] = $photoObject->field_photofile[0]["filename"];

  switch ($template){
	case "default":
      $photo['main_size'] = "";
      $photo['main_url'] = "";
      $photo['mini'] = "";
	  if ($photo['size'] > 0){
		$photo['emcode'] = theme('imagecache', '743x430',$photo['fullpath'],'', '');
		$photo['main_size'] = theme('imagecache', '743x430',$photo['fullpath'],'', '');
		$photo['main_url'] = imagecache_create_url('743x430', $photo['fullpath']);
		$photo['mini'] = theme('imagecache', 'article_48x32', $photo['fullpath'],'', '');
		$photo['ours'] = theme('imagecache', '185x123', $photo['fullpath'],'', '');
		$photo['medium'] = theme('imagecache', '236x158', $photo['fullpath'], '', '');
	  }
	  break;
  }

  return $photo;

}

/**
 * Render an array with a audioObject's infos for theming operations
 *
 */
function theunfold_get_audio_infos_and_display($audioObject){

  $audio = array();
  $audio["nid"] = $audioObject->nid;
  $audio["type"] = $audioObject->type;
  $audio['emcode'] = $audioObject->content['group_audio']['group']['field_audio3rdparty']['field']['items'][0]['#children'];
  $audio['title'] = $audioObject->title;
  $audio['summary'] = $audioObject->field_summary[0]['value'];
  $audio['credit'] = $audioObject->field_copyright[0]['value'];

  return $audio;

}

function theunfold_get_linkobject_infos_and_display($linkObject){

  $link = array();
  $link["nid"] = $linkObject->nid;
  if($linkObject->field_internal_link[0]["nid"]){
    $n=node_load($linkObject->field_internal_link[0]["nid"]);
    if($n->field_mainstory[0]["nid"]){
      $n=node_load($n->field_mainstory[0]["nid"]);
      $link['title'] = $n->title;
      $link['summary'] = $n->field_textbody[0]['value'];
    }
  }
  return $link;

}

/**
 * Render an array with a videoObject's infos for theming operations
 *
 */
function theunfold_get_video_infos_and_display($videoObject){
  $video = array();
  $video["nid"] = $videoObject->nid;
  $video["type"] = $videoObject->type;
  $video['emcode'] = $videoObject->content['group_video']['group']['field_video3rdparty']['field']['items']['#children'];
  $video['summary'] = $videoObject->field_summary[0]['value'];
  $video['credit'] = $videoObject->field_copyright[0]['value'];

  $video['link'] = l($videoObject->field_link[0]["title"], $videoObject->field_link[0]["url"]);
  $video['title'] = $videoObject->title;
  $video['thumbnail'] = $videoObject->field_video3rdparty[0]['data']['thumbnail']["url"];

  return $video;

}

/**
 * Render an array with a digitalObject's infos for theming operations
 *
 */
function theunfold_get_digitalobject_infos_and_display($digitalObject){
  $digital = array();
  $digital["nid"] = $digitalObject->nid;
  $digital["type"] = $digitalObject->type;
  $emcode = $digitalObject->content['group_digitalobject']['group']['field_object3rdparty']['field']['items']['#children'];

  // USE FOR GOOGLE DOC BECAUSE IFRAME IS IN THE SUMMARY
  if($emcode) $digital['emcode'] = $emcode;
  else $digital['emcode'] = $digitalObject->field_summary[0]['value'];
  ////////////////////////

  $digital['summary'] = $digitalObject->field_summary[0]['value'];
  $digital['credit'] = $digitalObject->field_copyright[0]['value'];
  $digital['link'] = l($digitalObject->field_link[0]["title"], $digitalObject->field_link[0]["url"]);
  $digital['title'] = $digitalObject->title;
  $digital['thumbnail'] = $digitalObject->field_digital3rdparty[0]['data']['thumbnail']["url"];
  $digital['provider'] = $digitalObject->field_object3rdparty[0]['provider'];

  return $digital;

}


/**
* Render the links for linkslist
* */
function theunfold_get_sorted_links($node){

	$allLinks = array();
	$listLinks = $node->field_linkedobjects_nodes;
	if(is_array($listLinks)){
		foreach ($listLinks as $ll) {
			$lLinks = array();
			$lLinks["title"] = $ll->title;
			$lLinks["summary"] = $ll->field_summary[0]["value"];
			$i = 0;
			if(isset($ll->field_links_list_nodes)){
				foreach ($ll->field_links_list_nodes as $l) {

		            if($l->field_internal_link_nodes[0]->field_packagelayout[0]["value"]) {
		              $package_layout = $l->field_internal_link_nodes[0]->field_packagelayout[0]["value"];
		              $package_layout = taxonomy_get_term($package_layout);
		              $package_layout_name = $package_layout->name;
		            }

					// teste s'il s'agit d'un lien interne
					if($l->field_internal_link[0]["nid"] != NULL) {
						$nodeTarget = node_load($l->field_internal_link[0]["nid"]);
		                $lLinks["links"][$i]["internal"] = 1;
						$lLinks["links"][$i]["title"] = $nodeTarget->title;
		                $lLinks["links"][$i]["target"] = '';
		                $lLinks["links"][$i]["status"] = $l->status;
						$lLinks["links"][$i]["url"] = "/".drupal_get_path_alias("node/".$nodeTarget->nid);
						$lLinks["links"][$i]["summary"] = $l->field_summary[0]["value"];
						if($package_layout_name) $lLinks["links"][$i]["packagelayout"] = $package_layout_name;
					} else {
						// lien vers un fichier?
						if($l->files) {
							$att = array_pop($l->files);
							$lLinks["links"][$i]["url"] = "/".$att->filepath;
							$lLinks["links"][$i]["title"] = $l->title;
							$lLinks["links"][$i]["summary"] = $l->field_summary[0]["value"];
						} else {
							// lien externe
						  $lLinks["links"][$i]["url"] = $l->field_link_item[0]["url"];
						  if (isset($l->field_link_item[0]["title"]) && ($l->field_link_item[0]["title"])!="" ) {
							$lLinks["links"][$i]["title"] = $l->field_link_item[0]["title"];
						  } else {
							$lLinks["links"][$i]["title"] = $l->title;
						  }
						  if ($l->field_link_item[0]["attributes"]["target"] === 0){
						    $lLinks["links"][$i]["target"] = '';
						  } else {
						    $lLinks["links"][$i]["target"] = $l->field_link_item[0]["attributes"]["target"];
						  }
						}
						$lLinks["links"][$i]["summary"] = $l->field_summary[0]["value"];
						$lLinks["links"][$i]["status"] = $l->status;
					}
					
					$i++;
				}
				array_push($allLinks,$lLinks);
			}
		}
	}
	return $allLinks;
}

function theunfold_get_map_fromcoordinate($latitude, $longitude, $width='100%', $height='100%'){
  
  $content = '';
  $url = 'http://maps.google.com/?q='.$latitude.','.$longitude;
  $item = array('embed' => $url);
  
  $modules = array('emother');
  $emfield = FALSE;
  foreach ($modules as $module) {
    $item = _emfield_field_submit_id($field, $item, $module);
    
    if (!empty($item['provider'])) {
      $element = array(
            '#item' => $item,
            '#formatter' => 'default',
      );
      $function = $module.'_field_formatter';
      $content = $function($field, $element['#item'], $element['#formatter'], $element['#node']);
      
      $content = preg_replace('+width=("|\')([0-9]{3})?("|\')+','width="'.$width.'"', $content);
      $content = preg_replace('+height=("|\')([0-9]{3})?("|\')+','height="'.$height.'"', $content);
    }
  }
  return $content;
}


/**
* Render the html of the embedlinks elements
* */

function theunfold_preprocess_node_build_embedded_links($embed){
  $embed_links = array();
  if ($embed->type == 'wally_linktype' && isset($embed->field_link_item[0]['url']) && !empty($embed->field_link_item[0]['url']) && !strstr($embed->field_link_item[0]['url'], 'extref://')) {
    $item = array('embed' => $embed->field_link_item[0]['url']);
    if ($embed->field_link_item[0]['query'] != NULL){
      $item['embed'] .= '?'.$embed->field_link_item[0]['query'];
    }
    
    $icon = 'external';
    
    $modules = array('emvideo', 'emother', 'emimage', 'emaudio', 'embonus', 'emimport', 'eminline', 'emthumb', 'emwave', 'image_ncck', 'video_cck', 'slideshare');
    $emfield = FALSE;
    foreach ($modules as $module) {
      $item = _emfield_field_submit_id($field, $item, $module);
      if (!empty($item['provider'])) {
        $element = array(
          '#item' => $item,
          '#formatter' => 'default',
          '#node' => $node,
        );
        $function = $module.'_field_formatter';
        $content = $function($field, $element['#item'], $element['#formatter'], $element['#node']);
  
        if ((($module == "emimage" || $module == 'emvideo') && ($item['provider'] != "flickr_sets" )) | ($item['provider'] == 'googlemaps')){
          //reduction de la taille
          $width = 743;
          $height = 430;
          $content = preg_replace('+width=("|\')([0-9]{3})?("|\')+','width="'.$width.'"', $content);
          $content = preg_replace('+height=("|\')([0-9]{3})?("|\')+','height="'.$height.'"', $content);
  
        }
                
        $title = $embed->title;
  
        if ($module == 'emimage'){
          $group_type = 'photo';
          $icon = 'image';
        } elseif ($module == 'emvideo'){
          $group_type = 'video';
          $icon = 'video';
        } else {
          $group_type = 'other';
        }

        switch ($item['provider']) {
					case "googlemaps" : $icon = "map"; break; 
					case "youtube" : $icon = "youtube"; break;
					case "picasa" : $icon = "picasa"; break;
					case "flickr" : $icon = "flickr"; break;
				}
	
        
        $embed_links[$embed->nid] = array(
          'title' => $title,
          'nid' => $embed->nid,
          'content' => $content,
          'main_size' => $content,
          'thumb' => $thumb,
          'group_type' => $group_type,
          'type' => $embed->type,
          'module' => $module,
          'provider' => $item['provider'],
          'summary' => $embed->field_link_item[0]['title'],
          'icon' => $icon,
        );
        $emfield = TRUE;
        break;
      }
    }

    if (!$emfield){
      $url = $embed->field_link_item[0]['display_url'];
      
      $content = '<a href="'.$url.'">'.t('Access to the link').'</a><iframe src="'.$url.'" width="100%" height="600" style="border: none;"></iframe>';
      $title = $embed->field_link_item[0]['title'];
      $thumb = "";
      $module = "";
      $provider = "";
      $embed_links[$embed->nid] = array(
        'title' => $title,
        'nid' => $embed->nid,
        'content' => $content,
        'thumb' => $thumb,
        'group_type' => 'document',
        'type' => $embed->type,
        'module' => $module,
        'provider' => $provider,
      	'icon' => $icon,
      );
    }
  } elseif ($embed->field_internal_link[0]['nid'] != NULL){
    //Link item to a package
    $package = node_load($embed->field_internal_link[0]['nid']);
    wallycontenttypes_packagepopulate($package);
    $photo_object = NULL;
    $mainobject = NULL;
    $text = '';
    if ($package->type == 'wally_articlepackage'){
      $icon = 'memo';
      $mainobject = $package->field_mainstory_nodes[0];
      node_build_content($mainobject);
      $text = $mainobject->field_textbody[0]['safe'];
      foreach ($package->field_embededobjects_nodes as $package_embeds){
        if ($package_embeds->type == 'wally_photoobject'){
          $photo_object[] = $package_embeds;
          break;
        }
      }
    } elseif ($package->type == 'wally_pollpackage'){
      $icon = 'graph';
      $mainobject = $package->field_mainpoll_nodes[0];
      $text = $package->field_summary[0]['safe'];
      foreach ($package->field_embededobjects_nodes as $package_embeds){
        if ($package_embeds->type == 'wally_photoobject'){
          $photo_object[] = $package_embeds;
        }
      }
    } elseif ($package->type == 'wally_gallerypackage'){
      $icon = 'images';
      $mainobject = $package->field_mainobject_nodes[0];
      $text = $package->field_summary[0]['safe'];
      $photo_object[] = $mainobject;
      foreach ($package->field_embededobjects_nodes as $package_embeds){
        if ($package_embeds->type == 'wally_photoobject'){
          $photo_object[] = $package_embeds;
        }
      }
    }
    
    
    $embed_links[$embed->nid] = array(
      'title' => $mainobject->title,
      'content' => '<a href="/'.drupal_get_path_alias('node/'.$package->nid).'">'.t('Access to the link').'</a>',
      'package' => $package,
      'mainobject' => $mainobject,
      'photo_object' => $photo_object,
      'group_type' => 'package',
      'text' => $text,
      'icon' => $icon,
    	
    );
  }
  return $embed_links;
}
/**
* Render the html of the text elements
* */
function theunfold_preprocess_node_build_embedded_text($embed){
  $embed_texts = array();
  if ($embed->type == 'wally_textobject'){
    $embed_texts[$embed->nid] = array(
      'title' => $embed->title,
      'package' => NULL,
      'mainobject' => $embed,
      'group_type' => 'text',
      'photo_object' => NULL,
      'text' => $embed->field_textbody[0]['safe'],
    	'icon' => 'memo',
    
    );
  }
  return $embed_texts;
}
/**
* Render the html of the photo elements
* */
function theunfold_preprocess_node_build_embedded_photos($embed){
  
  $embed_photos = array();

  if ($embed->type == 'wally_photoobject') {

    $photo_infos = theunfold_get_photo_infos_and_display($embed);
    $title = $photo_infos['title'];
    $thumb = $embed->field_thumbnail[0]['filepath'];
    $content = $photo_infos["main_size"];
    $module = '';
    $provider = '';
    $embed_photos[$embed->nid] = array(
      'title' => $title,
      'nid' => $embed->nid,
      'content' => $content,
      'thumb' => $thumb,
      'group_type' => 'photo',
      'type' => $embed->type,
      'module' => $module,
      'provider' => $provider,
      'summary' => $embed->field_summary[0]['value'],
    	'icon' => 'image',
    
    );
  }
  return $embed_photos;
}
/**
* Render the html of the video elements
* */
function theunfold_preprocess_node_build_embedded_videos($embed){
  $embed_videos = array();

  if ($embed->type == 'wally_videoobject') {
    //Petite astuce, on passe par le provider l'url de l'audio comme si elle était externe
    $url = 'http://'.$_SERVER['HTTP_HOST'].'/'.$embed->field_videofile[0]['filepath'];
          
    $item = array('embed' => $url);
    $modules = array('emvideo');
    $emfield = FALSE;
    foreach ($modules as $module) {
      $item = _emfield_field_submit_id($field, $item, $module);
      if (!empty($item['provider'])) {    
        $element = array(
          '#item' => $item,
          '#formatter' => 'default',
          '#node' => $node,
        );
        $function = $module.'_field_formatter';
          
        $content = $function($field, $element['#item'], $element['#formatter'], $element['#node']);
          
        $title = $embed->title;
        $thumb = $embed->field_thumbnail[0]['filepath'];
        $group_type = 'video';
          
        $embed_videos[$embed->nid] = array(
          'title' => $title,
          'nid' => $embed->nid,
          'content' => $content,
          'main_size' => $content,
          'thumb' => $thumb,
          'group_type' => $group_type,
          'type' => $embed->type,
          'module' => $module,
          'provider' => $item['provider'],
          'summary' => $embed->field_summary[0]['value'],
          'copyright' => $embed->field_copyright[0]['value'],
          'link' => $url,
        	'icon' => 'video',
        
        );
      }
    }
  }
  return $embed_videos;
}
/**
* Render the html of the documents elements
* */
function theunfold_preprocess_node_build_embedded_documents($embed){
  $embed_documents = array();
  if ($embed->type == 'wally_digitalobject') {

    $width = '100%';
    $height = '900';
    $url = url($embed->field_objectfile[0]["filepath"], array('absolute'=>TRUE));
    $content = '<iframe src="http://docs.google.com/viewer?url='.$url.'&embedded=true" width="'.$width.'" height="'.$height.'" style="border: none;"></iframe>';
    $title = $embed->title;
    $thumb = $embed->field_thumbnail[0]['filename'];
        
    $embed_documents[$embed->nid] = array(
      'title' => $title,
      'nid' => $embed->nid,
      'emcode' => $content,
      'content' => $content,
      'thumb' => $thumb,
      'group_type' => 'document',
      'type' => $embed->type,
      'module' => $module,
      'provider' => $provider,
      'summary' => $embed->field_summary[0]['value'],
      'copyright' => $embed->field_copyright[0]['value'],
      'link' => $url,
    	'icon' => 'memo',
    
    );
  }
  return $embed_documents;
}
/**
 * Render the html of the audio elements
 * */
function theunfold_preprocess_node_build_embedded_audios($embed){
  $embed_audios = array();
  
  if ($embed->type == 'wally_audioobject') {
    //Petite astuce, on passe par le provider l'url de l'audio comme si elle était externe
    $url = 'http://'.$_SERVER['HTTP_HOST'].'/'.$embed->field_audiofile[0]['filepath'];
    
    $item = array('embed' => $url);
    $modules = array('emvideo', 'emother', 'emimage', 'emaudio', 'embonus', 'emimport', 'eminline', 'emthumb', 'emwave', 'image_ncck', 'video_cck', 'slideshare');
    $emfield = FALSE;
    foreach ($modules as $module) {
      $item = _emfield_field_submit_id($field, $item, $module);
      if (!empty($item['provider'])) {
        $element = array(
          '#item' => $item,
          '#formatter' => 'default',
          '#node' => $node,
        );
        $function = $module.'_field_formatter';

        $content = $function($field, $element['#item'], $element['#formatter'], $element['#node']);
        $title = $embed->title;
        $thumb = $embed->field_thumbnail[0]['filepath'];
        $group_type = 'audio';

        $embed_audios[$embed->nid] = array(
          'title' => $title,
          'nid' => $embed->nid,
          'content' => $content,
          'main_size' => $content,
          'thumb' => $thumb,
          'group_type' => $group_type,
          'type' => $embed->type,
          'module' => $module,
          'provider' => $item['provider'],
          'summary' => $embed->field_summary[0]['value'],
          'copyright' => $embed->field_copyright[0]['value'],
          'link' => $url,
          'icon' => "music",
        );    
        break;
      }
    }
  }
  return $embed_audios; 
}
/**
 * Rewrite the theme function of media player audio
 * */
function theunfold_emaudio_flowplayer_flash($url = NULL, $width = "100%", $height = 0, $field = NULL, $data = array(), $node = NULL, $autoplay = FALSE) {
  // Display the audio using Flowplayer if it's available.
  if (module_exists('flowplayer')) {
    $config = array(
      'clip' => array(
        'autoPlay' => $autoplay,
        'url' => url($url, array('absolute' => TRUE)),
    ),
    );
    $attributes = array(
      'style' => "width:{$width};height:24px;",
    );
    return theme('flowplayer', $config, 'emaudio_flowplayer_flash', $attributes);
  }

  // Display the custom URL using the embed tag.
  switch($data['data']['type']) {
    case 'wav':
    case 'ra':
    case 'mp3':
    case 'mid':
      $autoplay = $autoplay ? 'true' : 'false';
      if (!preg_match('|^http(s)?://[a-z0-9-]+(.[a-z0-9-]+)*(:[0-9]+)?(/.*)?$|i', $url)){
        $url = '/'.$url;
      }
      return "<embed type='application/x-shockwave-flash' flashvars='audioUrl=$url' src='http://www.google.com/reader/ui/3523697345-audio-player.swf' width='100%' height='27' quality='best' autostart='$autoplay'></embed>";
  }
}


function theunfold_wallyct_flowmix($subtype, $context, $feed, $options){
  
  $widget['title'] = $options['title'];
  $widget['link'] = $options['url'];
  $widget['links'] = array();
  foreach ($feed as $item){
    
    switch ($options['flows'][$item['__flow_id']]->plugin_name){
      case 'destinations':
        if ($options['flows'][$item['__flow_id']]->conf['render_node'] == 'node'){
          //node case
          $widget['links'][] = theunfold_preprocess_flowmix_destination_default($item);
        } else {
          $widget['links'][] = theunfold_preprocess_flowmix_default($item);
        }
        break;
      default:
        $widget['links'][] = theunfold_preprocess_flowmix_default($item);
    }
  }
  $content = '';
  if (count($widget['links']) > 0){
    $content = theme('widget_linkslist', $widget);
  }
  return $content;
}
function theunfold_theme_flowmix_image($subtype, $context, $feed, $options){
  
  $title = $options['title'];
  $link = $options['url'];
  $flow = array();
  foreach ($feed as $item){
    switch ($options['flows'][$item['__flow_id']]->plugin_name){
      case 'destinations':
        if ($options['flows'][$item['__flow_id']]->conf['render_node'] == 'node'){
          //node case
          $flow[] = theunfold_preprocess_flowmix_destination_image($item);
        } else {
          $flow[] = theunfold_preprocess_flowmix_image($item);
        }
        break;
      default:
        $flow[] = theunfold_preprocess_flowmix_image($item);
    }
  }
  $content = '';
  if (count($flow) > 0){
    $content = theme('theunfold_flowmix_image', $title, $link, $flow);
  }
  return $content;
}
/**
 * Return values for a default flowmix
 */
function theunfold_preprocess_flowmix_default($item){
  $uri = '';
  if ($item['ExternalURI']['value'] != ''){
    //external
    $uri = $item['ExternalURI']['value'];
  } elseif (isset($item['nid'])){
    //internal
    $uri = '/'.drupal_get_path_alias('node/'.$item['nid']);
  }
  $item = array(
    'title' => $item['PackageTitle']['value'],
    'link'  => $uri,
  );
  return $item; 
}
/**
 * Return values for image flowmix
 */
function theunfold_preprocess_flowmix_image($item){
  $item_values = theunfold_preprocess_flowmix_default($item);
  $item_values['description'] = $item['MainStory']['TextBody']['value'];
  if ($item['EmbeddedContent'] != NULL && $item['EmbeddedContent']['EmbeddedObjects']['Object'][0] != NULL){
    if ($item['EmbeddedContent']['EmbeddedObjects']['Object'][0]['type'] == 'PhotoObjectType' &&
        $item['EmbeddedContent']['EmbeddedObjects']['Object'][0]['LocaleImage'] != NULL){
      $item_values['img'] = $item['EmbeddedContent']['EmbeddedObjects']['Object'][0]['LocaleImage']['filepath'];
    }
  }
  return $item_values;
}
/**
 * Return values for a default flowmix desitnation
 */
function theunfold_preprocess_flowmix_destination_default($item){
  $item = array(
    'title' => $item['PackageTitle']['value'],
    'link'  => '/'.drupal_get_path_alias('node/'.$item['node']->nid),
  );
  return $item;
}
function theunfold_preprocess_flowmix_destination_image($item){
  $item_values = theunfold_preprocess_flowmix_image($item);
  $item_values['description'] = theunfold_get_summary($item['node']->field_mainstory_nodes[0]);
  if ($item['node']->field_embededobjects_nodes[0] != NULL 
   && $item['node']->field_embededobjects_nodes[0]->type == 'wally_photoobject'){
   $item_values['img'] = $item['node']->field_embededobjects_nodes[0]->field_photofile[0]['filepath'];
  }
  return $item_values;
}
/**
 * Get the summary of a mainstory
 * */
function theunfold_get_summary($mainstory, $lenght = 200){
  if ($mainstory->field_textchapo[0]['value'] != NULL){
    $summary = $mainstory->field_textchapo[0]['value'];
  } else {
    $summary = theunfold_trimmed_string(check_plain(strip_tags($mainstory->field_textbody[0]['value'])), $lenght);
  }
  return $summary;
}
/**
 * hook_preprocess_panels_pane
 * */
function theunfold_preprocess_panels_pane(&$vars){
  
  // Node panel special actions. 
  $pane = &$vars["pane"];
  if ( $pane->type=="node" && !$pane->configuration['override_title']) {
    $vars['title'] = '';
  }
  
  //Delete panel title for flowmix
  if ($vars['pane']->type == 'flowmix'){
    $vars['title'] = '';
  }
  
  
}
/**
 * Cut string
 * 
 * */
function theunfold_trimmed_string($str, $size, $nocut_words=NULL) {

  if ($nocut_words == NULL) {
    $strapline = truncate_utf8($str, $size);

    // Store the actual length of the UTF8 string -- which might not be the same
    // as $size.
    $max_rpos = strlen($strapline);

    // How much to cut off the end of the strapline so that it doesn't end in the
    // middle of a paragraph, sentence, or word.
    // Initialize it to maximum in order to find the minimum.
    $min_rpos = $max_rpos;

    // Store the reverse of the strapline.  We use strpos on the reversed needle and
    // haystack for speed and convenience.
    $reversed = strrev($strapline);

    // Build an array of arrays of break points grouped by preference.
    $break_points = array();

    // A paragraph near the end of sliced strapline is most preferable.
    $break_points[] = array('</p>' => 0);

    // If no complete paragraph then treat line breaks as paragraphs.
    $line_breaks = array('<br />' => 6, '<br>' => 4);
    // Newline only indicates a line break if line break converter
    // filter is present.
    if (isset($filters['filter/1'])) {
      $line_breaks["\n"] = 1;
    }
    $break_points[] = $line_breaks;

    // If the first paragraph is too long, split at the end of a sentence.
    $break_points[] = array('. ' => 1, '! ' => 1, '? ' => 1, '。' => 0, '؟ ' => 1);

    // Iterate over the groups of break points until a break point is found.
    foreach ($break_points as $points) {
      // Look for each break point, starting at the end of the strapline.
      foreach ($points as $point => $offset) {
        // The teaser is already reversed, but the break point isn't.
        $rpos = strpos($reversed, strrev($point));
        if ($rpos !== FALSE) {
          $min_rpos = min($rpos + $offset, $min_rpos);
        }
      }

      // If a break point was found in this group, slice and return the strapline.
      if ($min_rpos !== $max_rpos) {
        // Don't slice with length 0.  Length must be <0 to slice from RHS.
        return ($min_rpos === 0) ? $strapline : substr($strapline, 0, 0 - $min_rpos);
      }
    }
  } else {
    $strapline = truncate_utf8($str, $size, " ");
    $strapline = wordwrap($strapline, $size);
  }

  // If a strapline was not found, still return a teaser.
  if($strapline) $strapline .=" [...]";

  return $strapline;
}
