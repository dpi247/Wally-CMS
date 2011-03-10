<?php
drupal_add_js(drupal_get_path('theme', 'wallynews').'/scripts/steph/test-jquery.prettyPhoto.js');
drupal_add_js(drupal_get_path('theme', 'wallynews').'/steph/rotator.js');

?>


<script type="text/javascript" charset="utf-8">
  $(document).ready(function(){
    $("a[rel^='prettyPhoto']").prettyPhoto();
  });
</script>

<?php  if (count($embededobjects)): ?>
  <span class="embedobjectsblock">
    <div class="photos">
    <ul>
 
  <?php foreach ($embededobjects as $embededobject) :
          $type = explode('_', $embededobject->type);
  ?>  
      <li>    
        <?php print theme('wallyct_'.$type[1], $embededobject); ?>
      </li>
  <?php endforeach;?>
    </ul>
    </div>
  </span>
<?php endif;?>
