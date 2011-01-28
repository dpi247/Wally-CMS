<?php
// $Id: panels-pane.tpl.php,v 1.1.2.1 2009/10/13 21:38:52 merlinofchaos Exp $
/**
 * @file panels-pane.tpl.php
 * Main panel pane template
 *
 * Variables available:
 * - $pane->type: the content type inside this pane
 * - $pane->subtype: The subtype, if applicable. If a view it will be the
 *   view name; if a node it will be the nid, etc.
 * - $title: The title of the content
 * - $content: The actual content
 * - $links: Any links associated with the content
 * - $more: An optional 'more' link (destination only)
 * - $admin_links: Administrative links associated with the content
 * - $feeds: Any feed icons or associated with the content
 * - $display: The complete panels display object containing all kinds of
 *   data including the contexts and all of the other panes being displayed.
 */
$all_voc = taxonomy_get_vocabularies();
foreach ($all_voc as $voc) {
  if ($voc->name == 'Destination Path') {
    $available_dests = taxonomy_get_tree($voc->vid, 0, -1, 1);
    break;
  }
}
foreach ($available_dests as $available_dest) {
  $view_results[] = views_get_view_result('fil_info', 'panel_pane_1', $available_dest->tid);
}
dsm($view_results);
drupal_add_js(drupal_get_path('module', 'wallyfinfo').'/scripts/carousel.js');
drupal_add_js(drupal_get_path('module', 'wallyfinfo').'/scripts/jquery.jcarousel.js');
//drupal_add_js(drupal_get_path('module', 'wallyfinfo').'/scripts/page-slide-category.js');
//drupal_add_js(_wallyfinfo_slidecategoryjs($available_dests), 'inline');
//drupal_add_js(_wallyfinfo_wallyfinfocarouseljs($available_dests), 'inline');
drupal_add_js(_wallyfinfo_slidecategoryjs($available_dests), 'inline');
drupal_add_css(drupal_get_path('module', 'wallyfinfo').'/css/packfilinfoscarousel.css');
drupal_add_css(drupal_get_path('module', 'wallyfinfo').'/css/pageinfos.css');
drupal_add_css(drupal_get_path('module', 'wallyfinfo').'/css/thicker.css');
?>

<div class='box'>
  <?php print $content; ?>
</div>
