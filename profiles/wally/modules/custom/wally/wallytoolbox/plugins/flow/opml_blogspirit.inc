<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('Plugin OPML blog Spirit'),
  'description' => t('Fetch an OPML feed.'),
  'types' => array('OPML'),
  'defaults' => wallytoolbox_opml_blogspirit_flow_settings_default(),
);

function wallytoolbox_opml_blogspirit_flow_settings_form(&$form, &$form_state) {
  $conf = wallytoolbox_opml_blogspirit_flow_settings_default($form_state);
  
  $form['uri'] = array(
    '#type' => 'textfield',
    '#title' => t('URI'),
    '#description' => t('URI of the OPML (could be an url or something else).'),
    '#default_value' => $conf['uri'],
    '#size' => 80,
    '#maxlength' => 512,
    '#required' => TRUE,
  );
  
  $form['maxitem'] = array (
    '#type'             => 'textfield',
    '#title'            => t('Max item to fetch for this feed'),
    '#default_value'    => $conf['maxitem'],
  );

  $form['embed_img'] = array(
    '#title'            => t('Use the first image embeded in the text if no media image found.'),
    '#type'             => 'checkbox',
    '#default_value'    => $conf['embed_img'],
  );
  
  $form['channel_img'] = array(
    '#title'            => t('Use of default image.'),
    '#type'             => 'checkbox',
    '#default_value'    => $conf['channel_img'],
  );

  $form['get_distant_img'] = array (
    '#type'             => 'checkbox',
    '#default_value'    => $conf['get_distant_img'],
    '#title'            => t('Copy RSS feeds images to locale folder.'),
  );  
}

function wallytoolbox_opml_blogspirit_flow_settings_default($form_state = array()) {
  if (isset($form_state) && !empty($form_state)) {
    return $form_state;
  }
  
  $defaults = array(
    'uri' => '',
    'maxitem' => '',
    'channel_img' => 0,
  );
  
  return $defaults;
}

function wallytoolbox_opml_blogspirit_flow_build(&$feed_infos) {
  $struct_array = wallytoolbox_flow_to_array_init_array($feed_infos);
  
  $maxitem = (isset($feed_infos['conf']['maxitem']) && $feed_infos['conf']['maxitem'] > 0) ? $feed_infos['conf']['maxitem'] : 1000;
  $feed_xml = drupal_http_request($feed_infos['conf']['uri']);
  if ($feed_xml && $feed_xml->code == '200') {
    $dom = new DomDocument();
    $dom->LoadXML($feed_xml->data);
    $head = $dom->getElementsByTagName('head');
    $opml_tags = $dom->getElementsByTagName('opml');
    $body = $dom->getElementsByTagName('body');
    $channel_last_build = 0;

    if ($body->item(0)) {
      $feed_destination = explode('/', $feed_infos['destination']);
      $sql_last_build = $feed_infos['last_build'];
      $feed_name = check_plain($feed_infos['name']);
      $channel_last_build = strtotime(_wallytoolbox_checkifelementexists($head->item(0), 'dateModified'));

      if ($channel_last_build > $sql_last_build || empty($channel_last_build) || !isset($channel_last_build) || $channel_last_build == 0) {
        $feed_infos['last_build'] = $channel_last_build;
        
        $feeds_md5 = (isset($feed_infos['feeds_md5']) && is_string($feed_infos['feeds_md5'])) ? unserialize($feed_infos['feeds_md5']) : array();
        $new_feeds_md5 = array();
        $temp_struct_array = array();
        
        foreach ($body->item(0)->getElementsByTagName('outline') as $item) {
          $temp_struct_array = wallytoolbox_opml_blogspirit_item_to_array($head->item(0), $item, $feed_infos);
          
          $elem_package = $temp_struct_array;
          $elem_md5 = md5($elem_package['PublicationDate'].$elem_package['PackageTitle']['value'].$elem_package['ExternalURI']['value'].$elem_package['MainStory']['TextBody']['value']);
          if (!isset($feeds_md5[$elem_md5]))
            $struct_array['Packages']['Package'][] = $temp_struct_array;
          
          $new_feeds_md5[$elem_md5] = TRUE;
          
          if (sizeof($struct_array['Packages']['Package']) >= $maxitem) break;
        }
        
        if (empty($struct_array['Packages']['Package']))
          _wallytoolbox_opml_blogspirit_set_log($struct_array['Packages'], FALSE, t('No modifications for feed').' : '.check_plain($feed_infos['name']));
        
        $feed_infos['feeds_md5'] = $new_feeds_md5;
      } else {
        _wallytoolbox_opml_blogspirit_set_log($struct_array['Packages'], FALSE, t('Channel not rebuild for feed').' : '.check_plain($feed_infos['name']));
      }
    } else {
      _wallytoolbox_opml_blogspirit_set_log($struct_array['Packages'], FALSE, t('No channel in the feed').' : '.check_plain($feed_infos['name']));
    }
  } else {
    _wallytoolbox_opml_blogspirit_set_log($struct_array['Packages'], FALSE, t('No HTTP response for feed').' : '.check_plain($feed_infos['name']));
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
function wallytoolbox_opml_blogspirit_item_to_array($head, $item, $feed_infos = array('name'=>'opml', 'destination'=>'0///', 'channel_img'=>0, 'prefix'=>'')) {
  $array_package = wallytoolbox_flow_to_array_new_package();
  
  // Get package informations
  list($feed_name, $prefix) = wallytoolbox_opml_blogspirit_item_to_array_set_package($array_package, $item, $feed_infos);
  // Get main story
  wallytoolbox_opml_blogspirit_item_to_array_set_mainstory($array_package, $item, $feed_name, $prefix);
  // Get Embeded Contents
  wallytoolbox_opml_blogspirit_item_to_array_set_medias($array_package, $item, $feed_name, $prefix);

  // Get embeded images in the text body
  if ($feed_infos['conf']['embed_img'] && !isset($array_package['EmbeddedContent'])) {
    wallytoolbox_opml_blogspirit_item_to_array_set_embeded_image($array_package, $item);
  }
  
  // Copy distant images to local
  if ($feed_infos['conf']['get_distant_img'] == 1 && isset($array_package['EmbeddedContent'])) {
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
function wallytoolbox_opml_blogspirit_item_to_array_set_package(&$array_item, $item, $feed_infos) {
  $feed_name = $feed_infos['name'];
  $prefix = ($feed_infos['prefix'] != '') ? $feed_infos['prefix'] : $feed_name;
  $feed_destination = explode('/',$feed_infos['destination']);
  $item_id = _wallytoolbox_get_dom_element_attribute_opml($item, 'blog_id').'_'._wallytoolbox_get_dom_element_attribute_opml($item, 'post_id');
  $item_id = ($item_id == '_') ? _wallytoolbox_get_dom_element_attribute_opml($item, 'post_permalink') : $item_id;
  $item_id = $prefix.':::'.$item_id;
  $array_item['PackageID'] = $item_id;
  $item_pubDate = strtotime(_wallytoolbox_get_dom_element_attribute_opml($item, 'post_pubdate'));
  $item_pubDate = ($item_pubDate == '') ? time() : $item_pubDate;
  $pubDate = date('Y-m-d', $item_pubDate).'T'.date('H:i:sP', $item_pubDate);
  $array_item['PublicationDate'] = $pubDate;
  $array_item['CreationDate'] = date('Y-m-d').'T'.date('H:i:sP');
  $array_item['PackageTitle']['value'] = _wallytoolbox_get_dom_element_attribute_opml($item, 'post_title');
  $array_item['ExternalReference']['value'] = $item_id;
  $array_item['ProductID']['value'] = variable_get('wallymport_productID', $_SERVER['HTTP_HOST']);
  $term = taxonomy_get_term($feed_destination[0]);
  $array_item['Destinations']['Destination']['DestinationRank'] = $feed_destination[3];
  $array_item['Destinations']['Destination']['Position'] = $feed_destination[1];
  $array_item['Destinations']['Destination']['DocumentLayout'] = $feed_destination[2];
  $array_item['Destinations']['Destination']['DestinationPath']['value'] = is_object($term) ? wallytoolbox_taxonomy_get_path_by_tid_or_term($term->tid) : '';
  $array_item['ExternalURI']['value'] = _wallytoolbox_get_dom_element_attribute_opml($item, 'post_permalink');

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
function wallytoolbox_opml_blogspirit_item_to_array_set_mainstory(&$array_item, $item, $feed_name, $prefix) {
  $story_id = $prefix.':::'._wallytoolbox_get_dom_element_attribute_opml($item, 'post_id');
  $array_item['MainStory'] = wallytoolbox_flow_to_array_new_textobject();
  $array_item['MainStory']['ObjectID'] = _wallytoolbox_get_dom_element_attribute_opml($item, 'post_id');
  $array_item['MainStory']['PublicationDate'] = $array_item['PublicationDate'];
  $array_item['MainStory']['CreationDate'] = $array_item['CreationDate'];
  $array_item['MainStory']['Title']['value'] = _wallytoolbox_get_dom_element_attribute_opml($item, 'post_title');
  $array_item['MainStory']['ExternalReference']['value'] =  $story_id;
  $categories = _wallytoolbox_get_dom_element_attribute_opml($item, 'post_tags');
  if ($categories != '') {
    $array_item['MainStory']['Taxonomy']['Vocabulary']['VocabularyName'] = 'OPML feeds';
    $categories = explode(',', $categories);
    foreach ($categories as $category) {
      $category = trim($category);
      $array_item['MainStory']['Taxonomy']['Vocabulary']['Term'][] = $feed_name.'/'.str_replace('/', '-', $category);
    }
  } else {
    unset($array_item['MainStory']['Taxonomy']);
  }
  $array_item['MainStory']['Copyright']['value'] = _wallytoolbox_get_dom_element_attribute_opml($item, 'user_username');
  $array_item['MainStory']['TextTitle']['value'] = _wallytoolbox_get_dom_element_attribute_opml($item, 'post_title');
  $array_item['MainStory']['Section']['value'] = _wallytoolbox_get_dom_element_attribute_opml($item, 'blog_title');
  $array_item['MainStory']['TextBody']['value'] = str_replace('&nbsp', ' ', _wallytoolbox_get_dom_element_attribute_opml($item, 'post_source'));
  
}

/**
 * Get available parameters from medias domElement of a opml feed
 * item and set an array item according to the structure defined in
 * Wallymport.
 * 
 * @param array &$array_item
 *   The array item under construction.
 * @param domElement $item
 *   The XML opml feed item domElement.
 * @param string $feed_name
 *   The name of the feed being analyzed.
 */
function wallytoolbox_opml_blogspirit_item_to_array_set_medias(&$array_item, $item, $feed_name, $prefix) {
  $media_type = _wallytoolbox_get_dom_element_attribute_opml($item, 'media_type');
  $media_url = _wallytoolbox_get_dom_element_attribute_opml($item, 'media_url');
  if ($media_type != '') {
    if (strstr($media_type, 'image')) {
      if (@getimagesize($media_url))
        $media_type = 'photoobject';
    } elseif (strstr($media_type, 'video')) {
      $providers = emfield_parse_embed(NULL, $media_url, 'emvideo');
      if (!empty($providers))
        $media_type = 'videoobject';
    } else {
      $media_type = '';
    }
  }

  if ($media_type != '')
    wallytoolbox_opml_blogspirit_item_to_array_set_media($array_item, $item, $feed_name, $media_type, $prefix);
}

/**
 * Get available parameters from a media domElement of a opml feed
 * item and set an array item according to the structure defined in
 * Wallymport.
 * 
 * @param array &$array_item
 *   The array item under construction.
 * @param domElement $item
 *   The XML opml feed item domElement.
 * @param string $feed_name
 *   The name of the feed being analyzed.
 */
function wallytoolbox_opml_blogspirit_item_to_array_set_media(&$array_item, $item, $feed_name, $media_type, $prefix) {
  static $media_num = 0;

  $array_item['EmbeddedContent'] = array();
  $array_item['EmbeddedContent']['EmbeddedObjects'] = array();
  $array_item['EmbeddedContent']['EmbeddedObjects']['Object'] = array();

  $tempobject = call_user_func('wallytoolbox_flow_to_array_new_'.$media_type);
  call_user_func_array('wallytoolbox_opml_blogspirit_item_to_array_set_'.$media_type, array(&$tempobject, &$array_item, $item, $feed_name, $media_type, $prefix));
  $media_num++;
  
  $array_item['EmbeddedContent']['EmbeddedObjects']['Object'][] = $tempobject;
}

function wallytoolbox_opml_blogspirit_item_to_array_set_photoobject(&$tempobject, &$array_item, $item, $feed_name, $media_type, $prefix) {
  $photo_id = $prefix.':::'._wallytoolbox_get_dom_element_attribute_opml($item, 'post_media_id');
  $tempobject['type'] = 'PhotoObjectType';
  $tempobject['ObjectID'] = $photo_id;
  $tempobject['PublicationDate'] = $array_item['PublicationDate'];
  $tempobject['MimeType'] = _wallytoolbox_get_dom_element_attribute_opml($item, 'media_type');
  $tempobject['Title']['value'] = _wallytoolbox_get_dom_element_attribute_opml($item, 'post_title');
  $tempobject['ExternalReference']['value'] = $photo_id;
  $tempobject['Copyright']['value'] = _wallytoolbox_get_dom_element_attribute_opml($item, 'user_username');
  $tempobject['FileURI']['value'] = _wallytoolbox_get_dom_element_attribute_opml($item, 'media_url');
  $tempobject['FileSize']['value'] = 0;
  $tempobject['Thumbnail']['URI'] = _wallytoolbox_get_dom_element_attribute_opml($item, 'media_thumbnail');
}

function wallytoolbox_opml_blogspirit_item_to_array_set_videoobject(&$tempobject, &$array_item, $item, $feed_name, $media_type, $prefix) {
  $video_id = $prefix.':::'._wallytoolbox_get_dom_element_attribute_opml($item, 'post_media_id');
  $tempobject['type'] = 'VideoObjectType';
  $tempobject['ObjectID'] = $video_id;
  $tempobject['PublicationDate'] = $array_item['PublicationDate'];
  $tempobject['MimeType'] = _wallytoolbox_get_dom_element_attribute_opml($item, 'media_type');
  $tempobject['Title']['value'] = _wallytoolbox_get_dom_element_attribute_opml($item, 'post_title');
  $tempobject['ExternalReference']['value'] = $video_id;
  $tempobject['Copyright']['value'] = _wallytoolbox_get_dom_element_attribute_opml($item, 'user_username');
  $tempobject['FileURI']['value'] = _wallytoolbox_get_dom_element_attribute_opml($item, 'media_url');
  $tempobject['FileSize']['value'] = 0;
  $tempobject['Thumbnail']['URI'] = _wallytoolbox_get_dom_element_attribute_opml($item, 'media_thumbnail');
}

/**
 * Get embeded image in the text body of a RSS feed
 * item and set an array item according to the structure defined in
 * Wallymport.
 * 
 * @param array &$array_item
 *   The array item under construction.
 * @param domElement $item
 *   The XML RSS feed item domElement.
 */
function wallytoolbox_opml_blogspirit_item_to_array_set_embeded_image(&$array_item, $item) {
  $text_body = $array_item['MainStory']['TextBody']['value'];
  $substr = strstr($text_body, '<img');
  $substr = $substr ? $substr : strstr($text_body, rawurlencode('<img'));
  $tmp_substr = $substr ? strstr($substr, '>', TRUE) : FALSE;
  $substr = $tmp_substr ? $tmp_substr : ($substr ? strstr($substr, rawurlencode('>'), TRUE) : FALSE);

  if ($substr) {
    $src_substr = strstr($substr, 'src=');
    $src_delimiter = substr($src_substr, 4, 1);
    $src_substr = substr($src_substr, 5);
    $img_url = strstr($src_substr, $src_delimiter, TRUE);
    
    $alt_substr = strstr($substr, 'alt=');
    if ($alt_substr) {
      $alt_delimiter = substr($alt_substr, 4, 1);
      $alt_substr = substr($alt_substr, 5);
      $img_alt = strstr($src_substr, $alt_delimiter, TRUE);
    } else {
      $img_alt = '';
    }
  
    if (@getimagesize($img_url)) {
      static $chan_img_num = 100;
      $array_item['EmbeddedContent'] = array();
      $array_item['EmbeddedContent']['EmbeddedObjects'] = array();
      $array_item['EmbeddedContent']['EmbeddedObjects']['Object'] = array();
      
      $tempobject = wallytoolbox_flow_to_array_new_photoobject();
      $tempobject['ObjectID'] = $img_url;
      $tempobject['PublicationDate'] = $array_item['PublicationDate'];
      $tempobject['MimeType'] = 'image';
      $media_title = $img_alt;
      $tempobject['Title']['value'] = ($media_title == '') ? $tempobject['ObjectID'].'_image_'.$chan_img_num : $media_title;
      $chan_img_num++;
      $tempobject['ExternalReference']['value'] = $img_url;
      $tempobject['FileURI']['value'] = $img_url;
      $tempobject['FileSize']['value'] = 0;
      $tempobject['LinkTitle']['value'] = $img_alt;
      $tempobject['LinkURI']['value'] = $img_url;
      $array_item['EmbeddedContent']['EmbeddedObjects']['Object'][] = $tempobject;
    }
  }
}

/**
 * Search an item for a desired attribute.
 * 
 * @param $dom_item
 *   The item in which the search is performed.
 * @param $name
 *   The name of the searched attribute.
 * @param $default
 *   Default value if the attribute is not found
 * 
 * @return
 *   The attribute value or default value.
 */
function _wallytoolbox_get_dom_element_attribute_opml($dom_item, $name, $default = '') {
  $return = '';
  if (is_object($dom_item)) {
    $return = $dom_item->hasAttribute($name) ? $dom_item->getAttribute($name) : $default;
  }
  return $return;
}

function _wallytoolbox_opml_blogspirit_set_log(&$item, $success, $msg, $severity = 'notice') {
  drupal_set_message($msg, $severity);
  $item['__log']['success'] = $success;
  $item['__log']['msg'] = $msg;
}
