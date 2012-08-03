<div class="bloc-01" id="comment">
  <h2>Les + commentés</h2>
  <div class="inner-bloc">
    <ul> 
      <?php foreach ($rows as $id => $row):?>
        <?php print $row; ?>
      <?php endforeach; ?>
    </ul>
  </div>    
</div>