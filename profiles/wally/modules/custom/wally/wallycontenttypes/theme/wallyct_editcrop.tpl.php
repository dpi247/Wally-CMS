<?php
	drupal_add_css(drupal_get_path('module', 'wallycontenttypes') . '/css/editcrop/editcrop.css'); 
?>
<div class="crop_wrap">
	<form class="crops" action="" method="post">
    	<label for="edit-crop">Move and resize crops : <span id="crop-current-name"><?php print t("Choose a preset to edit.") ?></span>
	    </label>
	    <div id="cropimage"><img src="<?php print base_path().$filepath ?>" id="cropbox" /></div>
	  	<div class="crops">
	  		<?php foreach ($defaultvalues as $presetname => $defaultvalue) { ?>
	      		<input type="hidden" id="<?php print $presetname.'_serialCoord' ?>" name="<?php print $presetname.'_serialCoord' ?>" value="<?php print implode($defaultvalue,',')?>"/>
	      		<input type="button" id="<?php print $presetname ?>" class="form-submit" value="<?php print t($presetname)?>">
	    	<?php } ?>
	 
	 	</div>
	 	<div class="crop_submit buttons">
	  		<input type="submit" class="form-submit" value="<?php print t('save')?>" id="edit-submit" name="op">
		</div>  
   	</form>
</div>
