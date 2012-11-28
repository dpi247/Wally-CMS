<?php
// $Id: views-view-row-rss.tpl.php,v 1.1 2008/12/02 22:17:42 merlinofchaos Exp $
/**
 * @file views-view-row-rss.tpl.php
 * Default view template to display a item in an Flow feed.
 *
 * @ingroup views_templates
 */

module_load_include('inc', 'jsonexporter', 'templates/theme');

global $base_url;
$json_product=$variables['view']->jsonexporter_product;

$row_view_index=$variables["view"]->row_index;
$view_offset = $variables["view"]->pager["offset"];
$row_index=$row_view_index+$view_offset;

$field_mainstory_nodes = $node->field_mainstory_nodes[0];

jsonexporter_build_embedded_links(&$node);

$crea_date = $node->field_creationdate[0];
$form_crea_date = date_make_date($crea_date['value'], $crea_date['timezone_db']);
$form_crea_date = (object)date_timezone_set($form_crea_date, timezone_open($crea_date['timezone']));
$form_crea_date = unserialize(serialize($form_crea_date));
$formatted_creadate = $form_crea_date->date;

$pub_date = $node->field_publicationdate[0];
$form_pub_date = date_make_date($pub_date['value'], $pub_date['timezone_db']);
$form_pub_date = (object)date_timezone_set($form_pub_date, timezone_open($pub_date['timezone']));
$form_pub_date = unserialize(serialize($form_pub_date));
$formatted_pubdate = $form_pub_date->date;

$editorial_update = $node->field_editorialupdatedate[0];
if ($editorial_update['value'] != NULL){
  $form_edit_date = date_make_date($editorial_update['value'], $editorial_update['timezone_db']);
  $form_edit_date = (object)date_timezone_set($form_edit_date, timezone_open($editorial_update['timezone']));
  $form_edit_date = unserialize(serialize($form_edit_date));
  $formatted_editorialupdate = $form_edit_date->date;
} else {
  $formatted_editorialupdate = $formatted_pubdate;
}

$json = array();
$json['nid']=$node->nid;
$json['comment']=$node->comment;
$json['title']= $field_mainstory_nodes->title;
$json['creationDate']=strtotime($formatted_creadate);
$json['pubDate']=strtotime($formatted_pubdate);
$json['editorialUpdate']=strtotime($formatted_editorialupdate);
$json['updateDate']=intval($node->changed);
$json['type']=$node->type;
$json['chapo']=$field_mainstory_nodes->field_textchapo[0]['value'];
$json['freeaccess']=$node->field_freeaccess[0]['value'];

jsonexporter_node_author($node);
/**
 * Récupératin des auteurs (avec lien vers le page de taxonomie).
 */
$authors_list = array();
if (count($node->authors) > 0) {
  //gestion CCI
  foreach ($node->authors as $author) {
    $authors_list[] = check_plain($author->title);
  }
} else {
  //gestion Hermes
  $signature = jsonexporter_get_package_signature($field_mainstory_nodes);
  if (!empty($signature)) {
    $authors_list[] = $signature;
  }
}

/*
 * Applique les filtres (filters)
*/
$texte_article = '';
if ($field_mainstory_nodes->field_textbody[0]['value'] != '' & $field_mainstory_nodes->field_textbody[0]['value'] != NULL){
  $texte_article = check_markup($field_mainstory_nodes->field_textbody[0]['value'], $field_mainstory_nodes->field_textbody[0]['format'], FALSE);
  $texte_article = _jsonexporter_clear_paragraph($texte_article);
}
$json['body']=$texte_article;

$json['foretitle']=$field_mainstory_nodes->field_textforetitle[0]['value'];
$json['subTitle']=$field_mainstory_nodes->field_textsubtitle[0]['value'];
$json['barette']=$field_mainstory_nodes->field_textbarette[0]['value'];
$json['byline']=$field_mainstory_nodes->field_byline[0]['value'];
$json['copyright']=$field_mainstory_nodes->field_copyright[0]['value'];
$json['authors']=$authors_list;
$aliases = wallytoolbox_get_all_aliases('node/'.$node->nid);
$json['url']= $aliases[0];
$json['mainDestination']=$node->field_destinations[0]["tid"];
$json['mainSection']=$node->field_destinations[0]["tid"].$node->field_destinations[0]["target"];
$json['relatedObjects'] = array();

foreach($node->field_embededobjects_nodes as $one){
  switch($one->type){

    case "wally_photoobject":
      $embed_one = jsonexporter_get_photo_infos_and_display($one);

      $photoobject= array(
          //$json['relatedObjects'][] = array(
          'nid'=> $embed_one["nid"],
          'type'=> 'wally_photoobject',
          'caption'=> !empty($embed_one["summary"]) ? $embed_one["summary"] : $embed_one['title'],
          'credit'=> $embed_one["credit"]
      );

      if (is_array($json_product->presets)) {
        foreach($json_product->presets as $preset_id){
          if($preset_id!=0){
            $preset=imagecache_preset($preset_id);
            $presetname=$preset["presetname"];
            $photoobject['crop'][$presetname]= array(
                'url'=> ($base_url."/sites/default/files/imagecache/". $presetname."/".$embed_one["fullpath"])
            );
          }
        }
      }

      $json['relatedObjects'][] = $photoobject;
      break;

    case "wally_videoobject":
    		$embed_one = jsonexporter_get_video_infos_and_display($one);

    		$videoobject= array(
    		    //$json['relatedObjects'][] = array(
    		    'nid'=> $embed_one["nid"],
    		    'type'=> 'wally_videoobject',
    		    'titre'=> $embed_one["title"],
    		    'summary'=> $embed_one["summary"],
    		    'url'=> $embed_one["link"],
    		    'thumbnail'=> $embed_one["thumbnail"],
    		    'emcode'=> $embed_one["emcode"],
    		    'credit'=> $embed_one["credit"]
    		);
    		 
    		$json['relatedObjects'][] = $videoobject;

    		break;
    
    case "wally_textobject":
      $embed_one = jsonexporter_get_text_infos_and_display($one);

    		$textobject= array(
    		    //$json['relatedObjects'][] = array(
    		    'nid'=> $embed_one["nid"],
    		    'type'=> 'wally_textobject',
    		    'titre'=> $embed_one["title"],
            'chapo' => $embed_one['chapo'],
            'body' => $embed_one['body'],
    		);

      $json['relatedObjects'][] = $textobject;
      break;

    case "wally_audioobject":
      $embed_one = jsonexporter_get_audio_infos_and_display($one);
      $json['relatedObjects'][] = array(
          'nid'=> $embed_one["nid"],
          'type'=> 'wally_audioobject',
          'filemime'=> $embed_one["filemime"],
          'titre'=> $embed_one["title"],
          'summary'=> $embed_one["summary"],
          'url'=> $embed_one["link"],
          'thumbnail'=> $embed_one["thumbnail"]
      );

    		break;

    case "wally_digitalobject":
      $embed_one = jsonexporter_get_digitalobject_infos_and_display($one);
      $dummy = array(
          'nid'=> $embed_one["nid"],
          'type'=> 'wally_digitalobject',
          'linkType'=> $embed_one["linkType"],
          'label'=> $embed_one["title"],
          'url'=> $embed_one["url"]
      );

      if($embed_one["thumbnail"]){
        $dummy["thumnail"]["url"]= $embed_one["thumbnail"];
        $dummy["thumnail"]["mime"]= $embed_one["mime"];
        $dummy["thumnail"]["link"]= $embed_one["thumbnail_img"];
      }
      $json['relatedObjects'][] = $dummy;

    		break;
  }
}

foreach($node->embed_links as $two){
  switch($two["group_type"]){

    case "video":
      //$json['embed_links'][] = array(
      $json['relatedObjects'][] = array(
      'nid'=> $two["nid"],
      'titre'=> $two["title"],
      'type'=> 'wally_videoobject',
      'thumbnail'=>$two["thumb"],
      'content'=>$two["content"],
      'module'=>$two["module"],
      'provider'=>$two["provider"]
      );
      break;

    case "extref":
      $embed_link= node_load(array('nid'=>$two["nid"]));
      $embed_pack= node_load(array('nid'=>$embed_link->field_internal_link[0]["nid"]));
      wallycontenttypes_packagepopulate($embed_pack);
       
      //authors
      $embed_pack_authors_list = NULL;
      if (count($embed_pack->authors) > 0) {
        //gestion CCI
        foreach ($embed_pack->authors as $author) {
          $embed_pack_authors_list[] = check_plain($author->title);
        }
      } else {
        //gestion Hermes
        $signature = jsonexporter_get_package_signature($embed_pack->field_mainstory_nodes[0]);
        if (!empty($signature)) {
          $embed_pack_authors_list[] = $signature;
        }
      }
      
      // $json['relatedObjects'][] = array(
      $extref= array(
      	'nid'=> $embed_pack->nid,
      	'titre'=> $embed_pack->title,
      	'type'=> $embed_pack->type,
        'title'=> $embed_pack->field_mainstory_nodes[0]->title,
        'chapo'=> $embed_pack->field_mainstory_nodes[0]->field_textchapo[0]['value'],
        'body'=> $embed_pack->field_mainstory_nodes[0]->field_textbody[0]['value'],
        'authors' => $embed_pack_authors_list,
      );

      foreach($embed_pack->field_embededobjects_nodes as $one){
        switch($one->type){
          case "wally_photoobject":
            $embed_one = jsonexporter_get_photo_infos_and_display($one);
            $photoobject= array(
                //$json['relatedObjects'][] = array(
                'nid'=> $embed_one["nid"],
                'type'=> 'wally_photoobject',
                'caption'=> $embed_one["summary"],
                'credit'=> $embed_one["credit"]
            );
            if (is_array($json_product->presets)) {
              foreach($json_product->presets as $preset_id){
                if($preset_id!=0){
                  $preset=imagecache_preset($preset_id);
                  $presetname=$preset["presetname"];
                  $photoobject['crop'][$presetname]= array(
                      'url'=> ($base_url."/sites/default/files/imagecache/". $presetname."/".$embed_one["fullpath"])
                  );
                }
              }
            }
            $extref['relatedObjects'][] = $photoobject;
            break;
          case "wally_videoobject":
            $embed_one = jsonexporter_get_video_infos_and_display($one);

            //$json['relatedObjects'][] = array(
            $videoobject= array(
                'nid'=> $embed_one["nid"],
                'type'=> 'wally_videoobject',
                'titre'=> $embed_one["title"],
                'summary'=> $embed_one["summary"],
                'url'=> $embed_one["link"],
                'thumbnail'=> $embed_one["thumbnail"],
                'emcode'=> $embed_one["emcode"]	,
                'credit'=> $embed_one["credit"]
            );
            $extref['relatedObjects'][] = $videoobject;
            break;

          case "wally_audioobject":
            $embed_one = jsonexporter_get_audio_infos_and_display($one);
            $audioobject= array(
                //$json['relatedObjects'][] = array(
                'nid'=> $embed_one["nid"],
                'type'=> 'wally_audioobject',
                'filemime'=> $embed_one["filemime"],
                'titre'=> $embed_one["title"],
                'summary'=> $embed_one["summary"],
                'url'=> $embed_one["link"],
                'thumbnail'=> $embed_one["thumbnail"]
            );
            $extref['relatedObjects'][] = $audioobject;

            break;

          case "wally_digitalobject":
            $embed_one = jsonexporter_get_digitalobject_infos_and_display($one);
            $dummy = array(
                'nid'=> $embed_one["nid"],
                'type'=> 'wally_digitalobject',
                'linkType'=> $embed_one["linkType"],
                'label'=> $embed_one["title"],
                'url'=> $embed_one["url"]
            );

            if($embed_one["thumbnail"]){
              $dummy["thumnail"]["url"]= $embed_one["thumbnail"];
              $dummy["thumnail"]["mime"]= $embed_one["mime"];
              $dummy["thumnail"]["link"]= $embed_one["thumbnail_img"];
            }
            $extref['relatedObjects'][] = $dummy;
            break;
        }
      }

      $json['relatedObjects'][] =	$extref;
       
      break;
  }
}

// field_linkedobjects
foreach($node->field_linkedobjects_nodes as $liensObjects){
  if($liensObjects->type=='wally_linkslistobject'){
    $ess =  array(
        'nid'=> $liensObjects->nid,
        'type'=> "wally_linkslistobject",
        'titre'=> $liensObjects->title
    );
    foreach ($liensObjects->field_links_list_nodes as $lienObj ){
      if ($lienObj->field_link_item['0']['url'] && !empty($lienObj->field_link_item['0']['url'])){
        $ess2 = $lienObj->field_link_item['0']['url'];
      } else {
        $ess2= 'node/'.$lienObj->field_internal_link['0']['nid'];
      }
      $ess["links"][] =  array(
          'nid'=> $lienObj->nid,
          'type'=> 'wally_linktype',
          'path'=>$lienObj->path,
          'title'=> !empty($lienObj->field_link_item['0']['title']) ? $lienObj->field_link_item['0']['title'] : $lienObj->title,
          'url'=>$ess2
      );
    }
    $json['relatedObjects'][] = $ess;
  }
}

if ($row_index > 0) {
  print (",");
}
print drupal_to_js($json);
