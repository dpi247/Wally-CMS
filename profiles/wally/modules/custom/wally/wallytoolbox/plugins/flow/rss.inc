<?php

/**
 * Build an array from a RSS item. The array structure is based on the
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
 *   The full RSS channel, needed because of the media fields which can
 *   be written anywhere in the feed.
 * @param $item
 *   The RSS to be transformed.
 * @param $feed_infos
 *   The infos of the feed as recorded in the DB.
 * 
 * @return
 *   The array-restructured item.
 */
function wallytoolbox_rss_item_to_array($channel, $item, $media_namespace = 'http://search.yahoo.com/mrss/', $feed_infos = array('name'=>'RSS', 'destination'=>'0///', 'channel_img'=>0, 'prefix'=>'')) {
  $array_item = wallytoolbox_flow_to_array_new_package();
  
  list($feed_name, $prefix) = wallytoolbox_rss_item_to_array_set_package($array_item, $item, $feed_infos);
  wallytoolbox_rss_item_to_array_set_mainstory($array_item, $item, $feed_name, $prefix);

  $enclosure = $item->getElementsByTagName('enclosure');
  if ($enclosure->item(0) && $enclosure->item(0)->getAttribute('type') && strstr($enclosure->item(0)->getAttribute('type'), 'image')) {
    wallytoolbox_rss_item_to_array_set_enclosure($array_item, $enclosure, $feed_name, $prefix);
  }

  $media_contents = $item->getElementsByTagNameNS($media_namespace, 'content');
  if ($media_contents->item(0)) {
    wallytoolbox_rss_item_to_array_set_medias($array_item, $channel, $item, $media_contents, $feed_name, $prefix, $media_namespace);
  }

  $channel_img = $channel->getElementsByTagName('image');
  if ($feed_infos['channel_img'] && !isset($array_item['Package']['EmbeddedContent']) && $channel_img->item(0)) {
    wallytoolbox_rss_item_to_array_set_image($array_item, $channel_img->item(0));
  }

  return $array_item;
}

/**
 * Search an item for a desired element.
 * 
 * @param $dom_item
 *   The item in which the search is performed.
 * @param $name
 *   The name of the searched element.
 * @param $namespace
 *   Value of the namespace if used.
 * @param $second_choice
 *   If the element given by $name isn\'t found, you have a second chance.
 * @param $feed_name
 *   The name of the feed as recorded in the DB. Needed as default
 *   external reference value.
 * 
 * @return
 *   The element value or ''.
 */
function _wallytoolbox_checkifelementexists($dom_item, $name, $namespace = 'none', $second_choice = '', $feed_name = '', $prefix = '') {
  $return = '';
  if ($namespace != 'none' && is_object($dom_item)) {
    $return = $dom_item->getElementsByTagNameNS($namespace, $name)->item(0) ? $dom_item->getElementsByTagNameNS($namespace, $name)->item(0)->firstChild->wholeText : '';
    if ($second_choice != '') {
      if ($return == '') {
        $return = $dom_item->getElementsByTagNameNS($namespace, $second_choice)->item(0) ? $dom_item->getElementsByTagNameNS($namespace, $second_choice)->item(0)->firstChild->wholeText : '';
      } else {
        $prefix = ($prefix != '') ? $prefix : $feed_name;
        $return = $prefix.':::'.$return;
      }
    }
  } elseif (is_object($dom_item)) {
    $return = $dom_item->getElementsByTagName($name)->item(0) ? $dom_item->getElementsByTagName($name)->item(0)->firstChild->wholeText : '';
    if ($second_choice != '') {
      if ($return == '') {
        $return = $dom_item->getElementsByTagName($second_choice)->item(0) ? $dom_item->getElementsByTagName($second_choice)->item(0)->firstChild->wholeText : '';
      } else {
        $prefix = ($prefix != '') ? $prefix : $feed_name;
        $return = $prefix.':::'.$return;
      }
    }
  }
  return $return;
}

/**
 * Perform a recursive search in a media item for a desired element.
 * 
 * @param $media_content
 *   The item in which the first step search is performed.
 * @param $item
 *   The item in which the second step search is performed.
 * @param $channel
 *   The channel in which the third and last step search is performed.
 * @param $namespace
 *   Value of the namespace if used.
 * @param $name
 *   The name of the searched element.
 * 
 * @return
 *   The element value or ''.
 */
function _wallytoolbox_checkifmediaelementexists($media_content, $item, $channel, $namespace, $name) {
  $return = _wallytoolbox_checkifelementexists($media_content, $name, $namespace);
  if ($return == '') {
    $return = _wallytoolbox_checkifelementexists($item, $name, $namespace);
    if ($return == '') {
      $return = _wallytoolbox_checkifelementexists($channel, $name, $namespace);
    }
  }
  return $return;
}

/**
 * Get package available parameters from an RSS feed item domElement
 * and set an array item according to the structure defined in
 * Wallymport.
 * 
 * @param array &$array_item
 *   The array item under construction.
 * @param domElement $item
 *   The XML RSS feed item domElement.
 * @param array $feed_infos
 *   General informations about the feed being analyzed.
 */
function wallytoolbox_rss_item_to_array_set_package(&$array_item, $item, $feed_infos) {
  $feed_name = $feed_infos['name'];
  $prefix = ($feed_infos['prefix'] != '') ? $feed_infos['prefix'] : $feed_name;
  $feed_destination = explode('/',$feed_infos['destination']);
  $array_item['Package']['PackageID'] = _wallytoolbox_checkifelementexists($item, 'guid', 'none', 'link', $feed_name);
  $channel_pubDate = strtotime(_wallytoolbox_checkifelementexists($item, 'pubDate'));
  $pubDate = date('Y-m-d', $channel_pubDate).'T'.date('H:i:sP', $channel_pubDate);
  $array_item['Package']['PublicationDate'] = $pubDate;
  $array_item['Package']['PackageTitle']['value'] = _wallytoolbox_checkifelementexists($item, 'title');
  $array_item['Package']['ExternalReference']['value'] = _wallytoolbox_checkifelementexists($item, 'guid', 'none', 'link', $feed_name, $prefix);
  $array_item['Package']['ProductID']['value'] = variable_get('wallymport_productID', $_SERVER['HTTP_HOST']);
  $term = taxonomy_get_term($feed_destination[0]);
  $array_item['Package']['Destinations']['Destination']['DestinationRank'] = $feed_destination[3];
  $array_item['Package']['Destinations']['Destination']['Position'] = $feed_destination[1];
  $array_item['Package']['Destinations']['Destination']['DocumentLayout'] = $feed_destination[2];
  $array_item['Package']['Destinations']['Destination']['DestinationPath']['value'] = is_object($term) ? wallytoolbox_taxonomy_get_path_by_tid_or_term($term->tid) : '';
  $array_item['Package']['ExternalURI']['value'] = _wallytoolbox_checkifelementexists($item, 'link');

  return array($feed_name, $prefix);
}

/**
 * Get mainstory available parameters from an RSS feed item domElement
 * and set an array item according to the structure defined in
 * Wallymport.
 * 
 * @param array &$array_item
 *   The array item under construction.
 * @param domElement $item
 *   The XML RSS feed item domElement.
 * @param string $feed_name
 *   The name of the feed being analyzed.
 */
function wallytoolbox_rss_item_to_array_set_mainstory(&$array_item, $item, $feed_name, $prefix) {
  $array_item['Package']['MainStory']['ObjectID'] = _wallytoolbox_checkifelementexists($item, 'title');
  $array_item['Package']['MainStory']['PublicationDate'] = $array_item['Package']['PublicationDate'];
  $array_item['Package']['MainStory']['Title']['value'] = _wallytoolbox_checkifelementexists($item, 'title');
  $array_item['Package']['MainStory']['ExternalReference']['value'] =  _wallytoolbox_checkifelementexists($item, 'guid', 'none', 'link', $feed_name, $prefix);
  $array_item['Package']['MainStory']['Taxonomy']['Vocabulary']['VocabularyName'] = 'RSS feeds';
  $categories = $item->getElementsByTagName('category');
  if ($categories->item(0)) {
    foreach ($categories as $category) {
      $array_item['Package']['MainStory']['Taxonomy']['Vocabulary']['Term'][] = $feed_name.'/'.str_replace('/', '-', $category->firstChild->wholeText);
    }
  }
  $array_item['Package']['MainStory']['Copyright']['value'] = _wallytoolbox_checkifelementexists($item, 'author');
  $array_item['Package']['MainStory']['TextTitle']['value'] = _wallytoolbox_checkifelementexists($item, 'title');
  $array_item['Package']['MainStory']['TextBody']['value'] = _wallytoolbox_checkifelementexists($item, 'description');
}

/**
 * Get available parameters from an enclosure domElement of a RSS feed
 * item and set an array item according to the structure defined in
 * Wallymport.
 * 
 * @param array &$array_item
 *   The array item under construction.
 * @param domElement $enclosure
 *   The enclosure domElement from the XML RSS feed item.
 * @param string $feed_name
 *   The name of the feed being analyzed.
 */
function wallytoolbox_rss_item_to_array_set_enclosure(&$array_item, $enclosure, $feed_name, $prefix) {
  $array_item['Package']['EmbeddedContent'] = array();
  $array_item['Package']['EmbeddedContent']['EmbeddedObjects'] = array();
  $array_item['Package']['EmbeddedContent']['EmbeddedObjects']['Object'] = array();
  $tempobject = wallytoolbox_flow_to_array_new_photoobject();
  $tempobject['ObjectID'] = _wallytoolbox_checkifelementexists($item, 'title');
  $tempobject['PublicationDate'] = $array_item['Package']['PublicationDate'];
  $tempobject['MimeType'] = $enclosure->item(0)->getAttribute('type');
  $tempobject['Title']['value'] = $enclosure->item(0)->getAttribute('url');
  $tempobject['ExternalReference']['value'] = _wallytoolbox_checkifelementexists($item, 'guid', 'none', 'link', $feed_name, $prefix);
  $tempobject['FileURI']['value'] = $enclosure->item(0)->getAttribute('url');
  $tempobject['FileSize']['value'] = $enclosure->item(0)->getAttribute('length');
  $array_item['Package']['EmbeddedContent']['EmbeddedObjects']['Object'][] = $tempobject;
}

/**
 * Get available parameters from medias domElement of a RSS feed
 * item and set an array item according to the structure defined in
 * Wallymport.
 * 
 * @param array &$array_item
 *   The array item under construction.
 * @param domNode $channel
 *   The full RSS channel from a RSS feed.
 * @param domElement $item
 *   The XML RSS feed item domElement.
 * @param domElement $media_contents
 *   The medias domElements from a RSS feed.
 * @param string $feed_name
 *   The name of the feed being analyzed.
 */
function wallytoolbox_rss_item_to_array_set_medias(&$array_item, $channel, $item, $media_contents, $feed_name, $prefix, $media_namespace = 'http://search.yahoo.com/mrss/') {
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
      wallytoolbox_rss_item_to_array_set_media($array_item, $channel, $item, $media_content, $feed_name, $first_media, $media_type, $media_namespace, $third_party_url);
      $first_media = FALSE;
    }
  }
}

/**
 * Get available parameters from a media domElement of a RSS feed
 * item and set an array item according to the structure defined in
 * Wallymport.
 * 
 * @param array &$array_item
 *   The array item under construction.
 * @param domNode $channel
 *   The full RSS channel from a RSS feed.
 * @param domElement $item
 *   The XML RSS feed item domElement.
 * @param domElement $media_content
 *   One of the media domElement from a RSS feed.
 * @param string $feed_name
 *   The name of the feed being analyzed.
 * @param boolean $first_media
 *   Check if it's the first media added to this array item.
 */
function wallytoolbox_rss_item_to_array_set_media(&$array_item, $channel, $item, $media_content, $feed_name, $first_media, $media_type, $media_namespace = 'http://search.yahoo.com/mrss/', $third_party_url = NULL) {
  static $media_num = 0;
  
  if ($first_media) {
    $array_item['Package']['EmbeddedContent'] = array();
    $array_item['Package']['EmbeddedContent']['EmbeddedObjects'] = array();
    $array_item['Package']['EmbeddedContent']['EmbeddedObjects']['Object'] = array();
  }
  
  $tempobject = call_user_func('wallytoolbox_flow_to_array_new_'.$media_type);
  call_user_func_array('wallytoolbox_rss_item_to_array_set_'.$media_type, array(&$tempobject, &$array_item, $channel, $item, $media_content, $feed_name, $media_num, $media_namespace, $third_party_url));
  $media_num++;
  
  $array_item['Package']['EmbeddedContent']['EmbeddedObjects']['Object'][] = $tempobject;
}

function wallytoolbox_rss_item_to_array_set_photoobject(&$tempobject, &$array_item, $channel, $item, $media_content, $feed_name, $media_num, $media_namespace = 'http://search.yahoo.com/mrss/', $third_party_url = NULL, $prefix = '') {
  $tempobject['type'] = 'PhotoObjectType';
  $tempobject['ObjectID'] = _wallytoolbox_checkifelementexists($item, 'title');
  $tempobject['PublicationDate'] = $array_item['Package']['PublicationDate'];
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
      $array_item['Package']['MainStory']['Taxonomy']['Vocabulary']['Term'][] = $feed_name.'/'.str_replace('/', '-', $keyword);
    }
  }
  $media_categories = _wallytoolbox_checkifmediaelementexists($media_content, $item, $channel, $media_namespace, 'category');
  if ($media_categories != '') {
    $media_categories = explode('/',$media_categories);
    foreach ($media_categories as $media_category) {
      $array_item['Package']['MainStory']['Taxonomy']['Vocabulary']['Term'][] = $feed_name.'/'.str_replace('/', '-', $media_category);
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

function wallytoolbox_rss_item_to_array_set_videoobject(&$tempobject, &$array_item, $channel, $item, $media_content, $feed_name, $media_num, $media_namespace = 'http://search.yahoo.com/mrss/', $third_party_url = NULL, $prefix = '') {
  $tempobject['type'] = 'VideoObjectType';
  $tempobject['ObjectID'] = _wallytoolbox_checkifelementexists($item, 'title');
  $tempobject['PublicationDate'] = $array_item['Package']['PublicationDate'];
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
      $array_item['Package']['MainStory']['Taxonomy']['Vocabulary']['Term'][] = $feed_name.'/'.str_replace('/', '-', $keyword);
    }
  }
  $media_categories = _wallytoolbox_checkifmediaelementexists($media_content, $item, $channel, $media_namespace, 'category');
  if ($media_categories != '') {
    $media_categories = explode('/',$media_categories);
    foreach ($media_categories as $media_category) {
      $array_item['Package']['MainStory']['Taxonomy']['Vocabulary']['Term'][] = $feed_name.'/'.str_replace('/', '-', $media_category);
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
 * Get available parameters from an image domElement of a RSS feed
 * item and set an array item according to the structure defined in
 * Wallymport.
 * 
 * @param array &$array_item
 *   The array item under construction.
 * @param domElement $channel_img
 *   The image domElement from the XML RSS feed item.
 */
function wallytoolbox_rss_item_to_array_set_image(&$array_item, $channel_img) {
  static $chan_img_num = 100;
  $array_item['Package']['EmbeddedContent'] = array();
  $array_item['Package']['EmbeddedContent']['EmbeddedObjects'] = array();
  $array_item['Package']['EmbeddedContent']['EmbeddedObjects']['Object'] = array();
  $tempobject = wallytoolbox_flow_to_array_new_photoobject();
  $tempobject['ObjectID'] = _wallytoolbox_checkifelementexists($channel_img, 'title');
  $tempobject['PublicationDate'] = $array_item['Package']['PublicationDate'];
  $tempobject['MimeType'] = 'image';
  $media_title = $channel_img->getAttribute('title');
  $tempobject['Title']['value'] = ($media_title == '') ? $tempobject['ObjectID'].'_image_'.$chan_img_num : $media_title;
  $chan_img_num++;
  $tempobject['ExternalReference']['value'] = $channel_img->getAttribute('url');
  $tempobject['FileURI']['value'] = $channel_img->getAttribute('url');
  $tempobject['FileSize']['value'] = 0;
  $tempobject['LinkTitle']['value'] = $channel_img->getAttribute('title');
  $tempobject['LinkURI']['value'] = $channel_img->getAttribute('link');
  $array_item['Package']['EmbeddedContent']['EmbeddedObjects']['Object'][] = $tempobject;
}
