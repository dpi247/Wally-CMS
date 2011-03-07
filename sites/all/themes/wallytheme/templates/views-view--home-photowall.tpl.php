<div id="the-wall" class="clearfix">
<?php 
if ($rows): ?>
	<?php print $rows; ?>
  <?php elseif ($empty): ?>
      <?php print $empty; ?>
  <?php endif; ?>
</div>