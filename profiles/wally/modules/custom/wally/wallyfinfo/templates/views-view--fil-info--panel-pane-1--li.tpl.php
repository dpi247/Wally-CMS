<?php
// $Id: views-view--fil-info--panel-pane-1--li.tpl.php,v 1.0 2011/02/14 ODM $
/**
 * @file views-view--fil-info--panel-pane-1--1.tpl.php
 * Default simple view template to display a row.
 * 
 * Variables available:
 * - $destination: The contextual destination
 * - $view_result: One result to display
 *
 * @ingroup views_templates
 */
?>
<li>
  <div class="<?php print strtolower($destination->name); ?>">
    <a href="<?php print drupal_get_path_alias('/node/'.$view_result->nid); ?>">
      <small><?php print date('H:m', check_plain($view_result->node_created)); ?></small>
      <h2><?php print check_plain($view_result->node_title); ?></h2>
    </a>
  </div>
</li>
