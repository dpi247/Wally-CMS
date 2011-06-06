<?php
  $path = base_path().path_to_theme();
?>

	<div id="top_content" class="grid_12">
		<?php if($content['top_content']) print $content['top_content'];  ?> 
	</div>
  
	<div id="col-01" class="grid_8">
		
		<div id="colone-above-8" class="grid_8 alpha omega">
			<?php if($content['colone_above_8']) print $content['colone_above_8'];  ?>
		</div>
		
		<div id="colone-above-4-1" class="grid_4 alpha">
			<?php if($content['colone_above_4_1']) print $content['colone_above_4_1'];  ?>
		</div>
		
		<div id="colone-above-4-2" class="grid_4 omega">
			<?php if($content['colone_above_4_2']) print $content['colone_above_4_2'];  ?>
		</div>
		
		<div id="colone-middle-8" class="grid_8 alpha omega">
			<?php if($content['colone_middle_8']) print $content['colone_middle_8'];  ?>
		</div>
		
		<div id="colone-middle-4-1" class="grid_4 alpha">
			<?php if($content['colone_middle_4_1']) print $content['colone_middle_4_1'];  ?>
		</div>
		
		<div id="colone-middle-4-2" class="grid_4 omega">
			<?php if($content['colone_middle_4_2']) print $content['colone_middle_4_2'];  ?>
		</div>
		
		<div id="colone-bottom-8" class="grid_8 alpha omega">
			<?php if($content['colone_bottom_8']) print $content['colone_bottom_8'];  ?>
		</div>              
	</div>

	<div id="col-02" class="grid_4">
		<?php if($content['coltwo']) print $content['coltwo'];  ?>  
	</div>

	<div id="soiree-tele" class="grid_12">
		<?php if($content['coltv']) print $content['coltv'];  ?>  
	</div>

	<div id="colone-bottom-8-1" class="grid_8">
		<?php if($content['colone_bottom_8_1']) print $content['colone_bottom_8_1'];  ?>
	</div>

	<div id="colone-middle-bottom-4-2" class="grid_4 alpha">
		<?php if($content['colone_middle_bottom_4_2']) print $content['colone_middle_bottom_4_2'];  ?>
	</div>  

	<div id="bottom_content" class="grid_12">
		<?php if($content['bottom_content']) print $content['bottom_content'];  ?>
	</div>
