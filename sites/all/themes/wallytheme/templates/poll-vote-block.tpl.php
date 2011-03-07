<?php
// $Id: poll-vote.tpl.php,v 1.2 2007/08/07 08:39:35 goba Exp $

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
?>
<div class="poll brique">
  <h2><a href="/poll">Sondage</a></h2>
      <?php if ($block): ?>
        <h3><?php print $title; ?></h3>
      <?php endif; ?>
      <?php print $choice; ?>
    <?php print $vote; ?>
  <?php // This is the 'rest' of the form, in case items have been added. ?>
  <?php print $rest ?>
  <ul>
    <li><a href="/poll">Tous les sondages</a></li>
  </ul>    
</div>
