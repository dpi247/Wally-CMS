<?php
// $Id: block.tpl.php,v 1.3 2007/08/07 08:39:36 goba Exp $
?>
<?php if (!empty($block->content)): ?>
  <div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="clear-block block block-<?php print $block->module ?>">
  <div class="content"><?php print $block->content ?></div>
  </div>
<?php endif;?>
