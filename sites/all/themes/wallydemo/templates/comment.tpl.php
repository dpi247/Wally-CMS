<?php
// $Id: comment.tpl.php,v 1.4.2.1 2008/03/21 21:58:28 goba Exp $

/**
 * @file comment.tpl.php
 * Default theme implementation for comments.
 *
 * Available variables:
 * - $author: Comment author. Can be link or plain text.
 * - $content: Body of the post.
 * - $date: Date and time of posting.
 * - $links: Various operational links.
 * - $new: New comment marker.
 * - $picture: Authors picture.
 * - $signature: Authors signature.
 * - $status: Comment status. Possible values are:
 *   comment-unpublished, comment-published or comment-preview.
 * - $submitted: By line with date and time.
 * - $title: Linked title.
 *
 * These two variables are provided for context.
 * - $comment: Full comment object.
 * - $node: Node object the comments are attached to.
 *
 * @see template_preprocess_comment()
 * @see theme_comment()
 */
?>
<div class="comment<?php print ($comment->new) ? ' comment-new' : ''; print ' '. $status ?> clear-block">
  <?php print $picture ?>

  <?php if ($comment->new): ?>
    <span class="new"><?php print $new ?></span>
  <?php endif; ?>

  <h3><?php print $title ?></h3>

  <div class="submitted">
    <?php print $submitted ?>
  </div>

  <div class="content">
    <?php print $content ?>
    <?php if ($signature): ?>
    <div class="user-signature clear-block">
      <?php print $signature ?>
    </div>
    <?php endif; ?>
  </div>

  <?php print $links ?>
</div>

<div id="posts">
<div class="post">
  <p class="right"> <a title="Ce message est choquant, diffamatoire ou déplacé?: avertissez-nous!" onClick="window.open(this.href, 'Abus', 'height=350, width=550,toolbar=no, menubar=yes, location=no, resizable=yes, scrollbars=yes, status=no, top=50, left=50'); return false;" href="http://reactions.sudpresse.be/index.php?act=alert&amp;t=96933&amp;p=843994&amp;f=76"><img width="116" height="19" border="0" alt="Signaler un abus" src="/sites/all/themes/wallydemo/images/btn_signaler_un_abus.gif"></a> </p>
  <h4><?php print $title ?></h4>
  &nbsp; <span class="heuredatepost">- <?php print $date ?></span>
  <p class="commentaire"><?php print $content ?></p>
</div>
</div>