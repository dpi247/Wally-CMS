<?php

/**
 * This hook is called when a XML sitemap DOM Document is about to be saved.
 * 
 * The 'type' parameter specifies in which XML creation context we are :
 *   - 'main' : DOM Document of the main sitemap
 *   - 'news' : DOM Document of a news sitemap
 *   - 'consolidate' : DOM Document of the main sitemap when being consolidated by the news sitemap module
 * 
 * @param object &$dom
 *   The DOM Document
 * @param string $type
 *   The creation context : 'main', 'node' or 'consolidate'
 */
function hook_wallysitemap_dom_alter(&$dom, $type) {
  switch ($type) {
    case 'main':
      // Add a custom URL ('/contact')
      $dom_urlset = $dom->getElementsByTagName('urlset')->item(0);
      $url = $dom_urlset->appendChild($dom->createElement('url'));
      
      $global_sitemap_settings = wally_variable_get('wallysitemap_sitemaps_settings', array());
      $ref_base_url = isset($global_sitemap_settings['wallysitemap_base_url']) ? $global_sitemap_settings['wallysitemap_base_url'] : $site_infos['base_url'];
      
      $url->appendChild($dom->createElement('loc', $ref_base_url.'/contact'));
      $url->appendChild($dom->createElement('lastmod', date('Y-m-d\TH:i:sP')));
      $url->appendChild($dom->createElement('changefreq', 'weekly'));
      $url->appendChild($dom->createElement('priority', '0.1'));
      break;
      
    case 'news':
    case 'consolidate':
    default :
      break;
  }
}

/**
 * This function is not really a hook, it is more like a simplified hook.
 * During the XML generation process, each time an url element is processed,
 * the existence of this function is checked, if it exists it is called.
 * Doing so, one module is allowed to alter a DOM url through this unique function.
 * The alteration can thus only be called only once.
 * 
 * This process has been set up to limit its impact on performances.
 * 
 * The 'type' parameter specifies in which XML creation context we are :
 *   - 'news' : URL element of a news
 *   - 'menu' : URL element of a menu item
 *   - 'destination' : URL element of a destination page
 * 
 * @param object $dom
 *   The complete DOM Document
 * @param object &$url
 *   The DOM url element
 * @param string $type
 *   The URL type : 'news', 'menu' or 'destination'
 * @param array $args
 *   Additionnal parameters, such a the node or the menu element. Depends on the URL type.
 */
function wallysitemap_url_alter($dom, &$url, $type, $args = array()) {
  switch ($type) {
    case 'node':
      $main_sitemap_settings = $args['main_settings'];
      $news_sitemap_settings = $args['news_settings'];
      $node = $args['node'];
      
      if (!isset($node->field_destinations) || empty($node->field_destinations)) {
        // Do something on the url element of a node who hasn't any destinations
      }
      
      if ($news_sitemap_settings['wallynewssitemap_add_images']) {
        $url_dom_image = $url->getElementsByTagName('image:image');
        // Do something on an image element
      }
      
      break;
      
    case 'menu':
      $settings = $args['settings'];
      $menu_item = $args['menu_item'];
      break;
      
    case 'destination':
      $settings = $args['settings'];
      $destination_term = $args['term'];
      break;
      
    default:
      break;
  }
}
