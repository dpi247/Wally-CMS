<?php
//dsm($variables);
?>
<div id="bloc-tvnews" class="brique color-02 text-list">
<h2>TV News</h2>
<?php 
if ($rows): ?>
	<?php print $rows; ?>
  <?php elseif ($empty): ?>
      <?php print $empty; ?>
  <?php endif; ?>
</ul>
</div>  