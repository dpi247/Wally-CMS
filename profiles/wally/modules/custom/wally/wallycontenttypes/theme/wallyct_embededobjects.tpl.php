<?php
drupal_add_js(drupal_get_path('theme', 'wallynews').'/scripts/steph/test-jquery.prettyPhoto.js');
drupal_add_js(drupal_get_path('theme', 'wallynews').'/steph/rotator.js');

?>


<script type="text/javascript" charset="utf-8">
		$(document).ready(function(){
			$("a[rel^='prettyPhoto']").prettyPhoto();
		});
	</script>
	<?php 

	?>
<?php  if (count($embededobjects)): ?>
  <span class="embedobjectsblock">
    <div class="photos">
    <ul>
 
  <?php foreach ($embededobjects as $embededobject) :?>  

        
      <li>
        
        <?php if($embededobject->type=="wally_videoobject"):?>
        <?php // dsm($embededobject->field_photofile[0]);?>
          <a title="<?php print $embededobject->title?>" rel="prettyPhoto[pp_gal]" href="<?php print $embededobject->field_video3rdparty[0]['embed']; ?>"> 
           <img src=  />
                    </a>
        <?php endif;?>
        <!-- 
        <?php if($embededobject->type=="wally_photoobject"):?>
         <a title="<?php print $embededobject->title?>" rel="prettyPhoto[pp_gal]" href="/<?php print $embededobject->field_photofile[0]['filepath']; ?>"> 
             <?php print  theme('imagecache','slider_preset',$embededobject->field_photofile[0]['filepath'])?>
          </a>
       <?php endif;?>
         -->
   
      </li>
  <?php endforeach;?>
    </ul>
    </div>
  </span>
<?php endif;?>
