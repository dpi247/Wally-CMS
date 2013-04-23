<?php
// $Id: views-view--fil-info--panel-pane-2.tpl.php,v 1.0 2011/01/12 ODM $
/**
 * @file views-view--fil-info--panel-pane-2.tpl.php
 * Default simple view template to display a list of rows.
 * To be used in the 'Main column' panel.
 * 
 * Variables available:
 * - $view: The view object
 *
 * @ingroup views_templates
 */
$available_dests = taxonomy_get_tree(variable_get('wallymport_destinationpath', 0));
$pager = $view->pager;
?>
<div class="categories">
  <div>
    <div id="catmenu" class="carousel"> 
      <ul>
      </ul>
    </div>
  </div>
</div>

<div class="pager_page">
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

<div class="ttesinfos">
  <?php foreach ($available_dests as $available_dest) { ?>
  <div id="car<?php print strtolower($available_dest->name); ?>_page" class="carousel">
    <ul id="liste<?php print strtolower($available_dest->name); ?>" class="hideme">
    </ul>
  </div>
  <?php } ?>
</div>
    
<div class="pager_page">
<?php
if($view->total_rows > $pager['items_per_page']) { 
  for($i=1; $i<6; $i++) {
?>
  <span id="page<?php print $i; ?>bis"><?php print $i; ?></span>
<?php
  }
}
?>
</div>
