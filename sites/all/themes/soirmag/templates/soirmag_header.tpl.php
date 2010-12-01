<?php
    $path = base_path().path_to_theme();
        
    $ad1 = theme("soirmag_adds", $node, 'ad_01.gif');
    $ad2 = theme("soirmag_adds", $node, 'ad_02.gif');
    $ad3 = theme("soirmag_adds", $node, 'ad_03.gif');

?>
	<div id="header" class="clearfix">

			<div class="container_12 clearfix">
				<div id="jfl-tools" class="grid_6 prefix_6">
					<ul>
						<li><a href="#colonne-article">Aller au contenu</a> &middot;</li>
						<li><a class="fluidify" href="#">mise en page fluide</a></li>
					</ul>
					<a class="slide" href="#jfl-tools">outils</a>
				</div>
				
				<a id="logo" href="javascript:void(0)">
					<img src="<?php print $path; ?>/mediastore/elements/logo.png" width="257" height="69" alt="Soirmag.be" />
					<span>Rage Of Lunacy edition</span>
				</a>
				<div class="adimage">
          <?php print $ad3; ?>
				</div>
				<div class="adimage">
          <?php print $ad2; ?>
				</div>
				<div class="adimage">
          <?php print $ad1; ?>
				</div>

			</div>
		</div>
