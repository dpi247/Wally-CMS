<?php

/**
 *  Implémentation du hook_theme(); 
 */
function wallytheme_theme(&$var) {

  $path = drupal_get_path('theme', 'wallytheme');
  $base = array(
    'file' => 'theme.inc',
    'path' => "$path/templates",
  );
  
 return array(
 
    'soirmag_footer' => $base + array(
    'arguments' => array("data_array"=>array()),
    'template' => 'footer',
    ),
    
    'soirmag_search_box' => $base + array(
    'arguments' => array(),
    'template' => 'search_box',
    ),
    
   	'soirmag_meta_og' => $base + array(
    'arguments' => array(),
    'template' => 'meta_og',
    ),
    
   	'soirmag_header' => $base + array(
    'arguments' => array("data_array"=>array()),
    'template' => 'header',
    ),
    
   	'soirmag_header_logo' => $base + array(
    'arguments' => array("data_array"=>array()),
    'template' => 'header_logo',
    ),    
    
   	'soirmag_header_meteo' => $base + array(
    'arguments' => array(),
    'template' => 'header_meteo',
    ),

   	'soirmag_header_social_bookmarks' => $base + array(
    'arguments' => array(),
    'template' => 'header_social_bookmarks',
    ),
    
    'soirmag_rss_sudpresse_crossmedia' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "feed" => NULL, "options" => NULL),
    'template' => 'rss_sudpresse_crossmedia',
    ),  

    'soirmag_rss_mix_all_blogs' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "feed" => NULL, "options" => NULL),
    'template' => 'rss_mix_all_blogs',
    ),      

    'soirmag_rss_galeries_g2' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "feed" => NULL, "options" => NULL),
    'template' => 'rss_galeries_g2',
    ),  
    
    'soirmag_rss_mix_high_tech' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "feed" => NULL, "options" => NULL),
    'template' => 'rss_mix_high_tech',
    ), 
     
    'soirmag_rss_mad' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "feed" => NULL, "options" => NULL),
    'template' => 'rss_mad',
    ),  
    
    'soirmag_rss_mix_last_blogs' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "feed" => NULL, "options" => NULL),
    'template' => 'rss_mix_last_blogs',
    ),  
    'soirmag_rss_mix_fil_info' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "feed" => NULL, "options" => NULL),
    'template' => 'rss_mix_fil_info',
    ),  
    
  );
}

/**
 * Override or insert PHPTemplate variables into the templates.
 */
function wallytheme_preprocess_page(&$vars) {
  $vars['sf_primarymenu'] = theme("wallyct_mainmenu", 'primary-links', 'menu-primary-links');
  $vars['sf_secondarymenu'] = theme("wallyct_mainmenu", 'secondary-links', 'menu-secondary-links');
  $vars['scripts'] = drupal_get_js();
  $vars['styles'] = drupal_get_css();
  $vars['theme_path'] = base_path() .'/'. path_to_theme();
}

/**
 * Generates IE CSS links for LTR and RTL languages.
 */
function wallytheme_get_ie_styles() {
}


/**
 * Render the differents date displays
 * 
 * @param $unix_time
 *   
 * @param $display
 */
function wallytheme_date_edition_diplay($unix_time, $display){
	$format = array(
	 'default_destination_view' => "l d F Y, H<abbr title=\"heure\">\h</abbr>i"
	);
	$displayed_date = date($format[$display],$unix_time);
	return $displayed_date;
}


/**
 * Try to find the package's first photoObject. 
 * If not, return false
 * 
 */
function wallytheme_get_first_photoEmbededObject_from_package($embededObjects_array){	
	foreach($embededObjects_array as $embededObject){  
	 if ($embededObject->type == "wally_photoobject"){
	   $photoObject = $embededObject;
	   break;
	 }
	}
  if (isset($photoObject)){
    return $photoObject; 
  }else{
    return false;
  }
}

/**
 * Extract the strapline from an Article
 * If the article's chapofield is empty, return a teaser
 * 
 * @param $node
 *  package
 * 
 * @param $mainstory
 *  mainstory object extracted from the package
 * 
 * @param $size
 *  max size for the strapline. 
 *  If no max size is wanted, indicate 0 as value
 * 
 */

function wallytheme_get_strapline($mainstory, $node, $size){
  $strapline_field = $mainstory->field_textchapo[0]['value'];
	 
  if ($strapline_field != "" && $size == 0){
	  return $strapline_field;
	}
  
	if ($strapline_field != "" && drupal_strlen($strapline_field) <= $size) {
    return $strapline_field;
  }
  
  $strapline = truncate_utf8($strapline_field, $size);
 
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

  // If a strapline was not found, still return a teaser.
    
  if ($strapline == ""){
    $teaser_length = $size;
    $teaser = theme("wallyct_teaser", $mainstory->field_textbody[0]['value'], $teaser_length, $node);
    $strapline = $teaser;
  }
   $strapline .=" [...]";
   return $strapline;
}

/**
 * 
 * Display the Facebook like button
 * @param $node_path
 */
function wallytheme_social_bookmarking_facebook_like($node_path){ 
	$button_html = '<script src="http://connect.facebook.net/fr_FR/all.js#xfbml=1"></script>
  <fb:like href="'.$node_path.'" show_faces="false" layout="button_count"></fb:like>';
	return $button_html ;
}

/**
 * Get nid from packages who reference the current node
 * 
 * @param $node
 *   Node currently processed(TextObject, AudioObject, DigitalObject,PhotoObject, Videoobject, PollObject)
 *
 * @return
 *   Array() of array() of nid
 */
function wallytheme_getpackagesid($node){
  $rows = array(); 
  
  if($node->type=='wally_textobject'){
    $res=db_query("SELECT nid FROM {content_type_wally_articlepackage} WHERE field_mainstory_nid = %d", $node->nid);
    while ($row = db_fetch_array($res)) {
      if (!in_array($row, $rows)) $rows[] = $row;
    }
  }
  elseif($node->type=='wally_audioobject' || $node->type=='wally_digitalobject' || $node->type=='wally_photoobject' || $node->type=='wally_videoobject'){   
     if ($res=db_query("SELECT nid FROM {content_field_embededobjects} WHERE field_embededobjects_nid = %d", $node->nid)){
      while ($row = db_fetch_array($res)) {
        if (!in_array($row, $rows)) $rows[] = $row;
      }
    }else
    if ($res2=db_query("SELECT nid FROM {content_type_wally_gallerypackage} WHERE field_mainobject_nid = %d", $node->nid)){
      while ($row = db_fetch_array($res2)) {
        if (!in_array($row, $rows)) $rows[] = $row;
      }
    }
  }
  elseif($node->type=='wally_pollobject'){
    $res=db_query("SELECT nid FROM {content_type_wally_pollpackage} WHERE field_mainpoll_nid = %d", $node->nid);
    while ($row = db_fetch_array($res)) {
      if (!in_array($row, $rows)) $rows[] = $row;
    }
  }
  return $rows;
}


/**
 * 
 * Order an array of Wally packages' id
 * @param $array
 *  array() of array() of packages's nid
 * @param $sort
 *  the key to sort the nid
 * @param $order
 *  ASC or DESC
 * @return Array() of array() ordered $out's values
 */

function wallytheme_order_packages($array,$sort,$order){
	$count = count($array);
  $cpt = 1;
  $rows = array();
	$query = "SELECT nid,".$sort." FROM node WHERE";
  foreach($array as $row){
    $query .= " nid=".$row['nid']."";
    if ($cpt<$count){
     $query .= " OR";
    }
    $cpt++;
  }
  $query .= " ORDER BY ".$sort." ".$order."";
  $res=db_query($query);
  while ($row = db_fetch_array($res)) {
    if (!in_array($row, $rows)) $rows[] = $row;
  }
  return $rows;   
}

/**
 * 
 * Display a "see also" html list from an packages' id array
 * Used in the taxonomy term list who is showing wallyObjects 
 * @param $packages_id
 *  Array() of array() packages' id
 *  
 *  @return
 *    htlm list
 */
function wallytheme_display_taxonomy_term_list_see_also($packages_id){
  $html = "<h3>Voir aussi</h3><ul class=\"see_also_taxo_term_list\">";
  $cpt = 1;
	foreach($packages_id as $package_id){
    if ($cpt > 1){
		  $node = node_load($package_id['nid']);
      $node_path = drupal_get_path_alias("node/".$node->nid);
      $html .= "<li><a href=\"/".$node_path."\">".$node->title."</a></li>";     
    }
    $cpt++;
  }
  $html .= "</ul>";
  return $html;
}

/**
 * Retrieve tags from an article package node to display with the correct soirmag format
 *  
 * @param $node
 *  $node which should be a Wally Article Package
 *  
 *  @return string list of tags formated as <a href="[TAXONOMY_PAGE]" class="[SAFE_CLASS_NAME]">[TERM_NAME]</a>, <a href.....
 */
function wallytheme_taxonomy_tags_particle($main_story){

					$vocabulary = taxonomy_get_vocabularies();
					$voclass = array();
					foreach ($vocabulary as $key => $value){
						$voclass[$key] = preg_replace('/[^a-z0-9]+/', '_', strtolower($value->name));
					}
					$cpt = 1;
					$htmltags = "";
					foreach($main_story->taxonomy as $termclass){
						if($cpt != 1){
							
							$htmltags .= ", ";
						}
					
						$htmltags .= "<a href=\"".url(taxonomy_term_path(taxonomy_get_term($termclass->tid)))."\" class=\"".$voclass[$termclass->vid]."\">".$termclass->name."</a>";
						$cpt++;					
					}
					return $htmltags;
}


function _wallytheme_rss_crossmedia_gen($feed) {
  $content = "";
  $cpt = 0;
  foreach ($feed as $k=>$item) {
    if ($cpt == 0){
    	$item['image_path'] = $item['Package']['EmbeddedContent']['EmbeddedObjects']['Object'][0]['LocaleImage']['filepath'];
      $content .= "<li>";
      if ($item['image_path'] != ""){
        $item['img'] = theme('imagecache', 'sm_crossmedia_thumb', $data['image_path']); 
        $content .="<a href='".$item['Package']['ExternalURI']['value']."'>
               ".$item['img']."
              </a>";
      }
      $content .="<h3><a href='".$item['Package']['ExternalURI']['value']."'>".check_plain($item['Package']['MainStory']['Title']['value'])."</a></h3>
             </li>";
    }else{
      $content .= "<li><a href='".$item['Package']['ExternalURI']['value']."'>".check_plain($item['Package']['MainStory']['Title']['value'])."</a></li>";        
    }
    $cpt++;
  }
  return $content; 
}