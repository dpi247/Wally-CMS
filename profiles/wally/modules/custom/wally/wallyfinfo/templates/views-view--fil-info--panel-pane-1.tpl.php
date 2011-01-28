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

$results = $view->result;
$pager = $view->pager;
?>

<div class="packfil">
  <div id="colinfos">
    <h2>Le fil info</h2>
    
    <div class="filhead">
      <a>Voir toutes les infos</a>
        <div class="centrercat">
          <div class="carousel" id="car2">
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
    
    <div>
    <?php
    if($view->total_rows > $pager['items_per_page']) { 
      for($i=0; $i<5; $i++) {
        if($i==$pager['current_page']) {
    ?>
      <span><?php print $i; ?></span>
    <?php
        } else {
    ?>
      <span><a href="?page=<?php print $i; ?>"><?php print $i; ?></a></span>
    <?php
        }
      }
    }
    ?>
    </div>
  </div>
</div>
