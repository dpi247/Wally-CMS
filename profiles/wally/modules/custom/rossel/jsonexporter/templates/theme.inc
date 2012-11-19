<?php

function jsonexporter_build_embedded_links(&$node) {
  $node->embed_links = array();
  if (isset($node->field_embededobjects_nodes) && !empty($node->field_embededobjects_nodes)) {
    foreach ($node->field_embededobjects_nodes as $delta => $embed) {
      if ($embed->type == 'wally_linktype' && isset($embed->field_link_item[0]['url']) && !empty($embed->field_link_item[0]['url']) && !strstr($embed->field_link_item[0]['url'], 'extref://')) {
        $item = array('embed' => $embed->field_link_item[0]['url']);

        $modules = array('emvideo', 'emother', 'emimage', 'emaudio', 'embonus', 'emimport', 'eminline', 'emthumb', 'emwave', 'image_ncck', 'video_cck', 'slideshare');
        $emfield = FALSE;
        
        //default values
        $thumb = NULL;
        $module = "";
        $provider = "";
        
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
                $thumb = $element['#item']['data']['thumbnail']['url'];
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
        $content = node_view($package);
        $title = $package->title;
        $node->embed_links[$embed->nid] = array(
            'title' => $title,
            'nid' => $embed->nid,
            'content' => $content,
            'thumb' => $thumb,
            'group_type' => 'extref',
            'type' => $embed->type,
            'module' => $module,
            'provider' => $provider
        );
      }
    }
  }
}

function _jsonexporter_clear_paragraph($texte) {
  // On enlève les balises p vide
  $texte = preg_replace("/<P>( *)<\/P>/i", "", $texte);
  $texte = preg_replace("/<P>( |\n)?<br ?\/>( |\n)?<\/P>/i", "", $texte);
  return $texte;
}

/**
 * Render an array with a photoObject's infos for theming operations
 */
function jsonexporter_get_photo_infos_and_display($photoObject,$template="default"){
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
        $photo['emcode'] = theme('imagecache', '475x317',$photo['fullpath'],'', '');
        $photo['main_size'] = theme('imagecache', '475x317',$photo['fullpath'],'', '');
        $photo['main_url'] = imagecache_create_url('475x317', $photo['fullpath']);
        $photo['mini'] = theme('imagecache', 'article_48x32', $photo['fullpath'],'', '');
        $photo['ours'] = theme('imagecache', '185x123', $photo['fullpath'],'', '');
        $photo['medium'] = theme('imagecache', '236x158', $photo['fullpath'], '', '');
      }
      break;
  }

  return $photo;
}

/**
 * Render an array with a videoObject's infos for theming operations
 */
function jsonexporter_get_video_infos_and_display($videoObject){
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
 * Render an array with a videoObject's infos for theming operations
 */
function jsonexporter_get_text_infos_and_display($textObject){
  $text = array();
  $text["nid"] = $textObject->nid;
  $text["type"] = $textObject->type;
  $text['title'] = $textObject->title;
  $text['chapo'] = $textObject->field_textchapo[0]['value'];
  $text['body'] = $textObject->field_textbody[0]['value'];
  return $text;
}

/**
 * Render an array with a audioObject's infos for theming operations
 */
function jsonexporter_get_audio_infos_and_display($audioObject){
  $audio = array();
  $audio["nid"] = $audioObject->nid;
  $audio["type"] = $audioObject->type;
  $audio['emcode'] = $audioObject->content['group_audio']['group']['field_audio3rdparty']['field']['items'][0]['#children'];
  $audio['title'] = $audioObject->title;
  $audio['summary'] = $audioObject->field_summary[0]['value'];
  $audio['credit'] = $audioObject->field_copyright[0]['value'];

  return $audio;
}

/**
 * Render an array with a digitalObject's infos for theming operations
 */
function jsonexporter_get_digitalobject_infos_and_display($digitalObject){
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
 * Render the package's signature
 */
function jsonexporter_get_package_signature($signature_field){
  if ($signature_field->field_copyright[0]["value"] != ""){
    $package_signature = '';
    list($first, $rest) = explode(',', $signature_field->field_copyright[0]["value"]);
    if (ctype_upper($first)){
      //Hermes signature
      $virgule = '';
      $authors = explode(';', $signature_field->field_copyright[0]["value"]);
      foreach ($authors as $auth){
        list($nom, $prenom) = explode(',', $auth);
        $package_signature .= $virgule.ucwords(strtolower($prenom.' '.$nom));
        $virgule = ', ';
      }
    } else {
      //Hermes source
      $package_signature = $signature_field->field_copyright[0]["value"];
    }
  } else {
    $package_signature = variable_get("default_package_signature",'');
  }
  return $package_signature;
}

/**
 * Récupération de(s) (l')auteur(s) d'un package.
 *
 * ## Logique de récupération des autheur(s)
 *
 * 1) Node authors
 * 2) Package Authors
 *
 * @return array of authors
 */
function jsonexporter_node_author(&$node) {
  if (isset($node->field_authors_nodes[0])) {
    $node->authors = $node->field_authors_nodes;
  } elseif (isset($node->field_authors[0]['value'])) {
    $author_term = taxonomy_get_term($node->field_authors[0]['value']);
    $node->authors[0] = new stdClass();
    $node->authors[0]->title = $author_term->name;
  } elseif (isset($node->field_packageauthors_nodes[0])) {
    $node->authors = $node->field_packageauthors_nodes;
  } elseif (isset($node->field_packageauthors[0]['value'])) {
    $author_term = taxonomy_get_term($node->field_packageauthors[0]['value']);
    $node->authors[0] = new stdClass();
    $node->authors[0]->title = $author_term->name;
  } else {
    // Rien sur le package, allons voir sur le mainobject/txtobject;
    if($node->type == "wally_articlepackage") {
      $mainstory = $node->field_mainstory_nodes[0];
    } else {
      $mainstory = $node->field_mainobject_nodes[0];
    }
    if (isset($mainstory->field_authors_nodes[0])) {
      $node->authors = $mainstory->field_authors_nodes;
    } else {
      $node->authors = NULL;
    }
  }
}
