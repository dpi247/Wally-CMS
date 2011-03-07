<?php
// $Id: poll-vote.tpl.php,v 1.2 2007/08/07 08:39:35 goba Exp $
// This template is used in node page and all polls page contexts

/**
 * @file poll-vote.tpl.php
 * Voting form for a poll.
 *
 * - $choice: The radio buttons for the choices in the poll.
 * - $title: The title of the poll.
 * - $block: True if this is being displayed as a block.
 * - $vote: The vote button
 * - $rest: Anything else in the form that may have been added via
 *   form_alter hooks.
 *
 * @see template_preprocess_poll_vote()
 */


if(!$title){
  $node_id = $variables['nid'];
  $node = node_load($node_id);
  $node_path = drupal_get_path_alias("node/".$node->nid);
  $title = $node->title;
  $title_html = "<h1>".$title."</h1>";
} else {
  $node_path = drupal_get_path_alias("node/".$variables['nid']);
  $title_html = "<h3><a href=\"/".$node_path."\">".$title."</a></h3>";
}

?>
<div class="poll brique">
  <?php print $title_html ; ?>
  <?php print $choice; ?>
  <?php print $vote; ?>
  <?php // This is the 'rest' of the form, in case items have been added. ?>
  <?php print $rest ?>
</div>
