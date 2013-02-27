<?php

function _wallynewssitemap_generate_sitemap_xml($settings) {
  module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');

  $time = time();
  $timestamp = $time - $time%3600;

  $nids = _wallynewssitemap_get_nids_to_generate($settings, $timestamp);

  $xmlfiles_to_generate = _wallynewssitemap_generate_urlset_xml($nids, $settings, $timestamp);

  wallysitemap_generate_xmlfile($xmlfiles_to_generate, $settings['wallynewssitemap_path']);

  _wallynewssitemap_cleanup_old_xmlfiles($settings, $timestamp - 3600);

  if ($settings['wallynewssitemap_consolidate_sitemap']) {
    _wallynewssitemap_generate_consolidate_xml($settings);
  }
}

function _wallynewssitemap_get_nids_to_generate($settings, $timestamp, $limit = 0) {
  $nids = array();

  $select = "SELECT n.nid FROM {node} n";
  $in_content_types = implode("', '", $settings['wallynewssitemap_content_types']);
  $where = " WHERE n.type IN ('$in_content_types') AND n.changed > %d";
  $order_by = " ORDER BY n.changed DESC";
  $limit = $limit ? " LIMIT 0, $limit" : ""; 
  $query = $select.$where.$order_by.$limit;
  $db_nids = db_query($query, $timestamp);
  while ($db_nid = db_fetch_array($db_nids)) {
    $nids[] = $db_nid['nid'];
  }

  return $nids;
}

function _wallynewssitemap_get_destinations_nids_to_generate($settings, $timestamp, $limit) {
  $nids = array();
  
  $global_sitemap_settings = wally_variable_get('wallysitemap_sitemaps_settings', array());
  $dest_settings = $global_sitemap_settings['wallysitemap_dests_params'] ? $global_sitemap_settings['wallysitemap_dests_params'] : array();

  $select = "SELECT cfd.nid FROM {content_field_destinations} cfd";
  $join = " LEFT JOIN {node} n ON n.nid = cfd.nid";
  $in_content_types = implode("', '", $settings['wallynewssitemap_content_types']);
  $where = " WHERE cfd.field_destinations_tid = %d AND n.changed > %d AND n.type IN ('$in_content_types')";
  $order_by = " ORDER BY n.changed DESC";
  $qlimit = " LIMIT 0, %d";
  $query = $select.$join.$where.$order_by.$qlimit;
  
  $terms = taxonomy_get_tree(variable_get('wallymport_destinationpath', 2));
  foreach ($terms as $term) {
    $tid = $term->tid;
    $term_settings = $dest_settings[$tid] ? $dest_settings[$tid] : array('check' => 1);
    if ($term_settings['check']) {
      $db_nids = db_query($query, $tid, $timestamp, $limit);
      while ($db_nid = db_fetch_array($db_nids)) {
        $nids[] = $db_nid['nid'];
      }
    }
  }

  return $nids;
}

function _wallynewssitemap_cleanup_old_xmlfiles($settings, $timestamp) {
  $files_to_delete = array();
  $limit_timestamp = $timestamp - $settings['wallynewssitemap_delta_hours'] * 3600 + 1800;

  $current_files = file_scan_directory($settings['wallynewssitemap_path'], '.*\.xml');
  foreach ($current_files as $current_file) {
    list($sitemap_prefix, $uniq_timestamp) = explode('_', $current_file->name);
    list($file_timestamp, $a) = explode('-', $uniq_timestamp);
    if ($sitemap_prefix == 'sitemapnews' && is_numeric($file_timestamp)) {
      if ($file_timestamp < $limit_timestamp) {
        $files_to_delete[] = $current_file->filename;
      }
    }
  }

  wallysitemap_remove_files($files_to_delete, $settings['wallynewssitemap_path']);
}

function _wallynewssitemap_generate_consolidate_xml($settings) {
  switch ($settings['wallynewssitemap_consolidate_sitemap_type']) {
    case 'global':
      $nids = _wallynewssitemap_get_nids_to_generate($settings, time() - $settings['wallynewssitemap_delta_hours'] * 3600, $settings['wallynewssitemap_consolidate_sitemap_amount']);
      break;
    case 'destination':
      $nids = _wallynewssitemap_get_destinations_nids_to_generate($settings, time() - $settings['wallynewssitemap_delta_hours'] * 3600, $settings['wallynewssitemap_consolidate_sitemap_amount']);
      break;
  }

  $main_settings = wally_variable_get('wallysitemap_sitemaps_settings', array());
  $dom = new DOMDocument('1.0', 'UTF-8');
  $dom->load($main_settings['wallysitemap_path'].'/sitemapmain.xml');
  $dom_urlset = $dom->getElementsByTagName('urlset')->item(0);
  $dom_urls = $dom->getElementsByTagName('url');
  foreach ($dom_urls as $dom_url) {
    $dom_lastmod = $dom_url->getElementsByTagName('lastmod');
    if ($dom_lastmod->item(0)) {
      $dom_urlset->removeChild($dom_url);
    }
  }
  
  $site_infos = wallysitemap_get_site_infos();
  foreach ($nids as $nid) {
    $node = node_load($nid);
    if (in_array($node->path, $settings['wallynewssitemap_blacklist'])) {
      continue;
    }
    $url = $dom_urlset->appendChild($dom->createElement('url'));
    _wallynewssitemap_generate_newsurl_xml_set_commons($dom, $url, $node, $site_infos, $settings);
  }
  
  $xmlfiles_to_generate = array(array(
    'dom' => $dom,
    'filename' => 'sitemapmain.xml',
  ));
  
  wallysitemap_generate_xmlfile($xmlfiles_to_generate, $main_settings['wallysitemap_path']);
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
function _wallynewssitemap_generate_urlset_xml($nids, $settings, $timestamp) {
  $xmlfiles_to_generate = array();
  $namespaces = wallysitemap_get_namespaces();
  $site_infos = wallysitemap_get_site_infos();
  $dom = _wallynewssitemap_new_dom($settings);
  $urlset = $dom->getElementsByTagName('urlset')->item(0);

  $i = $n = 0;
  foreach ($nids as $nid) {
    $node = node_load($nid);
    if (in_array($node->path, $settings['wallynewssitemap_blacklist'])) {
      continue;
    }

    if ($i == 999) {
      $i = 0;
      $xmlfiles_to_generate[] = array(
        'dom' => $dom,
        'filename' => 'sitemapnews_'.$timestamp.'-'.$n++.'.xml',
      );
      $dom = _wallynewssitemap_new_dom();
      $urlset = $dom->getElementsByTagName('urlset')->item(0);
    }
    $i++;

    $url = $urlset->appendChild($dom->createElement('url'));
    _wallynewssitemap_generate_newsurl_xml($dom, $url, $namespaces, $node, $site_infos, $settings);
  }

  $xmlfiles_to_generate[] = array(
    'dom' => $dom,
    'filename' => 'sitemapnews_'.$timestamp.'-'.$n++.'.xml',
  );

  return $xmlfiles_to_generate;
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
  _wallynewssitemap_generate_newsurl_xml_set_commons($dom, $url, $node, $site_infos, $settings);

  $main_news = $url->appendChild($dom->createElementNS($namespaces['news'], 'news:news'));
  _wallynewssitemap_generate_newsurl_xml_set_main_news($dom, $main_news, $namespaces['news'], $node, $site_infos, $settings);

  if ($settings['wallynewssitemap_add_images']) {
    wallycontenttypes_packagepopulate($node);
    if (isset($node->field_embededobjects_nodes) && !empty($node->field_embededobjects_nodes)) {
      _wallynewssitemap_generate_newsurl_xml_set_images($dom, $url, $namespaces['image'], $node, $site_infos, $settings);
    }
  }

  if ($settings['wallynewssitemap_add_videos']) {
    if (!isset($node->field_embededobjects_nodes)) {
      wallycontenttypes_packagepopulate($node);
    }
    if (isset($node->field_embededobjects_nodes) && !empty($node->field_embededobjects_nodes)) {
      _wallynewssitemap_generate_newsurl_xml_set_videos($dom, $url, $namespaces['video'], $node, $site_infos, $settings);
    }
  }
}

function _wallynewssitemap_generate_newsurl_xml_set_commons($dom, &$url, $node, $site_infos, $settings) {
  $global_sitemap_settings = wally_variable_get('wallysitemap_sitemaps_settings', array());
  $ref_base_url = isset($global_sitemap_settings['wallysitemap_base_url']) ? $global_sitemap_settings['wallysitemap_base_url'] : $site_infos['base_url'];
  
  $url->appendChild($dom->createElement('loc', $ref_base_url.'/'.$node->path));
  if (isset($node->field_destinations) && !empty($node->field_destinations)) {
    $tid = $node->field_destinations[0]['tid'];
    $dest_settings = $global_sitemap_settings['wallysitemap_dests_params'] ? $global_sitemap_settings['wallysitemap_dests_params'] : array();
    $term_settings = $dest_settings[$tid] ? $dest_settings[$tid] :
      array(
        'check' => 1,
        'weight' => 0,
        'freq' => 'daily',
      );
    if (isset($settings['wallynewssitemap_nodes_freqs']) && isset($settings['wallynewssitemap_nodes_freqs'][$tid]) && isset($settings['wallynewssitemap_nodes_freqs'][$tid]['freq']) && !empty($settings['wallynewssitemap_nodes_freqs'][$tid]['freq'])) {
      $term_settings['freq'] = $settings['wallynewssitemap_nodes_freqs'][$tid]['freq'];
    }
    if ($term_settings['check']) {
      if (isset($node->field_editorialupdatedate[0]) && !empty($node->field_editorialupdatedate[0]['value'])) {
        $form_date = date_make_date($node->field_editorialupdatedate[0]['value'], 'UTC');
        $form_date = (object)date_timezone_set($form_date, timezone_open($node->field_editorialupdatedate[0]['timezone']));
        $form_date = unserialize(serialize($form_date));
        $lastmod_date = strtotime($form_date->date);
      } else {
        $lastmod_date = $node->changed;
      }
      $url->appendChild($dom->createElement('lastmod', date('Y-m-d\TH:i:sP', $lastmod_date)));
      $url->appendChild($dom->createElement('changefreq', $term_settings['freq']));
      $url->appendChild($dom->createElement('priority', $term_settings['weight']));
    }
  } else {
    $url->appendChild($dom->createElement('lastmod', date('Y-m-d\TH:i:sP', $node->changed)));
    $url->appendChild($dom->createElement('changefreq', $settings['wallynewssitemap_default_freq']));
    // priority not handled yet
  }
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
  if (isset($node->field_publicationdate[0]) && !empty($node->field_publicationdate[0]['value'])) {
    $form_date = date_make_date($node->field_publicationdate[0]['value'], 'UTC');
    $form_date = (object)date_timezone_set($form_date, timezone_open($node->field_publicationdate[0]['timezone']));
    $form_date = unserialize(serialize($form_date));
    $publication_date = strtotime($form_date->date);
  } else {
    $publication_date = $node->created;
  }
  $main_news->appendChild($dom->createElementNS($newsNS, 'news:publication', date('Y-m-d\TH:i:sP', $publication_date)));
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

function _wallynewssitemap_generate_newsurl_xml_set_images($dom, &$url, $imageNS, $node, $site_infos, $settings) {
  foreach ($node->field_embededobjects_nodes as $embed) {
    if ($embed->type == 'wally_photoobject') {
      $image = $url->appendChild($dom->createElement('image:image'));
      _wallynewssitemap_generate_newsurl_xml_set_image($dom, $image, $imageNS, $embed, $site_infos, $settings);
    }

    if ($embed->type == 'wally_linktype') {
      if ($embed_content = _wallynewssitemap_build_embed_link($embed, 'image')) {
        $image = $url->appendChild($dom->createElement('image:image'));
        $embed->embed_data = $embed_content;
        _wallynewssitemap_generate_newsurl_xml_set_linked_image($dom, $image, $imageNS, $embed, $site_infos, $settings);
      }
    }
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
  $global_sitemap_settings = wally_variable_get('wallysitemap_sitemaps_settings', array());
  $ref_base_url = isset($global_sitemap_settings['wallysitemap_base_url']) ? $global_sitemap_settings['wallysitemap_base_url'] : $site_infos['base_url'];
  
  $image->appendChild($dom->createElementNS($imageNS, 'image:loc', $ref_base_url.imagecache_create_url($settings['wallynewssitemap_image_preset'], $embed->field_photofile[0]['filepath'], FALSE, FALSE)));
  if (isset($embed->field_summary[0]['value']) && !empty($embed->field_summary[0]['value'])) {
    $image_caption = $embed->field_summary[0]['value'];
  } elseif (isset($embed->field_photofile[0]['data']['description']) && !empty($embed->field_photofile[0]['data']['description'])) {
    $image_caption = $embed->field_photofile[0]['data']['description'];
  } elseif (isset($embed->field_objectdescription[0]['value']) && !empty($embed->field_objectdescription[0]['value'])) {
    $image_caption = $embed->field_objectdescription[0]['value'];
  } else {
    $image_caption = $embed->title;
  }
  $image->appendChild($dom->createElementNS($imageNS, 'image:caption', $image_caption));
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
function _wallynewssitemap_generate_newsurl_xml_set_linked_image($dom, &$image, $imageNS, $embed, $site_infos, $settings) {
  $image->appendChild($dom->createElementNS($imageNS, 'image:loc', $embed->field_link_item[0]['url']));
  if (isset($embed->field_summary[0]['value']) && !empty($embed->field_summary[0]['value'])) {
    $image_caption = $embed->field_summary[0]['value'];
  } elseif (isset($embed->field_link_item[0]['title']) && !empty($embed->field_link_item[0]['title'])) {
    $image_caption = $embed->field_link_item[0]['title'];
  } elseif (isset($embed->field_objectdescription[0]['value']) && !empty($embed->field_objectdescription[0]['value'])) {
    $image_caption = $embed->field_objectdescription[0]['value'];
  } else {
    $image_caption = $embed->title;
  }
  $image->appendChild($dom->createElementNS($imageNS, 'image:caption', $image_caption));
  // image:geo_location unable to be handled
  $image->appendChild($dom->createElementNS($imageNS, 'image:title', $embed->title));
  if (isset($settings['wallynewssitemap_image_license']) && !empty($settings['wallynewssitemap_image_license'])) {
    $image->appendChild($dom->createElementNS($imageNS, 'image:license', $settings['wallynewssitemap_image_license']));
  }
}

function _wallynewssitemap_generate_newsurl_xml_set_videos($dom, &$url, $videoNS, $node, $site_infos, $settings) {
  foreach ($node->field_embededobjects_nodes as $embed) {
    if ($embed->type == 'wally_videoobject') {
      $video = $url->appendChild($dom->createElement('video:video'));
      _wallynewssitemap_generate_newsurl_xml_set_video($dom, $video, $videoNS, $embed, $site_infos, $settings);
    }

    if ($embed->type == 'wally_linktype') {
      if ($embed_content = _wallynewssitemap_build_embed_link($embed, 'video')) {
        $video = $url->appendChild($dom->createElement('video:video'));
        $embed->embed_data = $embed_content;
        _wallynewssitemap_generate_newsurl_xml_set_linked_video($dom, $video, $videoNS, $embed, $site_infos, $settings);
      }
    }
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
  $global_sitemap_settings = wally_variable_get('wallysitemap_sitemaps_settings', array());
  $ref_base_url = isset($global_sitemap_settings['wallysitemap_base_url']) ? $global_sitemap_settings['wallysitemap_base_url'] : $site_infos['base_url'];
  
  if (isset($embed->field_thumbnail[0]['fid']) && is_numeric($embed->field_thumbnail[0]['fid']) && $embed->field_thumbnail[0]['fid'] > 0) {
    $video_thumb = imagecache_create_url($settings['wallynewssitemap_video_preset'], $embed->field_thumbnail[0]['filepath'], FALSE, FALSE);
  } else {
    $video_thumb = imagecache_create_url($settings['wallynewssitemap_video_preset'], $settings['wallynewssitemap_default_img'], FALSE, FALSE);
  }
  $video_thumb = $ref_base_url.$video_thumb;
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
    $video->appendChild($dom->createElementNS($videoNS, 'video:content_loc', $ref_base_url.'/'.$embed->field_videofile[0]['filepath']));
    // video:duration not handled yet
  }
  // video:expiration_date not handled yet
  // video:rating not handled yet
  // video:view_count not handled yet
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
  // video:category not handled yet
  // video:restriction not handled yet
  // video:gallery_loc not handled yet
  // video:price not handled yet
  // video:requires_subscription not handled yet
  // video:uploader not handled yet
  // video:platform not handled yet
  // video:live not handled yet
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
function _wallynewssitemap_generate_newsurl_xml_set_linked_video($dom, &$video, $videoNS, $embed, $site_infos, $settings) {
  $global_sitemap_settings = wally_variable_get('wallysitemap_sitemaps_settings', array());
  $ref_base_url = isset($global_sitemap_settings['wallysitemap_base_url']) ? $global_sitemap_settings['wallysitemap_base_url'] : $site_infos['base_url'];
  
  $embed_data = $embed->embed_data;
  if (isset($embed_data['data']['thumbnail']['url']) && !empty($embed_data['data']['thumbnail']['url'])) {
    $video_thumb = $embed_data['data']['thumbnail']['url'];
  } else {
    $video_thumb = $ref_base_url.imagecache_create_url($settings['wallynewssitemap_video_preset'], $settings['wallynewssitemap_default_img'], FALSE, FALSE);
  }
  $video->appendChild($dom->createElementNS($videoNS, 'video:thumbnail_loc', $video_thumb));
  $video->appendChild($dom->createElementNS($videoNS, 'video:title', $embed->title));
  if (isset($embed->field_summary[0]['value']) && !empty($embed->field_summary[0]['value'])) {
    $video_desc = $embed->field_summary[0]['value'];
  } elseif (isset($embed->field_link_item[0]['title']) && !empty($embed->field_link_item[0]['title'])) {
    $video_desc = $embed->field_link_item[0]['title'];
  } elseif (isset($embed->field_objectdescription[0]['value']) && !empty($embed->field_objectdescription[0]['value'])) {
    $video_desc = $embed->field_objectdescription[0]['value'];
  } else {
    $video_desc = $embed->title;
  }
  $video->appendChild($dom->createElementNS($videoNS, 'video:description', $video_desc));
  $player_loc = $video->appendChild($dom->createElementNS($videoNS, 'video:player_loc', $embed->field_link_item[0]['url']));
  $player_loc->setAttribute('allow_embed', $settings['wallynewssitemap_allow_embed']);
  // @todo : gérer l'autoplay
  if (isset($embed_data['data']['duration']) && !empty($embed_data['data']['duration'])) {
    $video->appendChild($dom->createElementNS($videoNS, 'video:duration', $embed_data['data']['duration']));
  }
  // video:expiration_date not handled yet
  // video:rating not handled yet
  // video:view_count not handled yet
  $video->appendChild($dom->createElementNS($videoNS, 'video:publication', date('Y-m-d\TH:i:sP', $embed->created)));
  // video:rating not handled yet
  // video:tag not handled yet
  // video:category not handled yet
  // video:restriction not handled yet
  // video:gallery_loc not handled yet
  // video:price not handled yet
  // video:requires_subscription not handled yet
  // video:uploader not handled yet
  // video:platform not handled yet
  // video:live not handled yet
}

function _wallynewssitemap_build_embed_link($embed, $type) {
  if (isset($embed->field_link_item[0]['url']) && !empty($embed->field_link_item[0]['url']) && !strstr($embed->field_link_item[0]['url'], 'extref://')) {
    $item = array('embed' => $embed->field_link_item[0]['url']);
    $modules = array(
      'image' => array('emimage'),
      'video' => array('emvideo'),
    );
    $field = array();
    if (isset($modules[$type])) {
      foreach ($modules[$type] as $module) {
        $item = _emfield_field_submit_id($field, $item, $module);
        if (!empty($item['provider'])) {
          return $item;
        }
      }
    }
  }
  return FALSE;
}

function _wallynewssitemap_new_dom($settings) {
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

  return $dom;
}

/**
 * Snipet de code pour insérer un DOM node dans un autre DOM document
 *
 * $a = '<loc>http://wally.drupal.dev/category/destination-path/home</loc><changefreq>hourly</changefreq><priority>1</priority>';
 $b = '<?xml version="1.0" encoding="UTF-8"?><url>'.$a.'</url>';
 $dom_a = new DomDocument();
 $dom_a->formatOutput = true;
 $dom_a->loadXML($b);
 dsm($dom_a->saveXML($dom_a));
 $dom = new DomDocument();
 $dom->formatOutput = true;
 $c = $dom_a->getElementsByTagName('url')->item(0);
 $url = $dom->appendChild($dom->importNode($c, true));
 dsm($dom->saveXML($dom));
 *
 */
