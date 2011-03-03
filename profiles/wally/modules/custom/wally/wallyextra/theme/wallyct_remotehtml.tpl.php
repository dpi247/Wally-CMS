<?php

if ($settings['override_title_text']) {
  $title = t($settings['override_title_text']);
} else {
  $title = t('');
}
?>
<div class="remotehtml">
<h2><? print check_plain($title); ?></h2>
<div class="content">
<?php print $htmlblock;
?>
</div>
</div>
