<?php 

$dest_url = check_url(drupal_get_path_alias("taxonomy/term/47"));
?>
<div class="buzz exergue clearfix">
  <h1>
    <a href="/<?php print($dest_url); ?>" title="aller vers la section buzz" >Buzz</a>
  </h1> 
<?php foreach ($rows as $id => $row):?>
    <?php print $row; ?>
<?php endforeach; ?>
</div>
