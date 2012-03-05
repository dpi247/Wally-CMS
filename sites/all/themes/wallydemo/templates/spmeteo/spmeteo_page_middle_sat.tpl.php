<?php
	$medias_folder = $my_data['medias_folder']; 
	$path = $_SERVER['DOCUMENT_ROOT']."/".$medias_folder."/"; 
	drupal_add_js(drupal_get_path('theme', 'wallydemo') . '/scripts/meteo.js', 'theme');
?>
<div id="satellite">
	<h2><span>Satellite</span></h2>
	<div class="anim_carte">
		<ul>
		<?php 
			$today =  date("d");
			if(file_exists($path.'sat00.jpg')){
				$date00 = date("d",filemtime($path.'sat00.jpg'));
				if($today==$date00){ ?>
				<li><img src="<?php echo "/".$medias_folder."/"; ?>sat00.jpg" width="163" height="123" alt="carte satellite" /></li>
				<?php }
			}
			if(file_exists($path.'sat06.jpg')){
				$date06 = date("d",filemtime($path.'sat06.jpg'));
				if($today==$date06){ ?>
					<li><img src="<?php echo "/".$medias_folder."/"; ?>sat06.jpg" width="163" height="123" alt="carte satellite" /></li><?php 
				}
			}
			if(file_exists($path.'sat12.jpg')){
				$date12 = date("d",filemtime($path.'sat12.jpg'));
				if($today==$date12) { ?>
					<li><img src="<?php echo "/".$medias_folder."/"; ?>sat12.jpg" width="163" height="123" alt="carte satellite" /></li><?php 
				}
			}
			if(file_exists($path.'sat18.jpg')){
				$date18 = date("d",filemtime($path.'sat18.jpg'));
				if($today==$date18){ ?>
					<li><img src="<?php echo "/".$medias_folder."/"; ?>sat18.jpg" width="163" height="123" alt="carte satellite" /></li><?php 
				}
			} 
		?>
		</ul>
	</div>
</div>
<?php 
//unset variables
unset($my_data);
?>