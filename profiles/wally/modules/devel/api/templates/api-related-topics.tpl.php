<?php

/**
 * @file
 * Displays related topics for an API object page.
 *
 * Available variables:
 * - $topics: Array containing topic descriptions keyed on topic.
 *
 * @ingroup themeable
 */
?>
<dl class="api-related-topics">
<?php foreach ($topics as $topic => $description) { ?>
  <dt><?php print $topic ?></dt>
  <dd><?php print $description ?></dd>
<?php } ?>
</dl>
