<?php
// $Id: poll-results.tpl.php,v 1.2 2007/08/07 08:39:35 goba Exp $
// This template is used in node page and all polls page contexts

/**
 * @file poll-results-block.tpl.php
 * Display the poll results in a block.
 *
 * Variables available:
 * - $title: The title of the poll.
 * - $results: The results of the poll.
 * - $votes: The total results in the poll.
 * - $links: Links in the poll.
 * - $nid: The nid of the poll
 * - $cancel_form: A form to cancel the user's vote, if allowed.
 * - $raw_links: The raw array of links.
 * - $vote: The choice number of the current user's vote.
 *
 * @see template_preprocess_poll_results()
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
  <div class="wrap_total">
    <?php print $results; ?>
  </div>    
  <div class="total">
    <?php print t('Total votes: @votes', array('@votes' => $votes)); ?>
  </div>
  <?php if (!empty($cancel_form)): ?>
    <?php print $cancel_form; ?>
  <?php endif; ?>  
</div>