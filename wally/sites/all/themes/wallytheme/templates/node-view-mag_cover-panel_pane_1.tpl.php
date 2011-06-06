<?php 

$coverpath = $node->field_smcover[0]['filepath'];
$coverimgcache = imagecache_create_url('sm_cover', $coverpath);
$titles = $node->field_smtitles;

$pagepath = drupal_get_path_alias("node/".$node->nid);

?>

					<div id="cette-semaine" class="brique">
						<h2>Cette semaine dans Le Soir Magazine</h2>

						<h3><?php echo check_plain($node->title);?></h3>
						<a class="visuel" href="<?php echo $pagepath;?>">
							<img src="<?php echo $coverimgcache; ?>" width="117" height="165" alt="*" />
							<span>&nbsp;</span>
						</a>
						<ul>
<?php $cpt = 0;
	while (isset($titles[$cpt])) {
		echo "<li><a href=\"".$pagepath."\">".check_plain($titles[$cpt]['value'])."</a></li>\n";
		$cpt++;
	}
?>						</ul>
	<!--					<a class="button t-moyen align-01" href="javascript:void(0)">S'abonner</a>-->
					</div>

