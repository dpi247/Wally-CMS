<?php

/**
 * @see http://support.google.com/webmasters/bin/answer.py?hl=en&answer=183668&topic=8476&ctx=topic
 */
function _wallysitemap_generate_sitemap_xml($site_infos, $settings) {
  module_load_include('inc', 'wallytoolbox', 'includes/wallytoolbox.helpers');
  
  $namespaces = wallysitemap_get_namespaces();
  $dom = new DOMDocument('1.0', 'UTF-8');
  $urlset = $dom->appendChild($dom->createElementNS($namespaces['sitemap'], 'urlset'));

  $menus = $settings['wallysitemap_handled_menus'];
  foreach ($menus as $menu_name) {
    $menu_infos = menu_tree_all_data($menu_name, NULL);
    _wallysitemap_generate_sitemap_xml_for_menu($dom, $urlset, $site_infos, $settings, $menu_infos);
  }
  
  $dest_terms = taxonomy_get_tree(variable_get('wallymport_destinationpath', 2));
  _wallysitemap_generate_sitemap_xml_for_taxonomy($dom, $urlset, $site_infos, $settings, $dest_terms);
  
  wallysitemap_generate_xmlfile($dom, $namespaces, $site_infos, $settings['wallysitemap_path'], 'sitemapmain.xml');
}

function _wallysitemap_generate_sitemap_xml_for_menu($dom, &$urlset, $site_infos, $settings, $menu_infos) {
  $processed_dest_tids = &_wallysitemap_get_static_processed_dest_tids();
  foreach ($menu_infos as $menu_item) {
    if (is_array($menu_item['below'])) {
      _wallysitemap_generate_sitemap_xml_for_menu($dom, $urlset, $site_infos, $settings, $menu_item['below']);
    }

    if ($menu_item['link']['router_path'] == 'taxonomy/term/%') {
      $expl_path = explode('/', $menu_item['link']['link_path']);
      $tid = $expl_path[2];
      if (in_array($tid, $processed_dest_tids)) {
        continue;
      } else {
        $processed_dest_tids[] = $tid;
      }
    }

    $url = $urlset->appendChild($dom->createElement('url'));
    $nice_path = wallytoolbox_get_main_alias($menu_item['link']['link_path']);
    $url->appendChild($dom->createElement('loc', $site_infos['base_url'].'/'.$nice_path));
  }
}

function _wallysitemap_generate_sitemap_xml_for_taxonomy($dom, &$urlset, $site_infos, $settings, $terms) {
  $processed_dest_tids = &_wallysitemap_get_static_processed_dest_tids();
  foreach ($terms as $term) {
    if (in_array($term->tid, $processed_dest_tids)) {
      continue;
    } else {
      $processed_dest_tids[] = $term->tid;
    }
    $url = $urlset->appendChild($dom->createElement('url'));
    $nice_path = wallytoolbox_get_main_alias('taxonomy/term/'.$term->tid);
    $url->appendChild($dom->createElement('loc', $site_infos['base_url'].'/'.$nice_path));
  }
}

function &_wallysitemap_get_static_processed_dest_tids() {
  static $processed_dest_tids = array();
  return $processed_dest_tids;
}
