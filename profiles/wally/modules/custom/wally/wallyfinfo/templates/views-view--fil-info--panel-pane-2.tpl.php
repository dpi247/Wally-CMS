<?php
// $Id: views-view--fil-info--panel-pane-2.tpl.php,v 1.0 2011/01/12 ODM $
/**
 * @file views-view--fil-info--panel-pane-2.tpl.php
 * Default simple view template to display a list of rows.
 * 
 * Variables available:
 * - $view: The view object
 *
 * @ingroup views_templates
 */
drupal_add_js(_wallyfinfo_getjsforfilinfotheme(), 'inline');
drupal_add_js(drupal_get_path('module', 'wallyfinfo').'/scripts/carousel.js');
drupal_add_css(drupal_get_path('module', 'wallyfinfo').'/css/pageinfos.css');
$results = $view->result;
$pager = $view->pager;
$nodes = array();
$mainstories = array();
foreach ($results as $result) {
  $temp_node = node_load($result->nid);
  wallycontenttypes_packagepopulate($temp_node, '');
  $nodes[] = $temp_node;
}
?>
<h2>Le fil info</h2>

<script>
$("#catmenu").jCarouselLite({
  btnNext: "#nextinfos",
  btnPrev: "#previnfos",
  auto: 0,
  vertical: false,
  visible: 6,
  circular: false,
  scroll: 1
});
</script>

<div class="ttesinfos">
  <div id="infobel">
    <ul id="listebelgique" class="infoshideme">
      <?php foreach($nodes as $node) { ?>
      <li>
        <div class="belgique">
          <h2 class="title"><?php print check_plain($node->title); ?></h2>
          <p class="date"><?php print date('d M Y, H:m', $node->created); ?></p> 
          <?php print $node->field_mainstory_nodes[0]->field_textbody[0]['value']; ?>
        </div>
      </li>
      <?php } ?>
    </ul>
  </div>
</div>
