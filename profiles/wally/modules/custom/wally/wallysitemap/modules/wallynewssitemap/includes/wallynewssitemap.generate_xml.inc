<?php

function _wallynewssitemap_generate_sitemap_xml($site_infos, $settings) {
  
}

/**
 * Generate news sitemap xml for a list of nodes
 *
 * @see http://support.google.com/webmasters/bin/answer.py?hl=en&answer=75717&topic=10078&ctx=topic
 *
 * Output example :
 * <?xml version="1.0" encoding="UTF-8"?>
 * <urlset xmlns="http://www.sitemaps.org/schemas/sitemap/0.9/"
 *     xmlns:news="http://www.google.com/schemas/sitemap-news/0.9/"
 *     xmlns:image="http://www.google.com/schemas/sitemap-image/1.1/"
 *     xmlns:video="http://www.google.com/schemas/sitemap-video/1.1/">
 *   <url>
 *     [...]
 *   </url>
 *   <url>
 *     [...]
 *   </url>
 * </urlset>
 */
function _wallynewssitemap_generate_urlset_xml($nids, $site_infos, $settings) {
  $namespaces = wallysitemap_get_namespaces();
  $dom = new DOMDocument('1.0', 'UTF-8');
  $urlset = $dom->appendChild($dom->createElementNS($namespaces['sitemap'], 'urlset'));
  $urlset->setAttributeNS($namespaces['w3'], 'xmlns:news', $namespaces['news']);
  if ($settings['wallynewssitemap_add_images']) {
    $urlset->setAttributeNS($namespaces['w3'], 'xmlns:image', $namespaces['image']);
  }
  if ($settings['wallynewssitemap_add_videos']) {
    $urlset->setAttributeNS($namespaces['w3'], 'xmlns:video', $namespaces['video']);
  }

  foreach ($nids as $nid) {
    $node = node_load($nid);
    $url = $urlset->appendChild($dom->createElement('url'));
    _wallynewssitemap_generate_newsurl_xml($dom, $url, $namespaces, $node, $site_infos, $settings);
  }

  if ($settings['wallynewssitemap_consolidate_sitemap']) {
    _wallynewssitemap_generate_consolidate_xml($sitemap_dom, $namespaces, $site_infos, $settings);
  }

  return $dom->saveXML();
}

/**
 * Generate news sitemap xml for a node
 *
 * @see http://support.google.com/webmasters/bin/answer.py?hl=en&answer=75717&topic=10078&ctx=topic
 *
 * Output example :
 * <url>
 *   <loc>http://www.example.org/business/article55.html</loc>
 *   <news:news>
 *     [...]
 *   </news:news>
 *   <image:image>
 *     [...]
 *   </image:image>
 *   <video:video>
 *     [...]
 *   </video:video>
 * </url>
 */
function _wallynewssitemap_generate_newsurl_xml($dom, &$url, $namespaces, $node, $site_infos, $settings) {
  $url->appendChild($dom->createElement('loc', $site_infos['base_url'].'/'.$node->path));

  $main_news = $url->appendChild($dom->createElementNS($namespaces['news'], 'news:news'));
  _wallynewssitemap_generate_newsurl_xml_set_main_news($dom, $main_news, $namespaces['news'], $node, $site_infos, $settings);

  if ($settings['wallynewssitemap_add_images']) {
    wallycontenttypes_packagepopulate($node);
    foreach ($node->field_embededobjects_nodes as $embed) {
      if ($embed->type == 'wally_photoobject') {
        $image = $url->appendChild($dom->createElement('image:image'));
        _wallynewssitemap_generate_newsurl_xml_set_image($dom, $image, $namespaces['image'], $embed, $site_infos, $settings);
      }
      
      if ($embed->type == 'wally_linktype') {
        
      }
    }
  }

  if ($settings['wallynewssitemap_add_videos']) {
    if (!isset($node->field_embededobjects_nodes)) {
      wallycontenttypes_packagepopulate($node);
    }
    foreach ($node->field_embededobjects_nodes as $embed) {
      if ($embed->type == 'wally_videoobject') {
        $video = $url->appendChild($dom->createElement('video:video'));
        _wallynewssitemap_generate_newsurl_xml_set_video($dom, $video, $namespaces['video'], $embed, $site_infos, $settings);
      }
    }
  }
}

function _wallynewssitemap_generate_consolidate_xml(&$sitemap_dom, $namespaces, $site_infos, $settings) {

}

/**
 * @see http://support.google.com/webmasters/bin/answer.py?hl=en&answer=74288&topic=10078&ctx=topic
 *
 * Output example :
 * <news:news>
 *   <news:publication> // required
 *     <news:name>The Example Times</news:name>
 *     <news:language>en</news:language>
 *   </news:publication>
 *   <news:access>Subscription</news:access> // required if not open
 *   <news:genres>PressRelease, Blog</news:genres> // required if applies
 *   <news:publication_date>2008-12-23</news:publication_date> // required
 *   <news:title>Companies A, B in Merger Talks</news:title>
 *   <news:geo_location>Etterbeek, Belgium</news:geo_location>
 *   <news:keywords>business, merger, acquisition, A, B</news:keywords>
 *   <news:stock_tickers>NASDAQ:A, NASDAQ:B</news:stock_tickers>
 * </news:news>
 */
function _wallynewssitemap_generate_newsurl_xml_set_main_news($dom, &$main_news, $newsNS, $node, $site_infos, $settings) {
  $news_pub = $main_news->appendChild($dom->createElementNS($newsNS, 'news:publication'));
  $news_pub->appendChild($dom->createElementNS($newsNS, 'news:name', $settings['wallynewssitemap_publication_name']));
  $news_pub->appendChild($dom->createElementNS($newsNS, 'news:language', $site_infos['language']));
  if (isset($node->field_freeaccess[0]['value']) && $node->field_freeaccess[0]['value'] == 'No') {
    $main_news->appendChild($dom->createElementNS($newsNS, 'news:access', 'Subscription'));
  }
  if (isset($node->field_sitemap_genre[0]['value']) && is_numeric($node->field_sitemap_genre[0]['value'])) {
    $sitemap_genres = array();
    foreach ($node->field_sitemap_genre as $node_sitemap_genre) {
      $term_sitemap_genre = taxonomy_get_term($node_sitemap_genre['value']);
      $sitemap_genres[] = $term_sitemap_genre->name;
    }
    $main_news->appendChild($dom->createElementNS($newsNS, 'news:genres', implode(', ', $sitemap_genres)));
  }
  $form_date = date_make_date($node->field_publicationdate[0]['value'], 'UTC');
  $form_date = (object)date_timezone_set($form_date, timezone_open($node->field_publicationdate[0]['timezone']));
  $form_date = unserialize(serialize($form_date));
  $main_news->appendChild($dom->createElementNS($newsNS, 'news:publication', date('Y-m-d\TH:i:sP', strtotime($form_date->date))));
  $main_news->appendChild($dom->createElementNS($newsNS, 'news:title', $node->title));
  $sitemap_geoloc = array();
  if (isset($node->field_markers[0]['city']) && !empty($node->field_markers[0]['city'])) {
    $sitemap_geoloc[] = $node->field_markers[0]['city'];
  }
  if (isset($node->field_markers[0]['province_name']) && !empty($node->field_markers[0]['province_name'])) {
    $sitemap_geoloc[] = $node->field_markers[0]['province_name'];
  }
  if (isset($node->field_markers[0]['country_name']) && !empty($node->field_markers[0]['country_name'])) {
    $sitemap_geoloc[] = $node->field_markers[0]['country_name'];
  }
  if (!empty($sitemap_geoloc)) {
    $main_news->appendChild($dom->createElementNS($newsNS, 'news:geo_location', implode(', ', $sitemap_geoloc)));
  }
  $sitemap_tags = array();
  foreach ($settings['wallynewssitemap_tags_fields'] as $tag_field) {
    if (isset($node->{$tag_field}[0]['value']) && is_numeric($node->{$tag_field}[0]['value'])) {
      foreach ($node->{$tag_field} as $node_tag) {
        $term_tag = taxonomy_get_term($node_tag['value']);
        $sitemap_tags[] = $term_tag->name;
      }
    }
  }
  if (!empty($sitemap_tags)) {
    $main_news->appendChild($dom->createElementNS($newsNS, 'news:keywords', implode(', ', $sitemap_tags)));
  }
}

/**
 * @see http://support.google.com/webmasters/bin/answer.py?hl=en&answer=178636&topic=20986&ctx=topic
 *
 * Output example :
 * <image:image>
 *   <image:loc>http://example.com/photo.jpg</image:loc> // required
 *   <image:caption>Caption of my image</image:caption>
 *   <image:geo_location>Etterbeek, Belgium</image:geo_location>
 *   <image:title>Companies A, B in Merger Talks</image:title>
 *   <image:license>http://www.mylicense.com/hello</image:license>
 * </image:image>
 */
function _wallynewssitemap_generate_newsurl_xml_set_image($dom, &$image, $imageNS, $embed, $site_infos, $settings) {
  $image->appendChild($dom->createElementNS($imageNS, 'image:loc', imagecache_create_url($settings['wallynewssitemap_image_preset'], $embed->field_photofile[0]['filepath'])));
  if (isset($embed->field_summary[0]['value']) && !empty($embed->field_summary[0]['value'])) {
    $image->appendChild($dom->createElementNS($imageNS, 'image:caption', $embed->field_summary[0]['value']));
  } elseif (isset($embed->field_photofile[0]['data']['description']) && !empty($embed->field_photofile[0]['data']['description'])) {
    $image->appendChild($dom->createElementNS($imageNS, 'image:caption', $embed->field_photofile[0]['data']['description']));
  }
  $sitemap_geoloc = array();
  if (isset($embed->field_markers[0]['city']) && !empty($embed->field_markers[0]['city'])) {
    $sitemap_geoloc[] = $embed->field_markers[0]['city'];
  }
  if (isset($embed->field_markers[0]['province_name']) && !empty($embed->field_markers[0]['province_name'])) {
    $sitemap_geoloc[] = $embed->field_markers[0]['province_name'];
  }
  if (isset($embed->field_markers[0]['country_name']) && !empty($embed->field_markers[0]['country_name'])) {
    $sitemap_geoloc[] = $embed->field_markers[0]['country_name'];
  }
  if (!empty($sitemap_geoloc)) {
    $image->appendChild($dom->createElementNS($imageNS, 'image:geo_location', implode(', ', $sitemap_geoloc)));
  }
  $image->appendChild($dom->createElementNS($imageNS, 'image:title', $embed->title));
  if (!empty($settings['wallynewssitemap_image_license'])) {
    $image->appendChild($dom->createElementNS($imageNS, 'image:license', $settings['wallynewssitemap_image_license']));
  }
}

/**
 * @see http://support.google.com/webmasters/bin/answer.py?hl=en&answer=80472&topic=10079&ctx=topic
 *
 * Output example :
 * <video:video>
 *   <video:thumbnail_loc>http://www.example.com/thumbs/123.jpg</video:thumbnail_loc> // required
 *   <video:title>Grilling steaks for summer</video:title> // required
 *   <video:description>Alkis shows you how to get perfectly done steaks every time</video:description> // required
 *   <video:content_loc>http://www.example.com/video123.flv</video:content_loc> // required (or player_loc)
 *   <video:player_loc allow_embed="yes" autoplay="ap=1">http://www.example.com/videoplayer.swf?video=123</video:player_loc> // required (or content_loc)
 *   <video:duration>600</video:duration>
 *   <video:expiration_date>2009-11-05T19:20:30+08:00</video:expiration_date>
 *   <video:rating>4.2</video:rating>
 *   <video:view_count>12345</video:view_count>
 *   <video:publication_date>2007-11-05T19:20:30+08:00</video:publication_date>
 *   <video:family_friendly>yes</video:family_friendly>
 *   <video:tag>business, merger, acquisition, A, B</video:tag>
 *   <video:category>Cooking</video:category>
 *   <video:restriction relationship="allow">IE GB US CA</video:restriction>
 *   <video:gallery_loc title="Cooking Videos">http://cooking.example.com</video:gallery_loc>
 *   <video:price currency="EUR">1.99</video:price>
 *   <video:requires_subscription>yes</video:requires_subscription>
 *   <video:uploader info="http://www.example.com/users/grillymcgrillerson">GrillyMcGrillerson</video:uploader>
 *   <video:platform>web mobile tv</video:platform>
 *   <video:live>no</video:live>
 * </video:video>
 */
function _wallynewssitemap_generate_newsurl_xml_set_video($dom, &$video, $videoNS, $embed, $site_infos, $settings) {
  if (isset($embed->field_thumbnail[0]['fid']) && is_numeric($embed->field_thumbnail[0]['fid']) && $embed->field_thumbnail[0]['fid'] > 0) {
    $video_thumb = imagecache_create_url($settings['wallynewssitemap_video_preset'], $embed->field_thumbnail[0]['filepath']);
  } else {
    $video_thumb = imagecache_create_url($settings['wallynewssitemap_video_preset'], $settings['wallynewssitemap_default_img']);
  }
  $video->appendChild($dom->createElementNS($videoNS, 'video:thumbnail_loc', $video_thumb));
  $video->appendChild($dom->createElementNS($videoNS, 'video:title', $embed->title));
  if (isset($embed->field_summary[0]['value']) && !empty($embed->field_summary[0]['value'])) {
    $video_desc = $embed->field_summary[0]['value'];
  } elseif (isset($embed->field_objectdescription[0]['value']) && !empty($embed->field_objectdescription[0]['value'])) {
    $video_desc = $embed->field_objectdescription[0]['value'];
  } elseif (isset($embed->field_video3rdparty[0]['description']) && !empty($embed->field_video3rdparty[0]['description'])) {
    $video_desc = $embed->field_video3rdparty[0]['description'];
  } elseif (isset($embed->field_videofile[0]['data']['description']) && !empty($embed->field_videofile[0]['data']['description'])) {
    $video_desc = $embed->field_videofile[0]['data']['description'];
  } else {
    $video_desc = $embed->title;
  }
  $video->appendChild($dom->createElementNS($videoNS, 'video:description', $video_desc));
  if (isset($embed->field_video3rdparty[0]['embed'])) {
    $player_loc = $video->appendChild($dom->createElementNS($videoNS, 'video:player_loc', $embed->field_video3rdparty[0]['embed']));
    $player_loc->setAttribute('allow_embed', $settings['wallynewssitemap_allow_embed']);
    // @todo : gérer l'autoplay
    $video->appendChild($dom->createElementNS($videoNS, 'video:duration', $embed->field_video3rdparty[0]['duration']));
  } else {
    $video->appendChild($dom->createElementNS($videoNS, 'video:content_loc', $site_infos['base_url'].'/'.$embed->field_videofile[0]['filepath']));
    // video:duration not handled
  }
  // video:expiration_date not handled
  // video:rating not handled
  // video:view_count not handled
  $video->appendChild($dom->createElementNS($videoNS, 'video:publication', date('Y-m-d\TH:i:sP', $embed->created)));
  if (isset($embed->field_rating[0]['value']) && is_numeric($embed->field_rating[0]['value']) && $embed->field_rating[0]['value'] > 0) {
    $rating_term = taxonomy_get_term($embed->field_rating[0]['value']);
    if ($rating_term->name == 'G') {
      $video->appendChild($dom->createElementNS($videoNS, 'video:family_friendly', 'yes'));
    } else {
      $video->appendChild($dom->createElementNS($videoNS, 'video:family_friendly', 'no'));
    }
  }
  $video_tags = array();
  foreach ($settings['wallynewssitemap_tags_fields'] as $tag_field) {
    if (isset($embed->{$tag_field}[0]['value']) && is_numeric($embed->{$tag_field}[0]['value'])) {
      foreach ($embed->{$tag_field} as $embed_tag) {
        $term_tag = taxonomy_get_term($embed_tag['value']);
        $video_tags[] = $term_tag->name;
      }
    }
  }
  if (!empty($video_tags)) {
    $video->appendChild($dom->createElementNS($videoNS, 'video:tag', implode(', ', $video_tags)));
  }
  // video:category not handled
  // video:restriction not handled
  // video:gallery_loc not handled
  // video:price not handled
  // video:requires_subscription not handled
  // video:uploader not handled
  // video:platform not handled
  // video:live not handled
}
