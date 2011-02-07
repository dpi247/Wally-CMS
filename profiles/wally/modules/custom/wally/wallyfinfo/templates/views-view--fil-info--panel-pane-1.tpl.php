<?php
// $Id: views-view--fil-info--panel-pane-1.tpl.php,v 1.0 2011/01/12 ODM $
/**
 * @file views-view--fil-info--panel-pane-1.tpl.php
 * Default simple view template to display a list of rows.
 * To be used in the 'Related column' panel.
 * 
 * Variables available:
 * - $view: The view object
 *
 * @ingroup views_templates
 */
$all_voc = taxonomy_get_vocabularies();
foreach ($all_voc as $voc) {
  if ($voc->name == 'Destination Path') {
    $available_dests = taxonomy_get_tree($voc->vid, 0, -1, 1);
    break;
  }
}
dsm(url("view/fil_info/panel_pane_2/4/0"));
$pager = $view->pager;
?>

<div class="packfil">
  <div id="colinfos">
    <h2>Le fil info</h2>
    
    <div class="filhead">
        <div class="centrercat">
          <div class="carousel" id="carCat">
            <div id="filcat">
              <ul>
              </ul>
            </div>
          </div>
        </div>
      <p class="jr"><?php print date('d M Y'); ?></p>
    </div>

	<?php foreach ($available_dests as $available_dest) { ?>
    <div id="car<?php print strtolower($available_dest->name); ?>" class="carousel">
      <ul id="liste<?php print strtolower($available_dest->name); ?>" class="hideme">
      </ul>
    </div>
    <?php } ?>
    
    <div class="pager">
    <?php
    if($view->total_rows > $pager['items_per_page']) { 
      for($i=1; $i<6; $i++) {
    ?>
      <span id="page<?php print $i; ?>"><?php print $i; ?></span>
    <?php
      }
    }
    ?>
    </div>
  </div>
</div>
