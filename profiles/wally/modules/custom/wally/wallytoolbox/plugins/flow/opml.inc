<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('OPML'),
  'description' => t('Fetch an OPML feed.'),
  'types' => array('Blog Spirit OPML')
);

function wallytoolbox_opml_flow_settings_form(&$form, &$form_state) {
  $conf = $form_state['conf'];
  
  $form['maxitem'] = array (
    '#type'             => 'textfield',
    '#title'            => t('Max item to fetch for this feed'),
    '#default_value'    => $conf['maxitem'],
    '#required'         => TRUE,
  );
  
  $form['channel_img'] = array(
    '#title'            => t('Use of default image.'),
    '#type'             => 'checkbox',
    '#default_value'    => $conf['channel_img'],
  );
}

function wallytoolbox_opml_flow_build(&$feed_infos) {
  $struct_array = wallytoolbox_flow_to_array_init_array();
  
  $maxitem = (isset($feed_infos['conf']['maxitem']) && $feed_infos['conf']['maxitem'] > 0) ? $feed_infos['conf']['maxitem'] : 1000;
  $feed_xml = drupal_http_request($feed_infos['uri']);
  if ($feed_xml && $feed_xml->code == '200') {
    $dom = new DomDocument();
    $dom->LoadXML($feed_xml->data);
    $channels = $dom->getElementsByTagName('channel');
    $opml_tags = $dom->getElementsByTagName('opml');
    $media_namespace = $opml_tags->item(0)->getAttributeNS('http://www.w3.org/2000/xmlns/', 'media');
    $channel_last_build = 0;

    if ($channels->item(0)) {
      $feed_destination = explode('/', $feed_infos['destination']);
      $sql_last_build = $feed_infos['last_build'];
      $feed_name = check_plain($feed_infos['name']);
      $channel_last_build = strtotime(_wallytoolbox_checkifelementexists($channels->item(0), 'lastBuildDate'));

      if ($channel_last_build > $sql_last_build || empty($channel_last_build) || !isset($channel_last_build) || $channel_last_build == 0) {
        $feed_infos['last_build'] = $channel_last_build;
        
        $feeds_md5 = isset($feed_infos['feeds_md5']) ? unserialize($feed_infos['feeds_md5']) : array();
        $new_feeds_md5 = array();
        $temp_struct_array = array();
        
        foreach ($channels->item(0)->getElementsByTagName('item') as $item) {
          $temp_struct_array = wallytoolbox_opml_item_to_array($channels->item(0), $item, $media_namespace, $feed_infos);
          
          $elem_package = $temp_struct_array;
          $elem_md5 = md5($elem_package['PublicationDate'].$elem_package['PackageTitle']['value'].$elem_package['ExternalURI']['value'].$elem_package['MainStory']['TextBody']['value']);
          if (!isset($feeds_md5[$elem_md5]))
            $struct_array['Packages']['Package'][] = $temp_struct_array;
          
          $new_feeds_md5[$elem_md5] = TRUE;
          
          if (sizeof($struct_array['Packages']['Package']) >= $maxitem) break;
        }
        
        if (empty($struct_array['Packages']['Package']))
          _wallytoolbox_opml_set_log($struct_array['Packages'], FALSE, t('No modifications for feed').' : '.check_plain($feed_infos['name']));
        
        $feed_infos['feeds_md5'] = $new_feeds_md5;
      } else {
        _wallytoolbox_opml_set_log($struct_array['Packages'], FALSE, t('Channel not rebuild for feed').' : '.check_plain($feed_infos['name']));
      }
    } else {
      _wallytoolbox_opml_set_log($struct_array['Packages'], FALSE, t('No channel in the feed').' : '.check_plain($feed_infos['name']));
    }
  } else {
    _wallytoolbox_opml_set_log($struct_array['Packages'], FALSE, t('No HTTP response for feed').' : '.$feed_infos['name']);
  }
  
  return $struct_array;
}

/**
 * Build an array from a opml item. The array structure is based on the
 * structure required by Wallymport.
 * 
 * The structure follows the rule :
 * $array[element_name][attribute_name] = attribute_value
 * or
 * $array[element_name][subelement_name] = subelement (array)
 * And so on..
 * 
 * If the subelement is just a string, use 'value' as subelement_name.
 * 
 * @param $channel
 *   The full opml channel, needed because of the media fields which can
 *   be written anywhere in the feed.
 * @param $item
 *   The opml to be transformed.
 * @param $feed_infos
 *   The infos of the feed as recorded in the DB.
 * 
 * @return
 *   The array-restructured item.
 */
function wallytoolbox_opml_item_to_array($channel, $item, $media_namespace = 'http://search.yahoo.com/mopml/', $feed_infos = array('name'=>'opml', 'destination'=>'0///', 'channel_img'=>0, 'prefix'=>'')) {
  $array_package = wallytoolbox_flow_to_array_new_package();
  
  list($feed_name, $prefix) = wallytoolbox_opml_item_to_array_set_package($array_package, $item, $feed_infos);
  wallytoolbox_opml_item_to_array_set_mainstory($array_package, $item, $feed_name, $prefix);

  $enclosure = $item->getElementsByTagName('enclosure');
  if ($enclosure->item(0) && $enclosure->item(0)->getAttribute('type') && strstr($enclosure->item(0)->getAttribute('type'), 'image')) {
    wallytoolbox_opml_item_to_array_set_enclosure($array_package, $enclosure, $feed_name, $prefix);
  }

  $media_contents = $item->getElementsByTagNameNS($media_namespace, 'content');
  if ($media_contents->item(0)) {
    wallytoolbox_opml_item_to_array_set_medias($array_package, $channel, $item, $media_contents, $feed_name, $prefix, $media_namespace);
  }

  $channel_img = $channel->getElementsByTagName('image');
  if ($feed_infos['conf']['channel_img'] && !isset($array_package['EmbeddedContent']) && $channel_img->item(0)) {
    wallytoolbox_opml_item_to_array_set_image($array_package, $channel_img->item(0));
  }
  
  if ($settings['get_distant_img'] == 1 && isset($array_package['EmbeddedContent'])) {
    $new_file_path = 'Flows/';
    $file_temp_dir = file_directory_path().'/'.$new_file_path;
    file_check_directory($file_temp_dir, 1);
    $new_file_path .= $settings['caid'];
    $new_file_path .= '_'.date('Ymd', time());
    foreach ($array_package['EmbeddedContent']['EmbeddedObjects']['Object'] as $object_key => $object) {
      if ($object['type'] == 'PhotoObjectType') {
        $file_uri = $object['FileURI']['value'];
        $array_package['EmbeddedContent']['EmbeddedObjects']['Object'][$object_key]['LocaleImage'] = _wimprthelp_create_file_from_uri($file_uri, $new_file_path);
      }
    }
  }

  return $array_package;
}

/**
 * Get package available parameters from an opml feed item domElement
 * and set an array item according to the structure defined in
 * Wallymport.
 * 
 * @param array &$array_item
 *   The array item under construction.
 * @param domElement $item
 *   The XML opml feed item domElement.
 * @param array $feed_infos
 *   General informations about the feed being analyzed.
 */
function wallytoolbox_opml_item_to_array_set_package(&$array_item, $item, $feed_infos) {
  $feed_name = $feed_infos['name'];
  $prefix = ($feed_infos['prefix'] != '') ? $feed_infos['prefix'] : $feed_name;
  $feed_destination = explode('/',$feed_infos['destination']);
  $array_item['PackageID'] = _wallytoolbox_checkifelementexists($item, 'guid', 'none', 'link', $feed_name);
  $channel_pubDate = strtotime(_wallytoolbox_checkifelementexists($item, 'pubDate'));
  $pubDate = date('Y-m-d', $channel_pubDate).'T'.date('H:i:sP', $channel_pubDate);
  $array_item['PublicationDate'] = $pubDate;
  $array_item['CreationDate'] = date('Y-m-d').'T'.date('H:i:sP');
  $array_item['PackageTitle']['value'] = _wallytoolbox_checkifelementexists($item, 'title');
  $array_item['ExternalReference']['value'] = _wallytoolbox_checkifelementexists($item, 'guid', 'none', 'link', $feed_name, $prefix);
  $array_item['ProductID']['value'] = variable_get('wallymport_productID', $_SERVER['HTTP_HOST']);
  $term = taxonomy_get_term($feed_destination[0]);
  $array_item['Destinations']['Destination']['DestinationRank'] = $feed_destination[3];
  $array_item['Destinations']['Destination']['Position'] = $feed_destination[1];
  $array_item['Destinations']['Destination']['DocumentLayout'] = $feed_destination[2];
  $array_item['Destinations']['Destination']['DestinationPath']['value'] = is_object($term) ? wallytoolbox_taxonomy_get_path_by_tid_or_term($term->tid) : '';
  $array_item['ExternalURI']['value'] = _wallytoolbox_checkifelementexists($item, 'link');

  return array($feed_name, $prefix);
}

/**
 * Get mainstory available parameters from an opml feed item domElement
 * and set an array item according to the structure defined in
 * Wallymport.
 * 
 * @param array &$array_item
 *   The array item under construction.
 * @param domElement $item
 *   The XML opml feed item domElement.
 * @param string $feed_name
 *   The name of the feed being analyzed.
 */
function wallytoolbox_opml_item_to_array_set_mainstory(&$array_item, $item, $feed_name, $prefix) {
  $array_item['MainStory'] = wallytoolbox_flow_to_array_new_textobject();
  $array_item['MainStory']['ObjectID'] = _wallytoolbox_checkifelementexists($item, 'title');
  $array_item['MainStory']['PublicationDate'] = $array_item['PublicationDate'];
  $array_item['MainStory']['CreationDate'] = $array_item['CreationDate'];
  $array_item['MainStory']['Title']['value'] = _wallytoolbox_checkifelementexists($item, 'title');
  $array_item['MainStory']['ExternalReference']['value'] =  _wallytoolbox_checkifelementexists($item, 'guid', 'none', 'link', $feed_name, $prefix);
  $array_item['MainStory']['Taxonomy']['Vocabulary']['VocabularyName'] = 'opml feeds';
  $categories = $item->getElementsByTagName('category');
  if ($categories->item(0)) {
    foreach ($categories as $category) {
      $array_item['MainStory']['Taxonomy']['Vocabulary']['Term'][] = $feed_name.'/'.str_replace('/', '-', $category->firstChild->wholeText);
    }
  }
  $array_item['MainStory']['Copyright']['value'] = _wallytoolbox_checkifelementexists($item, 'author');
  $array_item['MainStory']['TextTitle']['value'] = _wallytoolbox_checkifelementexists($item, 'title');
  $array_item['MainStory']['TextBody']['value'] = _wallytoolbox_checkifelementexists($item, 'description');
}

/**
 * Get available parameters from an enclosure domElement of a opml feed
 * item and set an array item according to the structure defined in
 * Wallymport.
 * 
 * @param array &$array_item
 *   The array item under construction.
 * @param domElement $enclosure
 *   The enclosure domElement from the XML opml feed item.
 * @param string $feed_name
 *   The name of the feed being analyzed.
 */
function wallytoolbox_opml_item_to_array_set_enclosure(&$array_item, $enclosure, $feed_name, $prefix) {
  $array_item['EmbeddedContent'] = array();
  $array_item['EmbeddedContent']['EmbeddedObjects'] = array();
  $array_item['EmbeddedContent']['EmbeddedObjects']['Object'] = array();
  $tempobject = wallytoolbox_flow_to_array_new_photoobject();
  $tempobject['ObjectID'] = _wallytoolbox_checkifelementexists($item, 'title');
  $tempobject['PublicationDate'] = $array_item['PublicationDate'];
  $tempobject['MimeType'] = $enclosure->item(0)->getAttribute('type');
  $tempobject['Title']['value'] = $enclosure->item(0)->getAttribute('url');
  $tempobject['ExternalReference']['value'] = _wallytoolbox_checkifelementexists($item, 'guid', 'none', 'link', $feed_name, $prefix);
  $tempobject['FileURI']['value'] = $enclosure->item(0)->getAttribute('url');
  $tempobject['FileSize']['value'] = $enclosure->item(0)->getAttribute('length');
  $array_item['EmbeddedContent']['EmbeddedObjects']['Object'][] = $tempobject;
}

/**
 * Get available parameters from medias domElement of a opml feed
 * item and set an array item according to the structure defined in
 * Wallymport.
 * 
 * @param array &$array_item
 *   The array item under construction.
 * @param domNode $channel
 *   The full opml channel from a opml feed.
 * @param domElement $item
 *   The XML opml feed item domElement.
 * @param domElement $media_contents
 *   The medias domElements from a opml feed.
 * @param string $feed_name
 *   The name of the feed being analyzed.
 */
function wallytoolbox_opml_item_to_array_set_medias(&$array_item, $channel, $item, $media_contents, $feed_name, $prefix, $media_namespace = 'http://search.yahoo.com/mopml/') {
  $first_media = TRUE;
  foreach ($media_contents as $media_content) {
    $media_type = NULL;
    if (($media_content->getAttribute('type') && strstr($media_content->getAttribute('type'), 'image')) || ($media_content->getAttribute('medium') && strstr($media_content->getAttribute('medium'), 'image'))) {
      if (@getimagesize($media_content->getAttribute('url')))
        $media_type = 'photoobject';
    } elseif (($media_content->getAttribute('type') && strstr($media_content->getAttribute('type'), 'video')) || ($media_content->getAttribute('medium') && strstr($media_content->getAttribute('medium'), 'video'))) {
      if ($media_content->hasAttribute('url')) {
        $third_party_url = $media_content->getAttribute('url');
      } else {
        $media_players = $item->getElementsByTagNameNS($media_namespace, 'player');
        if ($media_player = $media_players->item(0)) {
          $third_party_url = $media_player->hasAttribute('url') ? $media_player->getAttribute('url') : '';
        } else {
          return;
        }
      }
      $providers = emfield_parse_embed(NULL, $third_party_url, 'emvideo');
      if (!empty($providers))
        $media_type = 'videoobject';
    }
    
    if ($media_type) {
      wallytoolbox_opml_item_to_array_set_media($array_item, $channel, $item, $media_content, $feed_name, $first_media, $media_type, $media_namespace, $third_party_url);
      $first_media = FALSE;
    }
  }
}

/**
 * Get available parameters from a media domElement of a opml feed
 * item and set an array item according to the structure defined in
 * Wallymport.
 * 
 * @param array &$array_item
 *   The array item under construction.
 * @param domNode $channel
 *   The full opml channel from a opml feed.
 * @param domElement $item
 *   The XML opml feed item domElement.
 * @param domElement $media_content
 *   One of the media domElement from a opml feed.
 * @param string $feed_name
 *   The name of the feed being analyzed.
 * @param boolean $first_media
 *   Check if it's the first media added to this array item.
 */
function wallytoolbox_opml_item_to_array_set_media(&$array_item, $channel, $item, $media_content, $feed_name, $first_media, $media_type, $media_namespace = 'http://search.yahoo.com/mopml/', $third_party_url = NULL) {
  static $media_num = 0;
  
  if ($first_media) {
    $array_item['EmbeddedContent'] = array();
    $array_item['EmbeddedContent']['EmbeddedObjects'] = array();
    $array_item['EmbeddedContent']['EmbeddedObjects']['Object'] = array();
  }
  
  $tempobject = call_user_func('wallytoolbox_flow_to_array_new_'.$media_type);
  call_user_func_array('wallytoolbox_opml_item_to_array_set_'.$media_type, array(&$tempobject, &$array_item, $channel, $item, $media_content, $feed_name, $media_num, $media_namespace, $third_party_url));
  $media_num++;
  
  $array_item['EmbeddedContent']['EmbeddedObjects']['Object'][] = $tempobject;
}

function wallytoolbox_opml_item_to_array_set_photoobject(&$tempobject, &$array_item, $channel, $item, $media_content, $feed_name, $media_num, $media_namespace = 'http://search.yahoo.com/mopml/', $third_party_url = NULL, $prefix = '') {
  $tempobject['type'] = 'PhotoObjectType';
  $tempobject['ObjectID'] = _wallytoolbox_checkifelementexists($item, 'title');
  $tempobject['PublicationDate'] = $array_item['PublicationDate'];
  $tempobject['MimeType'] = $media_content->getAttribute('type');
  $media_title = _wallytoolbox_checkifmediaelementexists($media_content, $item, $channel, $media_namespace, 'title');
  $tempobject['Title']['value'] = ($media_title == '') ? $tempobject['ObjectID'].'_image_'.$media_num : $media_title;
  $tempobject['ExternalReference']['value'] = _wallytoolbox_checkifelementexists($item, 'guid', 'none', 'link', $feed_name, $prefix);
  $tempobject['Copyright']['value'] = _wallytoolbox_checkifmediaelementexists($media_content, $item, $channel, $media_namespace, 'copyright');
  $tempobject['FileURI']['value'] = $media_content->getAttribute('url');
  $object_filesize = $media_content->getAttribute('fileSize');
  if (!$object_filesize) $object_filesize = 0;
  $tempobject['FileSize']['value'] = $object_filesize;
  $keywords = _wallytoolbox_checkifmediaelementexists($media_content, $item, $channel, $media_namespace, 'keywords');
  if ($keywords != '') {
    $keywords = explode(',',$keywords);
    foreach ($keywords as $keyword) {
      $array_item['MainStory']['Taxonomy']['Vocabulary']['Term'][] = $feed_name.'/'.str_replace('/', '-', $keyword);
    }
  }
  $media_categories = _wallytoolbox_checkifmediaelementexists($media_content, $item, $channel, $media_namespace, 'category');
  if ($media_categories != '') {
    $media_categories = explode('/',$media_categories);
    foreach ($media_categories as $media_category) {
      $array_item['MainStory']['Taxonomy']['Vocabulary']['Term'][] = $feed_name.'/'.str_replace('/', '-', $media_category);
    }
  }
  $media_thumbnails = $media_content->getElementsByTagNameNS($media_namespace, 'thumbnail');
  if ($media_thumbnails->item(0)) {
    $media_thumbnail = $media_thumbnails->item(0);
    $tempobject['Thumbnail']['URI'] = $media_thumbnail->getAttribute('url');
    $thumbnail_w = $media_content->getAttribute('width');
    if ($thumbnail_w) $tempobject['Thumbnail']['W'] = $thumbnail_w;
    $thumbnail_h = $media_content->getAttribute('height');
    if ($thumbnail_h) $tempobject['Thumbnail']['H'] = $thumbnail_h;
  }
  $media_rating = _wallytoolbox_checkifmediaelementexists($media_content, $item, $channel, $media_namespace, 'rating');
  $acceptable_rating = array();
  $all_voc = taxonomy_get_vocabularies();
  foreach ($all_voc as $voc) {
    if ($voc->name == 'Rating') {
      $available_ratings = taxonomy_get_tree($voc->vid);
      foreach ($available_ratings as $available_rating) {
        $acceptable_rating[] = $available_rating->name;
      }
      break;
    }
  }
  if (in_array(strtoupper($media_rating), $acceptable_rating)) {
    $tempobject['Rating']['value'] = strtoupper($media_rating);
  }
}

function wallytoolbox_opml_item_to_array_set_videoobject(&$tempobject, &$array_item, $channel, $item, $media_content, $feed_name, $media_num, $media_namespace = 'http://search.yahoo.com/mopml/', $third_party_url = NULL, $prefix = '') {
  $tempobject['type'] = 'VideoObjectType';
  $tempobject['ObjectID'] = _wallytoolbox_checkifelementexists($item, 'title');
  $tempobject['PublicationDate'] = $array_item['PublicationDate'];
  $tempobject['MimeType'] = $media_content->getAttribute('type');
  $media_title = _wallytoolbox_checkifmediaelementexists($media_content, $item, $channel, $media_namespace, 'title');
  $tempobject['Title']['value'] = ($media_title == '') ? $tempobject['ObjectID'].'_image_'.$media_num : $media_title;
  $tempobject['ExternalReference']['value'] = _wallytoolbox_checkifelementexists($item, 'guid', 'none', 'link', $feed_name, $prefix);
  $tempobject['Copyright']['value'] = _wallytoolbox_checkifmediaelementexists($media_content, $item, $channel, $media_namespace, 'copyright');
  $tempobject['FileURI']['value'] = $third_party_url ? $third_party_url : '';
  $object_filesize = $media_content->getAttribute('fileSize');
  if (!$object_filesize) $object_filesize = 0;
  $tempobject['FileSize']['value'] = $object_filesize;
  $keywords = _wallytoolbox_checkifmediaelementexists($media_content, $item, $channel, $media_namespace, 'keywords');
  if ($keywords != '') {
    $keywords = explode(',',$keywords);
    foreach ($keywords as $keyword) {
      $array_item['MainStory']['Taxonomy']['Vocabulary']['Term'][] = $feed_name.'/'.str_replace('/', '-', $keyword);
    }
  }
  $media_categories = _wallytoolbox_checkifmediaelementexists($media_content, $item, $channel, $media_namespace, 'category');
  if ($media_categories != '') {
    $media_categories = explode('/',$media_categories);
    foreach ($media_categories as $media_category) {
      $array_item['MainStory']['Taxonomy']['Vocabulary']['Term'][] = $feed_name.'/'.str_replace('/', '-', $media_category);
    }
  }
  $media_thumbnails = $media_content->getElementsByTagNameNS($media_namespace, 'thumbnail');
  if ($media_thumbnails->item(0)) {
    $media_thumbnail = $media_thumbnails->item(0);
    $tempobject['Thumbnail']['URI'] = $media_thumbnail->getAttribute('url');
    $thumbnail_w = $media_content->getAttribute('width');
    if ($thumbnail_w) $tempobject['Thumbnail']['W'] = $thumbnail_w;
    $thumbnail_h = $media_content->getAttribute('height');
    if ($thumbnail_h) $tempobject['Thumbnail']['H'] = $thumbnail_h;
  }
  $media_rating = _wallytoolbox_checkifmediaelementexists($media_content, $item, $channel, $media_namespace, 'rating');
  $acceptable_rating = array();
  $all_voc = taxonomy_get_vocabularies();
  foreach ($all_voc as $voc) {
    if ($voc->name == 'Rating') {
      $available_ratings = taxonomy_get_tree($voc->vid);
      foreach ($available_ratings as $available_rating) {
        $acceptable_rating[] = $available_rating->name;
      }
      break;
    }
  }
  if (in_array(strtoupper($media_rating), $acceptable_rating)) {
    $tempobject['Rating']['value'] = strtoupper($media_rating);
  }
}

/**
 * Get available parameters from an image domElement of a opml feed
 * item and set an array item according to the structure defined in
 * Wallymport.
 * 
 * @param array &$array_item
 *   The array item under construction.
 * @param domElement $channel_img
 *   The image domElement from the XML opml feed item.
 */
function wallytoolbox_opml_item_to_array_set_image(&$array_item, $channel_img) {
  static $chan_img_num = 100;
  $array_item['EmbeddedContent'] = array();
  $array_item['EmbeddedContent']['EmbeddedObjects'] = array();
  $array_item['EmbeddedContent']['EmbeddedObjects']['Object'] = array();
  $tempobject = wallytoolbox_flow_to_array_new_photoobject();
  $tempobject['ObjectID'] = _wallytoolbox_checkifelementexists($channel_img, 'title');
  $tempobject['PublicationDate'] = $array_item['PublicationDate'];
  $tempobject['MimeType'] = 'image';
  $media_title = $channel_img->getAttribute('title');
  $tempobject['Title']['value'] = ($media_title == '') ? $tempobject['ObjectID'].'_image_'.$chan_img_num : $media_title;
  $chan_img_num++;
  $tempobject['ExternalReference']['value'] = $channel_img->getAttribute('url');
  $tempobject['FileURI']['value'] = $channel_img->getAttribute('url');
  $tempobject['FileSize']['value'] = 0;
  $tempobject['LinkTitle']['value'] = $channel_img->getAttribute('title');
  $tempobject['LinkURI']['value'] = $channel_img->getAttribute('link');
  $array_item['EmbeddedContent']['EmbeddedObjects']['Object'][] = $tempobject;
}

function _wallytoolbox_opml_set_log(&$item, $success, $msg, $severity = 'notice') {
  drupal_set_message($msg, $severity);
  $item['__log']['success'] = $success;
  $item['__log']['msg'] = $msg;
}
