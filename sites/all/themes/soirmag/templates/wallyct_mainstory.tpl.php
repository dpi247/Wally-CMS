<?php

    // FB comment should be exported to a module, with a ADMIN FORM to get the API ID.
    // this module will also implément a theming function to display comment 
    // form (additionnal options - Width / number of posts / ... 
    // 
    
    $path = base_path().path_to_theme();

    $nid = $mainstory->nid; 
    $textarette = $mainstory->field_textarette[0]["value"];
    $foretitle = $textarette." ".$mainstory->field_textforetitle[0]["value"];
    $title = $mainstory->title;
    $subtitle = $mainstory->field_textsubtitle[0]["value"];
    $body = $mainstory->field_textbody[0]["value"];
    $photo_gallery = theme("wallyct_photoobject_slideshow",$node->field_embededobjects_nodes, $node);

    $ariane = theme("soirmag_fil_ariane", $node->field_embededobjects_nodes, $node);
    $share = theme("soirmag_share", $mainstory, $node);

    $date = date('l jS \of F Y h:i:s A', $mainstory->changed);
 
?>
	
				<div id="article" class="grid_6 alpha omega">
					<div class="fil-ariane clearfix">
						<p>Vous &ecirc;tes ici : <?php print $ariane ?></p>
						<a class="feed" href="javascript:void(0)" title="S'abonner au RSS de la section + SECTION +"><img src="<?php print $path; ?>/mediastore/elements/comment_rss_16.png" width="16" height="16" alt="RSS" /></a>
					</div>

					<div class="inner">
						<h1><?php print $title ?></h1>
            <small><?php print $date; ?></small>
            <?php print $photo_gallery; ?>
						<?php print $body ?>

            <fb:comments xid="<?php print $nid; ?>" numposts="10" width="440"></fb:comments>
            
						<div class="share clearfix">
            <?php print $share; ?>
						</div>
						
					</div>
				</div>
      
