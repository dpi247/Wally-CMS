<?php
//dsm($variables);
?>
<div id="actu-stars" class="list-it brique color-02">
<h2>Actu Stars</h2>
<?php 
if ($rows): ?>
						<ul>
      <?php print $rows; ?>
    </ul>
  <?php elseif ($empty): ?>
      <?php print $empty; ?>
  <?php endif; ?>
</div>



