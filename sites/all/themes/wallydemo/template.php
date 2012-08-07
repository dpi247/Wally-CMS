<?php
/*
 * Fonction temporaire
 * A supprimer!
 * 
 */
function wallydemo_preprocess_views_view(&$vars){

 $funcs=_views_theme_functions('views_view', $vars['view'], $vars['view']->display[$vars['view']->current_display]);
 $new_funcs=array();
 foreach($funcs as $func){
     if($func!='views_view__' and isset($vars['view']->split_index_count)){
       $needle=str_replace('views_view','',$func);
       if($needle==''){
         $new_funcs[]='views_view__splitted_redacblock';
         if($vars['view']->split_index_count==1);
           $new_funcs[]='views_view__splitted_redacblock__first';
         if($vars['view']->split_index_count==$vars['view']->split_total_count);
           $new_funcs[]='views_view__splitted_redacblock__last';
         
       }
       else{ 
          $new_funcs[]='views_view__splitted_redacblock'.$needle;
         if($vars['view']->split_index_count==1);
           $new_funcs[]='views_view__splitted_redacblock__first'.$needle;
         if($vars['view']->split_index_count==$vars['view']->split_total_count);
           $new_funcs[]='views_view__splitted_redacblock__last'.$needle;
         
       }
     }
   }
  $result = array_merge($new_funcs,$funcs);
  $vars['template_files'] = array_merge($result,$vars['template_files']);
}

function wallydemo_preprocess_spnewsletter_html_form(&$vars){
  $messages = drupal_get_messages();
  if(count($messages['status']) > 0) $messages = $messages['status'][0];
  else $messages = $messages['error'][0];
  $vars['messages'] = $messages;
}
function wallydemo_preprocess_spscoop_html_form(&$vars){
  $messages = drupal_get_messages();
  if(count($messages['status']) > 0) $messages = $messages['status'][0];
  else $messages = $messages['error'][0];
  $vars['messages'] = $messages;
}


function wallydemo_preprocess_page(&$vars){

  $domain_url = $_SERVER["SERVER_NAME"];
  $domain = 'sudinfo';
  
  module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
  //ajoute un candidat template utilisé pour le contexte mobile
  if ($domain == "mobile"){
    $vars['template_files'][] = 'page-mobile';
  }
  
  $site_name = variable_get($domain.'_site_name', NULL);
  $site_url = variable_get($domain.'_site_url', NULL);
  $associated_brand = variable_get($domain.'_associated_brand', NULL);
  $current_path = wallydemo_get_current_path();
  module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
  $current_path_alias = wallytoolbox_get_all_aliases($current_path);  
  $vars['head'] = _set_meta_general($site_name, $site_url, $associated_brand, $domain);  
	$args = arg();
	if($args[0] == 'node'){
		$default_site_name = variable_get('site_name','Wally Sudpresse');
		$vars['head_title'] = str_replace(" | ".$default_site_name," - ".$site_url,$vars['head_title']);
    $tid = $vars['node']->field_destinations[0]['tid'];
		$body_classes = wallydemo_get_trimmed_taxonomy_term_path($tid);
		$vars['body_classes'] .= $body_classes;
		$vars['head'] .= _set_meta_fornode($vars["node"], $site_name, $site_url, $associated_brand, $domain, $current_path_alias);

	} else if($args[0] == 'meteo') {  
	  $page = page_manager_get_current_page();
		$default_site_name = variable_get('site_name','Wally Sudpresse');
		$vars['head'] .= _set_meta_formeteo($vars, $args, $page, $site_name, $site_url, $associated_brand, $domain, $current_path_alias);
	} else if($args[1] == 'carburant') {  
	  $page = page_manager_get_current_page();
		$default_site_name = variable_get('site_name','Wally Sudpresse');
		$vars['head'] .= _set_meta_forcarburant($vars, $args, $page, $site_name, $site_url, $associated_brand, $domain, $current_path_alias);			  
	}  else {
	  $page = page_manager_get_current_page();
	  switch($page['name']) {
	   case "term_view":
	     $tid = $page['contexts']['argument_terms_1']->data->tid;
	     $term = taxonomy_get_term($tid);
	     $vars['head_title'] = str_replace("/", " - ", wallytoolbox_taxonomy_get_path_by_tid_or_term($term->tid,2))." - ".$site_url;
       $body_classes = wallydemo_get_trimmed_taxonomy_term_path($term->tid);
       $vars['body_classes'] .= $body_classes;
       $vars['head'] .= _set_meta_fortaxonomies($page, $term, $site_name, $site_url, $associated_brand, $domain,$current_path_alias); 
	     //ajoute un candidat template utilisé pour les pages HDA
	     //if($tid == 287){
	       //$vars['template_files'][] = 'page-hda';
	     //}
	     break;
	   default :
       $vars['head_title'] = $page['subtask']['admin title']." - ".$site_url;	   	
	   	 $vars['head'] .= _set_meta_forpages($page, $site_name, $site_url, $associated_brand, $domain, $current_path_alias); 
	  }
  }
  $vars['head_title'] = strip_tags(html_entity_decode($vars['head_title']));
  
  $vars['SPmenutop'] .= '<div id="connect-overlay" style="display:none;"></div>';
  $vars['SPmenutop'] .= '<div id="connect-box" style="display:none;">'.'coucou'.'</div>';
}

function wallydemo_preprocess_sp_block_foot_regional(&$vars) {
  drupal_add_js(drupal_get_path('theme', 'wallydemo') . '/scripts/timer.js', 'theme'); 
  drupal_add_js(drupal_get_path('theme', 'wallydemo') . '/scripts/foot.js', 'theme'); 
}


function _set_meta_general($site_name=NULL,$site_url=NULL,$associated_brand=NULL,$domain=NULL){
  
  $site_robots = variable_get('general_site_default_robots',NULL);
  $pattern_owner = variable_get('general_site_default_owner',NULL);
  $pattern_author = variable_get('general_site_default_author',NULL);   
  $html = ""; 
  if($pattern_author){
    $site_author = str_replace("[site-name]",$site_name,$pattern_author);
    $site_author = str_replace("[site-url]",$site_url,$site_author);
    $site_author = str_replace("[associated-brand]",$associated_brand,$site_author);
    $html .= "<meta name=\"author\" content=\"".$site_author."\" />\n";
  } 
  if($pattern_owner){    
    $site_owner = str_replace("[site-name]",$site_name,$pattern_owner);
    $site_owner = str_replace("[site-url]",$site_url,$site_owner);
    $site_owner = str_replace("[associated-brand]",$associated_brand,$site_owner);
    $html .= "<meta name=\"owner\" content=\"".$site_owner."\" />\n";
  }
  
  $html .= "<meta name=\"google-site-verification\" content=\"D9bx_DMB7JV6j259foRzkilqRdQONm1vAv5qiTgmlnA\" />\n";
  
  /*
   * L'url complète des pages d'accueil ne doit pas être indexée par google
   */
  $current_path = $_SERVER['REQUEST_URI'];  
  $uneAlias1 = "/".drupal_get_path_alias('taxonomy/term/1');
  $uneAlias2 = "/".drupal_get_path_alias('taxonomy/term/2');
  $uneAlias3 = "/".drupal_get_path_alias('taxonomy/term/3');
  $uneAlias4 = "/".drupal_get_path_alias('taxonomy/term/4');
  $uneAlias5 = "/".drupal_get_path_alias('taxonomy/term/5');
  $uneAlias6 = "/".drupal_get_path_alias('taxonomy/term/6'); 
  if($current_path == $uneAlias1 || $current_path == $uneAlias2 || $current_path == $uneAlias3 || $current_path == $uneAlias4 || $current_path == $uneAlias5 || $current_path == $uneAlias6 ){
      $html .= "<meta name=\"robots\" content=\"noindex, follow, noarchive\" />\n";
  }else{
    if($site_robots){
      $html .= "<meta name=\"robots\" content=\"".$site_robots."\" />\n";
    }
  }

  //gestion favicon
  $html .="<link rel=\"icon\" type=\"image/png\" href=\"/".drupal_get_path('theme', 'wallydemo')."/images/logos/favicon_".$domain.".ico\" />\n";

  return $html;
}


/*
 * 
 */
function _set_meta_fornode($node,$site_name=NULL,$site_url=NULL,$associated_brand=NULL,$domain=NULL,$current_path=NULL) {

	$html = "";
	$node_destination_name = "";
	$node_destination_tid = "";
  $isPackage = FALSE;
  if($node->type == "wally_articlepackage" || $node->type == "wally_gallerypackage"){
   $isPackage = TRUE;
  }
	$photo = FALSE;
	if(isset($node->field_mainstory) || isset($node->field_mainobject)){
		if($node->type == "wally_articlepackage"){	
		  $mainstory = $node->field_mainstory[0];  
		  $mainstory = node_load($mainstory);
		} else {  
		  $mainstory = $node->field_mainobject[0];
		  $mainstory = node_load($mainstory);
		  $mainstory_type = $mainstory->type;
		  if($mainstory_type == "wally_photoobject"){ 
		    $photoObject_path = $mainstory->field_photofile[0]['filepath'];
		    $explfilepath = explode('/', $photoObject_path);
		    $photoObject_size == $mainstory->field_photofile[0]['filesize'];
		    if (isset($photoObject_path) && $photoObject_size > 0) {
		      $photo = TRUE;
		    }
		  }
	  }
	}
	if (!isset($photoObject_path)){
		if(isset($node->field_embededobjects)){
		  $embeded_nid_array = $node->field_embededobjects;
		  foreach($embeded_nid_array as $nid){
		  	 $embededObject = node_load($nid);
				 if ($embededObject->type == "wally_photoobject"){
			     $photoObject = $embededObject;
			     break;
			   }
		  }
		  If ($photoObject) {
		    $photoObject_path = $photoObject->field_photofile[0]['filepath'];
		    $explfilepath = explode('/', $photoObject_path);
		    $photoObject_size = $photoObject->field_photofile[0]['filesize'];
		    if (isset($photoObject_path) && $photoObject_size > 0) {
		      $photo = TRUE;
		    }    
		  }
		}
		if($node->type == "wally_photoobject"){
      $photoObject_path = $node->field_photofile[0]['filepath'];
      $explfilepath = explode('/', $photoObject_path);
      $photoObject_size = $node->field_photofile[0]['filesize'];
      if (isset($photoObject_path) && $photoObject_size > 0) {
        $photo = TRUE;
      }  		
		}
		if($node->type == "wally_videoobject"){
		  if(isset($node->field_video3rdparty[0]['data']['thumbnail']["url"])){
		    if($node->field_video3rdparty[0]['data']['thumbnail']["url"] != ""){
			    $photoObject_path = $node->field_video3rdparty[0]['data']['thumbnail']["url"];
			    if(isset($photoObject_path)){
			      $photo = TRUE;
			    }
		    }
		  }
		}
	}
	
 if($isPackage == FALSE){
   $node_title = $node->title ;
   $mainstory = $node;
   $strapline_length = 200;
   $node_description = _wallydemo_get_strapline($mainstory,$node,$strapline_length); 
  }else{
	 $node_destination_tid = $node->field_destinations[0]['tid'];
	 $node_destination = taxonomy_get_term($node_destination_tid);
	 $node_destination_name = $node_destination->name;
	 $node_title = $mainstory->title;
	 $strapline_length = 200;
	 $node_description = _wallydemo_get_strapline($mainstory,$node,$strapline_length);
  }
  
  //$aliases = wallytoolbox_get_path_aliases("node/".$node->nid);
  module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
  $aliases = wallytoolbox_get_all_aliases("node/".$node->nid);
	$node_path = $aliases[0];
	
	$server_name = $_SERVER['SERVER_NAME'];
	if($node_destination_tid == "20" || strstr($server_name, "mobile")){
	 $server_name = "www.sudinfo.be";
	}

  if ($photo == TRUE){
    if(strstr($photoObject_path, "http://")){
      $photo_src = $photoObject_path;
    } else {
      $photo_src = "http://".$server_name."/".$photoObject_path ;
    }
  }else{
   $photo_default = variable_get($domain.'_og_default_image',NULL);
   $logo_src = variable_get('logo_'.$domain,NULL);
   $photo_src = str_replace('[site-logo]',$logo_src,$photo_default);
  } 	
	
	$html .= "<link rel=\"canonical\" href=\"http://".$server_name."/".$node_path."\"/>\n";
	
	if($node_destination_name != ""){
	$node_destination_name = strip_tags(str_replace('"', '', $node_destination_name));
	$html .= "<link rel=\"alternate\" title=\"".$node_destination_name."\" href=\"http://".$server_name."/feed/".$node_destination_tid."?format=rss\" type=\"application/rss+xml\" />\n";
	}
	$node_description = strip_tags(str_replace('"', '', $node_description));
  $html .= "<meta name=\"description\" content=\"".$node_description."\" />\n";
  $node_keywords = wallydemo_taxonomy_text_tags_particle($mainstory);
  
  //todo: jeter ça à la poubelle dés que TME est arrivé!
  if($node_keywords == ""){
  	$title_words = preg_split("+ +", $node_title);
  	$size = count($title_words);
  	$cpt = 1;
  	$keywords = array();
  	foreach($title_words as $word){
  	 if(strlen($word) > 4){
  	 	 $replace_paterns = array("l'","l’","c'","c’","d'","d’","qu'","qu’","parcequ’","parcequ'","lorsqu'","lorsqu’",":",",",".","'",'"',"’");
  	 	 $word = str_replace($replace_paterns, "", $word);
  	   if(strlen($word) > 4){
  	     $node_keywords .= $word;
         if ($cpt < $size){
          $node_keywords .= ", ";
         }
  	   }
  	 }
  	 $cpt++;
  	}
  }
  
  if($node_keywords != ""){
  	$node_keywords = strip_tags(str_replace('"', '', $node_keywords));
    $html .= "  <meta name=\"keywords\" content=\"".$node_keywords."\" />\n";
  }
  $pattern_published = variable_get('general_site_default_published',NULL); 
  if($pattern_published){
  	if($isPackage == FALSE){
  	 $unix = $node->changed;
  	}else{
  	 $unix = strtotime($node->field_publicationdate[0]['value']);
  	}
    $site_published = date($pattern_published,$unix);
    $html .= "<meta name=\"published\" content=\"".$site_published."\" />\n";
  }   
  $html .= "<meta name=\"story_id\" content=\"".$node->nid."\" />\n";
  $html .= "<!-- <meta name=\"archi_id\" content=\"t-20110710-H37CPE\" /> -->\n";
  $node_title = strip_tags(str_replace('"', '', $node_title));
  $html .= "<meta property=\"og:title\" content=\"".$node_title."\"/>\n";
  $html .= "<meta property=\"og:type\" content=\"article\"/>\n";
  $html .= "<meta property=\"og:site_name\" content=\"".$site_name."\"/>\n";
  
	$url = "http://".$server_name."/".$node_path;
  
  $html .= "<meta property=\"og:url\" content=\"".$url."\"/>\n";
  $html .= "<meta property=\"og:image\" content=\"".imagecache_create_url("article_300x200", $photoObject_path,FALSE,TRUE)."\"/>\n";
  $html .= "<meta property=\"og:description\" content=\"".$node_description."\"/>\n";
  $facebook_api = variable_get($domain.'_og_facebook_api',NULL);
  $facebook_admins = variable_get($domain.'_og_facebook_administrators',NULL);
  if($facebook_api){
    $html .= "<meta property=\"fb:app_id\" content=\"".$facebook_api."\" />\n";
  }
  if($facebook_admins){
    $html .= "<meta property=\"fb:admins\" content=\"".$facebook_admins."\" />\n";
  }
  
  
  
  return $html;
}


/**
 * Build meta tags for meteo page
 *  
 * @param $vars, 
 *  $vars is the result
 * @param $args, 
 *  $args from url
 * @param $page, 
 *  $vars from current variant
 * @param $site_name 
 * @param $site_url
 * @param $domain   
 *  
 *  @return an array with html
 */
function _set_meta_formeteo($vars, $args,$page,$site_name=NULL,$site_url=NULL,$associated_brand=NULL,$domain=NULL,$current_path=NULL) {
  
  $page_handler = $page['handler']->name; 
  $pattern_description = variable_get($page_handler.'_description',NULL);
  if($pattern_description == NULL){
   $pattern_description = variable_get('general_site_default_description',NULL);
  }
  
  $pattern_keywords = variable_get($page_handler.'_keyword',NULL);
  if($pattern_keywords == NULL){
   $pattern_keywords = variable_get('general_site_default_keywords',NULL);
  } 
  
  $pattern_published = variable_get('general_site_default_published',NULL);  
  if($pattern_description){
    $page_description = str_replace('[site-name]', $site_name, $pattern_description);
    $page_description = str_replace('[site-url]', $site_url, $page_description);
    $page_description = str_replace('[page-name]', $page_name, $page_description);
    $page_description = str_replace('[local-name]', $args[1], $page_description);
    $page_description = str_replace('[associated-brand]', $associated_brand, $page_description);
    $page_description = strip_tags(str_replace('"', '', $page_description));
    $html .= "<meta name=\"description\" content=\"".$page_description."\">\n";   
  } 
  if($pattern_keywords){   
    $page_keywords = str_replace('[site-name]', $site_name, $pattern_keywords);
    $page_keywords = str_replace('[site-url]', $site_url, $page_keywords);
    $page_keywords = str_replace('[page-name]', $page_name, $page_keywords);
    $page_keywords = str_replace('[local-name]', $args[1], $page_keywords);
    $page_keywords = str_replace('[associated-brand]', $associated_brand, $page_keywords);
    $page_keywords = strip_tags(str_replace('"', '', $page_keywords));
    $html .= "<meta name=\"keywords\" content=\"".$page_keywords."\">\n";   
  }
  if($pattern_published){
    $unix = time();
    $site_published = date($pattern_published,$unix);   
    $html .= "<meta name=\"published\" content=\"".$site_published."\" />\n";
  }  
  
  if($args[1] == '') {
    $vars['head_title'] = "La météo du jour en belgique - ".$site_url; 
  } elseif ($args[1] == 'demain') {
    $vars['head_title'] = "La météo de demain en belgique - ".$site_url;
  } elseif ($args[1] == 'apres_demain') {
    $vars['head_title'] = "La météo d'après demain en belgique - ".$site_url;
  } else {
    $vars['head_title'] = "La météo à ".$args[1]." - ".$site_url;
  }

  $server_name = $_SERVER['SERVER_NAME'];
  if(strstr($server_name, "mobile")){
   $server_name = "www.sudinfo.be";
  }
  
  $photo_default = variable_get($domain.'_og_default_image',NULL);
  $logo_src = variable_get('logo_'.$domain,NULL);
  $photo_src = str_replace('[site-logo]',$logo_src,$photo_default);
  $facebook_api = variable_get($domain.'_og_facebook_api',NULL);
  $facebook_admins = variable_get($domain.'_og_facebook_administrators',NULL);
  $vars['head_title'] = strip_tags(str_replace('"', '', $vars['head_title']));
  $html .= "<meta property=\"og:title\" content=\"".$vars['head_title']."\"/>\n";
  $html .= "<meta property=\"og:type\" content=\"article\"/>\n";
  $html .= "<meta property=\"og:site_name\" content=\"".$site_name."\"/>\n";
  $html .= "<meta property=\"og:url\" content=\"http://".$site_url."/".$current_path[0]."\"/>\n";
  $html .= "<meta property=\"og:image\" content=\"http://".$server_name."/".$photo_src."\"/>\n";
  $page_description = strip_tags(str_replace('"', '', $page_description));
  $html .= "<meta property=\"og:description\" content=\"".$page_description."\"/>\n";
  if($facebook_api){
    $html .= "<meta property=\"fb:app_id\" content=\"".$facebook_api."\" />\n";
  }
  if($facebook_admins){
    $html .= "<meta property=\"fb:admins\" content=\"".$facebook_admins."\" />\n";
  }    
    
  return $html;
}


/**
 * Build meta tags for meteo page
 *  
 * @param $vars, 
 *  $vars is the result
 * @param $args, 
 *  $args from url
 * @param $page, 
 *  $vars from current variant
 * @param $site_name 
 * @param $site_url
 * @param $domain   
 *  
 *  @return an array with html
 */
function _set_meta_forcarburant($vars, $args,$page,$site_name=NULL,$site_url=NULL,$associated_brand=NULL,$domain=NULL,$current_path=NULL) {
  
  $page_handler = $page['handler']->name; 
  $pattern_description = variable_get($page_handler.'_description',NULL);
  if($pattern_description == NULL){
   $pattern_description = variable_get('general_site_default_description',NULL);
  }
  
  $pattern_keywords = variable_get($page_handler.'_keyword',NULL);
  if($pattern_keywords == NULL){
   $pattern_keywords = variable_get('general_site_default_keywords',NULL);
  } 
  
  $pattern_published = variable_get('general_site_default_published',NULL);  
  if($pattern_description){
    $page_description = str_replace('[site-name]', $site_name, $pattern_description);
    $page_description = str_replace('[site-url]', $site_url, $page_description);
    $page_description = str_replace('[page-name]', $page_name, $page_description);
    $page_description = str_replace('[local-name]', $args[1], $page_description);
    $page_description = str_replace('[associated-brand]', $associated_brand, $page_description);
    $page_description = strip_tags(str_replace('"', '', $page_description));
    $html .= "<meta name=\"description\" content=\"".$page_description."\">\n";   
  } 
  if($pattern_keywords){   
    $page_keywords = str_replace('[site-name]', $site_name, $pattern_keywords);
    $page_keywords = str_replace('[site-url]', $site_url, $page_keywords);
    $page_keywords = str_replace('[page-name]', $page_name, $page_keywords);
    $page_keywords = str_replace('[local-name]', $args[1], $page_keywords);
    $page_keywords = str_replace('[associated-brand]', $associated_brand, $page_keywords);
    $page_keywords = strip_tags(str_replace('"', '', $page_keywords));
    $html .= "<meta name=\"keywords\" content=\"".$page_keywords."\">\n";   
  }
  if($pattern_published){
    $unix = time();
    $site_published = date($pattern_published,$unix);   
    $html .= "<meta name=\"published\" content=\"".$site_published."\" />\n";
  }  
  $vars['head_title'] = "Tous les carburants et leurs meilleurs prix - ".$site_url; 

  $server_name = $_SERVER['SERVER_NAME'];
  if(strstr($server_name, "mobile")){
   $server_name = "www.sudinfo.be";
  }  
  
  $photo_default = variable_get($domain.'_og_default_image',NULL);
  $logo_src = variable_get('logo_'.$domain,NULL);
  $photo_src = str_replace('[site-logo]',$logo_src,$photo_default);
  $facebook_api = variable_get($domain.'_og_facebook_api',NULL);
  $facebook_admins = variable_get($domain.'_og_facebook_administrators',NULL);
  $vars['head_title'] =  strip_tags(str_replace('"', '', $vars['head_title']));
  $html .= "<meta property=\"og:title\" content=\"".$vars['head_title']."\"/>\n";
  $html .= "<meta property=\"og:type\" content=\"article\"/>\n";
  $html .= "<meta property=\"og:site_name\" content=\"".$site_name."\"/>\n";
  $html .= "<meta property=\"og:url\" content=\"http://".$site_url."/".$current_path[0]."\"/>\n";
  $html .= "<meta property=\"og:image\" content=\"http://".$server_name."/".$photo_src."\"/>\n";
 	$page_description = strip_tags(str_replace('"', '', $page_description));
  $html .= "<meta property=\"og:description\" content=\"".$page_description."\"/>\n";
  if($facebook_api){
    $html .= "<meta property=\"fb:app_id\" content=\"".$facebook_api."\" />\n";
  }
  if($facebook_admins){
    $html .= "<meta property=\"fb:admins\" content=\"".$facebook_admins."\" />\n";
  }    
  
  return $html;
}

/*
 * get info from admin for taxonomies
 * or get default values for taxonomies
 */
function _set_meta_fortaxonomies($page,$term,$site_name=NULL,$site_url=NULL,$associated_brand=NULL,$domain=NULL,$current_path=NULL) {
  
  $html = "";
  $page_handler = $page['handler']->name; 
  $page_name = $page["contexts"]["argument_terms_1"]->title;
  $term_name = $term->name;
  $term_tid = $term->tid;
  $term_description = $term->description;  
  
  $server_name = $_SERVER['SERVER_NAME'];
  if(strstr($server_name, "mobile")){
   $server_name = "www.sudinfo.be";
  }
  
  //$html .= "<link rel=\"alternate\" title=\"".$term_name."\" href=\"http://".$server_name."/rss/".$term_name."/rss.xml\" type=\"application/rss+xml\" />\n";
  
  $html .= "<link rel=\"alternate\" title=\"".$term_name."\" href=\"http://".$server_name."/feed/".$term_tid."?format=rss\" type=\"application/rss+xml\" />\n";
  
  $pattern_description = variable_get($page_handler.'_description',NULL);
  if($pattern_description == NULL){
   $pattern_description = variable_get('general_site_default_description',NULL);
  }
  $pattern_keywords = variable_get($page_handler.'_keywords',NULL);
  if($pattern_keywords == NULL){
   $pattern_keywords = variable_get('general_site_default_keywords',NULL);
  }  
  $pattern_published = variable_get('general_site_default_published',NULL);  
  if($pattern_description){
    $page_description = str_replace('[site-name]', $site_name, $pattern_description);
    $page_description = str_replace('[site-url]', $site_url, $page_description);
    $page_description = str_replace('[page-name]', $page_name, $page_description);
    $page_description = str_replace('[term-name]', $term_name, $page_description);
    $page_description = str_replace('[term-description]', $term_description, $page_description);
    $page_description = str_replace('[associated-brand]', $associated_brand, $page_description);
    $page_description = strip_tags(str_replace('"', '', $page_description));
    $html .= "<meta name=\"description\" content=\"".$page_description."\">\n";   
  }
  if($pattern_keywords){
    $page_keywords = str_replace('[site-name]', $site_name, $pattern_keywords);
    $page_keywords = str_replace('[site-url]', $site_url, $page_keywords);
    $page_keywords = str_replace('[page-name]', $page_name, $page_keywords);
    $page_keywords = str_replace('[term-name]', $term_name, $page_keywords);
    $page_keywords = str_replace('[term-description]', $term_description, $page_keywords);
    $page_keywords = str_replace('[associated-brand]', $associated_brand, $page_keywords);
    $page_keywords = strip_tags(str_replace('"', '', $page_keywords));
    $html .= "<meta name=\"keywords\" content=\"".$page_keywords."\">\n";   
  }
  if($pattern_published){
    $unix = time();
    $site_published = date($pattern_published,$unix);
    $html .= "<meta name=\"published\" content=\"".$site_published."\" />\n";
  }  
  $tid = $term->tid;
  $og_page_type = "article";
  if ($tid == 1 || $tid == 2 || $tid == 3 || $tid == 4 || $tid == 5 || $tid == 6 || $tid == 54 || $tid == 48 || $tid == 55 || $tid == 56 || $tid == 61 || $tid == 59 || $tid == 60 || $tid == 53 || $tid == 58 || $tid == 57 || $tid == 49 || $tid == 52 || $tid == 51 || $tid == 50){
    $site_refresh = variable_get('general_site_default_refresh',NULL);
    if($site_refresh){
      $html .= "<meta http-equiv=\"refresh\" content=\"".$site_refresh."\" />\n";
      $og_page_type = "website"; 
    } 
  }  
  
  $photo_default = variable_get($domain.'_og_default_image',NULL);
  $logo_src = variable_get('logo_'.$domain,NULL);
  $photo_src = str_replace('[site-logo]',$logo_src,$photo_default);
  $facebook_api = variable_get($domain.'_og_facebook_api',NULL);
  $facebook_admins = variable_get($domain.'_og_facebook_administrators',NULL);
  $html .= "<meta property=\"og:title\" content=\"".$term_name." | ".$site_name."\"/>\n";
  $html .= "<meta property=\"og:type\" content=\"".$og_page_type."\"/>\n";
  $html .= "<meta property=\"og:site_name\" content=\"".$site_name."\"/>\n";
  $html .= "<meta property=\"og:url\" content=\"http://".$site_url."/".$current_path[0]."\"/>\n";
  $html .= "<meta property=\"og:image\" content=\"http://".$server_name."/".$photo_src."\"/>\n";
  $page_description = strip_tags(str_replace('"', '', $page_description));
  $html .= "<meta property=\"og:description\" content=\"".$page_description."\"/>\n";
  if($facebook_api){
    $html .= "<meta property=\"fb:app_id\" content=\"".$facebook_api."\" />\n";
  }
  if($facebook_admins){
    $html .= "<meta property=\"fb:admins\" content=\"".$facebook_admins."\" />\n";
  }    
  
  drupal_set_title(wallydemo_check_plain($term->name)); 
  
  return $html;
}

function _set_meta_forpages($page,$site_name=NULL,$site_url=NULL,$associated_brand=NULL,$domain=NULL,$current_path=NULL){

	$page_name = $page['subtask']['admin title'];
  $html = "";
  $page_handler = $page['handler']->name; 
  $pattern_description = variable_get($page_handler.'_description',NULL);
  if($pattern_description == NULL){
   $pattern_description = variable_get('general_site_default_description',NULL);
  }
  $pattern_keywords = variable_get($page_handler.'_keyword',NULL);
  if($pattern_keywords == NULL){
   $pattern_keywords = variable_get('general_site_default_keywords',NULL);
  } 
  $pattern_published = variable_get('general_site_default_published',NULL);  
  if($pattern_description){
    $page_description = str_replace('[site-name]', $site_name, $pattern_description);
    $page_description = str_replace('[site-url]', $site_url, $page_description);
    $page_description = str_replace('[page-name]', $page_name, $page_description);
    $page_description = str_replace('[associated-brand]', $associated_brand, $page_description);
    $page_description =  strip_tags(str_replace('"', '', $page_description));
    $html .= "<meta name=\"description\" content=\"".$page_description."\">\n";   
  } 
  if($pattern_keywords){
    $page_keywords = str_replace('[site-name]', $site_name, $pattern_keywords);
    $page_keywords = str_replace('[site-url]', $site_url, $page_keywords);
    $page_keywords = str_replace('[page-name]', $page_name, $page_keywords);
    $page_keywords = str_replace('[associated-brand]', $associated_brand, $page_keywords);
    $page_keywords =  strip_tags(str_replace('"', '', $page_keywords));
    $html .= "<meta name=\"keywords\" content=\"".$page_keywords."\">\n";   
  }
  if($pattern_published){
    $unix = time();
    $site_published = date($pattern_published,$unix);   
    $html .= "<meta name=\"published\" content=\"".$site_published."\" />\n";
  }  
  
  $html .= "<meta name=\"google-site-verification\" content=\"D9bx_DMB7JV6j259foRzkilqRdQONm1vAv5qiTgmlnA\" />\n";
  
  $server_name = $_SERVER['SERVER_NAME'];
  if(strstr($server_name, "mobile")){
   $server_name = "www.sudinfo.be";
  }
  
  $photo_default = variable_get($domain.'_og_default_image',NULL);
  $logo_src = variable_get('logo_'.$domain,NULL);
  $photo_src = str_replace('[site-logo]',$logo_src,$photo_default);
  $facebook_api = variable_get($domain.'_og_facebook_api',NULL);
  $facebook_admins = variable_get($domain.'_og_facebook_administrators',NULL);
  $page_name = strip_tags(str_replace('"', '', $page_name));
  
  
  $html .= "<meta property=\"og:title\" content=\"".$page_name." | ".$site_name."\"/>\n";
  $html .= "<meta property=\"og:type\" content=\"article\"/>\n";
  $html .= "<meta property=\"og:site_name\" content=\"".$site_name."\"/>\n";
  $html .= "<meta property=\"og:url\" content=\"http://".$site_url."/".$current_path[0]."\"/>\n";
  $html .= "<meta property=\"og:image\" content=\"http://".$server_name."/".$photo_src."\"/>\n";
  $page_description = strip_tags(str_replace('"', '', $page_description));
  $html .= "<meta property=\"og:description\" content=\"".$page_description."\"/>\n";
  if($facebook_api){
    $html .= "<meta property=\"fb:app_id\" content=\"".$facebook_api."\" />\n";
  }
  if($facebook_admins){
    $html .= "<meta property=\"fb:admins\" content=\"".$facebook_admins."\" />\n";
  }     
  
  drupal_set_title(wallydemo_check_plain($term->name)); 
  
  return $html;  
  
}

/**
 * Cette foonction permet de lancer le call back de la page forward 
 * dans une fenetre modale. 
 * 
 * @param
 *  $path : le path machine du noeud (/node/nid).
 *  $title : le title du noeud
 *  $text : le texte du lien
 */
function forward_modal_link($path, $title, $text) {

	$class = 'forward-page';
	$url = 'forward';
	if (module_exists('ctools')) {
		ctools_include('modal');
	  ctools_modal_add_js();
	  $class .= ' ctools-use-modal ctools-modal-forward-modal-style';
	  $url .= '/ajax';
	
		drupal_add_js(array(
	  	'forward-modal-style' => array(
	    'modalSize' => array(
		    'type' => 'fixed',
		    'width' => 640,
	 			'height' => 550,
			),
			'modalOptions' => array(
	    	'opacity' => .85,
	      'background' => '#444',
	     ),
	    ),
	    ), 'setting');
	}
  
	$content = l($text, $url, array(
                'title'      => $title,
                'html'       => TRUE,
                'attributes' => array('title' => urlencode($title), 'class' => $class),
                'query'      => array('path' => $path, 'title' => urlencode($title),
              )));
              
	return $content; 
}

/**
 *  Implémentation du hook_theme(); 
 */
function wallydemo_theme(&$var) {
//function phptemplate_theme(&$var) {
	
  $path = drupal_get_path('theme', 'wallydemo');
  $base = array(
    'file' => 'theme.inc',
//  'path' => "$path/templates",
  );
  
 return array(
    'comment_form' => $base + array(
    'arguments' => array('form' => NULL),
    'path' => "$path/templates",
		 ),
    'sp_header' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "data_array" => NULL, "options" => NULL),
    'template' => 'sp_header',
    'path' => "$path/templates/general",
		 ), 
    'sp_footer' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "data_array" => NULL, "options" => NULL),
    'template' => 'sp_footer',
    'path' => "$path/templates/general",
		 ),
    'sp_leaderboard' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "data_array" => NULL, "options" => NULL),
    'template' => 'sp_leaderboard',
    'path' => "$path/templates/general",
		 ),	 
    'sp_logo' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "data_array" => NULL, "options" => NULL),
    'template' => 'sp_logo',
    'path' => "$path/templates/general",
		 ),   
    'sp_regions_date' => $base + array(
    'arguments' => array("menu" => NULL),
    'template' => 'sp_regions_date',
    'path' => "$path/templates/general",
		 ),   
    'sp_recherche_menu' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "data_array" => NULL, "options" => NULL),
    'template' => 'sp_recherche_menu',
    'path' => "$path/templates",
		 ),           
    'sp_trafic' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "data_array" => NULL, "options" => NULL),
    'template' => 'sp_trafic',
    'path' => "$path/templates/general",
		 ),   
    'sp_meteo' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "data_array" => NULL, "options" => NULL),
    'template' => 'sp_meteo',
    'path' => "$path/templates/general",
		 ), 
    'sp_carburant' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "data_array" => NULL, "options" => NULL),
    'template' => 'sp_carburant',
    'path' => "$path/templates/general",
		 ),
    'sp_pagersimple' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "data_array" => NULL, "options" => NULL),
    'template' => 'sp_pagersimple',
		'path' => "$path/templates/general",
    ), 
    'rss_mix_crossmedia_saveurs' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "feed" => NULL, "options" => NULL),
    'template' => 'rss_mix_crossmedia_saveurs',
    'path' => "$path/templates/rssmix",
    ),
    'rss_mix_crossmedia_lesoir' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "feed" => NULL, "options" => NULL),
    'template' => 'rss_mix_crossmedia_lesoir',
    'path' => "$path/templates/rssmix",
    ),
    'rss_mix_crossmedia_soirmag' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "feed" => NULL, "options" => NULL),
    'template' => 'rss_mix_crossmedia_soirmag',
    'path' => "$path/templates/rssmix",
    ),
    'rss_mix_crossmedia_cinenews' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "feed" => NULL, "options" => NULL),
    'template' => 'rss_mix_crossmedia_cinenews',
    'path' => "$path/templates/rssmix",
    ),
    'rss_mix_crossmedia_references' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "feed" => NULL, "options" => NULL),
    'template' => 'rss_mix_crossmedia_references',
    'path' => "$path/templates/rssmix",
    ),
    'rss_mix_crossmedia_sante' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "feed" => NULL, "options" => NULL),
    'template' => 'rss_mix_crossmedia_sante',
    'path' => "$path/templates/rssmix",
    ),
    'rss_mix_crossmedia_starnews' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "feed" => NULL, "options" => NULL),
    'template' => 'rss_mix_crossmedia_starnews',
    'path' => "$path/templates/rssmix",
    ),
    'rss_mix_crossmedia_standard' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "feed" => NULL, "options" => NULL),
    'template' => 'rss_mix_crossmedia_standard',
    'path' => "$path/templates/rssmix",
    ),   
    'rss_mix_last_video' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "feed" => NULL, "options" => NULL),
    'template' => 'rss_mix_last_video',
    'path' => "$path/templates/rssmix",
    ),   
    'rss_mix_last_gallery' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "feed" => NULL, "options" => NULL),
    'template' => 'rss_mix_last_gallery',
    'path' => "$path/templates/rssmix",
    ), 
    'rss_mix_imagedujour' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "feed" => NULL, "options" => NULL),
    'template' => 'rss_mix_imagedujour',
    'path' => "$path/templates/rssmix",
    ),
    'flow_mix_regions_titres' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "feed" => NULL, "options" => NULL),
    'template' => 'flow_mix_regions_titres',
    'path' => "$path/templates/rssmix",
    ),
    'flow_mix_regions_titres_liste' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "feed" => NULL, "options" => NULL),
    'template' => 'flow_mix_regions_titres_liste',
    'path' => "$path/templates/rssmix",
    ),    
    'flow_mix_boutique' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "feed" => NULL, "options" => NULL),
    'template' => 'flow_mix_boutique',
    'path' => "$path/templates/rssmix",
    ),
    'flow_mix_blogs_thematiques' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "feed" => NULL, "options" => NULL),
    'template' => 'flow_mix_blogs_thematiques',
    'path' => "$path/templates/rssmix",
    ),      
    'flow_mix_blogs_regionaux' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "feed" => NULL, "options" => NULL),
    'template' => 'flow_mix_blogs_regionaux',
    'path' => "$path/templates/rssmix",
    ),       
    'sp_block_foot_regional' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "data_array" => NULL, "options" => NULL),
    'template' => 'sp_block_foot_regional',
    'path' => "$path/templates/general",   
    ),
    'mobile_header' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "data_array" => NULL, "options" => NULL),
    'template' => 'mobile_header',
    'path' => "$path/templates/mobile",   
    ),
    'mobile_footer' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "data_array" => NULL, "options" => NULL),
    'template' => 'mobile_footer',
    'path' => "$path/templates/mobile",   
    ),
    'mobile_mainmenu' => $base + array(
    'arguments' => array("menu" => NULL),
    'template' => 'mobile_mainmenu',
    'path' => "$path/templates/mobile",
    ),           
    'mobile_unebis_regions' => $base + array(
    'arguments' => array("data" => NULL),
    'template' => 'mobile_unebis_regions',
    'path' => "$path/templates/mobile",   
    ),   
    'spgennews_default' => $base + array(
    'arguments' => array("data" => NULL),
    'template' => 'spgennews_default',
    'path' => "$path/templates/newsletters",   
    ),       
    'rss_mix_crossmedia_regionjobs' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "feed" => NULL, "options" => NULL),
    'template' => 'rss_mix_crossmedia_regionjobs',
    'path' => "$path/templates/rssmix",   
    ),       
    'flow_mix_external_titre_photo_date' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "feed" => NULL, "options" => NULL),
    'template' => 'flow_mix_external_titre_photo_date',
    'path' => "$path/templates/rssmix",   
    ),
    'flow_mix_dest_titre_photo_date' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "feed" => NULL, "options" => NULL),
    'template' => 'flow_mix_dest_titre_photo_date',
    'path' => "$path/templates/rssmix",   
    ),
    'flow_mix_dest_titre' => $base + array(
    'arguments' => array("subtype" => NULL, "context" => NULL, "feed" => NULL, "options" => NULL),
    'template' => 'flow_mix_dest_titre',
    'path' => "$path/templates/rssmix",   
    ),
  );
}


/**
 * Filter front text 
 * This function has to be completed with our own text filter
 */
function wallydemo_check_plain($text){
	$text = str_replace("’", "'", $text);
  return $text;
}

/**
 * Display the breadcrumb ( for article view and node ) 
 * return the current destination and his different parents (it use term_hierarchy)
 * 
 * @param $node
 *  package
 * 
 * @param $main_destination_tid
 *  the destination id
 * 
 */
function _wallydemo_breadcrumb_display($main_destination_tid, $type = NULL){
  if($type == 'une') {
    $countLevel = 1;
  } else {
    $countLevel = 10;
  }

  //retrouver l'url de la destination avec son tid;
  $url = "taxonomy/term/".$main_destination_tid;
  $url_alias = drupal_get_path_alias($url);
  $nameF = taxonomy_get_term($main_destination_tid);
  $last = ' <a href="/'.$url_alias.'" class="last"><span>'.$nameF->name.'</span></a>';
  $i = 0;
  $html = "";
  //trouver a quel item de menu correspond l'url
  while($main_destination_tid != 0 && $i < $countLevel) {
    $res = db_query('SELECT * FROM {term_hierarchy} WHERE tid = %d', $main_destination_tid);
    $d = db_fetch_array($res);
    $url = 'taxonomy/term/'.$d["tid"];
    $url_alias = drupal_get_path_alias($url);
    $name = taxonomy_get_term($d["tid"]);
    if($nameF->name != $name->name) {
      $html = '<a href="/'.$url_alias.'"><span>'.$name->name.'</span></a> <img width="3" height="5" alt="&gt;" src="/sites/all/themes/wallydemo/images/ico_arrow_transparent.gif"> '.$html;
      $i++;
    }
    $main_destination_tid = $d["parent"];
  }
  $html = $html.$last;
  $data = '<p class="fil_ariane novisited">'.$html.'</p>';
   
  if ($type == "last") {
    return $last;
  } else {
    return $data;
  }

}

function _custom_sudpresse_breadcrumbStaticPage_display(){
  return _wallydemo_breadcrumbStaticPage_display();
}

/**
 * Display the breadcrumb ( for static blank page ) 
 * return the current destination and his different parents ( it use the menu hierarchy )
 * 
 * @param $node
 *  package
 * 
 * @param $main_destination_tid
 *  the destination id
 * 
 */
function _wallydemo_breadcrumbStaticPage_display(){
  $cid = "spmain_menu_".$_SERVER["SERVER_NAME"];
  $cached_data = cache_get($cid);
  $item = menu_get_item();

  $data = getItem($cached_data->data, $item["tab_root"]);
  $data = '<div class="breadcrumb"><p class="fil_ariane left">Vous êtes ici : '.$data.'</p></div>';

	return $data;
}

/**
 * Recursive function to retrieve the different levels of menu
 * return hierarchy of a point menu
 * 
 * @param $node
 *  package
 * 
 * @param the path oof element
 *  
 * 
 */
function getItem($tab,$name) {
  $html = '';
  if(is_array($tab)){
	  foreach ($tab as $t) {
	    if(isset($t["link"]["link_path"]) && $name == $t["link"]["link_path"]) {
	      if($t["link"]["link_path"] && $t["link"]["link_path"] != '<front>') $html = '<a href="'.drupal_get_path_alias($t["link"]["link_path"]).'">'.$t["link"]["link_title"].'</a>'.$html;
	      else $html = $t["link"]["link_title"].$html;
	    } else {
	      if(count($t["below"] > 0)) {
	        $html .= getItem($t["below"], $name);
	        if($html) {
	          if($t["link"]["link_path"] && $t["link"]["link_path"] != '<front>') $html = '<a href="'.drupal_get_path_alias($t["link"]["link_path"]).'">'.$t["link"]["link_title"].'</a> &gt; '.$html;
	          else $html = $t["link"]["link_title"].' &gt; '.$html;
	        }
	      }
	    }
	    if($html) break;
	  }
  }
  return $html;
}

function _custom_sudpresse_get_strapline($textObject=NULL, $node, $size){
  return _wallydemo_get_strapline($textObject, $node, $size);
}

/**
 * Extract the strapline from an Article
 * If the article's chapofield is empty, return a teaser
 * 
 * @param $node
 *  package
 * 
 * @param $textObject
 *  the main textObject. Often the mainstory
 * 
 * @param $size
 *  max size for the strapline. 
 *  If no max size is wanted, indicate 0 as value
 * 
 */

function _wallydemo_get_strapline($textObject=NULL, $node, $size){
  
  $strapline = "";
  if ($textObject == NULL | $textObject->type != 'wally_textobject'){
	$mainstory = $node;
  } else {
    $mainstory = $textObject;
  }
  if ($mainstory->type == "wally_textobject"){
   $strapline = $mainstory->field_textchapo[0]['value'];
  } else {
   $strapline = $mainstory->field_summary[0]['value'];
  }
  if ($strapline != "" && $size == 0){
    return $strapline;
  }
  
  if ($strapline != "" && drupal_strlen($strapline) <= $size) {
    return $strapline;
  } else {
 	$strapline = _wallydemo_cut_string($strapline, $size);
  }
 	
  if ($strapline == ""){
    $teaser_length = $size;
    $teaser = theme("wallyct_teaser", $mainstory->field_textbody[0]['value'], $teaser_length, $node);
    $strapline = $teaser;
    if($size > 0 && drupal_strlen($mainstory->field_textbody[0]['value']) > $size) $strapline .=" [...]";
  }
   return $strapline;
}


/**
 * Render a cut string
 * 
 * @param $string
 *   
 *    * @param $size
 *  max size for the strapline. 
 *  If no max size is wanted, indicate 0 as value
 *  
 *  @return $cut_string
 */

function _wallydemo_cut_string($str,$size) {
  
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
  // If a strapline was not found, still return a teaser.
    
 if($strapline) $strapline .=" [...]";
   
  return $strapline;
}

/**
 * Render the differents date displays
 * 
 * @param $unix_time
 *   
 * @param $display
 *  
 *  'filinfo' -> '00:00'
 *  'unebis' -> 'lundi 01 janvier 2011, 00h00'
 *  'date_courte' -> '30/05 - 13h24'
 *  'default' -> 'publié le 01/01 à 00h00'
 *  
 *  @return $displayed_date
 */
function _wallydemo_date_edition_diplay($unix_time, $display='default'){
  $french_days = array('0'=>'Dimanche', '1'=>'Lundi', '2'=>'Mardi', '3'=>'Mercredi', '4'=> 'Jeudi', '5'=>'Vendredi', '6'=>'Samedi');
  $french_months = array('1'=>'Janvier', '2'=>'Février', '3'=>'Mars', '4'=>'Avril', '5'=>'Mai', '6'=>'Juin','7'=>'Juillet', '8'=>'Août', '9'=>'Septembre', '10'=>'Octobre', '11'=>'Novembre', '12'=>'Décembre');	
  switch ($display){
  	case 'filinfo':
  		//11<abbr title="heure">:</abbr>28
  		$displayed_date = date('H', $unix_time).'<abbr title="heure">:</abbr>'.date('i', $unix_time);
  	 break;
    case 'flux_rss':
      //Thu, 01 Mar 2012 12:01:11 +0100
      $displayed_date = date('r', $unix_time);
     break;  	 
  	case 'unebis':
  		//jeudi 26 mai 2011, 15:54
  		$displayed_date=$french_days[date('w', $unix_time)]." ".date('j', $unix_time)." ".$french_months[date('n', $unix_time)]." ".date('Y, H<abbr title=\"heure\">\h</abbr>i', $unix_time);
  		break;
  	case "date_courte":
  		//30/05 - 13h24	
  		$displayed_date = date('j/m \- H<abbr title=\"heure\">\h</abbr>i', $unix_time);
  		break;
    case "date_jour":
      //mercredi 8 juin 2011 
      $displayed_date = $french_days[date('w', $unix_time)]." ".date('j', $unix_time)." ".$french_months[date('n', $unix_time)]." ".date('Y', $unix_time);
      break;  		
    case "date_jour_heure":
      //mercredi 8 juin 2011 à 16h36 
      $displayed_date = $french_days[date('w', $unix_time)]." ".date('j', $unix_time)." ".$french_months[date('n', $unix_time)]." ".date('Y', $unix_time)." à ".date('H<abbr title=\"heure\">\h</abbr>i', $unix_time);
      break;  		
  	default:
      //publié le 26/05 à 15h22
      $displayed_date = date('\p\u\b\l\i\é \l\e j/m \à H<abbr title=\"heure\">\h</abbr>i', $unix_time);
  }
  return $displayed_date;
}

/**
 * Render the package's signature
 * 
 */
function _wallydemo_get_package_signature($signature_field){
	if($signature_field->field_copyright[0]["value"] != ""){
	 $package_signature =  $signature_field->field_copyright[0]["value"];
	} else {
	 $package_signature = variable_get("default_package_signature",NULL);
	}
  return $package_signature;
}



/**
 * Render the crossmedia block html content
 * 
 * @param $feed object
 * 
 * @return $content
 *  html 
 */

function _wallydemo_rss_crossmedia_gen($feed) {
  $content = "";
  $cpt = 0;
  foreach ($feed as $k=>$item) {
    if ($cpt == 0){
    	if(isset($item['EmbeddedContent']['EmbeddedObjects']['Object'][0]['LocaleImage']['filepath'])){
        $item['image_path'] = $item['EmbeddedContent']['EmbeddedObjects']['Object'][0]['LocaleImage']['filepath'];
	      $content .= "<li class=\"avecphoto\">";
	      if ($item['image_path'] != ""){
	        $item['img'] = theme('imagecache', 'divers_201x134', $item['image_path']); 
	        $content .="<a href='".$item['ExternalURI']['value']."' target=\"_blank\">
	               ".$item['img']."
	              </a>";
	      }else{
        $content .="<li>";
	      }
    	}else{
    	 $content .="<li>";
    	}
      $content .="<a href='".$item['ExternalURI']['value']."'>".wallydemo_check_plain($item['MainStory']['Title']['value'])."</a>
             </li>";
    }else{
      $content .= "<li><a href='".$item['ExternalURI']['value']."'>".wallydemo_check_plain($item['MainStory']['Title']['value'])."</a></li>";        
    }
    $cpt++;
    unset($item);
  }
  unset($feed);
  return $content; 
}

/**
 * Render the pager html content
 * 
 * @param $feed object
 * 
 * @return $content
 *  html 
 */
function _wallydemo_pager($tags = array(), $limit = 10, $element = 0, $parameters = array(), $quantity = 9) {
	global $pager_page_array, $pager_total;
	
	$path = drupal_get_path_alias('taxonomy/term/'.$parameters['tid']);
  	// Calculate various markers within this pager piece:
  	//Middle is used to "center" pages around the current page.
  	$pager_middle = ceil($quantity / 2);
	// current is the page we are currently paged to
  	$pager_current = $pager_page_array[$element] + 1;
  	// first is the first page listed by this pager piece (re quantity)
  	$pager_first = $pager_current - $pager_middle + 1;
  	// last is the last page listed by this pager piece (re quantity)
  	$pager_last = $pager_current + $quantity - $pager_middle;
  	// max is the maximum page number
  	$pager_max = $pager_total[$element];
  	// End of marker calculations.

  	// Prepare for generation loop.
  	$i = $pager_first;
  	if ($pager_last > $pager_max) {
    	// Adjust "center" if at end of query.
    	$i = $i + ($pager_max - $pager_last);
    	$pager_last = $pager_max;
  	}
  	if ($i <= 0) {
    	// Adjust "center" if at start of query.
    	$pager_last = $pager_last + (1 - $i);
    	$i = 1;
  	}
  	// End of generation loop preparation.
  	
  	// Display the number of current page
  	$items[] = array('data' => 'Page '.$pager_current.'/'.$pager_total[$element].' : <span>');
  	if ($pager_total[$element] > 1) {
  		
		// Display PREVIOUS page button
  		if ($pager_current > 1) {
      		$items[] = array('data' => '<a href="'.$path.'?page='.($pager_current-2).'">< </a>');
    	}
      	
		// Now generate the actual pager piece.
      	for (; $i <= $pager_last && $i <= $pager_max; $i++) {
        	if ($i < $pager_current) {
          		$items[] = array('data' => '<a href="'.$path.'?page='.($i-1).'">'.$i.' </a>');
        	}
        	if ($i == $pager_current) {
          		$items[] = array('data' => '<a href="'.$path.'?page='.($i-1).'"><strong>'.$i.' </strong></a>');
        	}
        	if ($i > $pager_current) {
        		$items[] = array('data' => '<a href="'.$path.'?page='.($i-1).'">'.$i.' </a>');
        	}
      	}

		// Display NEXT page button 
      	if ($pager_current < $pager_total[$element]) {
        	$items[] = array('data' => '<a href="'.$path.'?page='.($pager_current).'">> </a>');
      	}
    }
    $items[] = array('data' => '<span>');

    
    return theme('sp_pagersimple', $items);
  }	

function _wallydemo_get_trimmed_string($string){
  $trimmed_string = preg_replace("+</?[^>]*?>+","",$string);
  return $trimmed_string;
}

function _custom_sudpresse_get_sorted_links($node) {
  return _wallydemo_get_sorted_links($node);
}

function _wallydemo_get_sorted_links($node){
  $allLinks = array();
  $listLinks = $node->field_linkedobjects_nodes;
  if ($listLinks != NULL){
    foreach ($listLinks as $ll) {
      $lLinks = array();
  	  $lLinks["title"] = $ll->title;
  	  $i = 0;   
  	  if (isset($ll->field_links_list_nodes)){
  	    foreach ($ll->field_links_list_nodes as $l) {			
  	      if ($l->field_internal_link_nodes[0]->field_packagelayout[0]["value"]) {
  	        $package_layout = $l->field_internal_link_nodes[0]->field_packagelayout[0]["value"];
  	        $package_layout = taxonomy_get_term($package_layout);
  	        $package_layout_name = $package_layout->name;
  	      }
  		  // teste s'il s'agit d'un lien interne
  		  if ($l->field_internal_link[0]["nid"] != NULL) {
  		    $nodeTarget = node_load($l->field_internal_link[0]["nid"]);
    	    $lLinks["links"][$i]["internal"] = 1;
    		$lLinks["links"][$i]["title"] = $nodeTarget->title;
    	    $lLinks["links"][$i]["target"] = NULL;
    	    $lLinks["links"][$i]["status"] = $l->status;				
    		$lLinks["links"][$i]["url"] = "/".drupal_get_path_alias("node/".$nodeTarget->nid);
    		if ($package_layout_name) $lLinks["links"][$i]["packagelayout"] = $package_layout_name;
  		  } else {
  		    if ($l->files) {
  			  $att = array_pop($l->files);
  			  $lLinks["links"][$i]["url"] = "/".$att->filepath;
  			  $lLinks["links"][$i]["title"] = $l->title;
  	        } else {
  			  $lLinks["links"][$i]["url"] = $l->field_link_item[0]["url"];
  			  if (isset($l->field_link_item[0]["title"]) && ($l->field_link_item[0]["title"])!="" ) {
  			    $lLinks["links"][$i]["title"] = $l->field_link_item[0]["title"];
  			  } else {
  		        $lLinks["links"][$i]["title"] = $l->title;
  			  }
  			  $lLinks["links"][$i]["target"] = $l->field_link_item[0]["attributes"]["target"];
  		    } 
  		    $lLinks["links"][$i]["status"] = $l->status;
  		  }
  		  $lLinks["links"][$i] = _wallydemo_get_link_type(&$lLinks["links"][$i]);			
  		 $i++;
  	    }
  	    array_push($allLinks,$lLinks);
  	  }
    }
  }
  return $allLinks;
}

/**
 *  Type un Tlink en fonction de son url
 */
function _wallydemo_get_link_type(&$link){
  
	if (strstr($link["url"], "videos.sudpresse")){
	  $link["type"] = "media-video";
  } else if (strstr($link["url"], "http://www.youtube.com/")){
    $link["type"] = "media-video";
  } else if (strstr($link["url"], "dailymotion.com/")){
    $link["type"] = "media-video";
  } else if (strstr($link["url"], "ustream.tv/")){
	  $link["type"] = "media-video";
  } else if (strstr($link["url"], "vimeo.com/")){
    $link["type"] = "media-video";	
  } else if (strstr($link["url"], "video.rtlinfo.be/")){
    $link["type"] = "media-video";	
  } else if (strstr($link["url"], "/cache/page/widget_")){
    $link["type"] = "idalgo";
  } else if (strstr($link["url"], "portfolio.sudpresse")){
    $link["type"] = "portfolio";	
  } else if (strstr($link["url"], "dossiers")){
    $link["type"] = "dossier";
  } else if (strstr($link["url"], "blogs.sudpresse'")){
  	$link["type"] = "blog";
  } else {
  	$link["type"] = "media-press";
  }

  return $link;
}

/**
 * Try to find the package's first photoObject. 
 * If not, return false
 * 
 */
function custom_sp_get_first_photoEmbededObject_from_package($embededObjects_array){
  return wallydemo_get_first_photoEmbededObject_from_package($embededObjects_array);
}

/**
 * Try to find the package's first photoObject. 
 * If not, return false
 * 
 */
function wallydemo_get_first_photoEmbededObject_from_package($embededObjects_array){
  if(is_array($embededObjects_array)){
		foreach($embededObjects_array as $embededObject){  
	   if ($embededObject->type == "wally_photoobject"){
	     $photoObject = $embededObject;
	     break;
	   }
	  }
  }
  if (isset($photoObject)){
    return $photoObject; 
  }else{
    return false;
  }
}

/**
 * Renvoi un lien embedd en html
 * 
 * @param $link
 * L'objet lien
 * 
 * @return html
 */
function wallydemo_displayembeddedlink($link, &$embeds_photo){
  $content = '';
  
  if (preg_match('/www.youtube.com/i', $link['display_url']) != 0){
    $content ='<object style="height: 345px; width: 420px"><param name="movie" value="'.$link['display_url'].'" type="application/x-shockwave-flash" allowfullscreen="true" allowScriptAccess="always" width="420" height="345"></object>';
  }
  if (preg_match('/(.jpeg|.jpg)/i', $link['display_url']) != 0){
    $embeds_photo[] = array(
      'mini' => '<img src="'.$link['display_url'].'" alt="" title="" class="" width="48" height="32" />', 
      'nid' => $link['nid'],
      'main_size' => '<img class = "imagecache imagecache-article_300x200" src="'.$link['display_url'].'" alt="" title="" class=""   width = "300"/>',
    );
  }
  return $content;
}

/**
 * Brackets the embeddedObjects to be displayed in the package template
 * @param $node
 * 
 * 
 * @return $data
 *  new array of node's embdeddedObjects
 * 
 */

function wallydemo_bracket_embeddedObjects_from_package($node){
  $data = array();
  $photos = array();
  $videos = array();
  $audios = array();
  $digital = array();
  $link = array();
  $text = array();
  $packages = array();
  $embeddedObjects = $node->field_embededobjects_nodes;
  if($node->type == "wally_articlepackage"){
    $data["mainObject"] = $embeddedObjects[0];
  } else {
	$data["mainObject"] = $node->field_mainobject_nodes[0];
  }
  if ($embeddedObjects != NULL){
    foreach($embeddedObjects as $embeddedObject){
      switch($embeddedObject->type){
    	case "wally_photoobject":
    		array_push($photos, $embeddedObject);
    		break;
    	case "wally_videoobject":
    		array_push($videos, $embeddedObject);
    		break;
    	case "wally_audioobject":
        array_push($audios, $embeddedObject);
    		break;
    	case "wally_digitalobject":
         array_push($digital, $embeddedObject);
    		break;   	  
    	case "wally_linktype":
    	  if ($node->embed_links[$embeddedObject->nid]['group_type'] == 'photo'){
    	    array_push($photos, $embeddedObject);
    	  } elseif ($node->embed_links[$embeddedObject->nid]['group_type'] == 'video'){
            array_push($videos, $embeddedObject);
    	  } elseif ($node->embed_links[$embeddedObject->nid]['provider'] == 'coveritlive'){
    	    $embeddedObject->coveritlive = TRUE;
    	    array_push($digital, $embeddedObject);
    	  } elseif ($embeddedObject->field_internal_link[0]['nid'] != NULL) {
    	    $pack = node_load(array('nid' => $embeddedObject->field_internal_link[0]['nid']));
    	    wallycontenttypes_packagepopulate($pack);
    	    array_push($packages, $pack);
    	  }
    	  break;
      }
    }
  }
  $data["photos"] = $photos;
  $data["videos"] = $videos;
  $data["audios"] = $audios;
  $data["digital"] = $digital;
  $data["link"] = $link;
  $data["text"] = $text;
  $data["package"] = $packages;
  return $data;
}

/**
 * Render an array with a photoObject's infos for theming operations
 * 
 */
function wallydemo_get_photo_infos_and_display($photoObject,$template="default"){
  if ($photoObject->type == "wally_photoobject"){
    $photo = array();
    $photo["nid"] = $photoObject->nid;
    $photo["title"] = $photoObject->title;
    $photo["type"] = $photoObject->type;
    $photo['credit'] = $photoObject->field_copyright[0]['value'];
    $photo['summary'] = $photoObject->field_summary[0]['value'];
    $photo['fullpath'] = $photoObject->field_photofile[0]['filepath'];
    $photo['size'] = $photoObject->field_photofile[0]['filesize'];
    $photo['filename'] = $photoObject->field_photofile[0]["filename"];
    $photo['filepath'] = $photoObject->field_photofile[0]["filepath"];

    switch ($template){
      case "default":
        $photo['main_size'] = "";
        $photo['main_url'] = "";
        $photo['mini'] = "";
        if ($photo['size'] > 0){
          $photo['main_size'] = theme('imagecache', 'article_300x200',$photo['filepath'],strip_tags($photo['summary']),strip_tags($photo['summary']));
          $photo['main_url'] = imagecache_create_url('article_300x200', $photo['fullpath']);
          $photo['mini'] = theme('imagecache', 'article_48x32', $photo['filepath'],strip_tags($photo['summary']),strip_tags($photo['summary']));
        }
        break;
    }
  }
  return $photo;
}

/**
 * Render an array with a digitalObject's infos for theming operations
 * 
 */
function wallydemo_get_digitalobject_infos_and_display($digitalObject){
  $digital = array();
  $digital["nid"] = $digitalObject->nid;
  $digital["type"] = $digitalObject->type;
  $emcode = $digitalObject->content['group_digitalobject']['group']['field_object3rdparty']['field']['items']['#children'];
  
  // USE FOR GOOGLE DOC BECAUSE IFRAME IS IN THE SUMMARY
  if ($emcode) $digital['emcode'] = $emcode;
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

function custom_sp_get_video_infos_and_display($videoObject){
  return wallydemo_get_video_infos_and_display($videoObject);
}

/**
 * Render an array with a videoObject's infos for theming operations
 * 
 */
function wallydemo_get_video_infos_and_display($videoObject){
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
 * Render an array with a audioObject's infos for theming operations
 * 
 */
function wallydemo_get_audio_infos_and_display($audioObject){

  $audio = array();
  $audio["nid"] = $audioObject->nid;
  $audio["type"] = $audioObject->type;
  $audio['emcode'] = $audioObject->content['group_audio']['group']['field_audio3rdparty']['field']['items'][0]['#children'];
  $audio['title'] = $audioObject->title;
  $audio['summary'] = $audioObject->field_summary[0]['value'];
  $audio['credit'] = $audioObject->field_copyright[0]['value'];

  return $audio;
  
}


// theme the crap out of the comment form
function wallydemo_comment_form($form) {
  // Add some intro text.
  
  // Weight it so it floats to the top.
  $form['intro']['#weight']  = -40;
 
  // Make the text-area smaller.
  $form['comment_filter']['comment']['#rows']   = 5;
  // Change the text-area title
  $form['comment_filter']['comment']['#title']  = t('Your message');
  // Add a div wrapper for themeing.
  // Remove input formats information.
  $form['comment_filter']['format'] = NULL;
  
  // Remove the preview button
  $form['preview'] = NULL;
  
  return drupal_render($form);
}

/**
 * Retrieve tags from an article package node to display with the correct sudpresse format
 *  
 * @param $node
 *  $node which should be a Wally Article Package
 *  
 *  @return string list of tags formated as <a href="[TAXONOMY_PAGE]" class="[SAFE_CLASS_NAME]">[TERM_NAME]</a>, <a href.....
 */
function wallydemo_taxonomy_tags_particle($main_story){

          $vocabulary = taxonomy_get_vocabularies();
          $voclass = array();
          foreach ($vocabulary as $key => $value){
            $voclass[$key] = preg_replace('/[^a-z0-9]+/', '_', strtolower($value->name));
          }
          $cpt = 1;
          $htmltags = "";
          if (is_array($main_story->taxonomy)) {
            foreach($main_story->taxonomy as $termclass){
              if($cpt != 1){
                
                $htmltags .= ", ";
              }
            
              $htmltags .= "<a href=\"".url(taxonomy_term_path(taxonomy_get_term($termclass->tid)))."\" class=\"".$voclass[$termclass->vid]."\">".$termclass->name."</a>";
              $cpt++;         
            }
          }
          return $htmltags;
}

function wallydemo_taxonomy_text_tags_particle($main_story){

          $vocabulary = taxonomy_get_vocabularies();
          $voclass = array();
          foreach ($vocabulary as $key => $value){
            $voclass[$key] = preg_replace('/[^a-z0-9]+/', '_', strtolower($value->name));
          }
          $cpt = 1;
          $htmltags = "";
          if (is_array($main_story->taxonomy)) {
	          foreach($main_story->taxonomy as $termclass){
	            if($cpt != 1){
	              $htmltags .= ", ";
	            }
	          
	            $htmltags .= $termclass->name;
	            $cpt++;         
	          }
					}
          return $htmltags;
}
/**
 * Set and get cached data for a menu
 * @param $menu_name
 * 
 *  @return $data
 *  
 */
function wallydemo_menu_get_cache($menu_name){
	$cid = $menu_name;
	$cached_block = cache_get($cid);
  if (!is_object($cached_block) || !isset($cached_block) || empty($cached_block) || ($cached_block->expire < time()) || ($cached_block->expire == -1)) {
    $data = menu_tree_all_data($menu_name);
    cache_set($cid, $data, 'cache', time() + 60*30 + 1);
  } else {
	 $data = $cached_block->data;
  }
  return $data;
}

function _custom_sp_get_logo_data() {
  return _wallydemo_get_logo_data();
}

/**
 * 
 * Render the logo information functions of the current domain
 * 
 */
function _wallydemo_get_logo_data(){
  $domain_url = $_SERVER["SERVER_NAME"];
  $domain = 'sudinfo';
  $settings = variable_get('theme_wallydemo_settings',array());
  $theme_path = drupal_get_path('theme','wallydemo');
  $data = array();
  switch ($domain){
    case "lameuse":
    	if(isset($settings["logo_lameuse_default"])){
    	 $data["default"] = $settings["logo_lameuse_default"];
    	} else {
    	 $data["default"] = 1;
    	}
    	$data["default_path"] = variable_get('logo_lameuse',$theme_path.'/images/logos/logo_lameuse.gif');
    	$data["eve_path"] = $settings["logo_lameuse_eve_path"];
    	$data["html_id"] = "la_meuse";
    	$data["html_alt"] = "Lameuse";
    	$data["html_target"] = "http://www.lameuse.be";
      break;
    case "lacapitale":
      if(isset($settings["logo_lacapitale_default"])){
       $data["default"] = $settings["logo_lacapitale_default"];
      } else {
       $data["default"] = 1;
      }
      $data["default_path"] = variable_get('logo_lacapitale',$theme_path.'/images/logos/logo_lacapitale.gif');
      $data["eve_path"] = $settings["logo_lameuse_eve_path"];
      $data["html_id"] = "la_capitale";
      $data["html_alt"] = "Lacapitale";
      $data["html_target"] = "http://www.lacapitale.be";      
      break;
    case "lanouvellegazette":
      if(isset($settings["logo_lanouvellegazette_default"])){
       $data["default"] = $settings["logo_lanouvellegazette_default"];
      } else {
       $data["default"] = 1;
      }
      $data["default_path"] = variable_get('logo_lanouvellegazette',$theme_path.'/images/logos/logo_lanouvellegazette.gif');
      $data["eve_path"] = $settings["logo_lanouvellegazette_eve_path"];
      $data["html_id"] = "la_nouvellegazette";
      $data["html_alt"] = "Lanouvellegazette";
      $data["html_target"] = "http://www.lanouvellegazette.be";      
      break;
    case "laprovince":
      if(isset($settings["logo_laprovince_default"])){
       $data["default"] = $settings["logo_laprovince_default"];
      } else {
       $data["default"] = 1;
      }
      $data["default_path"] = variable_get('logo_laprovince',$theme_path.'/images/logos/logo_laprovince.gif');
      $data["eve_path"] = $settings["logo_laprovince_eve_path"];
      $data["html_id"] = "la_province";
      $data["html_alt"] = "Laprovince";
      $data["html_target"] = "http://www.laprovince.be";      
      break;
    case "nordeclair":
      if(isset($settings["logo_nordeclair_default"])){
       $data["default"] = $settings["logo_nordeclair_default"];
      } else {
       $data["default"] = 1;
      }
      $data["default_path"] = variable_get('logo_nordeclair',$theme_path.'/images/logos/logo_nordeclair.gif');
      $data["eve_path"] = $settings["logo_nordeclair_eve_path"];
      $data["html_id"] = "nord_eclair";
      $data["html_alt"] = "Nordeclair";
      $data["html_target"] = "http://www.nordeclair.be";      
      break;
    default:
      if(isset($settings["logo_sudinfo_default"])){
       $data["default"] = $settings["logo_sudinfo_default"];
      } else {
       $data["default"] = 1;
      }
      $data["default_path"] = variable_get('logo_sudinfo',$theme_path.'/images/logos/logo_sudinfo.gif');
      $data["eve_path"] = $settings["logo_sudinfo_eve_path"];
      $data["html_id"] = "sudinfo";
      $data["html_alt"] = "Sudinfo";
      $data["html_target"] = "http://www.sudinfo.be";      
      break;
  }     
  return $data;
}
/** Retourne l'url de base pour un domaine en fonction du domaine courant
 * 
 */

function wallydemo_get_fixed_domain_url($domain){
  $domain_url = "";
	switch($domain){
    case 'lameuse':
      $domain_url = "http://www.lameuse.be";
      break;
    case 'lanouvellegazette':
      $domain_url = "http://www.lanouvellegazette.be";
      break;
    case 'laprovince':
      $domain_url = "http://www.laprovince.be";
      break;
    case 'lacapitale':
      $domain_url = "http://www.lacapitale.be";
      break;
    case 'nordeclair':
      $domain_url = "http://www.nordeclair.be";
      break;
    case 'mobile':
      $domain_url = "http://mobile.sudinfo.be";
      break;
    default:
    	$domain_url = "http://www.sudinfo.be";
  }
	return $domain_url;
}

/** Retourne l'url de base pour les outils sociaux en fonction d'un term id et d'un domaine 
 * 
 * 
 */
function wallydemo_get_social_sharing_base_url($tid,$domain){
  $base_url = "";
	switch($tid){
		case 48:
		case 62:
		case 63:
    case 53:
    case 72:
    case 73:
    case 49:
    case 64:
    case 65:
    case 68:
    case 69:
    case 51:
    case 66:
    case 67:
    case 50:
    case 71:    	    	    	    		
			$base_url = wallydemo_get_fixed_domain_url('lameuse');
			break;
    case 78:
    case 79:
    case 56:
    case 80:
    case 81:
    case 57:
    case 82:
    case 83:
    case 58:    	    	
      $base_url = wallydemo_get_fixed_domain_url('lanouvellegazette');
      break;			
    case 86:
    case 87:
    case 60:
    case 84:
    case 85:
    case 59:    	            
      $base_url = wallydemo_get_fixed_domain_url('nordeclair');
      break;
    case 74:
    case 75:
    case 54:
    case 76:
    case 77:
    case 55:    	                  
      $base_url = wallydemo_get_fixed_domain_url('lacapitale');
      break;
    case 88:
    case 89:
    case 61:                        
      $base_url = wallydemo_get_fixed_domain_url('laprovince');
      break;
    default:
    	$base_url = wallydemo_get_fixed_domain_url('sudinfo');
	}
	return $base_url;
}

function wallydemo_get_current_path(){
$current_path = explode('=', drupal_get_destination());
  // Extracting URL from $current_path
  if(is_array($current_path) && count($current_path) >= 2) {
    if(trim($current_path[1]) != ''){
      $current_url_full = htmlspecialchars( urldecode($current_path[1]) );
      // Removing query string
      $current_url_elements = explode('?', $current_url_full);
      if(is_array($current_url_elements)) {
        return trim($current_url_elements[0]);
      }
      else{
        return trim($current_url_elements);
      }
     
    }     
    else {
      return $_REQUEST['q'];
    }  
  }	
}


function string_to_numericentities_mod($str) {
  $cvt_char = 
  array (
  	"À"=>"&#192",	
  	"à"=>"&#224",
  	"Á"=>"&#193",
  	"á"=>"&#225",
  	"Â"=>"&#194",
  	"â"=>"&#226",
  	"Ã"=>"&#195",
  	"ã"=>"&#227",
  	"Ä"=>"&#196",
  	"ä"=>"&#228",
  	"Å"=>"&#197",
  	"å"=>"&#229",
  	"Æ"=>"&#198",
  	"æ"=>"&#230",
  	"Ç"=>"&#199",
  	"ç"=>"&#231",
  	"È"=>"&#200",
  	"è"=>"&#232",
  	"É"=>"&#201",
  	"é"=>"&#233",
  	"Ê"=>"&#202",
  	"ê"=>"&#234",
  	"Ë"=>"&#203",
  	"ë"=>"&#235",
  	"Ì"=>"&#203",
  	"ì"=>"&#236",
  	"Í"=>"&#205",
  	"í"=>"&#237",
  	"Î"=>"&#206",
  	"î"=>"&#238",
  	"Ï"=>"&#207",
  	"ï"=>"&#239",
  	"Ñ"=>"&#209",
  	"ñ"=>"&#241",
  	"Ò"=>"&#210",
  	"ò"=>"&#242",
  	"Ó"=>"&#211",
  	"ó"=>"&#243",
  	"Ô"=>"&#212",
  	"ô"=>"&#244",
  	"Õ"=>"&#213",
  	"õ"=>"&#245",
  	"Ö"=>"&#214",
  	"ö"=>"&#246",
  	"Ø"=>"&#216",
  	"ø"=>"&#248",
  	"Ù"=>"&#217",
  	"ù"=>"&#249",
  	"Ú"=>"&#218",
  	"ú"=>"&#250",
  	"Û"=>"&#219",
  	"û"=>"&#251",
  	"Ü"=>"&#220",
  	"ü"=>"&#252",
  	"Ý"=>"&#221",
  	"ý"=>"&#253",
  	"Ÿ"=>"&#376",
  	"ÿ"=>"&#255",
  	"Œ"=>"&#338",
  	"œ"=>"&#339",
  	"Š"=>"&#352",
  	"š"=>"&#353"
  );
  
  
  return strtr($str,$cvt_char);

}
/**
 * Retourne le cache des données de commentaires facebook en fonction d'un nid
 * Si pas de cache, return false
 */
function wallydemo_get_data_facebook_reactions_for_nid($nid){
  $cid = 'spreactions_facebook_'.$nid;
  $cached_data = cache_get($cid);
  return $cached_data;
}

/**
 * 
 * Remplace les caractères accentués par leurs équivalents 
 * non accentués dans une chaîne de caractères...
 * @param $string
 */

function wallydemo_replace_accents($string){
  return str_replace( array('à','á','â','ã','ä', 'ç', 'è','é','ê','ë', 'ì','í','î','ï', 'ñ', 'ò','ó','ô','õ','ö', 'ù','ú','û','ü', 'ý','ÿ', 'À','Á','Â','Ã','Ä', 'Ç', 'È','É','Ê','Ë', 'Ì','Í','Î','Ï', 'Ñ', 'Ò','Ó','Ô','Õ','Ö', 'Ù','Ú','Û','Ü', 'Ý'), array('a','a','a','a','a', 'c', 'e','e','e','e', 'i','i','i','i', 'n', 'o','o','o','o','o', 'u','u','u','u', 'y','y', 'A','A','A','A','A', 'C', 'E','E','E','E', 'I','I','I','I', 'N', 'O','O','O','O','O', 'U','U','U','U', 'Y'), $string); 
}

/**
 * 
 * Retourne le path d'un term de taxonomy dont les éléments sont en minuscules, 
 * sans accents et séparés par un espace 
 * @param int $tid
 */

function wallydemo_get_trimmed_taxonomy_term_path($tid){
  $taxonomy_term_path = strtolower(str_replace("/", " ", wallytoolbox_taxonomy_get_path_by_tid_or_term($tid,2)));
  $string = wallydemo_replace_accents($taxonomy_term_path); 
  return $string;
}
function wallydemo_preprocess_node(&$vars) {
  
  $node = &$vars['node'];
  if ($node->type == "wally_articlepackage" || $node->type == "wally_pollpackage" || $node->type == "wally_gallerypackage"){

    $pub_date = $node->field_publicationdate[0];
    $form_date = date_make_date($pub_date['value'], $pub_date['timezone_db']);
    $form_date = (object)date_timezone_set($form_date, timezone_open($pub_date['timezone']));
    $form_date = unserialize(serialize($form_date));
    $vars['node']->field_publicationdate[0]['safe'] = $form_date->date;

    if ($editorial_update == NULL){
      $node->field_editorialupdatedate[0] = $node->field_publicationdate[0];
    }
    $editorial_update = $node->field_editorialupdatedate[0];
    $form_date = date_make_date($editorial_update['value'], $editorial_update['timezone_db']);
    $form_date = (object)date_timezone_set($form_date, timezone_open($editorial_update['timezone']));
    $form_date = unserialize(serialize($form_date));
    $vars['node']->field_editorialupdatedate[0]['safe'] = $form_date->date;
    
  }

  if ($node->type == "wally_articlepackage"){
    //Ne faire le traitement que si on voit le noeud
    if ($node->nid == arg(1) | $node->preview){
      $vars['bool_node_page'] = TRUE;
      if ($node->preview && isset($node->field_embededobjects_nodes) && !empty($node->field_embededobjects_nodes)) {
        foreach ($node->field_embededobjects_nodes as $delta => $embed) {
          // Fake nid in case of preview
          $node->field_embededobjects_nodes[$delta]->nid = $delta;
        }
      }
      wallydemo_preprocess_node_build_embedded_links($vars);
      wallydemo_preprocess_node_build_embedded_photos($vars);
      wallydemo_preprocess_node_build_embedded_videos($vars);
      wallydemo_preprocess_node_build_embedded_documents($vars);
      wallydemo_preprocess_node_build_embedded_audios($vars);
      $merged_medias = wallydemo_preprocess_node_article_merge_medias($vars);
      $mediaboxItems = array();
      $bottomItems = array();
      wallydemo_preprocess_node_article_dispatch_top_bottom($vars, $merged_medias, $mediaboxItems, $bottomItems);
      
      $linkslist = _wallydemo_get_sorted_links($vars['node']);
      $mainMediaboxObject_html = theme_wallydemo_article_mediaboxobject($mediaboxItems);
      $vars['mediabox_html'] = $mainMediaboxObject_html;
      $vars['bottom_html'] = theme_wallydemo_article_bottom_items($bottomItems);
      $vars['linkslist_html'] = theme_wallydemo_article_links_lists($linkslist);
      $vars['breadcrumb'] = _wallydemo_breadcrumb_display($node->field_destinations[0]["tid"]);

      $htmltags = wallydemo_taxonomy_tags_particle($node);
      $taxonomy = $node->field_destinations[0]["tid"];
      if ($htmltags != "" && $taxonomy != "20"){
        $vars['htmltags_html'] .= "<div class=\"tags\"><h2>Termes associés : </h2>".$htmltags."</div>";
      }
    }
  } elseif ($node->type == "wally_pollpackage"){
    if ($node->nid == arg(1) | $node->preview){
      $vars['bool_node_page'] = TRUE;
      if ($node->preview) {
        if (isset($node->field_embededobjects_nodes) && !empty($node->field_embededobjects_nodes)) {
          foreach ($node->field_embededobjects_nodes as $delta => $embed) {
            // Fake nid in case of preview
            $node->field_embededobjects_nodes[$delta]->nid = $delta + 1;
          }
        }
        $node->field_mainpoll_nodes[0]->nid = 1;
      }
      wallydemo_preprocess_node_build_embedded_photos($vars);
      $merged_medias = $vars['node']->embed_photos;
      $mediaboxItems = array();
      $bottomItems = array();
      wallydemo_preprocess_node_article_dispatch_top_bottom($vars, $merged_medias, $mediaboxItems, $bottomItems);
      $mainMediaboxObject_html = theme_wallydemo_article_mediaboxobject($mediaboxItems);
      $vars['mediabox_html'] = $mainMediaboxObject_html;
      $vars['bottom_html'] = theme_wallydemo_article_bottom_items($bottomItems);
      $vars['breadcrumb'] = _wallydemo_breadcrumb_display($node->field_destinations[0]["tid"]);
      
      $htmltags = wallydemo_taxonomy_tags_particle($node);
      $taxonomy = $node->field_destinations[0]["tid"];
      if ($htmltags != "" && $taxonomy != "20"){
        $vars['htmltags_html'] .= "<div class=\"tags\"><h2>Termes associés : </h2>".$htmltags."</div>";
      }
    }
  }

}
function wallydemo_preprocess_node_build_embedded_links(&$vars){
  if (isset($vars['node'])) {
    $node = &$vars['node'];
    $node->embed_links = array();

    if (isset($node->field_embededobjects_nodes) && !empty($node->field_embededobjects_nodes)) {
      foreach ($node->field_embededobjects_nodes as $delta => $embed) {
        if ($embed->type == 'wally_linktype' && isset($embed->field_link_item[0]['url']) && !empty($embed->field_link_item[0]['url']) && !strstr($embed->field_link_item[0]['url'], 'extref://')) {
          $item = array('embed' => $embed->field_link_item[0]['url']);
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
              if (($module == "emimage" || $module == 'emvideo') && ($item['provider'] != "flickr_sets" && $item['provider'] != "slideshare")){
                //reduction de la taille
                $width = 300;
                $height = 200;
                if ($delta != 0 & $module == 'emvideo') {
                  $width = 425;
                  $height = 350;
                }
                $content = preg_replace('+width=("|\')([0-9]{3})?("|\')+','width="'.$width.'"', $content);
                $content = preg_replace('+height=("|\')([0-9]{3})?("|\')+','height="'.$height.'"', $content);
                if ($element['#item']['data']['thumbnail']['url'] != NULL & $element['#item']['data']['thumbnail']['url'] != ''){
                  $thumb = '<img src= "'.$element['#item']['data']['thumbnail']['url'].'" width = "48" height = "32">';
                } else {
                  $thumb = preg_replace('+width=("|\')([0-9]{3})?("|\')+','width="48"', $content);
                  $thumb = preg_replace('+height=("|\')([0-9]{3})?("|\')+','height="32"', $thumb);
                }
              }
              $node->field_embededobjects_nodes[$delta]->field_link_item[0]['embed'] = $content;
              $title = $node->field_embededobjects_nodes[$delta]->field_link_item[0]['title'];

              if ($module == 'emimage'){
                $group_type = 'photo';
              } elseif ($module == 'emvideo'){
                $group_type = 'video';
              } else {
                $group_type = 'other';
              }

              $node->embed_links[$embed->nid] = array(
                'title' => $title,
                'nid' => $embed->nid,
                'content' => $content,
                'main_size' => $content,
                'thumb' => $thumb,
                'group_type' => $group_type,
                'type' => $embed->type,
                'module' => $module,
                'provider' => $item['provider']
              );
              $emfield = TRUE;
              break;
            }
          }
          if (!$emfield){
            $target = '';
            if ($embed->field_link_item[0]['attributes']['target'] == 1){
              $target = 'target=_blank';
            }
            $content = '<a '.$target.' href = "'.$embed->field_link_item[0]['url'].'">'.$embed->field_link_item[0]['title'].'</a>';
            $title = $embed->field_link_item[0]['title'];
            $thumb = "";
            $module = "";
            $provider = "";
            $node->embed_links[$embed->nid] = array(
              'title' => $title,
              'nid' => $embed->nid,
              'content' => $content, 
              'thumb' => $thumb,
              'group_type' => 'links',
              'type' => $embed->type,
              'module' => $module,
              'provider' => $provider
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
            $mainobject = $package->field_mainpoll_nodes[0];
            $text = $package->field_summary[0]['safe'];
            foreach ($package->field_embededobjects_nodes as $package_embeds){
              if ($package_embeds->type == 'wally_photoobject'){
                $photo_object[] = $package_embeds;
              }
            }
          } elseif ($package->type == 'wally_gallerypackage'){
            $mainobject = $package->field_mainobject_nodes[0];
            $text = $package->field_summary[0]['safe'];
            $photo_object[] = $mainobject;
            foreach ($package->field_embededobjects_nodes as $package_embeds){
              if ($package_embeds->type == 'wally_photoobject'){
                $photo_object[] = $package_embeds;
              }
            }
          }
          $node->embed_package[$embed->nid] = array(
            'title' => $mainobject->title,
            'package' => $package,
            'mainobject' => $mainobject,
            'photo_object' => $photo_object,
            'text' => $text,
            'signature' => $package_signature = _wallydemo_get_package_signature($mainobject),
          );
        }
      }
    }
  }
}

function wallydemo_preprocess_node_build_embedded_photos(&$vars){
  if (isset($vars['node'])) {
    $node = &$vars['node'];
    $node->embed_photos = array();
    
    if (isset($node->field_embededobjects_nodes) && !empty($node->field_embededobjects_nodes)) {
      foreach ($node->field_embededobjects_nodes as $delta => $embed) {
        if ($embed->type == 'wally_photoobject') {
          if ($embed->field_photo3rdparty[0]['value'] == NULL){        
            node_build_content($embed);
            drupal_render($embed->content);
            node_view($embed);
            $node->embed_photos[$embed->nid] = wallydemo_get_photo_infos_and_display($embed);
            $title = $node->embed_photos[$embed->nid]['title'];
            $thumb = $node->embed_photos[$embed->nid]["mini"];
            $content = $node->embed_photos[$embed->nid]["main_size"];
            $module = "";
            $provider = "";
            unset($node->embed_photos[$embed->nid]['mini']);
            $node->embed_photos[$embed->nid] += array(
              'title' => $title,
              'nid' => $embed->nid,
              'content' => $content, 
              'thumb' => $thumb,
              'group_type' => 'photo',
              'type' => $embed->type,
              'module' => $module,
              'provider' => $provider
            );
          } else {
            //third party
            $item = array('embed' => $embed->field_photo3rdparty[0]['value']);
            $field = NULL;
            $emfield = FALSE;
            $item = _emfield_field_submit_id($field, $item, 'emimage');
            if (!empty($item['provider'])) {
              $element = array(
                '#item' => $item,
                '#formatter' => 'default',
                '#node' => $node,
              );
              $content = emimage_field_formatter($field, $element['#item'], $element['#formatter'], $element['#node']);
              //reduction de la taille
              $width = 300;
              $height = 200;
              
              $content = preg_replace('+width=("|\')([0-9]{3})?("|\')+','width="'.$width.'"', $content);
              $content = preg_replace('+height=("|\')([0-9]{3})?("|\')+','height="'.$height.'"', $content);
              if ($element['#item']['data']['thumbnail']['url'] != NULL & $element['#item']['data']['thumbnail']['url'] != ''){
                $thumb = '<img src= "'.$element['#item']['data']['thumbnail']['url'].'" width = "48" height = "32">';
              } else {
                $thumb = preg_replace('+width=("|\')([0-9]{3})?("|\')+','width="48"', $content);
                $thumb = preg_replace('+height=("|\')([0-9]{3})?("|\')+','height="32"', $thumb);
              }
              $node->field_embededobjects_nodes[$delta]->field_photo3rdparty[0]['value'] = $content;
              $title = $node->field_embededobjects_nodes[$delta]->field_photo3rdparty[0]['title'];
              $node->embed_links[$embed->nid] = array(
                'title' => $title,
                'nid' => $embed->nid,
                'content' => $content,
                'main_size' => $content,
                'thumb' => $thumb,
                'group_type' => 'photo',
                'type' => $embed->type,
                'module' => $module,
                'provider' => $item['provider']
              );
              $emfield = TRUE;
            }
          }
        }
      }
    }
  }
}

function wallydemo_preprocess_node_build_embedded_videos(&$vars){
  if (isset($vars['node'])) {
    $node = &$vars['node'];
    $node->embed_videos = array();
    if (isset($node->field_embededobjects_nodes) && !empty($node->field_embededobjects_nodes)) {
      foreach ($node->field_embededobjects_nodes as $delta => $embed) {
        if ($embed->type == 'wally_videoobject') {
          node_view($embed);
          $node->embed_videos[$embed->nid]=wallydemo_get_video_infos_and_display($embed);
          $content = $embed->field_video3rdparty[0]["view"];
          $title = $node->embed_videos[$embed->nid]['title'];
          $thumb = "<img width=\"48\" height=\"32\" src=\"".$node->embed_videos[$embed->nid]['thumbnail']."\">";
          $module = "";
          $provider = "";
           
          $node->embed_videos[$embed->nid] = array(
          	'title' => $title,
            'nid' => $embed->nid,
          	'emcode' => $content,
          	'content' => $content, 
          	'thumb' => $thumb,
          	'group_type' => 'video',
          	'type' => $embed->type,
          	'module' => $module,
          	'provider' => $provider
          );
        }
      }
    }
  }
}

function wallydemo_preprocess_node_build_embedded_documents(&$vars){
  $node = &$vars['node'];
  $node->embed_documents = array();

  if (isset($node->field_embededobjects_nodes) && !empty($node->field_embededobjects_nodes)) {
    foreach ($node->field_embededobjects_nodes as $delta => $embed) {
      if ($embed->type == 'wally_digitalobject') {
        node_view($embed);
        if ($embed->field_object3rdparty[0]['value']){
          $node->embed_videos[$embed->nid] = wallydemo_get_digitalobject_infos_and_display($embed);
          $content = $embed->field_object3rdparty[0]["view"];
          $title = $node->embed_videos[$embed->nid]['title'];
          $thumb = "<img width=\"48\" height=\"32\" src=\"".$node->embed_videos[$embed->nid]['thumbnail']."\">";
          $module = "";
          $provider = "";
        } else {
          $width = '600px';
          $height = '400px';
          $url = url($embed->field_objectfile[0]["filepath"], array('absolute'=>TRUE));
          $content = '<iframe src="http://docs.google.com/viewer?url='.$url.'&embedded=true" width="'.$width.'" height="'.$height.'" style="border: none;"></iframe>';
          $title = $node->embed_videos[$embed->nid]['title'];
          $thumb = "<img width=\"48\" height=\"32\" src=\"".$node->embed_videos[$embed->nid]['thumbnail']."\">";
          $module = "";
          $provider = "";

        }
        $node->embed_videos[$embed->nid] = array(
          	'title' => $title,
            'nid' => $embed->nid,
          	'emcode' => $content,
          	'content' => $content, 
          	'thumb' => $thumb,
          	'group_type' => 'document',
          	'type' => $embed->type,
          	'module' => $module,
          	'provider' => $provider
        );
      }
    }
  }
}

function wallydemo_preprocess_node_build_embedded_audios(&$vars){
  $node = &$vars['node'];
  $node->embed_audios = array();
}
function wallydemo_preprocess_node_article_merge_medias($vars){
  //We use the + operator instead of array_merge to preserve numeric keys.
  return $vars['node']->embed_videos+ $vars['node']->embed_photos+$vars['node']->embed_links+$vars['node']->embed_audios + $vars['node']->embed_documents;
}
function wallydemo_preprocess_node_article_dispatch_top_bottom($vars,$allItems,&$top, &$bottom){
  $node = $vars['node'];
  //First we set the top
  if ($node->field_embededobjects_nodes != NULL){
    foreach ($node->field_embededobjects_nodes as $nid => $embed){
      if ($item = $allItems[$embed->nid]){
        switch ($item['group_type']){
          case 'photo':
            $top[$embed->nid] = $item;
            $switch = TRUE;
            break;
            
          case 'video':
            if ($switch != TRUE and $item['provider'] != 'slideshare'){
              $top[$embed->nid] = $item;
              $switch = TRUE;
            }
            break;
          case 'link':
            break;
        }
      }
    }
  }
  //then we set the bottom
  if ($node->field_embededobjects_nodes != NULL){
    foreach ($node->field_embededobjects_nodes as $nid => $embed){
      if ($item = $allItems[$embed->nid]){
        //We simply put on bottom all content not include in top ...
        if (!isset($top[$embed->nid])){
          $bottom[$embed->nid] = $item;
        }
      }
    }
  }
}
function theme_wallydemo_article_mediaboxobject($mediaboxItems){
  $mainObject_html = "";
  $width = count($mediaboxItems)*300;

  $mainObject_html .= '<div class="allMedias">';
  $mainObject_html .= '<div class="wrappAllMedia"  style="width:'.$width.'px;">';

  foreach ($mediaboxItems as $nid => $item){
    switch($item['group_type']){
       
      case "video":
        if (stripos($item["content"], 'www.youtube.com') !== FALSE) {
          $temp = 'height="350" width="425"';
          $temp2 = 'width="425" height="350"';
          $item["emcode"] = str_replace($temp, "height='200' width='300'", $item["content"]);
          $item["emcode"] = str_replace($temp2, "height='200' width='300'", $item["content"]);
        } else {
          $item["emcode"] = preg_replace('+width=("|\')[0-9]{3}("|\')+','width="300"',$item["content"]);
          $item["emcode"] = preg_replace('+height=("|\')[0-9]{3}("|\')+','height="200"',$item["content"]);
        }
        $mainObject_html .= "<a name=\"".$item['nid']."\" ></a>";
        $mainObject_html .= "<div id=\"item".$item['nid']."\" class=\"item_media\">".$item["content"];
        if ($item["summary"] != ""){
          $mainObject_html .= "<div class=\"pic_description\">".$item["summary"]."</div>";
        }
        $mainObject_html .= "</div>";
        break;
         
      case "photo":
        $mainObject_html .= "<div id=\"item".$item['nid']."\" class=\"item_media\">".$item["main_size"];
        if ($item["credit"] != ""){
          $mainObject_html .= "<p class=\"credit\">".$item["credit"]."</p>";
        }
        if (trim(strip_tags($item["summary"])) != ""){
          $mainObject_html .= "<p class=\"pic_description\">".strip_tags($item["summary"])."</p>";
        }
        $mainObject_html .= "</div>";
        break;
    }
  }

  $mainObject_html .= '</div>';
  $mainObject_html .= '</div>';
  $galMedias = count($mediaboxItems) > 1;
  
  if ($galMedias == TRUE){
    $mainObject_html .= "<div class=\"bloc-01 pf_article\"><h2>Medias</h2><div class=\"inner-bloc\"><ul class=\"mini-pagination\">";
    foreach ($mediaboxItems as $nid=>$embed){
      if(TRUE || ($emblink['type']=="emimage"||$emblink['type']=="emvideo")&&($emblink['provider']!='flickr_sets'&&$emblink['provider']!='slideshare')){
        $mainObject_html .="<li><a href=\"#item".$embed['nid']."\">\n\t".$embed['thumb']."</a>\n</li>\n";
      }
    }
    $mainObject_html .= "</ul></div></div>";
  }

  return  $mainObject_html;
}
function theme_wallydemo_article_bottom_items($bottomItems){
  if (count($bottomItems)){
    $bottom_html .= '<div class="digital-wally_digitalobject">';
    $bottom_html .= '  <ul>';
    foreach ($bottomItems as $id=>$item){
      $bottom_html .= "    <li class=".$item["group_type"].">";
      $bottom_html .= "      <h3>".$item["title"]."</h3>";
      $bottom_html .= "      <span>".$item["content"]."</span>";
      $bottom_html .= "    </i>";
    }
    $bottom_html .= "  </ul>";
    $bottom_html .= "</div>";
  }
  return $bottom_html;
}

function theme_wallydemo_article_links_lists($linkslist){
  foreach ($linkslist as $linksList){
    if (isset($linksList["title"])){
      $list_titre = $linksList["title"];
      $links_html .= "<div class=\"bloc-01\"><h2>".$list_titre."</h2><div class=\"inner-bloc\"><ul>";
      if (isset($linksList["links"])){
        foreach($linksList["links"] as $link){
          $link_url = $link["url"];
          $link_title = $link["title"];
          $link_target = $link["target"];
          $link_type = $link["type"];
          if ($link["packagelayout"] == 'Article Wiki') {
            $links_html .= "<li class=\"media-dossier\">" ."<a class=\"novisited\" href=\"".$link_url."\" target=\"".$link_target."\">".$link_title."</a></li>";
          } else {
            $links_html .= "<li class=\"media-press\">" ."<a href=\"".$link_url."\" target=\"".$link_target."\">".$link_title."</a></li>";
          }
        }
      }
      $links_html .= "</ul></div></div>";
    }
  }

  return $links_html;
}
/*
* Renvoi les résultats d'un poll
*/
function wallydemo_displaypollresult($node){
  $content = '<div class="bloc_sondage_results">';
  foreach ($node->webform['components'] as $cid => $component){

    $content .= '<h3>'.$component['name'].'</h3>';
    $items = explode("\n", $component['extra']['items']);
    if ($component['extra']['multiple'] == 0){
      $result = db_query("SELECT count(data) as total FROM {webform_submitted_data} WHERE nid = %d AND cid = %d", $node->nid, $cid);
      $total = db_fetch_object($result);
      $total = $total->total;
      foreach ($items as $item){
        $values = explode('|', $item);
        $result = db_query("SELECT count(data) as count FROM {webform_submitted_data} WHERE nid = %d AND cid = %d AND data = '%s'", $node->nid, $cid, $values['0']);
        $count = db_fetch_object($result);
        $content .= '<div class="text">'.$values[1].'</div>';
        if ($total != 0){
          $percent = round($count->count/$total, 2)*100;
        } else {
          $percent = 0;
        }
        $content .= '<div class="bar">';
        $content .= '<div class="foreground" style="width: '.$percent.'%;"></div>';
        $content .= '</div>';
        $content .= '<div class="percent"> '.$percent.'% ('.$count->count.' '.t('vote').') </div>';
      }
      $content .= '<div class="total">'.t('Total votes:').' '.$total.' </div>';
    } else {
      $max = 0;
      $choices = array();
      foreach ($items as $item){
        $values = explode('|', $item);
        $result = db_query("SELECT count(data) as count FROM {webform_submitted_data} WHERE nid = %d AND cid = %d AND data = '%s'", $node->nid, $cid, $values['0']);
        $count = db_fetch_object($result);
        $choices[] = array('value' => $values[1], 'count' => $count->count);
        if ($max < $count->count){
          $max = $count->count;
        }
      }
      foreach ($choices as $choice){
        $content .= '<div class="text">'.$choice['value'].'</div>';
        if ($max != 0){
          $percent = round($choice['count']/$max, 2)*100;
        } else {
          $percent = 0;
        }
        $content .= '<div class="bar">';
        $content .= '<div class="foreground" style="width: '.$percent.'%;"></div>';
        $content .= '</div>';
        $content .= '<div class="percent"> '.$choice['count'].' '.t('vote').' </div>';
      }
      $content .= '<div class = "total"></div>';
    }
  }
  $content .= '</div>';
  return $content;
}
function wallydemo_get_node_uri($node) {
  if (isset($node->field_externaluri[0]['value']) && !empty($node->field_externaluri[0]['value'])) {
    return check_url($node->field_externaluri[0]['value']);
  } else {
    return '/'.check_url(drupal_get_path_alias('node/'.$node->nid));
  }
}
