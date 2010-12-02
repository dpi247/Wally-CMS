<?php
  $path = base_path().path_to_theme();
?>

      <div id="colonne-article" class="grid_6">
        <?php if($content['colarticle']) print $content['colarticle'];  ?>
      </div>
      <?php if($content['colrelated']) print $content['colrelated'];  ?>

			<div id="colonne-milieu" class="grid_2">
        <?php if($content['colmilieu']) print $content['colmilieu'];  ?>
			</div>

			<div id="colonne-droite" class="grid_4">

        <?php if($content['coldroite']) print $content['coldroite'];  ?>
				
			</div>
