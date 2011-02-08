<?php
// $Id: views-view--fil-info--panel-pane-3.tpl.php,v 1.0 2011/01/12 ODM $
/**
 * @file views-view--fil-info--panel-pane-3.tpl.php
 * Default simple view template to display a list of rows.
 * To be used in the 'Header' panel.
 * 
 * Variables available:
 * - $view: The view object
 *
 * @ingroup views_templates
 */
$results = $view->result;
?>

<div id="divthicker">
  <a id="today" href="#"><?php print t('Today').':'; ?></a>
  <div id="thicker" class="carousel">
    <ul>
      <?php foreach ($results as $result) { ?>
      <li>
        <div class="belgique">
          <a href="<?php print drupal_get_path_alias("/node/".$result->nid); ?>"><small><?php print date('H:m', check_plain($result->node_created)); ?></small><h2><?php print check_plain($result->node_title); ?></h2></a>
        </div>
      </li>
      <?php } ?>
    </ul>
  </div>
</div>
