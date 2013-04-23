<?php

if ($settings['override_title_text']) {
  $title = t($settings['override_title_text']);
} else {
  $title = '';
}
?>
<div class="remotehtml brique">
<h2><?php print check_plain($title); ?></h2>
<div class="content">
<?php print $htmlblock;
?>
</div>
</div>
