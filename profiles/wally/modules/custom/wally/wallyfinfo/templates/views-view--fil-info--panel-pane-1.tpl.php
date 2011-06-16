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
$available_dests = taxonomy_get_tree(variable_get('wallymport_destinationpath', '2'));
?>
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
for($i=1; $i<6; $i++) {
?>
  <span id="page<?php print $i; ?>"><?php print $i; ?></span>
<?php
}
?>
</div>
