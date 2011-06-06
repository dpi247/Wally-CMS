<?php
// $Id: views-view--fil-info--panel-pane-2--li.tpl.php,v 1.0 2011/02/14 ODM $
/**
 * @file views-view--fil-info--panel-pane-2--1.tpl.php
 * Default simple view template to display a row.
 * 
 * Variables available:
 * - $destination: The contextual destination
 * - $view_result: One result to display
 *
 * @ingroup views_templates
 */
$node = node_load($view_result->nid);
wallycontenttypes_packagepopulate($node, null);
$text_body = $node->field_mainstory_nodes[0]->field_textbody[0]['value'];
?>
<li>
  <div class="<?php print strtolower($destination->name); ?>_page">
    <h2 class="title"><?php print check_plain($node->title); ?></h2>
    <p class="date"><?php print date('d M Y, H:m', $node->created); ?></p> 
    <?php print (strlen($text_body) > 250) ? substr($text_body, 0, 250).'...' : $text_body; ?>
  </div>
</li>
