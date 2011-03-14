<?php
drupal_add_js(drupal_get_path('theme', 'wallynews').'/scripts/rotator.js');
?>

<?php  if (count($embededobjects)>0): ?>
  <span class="embedobjectsblock">
    <div class="photos">
      <ul>
        <?php foreach ($embededobjects as $embededobject) :
          $type = explode('_', $embededobject->type);
        ?>  
        <li>    
          <?php print theme('wallyct_'.$type[1], $embededobject); dsm($type); ?>
        </li>
        <?php endforeach;?>
      </ul>
    </div>
  </span>
<?php endif;?>
