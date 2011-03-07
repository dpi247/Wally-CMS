<?php
  $path = base_path().path_to_theme();
?>


  
	<div id="top_content" class="grid_12">
		<?php if($content['top_content']) print $content['top_content'];  ?> 
	</div>
  
	<div id="colonne-article" class="grid_6">
		<?php if($content['colarticle']) print $content['colarticle'];  ?>
	</div>

	<div id="colonne-milieu" class="grid_2">
		<?php if($content['colmiddle']) print $content['colmiddle'];  ?>
	</div>

	<div id="col-02" class="grid_4">
		<?php if($content['coltier']) print $content['coltier'];  ?> 
	</div>
 
	<div id="bottom_content" class="grid_12">
		<?php if($content['bottom_content']) print $content['bottom_content'];  ?>
	</div>
