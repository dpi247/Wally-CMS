<?php

/**
 * @see http://support.google.com/webmasters/bin/answer.py?hl=en&answer=183668&topic=8476&ctx=topic
 */
function _wallysitemap_generate_sitemap_xml($site_infos, $settings) {
  module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
  
  $namespaces = wallysitemap_get_namespaces();
  $dom = new DOMDocument('1.0', 'UTF-8');
  $urlset = $dom->appendChild($dom->createElementNS($namespaces['sitemap'], 'urlset'));
  
  $dest_terms = taxonomy_get_tree(variable_get('wallymport_destinationpath', 2));
  _wallysitemap_generate_sitemap_xml_for_taxonomy($dom, $urlset, $site_infos, $settings, $dest_terms);

  $menus = $settings['wallysitemap_handled_menus'];
  foreach ($menus as $menu_name) {
    $menu_infos = menu_tree_all_data($menu_name, NULL);
    _wallysitemap_generate_sitemap_xml_for_menu($dom, $urlset, $site_infos, $settings, $menu_infos);
  }
  
  $xmlfiles_to_generate = array(array(
    'dom' => $dom,
    'filename' => 'sitemapmain.xml',
  ));
  
  wallysitemap_generate_xmlfile($xmlfiles_to_generate, $settings['wallysitemap_path']);
}

function _wallysitemap_generate_sitemap_xml_for_menu($dom, &$urlset, $site_infos, $settings, $menu_infos) {
  $ref_base_url = $settings['wallysitemap_base_url'];
  $processed_dest_tids = &_wallysitemap_get_static_processed_dest_tids();
  foreach ($menu_infos as $menu_item) {
    if (is_array($menu_item['below'])) {
      _wallysitemap_generate_sitemap_xml_for_menu($dom, $urlset, $site_infos, $settings, $menu_item['below']);
    }
    
    $menu_item_aliases = wallytoolbox_get_all_aliases($menu_item['link']['link_path']);
    if (array_intersect($menu_item_aliases, $settings['wallysitemap_blacklist'])) {
      continue;
    }

    if ($menu_item['link']['router_path'] == 'taxonomy/term/%') {
      $expl_path = explode('/', $menu_item['link']['link_path']);
      $tid = $expl_path[2];
      $term = taxonomy_get_term($tid);
      $dest_vid = variable_get('wallymport_destinationpath', 2);
      if ($term->vid == $dest_vid || in_array($tid, $processed_dest_tids)) {
        continue;
      } else {
        $processed_dest_tids[] = $tid;
      }
    }
    
    if ($menu_item['link']['link_path'] == '<front>') {
      $menu_item['link']['link_path'] = '';
    }

    $url = $urlset->appendChild($dom->createElement('url'));
    $nice_path = wallytoolbox_get_main_alias($menu_item['link']['link_path']);
    $url->appendChild($dom->createElement('loc', $ref_base_url.'/'.$nice_path));
    // lastmod not handled yet
    // changefreq not handled yet
    // priority not handled yet
  }
}

function _wallysitemap_generate_sitemap_xml_for_taxonomy($dom, &$urlset, $site_infos, $settings, $terms) {
  $ref_base_url = $settings['wallysitemap_base_url'];
  $processed_dest_tids = &_wallysitemap_get_static_processed_dest_tids();
  $terms_settings = $settings['wallysitemap_dests_params'] ? $settings['wallysitemap_dests_params'] : array();
  foreach ($terms as $term) {
    $dest_term_aliases = wallytoolbox_get_all_aliases('taxonomy/term/'.$term->tid);
    if (array_intersect($dest_term_aliases, $settings['wallysitemap_blacklist'])) {
      continue;
    }
    
    $term_settings = $terms_settings[$term->tid] ?
      $terms_settings[$term->tid] :
      array(
        'check' => 1,
        'weight' => 0,
        'freq' => 'daily',
      );
    if (!$term_settings['check'] || in_array($term->tid, $processed_dest_tids)) {
      continue;
    } else {
      $processed_dest_tids[] = $term->tid;
    }
    $url = $urlset->appendChild($dom->createElement('url'));
    $nice_path = wallytoolbox_get_main_alias('taxonomy/term/'.$term->tid);
    $url->appendChild($dom->createElement('loc', $ref_base_url.'/'.$nice_path));
    // lastmod not handled yet
    $url->appendChild($dom->createElement('changefreq', $term_settings['freq']));
    $url->appendChild($dom->createElement('priority', $term_settings['weight']));
  }
}

function &_wallysitemap_get_static_processed_dest_tids() {
  static $processed_dest_tids = array();
  return $processed_dest_tids;
}
