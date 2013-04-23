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
$pane_config = $pane->configuration;
$available_dests = array();
if (isset($pane_config['available_dests'][0]) && $pane_config['available_dests'][0] == 'contextual') {
  $orig_url = $_GET['q'];
  $expl_url = explode('/', $orig_url);

  switch ($expl_url[0]) {
    case 'node':
      if ($expl_url[1]) {
        $node = node_load($expl_url[1]);
        $node_dest = $node->field_destinations;
        $available_dests[] = taxonomy_get_term($node_dest[0]['tid']);
      }
      break;
    case 'taxonomy':
      if ($expl_url[2]) {
        $tids = $expl_url[2];
        $expl_tids = preg_split('(\+|\,|\ )', $tids);
        $available_dests[] = taxonomy_get_term($expl_tids[0]);
      }
      break;
    default:
  }
} else {
  $available_dests = $pane_config['available_dests'];
}
drupal_add_js(drupal_get_path('module', 'wallyfinfo').'/scripts/jquery.jcarousel.js');
drupal_add_js(_wallyfinfo_slidecategoryjs_box($available_dests), 'inline');
drupal_add_css(drupal_get_path('module', 'wallyfinfo').'/css/packfilinfoscarousel.css');
?>

<div class='box'>
  <?php
    if (sizeof($available_dests)>0) {
  ?>
      <div class="packfil">
        <div id="colinfos">
          <h2><?php print $pane_config['override_title'] ? check_plain($pane_config['override_title_text']) : t('Le fil info'); ?></h2>
          <?php print $content; ?>
        </div>
      </div>
  <?php
    } else {
      drupal_set_message('No destinations available', 'error');
      print '';
    }
  ?>
</div>
